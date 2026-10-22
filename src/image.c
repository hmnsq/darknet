#अन्तर्भाव "image.h"
#अन्तर्भाव "utils.h"
#अन्तर्भाव "blas.h"
#अन्तर्भाव "cuda.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <math.h>

#define STB_IMAGE_IMPLEMENTATION
#अन्तर्भाव "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#अन्तर्भाव "stb_image_write.h"

पूर्णांक windows = 0;

दशांश colors[6][3] = { {1,0,1}, {0,0,1},{0,1,1},{0,1,0},{1,1,0},{1,0,0} };

दशांश get_color(पूर्णांक c, पूर्णांक x, पूर्णांक max)
{
    दशांश ratio = ((दशांश)x/max)*5;
    पूर्णांक i = न्यूनमान(ratio);
    पूर्णांक j = उच्चमान(ratio);
    ratio -= i;
    दशांश r = (1-ratio) * colors[i][c] + ratio*colors[j][c];
    //printf("%f\n", r);
    परत r;
}

image mask_to_rgb(image mask)
{
    पूर्णांक n = mask.c;
    image im = make_image(mask.w, mask.h, 3);
    पूर्णांक i, j;
    साठी(j = 0; j < n; ++j){
        पूर्णांक offset = j*123457 % n;
        दशांश red = get_color(2,offset,n);
        दशांश green = get_color(1,offset,n);
        दशांश blue = get_color(0,offset,n);
        साठी(i = 0; i < im.w*im.h; ++i){
            im.data[i + 0*im.w*im.h] += mask.data[j*im.h*im.w + i]*red;
            im.data[i + 1*im.w*im.h] += mask.data[j*im.h*im.w + i]*green;
            im.data[i + 2*im.w*im.h] += mask.data[j*im.h*im.w + i]*blue;
        }
    }
    परत im;
}

स्थिर दशांश get_pixel(image m, पूर्णांक x, पूर्णांक y, पूर्णांक c)
{
    प्रतिज्ञा(x < m.w && y < m.h && c < m.c);
    परत m.data[c*m.h*m.w + y*m.w + x];
}
स्थिर दशांश get_pixel_extend(image m, पूर्णांक x, पूर्णांक y, पूर्णांक c)
{
    जर(x < 0 || x >= m.w || y < 0 || y >= m.h) परत 0;
    /*
    if(x < 0) x = 0;
    if(x >= m.w) x = m.w-1;
    if(y < 0) y = 0;
    if(y >= m.h) y = m.h-1;
    */
    जर(c < 0 || c >= m.c) परत 0;
    परत get_pixel(m, x, y, c);
}
स्थिर शून्य set_pixel(image m, पूर्णांक x, पूर्णांक y, पूर्णांक c, दशांश val)
{
    जर (x < 0 || y < 0 || c < 0 || x >= m.w || y >= m.h || c >= m.c) परत;
    प्रतिज्ञा(x < m.w && y < m.h && c < m.c);
    m.data[c*m.h*m.w + y*m.w + x] = val;
}
स्थिर शून्य add_pixel(image m, पूर्णांक x, पूर्णांक y, पूर्णांक c, दशांश val)
{
    प्रतिज्ञा(x < m.w && y < m.h && c < m.c);
    m.data[c*m.h*m.w + y*m.w + x] += val;
}

स्थिर दशांश bilinear_interpolate(image im, दशांश x, दशांश y, पूर्णांक c)
{
    पूर्णांक ix = (पूर्णांक) floorf(x);
    पूर्णांक iy = (पूर्णांक) floorf(y);

    दशांश dx = x - ix;
    दशांश dy = y - iy;

    दशांश val = (1-dy) * (1-dx) * get_pixel_extend(im, ix, iy, c) + 
        dy     * (1-dx) * get_pixel_extend(im, ix, iy+1, c) + 
        (1-dy) *   dx   * get_pixel_extend(im, ix+1, iy, c) +
        dy     *   dx   * get_pixel_extend(im, ix+1, iy+1, c);
    परत val;
}


शून्य composite_image(image source, image dest, पूर्णांक dx, पूर्णांक dy)
{
    पूर्णांक x,y,k;
    साठी(k = 0; k < source.c; ++k){
        साठी(y = 0; y < source.h; ++y){
            साठी(x = 0; x < source.w; ++x){
                दशांश val = get_pixel(source, x, y, k);
                दशांश val2 = get_pixel_extend(dest, dx+x, dy+y, k);
                set_pixel(dest, dx+x, dy+y, k, val * val2);
            }
        }
    }
}

image border_image(image a, पूर्णांक border)
{
    image b = make_image(a.w + 2*border, a.h + 2*border, a.c);
    पूर्णांक x,y,k;
    साठी(k = 0; k < b.c; ++k){
        साठी(y = 0; y < b.h; ++y){
            साठी(x = 0; x < b.w; ++x){
                दशांश val = get_pixel_extend(a, x - border, y - border, k);
                जर(x - border < 0 || x - border >= a.w || y - border < 0 || y - border >= a.h) val = 1;
                set_pixel(b, x, y, k, val);
            }
        }
    }
    परत b;
}

image tile_images(image a, image b, पूर्णांक dx)
{
    जर(a.w == 0) परत copy_image(b);
    image c = make_image(a.w + b.w + dx, (a.h > b.h) ? a.h : b.h, (a.c > b.c) ? a.c : b.c);
    fill_cpu(c.w*c.h*c.c, 1, c.data, 1);
    embed_image(a, c, 0, 0); 
    composite_image(b, c, a.w + dx, 0);
    परत c;
}

image get_label(image **characters, अक्षर *string, पूर्णांक size)
{
    size = size/10;
    जर(size > 7) size = 7;
    image label = make_empty_image(0,0,0);
    पर्यंत(*string){
        image l = characters[size][(पूर्णांक)*string];
        image n = tile_images(label, l, -size - 1 + (size+1)/2);
        free_image(label);
        label = n;
        ++string;
    }
    image b = border_image(label, label.h*.25);
    free_image(label);
    परत b;
}

