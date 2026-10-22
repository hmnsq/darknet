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

पूर्णाङ्क windows = 0;

भिन्न colors[6][3] = { {1,0,1}, {0,0,1},{0,1,1},{0,1,0},{1,1,0},{1,0,0} };

भिन्न get_color(पूर्णाङ्क c, पूर्णाङ्क x, पूर्णाङ्क max)
{
    भिन्न ratio = ((भिन्न)x/max)*5;
    पूर्णाङ्क i = floor(ratio);
    पूर्णाङ्क j = ceil(ratio);
    ratio -= i;
    भिन्न r = (1-ratio) * colors[i][c] + ratio*colors[j][c];
    //printf("%f\n", r);
    प्रत्यागम r;
}

image mask_to_rgb(image mask)
{
    पूर्णाङ्क n = mask.c;
    image im = make_image(mask.w, mask.h, 3);
    पूर्णाङ्क i, j;
    प्रति(j = 0; j < n; ++j){
        पूर्णाङ्क offset = j*123457 % n;
        भिन्न red = get_color(2,offset,n);
        भिन्न green = get_color(1,offset,n);
        भिन्न blue = get_color(0,offset,n);
        प्रति(i = 0; i < im.w*im.h; ++i){
            im.data[i + 0*im.w*im.h] += mask.data[j*im.h*im.w + i]*red;
            im.data[i + 1*im.w*im.h] += mask.data[j*im.h*im.w + i]*green;
            im.data[i + 2*im.w*im.h] += mask.data[j*im.h*im.w + i]*blue;
        }
    }
    प्रत्यागम im;
}

स्थिर भिन्न get_pixel(image m, पूर्णाङ्क x, पूर्णाङ्क y, पूर्णाङ्क c)
{
    प्रतिज्ञा(x < m.w && y < m.h && c < m.c);
    प्रत्यागम m.data[c*m.h*m.w + y*m.w + x];
}
स्थिर भिन्न get_pixel_extend(image m, पूर्णाङ्क x, पूर्णाङ्क y, पूर्णाङ्क c)
{
    यदि(x < 0 || x >= m.w || y < 0 || y >= m.h) प्रत्यागम 0;
    /*
    if(x < 0) x = 0;
    if(x >= m.w) x = m.w-1;
    if(y < 0) y = 0;
    if(y >= m.h) y = m.h-1;
    */
    यदि(c < 0 || c >= m.c) प्रत्यागम 0;
    प्रत्यागम get_pixel(m, x, y, c);
}
स्थिर शून्य set_pixel(image m, पूर्णाङ्क x, पूर्णाङ्क y, पूर्णाङ्क c, भिन्न val)
{
    यदि (x < 0 || y < 0 || c < 0 || x >= m.w || y >= m.h || c >= m.c) प्रत्यागम;
    प्रतिज्ञा(x < m.w && y < m.h && c < m.c);
    m.data[c*m.h*m.w + y*m.w + x] = val;
}
स्थिर शून्य add_pixel(image m, पूर्णाङ्क x, पूर्णाङ्क y, पूर्णाङ्क c, भिन्न val)
{
    प्रतिज्ञा(x < m.w && y < m.h && c < m.c);
    m.data[c*m.h*m.w + y*m.w + x] += val;
}

स्थिर भिन्न bilinear_interpolate(image im, भिन्न x, भिन्न y, पूर्णाङ्क c)
{
    पूर्णाङ्क ix = (पूर्णाङ्क) floorf(x);
    पूर्णाङ्क iy = (पूर्णाङ्क) floorf(y);

    भिन्न dx = x - ix;
    भिन्न dy = y - iy;

    भिन्न val = (1-dy) * (1-dx) * get_pixel_extend(im, ix, iy, c) + 
        dy     * (1-dx) * get_pixel_extend(im, ix, iy+1, c) + 
        (1-dy) *   dx   * get_pixel_extend(im, ix+1, iy, c) +
        dy     *   dx   * get_pixel_extend(im, ix+1, iy+1, c);
    प्रत्यागम val;
}


शून्य composite_image(image source, image dest, पूर्णाङ्क dx, पूर्णाङ्क dy)
{
    पूर्णाङ्क x,y,k;
    प्रति(k = 0; k < source.c; ++k){
        प्रति(y = 0; y < source.h; ++y){
            प्रति(x = 0; x < source.w; ++x){
                भिन्न val = get_pixel(source, x, y, k);
                भिन्न val2 = get_pixel_extend(dest, dx+x, dy+y, k);
                set_pixel(dest, dx+x, dy+y, k, val * val2);
            }
        }
    }
}

image border_image(image a, पूर्णाङ्क border)
{
    image b = make_image(a.w + 2*border, a.h + 2*border, a.c);
    पूर्णाङ्क x,y,k;
    प्रति(k = 0; k < b.c; ++k){
        प्रति(y = 0; y < b.h; ++y){
            प्रति(x = 0; x < b.w; ++x){
                भिन्न val = get_pixel_extend(a, x - border, y - border, k);
                यदि(x - border < 0 || x - border >= a.w || y - border < 0 || y - border >= a.h) val = 1;
                set_pixel(b, x, y, k, val);
            }
        }
    }
    प्रत्यागम b;
}

image tile_images(image a, image b, पूर्णाङ्क dx)
{
    यदि(a.w == 0) प्रत्यागम copy_image(b);
    image c = make_image(a.w + b.w + dx, (a.h > b.h) ? a.h : b.h, (a.c > b.c) ? a.c : b.c);
    fill_cpu(c.w*c.h*c.c, 1, c.data, 1);
    embed_image(a, c, 0, 0); 
    composite_image(b, c, a.w + dx, 0);
    प्रत्यागम c;
}

image get_label(image **characters, वर्ण *string, पूर्णाङ्क size)
{
    size = size/10;
    यदि(size > 7) size = 7;
    image label = make_empty_image(0,0,0);
    यावत्(*string){
        image l = characters[size][(पूर्णाङ्क)*string];
        image n = tile_images(label, l, -size - 1 + (size+1)/2);
        free_image(label);
        label = n;
        ++string;
    }
    image b = border_image(label, label.h*.25);
    free_image(label);
    प्रत्यागम b;
}

