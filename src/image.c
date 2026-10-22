#समावेश "image.h"
#समावेश "utils.h"
#समावेश "blas.h"
#समावेश "cuda.h"
#समावेश <stdio.h>
#समावेश <math.h>

#घोषणा STB_IMAGE_IMPLEMENTATION
#समावेश "stb_image.h"
#घोषणा STB_IMAGE_WRITE_IMPLEMENTATION
#समावेश "stb_image_write.h"

पूर्णांक windows = 0;

भग्न colors[6][3] = { {1,0,1}, {0,0,1},{0,1,1},{0,1,0},{1,1,0},{1,0,0} };

भग्न get_color(पूर्णांक c, पूर्णांक x, पूर्णांक max)
{
    भग्न ratio = ((भग्न)x/max)*5;
    पूर्णांक i = न्यूनमान(ratio);
    पूर्णांक j = उच्चमान(ratio);
    ratio -= i;
    भग्न r = (1-ratio) * colors[i][c] + ratio*colors[j][c];
    //printf("%f\n", r);
    वापस r;
}

image mask_to_rgb(image mask)
{
    पूर्णांक n = mask.c;
    image im = make_image(mask.w, mask.h, 3);
    पूर्णांक i, j;
    क्रम(j = 0; j < n; ++j){
        पूर्णांक offset = j*123457 % n;
        भग्न red = get_color(2,offset,n);
        भग्न green = get_color(1,offset,n);
        भग्न blue = get_color(0,offset,n);
        क्रम(i = 0; i < im.w*im.h; ++i){
            im.तथ्य[i + 0*im.w*im.h] += mask.तथ्य[j*im.h*im.w + i]*red;
            im.तथ्य[i + 1*im.w*im.h] += mask.तथ्य[j*im.h*im.w + i]*green;
            im.तथ्य[i + 2*im.w*im.h] += mask.तथ्य[j*im.h*im.w + i]*blue;
        }
    }
    वापस im;
}

अटल भग्न get_pixel(image m, पूर्णांक x, पूर्णांक y, पूर्णांक c)
{
    निश्चित(x < m.w && y < m.h && c < m.c);
    वापस m.तथ्य[c*m.h*m.w + y*m.w + x];
}
अटल भग्न get_pixel_extend(image m, पूर्णांक x, पूर्णांक y, पूर्णांक c)
{
    अगर(x < 0 || x >= m.w || y < 0 || y >= m.h) वापस 0;
    /*
    if(x < 0) x = 0;
    if(x >= m.w) x = m.w-1;
    if(y < 0) y = 0;
    if(y >= m.h) y = m.h-1;
    */
    अगर(c < 0 || c >= m.c) वापस 0;
    वापस get_pixel(m, x, y, c);
}
अटल व्योम set_pixel(image m, पूर्णांक x, पूर्णांक y, पूर्णांक c, भग्न val)
{
    अगर (x < 0 || y < 0 || c < 0 || x >= m.w || y >= m.h || c >= m.c) वापस;
    निश्चित(x < m.w && y < m.h && c < m.c);
    m.तथ्य[c*m.h*m.w + y*m.w + x] = val;
}
अटल व्योम add_pixel(image m, पूर्णांक x, पूर्णांक y, पूर्णांक c, भग्न val)
{
    निश्चित(x < m.w && y < m.h && c < m.c);
    m.तथ्य[c*m.h*m.w + y*m.w + x] += val;
}

अटल भग्न bilinear_interpolate(image im, भग्न x, भग्न y, पूर्णांक c)
{
    पूर्णांक ix = (पूर्णांक) floorf(x);
    पूर्णांक iy = (पूर्णांक) floorf(y);

    भग्न dx = x - ix;
    भग्न dy = y - iy;

    भग्न val = (1-dy) * (1-dx) * get_pixel_extend(im, ix, iy, c) + 
        dy     * (1-dx) * get_pixel_extend(im, ix, iy+1, c) + 
        (1-dy) *   dx   * get_pixel_extend(im, ix+1, iy, c) +
        dy     *   dx   * get_pixel_extend(im, ix+1, iy+1, c);
    वापस val;
}


व्योम composite_image(image source, image dest, पूर्णांक dx, पूर्णांक dy)
{
    पूर्णांक x,y,k;
    क्रम(k = 0; k < source.c; ++k){
        क्रम(y = 0; y < source.h; ++y){
            क्रम(x = 0; x < source.w; ++x){
                भग्न val = get_pixel(source, x, y, k);
                भग्न val2 = get_pixel_extend(dest, dx+x, dy+y, k);
                set_pixel(dest, dx+x, dy+y, k, val * val2);
            }
        }
    }
}

image border_image(image a, पूर्णांक border)
{
    image b = make_image(a.w + 2*border, a.h + 2*border, a.c);
    पूर्णांक x,y,k;
    क्रम(k = 0; k < b.c; ++k){
        क्रम(y = 0; y < b.h; ++y){
            क्रम(x = 0; x < b.w; ++x){
                भग्न val = get_pixel_extend(a, x - border, y - border, k);
                अगर(x - border < 0 || x - border >= a.w || y - border < 0 || y - border >= a.h) val = 1;
                set_pixel(b, x, y, k, val);
            }
        }
    }
    वापस b;
}

image tile_images(image a, image b, पूर्णांक dx)
{
    अगर(a.w == 0) वापस copy_image(b);
    image c = make_image(a.w + b.w + dx, (a.h > b.h) ? a.h : b.h, (a.c > b.c) ? a.c : b.c);
    fill_cpu(c.w*c.h*c.c, 1, c.तथ्य, 1);
    embed_image(a, c, 0, 0); 
    composite_image(b, c, a.w + dx, 0);
    वापस c;
}

image get_label(image **characters, अक्षर *string, पूर्णांक आकार)
{
    आकार = आकार/10;
    अगर(आकार > 7) आकार = 7;
    image label = make_empty_image(0,0,0);
    जबतक(*string){
        image l = characters[आकार][(पूर्णांक)*string];
        image n = tile_images(label, l, -आकार - 1 + (आकार+1)/2);
        free_image(label);
        label = n;
        ++string;
    }
    image b = border_image(label, label.h*.25);
    free_image(label);
    वापस b;
}

व्योम draw_label(image a, पूर्णांक r, पूर्णांक c, image label, स्थिर भग्न *rgb)
{
    पूर्णांक w = label.w;
    पूर्णांक h = label.h;
    अगर (r - h >= 0) r = r - h;

    पूर्णांक i, j, k;
    क्रम(j = 0; j < h && j + r < a.h; ++j){
        क्रम(i = 0; i < w && i + c < a.w; ++i){
            क्रम(k = 0; k < label.c; ++k){
                भग्न val = get_pixel(label, i, j, k);
                set_pixel(a, i+c, j+r, k, rgb[k] * val);
            }
        }
    }
}