शून्य draw_label(image a, पूर्णांक r, पूर्णांक c, image label, स्थिरांक दशांश *rgb)
{
    पूर्णांक w = label.w;
    पूर्णांक h = label.h;
    जर (r - h >= 0) r = r - h;

    पूर्णांक i, j, k;
    साठी(j = 0; j < h && j + r < a.h; ++j){
        साठी(i = 0; i < w && i + c < a.w; ++i){
            साठी(k = 0; k < label.c; ++k){
                दशांश val = get_pixel(label, i, j, k);
                set_pixel(a, i+c, j+r, k, rgb[k] * val);
            }
        }
    }
}

शून्य draw_box(image a, पूर्णांक x1, पूर्णांक y1, पूर्णांक x2, पूर्णांक y2, दशांश r, दशांश g, दशांश b)
{
    //normalize_image(a);
    पूर्णांक i;
    जर(x1 < 0) x1 = 0;
    जर(x1 >= a.w) x1 = a.w-1;
    जर(x2 < 0) x2 = 0;
    जर(x2 >= a.w) x2 = a.w-1;

    जर(y1 < 0) y1 = 0;
    जर(y1 >= a.h) y1 = a.h-1;
    जर(y2 < 0) y2 = 0;
    जर(y2 >= a.h) y2 = a.h-1;

    साठी(i = x1; i <= x2; ++i){
        a.data[i + y1*a.w + 0*a.w*a.h] = r;
        a.data[i + y2*a.w + 0*a.w*a.h] = r;

        a.data[i + y1*a.w + 1*a.w*a.h] = g;
        a.data[i + y2*a.w + 1*a.w*a.h] = g;

        a.data[i + y1*a.w + 2*a.w*a.h] = b;
        a.data[i + y2*a.w + 2*a.w*a.h] = b;
    }
    साठी(i = y1; i <= y2; ++i){
        a.data[x1 + i*a.w + 0*a.w*a.h] = r;
        a.data[x2 + i*a.w + 0*a.w*a.h] = r;

        a.data[x1 + i*a.w + 1*a.w*a.h] = g;
        a.data[x2 + i*a.w + 1*a.w*a.h] = g;

        a.data[x1 + i*a.w + 2*a.w*a.h] = b;
        a.data[x2 + i*a.w + 2*a.w*a.h] = b;
    }
}

शून्य draw_box_width(image a, पूर्णांक x1, पूर्णांक y1, पूर्णांक x2, पूर्णांक y2, पूर्णांक w, दशांश r, दशांश g, दशांश b)
{
    पूर्णांक i;
    साठी(i = 0; i < w; ++i){
        draw_box(a, x1+i, y1+i, x2-i, y2-i, r, g, b);
    }
}

शून्य draw_bbox(image a, box bbox, पूर्णांक w, दशांश r, दशांश g, दशांश b)
{
    पूर्णांक डावा  = (bbox.x-bbox.w/2)*a.w;
    पूर्णांक उजवा = (bbox.x+bbox.w/2)*a.w;
    पूर्णांक शीर्ष   = (bbox.y-bbox.h/2)*a.h;
    पूर्णांक bot   = (bbox.y+bbox.h/2)*a.h;

    पूर्णांक i;
    साठी(i = 0; i < w; ++i){
        draw_box(a, डावा+i, शीर्ष+i, उजवा-i, bot-i, r, g, b);
    }
}

image **load_alphabet()
{
    पूर्णांक i, j;
    स्थिरांक पूर्णांक nsize = 8;
    image **alphabets = सुस्मृति(nsize, आकार(image));
    साठी(j = 0; j < nsize; ++j){
        alphabets[j] = सुस्मृति(128, आकार(image));
        साठी(i = 32; i < 127; ++i){
            अक्षर buff[256];
            प्र_लिखो(buff, "data/labels/%d_%d.png", i, j);
            alphabets[j][i] = load_image_color(buff, 0, 0);
        }
    }
    परत alphabets;
}

शून्य draw_detections(image im, detection *dets, पूर्णांक अंक, दशांश thresh, अक्षर **names, image **alphabet, पूर्णांक classes)
{
    पूर्णांक i,j;

    साठी(i = 0; i < अंक; ++i){
        अक्षर labelstr[4096] = {0};
        पूर्णांक class = -1;
        साठी(j = 0; j < classes; ++j){
            जर (dets[i].prob[j] > thresh){
                जर (class < 0) {
                    म_जोड़ो(labelstr, names[j]);
                    class = j;
                } अन्यथा {
                    म_जोड़ो(labelstr, ", ");
                    म_जोड़ो(labelstr, names[j]);
                }
                म_लिखो("%s: %.0f%%\n", names[j], dets[i].prob[j]*100);
            }
        }
        जर(class >= 0){
            पूर्णांक width = im.h * .006;

            /*
               if(0){
               width = pow(prob, 1./2.)*10+1;
               alphabet = 0;
               }
             */

            //printf("%d %s: %.0f%%\n", i, names[class], prob*100);
            पूर्णांक offset = class*123457 % classes;
            दशांश red = get_color(2,offset,classes);
            दशांश green = get_color(1,offset,classes);
            दशांश blue = get_color(0,offset,classes);
            दशांश rgb[3];

            //width = prob*20+2;

            rgb[0] = red;
            rgb[1] = green;
            rgb[2] = blue;
            box b = dets[i].bbox;
            //printf("%f %f %f %f\n", b.x, b.y, b.w, b.h);

            पूर्णांक डावा  = (b.x-b.w/2.)*im.w;
            पूर्णांक उजवा = (b.x+b.w/2.)*im.w;
            पूर्णांक शीर्ष   = (b.y-b.h/2.)*im.h;
            पूर्णांक bot   = (b.y+b.h/2.)*im.h;

            जर(डावा < 0) डावा = 0;
            जर(उजवा > im.w-1) उजवा = im.w-1;
            जर(शीर्ष < 0) शीर्ष = 0;
            जर(bot > im.h-1) bot = im.h-1;

            draw_box_width(im, डावा, शीर्ष, उजवा, bot, width, red, green, blue);
            जर (alphabet) {
                image label = get_label(alphabet, labelstr, (im.h*.03));
                draw_label(im, शीर्ष + width, डावा, label, rgb);
                free_image(label);
            }
            जर (dets[i].mask){
                image mask = float_to_image(14, 14, 1, dets[i].mask);
                image resized_mask = resize_image(mask, b.w*im.w, b.h*im.h);
                image tmask = threshold_image(resized_mask, .5);
                embed_image(tmask, im, डावा, शीर्ष);
                free_image(mask);
                free_image(resized_mask);
                free_image(tmask);
            }
        }
    }
}