शून्य draw_label(image a, पूर्णाङ्क r, पूर्णाङ्क c, image label, ध्रुव भिन्न *rgb)
{
    पूर्णाङ्क w = label.w;
    पूर्णाङ्क h = label.h;
    यदि (r - h >= 0) r = r - h;

    पूर्णाङ्क i, j, k;
    प्रति(j = 0; j < h && j + r < a.h; ++j){
        प्रति(i = 0; i < w && i + c < a.w; ++i){
            प्रति(k = 0; k < label.c; ++k){
                भिन्न val = get_pixel(label, i, j, k);
                set_pixel(a, i+c, j+r, k, rgb[k] * val);
            }
        }
    }
}

शून्य draw_box(image a, पूर्णाङ्क x1, पूर्णाङ्क y1, पूर्णाङ्क x2, पूर्णाङ्क y2, भिन्न r, भिन्न g, भिन्न b)
{
    //normalize_image(a);
    पूर्णाङ्क i;
    यदि(x1 < 0) x1 = 0;
    यदि(x1 >= a.w) x1 = a.w-1;
    यदि(x2 < 0) x2 = 0;
    यदि(x2 >= a.w) x2 = a.w-1;

    यदि(y1 < 0) y1 = 0;
    यदि(y1 >= a.h) y1 = a.h-1;
    यदि(y2 < 0) y2 = 0;
    यदि(y2 >= a.h) y2 = a.h-1;

    प्रति(i = x1; i <= x2; ++i){
        a.data[i + y1*a.w + 0*a.w*a.h] = r;
        a.data[i + y2*a.w + 0*a.w*a.h] = r;

        a.data[i + y1*a.w + 1*a.w*a.h] = g;
        a.data[i + y2*a.w + 1*a.w*a.h] = g;

        a.data[i + y1*a.w + 2*a.w*a.h] = b;
        a.data[i + y2*a.w + 2*a.w*a.h] = b;
    }
    प्रति(i = y1; i <= y2; ++i){
        a.data[x1 + i*a.w + 0*a.w*a.h] = r;
        a.data[x2 + i*a.w + 0*a.w*a.h] = r;

        a.data[x1 + i*a.w + 1*a.w*a.h] = g;
        a.data[x2 + i*a.w + 1*a.w*a.h] = g;

        a.data[x1 + i*a.w + 2*a.w*a.h] = b;
        a.data[x2 + i*a.w + 2*a.w*a.h] = b;
    }
}

शून्य draw_box_width(image a, पूर्णाङ्क x1, पूर्णाङ्क y1, पूर्णाङ्क x2, पूर्णाङ्क y2, पूर्णाङ्क w, भिन्न r, भिन्न g, भिन्न b)
{
    पूर्णाङ्क i;
    प्रति(i = 0; i < w; ++i){
        draw_box(a, x1+i, y1+i, x2-i, y2-i, r, g, b);
    }
}

शून्य draw_bbox(image a, box bbox, पूर्णाङ्क w, भिन्न r, भिन्न g, भिन्न b)
{
    पूर्णाङ्क डावा  = (bbox.x-bbox.w/2)*a.w;
    पूर्णाङ्क उजवा = (bbox.x+bbox.w/2)*a.w;
    पूर्णाङ्क शीर्ष   = (bbox.y-bbox.h/2)*a.h;
    पूर्णाङ्क bot   = (bbox.y+bbox.h/2)*a.h;

    पूर्णाङ्क i;
    प्रति(i = 0; i < w; ++i){
        draw_box(a, डावा+i, शीर्ष+i, उजवा-i, bot-i, r, g, b);
    }
}

image **load_alphabet()
{
    पूर्णाङ्क i, j;
    ध्रुव पूर्णाङ्क nsize = 8;
    image **alphabets = calloc(nsize, परिमाण(image));
    प्रति(j = 0; j < nsize; ++j){
        alphabets[j] = calloc(128, परिमाण(image));
        प्रति(i = 32; i < 127; ++i){
            वर्ण buff[256];
            sprintf(buff, "data/labels/%d_%d.png", i, j);
            alphabets[j][i] = load_image_color(buff, 0, 0);
        }
    }
    प्रत्यागम alphabets;
}