व्योम draw_box(image a, पूर्णांक x1, पूर्णांक y1, पूर्णांक x2, पूर्णांक y2, भग्न r, भग्न g, भग्न b)
{
    //normalize_image(a);
    पूर्णांक i;
    अगर(x1 < 0) x1 = 0;
    अगर(x1 >= a.w) x1 = a.w-1;
    अगर(x2 < 0) x2 = 0;
    अगर(x2 >= a.w) x2 = a.w-1;

    अगर(y1 < 0) y1 = 0;
    अगर(y1 >= a.h) y1 = a.h-1;
    अगर(y2 < 0) y2 = 0;
    अगर(y2 >= a.h) y2 = a.h-1;

    क्रम(i = x1; i <= x2; ++i){
        a.तथ्य[i + y1*a.w + 0*a.w*a.h] = r;
        a.तथ्य[i + y2*a.w + 0*a.w*a.h] = r;

        a.तथ्य[i + y1*a.w + 1*a.w*a.h] = g;
        a.तथ्य[i + y2*a.w + 1*a.w*a.h] = g;

        a.तथ्य[i + y1*a.w + 2*a.w*a.h] = b;
        a.तथ्य[i + y2*a.w + 2*a.w*a.h] = b;
    }
    क्रम(i = y1; i <= y2; ++i){
        a.तथ्य[x1 + i*a.w + 0*a.w*a.h] = r;
        a.तथ्य[x2 + i*a.w + 0*a.w*a.h] = r;

        a.तथ्य[x1 + i*a.w + 1*a.w*a.h] = g;
        a.तथ्य[x2 + i*a.w + 1*a.w*a.h] = g;

        a.तथ्य[x1 + i*a.w + 2*a.w*a.h] = b;
        a.तथ्य[x2 + i*a.w + 2*a.w*a.h] = b;
    }
}

व्योम draw_box_width(image a, पूर्णांक x1, पूर्णांक y1, पूर्णांक x2, पूर्णांक y2, पूर्णांक w, भग्न r, भग्न g, भग्न b)
{
    पूर्णांक i;
    क्रम(i = 0; i < w; ++i){
        draw_box(a, x1+i, y1+i, x2-i, y2-i, r, g, b);
    }
}

व्योम draw_bbox(image a, box bbox, पूर्णांक w, भग्न r, भग्न g, भग्न b)
{
    पूर्णांक बायाँ  = (bbox.x-bbox.w/2)*a.w;
    पूर्णांक दायाँ = (bbox.x+bbox.w/2)*a.w;
    पूर्णांक शीर्ष   = (bbox.y-bbox.h/2)*a.h;
    पूर्णांक bot   = (bbox.y+bbox.h/2)*a.h;

    पूर्णांक i;
    क्रम(i = 0; i < w; ++i){
        draw_box(a, बायाँ+i, शीर्ष+i, दायाँ-i, bot-i, r, g, b);
    }
}

image **load_alphabet()
{
    पूर्णांक i, j;
    स्थिर पूर्णांक nsize = 8;
    image **alphabets = सुस्मृति(nsize, माप(image));
    क्रम(j = 0; j < nsize; ++j){
        alphabets[j] = सुस्मृति(128, माप(image));
        क्रम(i = 32; i < 127; ++i){
            अक्षर buff[256];
            प्र_लिखो(buff, "data/labels/%d_%d.png", i, j);
            alphabets[j][i] = load_image_color(buff, 0, 0);
        }
    }
    वापस alphabets;
}

व्योम draw_detections(image im, detection *dets, पूर्णांक अंक, भग्न thresh, अक्षर **names, image **alphabet, पूर्णांक classes)
{
    पूर्णांक i,j;

    क्रम(i = 0; i < अंक; ++i){
        अक्षर labelstr[4096] = {0};
        पूर्णांक class = -1;
        क्रम(j = 0; j < classes; ++j){
            अगर (dets[i].prob[j] > thresh){
                अगर (class < 0) {
                    म_जोड़ो(labelstr, names[j]);
                    class = j;
                } अन्यथा {
                    म_जोड़ो(labelstr, ", ");
                    म_जोड़ो(labelstr, names[j]);
                }
                म_लिखो("%s: %.0f%%\n", names[j], dets[i].prob[j]*100);
            }
        }
        अगर(class >= 0){
            पूर्णांक width = im.h * .006;

            /*
               if(0){
               width = pow(prob, 1./2.)*10+1;
               alphabet = 0;
               }
             */

            //printf("%d %s: %.0f%%\n", i, names[class], prob*100);
            पूर्णांक offset = class*123457 % classes;
            भग्न red = get_color(2,offset,classes);
            भग्न green = get_color(1,offset,classes);
            भग्न blue = get_color(0,offset,classes);
            भग्न rgb[3];

            //width = prob*20+2;

            rgb[0] = red;
            rgb[1] = green;
            rgb[2] = blue;
            box b = dets[i].bbox;
            //printf("%f %f %f %f\n", b.x, b.y, b.w, b.h);

            पूर्णांक बायाँ  = (b.x-b.w/2.)*im.w;
            पूर्णांक दायाँ = (b.x+b.w/2.)*im.w;
            पूर्णांक शीर्ष   = (b.y-b.h/2.)*im.h;
            पूर्णांक bot   = (b.y+b.h/2.)*im.h;

            अगर(बायाँ < 0) बायाँ = 0;
            अगर(दायाँ > im.w-1) दायाँ = im.w-1;
            अगर(शीर्ष < 0) शीर्ष = 0;
            अगर(bot > im.h-1) bot = im.h-1;

            draw_box_width(im, बायाँ, शीर्ष, दायाँ, bot, width, red, green, blue);
            अगर (alphabet) {
                image label = get_label(alphabet, labelstr, (im.h*.03));
                draw_label(im, शीर्ष + width, बायाँ, label, rgb);
                free_image(label);
            }
            अगर (dets[i].mask){
                image mask = float_to_image(14, 14, 1, dets[i].mask);
                image resized_mask = resize_image(mask, b.w*im.w, b.h*im.h);
                image tmask = threshold_image(resized_mask, .5);
                embed_image(tmask, im, बायाँ, शीर्ष);
                free_image(mask);
                free_image(resized_mask);
                free_image(tmask);
            }
        }
    }
}

व्योम transpose_image(image im)
{
    निश्चित(im.w == im.h);
    पूर्णांक n, m;
    पूर्णांक c;
    क्रम(c = 0; c < im.c; ++c){
        क्रम(n = 0; n < im.w-1; ++n){
            क्रम(m = n + 1; m < im.w; ++m){
                भग्न swap = im.तथ्य[m + im.w*(n + im.h*c)];
                im.तथ्य[m + im.w*(n + im.h*c)] = im.तथ्य[n + im.w*(m + im.h*c)];
                im.तथ्य[n + im.w*(m + im.h*c)] = swap;
            }
        }
    }
}