शून्य transpose_image(image im)
{
    प्रतिज्ञा(im.w == im.h);
    पूर्णांक n, m;
    पूर्णांक c;
    साठी(c = 0; c < im.c; ++c){
        साठी(n = 0; n < im.w-1; ++n){
            साठी(m = n + 1; m < im.w; ++m){
                दशांश swap = im.data[m + im.w*(n + im.h*c)];
                im.data[m + im.w*(n + im.h*c)] = im.data[n + im.w*(m + im.h*c)];
                im.data[n + im.w*(m + im.h*c)] = swap;
            }
        }
    }
}

शून्य rotate_image_cw(image im, पूर्णांक बार)
{
    प्रतिज्ञा(im.w == im.h);
    बार = (बार + 400) % 4;
    पूर्णांक i, x, y, c;
    पूर्णांक n = im.w;
    साठी(i = 0; i < बार; ++i){
        साठी(c = 0; c < im.c; ++c){
            साठी(x = 0; x < n/2; ++x){
                साठी(y = 0; y < (n-1)/2 + 1; ++y){
                    दशांश temp = im.data[y + im.w*(x + im.h*c)];
                    im.data[y + im.w*(x + im.h*c)] = im.data[n-1-x + im.w*(y + im.h*c)];
                    im.data[n-1-x + im.w*(y + im.h*c)] = im.data[n-1-y + im.w*(n-1-x + im.h*c)];
                    im.data[n-1-y + im.w*(n-1-x + im.h*c)] = im.data[x + im.w*(n-1-y + im.h*c)];
                    im.data[x + im.w*(n-1-y + im.h*c)] = temp;
                }
            }
        }
    }
}

शून्य flip_image(image a)
{
    पूर्णांक i,j,k;
    साठी(k = 0; k < a.c; ++k){
        साठी(i = 0; i < a.h; ++i){
            साठी(j = 0; j < a.w/2; ++j){
                पूर्णांक index = j + a.w*(i + a.h*(k));
                पूर्णांक flip = (a.w - j - 1) + a.w*(i + a.h*(k));
                दशांश swap = a.data[flip];
                a.data[flip] = a.data[index];
                a.data[index] = swap;
            }
        }
    }
}

image image_distance(image a, image b)
{
    पूर्णांक i,j;
    image dist = make_image(a.w, a.h, 1);
    साठी(i = 0; i < a.c; ++i){
        साठी(j = 0; j < a.h*a.w; ++j){
            dist.data[j] += घात(a.data[i*a.h*a.w+j]-b.data[i*a.h*a.w+j],2);
        }
    }
    साठी(j = 0; j < a.h*a.w; ++j){
        dist.data[j] = वर्ग_मूल(dist.data[j]);
    }
    परत dist;
}

शून्य ghost_image(image source, image dest, पूर्णांक dx, पूर्णांक dy)
{
    पूर्णांक x,y,k;
    दशांश max_dist = वर्ग_मूल((-source.w/2. + .5)*(-source.w/2. + .5));
    साठी(k = 0; k < source.c; ++k){
        साठी(y = 0; y < source.h; ++y){
            साठी(x = 0; x < source.w; ++x){
                दशांश dist = वर्ग_मूल((x - source.w/2. + .5)*(x - source.w/2. + .5) + (y - source.h/2. + .5)*(y - source.h/2. + .5));
                दशांश alpha = (1 - dist/max_dist);
                जर(alpha < 0) alpha = 0;
                दशांश v1 = get_pixel(source, x,y,k);
                दशांश v2 = get_pixel(dest, dx+x,dy+y,k);
                दशांश val = alpha*v1 + (1-alpha)*v2;
                set_pixel(dest, dx+x, dy+y, k, val);
            }
        }
    }
}

शून्य blocky_image(image im, पूर्णांक s)
{
    पूर्णांक i,j,k;
    साठी(k = 0; k < im.c; ++k){
        साठी(j = 0; j < im.h; ++j){
            साठी(i = 0; i < im.w; ++i){
                im.data[i + im.w*(j + im.h*k)] = im.data[i/s*s + im.w*(j/s*s + im.h*k)];
            }
        }
    }
}

शून्य censor_image(image im, पूर्णांक dx, पूर्णांक dy, पूर्णांक w, पूर्णांक h)
{
    पूर्णांक i,j,k;
    पूर्णांक s = 32;
    जर(dx < 0) dx = 0;
    जर(dy < 0) dy = 0;

    साठी(k = 0; k < im.c; ++k){
        साठी(j = dy; j < dy + h && j < im.h; ++j){
            साठी(i = dx; i < dx + w && i < im.w; ++i){
                im.data[i + im.w*(j + im.h*k)] = im.data[i/s*s + im.w*(j/s*s + im.h*k)];
                //im.data[i + j*im.w + k*im.w*im.h] = 0;
            }
        }
    }
}