शून्य draw_detections(image im, detection *dets, पूर्णाङ्क अंक, भिन्न thresh, वर्ण **names, image **alphabet, पूर्णाङ्क classes)
{
    पूर्णाङ्क i,j;

    प्रति(i = 0; i < अंक; ++i){
        वर्ण labelstr[4096] = {0};
        पूर्णाङ्क class = -1;
        प्रति(j = 0; j < classes; ++j){
            यदि (dets[i].prob[j] > thresh){
                यदि (class < 0) {
                    strcat(labelstr, names[j]);
                    class = j;
                } अन्यथा {
                    strcat(labelstr, ", ");
                    strcat(labelstr, names[j]);
                }
                printf("%s: %.0f%%\n", names[j], dets[i].prob[j]*100);
            }
        }
        यदि(class >= 0){
            पूर्णाङ्क width = im.h * .006;

            /*
               if(0){
               width = pow(prob, 1./2.)*10+1;
               alphabet = 0;
               }
             */

            //printf("%d %s: %.0f%%\n", i, names[class], prob*100);
            पूर्णाङ्क offset = class*123457 % classes;
            भिन्न red = get_color(2,offset,classes);
            भिन्न green = get_color(1,offset,classes);
            भिन्न blue = get_color(0,offset,classes);
            भिन्न rgb[3];

            //width = prob*20+2;

            rgb[0] = red;
            rgb[1] = green;
            rgb[2] = blue;
            box b = dets[i].bbox;
            //printf("%f %f %f %f\n", b.x, b.y, b.w, b.h);

            पूर्णाङ्क डावा  = (b.x-b.w/2.)*im.w;
            पूर्णाङ्क उजवा = (b.x+b.w/2.)*im.w;
            पूर्णाङ्क शीर्ष   = (b.y-b.h/2.)*im.h;
            पूर्णाङ्क bot   = (b.y+b.h/2.)*im.h;

            यदि(डावा < 0) डावा = 0;
            यदि(उजवा > im.w-1) उजवा = im.w-1;
            यदि(शीर्ष < 0) शीर्ष = 0;
            यदि(bot > im.h-1) bot = im.h-1;

            draw_box_width(im, डावा, शीर्ष, उजवा, bot, width, red, green, blue);
            यदि (alphabet) {
                image label = get_label(alphabet, labelstr, (im.h*.03));
                draw_label(im, शीर्ष + width, डावा, label, rgb);
                free_image(label);
            }
            यदि (dets[i].mask){
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
    पूर्णाङ्क n, m;
    पूर्णाङ्क c;
    प्रति(c = 0; c < im.c; ++c){
        प्रति(n = 0; n < im.w-1; ++n){
            प्रति(m = n + 1; m < im.w; ++m){
                भिन्न swap = im.data[m + im.w*(n + im.h*c)];
                im.data[m + im.w*(n + im.h*c)] = im.data[n + im.w*(m + im.h*c)];
                im.data[n + im.w*(m + im.h*c)] = swap;
            }
        }
    }
}

शून्य rotate_image_cw(image im, पूर्णाङ्क times)
{
    प्रतिज्ञा(im.w == im.h);
    times = (times + 400) % 4;
    पूर्णाङ्क i, x, y, c;
    पूर्णाङ्क n = im.w;
    प्रति(i = 0; i < times; ++i){
        प्रति(c = 0; c < im.c; ++c){
            प्रति(x = 0; x < n/2; ++x){
                प्रति(y = 0; y < (n-1)/2 + 1; ++y){
                    भिन्न temp = im.data[y + im.w*(x + im.h*c)];
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
    पूर्णाङ्क i,j,k;
    प्रति(k = 0; k < a.c; ++k){
        प्रति(i = 0; i < a.h; ++i){
            प्रति(j = 0; j < a.w/2; ++j){
                पूर्णाङ्क index = j + a.w*(i + a.h*(k));
                पूर्णाङ्क flip = (a.w - j - 1) + a.w*(i + a.h*(k));
                भिन्न swap = a.data[flip];
                a.data[flip] = a.data[index];
                a.data[index] = swap;
            }
        }
    }
}

image image_distance(image a, image b)
{
    पूर्णाङ्क i,j;
    image dist = make_image(a.w, a.h, 1);
    प्रति(i = 0; i < a.c; ++i){
        प्रति(j = 0; j < a.h*a.w; ++j){
            dist.data[j] += pow(a.data[i*a.h*a.w+j]-b.data[i*a.h*a.w+j],2);
        }
    }
    प्रति(j = 0; j < a.h*a.w; ++j){
        dist.data[j] = sqrt(dist.data[j]);
    }
    प्रत्यागम dist;
}

शून्य ghost_image(image source, image dest, पूर्णाङ्क dx, पूर्णाङ्क dy)
{
    पूर्णाङ्क x,y,k;
    भिन्न max_dist = sqrt((-source.w/2. + .5)*(-source.w/2. + .5));
    प्रति(k = 0; k < source.c; ++k){
        प्रति(y = 0; y < source.h; ++y){
            प्रति(x = 0; x < source.w; ++x){
                भिन्न dist = sqrt((x - source.w/2. + .5)*(x - source.w/2. + .5) + (y - source.h/2. + .5)*(y - source.h/2. + .5));
                भिन्न alpha = (1 - dist/max_dist);
                यदि(alpha < 0) alpha = 0;
                भिन्न v1 = get_pixel(source, x,y,k);
                भिन्न v2 = get_pixel(dest, dx+x,dy+y,k);
                भिन्न val = alpha*v1 + (1-alpha)*v2;
                set_pixel(dest, dx+x, dy+y, k, val);
            }
        }
    }
}

शून्य blocky_image(image im, पूर्णाङ्क s)
{
    पूर्णाङ्क i,j,k;
    प्रति(k = 0; k < im.c; ++k){
        प्रति(j = 0; j < im.h; ++j){
            प्रति(i = 0; i < im.w; ++i){
                im.data[i + im.w*(j + im.h*k)] = im.data[i/s*s + im.w*(j/s*s + im.h*k)];
            }
        }
    }
}

शून्य censor_image(image im, पूर्णाङ्क dx, पूर्णाङ्क dy, पूर्णाङ्क w, पूर्णाङ्क h)
{
    पूर्णाङ्क i,j,k;
    पूर्णाङ्क s = 32;
    यदि(dx < 0) dx = 0;
    यदि(dy < 0) dy = 0;

    प्रति(k = 0; k < im.c; ++k){
        प्रति(j = dy; j < dy + h && j < im.h; ++j){
            प्रति(i = dx; i < dx + w && i < im.w; ++i){
                im.data[i + im.w*(j + im.h*k)] = im.data[i/s*s + im.w*(j/s*s + im.h*k)];
                //im.data[i + j*im.w + k*im.w*im.h] = 0;
            }
        }
    }
}

शून्य embed_image(image source, image dest, पूर्णाङ्क dx, पूर्णाङ्क dy)
{
    पूर्णाङ्क x,y,k;
    प्रति(k = 0; k < source.c; ++k){
        प्रति(y = 0; y < source.h; ++y){
            प्रति(x = 0; x < source.w; ++x){
                भिन्न val = get_pixel(source, x,y,k);
                set_pixel(dest, dx+x, dy+y, k, val);
            }
        }
    }
}

image collapse_image_layers(image source, पूर्णाङ्क border)
{
    पूर्णाङ्क h = source.h;
    h = (h+border)*source.c - border;
    image dest = make_image(source.w, h, 1);
    पूर्णाङ्क i;
    प्रति(i = 0; i < source.c; ++i){
        image layer = get_image_layer(source, i);
        पूर्णाङ्क h_offset = i*(source.h+border);
        embed_image(layer, dest, 0, h_offset);
        free_image(layer);
    }
    प्रत्यागम dest;
}

शून्य constrain_image(image im)
{
    पूर्णाङ्क i;
    प्रति(i = 0; i < im.w*im.h*im.c; ++i){
        यदि(im.data[i] < 0) im.data[i] = 0;
        यदि(im.data[i] > 1) im.data[i] = 1;
    }
}

शून्य normalize_image(image p)
{
    पूर्णाङ्क i;
    भिन्न min = 9999999;
    भिन्न max = -999999;

    प्रति(i = 0; i < p.h*p.w*p.c; ++i){
        भिन्न v = p.data[i];
        यदि(v < min) min = v;
        यदि(v > max) max = v;
    }
    यदि(max - min < .000000001){
        min = 0;
        max = 1;
    }
    प्रति(i = 0; i < p.c*p.w*p.h; ++i){
        p.data[i] = (p.data[i] - min)/(max-min);
    }
}

शून्य normalize_image2(image p)
{
    भिन्न *min = calloc(p.c, परिमाण(भिन्न));
    भिन्न *max = calloc(p.c, परिमाण(भिन्न));
    पूर्णाङ्क i,j;
    प्रति(i = 0; i < p.c; ++i) min[i] = max[i] = p.data[i*p.h*p.w];

    प्रति(j = 0; j < p.c; ++j){
        प्रति(i = 0; i < p.h*p.w; ++i){
            भिन्न v = p.data[i+j*p.h*p.w];
            यदि(v < min[j]) min[j] = v;
            यदि(v > max[j]) max[j] = v;
        }
    }
    प्रति(i = 0; i < p.c; ++i){
        यदि(max[i] - min[i] < .000000001){
            min[i] = 0;
            max[i] = 1;
        }
    }
    प्रति(j = 0; j < p.c; ++j){
        प्रति(i = 0; i < p.w*p.h; ++i){
            p.data[i+j*p.h*p.w] = (p.data[i+j*p.h*p.w] - min[j])/(max[j]-min[j]);
        }
    }
    मुक्त(min);
    मुक्त(max);
}

शून्य copy_image_into(image src, image dest)
{
    memcpy(dest.data, src.data, src.h*src.w*src.c*परिमाण(भिन्न));
}

image copy_image(image p)
{
    image copy = p;
    copy.data = calloc(p.h*p.w*p.c, परिमाण(भिन्न));
    memcpy(copy.data, p.data, p.h*p.w*p.c*परिमाण(भिन्न));
    प्रत्यागम copy;
}

शून्य rgbgr_image(image im)
{
    पूर्णाङ्क i;
    प्रति(i = 0; i < im.w*im.h; ++i){
        भिन्न swap = im.data[i];
        im.data[i] = im.data[i+im.w*im.h*2];
        im.data[i+im.w*im.h*2] = swap;
    }
}

पूर्णाङ्क show_image(image p, ध्रुव वर्ण *name, पूर्णाङ्क ms)
{
#ifdef OPENCV
    पूर्णाङ्क c = show_image_cv(p, name, ms);
    प्रत्यागम c;
#अन्यथा
    fprintf(stderr, "Not compiled with OpenCV, saving to %s.png instead\n", name);
    save_image(p, name);
    प्रत्यागम -1;
#endif
}

शून्य save_image_options(image im, ध्रुव वर्ण *name, IMTYPE f, पूर्णाङ्क quality)
{
    वर्ण buff[256];
    //sprintf(buff, "%s (%d)", name, windows);
    यदि(f == PNG)       sprintf(buff, "%s.png", name);
    अन्यथा यदि (f == BMP) sprintf(buff, "%s.bmp", name);
    अन्यथा यदि (f == TGA) sprintf(buff, "%s.tga", name);
    अन्यथा यदि (f == JPG) sprintf(buff, "%s.jpg", name);
    अन्यथा               sprintf(buff, "%s.png", name);
    unsigned वर्ण *data = calloc(im.w*im.h*im.c, परिमाण(वर्ण));
    पूर्णाङ्क i,k;
    प्रति(k = 0; k < im.c; ++k){
        प्रति(i = 0; i < im.w*im.h; ++i){
            data[i*im.c+k] = (unsigned वर्ण) (255*im.data[i + k*im.w*im.h]);
        }
    }
    पूर्णाङ्क success = 0;
    यदि(f == PNG)       success = stbi_write_png(buff, im.w, im.h, im.c, data, im.w*im.c);
    अन्यथा यदि (f == BMP) success = stbi_write_bmp(buff, im.w, im.h, im.c, data);
    अन्यथा यदि (f == TGA) success = stbi_write_tga(buff, im.w, im.h, im.c, data);
    अन्यथा यदि (f == JPG) success = stbi_write_jpg(buff, im.w, im.h, im.c, data, quality);
    मुक्त(data);
    यदि(!success) fprintf(stderr, "Failed to write image %s\n", buff);
}

शून्य save_image(image im, ध्रुव वर्ण *name)
{
    save_image_options(im, name, JPG, 80);
}

शून्य show_image_layers(image p, वर्ण *name)
{
    पूर्णाङ्क i;
    वर्ण buff[256];
    प्रति(i = 0; i < p.c; ++i){
        sprintf(buff, "%s - Layer %d", name, i);
        image layer = get_image_layer(p, i);
        show_image(layer, buff, 1);
        free_image(layer);
    }
}

शून्य show_image_collapsed(image p, वर्ण *name)
{
    image c = collapse_image_layers(p, 1);
    show_image(c, name, 1);
    free_image(c);
}

image make_empty_image(पूर्णाङ्क w, पूर्णाङ्क h, पूर्णाङ्क c)
{
    image out;
    out.data = 0;
    out.h = h;
    out.w = w;
    out.c = c;
    प्रत्यागम out;
}

image make_image(पूर्णाङ्क w, पूर्णाङ्क h, पूर्णाङ्क c)
{
    image out = make_empty_image(w,h,c);
    out.data = calloc(h*w*c, परिमाण(भिन्न));
    प्रत्यागम out;
}

image make_random_image(पूर्णाङ्क w, पूर्णाङ्क h, पूर्णाङ्क c)
{
    image out = make_empty_image(w,h,c);
    out.data = calloc(h*w*c, परिमाण(भिन्न));
    पूर्णाङ्क i;
    प्रति(i = 0; i < w*h*c; ++i){
        out.data[i] = (rand_normal() * .25) + .5;
    }
    प्रत्यागम out;
}

image float_to_image(पूर्णाङ्क w, पूर्णाङ्क h, पूर्णाङ्क c, भिन्न *data)
{
    image out = make_empty_image(w,h,c);
    out.data = data;
    प्रत्यागम out;
}

शून्य place_image(image im, पूर्णाङ्क w, पूर्णाङ्क h, पूर्णाङ्क dx, पूर्णाङ्क dy, image canvas)
{
    पूर्णाङ्क x, y, c;
    प्रति(c = 0; c < im.c; ++c){
        प्रति(y = 0; y < h; ++y){
            प्रति(x = 0; x < w; ++x){
                भिन्न rx = ((भिन्न)x / w) * im.w;
                भिन्न ry = ((भिन्न)y / h) * im.h;
                भिन्न val = bilinear_interpolate(im, rx, ry, c);
                set_pixel(canvas, x + dx, y + dy, c, val);
            }
        }
    }
}

image center_crop_image(image im, पूर्णाङ्क w, पूर्णाङ्क h)
{
    पूर्णाङ्क m = (im.w < im.h) ? im.w : im.h;   
    image c = crop_image(im, (im.w - m) / 2, (im.h - m)/2, m, m);
    image r = resize_image(c, w, h);
    free_image(c);
    प्रत्यागम r;
}

image rotate_crop_image(image im, भिन्न rad, भिन्न s, पूर्णाङ्क w, पूर्णाङ्क h, भिन्न dx, भिन्न dy, भिन्न aspect)
{
    पूर्णाङ्क x, y, c;
    भिन्न cx = im.w/2.;
    भिन्न cy = im.h/2.;
    image rot = make_image(w, h, im.c);
    प्रति(c = 0; c < im.c; ++c){
        प्रति(y = 0; y < h; ++y){
            प्रति(x = 0; x < w; ++x){
                भिन्न rx = cos(rad)*((x - w/2.)/s*aspect + dx/s*aspect) - sin(rad)*((y - h/2.)/s + dy/s) + cx;
                भिन्न ry = sin(rad)*((x - w/2.)/s*aspect + dx/s*aspect) + cos(rad)*((y - h/2.)/s + dy/s) + cy;
                भिन्न val = bilinear_interpolate(im, rx, ry, c);
                set_pixel(rot, x, y, c, val);
            }
        }
    }
    प्रत्यागम rot;
}

image rotate_image(image im, भिन्न rad)
{
    पूर्णाङ्क x, y, c;
    भिन्न cx = im.w/2.;
    भिन्न cy = im.h/2.;
    image rot = make_image(im.w, im.h, im.c);
    प्रति(c = 0; c < im.c; ++c){
        प्रति(y = 0; y < im.h; ++y){
            प्रति(x = 0; x < im.w; ++x){
                भिन्न rx = cos(rad)*(x-cx) - sin(rad)*(y-cy) + cx;
                भिन्न ry = sin(rad)*(x-cx) + cos(rad)*(y-cy) + cy;
                भिन्न val = bilinear_interpolate(im, rx, ry, c);
                set_pixel(rot, x, y, c, val);
            }
        }
    }
    प्रत्यागम rot;
}

शून्य fill_image(image m, भिन्न s)
{
    पूर्णाङ्क i;
    प्रति(i = 0; i < m.h*m.w*m.c; ++i) m.data[i] = s;
}

शून्य translate_image(image m, भिन्न s)
{
    पूर्णाङ्क i;
    प्रति(i = 0; i < m.h*m.w*m.c; ++i) m.data[i] += s;
}

शून्य scale_image(image m, भिन्न s)
{
    पूर्णाङ्क i;
    प्रति(i = 0; i < m.h*m.w*m.c; ++i) m.data[i] *= s;
}

image crop_image(image im, पूर्णाङ्क dx, पूर्णाङ्क dy, पूर्णाङ्क w, पूर्णाङ्क h)
{
    image cropped = make_image(w, h, im.c);
    पूर्णाङ्क i, j, k;
    प्रति(k = 0; k < im.c; ++k){
        प्रति(j = 0; j < h; ++j){
            प्रति(i = 0; i < w; ++i){
                पूर्णाङ्क r = j + dy;
                पूर्णाङ्क c = i + dx;
                भिन्न val = 0;
                r = constrain_int(r, 0, im.h-1);
                c = constrain_int(c, 0, im.w-1);
                val = get_pixel(im, c, r, k);
                set_pixel(cropped, i, j, k, val);
            }
        }
    }
    प्रत्यागम cropped;
}

पूर्णाङ्क best_3d_shift_r(image a, image b, पूर्णाङ्क min, पूर्णाङ्क max)
{
    यदि(min == max) प्रत्यागम min;
    पूर्णाङ्क mid = floor((min + max) / 2.);
    image c1 = crop_image(b, 0, mid, b.w, b.h);
    image c2 = crop_image(b, 0, mid+1, b.w, b.h);
    भिन्न d1 = dist_array(c1.data, a.data, a.w*a.h*a.c, 10);
    भिन्न d2 = dist_array(c2.data, a.data, a.w*a.h*a.c, 10);
    free_image(c1);
    free_image(c2);
    यदि(d1 < d2) प्रत्यागम best_3d_shift_r(a, b, min, mid);
    अन्यथा प्रत्यागम best_3d_shift_r(a, b, mid+1, max);
}

पूर्णाङ्क best_3d_shift(image a, image b, पूर्णाङ्क min, पूर्णाङ्क max)
{
    पूर्णाङ्क i;
    पूर्णाङ्क best = 0;
    भिन्न best_distance = FLT_MAX;
    प्रति(i = min; i <= max; i += 2){
        image c = crop_image(b, 0, i, b.w, b.h);
        भिन्न d = dist_array(c.data, a.data, a.w*a.h*a.c, 100);
        यदि(d < best_distance){
            best_distance = d;
            best = i;
        }
        printf("%d %f\n", i, d);
        free_image(c);
    }
    प्रत्यागम best;
}

शून्य composite_3d(वर्ण *f1, वर्ण *f2, वर्ण *out, पूर्णाङ्क delta)
{
    यदि(!out) out = "out";
    image a = load_image(f1, 0,0,0);
    image b = load_image(f2, 0,0,0);
    पूर्णाङ्क shift = best_3d_shift_r(a, b, -a.h/100, a.h/100);

    image c1 = crop_image(b, 10, shift, b.w, b.h);
    भिन्न d1 = dist_array(c1.data, a.data, a.w*a.h*a.c, 100);
    image c2 = crop_image(b, -10, shift, b.w, b.h);
    भिन्न d2 = dist_array(c2.data, a.data, a.w*a.h*a.c, 100);

    यदि(d2 < d1 && 0){
        image swap = a;
        a = b;
        b = swap;
        shift = -shift;
        printf("swapped, %d\n", shift);
    }
    अन्यथा{
        printf("%d\n", shift);
    }

    image c = crop_image(b, delta, shift, a.w, a.h);
    पूर्णाङ्क i;
    प्रति(i = 0; i < c.w*c.h; ++i){
        c.data[i] = a.data[i];
    }
    save_image(c, out);
}

शून्य letterbox_image_into(image im, पूर्णाङ्क w, पूर्णाङ्क h, image boxed)
{
    पूर्णाङ्क new_w = im.w;
    पूर्णाङ्क new_h = im.h;
    यदि (((भिन्न)w/im.w) < ((भिन्न)h/im.h)) {
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

image letterbox_image(image im, पूर्णाङ्क w, पूर्णाङ्क h)
{
    पूर्णाङ्क new_w = im.w;
    पूर्णाङ्क new_h = im.h;
    यदि (((भिन्न)w/im.w) < ((भिन्न)h/im.h)) {
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
    प्रत्यागम boxed;
}

image resize_max(image im, पूर्णाङ्क max)
{
    पूर्णाङ्क w = im.w;
    पूर्णाङ्क h = im.h;
    यदि(w > h){
        h = (h * max) / w;
        w = max;
    } अन्यथा {
        w = (w * max) / h;
        h = max;
    }
    यदि(w == im.w && h == im.h) प्रत्यागम im;
    image resized = resize_image(im, w, h);
    प्रत्यागम resized;
}

image resize_min(image im, पूर्णाङ्क min)
{
    पूर्णाङ्क w = im.w;
    पूर्णाङ्क h = im.h;
    यदि(w < h){
        h = (h * min) / w;
        w = min;
    } अन्यथा {
        w = (w * min) / h;
        h = min;
    }
    यदि(w == im.w && h == im.h) प्रत्यागम im;
    image resized = resize_image(im, w, h);
    प्रत्यागम resized;
}

image random_crop_image(image im, पूर्णाङ्क w, पूर्णाङ्क h)
{
    पूर्णाङ्क dx = rand_int(0, im.w - w);
    पूर्णाङ्क dy = rand_int(0, im.h - h);
    image crop = crop_image(im, dx, dy, w, h);
    प्रत्यागम crop;
}

augment_args random_augment_args(image im, भिन्न angle, भिन्न aspect, पूर्णाङ्क खालचा, पूर्णाङ्क वरचा, पूर्णाङ्क w, पूर्णाङ्क h)
{
    augment_args a = {0};
    aspect = rand_scale(aspect);
    पूर्णाङ्क r = rand_int(खालचा, वरचा);
    पूर्णाङ्क min = (im.h < im.w*aspect) ? im.h : im.w*aspect;
    भिन्न scale = (भिन्न)r / min;

    भिन्न rad = rand_uniform(-angle, angle) * TWO_PI / 360.;

    भिन्न dx = (im.w*scale/aspect - w) / 2.;
    भिन्न dy = (im.h*scale - w) / 2.;
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
    प्रत्यागम a;
}

image random_augment_image(image im, भिन्न angle, भिन्न aspect, पूर्णाङ्क खालचा, पूर्णाङ्क वरचा, पूर्णाङ्क w, पूर्णाङ्क h)
{
    augment_args a = random_augment_args(im, angle, aspect, खालचा, वरचा, w, h);
    image crop = rotate_crop_image(im, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);
    प्रत्यागम crop;
}

भिन्न three_way_max(भिन्न a, भिन्न b, भिन्न c)
{
    प्रत्यागम (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

भिन्न three_way_min(भिन्न a, भिन्न b, भिन्न c)
{
    प्रत्यागम (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

शून्य yuv_to_rgb(image im)
{
    प्रतिज्ञा(im.c == 3);
    पूर्णाङ्क i, j;
    भिन्न r, g, b;
    भिन्न y, u, v;
    प्रति(j = 0; j < im.h; ++j){
        प्रति(i = 0; i < im.w; ++i){
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
    पूर्णाङ्क i, j;
    भिन्न r, g, b;
    भिन्न y, u, v;
    प्रति(j = 0; j < im.h; ++j){
        प्रति(i = 0; i < im.w; ++i){
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
    पूर्णाङ्क i, j;
    भिन्न r, g, b;
    भिन्न h, s, v;
    प्रति(j = 0; j < im.h; ++j){
        प्रति(i = 0; i < im.w; ++i){
            r = get_pixel(im, i , j, 0);
            g = get_pixel(im, i , j, 1);
            b = get_pixel(im, i , j, 2);
            भिन्न max = three_way_max(r,g,b);
            भिन्न min = three_way_min(r,g,b);
            भिन्न delta = max - min;
            v = max;
            यदि(max == 0){
                s = 0;
                h = 0;
            }अन्यथा{
                s = delta/max;
                यदि(r == max){
                    h = (g - b) / delta;
                } अन्यथा यदि (g == max) {
                    h = 2 + (b - r) / delta;
                } अन्यथा {
                    h = 4 + (r - g) / delta;
                }
                यदि (h < 0) h += 6;
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
    पूर्णाङ्क i, j;
    भिन्न r, g, b;
    भिन्न h, s, v;
    भिन्न f, p, q, t;
    प्रति(j = 0; j < im.h; ++j){
        प्रति(i = 0; i < im.w; ++i){
            h = 6 * get_pixel(im, i , j, 0);
            s = get_pixel(im, i , j, 1);
            v = get_pixel(im, i , j, 2);
            यदि (s == 0) {
                r = g = b = v;
            } अन्यथा {
                पूर्णाङ्क index = floor(h);
                f = h - index;
                p = v*(1-s);
                q = v*(1-s*f);
                t = v*(1-s*(1-f));
                यदि(index == 0){
                    r = v; g = t; b = p;
                } अन्यथा यदि(index == 1){
                    r = q; g = v; b = p;
                } अन्यथा यदि(index == 2){
                    r = p; g = v; b = t;
                } अन्यथा यदि(index == 3){
                    r = p; g = q; b = v;
                } अन्यथा यदि(index == 4){
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
    पूर्णाङ्क i, j, k;
    भिन्न scale[] = {0.299, 0.587, 0.114};
    प्रति(j = 0; j < im.h; ++j){
        प्रति(i = 0; i < im.w; ++i){
            भिन्न val = 0;
            प्रति(k = 0; k < 3; ++k){
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
    पूर्णाङ्क i, j, k;
    image gray = make_image(im.w, im.h, 1);
    भिन्न scale[] = {0.299, 0.587, 0.114};
    प्रति(k = 0; k < im.c; ++k){
        प्रति(j = 0; j < im.h; ++j){
            प्रति(i = 0; i < im.w; ++i){
                gray.data[i+im.w*j] += scale[k]*get_pixel(im, i, j, k);
            }
        }
    }
    प्रत्यागम gray;
}

image threshold_image(image im, भिन्न thresh)
{
    पूर्णाङ्क i;
    image t = make_image(im.w, im.h, im.c);
    प्रति(i = 0; i < im.w*im.h*im.c; ++i){
        t.data[i] = im.data[i]>thresh ? 1 : 0;
    }
    प्रत्यागम t;
}

image blend_image(image fore, image back, भिन्न alpha)
{
    प्रतिज्ञा(fore.w == back.w && fore.h == back.h && fore.c == back.c);
    image blend = make_image(fore.w, fore.h, fore.c);
    पूर्णाङ्क i, j, k;
    प्रति(k = 0; k < fore.c; ++k){
        प्रति(j = 0; j < fore.h; ++j){
            प्रति(i = 0; i < fore.w; ++i){
                भिन्न val = alpha * get_pixel(fore, i, j, k) + 
                    (1 - alpha)* get_pixel(back, i, j, k);
                set_pixel(blend, i, j, k, val);
            }
        }
    }
    प्रत्यागम blend;
}

शून्य scale_image_channel(image im, पूर्णाङ्क c, भिन्न v)
{
    पूर्णाङ्क i, j;
    प्रति(j = 0; j < im.h; ++j){
        प्रति(i = 0; i < im.w; ++i){
            भिन्न pix = get_pixel(im, i, j, c);
            pix = pix*v;
            set_pixel(im, i, j, c, pix);
        }
    }
}

शून्य translate_image_channel(image im, पूर्णाङ्क c, भिन्न v)
{
    पूर्णाङ्क i, j;
    प्रति(j = 0; j < im.h; ++j){
        प्रति(i = 0; i < im.w; ++i){
            भिन्न pix = get_pixel(im, i, j, c);
            pix = pix+v;
            set_pixel(im, i, j, c, pix);
        }
    }
}

image binarize_image(image im)
{
    image c = copy_image(im);
    पूर्णाङ्क i;
    प्रति(i = 0; i < im.w * im.h * im.c; ++i){
        यदि(c.data[i] > .5) c.data[i] = 1;
        अन्यथा c.data[i] = 0;
    }
    प्रत्यागम c;
}

शून्य saturate_image(image im, भिन्न sat)
{
    rgb_to_hsv(im);
    scale_image_channel(im, 1, sat);
    hsv_to_rgb(im);
    constrain_image(im);
}

शून्य hue_image(image im, भिन्न hue)
{
    rgb_to_hsv(im);
    पूर्णाङ्क i;
    प्रति(i = 0; i < im.w*im.h; ++i){
        im.data[i] = im.data[i] + hue;
        यदि (im.data[i] > 1) im.data[i] -= 1;
        यदि (im.data[i] < 0) im.data[i] += 1;
    }
    hsv_to_rgb(im);
    constrain_image(im);
}

शून्य exposure_image(image im, भिन्न sat)
{
    rgb_to_hsv(im);
    scale_image_channel(im, 2, sat);
    hsv_to_rgb(im);
    constrain_image(im);
}

शून्य distort_image(image im, भिन्न hue, भिन्न sat, भिन्न val)
{
    rgb_to_hsv(im);
    scale_image_channel(im, 1, sat);
    scale_image_channel(im, 2, val);
    पूर्णाङ्क i;
    प्रति(i = 0; i < im.w*im.h; ++i){
        im.data[i] = im.data[i] + hue;
        यदि (im.data[i] > 1) im.data[i] -= 1;
        यदि (im.data[i] < 0) im.data[i] += 1;
    }
    hsv_to_rgb(im);
    constrain_image(im);
}

शून्य random_distort_image(image im, भिन्न hue, भिन्न saturation, भिन्न exposure)
{
    भिन्न dhue = rand_uniform(-hue, hue);
    भिन्न dsat = rand_scale(saturation);
    भिन्न dexp = rand_scale(exposure);
    distort_image(im, dhue, dsat, dexp);
}

शून्य saturate_exposure_image(image im, भिन्न sat, भिन्न exposure)
{
    rgb_to_hsv(im);
    scale_image_channel(im, 1, sat);
    scale_image_channel(im, 2, exposure);
    hsv_to_rgb(im);
    constrain_image(im);
}

image resize_image(image im, पूर्णाङ्क w, पूर्णाङ्क h)
{
    image resized = make_image(w, h, im.c);   
    image part = make_image(w, im.h, im.c);
    पूर्णाङ्क r, c, k;
    भिन्न w_scale = (भिन्न)(im.w - 1) / (w - 1);
    भिन्न h_scale = (भिन्न)(im.h - 1) / (h - 1);
    प्रति(k = 0; k < im.c; ++k){
        प्रति(r = 0; r < im.h; ++r){
            प्रति(c = 0; c < w; ++c){
                भिन्न val = 0;
                यदि(c == w-1 || im.w == 1){
                    val = get_pixel(im, im.w-1, r, k);
                } अन्यथा {
                    भिन्न sx = c*w_scale;
                    पूर्णाङ्क ix = (पूर्णाङ्क) sx;
                    भिन्न dx = sx - ix;
                    val = (1 - dx) * get_pixel(im, ix, r, k) + dx * get_pixel(im, ix+1, r, k);
                }
                set_pixel(part, c, r, k, val);
            }
        }
    }
    प्रति(k = 0; k < im.c; ++k){
        प्रति(r = 0; r < h; ++r){
            भिन्न sy = r*h_scale;
            पूर्णाङ्क iy = (पूर्णाङ्क) sy;
            भिन्न dy = sy - iy;
            प्रति(c = 0; c < w; ++c){
                भिन्न val = (1-dy) * get_pixel(part, c, iy, k);
                set_pixel(resized, c, r, k, val);
            }
            यदि(r == h-1 || im.h == 1) अनुवर्त;
            प्रति(c = 0; c < w; ++c){
                भिन्न val = dy * get_pixel(part, c, iy+1, k);
                add_pixel(resized, c, r, k, val);
            }
        }
    }

    free_image(part);
    प्रत्यागम resized;
}


शून्य test_resize(वर्ण *filename)
{
    image im = load_image(filename, 0,0, 3);
    भिन्न mag = mag_array(im.data, im.w*im.h*im.c);
    printf("L2 Norm: %f\n", mag);
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
    यावत्(1){
        image aug = random_augment_image(im, 0, .75, 320, 448, 320, 320);
        show_image(aug, "aug", 1);
        free_image(aug);


        भिन्न exposure = 1.15;
        भिन्न saturation = 1.15;
        भिन्न hue = .05;

        image c = copy_image(im);

        भिन्न dexp = rand_scale(exposure);
        भिन्न dsat = rand_scale(saturation);
        भिन्न dhue = rand_uniform(-hue, hue);

        distort_image(c, dhue, dsat, dexp);
        show_image(c, "rand", 1);
        printf("%f %f %f\n", dhue, dsat, dexp);
        free_image(c);
    }
#endif
}


image load_image_stb(वर्ण *filename, पूर्णाङ्क channels)
{
    पूर्णाङ्क w, h, c;
    unsigned वर्ण *data = stbi_load(filename, &w, &h, &c, channels);
    यदि (!data) {
        fprintf(stderr, "Cannot load image \"%s\"\nSTB Reason: %s\n", filename, stbi_failure_reason());
        exit(0);
    }
    यदि(channels) c = channels;
    पूर्णाङ्क i,j,k;
    image im = make_image(w, h, c);
    प्रति(k = 0; k < c; ++k){
        प्रति(j = 0; j < h; ++j){
            प्रति(i = 0; i < w; ++i){
                पूर्णाङ्क dst_index = i + w*j + w*h*k;
                पूर्णाङ्क src_index = k + c*i + c*w*j;
                im.data[dst_index] = (भिन्न)data[src_index]/255.;
            }
        }
    }
    मुक्त(data);
    प्रत्यागम im;
}

image load_image(वर्ण *filename, पूर्णाङ्क w, पूर्णाङ्क h, पूर्णाङ्क c)
{
#ifdef OPENCV
    image out = load_image_cv(filename, c);
#अन्यथा
    image out = load_image_stb(filename, c);
#endif

    यदि((h && w) && (h != out.h || w != out.w)){
        image resized = resize_image(out, w, h);
        free_image(out);
        out = resized;
    }
    प्रत्यागम out;
}

image load_image_color(वर्ण *filename, पूर्णाङ्क w, पूर्णाङ्क h)
{
    प्रत्यागम load_image(filename, w, h, 3);
}

image get_image_layer(image m, पूर्णाङ्क l)
{
    image out = make_image(m.w, m.h, 1);
    पूर्णाङ्क i;
    प्रति(i = 0; i < m.h*m.w; ++i){
        out.data[i] = m.data[i+l*m.h*m.w];
    }
    प्रत्यागम out;
}
शून्य print_image(image m)
{
    पूर्णाङ्क i, j, k;
    प्रति(i =0 ; i < m.c; ++i){
        प्रति(j =0 ; j < m.h; ++j){
            प्रति(k = 0; k < m.w; ++k){
                printf("%.2lf, ", m.data[i*m.h*m.w + j*m.w + k]);
                यदि(k > 30) भङ्ग;
            }
            printf("\n");
            यदि(j > 30) भङ्ग;
        }
        printf("\n");
    }
    printf("\n");
}

image collapse_images_vert(image *ims, पूर्णाङ्क n)
{
    पूर्णाङ्क color = 1;
    पूर्णाङ्क border = 1;
    पूर्णाङ्क h,w,c;
    w = ims[0].w;
    h = (ims[0].h + border) * n - border;
    c = ims[0].c;
    यदि(c != 3 || !color){
        w = (w+border)*c - border;
        c = 1;
    }

    image filters = make_image(w, h, c);
    पूर्णाङ्क i,j;
    प्रति(i = 0; i < n; ++i){
        पूर्णाङ्क h_offset = i*(ims[0].h+border);
        image copy = copy_image(ims[i]);
        //normalize_image(copy);
        यदि(c == 3 && color){
            embed_image(copy, filters, 0, h_offset);
        }
        अन्यथा{
            प्रति(j = 0; j < copy.c; ++j){
                पूर्णाङ्क w_offset = j*(ims[0].w+border);
                image layer = get_image_layer(copy, j);
                embed_image(layer, filters, w_offset, h_offset);
                free_image(layer);
            }
        }
        free_image(copy);
    }
    प्रत्यागम filters;
} 

image collapse_images_horz(image *ims, पूर्णाङ्क n)
{
    पूर्णाङ्क color = 1;
    पूर्णाङ्क border = 1;
    पूर्णाङ्क h,w,c;
    पूर्णाङ्क size = ims[0].h;
    h = size;
    w = (ims[0].w + border) * n - border;
    c = ims[0].c;
    यदि(c != 3 || !color){
        h = (h+border)*c - border;
        c = 1;
    }

    image filters = make_image(w, h, c);
    पूर्णाङ्क i,j;
    प्रति(i = 0; i < n; ++i){
        पूर्णाङ्क w_offset = i*(size+border);
        image copy = copy_image(ims[i]);
        //normalize_image(copy);
        यदि(c == 3 && color){
            embed_image(copy, filters, w_offset, 0);
        }
        अन्यथा{
            प्रति(j = 0; j < copy.c; ++j){
                पूर्णाङ्क h_offset = j*(size+border);
                image layer = get_image_layer(copy, j);
                embed_image(layer, filters, w_offset, h_offset);
                free_image(layer);
            }
        }
        free_image(copy);
    }
    प्रत्यागम filters;
} 

शून्य show_image_normalized(image im, ध्रुव वर्ण *name)
{
    image c = copy_image(im);
    normalize_image(c);
    show_image(c, name, 1);
    free_image(c);
}

शून्य show_images(image *ims, पूर्णाङ्क n, वर्ण *window)
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
    यदि(m.data){
        मुक्त(m.data);
    }
}