व्योम rotate_image_cw(image im, पूर्णांक बार)
{
    निश्चित(im.w == im.h);
    बार = (बार + 400) % 4;
    पूर्णांक i, x, y, c;
    पूर्णांक n = im.w;
    क्रम(i = 0; i < बार; ++i){
        क्रम(c = 0; c < im.c; ++c){
            क्रम(x = 0; x < n/2; ++x){
                क्रम(y = 0; y < (n-1)/2 + 1; ++y){
                    भग्न अस्थायी = im.तथ्य[y + im.w*(x + im.h*c)];
                    im.तथ्य[y + im.w*(x + im.h*c)] = im.तथ्य[n-1-x + im.w*(y + im.h*c)];
                    im.तथ्य[n-1-x + im.w*(y + im.h*c)] = im.तथ्य[n-1-y + im.w*(n-1-x + im.h*c)];
                    im.तथ्य[n-1-y + im.w*(n-1-x + im.h*c)] = im.तथ्य[x + im.w*(n-1-y + im.h*c)];
                    im.तथ्य[x + im.w*(n-1-y + im.h*c)] = अस्थायी;
                }
            }
        }
    }
}

व्योम flip_image(image a)
{
    पूर्णांक i,j,k;
    क्रम(k = 0; k < a.c; ++k){
        क्रम(i = 0; i < a.h; ++i){
            क्रम(j = 0; j < a.w/2; ++j){
                पूर्णांक सूचकांक = j + a.w*(i + a.h*(k));
                पूर्णांक flip = (a.w - j - 1) + a.w*(i + a.h*(k));
                भग्न swap = a.तथ्य[flip];
                a.तथ्य[flip] = a.तथ्य[सूचकांक];
                a.तथ्य[सूचकांक] = swap;
            }
        }
    }
}

image image_distance(image a, image b)
{
    पूर्णांक i,j;
    image dist = make_image(a.w, a.h, 1);
    क्रम(i = 0; i < a.c; ++i){
        क्रम(j = 0; j < a.h*a.w; ++j){
            dist.तथ्य[j] += घात(a.तथ्य[i*a.h*a.w+j]-b.तथ्य[i*a.h*a.w+j],2);
        }
    }
    क्रम(j = 0; j < a.h*a.w; ++j){
        dist.तथ्य[j] = वर्ग_मूल(dist.तथ्य[j]);
    }
    वापस dist;
}

व्योम ghost_image(image source, image dest, पूर्णांक dx, पूर्णांक dy)
{
    पूर्णांक x,y,k;
    भग्न max_dist = वर्ग_मूल((-source.w/2. + .5)*(-source.w/2. + .5));
    क्रम(k = 0; k < source.c; ++k){
        क्रम(y = 0; y < source.h; ++y){
            क्रम(x = 0; x < source.w; ++x){
                भग्न dist = वर्ग_मूल((x - source.w/2. + .5)*(x - source.w/2. + .5) + (y - source.h/2. + .5)*(y - source.h/2. + .5));
                भग्न alpha = (1 - dist/max_dist);
                अगर(alpha < 0) alpha = 0;
                भग्न v1 = get_pixel(source, x,y,k);
                भग्न v2 = get_pixel(dest, dx+x,dy+y,k);
                भग्न val = alpha*v1 + (1-alpha)*v2;
                set_pixel(dest, dx+x, dy+y, k, val);
            }
        }
    }
}

व्योम blocky_image(image im, पूर्णांक s)
{
    पूर्णांक i,j,k;
    क्रम(k = 0; k < im.c; ++k){
        क्रम(j = 0; j < im.h; ++j){
            क्रम(i = 0; i < im.w; ++i){
                im.तथ्य[i + im.w*(j + im.h*k)] = im.तथ्य[i/s*s + im.w*(j/s*s + im.h*k)];
            }
        }
    }
}

व्योम censor_image(image im, पूर्णांक dx, पूर्णांक dy, पूर्णांक w, पूर्णांक h)
{
    पूर्णांक i,j,k;
    पूर्णांक s = 32;
    अगर(dx < 0) dx = 0;
    अगर(dy < 0) dy = 0;

    क्रम(k = 0; k < im.c; ++k){
        क्रम(j = dy; j < dy + h && j < im.h; ++j){
            क्रम(i = dx; i < dx + w && i < im.w; ++i){
                im.तथ्य[i + im.w*(j + im.h*k)] = im.तथ्य[i/s*s + im.w*(j/s*s + im.h*k)];
                //im.data[i + j*im.w + k*im.w*im.h] = 0;
            }
        }
    }
}

व्योम embed_image(image source, image dest, पूर्णांक dx, पूर्णांक dy)
{
    पूर्णांक x,y,k;
    क्रम(k = 0; k < source.c; ++k){
        क्रम(y = 0; y < source.h; ++y){
            क्रम(x = 0; x < source.w; ++x){
                भग्न val = get_pixel(source, x,y,k);
                set_pixel(dest, dx+x, dy+y, k, val);
            }
        }
    }
}

image collapse_image_layers(image source, पूर्णांक border)
{
    पूर्णांक h = source.h;
    h = (h+border)*source.c - border;
    image dest = make_image(source.w, h, 1);
    पूर्णांक i;
    क्रम(i = 0; i < source.c; ++i){
        image layer = get_image_layer(source, i);
        पूर्णांक h_offset = i*(source.h+border);
        embed_image(layer, dest, 0, h_offset);
        free_image(layer);
    }
    वापस dest;
}

व्योम constrain_image(image im)
{
    पूर्णांक i;
    क्रम(i = 0; i < im.w*im.h*im.c; ++i){
        अगर(im.तथ्य[i] < 0) im.तथ्य[i] = 0;
        अगर(im.तथ्य[i] > 1) im.तथ्य[i] = 1;
    }
}

व्योम normalize_image(image p)
{
    पूर्णांक i;
    भग्न min = 9999999;
    भग्न max = -999999;

    क्रम(i = 0; i < p.h*p.w*p.c; ++i){
        भग्न v = p.तथ्य[i];
        अगर(v < min) min = v;
        अगर(v > max) max = v;
    }
    अगर(max - min < .000000001){
        min = 0;
        max = 1;
    }
    क्रम(i = 0; i < p.c*p.w*p.h; ++i){
        p.तथ्य[i] = (p.तथ्य[i] - min)/(max-min);
    }
}