शून्य embed_image(image source, image dest, पूर्णांक dx, पूर्णांक dy)
{
    पूर्णांक x,y,k;
    साठी(k = 0; k < source.c; ++k){
        साठी(y = 0; y < source.h; ++y){
            साठी(x = 0; x < source.w; ++x){
                दशांश val = get_pixel(source, x,y,k);
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
    साठी(i = 0; i < source.c; ++i){
        image layer = get_image_layer(source, i);
        पूर्णांक h_offset = i*(source.h+border);
        embed_image(layer, dest, 0, h_offset);
        free_image(layer);
    }
    परत dest;
}

शून्य constrain_image(image im)
{
    पूर्णांक i;
    साठी(i = 0; i < im.w*im.h*im.c; ++i){
        जर(im.data[i] < 0) im.data[i] = 0;
        जर(im.data[i] > 1) im.data[i] = 1;
    }
}

शून्य normalize_image(image p)
{
    पूर्णांक i;
    दशांश min = 9999999;
    दशांश max = -999999;

    साठी(i = 0; i < p.h*p.w*p.c; ++i){
        दशांश v = p.data[i];
        जर(v < min) min = v;
        जर(v > max) max = v;
    }
    जर(max - min < .000000001){
        min = 0;
        max = 1;
    }
    साठी(i = 0; i < p.c*p.w*p.h; ++i){
        p.data[i] = (p.data[i] - min)/(max-min);
    }
}

शून्य normalize_image2(image p)
{
    दशांश *min = सुस्मृति(p.c, आकार(दशांश));
    दशांश *max = सुस्मृति(p.c, आकार(दशांश));
    पूर्णांक i,j;
    साठी(i = 0; i < p.c; ++i) min[i] = max[i] = p.data[i*p.h*p.w];

    साठी(j = 0; j < p.c; ++j){
        साठी(i = 0; i < p.h*p.w; ++i){
            दशांश v = p.data[i+j*p.h*p.w];
            जर(v < min[j]) min[j] = v;
            जर(v > max[j]) max[j] = v;
        }
    }
    साठी(i = 0; i < p.c; ++i){
        जर(max[i] - min[i] < .000000001){
            min[i] = 0;
            max[i] = 1;
        }
    }
    साठी(j = 0; j < p.c; ++j){
        साठी(i = 0; i < p.w*p.h; ++i){
            p.data[i+j*p.h*p.w] = (p.data[i+j*p.h*p.w] - min[j])/(max[j]-min[j]);
        }
    }
    मुक्त(min);
    मुक्त(max);
}

शून्य copy_image_into(image src, image dest)
{
    स_नकल(dest.data, src.data, src.h*src.w*src.c*आकार(दशांश));
}

image copy_image(image p)
{
    image copy = p;
    copy.data = सुस्मृति(p.h*p.w*p.c, आकार(दशांश));
    स_नकल(copy.data, p.data, p.h*p.w*p.c*आकार(दशांश));
    परत copy;
}

शून्य rgbgr_image(image im)
{
    पूर्णांक i;
    साठी(i = 0; i < im.w*im.h; ++i){
        दशांश swap = im.data[i];
        im.data[i] = im.data[i+im.w*im.h*2];
        im.data[i+im.w*im.h*2] = swap;
    }
}

पूर्णांक show_image(image p, स्थिरांक अक्षर *name, पूर्णांक ms)
{
#ifdef OPENCV
    पूर्णांक c = show_image_cv(p, name, ms);
    परत c;
#अन्यथा
    ख_लिखो(मानक_त्रुटि, "Not compiled with OpenCV, saving to %s.png instead\n", name);
    save_image(p, name);
    परत -1;
#endif
}

शून्य save_image_options(image im, स्थिरांक अक्षर *name, IMTYPE f, पूर्णांक quality)
{
    अक्षर buff[256];
    //sprintf(buff, "%s (%d)", name, windows);
    जर(f == PNG)       प्र_लिखो(buff, "%s.png", name);
    अन्यथा जर (f == BMP) प्र_लिखो(buff, "%s.bmp", name);
    अन्यथा जर (f == TGA) प्र_लिखो(buff, "%s.tga", name);
    अन्यथा जर (f == JPG) प्र_लिखो(buff, "%s.jpg", name);
    अन्यथा               प्र_लिखो(buff, "%s.png", name);
    अचिन्हित अक्षर *data = सुस्मृति(im.w*im.h*im.c, आकार(अक्षर));
    पूर्णांक i,k;
    साठी(k = 0; k < im.c; ++k){
        साठी(i = 0; i < im.w*im.h; ++i){
            data[i*im.c+k] = (अचिन्हित अक्षर) (255*im.data[i + k*im.w*im.h]);
        }
    }
    पूर्णांक success = 0;
    जर(f == PNG)       success = stbi_write_png(buff, im.w, im.h, im.c, data, im.w*im.c);
    अन्यथा जर (f == BMP) success = stbi_write_bmp(buff, im.w, im.h, im.c, data);
    अन्यथा जर (f == TGA) success = stbi_write_tga(buff, im.w, im.h, im.c, data);
    अन्यथा जर (f == JPG) success = stbi_write_jpg(buff, im.w, im.h, im.c, data, quality);
    मुक्त(data);
    जर(!success) ख_लिखो(मानक_त्रुटि, "Failed to write image %s\n", buff);
}

शून्य save_image(image im, स्थिरांक अक्षर *name)
{
    save_image_options(im, name, JPG, 80);
}

शून्य show_image_layers(image p, अक्षर *name)
{
    पूर्णांक i;
    अक्षर buff[256];
    साठी(i = 0; i < p.c; ++i){
        प्र_लिखो(buff, "%s - Layer %d", name, i);
        image layer = get_image_layer(p, i);
        show_image(layer, buff, 1);
        free_image(layer);
    }
}

शून्य show_image_collapsed(image p, अक्षर *name)
{
    image c = collapse_image_layers(p, 1);
    show_image(c, name, 1);
    free_image(c);
}

image make_empty_image(पूर्णांक w, पूर्णांक h, पूर्णांक c)
{
    image out;
    out.data = 0;
    out.h = h;
    out.w = w;
    out.c = c;
    परत out;
}

image make_image(पूर्णांक w, पूर्णांक h, पूर्णांक c)
{
    image out = make_empty_image(w,h,c);
    out.data = सुस्मृति(h*w*c, आकार(दशांश));
    परत out;
}

image make_random_image(पूर्णांक w, पूर्णांक h, पूर्णांक c)
{
    image out = make_empty_image(w,h,c);
    out.data = सुस्मृति(h*w*c, आकार(दशांश));
    पूर्णांक i;
    साठी(i = 0; i < w*h*c; ++i){
        out.data[i] = (rand_normal() * .25) + .5;
    }
    परत out;
}

image float_to_image(पूर्णांक w, पूर्णांक h, पूर्णांक c, दशांश *data)
{
    image out = make_empty_image(w,h,c);
    out.data = data;
    परत out;
}

शून्य place_image(image im, पूर्णांक w, पूर्णांक h, पूर्णांक dx, पूर्णांक dy, image canvas)
{
    पूर्णांक x, y, c;
    साठी(c = 0; c < im.c; ++c){
        साठी(y = 0; y < h; ++y){
            साठी(x = 0; x < w; ++x){
                दशांश rx = ((दशांश)x / w) * im.w;
                दशांश ry = ((दशांश)y / h) * im.h;
                दशांश val = bilinear_interpolate(im, rx, ry, c);
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
    परत r;
}

image rotate_crop_image(image im, दशांश rad, दशांश s, पूर्णांक w, पूर्णांक h, दशांश dx, दशांश dy, दशांश aspect)
{
    पूर्णांक x, y, c;
    दशांश cx = im.w/2.;
    दशांश cy = im.h/2.;
    image rot = make_image(w, h, im.c);
    साठी(c = 0; c < im.c; ++c){
        साठी(y = 0; y < h; ++y){
            साठी(x = 0; x < w; ++x){
                दशांश rx = cos(rad)*((x - w/2.)/s*aspect + dx/s*aspect) - sin(rad)*((y - h/2.)/s + dy/s) + cx;
                दशांश ry = sin(rad)*((x - w/2.)/s*aspect + dx/s*aspect) + cos(rad)*((y - h/2.)/s + dy/s) + cy;
                दशांश val = bilinear_interpolate(im, rx, ry, c);
                set_pixel(rot, x, y, c, val);
            }
        }
    }
    परत rot;
}

image rotate_image(image im, दशांश rad)
{
    पूर्णांक x, y, c;
    दशांश cx = im.w/2.;
    दशांश cy = im.h/2.;
    image rot = make_image(im.w, im.h, im.c);
    साठी(c = 0; c < im.c; ++c){
        साठी(y = 0; y < im.h; ++y){
            साठी(x = 0; x < im.w; ++x){
                दशांश rx = cos(rad)*(x-cx) - sin(rad)*(y-cy) + cx;
                दशांश ry = sin(rad)*(x-cx) + cos(rad)*(y-cy) + cy;
                दशांश val = bilinear_interpolate(im, rx, ry, c);
                set_pixel(rot, x, y, c, val);
            }
        }
    }
    परत rot;
}

शून्य fill_image(image m, दशांश s)
{
    पूर्णांक i;
    साठी(i = 0; i < m.h*m.w*m.c; ++i) m.data[i] = s;
}

शून्य translate_image(image m, दशांश s)
{
    पूर्णांक i;
    साठी(i = 0; i < m.h*m.w*m.c; ++i) m.data[i] += s;
}

शून्य scale_image(image m, दशांश s)
{
    पूर्णांक i;
    साठी(i = 0; i < m.h*m.w*m.c; ++i) m.data[i] *= s;
}

image crop_image(image im, पूर्णांक dx, पूर्णांक dy, पूर्णांक w, पूर्णांक h)
{
    image cropped = make_image(w, h, im.c);
    पूर्णांक i, j, k;
    साठी(k = 0; k < im.c; ++k){
        साठी(j = 0; j < h; ++j){
            साठी(i = 0; i < w; ++i){
                पूर्णांक r = j + dy;
                पूर्णांक c = i + dx;
                दशांश val = 0;
                r = constrain_int(r, 0, im.h-1);
                c = constrain_int(c, 0, im.w-1);
                val = get_pixel(im, c, r, k);
                set_pixel(cropped, i, j, k, val);
            }
        }
    }
    परत cropped;
}

पूर्णांक best_3d_shift_r(image a, image b, पूर्णांक min, पूर्णांक max)
{
    जर(min == max) परत min;
    पूर्णांक mid = न्यूनमान((min + max) / 2.);
    image c1 = crop_image(b, 0, mid, b.w, b.h);
    image c2 = crop_image(b, 0, mid+1, b.w, b.h);
    दशांश d1 = dist_array(c1.data, a.data, a.w*a.h*a.c, 10);
    दशांश d2 = dist_array(c2.data, a.data, a.w*a.h*a.c, 10);
    free_image(c1);
    free_image(c2);
    जर(d1 < d2) परत best_3d_shift_r(a, b, min, mid);
    अन्यथा परत best_3d_shift_r(a, b, mid+1, max);
}

पूर्णांक best_3d_shift(image a, image b, पूर्णांक min, पूर्णांक max)
{
    पूर्णांक i;
    पूर्णांक best = 0;
    दशांश best_distance = भग्न_उच्च;
    साठी(i = min; i <= max; i += 2){
        image c = crop_image(b, 0, i, b.w, b.h);
        दशांश d = dist_array(c.data, a.data, a.w*a.h*a.c, 100);
        जर(d < best_distance){
            best_distance = d;
            best = i;
        }
        म_लिखो("%d %f\n", i, d);
        free_image(c);
    }
    परत best;
}

शून्य composite_3d(अक्षर *f1, अक्षर *f2, अक्षर *out, पूर्णांक delta)
{
    जर(!out) out = "out";
    image a = load_image(f1, 0,0,0);
    image b = load_image(f2, 0,0,0);
    पूर्णांक shift = best_3d_shift_r(a, b, -a.h/100, a.h/100);

    image c1 = crop_image(b, 10, shift, b.w, b.h);
    दशांश d1 = dist_array(c1.data, a.data, a.w*a.h*a.c, 100);
    image c2 = crop_image(b, -10, shift, b.w, b.h);
    दशांश d2 = dist_array(c2.data, a.data, a.w*a.h*a.c, 100);

    जर(d2 < d1 && 0){
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
    साठी(i = 0; i < c.w*c.h; ++i){
        c.data[i] = a.data[i];
    }
    save_image(c, out);
}

शून्य letterbox_image_into(image im, पूर्णांक w, पूर्णांक h, image boxed)
{
    पूर्णांक new_w = im.w;
    पूर्णांक new_h = im.h;
    जर (((दशांश)w/im.w) < ((दशांश)h/im.h)) {
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
    जर (((दशांश)w/im.w) < ((दशांश)h/im.h)) {
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
    परत boxed;
}

image resize_max(image im, पूर्णांक max)
{
    पूर्णांक w = im.w;
    पूर्णांक h = im.h;
    जर(w > h){
        h = (h * max) / w;
        w = max;
    } अन्यथा {
        w = (w * max) / h;
        h = max;
    }
    जर(w == im.w && h == im.h) परत im;
    image resized = resize_image(im, w, h);
    परत resized;
}

image resize_min(image im, पूर्णांक min)
{
    पूर्णांक w = im.w;
    पूर्णांक h = im.h;
    जर(w < h){
        h = (h * min) / w;
        w = min;
    } अन्यथा {
        w = (w * min) / h;
        h = min;
    }
    जर(w == im.w && h == im.h) परत im;
    image resized = resize_image(im, w, h);
    परत resized;
}

image random_crop_image(image im, पूर्णांक w, पूर्णांक h)
{
    पूर्णांक dx = rand_int(0, im.w - w);
    पूर्णांक dy = rand_int(0, im.h - h);
    image crop = crop_image(im, dx, dy, w, h);
    परत crop;
}

augment_args random_augment_args(image im, दशांश angle, दशांश aspect, पूर्णांक खालचा, पूर्णांक वरचा, पूर्णांक w, पूर्णांक h)
{
    augment_args a = {0};
    aspect = rand_scale(aspect);
    पूर्णांक r = rand_int(खालचा, वरचा);
    पूर्णांक min = (im.h < im.w*aspect) ? im.h : im.w*aspect;
    दशांश scale = (दशांश)r / min;

    दशांश rad = rand_uniform(-angle, angle) * TWO_PI / 360.;

    दशांश dx = (im.w*scale/aspect - w) / 2.;
    दशांश dy = (im.h*scale - w) / 2.;
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
    परत a;
}

image random_augment_image(image im, दशांश angle, दशांश aspect, पूर्णांक खालचा, पूर्णांक वरचा, पूर्णांक w, पूर्णांक h)
{
    augment_args a = random_augment_args(im, angle, aspect, खालचा, वरचा, w, h);
    image crop = rotate_crop_image(im, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);
    परत crop;
}

दशांश three_way_max(दशांश a, दशांश b, दशांश c)
{
    परत (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

दशांश three_way_min(दशांश a, दशांश b, दशांश c)
{
    परत (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

शून्य yuv_to_rgb(image im)
{
    प्रतिज्ञा(im.c == 3);
    पूर्णांक i, j;
    दशांश r, g, b;
    दशांश y, u, v;
    साठी(j = 0; j < im.h; ++j){
        साठी(i = 0; i < im.w; ++i){
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

शून्य rgb_to_yuv(image im)
{
    प्रतिज्ञा(im.c == 3);
    पूर्णांक i, j;
    दशांश r, g, b;
    दशांश y, u, v;
    साठी(j = 0; j < im.h; ++j){
        साठी(i = 0; i < im.w; ++i){
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
शून्य rgb_to_hsv(image im)
{
    प्रतिज्ञा(im.c == 3);
    पूर्णांक i, j;
    दशांश r, g, b;
    दशांश h, s, v;
    साठी(j = 0; j < im.h; ++j){
        साठी(i = 0; i < im.w; ++i){
            r = get_pixel(im, i , j, 0);
            g = get_pixel(im, i , j, 1);
            b = get_pixel(im, i , j, 2);
            दशांश max = three_way_max(r,g,b);
            दशांश min = three_way_min(r,g,b);
            दशांश delta = max - min;
            v = max;
            जर(max == 0){
                s = 0;
                h = 0;
            }अन्यथा{
                s = delta/max;
                जर(r == max){
                    h = (g - b) / delta;
                } अन्यथा जर (g == max) {
                    h = 2 + (b - r) / delta;
                } अन्यथा {
                    h = 4 + (r - g) / delta;
                }
                जर (h < 0) h += 6;
                h = h/6.;
            }
            set_pixel(im, i, j, 0, h);
            set_pixel(im, i, j, 1, s);
            set_pixel(im, i, j, 2, v);
        }
    }
}

शून्य hsv_to_rgb(image im)
{
    प्रतिज्ञा(im.c == 3);
    पूर्णांक i, j;
    दशांश r, g, b;
    दशांश h, s, v;
    दशांश f, p, q, t;
    साठी(j = 0; j < im.h; ++j){
        साठी(i = 0; i < im.w; ++i){
            h = 6 * get_pixel(im, i , j, 0);
            s = get_pixel(im, i , j, 1);
            v = get_pixel(im, i , j, 2);
            जर (s == 0) {
                r = g = b = v;
            } अन्यथा {
                पूर्णांक index = न्यूनमान(h);
                f = h - index;
                p = v*(1-s);
                q = v*(1-s*f);
                t = v*(1-s*(1-f));
                जर(index == 0){
                    r = v; g = t; b = p;
                } अन्यथा जर(index == 1){
                    r = q; g = v; b = p;
                } अन्यथा जर(index == 2){
                    r = p; g = v; b = t;
                } अन्यथा जर(index == 3){
                    r = p; g = q; b = v;
                } अन्यथा जर(index == 4){
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

शून्य grayscale_image_3c(image im)
{
    प्रतिज्ञा(im.c == 3);
    पूर्णांक i, j, k;
    दशांश scale[] = {0.299, 0.587, 0.114};
    साठी(j = 0; j < im.h; ++j){
        साठी(i = 0; i < im.w; ++i){
            दशांश val = 0;
            साठी(k = 0; k < 3; ++k){
                val += scale[k]*get_pixel(im, i, j, k);
            }
            im.data[0*im.h*im.w + im.w*j + i] = val;
            im.data[1*im.h*im.w + im.w*j + i] = val;
            im.data[2*im.h*im.w + im.w*j + i] = val;
        }
    }
}

image grayscale_image(image im)
{
    प्रतिज्ञा(im.c == 3);
    पूर्णांक i, j, k;
    image gray = make_image(im.w, im.h, 1);
    दशांश scale[] = {0.299, 0.587, 0.114};
    साठी(k = 0; k < im.c; ++k){
        साठी(j = 0; j < im.h; ++j){
            साठी(i = 0; i < im.w; ++i){
                gray.data[i+im.w*j] += scale[k]*get_pixel(im, i, j, k);
            }
        }
    }
    परत gray;
}

image threshold_image(image im, दशांश thresh)
{
    पूर्णांक i;
    image t = make_image(im.w, im.h, im.c);
    साठी(i = 0; i < im.w*im.h*im.c; ++i){
        t.data[i] = im.data[i]>thresh ? 1 : 0;
    }
    परत t;
}

image blend_image(image fore, image back, दशांश alpha)
{
    प्रतिज्ञा(fore.w == back.w && fore.h == back.h && fore.c == back.c);
    image blend = make_image(fore.w, fore.h, fore.c);
    पूर्णांक i, j, k;
    साठी(k = 0; k < fore.c; ++k){
        साठी(j = 0; j < fore.h; ++j){
            साठी(i = 0; i < fore.w; ++i){
                दशांश val = alpha * get_pixel(fore, i, j, k) + 
                    (1 - alpha)* get_pixel(back, i, j, k);
                set_pixel(blend, i, j, k, val);
            }
        }
    }
    परत blend;
}

शून्य scale_image_channel(image im, पूर्णांक c, दशांश v)
{
    पूर्णांक i, j;
    साठी(j = 0; j < im.h; ++j){
        साठी(i = 0; i < im.w; ++i){
            दशांश pix = get_pixel(im, i, j, c);
            pix = pix*v;
            set_pixel(im, i, j, c, pix);
        }
    }
}

शून्य translate_image_channel(image im, पूर्णांक c, दशांश v)
{
    पूर्णांक i, j;
    साठी(j = 0; j < im.h; ++j){
        साठी(i = 0; i < im.w; ++i){
            दशांश pix = get_pixel(im, i, j, c);
            pix = pix+v;
            set_pixel(im, i, j, c, pix);
        }
    }
}

image binarize_image(image im)
{
    image c = copy_image(im);
    पूर्णांक i;
    साठी(i = 0; i < im.w * im.h * im.c; ++i){
        जर(c.data[i] > .5) c.data[i] = 1;
        अन्यथा c.data[i] = 0;
    }
    परत c;
}

शून्य saturate_image(image im, दशांश sat)
{
    rgb_to_hsv(im);
    scale_image_channel(im, 1, sat);
    hsv_to_rgb(im);
    constrain_image(im);
}

शून्य hue_image(image im, दशांश hue)
{
    rgb_to_hsv(im);
    पूर्णांक i;
    साठी(i = 0; i < im.w*im.h; ++i){
        im.data[i] = im.data[i] + hue;
        जर (im.data[i] > 1) im.data[i] -= 1;
        जर (im.data[i] < 0) im.data[i] += 1;
    }
    hsv_to_rgb(im);
    constrain_image(im);
}

शून्य exposure_image(image im, दशांश sat)
{
    rgb_to_hsv(im);
    scale_image_channel(im, 2, sat);
    hsv_to_rgb(im);
    constrain_image(im);
}

शून्य distort_image(image im, दशांश hue, दशांश sat, दशांश val)
{
    rgb_to_hsv(im);
    scale_image_channel(im, 1, sat);
    scale_image_channel(im, 2, val);
    पूर्णांक i;
    साठी(i = 0; i < im.w*im.h; ++i){
        im.data[i] = im.data[i] + hue;
        जर (im.data[i] > 1) im.data[i] -= 1;
        जर (im.data[i] < 0) im.data[i] += 1;
    }
    hsv_to_rgb(im);
    constrain_image(im);
}

शून्य random_distort_image(image im, दशांश hue, दशांश saturation, दशांश exposure)
{
    दशांश dhue = rand_uniform(-hue, hue);
    दशांश dsat = rand_scale(saturation);
    दशांश dexp = rand_scale(exposure);
    distort_image(im, dhue, dsat, dexp);
}

शून्य saturate_exposure_image(image im, दशांश sat, दशांश exposure)
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
    दशांश w_scale = (दशांश)(im.w - 1) / (w - 1);
    दशांश h_scale = (दशांश)(im.h - 1) / (h - 1);
    साठी(k = 0; k < im.c; ++k){
        साठी(r = 0; r < im.h; ++r){
            साठी(c = 0; c < w; ++c){
                दशांश val = 0;
                जर(c == w-1 || im.w == 1){
                    val = get_pixel(im, im.w-1, r, k);
                } अन्यथा {
                    दशांश sx = c*w_scale;
                    पूर्णांक ix = (पूर्णांक) sx;
                    दशांश dx = sx - ix;
                    val = (1 - dx) * get_pixel(im, ix, r, k) + dx * get_pixel(im, ix+1, r, k);
                }
                set_pixel(part, c, r, k, val);
            }
        }
    }
    साठी(k = 0; k < im.c; ++k){
        साठी(r = 0; r < h; ++r){
            दशांश sy = r*h_scale;
            पूर्णांक iy = (पूर्णांक) sy;
            दशांश dy = sy - iy;
            साठी(c = 0; c < w; ++c){
                दशांश val = (1-dy) * get_pixel(part, c, iy, k);
                set_pixel(resized, c, r, k, val);
            }
            जर(r == h-1 || im.h == 1) सुरूठेवा;
            साठी(c = 0; c < w; ++c){
                दशांश val = dy * get_pixel(part, c, iy+1, k);
                add_pixel(resized, c, r, k, val);
            }
        }
    }

    free_image(part);
    परत resized;
}


शून्य test_resize(अक्षर *filename)
{
    image im = load_image(filename, 0,0, 3);
    दशांश mag = mag_array(im.data, im.w*im.h*im.c);
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
#ifdef OPENCV
    पर्यंत(1){
        image aug = random_augment_image(im, 0, .75, 320, 448, 320, 320);
        show_image(aug, "aug", 1);
        free_image(aug);


        दशांश exposure = 1.15;
        दशांश saturation = 1.15;
        दशांश hue = .05;

        image c = copy_image(im);

        दशांश dexp = rand_scale(exposure);
        दशांश dsat = rand_scale(saturation);
        दशांश dhue = rand_uniform(-hue, hue);

        distort_image(c, dhue, dsat, dexp);
        show_image(c, "rand", 1);
        म_लिखो("%f %f %f\n", dhue, dsat, dexp);
        free_image(c);
    }
#endif
}


image load_image_stb(अक्षर *filename, पूर्णांक channels)
{
    पूर्णांक w, h, c;
    अचिन्हित अक्षर *data = stbi_load(filename, &w, &h, &c, channels);
    जर (!data) {
        ख_लिखो(मानक_त्रुटि, "Cannot load image \"%s\"\nSTB Reason: %s\n", filename, stbi_failure_reason());
        निकास(0);
    }
    जर(channels) c = channels;
    पूर्णांक i,j,k;
    image im = make_image(w, h, c);
    साठी(k = 0; k < c; ++k){
        साठी(j = 0; j < h; ++j){
            साठी(i = 0; i < w; ++i){
                पूर्णांक dst_index = i + w*j + w*h*k;
                पूर्णांक src_index = k + c*i + c*w*j;
                im.data[dst_index] = (दशांश)data[src_index]/255.;
            }
        }
    }
    मुक्त(data);
    परत im;
}

image load_image(अक्षर *filename, पूर्णांक w, पूर्णांक h, पूर्णांक c)
{
#ifdef OPENCV
    image out = load_image_cv(filename, c);
#अन्यथा
    image out = load_image_stb(filename, c);
#endif

    जर((h && w) && (h != out.h || w != out.w)){
        image resized = resize_image(out, w, h);
        free_image(out);
        out = resized;
    }
    परत out;
}

image load_image_color(अक्षर *filename, पूर्णांक w, पूर्णांक h)
{
    परत load_image(filename, w, h, 3);
}

image get_image_layer(image m, पूर्णांक l)
{
    image out = make_image(m.w, m.h, 1);
    पूर्णांक i;
    साठी(i = 0; i < m.h*m.w; ++i){
        out.data[i] = m.data[i+l*m.h*m.w];
    }
    परत out;
}
शून्य print_image(image m)
{
    पूर्णांक i, j, k;
    साठी(i =0 ; i < m.c; ++i){
        साठी(j =0 ; j < m.h; ++j){
            साठी(k = 0; k < m.w; ++k){
                म_लिखो("%.2lf, ", m.data[i*m.h*m.w + j*m.w + k]);
                जर(k > 30) थांबा;
            }
            म_लिखो("\n");
            जर(j > 30) थांबा;
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
    जर(c != 3 || !color){
        w = (w+border)*c - border;
        c = 1;
    }

    image filters = make_image(w, h, c);
    पूर्णांक i,j;
    साठी(i = 0; i < n; ++i){
        पूर्णांक h_offset = i*(ims[0].h+border);
        image copy = copy_image(ims[i]);
        //normalize_image(copy);
        जर(c == 3 && color){
            embed_image(copy, filters, 0, h_offset);
        }
        अन्यथा{
            साठी(j = 0; j < copy.c; ++j){
                पूर्णांक w_offset = j*(ims[0].w+border);
                image layer = get_image_layer(copy, j);
                embed_image(layer, filters, w_offset, h_offset);
                free_image(layer);
            }
        }
        free_image(copy);
    }
    परत filters;
} 

image collapse_images_horz(image *ims, पूर्णांक n)
{
    पूर्णांक color = 1;
    पूर्णांक border = 1;
    पूर्णांक h,w,c;
    पूर्णांक size = ims[0].h;
    h = size;
    w = (ims[0].w + border) * n - border;
    c = ims[0].c;
    जर(c != 3 || !color){
        h = (h+border)*c - border;
        c = 1;
    }

    image filters = make_image(w, h, c);
    पूर्णांक i,j;
    साठी(i = 0; i < n; ++i){
        पूर्णांक w_offset = i*(size+border);
        image copy = copy_image(ims[i]);
        //normalize_image(copy);
        जर(c == 3 && color){
            embed_image(copy, filters, w_offset, 0);
        }
        अन्यथा{
            साठी(j = 0; j < copy.c; ++j){
                पूर्णांक h_offset = j*(size+border);
                image layer = get_image_layer(copy, j);
                embed_image(layer, filters, w_offset, h_offset);
                free_image(layer);
            }
        }
        free_image(copy);
    }
    परत filters;
} 

शून्य show_image_normalized(image im, स्थिरांक अक्षर *name)
{
    image c = copy_image(im);
    normalize_image(c);
    show_image(c, name, 1);
    free_image(c);
}

शून्य show_images(image *ims, पूर्णांक n, अक्षर *window)
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

शून्य free_image(image m)
{
    जर(m.data){
        मुक्त(m.data);
    }
}