व्योम normalize_image2(image p)
{
    भग्न *min = सुस्मृति(p.c, माप(भग्न));
    भग्न *max = सुस्मृति(p.c, माप(भग्न));
    पूर्णांक i,j;
    क्रम(i = 0; i < p.c; ++i) min[i] = max[i] = p.तथ्य[i*p.h*p.w];

    क्रम(j = 0; j < p.c; ++j){
        क्रम(i = 0; i < p.h*p.w; ++i){
            भग्न v = p.तथ्य[i+j*p.h*p.w];
            अगर(v < min[j]) min[j] = v;
            अगर(v > max[j]) max[j] = v;
        }
    }
    क्रम(i = 0; i < p.c; ++i){
        अगर(max[i] - min[i] < .000000001){
            min[i] = 0;
            max[i] = 1;
        }
    }
    क्रम(j = 0; j < p.c; ++j){
        क्रम(i = 0; i < p.w*p.h; ++i){
            p.तथ्य[i+j*p.h*p.w] = (p.तथ्य[i+j*p.h*p.w] - min[j])/(max[j]-min[j]);
        }
    }
    मुक्त(min);
    मुक्त(max);
}

व्योम copy_image_into(image src, image dest)
{
    स_नकल(dest.तथ्य, src.तथ्य, src.h*src.w*src.c*माप(भग्न));
}

image copy_image(image p)
{
    image copy = p;
    copy.तथ्य = सुस्मृति(p.h*p.w*p.c, माप(भग्न));
    स_नकल(copy.तथ्य, p.तथ्य, p.h*p.w*p.c*माप(भग्न));
    वापस copy;
}

व्योम rgbgr_image(image im)
{
    पूर्णांक i;
    क्रम(i = 0; i < im.w*im.h; ++i){
        भग्न swap = im.तथ्य[i];
        im.तथ्य[i] = im.तथ्य[i+im.w*im.h*2];
        im.तथ्य[i+im.w*im.h*2] = swap;
    }
}

पूर्णांक show_image(image p, स्थिर अक्षर *name, पूर्णांक ms)
{
#अगर_घोषित OPENCV
    पूर्णांक c = show_image_cv(p, name, ms);
    वापस c;
#अन्यथा
    ख_लिखो(मानक_त्रुटि, "Not compiled with OpenCV, saving to %s.png instead\n", name);
    save_image(p, name);
    वापस -1;
#पूर्ण_अगर
}

व्योम save_image_options(image im, स्थिर अक्षर *name, IMTYPE f, पूर्णांक quality)
{
    अक्षर buff[256];
    //sprintf(buff, "%s (%d)", name, windows);
    अगर(f == PNG)       प्र_लिखो(buff, "%s.png", name);
    अन्यथा अगर (f == BMP) प्र_लिखो(buff, "%s.bmp", name);
    अन्यथा अगर (f == TGA) प्र_लिखो(buff, "%s.tga", name);
    अन्यथा अगर (f == JPG) प्र_लिखो(buff, "%s.jpg", name);
    अन्यथा               प्र_लिखो(buff, "%s.png", name);
    अचिन्हित अक्षर *तथ्य = सुस्मृति(im.w*im.h*im.c, माप(अक्षर));
    पूर्णांक i,k;
    क्रम(k = 0; k < im.c; ++k){
        क्रम(i = 0; i < im.w*im.h; ++i){
            तथ्य[i*im.c+k] = (अचिन्हित अक्षर) (255*im.तथ्य[i + k*im.w*im.h]);
        }
    }
    पूर्णांक success = 0;
    अगर(f == PNG)       success = stbi_write_png(buff, im.w, im.h, im.c, तथ्य, im.w*im.c);
    अन्यथा अगर (f == BMP) success = stbi_write_bmp(buff, im.w, im.h, im.c, तथ्य);
    अन्यथा अगर (f == TGA) success = stbi_write_tga(buff, im.w, im.h, im.c, तथ्य);
    अन्यथा अगर (f == JPG) success = stbi_write_jpg(buff, im.w, im.h, im.c, तथ्य, quality);
    मुक्त(तथ्य);
    अगर(!success) ख_लिखो(मानक_त्रुटि, "Failed to write image %s\n", buff);
}

व्योम save_image(image im, स्थिर अक्षर *name)
{
    save_image_options(im, name, JPG, 80);
}

व्योम show_image_layers(image p, अक्षर *name)
{
    पूर्णांक i;
    अक्षर buff[256];
    क्रम(i = 0; i < p.c; ++i){
        प्र_लिखो(buff, "%s - Layer %d", name, i);
        image layer = get_image_layer(p, i);
        show_image(layer, buff, 1);
        free_image(layer);
    }
}

व्योम show_image_collapsed(image p, अक्षर *name)
{
    image c = collapse_image_layers(p, 1);
    show_image(c, name, 1);
    free_image(c);
}

image make_empty_image(पूर्णांक w, पूर्णांक h, पूर्णांक c)
{
    image out;
    out.तथ्य = 0;
    out.h = h;
    out.w = w;
    out.c = c;
    वापस out;
}

image make_image(पूर्णांक w, पूर्णांक h, पूर्णांक c)
{
    image out = make_empty_image(w,h,c);
    out.तथ्य = सुस्मृति(h*w*c, माप(भग्न));
    वापस out;
}

image make_random_image(पूर्णांक w, पूर्णांक h, पूर्णांक c)
{
    image out = make_empty_image(w,h,c);
    out.तथ्य = सुस्मृति(h*w*c, माप(भग्न));
    पूर्णांक i;
    क्रम(i = 0; i < w*h*c; ++i){
        out.तथ्य[i] = (rand_normal() * .25) + .5;
    }
    वापस out;
}

image float_to_image(पूर्णांक w, पूर्णांक h, पूर्णांक c, भग्न *तथ्य)
{
    image out = make_empty_image(w,h,c);
    out.तथ्य = तथ्य;
    वापस out;
}

व्योम place_image(image im, पूर्णांक w, पूर्णांक h, पूर्णांक dx, पूर्णांक dy, image canvas)
{
    पूर्णांक x, y, c;
    क्रम(c = 0; c < im.c; ++c){
        क्रम(y = 0; y < h; ++y){
            क्रम(x = 0; x < w; ++x){
                भग्न rx = ((भग्न)x / w) * im.w;
                भग्न ry = ((भग्न)y / h) * im.h;
                भग्न val = bilinear_interpolate(im, rx, ry, c);
                set_pixel(canvas, x + dx, y + dy, c, val);
            }
        }
    }
}

image center_crop_image(image im, पूर्णांक w, पूर्णांक h)
{
    पूर्णांक m = (im.w < im.h) ? im.w : im.h;   
    image c = crop_image(im, (im.w - m) / 2, (im.h - m)/2, m, m);
    image r = resize_image(c, w, h);
    free_image(c);
    वापस r;
}

image rotate_crop_image(image im, भग्न rad, भग्न s, पूर्णांक w, पूर्णांक h, भग्न dx, भग्न dy, भग्न aspect)
{
    पूर्णांक x, y, c;
    भग्न cx = im.w/2.;
    भग्न cy = im.h/2.;
    image rot = make_image(w, h, im.c);
    क्रम(c = 0; c < im.c; ++c){
        क्रम(y = 0; y < h; ++y){
            क्रम(x = 0; x < w; ++x){
                भग्न rx = cos(rad)*((x - w/2.)/s*aspect + dx/s*aspect) - sin(rad)*((y - h/2.)/s + dy/s) + cx;
                भग्न ry = sin(rad)*((x - w/2.)/s*aspect + dx/s*aspect) + cos(rad)*((y - h/2.)/s + dy/s) + cy;
                भग्न val = bilinear_interpolate(im, rx, ry, c);
                set_pixel(rot, x, y, c, val);
            }
        }
    }
    वापस rot;
}

image rotate_image(image im, भग्न rad)
{
    पूर्णांक x, y, c;
    भग्न cx = im.w/2.;
    भग्न cy = im.h/2.;
    image rot = make_image(im.w, im.h, im.c);
    क्रम(c = 0; c < im.c; ++c){
        क्रम(y = 0; y < im.h; ++y){
            क्रम(x = 0; x < im.w; ++x){
                भग्न rx = cos(rad)*(x-cx) - sin(rad)*(y-cy) + cx;
                भग्न ry = sin(rad)*(x-cx) + cos(rad)*(y-cy) + cy;
                भग्न val = bilinear_interpolate(im, rx, ry, c);
                set_pixel(rot, x, y, c, val);
            }
        }
    }
    वापस rot;
}

व्योम fill_image(image m, भग्न s)
{
    पूर्णांक i;
    क्रम(i = 0; i < m.h*m.w*m.c; ++i) m.तथ्य[i] = s;
}

व्योम translate_image(image m, भग्न s)
{
    पूर्णांक i;
    क्रम(i = 0; i < m.h*m.w*m.c; ++i) m.तथ्य[i] += s;
}

व्योम scale_image(image m, भग्न s)
{
    पूर्णांक i;
    क्रम(i = 0; i < m.h*m.w*m.c; ++i) m.तथ्य[i] *= s;
}

image crop_image(image im, पूर्णांक dx, पूर्णांक dy, पूर्णांक w, पूर्णांक h)
{
    image cropped = make_image(w, h, im.c);
    पूर्णांक i, j, k;
    क्रम(k = 0; k < im.c; ++k){
        क्रम(j = 0; j < h; ++j){
            क्रम(i = 0; i < w; ++i){
                पूर्णांक r = j + dy;
                पूर्णांक c = i + dx;
                भग्न val = 0;
                r = constrain_int(r, 0, im.h-1);
                c = constrain_int(c, 0, im.w-1);
                val = get_pixel(im, c, r, k);
                set_pixel(cropped, i, j, k, val);
            }
        }
    }
    वापस cropped;
}

पूर्णांक best_3d_shift_r(image a, image b, पूर्णांक min, पूर्णांक max)
{
    अगर(min == max) वापस min;
    पूर्णांक mid = न्यूनमान((min + max) / 2.);
    image c1 = crop_image(b, 0, mid, b.w, b.h);
    image c2 = crop_image(b, 0, mid+1, b.w, b.h);
    भग्न d1 = dist_array(c1.तथ्य, a.तथ्य, a.w*a.h*a.c, 10);
    भग्न d2 = dist_array(c2.तथ्य, a.तथ्य, a.w*a.h*a.c, 10);
    free_image(c1);
    free_image(c2);
    अगर(d1 < d2) वापस best_3d_shift_r(a, b, min, mid);
    अन्यथा वापस best_3d_shift_r(a, b, mid+1, max);
}

पूर्णांक best_3d_shift(image a, image b, पूर्णांक min, पूर्णांक max)
{
    पूर्णांक i;
    पूर्णांक best = 0;
    भग्न best_distance = भग्न_उच्च;
    क्रम(i = min; i <= max; i += 2){
        image c = crop_image(b, 0, i, b.w, b.h);
        भग्न d = dist_array(c.तथ्य, a.तथ्य, a.w*a.h*a.c, 100);
        अगर(d < best_distance){
            best_distance = d;
            best = i;
        }
        म_लिखो("%d %f\n", i, d);
        free_image(c);
    }
    वापस best;
}

व्योम composite_3d(अक्षर *f1, अक्षर *f2, अक्षर *out, पूर्णांक delta)
{
    अगर(!out) out = "out";
    image a = load_image(f1, 0,0,0);
    image b = load_image(f2, 0,0,0);
    पूर्णांक shift = best_3d_shift_r(a, b, -a.h/100, a.h/100);

    image c1 = crop_image(b, 10, shift, b.w, b.h);
    भग्न d1 = dist_array(c1.तथ्य, a.तथ्य, a.w*a.h*a.c, 100);
    image c2 = crop_image(b, -10, shift, b.w, b.h);
    भग्न d2 = dist_array(c2.तथ्य, a.तथ्य, a.w*a.h*a.c, 100);

    अगर(d2 < d1 && 0){
        image swap = a;
        a = b;
        b = swap;
        shift = -shift;
        म_लिखो("swapped, %d\n", shift);
    }
    अन्यथा{
        म_लिखो("%d\n", shift);
    }

    image c = crop_image(b, delta, shift, a.w, a.h);
    पूर्णांक i;
    क्रम(i = 0; i < c.w*c.h; ++i){
        c.तथ्य[i] = a.तथ्य[i];
    }
    save_image(c, out);
}

व्योम letterbox_image_into(image im, पूर्णांक w, पूर्णांक h, image boxed)
{
    पूर्णांक new_w = im.w;
    पूर्णांक new_h = im.h;
    अगर (((भग्न)w/im.w) < ((भग्न)h/im.h)) {
        new_w = w;
        new_h = (im.h * w)/im.w;
    } अन्यथा {
        new_h = h;
        new_w = (im.w * h)/im.h;
    }
    image resized = resize_image(im, new_w, new_h);
    embed_image(resized, boxed, (w-new_w)/2, (h-new_h)/2); 
    free_image(resized);
}

image letterbox_image(image im, पूर्णांक w, पूर्णांक h)
{
    पूर्णांक new_w = im.w;
    पूर्णांक new_h = im.h;
    अगर (((भग्न)w/im.w) < ((भग्न)h/im.h)) {
        new_w = w;
        new_h = (im.h * w)/im.w;
    } अन्यथा {
        new_h = h;
        new_w = (im.w * h)/im.h;
    }
    image resized = resize_image(im, new_w, new_h);
    image boxed = make_image(w, h, im.c);
    fill_image(boxed, .5);
    //int i;
    //for(i = 0; i < boxed.w*boxed.h*boxed.c; ++i) boxed.data[i] = 0;
    embed_image(resized, boxed, (w-new_w)/2, (h-new_h)/2); 
    free_image(resized);
    वापस boxed;
}

image resize_max(image im, पूर्णांक max)
{
    पूर्णांक w = im.w;
    पूर्णांक h = im.h;
    अगर(w > h){
        h = (h * max) / w;
        w = max;
    } अन्यथा {
        w = (w * max) / h;
        h = max;
    }
    अगर(w == im.w && h == im.h) वापस im;
    image resized = resize_image(im, w, h);
    वापस resized;
}

image resize_min(image im, पूर्णांक min)
{
    पूर्णांक w = im.w;
    पूर्णांक h = im.h;
    अगर(w < h){
        h = (h * min) / w;
        w = min;
    } अन्यथा {
        w = (w * min) / h;
        h = min;
    }
    अगर(w == im.w && h == im.h) वापस im;
    image resized = resize_image(im, w, h);
    वापस resized;
}

image random_crop_image(image im, पूर्णांक w, पूर्णांक h)
{
    पूर्णांक dx = rand_int(0, im.w - w);
    पूर्णांक dy = rand_int(0, im.h - h);
    image crop = crop_image(im, dx, dy, w, h);
    वापस crop;
}

augment_args random_augment_args(image im, भग्न angle, भग्न aspect, पूर्णांक निचला, पूर्णांक ऊपरी, पूर्णांक w, पूर्णांक h)
{
    augment_args a = {0};
    aspect = rand_scale(aspect);
    पूर्णांक r = rand_int(निचला, ऊपरी);
    पूर्णांक min = (im.h < im.w*aspect) ? im.h : im.w*aspect;
    भग्न scale = (भग्न)r / min;

    भग्न rad = rand_uniform(-angle, angle) * TWO_PI / 360.;

    भग्न dx = (im.w*scale/aspect - w) / 2.;
    भग्न dy = (im.h*scale - w) / 2.;
    //if(dx < 0) dx = 0;
    //if(dy < 0) dy = 0;
    dx = rand_uniform(-dx, dx);
    dy = rand_uniform(-dy, dy);

    a.rad = rad;
    a.scale = scale;
    a.w = w;
    a.h = h;
    a.dx = dx;
    a.dy = dy;
    a.aspect = aspect;
    वापस a;
}

image random_augment_image(image im, भग्न angle, भग्न aspect, पूर्णांक निचला, पूर्णांक ऊपरी, पूर्णांक w, पूर्णांक h)
{
    augment_args a = random_augment_args(im, angle, aspect, निचला, ऊपरी, w, h);
    image crop = rotate_crop_image(im, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);
    वापस crop;
}

भग्न three_way_max(भग्न a, भग्न b, भग्न c)
{
    वापस (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

भग्न three_way_min(भग्न a, भग्न b, भग्न c)
{
    वापस (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

व्योम yuv_to_rgb(image im)
{
    निश्चित(im.c == 3);
    पूर्णांक i, j;
    भग्न r, g, b;
    भग्न y, u, v;
    क्रम(j = 0; j < im.h; ++j){
        क्रम(i = 0; i < im.w; ++i){
            y = get_pixel(im, i , j, 0);
            u = get_pixel(im, i , j, 1);
            v = get_pixel(im, i , j, 2);

            r = y + 1.13983*v;
            g = y + -.39465*u + -.58060*v;
            b = y + 2.03211*u;

            set_pixel(im, i, j, 0, r);
            set_pixel(im, i, j, 1, g);
            set_pixel(im, i, j, 2, b);
        }
    }
}

व्योम rgb_to_yuv(image im)
{
    निश्चित(im.c == 3);
    पूर्णांक i, j;
    भग्न r, g, b;
    भग्न y, u, v;
    क्रम(j = 0; j < im.h; ++j){
        क्रम(i = 0; i < im.w; ++i){
            r = get_pixel(im, i , j, 0);
            g = get_pixel(im, i , j, 1);
            b = get_pixel(im, i , j, 2);

            y = .299*r + .587*g + .114*b;
            u = -.14713*r + -.28886*g + .436*b;
            v = .615*r + -.51499*g + -.10001*b;

            set_pixel(im, i, j, 0, y);
            set_pixel(im, i, j, 1, u);
            set_pixel(im, i, j, 2, v);
        }
    }
}

// http://www.cs.rit.edu/~ncs/color/t_convert.html
व्योम rgb_to_hsv(image im)
{
    निश्चित(im.c == 3);
    पूर्णांक i, j;
    भग्न r, g, b;
    भग्न h, s, v;
    क्रम(j = 0; j < im.h; ++j){
        क्रम(i = 0; i < im.w; ++i){
            r = get_pixel(im, i , j, 0);
            g = get_pixel(im, i , j, 1);
            b = get_pixel(im, i , j, 2);
            भग्न max = three_way_max(r,g,b);
            भग्न min = three_way_min(r,g,b);
            भग्न delta = max - min;
            v = max;
            अगर(max == 0){
                s = 0;
                h = 0;
            }अन्यथा{
                s = delta/max;
                अगर(r == max){
                    h = (g - b) / delta;
                } अन्यथा अगर (g == max) {
                    h = 2 + (b - r) / delta;
                } अन्यथा {
                    h = 4 + (r - g) / delta;
                }
                अगर (h < 0) h += 6;
                h = h/6.;
            }
            set_pixel(im, i, j, 0, h);
            set_pixel(im, i, j, 1, s);
            set_pixel(im, i, j, 2, v);
        }
    }
}

व्योम hsv_to_rgb(image im)
{
    निश्चित(im.c == 3);
    पूर्णांक i, j;
    भग्न r, g, b;
    भग्न h, s, v;
    भग्न f, p, q, t;
    क्रम(j = 0; j < im.h; ++j){
        क्रम(i = 0; i < im.w; ++i){
            h = 6 * get_pixel(im, i , j, 0);
            s = get_pixel(im, i , j, 1);
            v = get_pixel(im, i , j, 2);
            अगर (s == 0) {
                r = g = b = v;
            } अन्यथा {
                पूर्णांक सूचकांक = न्यूनमान(h);
                f = h - सूचकांक;
                p = v*(1-s);
                q = v*(1-s*f);
                t = v*(1-s*(1-f));
                अगर(सूचकांक == 0){
                    r = v; g = t; b = p;
                } अन्यथा अगर(सूचकांक == 1){
                    r = q; g = v; b = p;
                } अन्यथा अगर(सूचकांक == 2){
                    r = p; g = v; b = t;
                } अन्यथा अगर(सूचकांक == 3){
                    r = p; g = q; b = v;
                } अन्यथा अगर(सूचकांक == 4){
                    r = t; g = p; b = v;
                } अन्यथा {
                    r = v; g = p; b = q;
                }
            }
            set_pixel(im, i, j, 0, r);
            set_pixel(im, i, j, 1, g);
            set_pixel(im, i, j, 2, b);
        }
    }
}

व्योम grayscale_image_3c(image im)
{
    निश्चित(im.c == 3);
    पूर्णांक i, j, k;
    भग्न scale[] = {0.299, 0.587, 0.114};
    क्रम(j = 0; j < im.h; ++j){
        क्रम(i = 0; i < im.w; ++i){
            भग्न val = 0;
            क्रम(k = 0; k < 3; ++k){
                val += scale[k]*get_pixel(im, i, j, k);
            }
            im.तथ्य[0*im.h*im.w + im.w*j + i] = val;
            im.तथ्य[1*im.h*im.w + im.w*j + i] = val;
            im.तथ्य[2*im.h*im.w + im.w*j + i] = val;
        }
    }
}

image grayscale_image(image im)
{
    निश्चित(im.c == 3);
    पूर्णांक i, j, k;
    image gray = make_image(im.w, im.h, 1);
    भग्न scale[] = {0.299, 0.587, 0.114};
    क्रम(k = 0; k < im.c; ++k){
        क्रम(j = 0; j < im.h; ++j){
            क्रम(i = 0; i < im.w; ++i){
                gray.तथ्य[i+im.w*j] += scale[k]*get_pixel(im, i, j, k);
            }
        }
    }
    वापस gray;
}

image threshold_image(image im, भग्न thresh)
{
    पूर्णांक i;
    image t = make_image(im.w, im.h, im.c);
    क्रम(i = 0; i < im.w*im.h*im.c; ++i){
        t.तथ्य[i] = im.तथ्य[i]>thresh ? 1 : 0;
    }
    वापस t;
}

image blend_image(image fore, image back, भग्न alpha)
{
    निश्चित(fore.w == back.w && fore.h == back.h && fore.c == back.c);
    image blend = make_image(fore.w, fore.h, fore.c);
    पूर्णांक i, j, k;
    क्रम(k = 0; k < fore.c; ++k){
        क्रम(j = 0; j < fore.h; ++j){
            क्रम(i = 0; i < fore.w; ++i){
                भग्न val = alpha * get_pixel(fore, i, j, k) + 
                    (1 - alpha)* get_pixel(back, i, j, k);
                set_pixel(blend, i, j, k, val);
            }
        }
    }
    वापस blend;
}

व्योम scale_image_channel(image im, पूर्णांक c, भग्न v)
{
    पूर्णांक i, j;
    क्रम(j = 0; j < im.h; ++j){
        क्रम(i = 0; i < im.w; ++i){
            भग्न pix = get_pixel(im, i, j, c);
            pix = pix*v;
            set_pixel(im, i, j, c, pix);
        }
    }
}

व्योम translate_image_channel(image im, पूर्णांक c, भग्न v)
{
    पूर्णांक i, j;
    क्रम(j = 0; j < im.h; ++j){
        क्रम(i = 0; i < im.w; ++i){
            भग्न pix = get_pixel(im, i, j, c);
            pix = pix+v;
            set_pixel(im, i, j, c, pix);
        }
    }
}

image binarize_image(image im)
{
    image c = copy_image(im);
    पूर्णांक i;
    क्रम(i = 0; i < im.w * im.h * im.c; ++i){
        अगर(c.तथ्य[i] > .5) c.तथ्य[i] = 1;
        अन्यथा c.तथ्य[i] = 0;
    }
    वापस c;
}

व्योम saturate_image(image im, भग्न sat)
{
    rgb_to_hsv(im);
    scale_image_channel(im, 1, sat);
    hsv_to_rgb(im);
    constrain_image(im);
}

व्योम hue_image(image im, भग्न hue)
{
    rgb_to_hsv(im);
    पूर्णांक i;
    क्रम(i = 0; i < im.w*im.h; ++i){
        im.तथ्य[i] = im.तथ्य[i] + hue;
        अगर (im.तथ्य[i] > 1) im.तथ्य[i] -= 1;
        अगर (im.तथ्य[i] < 0) im.तथ्य[i] += 1;
    }
    hsv_to_rgb(im);
    constrain_image(im);
}

व्योम exposure_image(image im, भग्न sat)
{
    rgb_to_hsv(im);
    scale_image_channel(im, 2, sat);
    hsv_to_rgb(im);
    constrain_image(im);
}

व्योम distort_image(image im, भग्न hue, भग्न sat, भग्न val)
{
    rgb_to_hsv(im);
    scale_image_channel(im, 1, sat);
    scale_image_channel(im, 2, val);
    पूर्णांक i;
    क्रम(i = 0; i < im.w*im.h; ++i){
        im.तथ्य[i] = im.तथ्य[i] + hue;
        अगर (im.तथ्य[i] > 1) im.तथ्य[i] -= 1;
        अगर (im.तथ्य[i] < 0) im.तथ्य[i] += 1;
    }
    hsv_to_rgb(im);
    constrain_image(im);
}

व्योम random_distort_image(image im, भग्न hue, भग्न saturation, भग्न exposure)
{
    भग्न dhue = rand_uniform(-hue, hue);
    भग्न dsat = rand_scale(saturation);
    भग्न dexp = rand_scale(exposure);
    distort_image(im, dhue, dsat, dexp);
}

व्योम saturate_exposure_image(image im, भग्न sat, भग्न exposure)
{
    rgb_to_hsv(im);
    scale_image_channel(im, 1, sat);
    scale_image_channel(im, 2, exposure);
    hsv_to_rgb(im);
    constrain_image(im);
}

image resize_image(image im, पूर्णांक w, पूर्णांक h)
{
    image resized = make_image(w, h, im.c);   
    image part = make_image(w, im.h, im.c);
    पूर्णांक r, c, k;
    भग्न w_scale = (भग्न)(im.w - 1) / (w - 1);
    भग्न h_scale = (भग्न)(im.h - 1) / (h - 1);
    क्रम(k = 0; k < im.c; ++k){
        क्रम(r = 0; r < im.h; ++r){
            क्रम(c = 0; c < w; ++c){
                भग्न val = 0;
                अगर(c == w-1 || im.w == 1){
                    val = get_pixel(im, im.w-1, r, k);
                } अन्यथा {
                    भग्न sx = c*w_scale;
                    पूर्णांक ix = (पूर्णांक) sx;
                    भग्न dx = sx - ix;
                    val = (1 - dx) * get_pixel(im, ix, r, k) + dx * get_pixel(im, ix+1, r, k);
                }
                set_pixel(part, c, r, k, val);
            }
        }
    }
    क्रम(k = 0; k < im.c; ++k){
        क्रम(r = 0; r < h; ++r){
            भग्न sy = r*h_scale;
            पूर्णांक iy = (पूर्णांक) sy;
            भग्न dy = sy - iy;
            क्रम(c = 0; c < w; ++c){
                भग्न val = (1-dy) * get_pixel(part, c, iy, k);
                set_pixel(resized, c, r, k, val);
            }
            अगर(r == h-1 || im.h == 1) जारी;
            क्रम(c = 0; c < w; ++c){
                भग्न val = dy * get_pixel(part, c, iy+1, k);
                add_pixel(resized, c, r, k, val);
            }
        }
    }

    free_image(part);
    वापस resized;
}


व्योम test_resize(अक्षर *filename)
{
    image im = load_image(filename, 0,0, 3);
    भग्न mag = mag_array(im.तथ्य, im.w*im.h*im.c);
    म_लिखो("L2 Norm: %f\n", mag);
    image gray = grayscale_image(im);

    image c1 = copy_image(im);
    image c2 = copy_image(im);
    image c3 = copy_image(im);
    image c4 = copy_image(im);
    distort_image(c1, .1, 1.5, 1.5);
    distort_image(c2, -.1, .66666, .66666);
    distort_image(c3, .1, 1.5, .66666);
    distort_image(c4, .1, .66666, 1.5);


    show_image(im,   "Original", 1);
    show_image(gray, "Gray", 1);
    show_image(c1, "C1", 1);
    show_image(c2, "C2", 1);
    show_image(c3, "C3", 1);
    show_image(c4, "C4", 1);
#अगर_घोषित OPENCV
    जबतक(1){
        image aug = random_augment_image(im, 0, .75, 320, 448, 320, 320);
        show_image(aug, "aug", 1);
        free_image(aug);


        भग्न exposure = 1.15;
        भग्न saturation = 1.15;
        भग्न hue = .05;

        image c = copy_image(im);

        भग्न dexp = rand_scale(exposure);
        भग्न dsat = rand_scale(saturation);
        भग्न dhue = rand_uniform(-hue, hue);

        distort_image(c, dhue, dsat, dexp);
        show_image(c, "rand", 1);
        म_लिखो("%f %f %f\n", dhue, dsat, dexp);
        free_image(c);
    }
#पूर्ण_अगर
}


image load_image_stb(अक्षर *filename, पूर्णांक channels)
{
    पूर्णांक w, h, c;
    अचिन्हित अक्षर *तथ्य = stbi_load(filename, &w, &h, &c, channels);
    अगर (!तथ्य) {
        ख_लिखो(मानक_त्रुटि, "Cannot load image \"%s\"\nSTB Reason: %s\n", filename, stbi_failure_reason());
        निकास(0);
    }
    अगर(channels) c = channels;
    पूर्णांक i,j,k;
    image im = make_image(w, h, c);
    क्रम(k = 0; k < c; ++k){
        क्रम(j = 0; j < h; ++j){
            क्रम(i = 0; i < w; ++i){
                पूर्णांक dst_सूचकांक = i + w*j + w*h*k;
                पूर्णांक src_सूचकांक = k + c*i + c*w*j;
                im.तथ्य[dst_सूचकांक] = (भग्न)तथ्य[src_सूचकांक]/255.;
            }
        }
    }
    मुक्त(तथ्य);
    वापस im;
}

image load_image(अक्षर *filename, पूर्णांक w, पूर्णांक h, पूर्णांक c)
{
#अगर_घोषित OPENCV
    image out = load_image_cv(filename, c);
#अन्यथा
    image out = load_image_stb(filename, c);
#पूर्ण_अगर

    अगर((h && w) && (h != out.h || w != out.w)){
        image resized = resize_image(out, w, h);
        free_image(out);
        out = resized;
    }
    वापस out;
}

image load_image_color(अक्षर *filename, पूर्णांक w, पूर्णांक h)
{
    वापस load_image(filename, w, h, 3);
}

image get_image_layer(image m, पूर्णांक l)
{
    image out = make_image(m.w, m.h, 1);
    पूर्णांक i;
    क्रम(i = 0; i < m.h*m.w; ++i){
        out.तथ्य[i] = m.तथ्य[i+l*m.h*m.w];
    }
    वापस out;
}
व्योम print_image(image m)
{
    पूर्णांक i, j, k;
    क्रम(i =0 ; i < m.c; ++i){
        क्रम(j =0 ; j < m.h; ++j){
            क्रम(k = 0; k < m.w; ++k){
                म_लिखो("%.2lf, ", m.तथ्य[i*m.h*m.w + j*m.w + k]);
                अगर(k > 30) अवरोध;
            }
            म_लिखो("\n");
            अगर(j > 30) अवरोध;
        }
        म_लिखो("\n");
    }
    म_लिखो("\n");
}

image collapse_images_vert(image *ims, पूर्णांक n)
{
    पूर्णांक color = 1;
    पूर्णांक border = 1;
    पूर्णांक h,w,c;
    w = ims[0].w;
    h = (ims[0].h + border) * n - border;
    c = ims[0].c;
    अगर(c != 3 || !color){
        w = (w+border)*c - border;
        c = 1;
    }

    image filters = make_image(w, h, c);
    पूर्णांक i,j;
    क्रम(i = 0; i < n; ++i){
        पूर्णांक h_offset = i*(ims[0].h+border);
        image copy = copy_image(ims[i]);
        //normalize_image(copy);
        अगर(c == 3 && color){
            embed_image(copy, filters, 0, h_offset);
        }
        अन्यथा{
            क्रम(j = 0; j < copy.c; ++j){
                पूर्णांक w_offset = j*(ims[0].w+border);
                image layer = get_image_layer(copy, j);
                embed_image(layer, filters, w_offset, h_offset);
                free_image(layer);
            }
        }
        free_image(copy);
    }
    वापस filters;
} 

image collapse_images_horz(image *ims, पूर्णांक n)
{
    पूर्णांक color = 1;
    पूर्णांक border = 1;
    पूर्णांक h,w,c;
    पूर्णांक आकार = ims[0].h;
    h = आकार;
    w = (ims[0].w + border) * n - border;
    c = ims[0].c;
    अगर(c != 3 || !color){
        h = (h+border)*c - border;
        c = 1;
    }

    image filters = make_image(w, h, c);
    पूर्णांक i,j;
    क्रम(i = 0; i < n; ++i){
        पूर्णांक w_offset = i*(आकार+border);
        image copy = copy_image(ims[i]);
        //normalize_image(copy);
        अगर(c == 3 && color){
            embed_image(copy, filters, w_offset, 0);
        }
        अन्यथा{
            क्रम(j = 0; j < copy.c; ++j){
                पूर्णांक h_offset = j*(आकार+border);
                image layer = get_image_layer(copy, j);
                embed_image(layer, filters, w_offset, h_offset);
                free_image(layer);
            }
        }
        free_image(copy);
    }
    वापस filters;
} 

व्योम show_image_normalized(image im, स्थिर अक्षर *name)
{
    image c = copy_image(im);
    normalize_image(c);
    show_image(c, name, 1);
    free_image(c);
}

व्योम show_images(image *ims, पूर्णांक n, अक्षर *window)
{
    image m = collapse_images_vert(ims, n);
    /*
       int w = 448;
       int h = ((float)m.h/m.w) * 448;
       if(h > 896){
       h = 896;
       w = ((float)m.w/m.h) * 896;
       }
       image sized = resize_image(m, w, h);
     */
    normalize_image(m);
    save_image(m, window);
    show_image(m, window, 1);
    free_image(m);
}

व्योम free_image(image m)
{
    अगर(m.तथ्य){
        मुक्त(m.तथ्य);
    }
}
