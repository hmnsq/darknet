#അന്തര്ഭാവ "image.h"
#അന്തര്ഭാവ "utils.h"
#അന്തര്ഭാവ "blas.h"
#അന്തര്ഭാവ "cuda.h"
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <math.h>

#define STB_IMAGE_IMPLEMENTATION
#അന്തര്ഭാവ "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#അന്തര്ഭാവ "stb_image_write.h"

പൂർണ്ണസംഖ്യ windows = 0;

ദശാംശം colors[6][3] = { {1,0,1}, {0,0,1},{0,1,1},{0,1,0},{1,1,0},{1,0,0} };

ദശാംശം get_color(പൂർണ്ണസംഖ്യ c, പൂർണ്ണസംഖ്യ x, പൂർണ്ണസംഖ്യ max)
{
    ദശാംശം ratio = ((ദശാംശം)x/max)*5;
    പൂർണ്ണസംഖ്യ i = floor(ratio);
    പൂർണ്ണസംഖ്യ j = ceil(ratio);
    ratio -= i;
    ദശാംശം r = (1-ratio) * colors[i][c] + ratio*colors[j][c];
    //printf("%f\n", r);
    തിരികെ r;
}

image mask_to_rgb(image mask)
{
    പൂർണ്ണസംഖ്യ n = mask.c;
    image im = make_image(mask.w, mask.h, 3);
    പൂർണ്ണസംഖ്യ i, j;
    വേണ്ടി(j = 0; j < n; ++j){
        പൂർണ്ണസംഖ്യ offset = j*123457 % n;
        ദശാംശം red = get_color(2,offset,n);
        ദശാംശം green = get_color(1,offset,n);
        ദശാംശം blue = get_color(0,offset,n);
        വേണ്ടി(i = 0; i < im.w*im.h; ++i){
            im.data[i + 0*im.w*im.h] += mask.data[j*im.h*im.w + i]*red;
            im.data[i + 1*im.w*im.h] += mask.data[j*im.h*im.w + i]*green;
            im.data[i + 2*im.w*im.h] += mask.data[j*im.h*im.w + i]*blue;
        }
    }
    തിരികെ im;
}

സ്ഥിതം ദശാംശം get_pixel(image m, പൂർണ്ണസംഖ്യ x, പൂർണ്ണസംഖ്യ y, പൂർണ്ണസംഖ്യ c)
{
    പ്രതിജ്ഞാ(x < m.w && y < m.h && c < m.c);
    തിരികെ m.data[c*m.h*m.w + y*m.w + x];
}
സ്ഥിതം ദശാംശം get_pixel_extend(image m, പൂർണ്ണസംഖ്യ x, പൂർണ്ണസംഖ്യ y, പൂർണ്ണസംഖ്യ c)
{
    എങ്കിൽ(x < 0 || x >= m.w || y < 0 || y >= m.h) തിരികെ 0;
    /*
    if(x < 0) x = 0;
    if(x >= m.w) x = m.w-1;
    if(y < 0) y = 0;
    if(y >= m.h) y = m.h-1;
    */
    എങ്കിൽ(c < 0 || c >= m.c) തിരികെ 0;
    തിരികെ get_pixel(m, x, y, c);
}
സ്ഥിതം ശൂന്യം set_pixel(image m, പൂർണ്ണസംഖ്യ x, പൂർണ്ണസംഖ്യ y, പൂർണ്ണസംഖ്യ c, ദശാംശം val)
{
    എങ്കിൽ (x < 0 || y < 0 || c < 0 || x >= m.w || y >= m.h || c >= m.c) തിരികെ;
    പ്രതിജ്ഞാ(x < m.w && y < m.h && c < m.c);
    m.data[c*m.h*m.w + y*m.w + x] = val;
}
സ്ഥിതം ശൂന്യം add_pixel(image m, പൂർണ്ണസംഖ്യ x, പൂർണ്ണസംഖ്യ y, പൂർണ്ണസംഖ്യ c, ദശാംശം val)
{
    പ്രതിജ്ഞാ(x < m.w && y < m.h && c < m.c);
    m.data[c*m.h*m.w + y*m.w + x] += val;
}

സ്ഥിതം ദശാംശം bilinear_interpolate(image im, ദശാംശം x, ദശാംശം y, പൂർണ്ണസംഖ്യ c)
{
    പൂർണ്ണസംഖ്യ ix = (പൂർണ്ണസംഖ്യ) floorf(x);
    പൂർണ്ണസംഖ്യ iy = (പൂർണ്ണസംഖ്യ) floorf(y);

    ദശാംശം dx = x - ix;
    ദശാംശം dy = y - iy;

    ദശാംശം val = (1-dy) * (1-dx) * get_pixel_extend(im, ix, iy, c) + 
        dy     * (1-dx) * get_pixel_extend(im, ix, iy+1, c) + 
        (1-dy) *   dx   * get_pixel_extend(im, ix+1, iy, c) +
        dy     *   dx   * get_pixel_extend(im, ix+1, iy+1, c);
    തിരികെ val;
}


ശൂന്യം composite_image(image source, image dest, പൂർണ്ണസംഖ്യ dx, പൂർണ്ണസംഖ്യ dy)
{
    പൂർണ്ണസംഖ്യ x,y,k;
    വേണ്ടി(k = 0; k < source.c; ++k){
        വേണ്ടി(y = 0; y < source.h; ++y){
            വേണ്ടി(x = 0; x < source.w; ++x){
                ദശാംശം val = get_pixel(source, x, y, k);
                ദശാംശം val2 = get_pixel_extend(dest, dx+x, dy+y, k);
                set_pixel(dest, dx+x, dy+y, k, val * val2);
            }
        }
    }
}

image border_image(image a, പൂർണ്ണസംഖ്യ border)
{
    image b = make_image(a.w + 2*border, a.h + 2*border, a.c);
    പൂർണ്ണസംഖ്യ x,y,k;
    വേണ്ടി(k = 0; k < b.c; ++k){
        വേണ്ടി(y = 0; y < b.h; ++y){
            വേണ്ടി(x = 0; x < b.w; ++x){
                ദശാംശം val = get_pixel_extend(a, x - border, y - border, k);
                എങ്കിൽ(x - border < 0 || x - border >= a.w || y - border < 0 || y - border >= a.h) val = 1;
                set_pixel(b, x, y, k, val);
            }
        }
    }
    തിരികെ b;
}

image tile_images(image a, image b, പൂർണ്ണസംഖ്യ dx)
{
    എങ്കിൽ(a.w == 0) തിരികെ copy_image(b);
    image c = make_image(a.w + b.w + dx, (a.h > b.h) ? a.h : b.h, (a.c > b.c) ? a.c : b.c);
    fill_cpu(c.w*c.h*c.c, 1, c.data, 1);
    embed_image(a, c, 0, 0); 
    composite_image(b, c, a.w + dx, 0);
    തിരികെ c;
}

image get_label(image **characters, അക്ഷരം *string, പൂർണ്ണസംഖ്യ size)
{
    size = size/10;
    എങ്കിൽ(size > 7) size = 7;
    image label = make_empty_image(0,0,0);
    ആകുമ്പോൾ(*string){
        image l = characters[size][(പൂർണ്ണസംഖ്യ)*string];
        image n = tile_images(label, l, -size - 1 + (size+1)/2);
        free_image(label);
        label = n;
        ++string;
    }
    image b = border_image(label, label.h*.25);
    free_image(label);
    തിരികെ b;
}

ശൂന്യം draw_label(image a, പൂർണ്ണസംഖ്യ r, പൂർണ്ണസംഖ്യ c, image label, സ്ഥിരം ദശാംശം *rgb)
{
    പൂർണ്ണസംഖ്യ w = label.w;
    പൂർണ്ണസംഖ്യ h = label.h;
    എങ്കിൽ (r - h >= 0) r = r - h;

    പൂർണ്ണസംഖ്യ i, j, k;
    വേണ്ടി(j = 0; j < h && j + r < a.h; ++j){
        വേണ്ടി(i = 0; i < w && i + c < a.w; ++i){
            വേണ്ടി(k = 0; k < label.c; ++k){
                ദശാംശം val = get_pixel(label, i, j, k);
                set_pixel(a, i+c, j+r, k, rgb[k] * val);
            }
        }
    }
}

ശൂന്യം draw_box(image a, പൂർണ്ണസംഖ്യ x1, പൂർണ്ണസംഖ്യ y1, പൂർണ്ണസംഖ്യ x2, പൂർണ്ണസംഖ്യ y2, ദശാംശം r, ദശാംശം g, ദശാംശം b)
{
    //normalize_image(a);
    പൂർണ്ണസംഖ്യ i;
    എങ്കിൽ(x1 < 0) x1 = 0;
    എങ്കിൽ(x1 >= a.w) x1 = a.w-1;
    എങ്കിൽ(x2 < 0) x2 = 0;
    എങ്കിൽ(x2 >= a.w) x2 = a.w-1;

    എങ്കിൽ(y1 < 0) y1 = 0;
    എങ്കിൽ(y1 >= a.h) y1 = a.h-1;
    എങ്കിൽ(y2 < 0) y2 = 0;
    എങ്കിൽ(y2 >= a.h) y2 = a.h-1;

    വേണ്ടി(i = x1; i <= x2; ++i){
        a.data[i + y1*a.w + 0*a.w*a.h] = r;
        a.data[i + y2*a.w + 0*a.w*a.h] = r;

        a.data[i + y1*a.w + 1*a.w*a.h] = g;
        a.data[i + y2*a.w + 1*a.w*a.h] = g;

        a.data[i + y1*a.w + 2*a.w*a.h] = b;
        a.data[i + y2*a.w + 2*a.w*a.h] = b;
    }
    വേണ്ടി(i = y1; i <= y2; ++i){
        a.data[x1 + i*a.w + 0*a.w*a.h] = r;
        a.data[x2 + i*a.w + 0*a.w*a.h] = r;

        a.data[x1 + i*a.w + 1*a.w*a.h] = g;
        a.data[x2 + i*a.w + 1*a.w*a.h] = g;

        a.data[x1 + i*a.w + 2*a.w*a.h] = b;
        a.data[x2 + i*a.w + 2*a.w*a.h] = b;
    }
}

ശൂന്യം draw_box_width(image a, പൂർണ്ണസംഖ്യ x1, പൂർണ്ണസംഖ്യ y1, പൂർണ്ണസംഖ്യ x2, പൂർണ്ണസംഖ്യ y2, പൂർണ്ണസംഖ്യ w, ദശാംശം r, ദശാംശം g, ദശാംശം b)
{
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < w; ++i){
        draw_box(a, x1+i, y1+i, x2-i, y2-i, r, g, b);
    }
}

ശൂന്യം draw_bbox(image a, box bbox, പൂർണ്ണസംഖ്യ w, ദശാംശം r, ദശാംശം g, ദശാംശം b)
{
    പൂർണ്ണസംഖ്യ ഇടത്  = (bbox.x-bbox.w/2)*a.w;
    പൂർണ്ണസംഖ്യ വലത് = (bbox.x+bbox.w/2)*a.w;
    പൂർണ്ണസംഖ്യ മുകൾ   = (bbox.y-bbox.h/2)*a.h;
    പൂർണ്ണസംഖ്യ bot   = (bbox.y+bbox.h/2)*a.h;

    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < w; ++i){
        draw_box(a, ഇടത്+i, മുകൾ+i, വലത്-i, bot-i, r, g, b);
    }
}

image **load_alphabet()
{
    പൂർണ്ണസംഖ്യ i, j;
    സ്ഥിരം പൂർണ്ണസംഖ്യ nsize = 8;
    image **alphabets = calloc(nsize, വലുപ്പം(image));
    വേണ്ടി(j = 0; j < nsize; ++j){
        alphabets[j] = calloc(128, വലുപ്പം(image));
        വേണ്ടി(i = 32; i < 127; ++i){
            അക്ഷരം buff[256];
            sprintf(buff, "data/labels/%d_%d.png", i, j);
            alphabets[j][i] = load_image_color(buff, 0, 0);
        }
    }
    തിരികെ alphabets;
}

ശൂന്യം draw_detections(image im, detection *dets, പൂർണ്ണസംഖ്യ അക്കം, ദശാംശം thresh, അക്ഷരം **names, image **alphabet, പൂർണ്ണസംഖ്യ classes)
{
    പൂർണ്ണസംഖ്യ i,j;

    വേണ്ടി(i = 0; i < അക്കം; ++i){
        അക്ഷരം labelstr[4096] = {0};
        പൂർണ്ണസംഖ്യ class = -1;
        വേണ്ടി(j = 0; j < classes; ++j){
            എങ്കിൽ (dets[i].prob[j] > thresh){
                എങ്കിൽ (class < 0) {
                    strcat(labelstr, names[j]);
                    class = j;
                } അല്ലെങ്കിൽ {
                    strcat(labelstr, ", ");
                    strcat(labelstr, names[j]);
                }
                printf("%s: %.0f%%\n", names[j], dets[i].prob[j]*100);
            }
        }
        എങ്കിൽ(class >= 0){
            പൂർണ്ണസംഖ്യ width = im.h * .006;

            /*
               if(0){
               width = pow(prob, 1./2.)*10+1;
               alphabet = 0;
               }
             */

            //printf("%d %s: %.0f%%\n", i, names[class], prob*100);
            പൂർണ്ണസംഖ്യ offset = class*123457 % classes;
            ദശാംശം red = get_color(2,offset,classes);
            ദശാംശം green = get_color(1,offset,classes);
            ദശാംശം blue = get_color(0,offset,classes);
            ദശാംശം rgb[3];

            //width = prob*20+2;

            rgb[0] = red;
            rgb[1] = green;
            rgb[2] = blue;
            box b = dets[i].bbox;
            //printf("%f %f %f %f\n", b.x, b.y, b.w, b.h);

            പൂർണ്ണസംഖ്യ ഇടത്  = (b.x-b.w/2.)*im.w;
            പൂർണ്ണസംഖ്യ വലത് = (b.x+b.w/2.)*im.w;
            പൂർണ്ണസംഖ്യ മുകൾ   = (b.y-b.h/2.)*im.h;
            പൂർണ്ണസംഖ്യ bot   = (b.y+b.h/2.)*im.h;

            എങ്കിൽ(ഇടത് < 0) ഇടത് = 0;
            എങ്കിൽ(വലത് > im.w-1) വലത് = im.w-1;
            എങ്കിൽ(മുകൾ < 0) മുകൾ = 0;
            എങ്കിൽ(bot > im.h-1) bot = im.h-1;

            draw_box_width(im, ഇടത്, മുകൾ, വലത്, bot, width, red, green, blue);
            എങ്കിൽ (alphabet) {
                image label = get_label(alphabet, labelstr, (im.h*.03));
                draw_label(im, മുകൾ + width, ഇടത്, label, rgb);
                free_image(label);
            }
            എങ്കിൽ (dets[i].mask){
                image mask = float_to_image(14, 14, 1, dets[i].mask);
                image resized_mask = resize_image(mask, b.w*im.w, b.h*im.h);
                image tmask = threshold_image(resized_mask, .5);
                embed_image(tmask, im, ഇടത്, മുകൾ);
                free_image(mask);
                free_image(resized_mask);
                free_image(tmask);
            }
        }
    }
}

ശൂന്യം transpose_image(image im)
{
    പ്രതിജ്ഞാ(im.w == im.h);
    പൂർണ്ണസംഖ്യ n, m;
    പൂർണ്ണസംഖ്യ c;
    വേണ്ടി(c = 0; c < im.c; ++c){
        വേണ്ടി(n = 0; n < im.w-1; ++n){
            വേണ്ടി(m = n + 1; m < im.w; ++m){
                ദശാംശം swap = im.data[m + im.w*(n + im.h*c)];
                im.data[m + im.w*(n + im.h*c)] = im.data[n + im.w*(m + im.h*c)];
                im.data[n + im.w*(m + im.h*c)] = swap;
            }
        }
    }
}

ശൂന്യം rotate_image_cw(image im, പൂർണ്ണസംഖ്യ times)
{
    പ്രതിജ്ഞാ(im.w == im.h);
    times = (times + 400) % 4;
    പൂർണ്ണസംഖ്യ i, x, y, c;
    പൂർണ്ണസംഖ്യ n = im.w;
    വേണ്ടി(i = 0; i < times; ++i){
        വേണ്ടി(c = 0; c < im.c; ++c){
            വേണ്ടി(x = 0; x < n/2; ++x){
                വേണ്ടി(y = 0; y < (n-1)/2 + 1; ++y){
                    ദശാംശം temp = im.data[y + im.w*(x + im.h*c)];
                    im.data[y + im.w*(x + im.h*c)] = im.data[n-1-x + im.w*(y + im.h*c)];
                    im.data[n-1-x + im.w*(y + im.h*c)] = im.data[n-1-y + im.w*(n-1-x + im.h*c)];
                    im.data[n-1-y + im.w*(n-1-x + im.h*c)] = im.data[x + im.w*(n-1-y + im.h*c)];
                    im.data[x + im.w*(n-1-y + im.h*c)] = temp;
                }
            }
        }
    }
}

ശൂന്യം flip_image(image a)
{
    പൂർണ്ണസംഖ്യ i,j,k;
    വേണ്ടി(k = 0; k < a.c; ++k){
        വേണ്ടി(i = 0; i < a.h; ++i){
            വേണ്ടി(j = 0; j < a.w/2; ++j){
                പൂർണ്ണസംഖ്യ index = j + a.w*(i + a.h*(k));
                പൂർണ്ണസംഖ്യ flip = (a.w - j - 1) + a.w*(i + a.h*(k));
                ദശാംശം swap = a.data[flip];
                a.data[flip] = a.data[index];
                a.data[index] = swap;
            }
        }
    }
}

image image_distance(image a, image b)
{
    പൂർണ്ണസംഖ്യ i,j;
    image dist = make_image(a.w, a.h, 1);
    വേണ്ടി(i = 0; i < a.c; ++i){
        വേണ്ടി(j = 0; j < a.h*a.w; ++j){
            dist.data[j] += pow(a.data[i*a.h*a.w+j]-b.data[i*a.h*a.w+j],2);
        }
    }
    വേണ്ടി(j = 0; j < a.h*a.w; ++j){
        dist.data[j] = sqrt(dist.data[j]);
    }
    തിരികെ dist;
}

ശൂന്യം ghost_image(image source, image dest, പൂർണ്ണസംഖ്യ dx, പൂർണ്ണസംഖ്യ dy)
{
    പൂർണ്ണസംഖ്യ x,y,k;
    ദശാംശം max_dist = sqrt((-source.w/2. + .5)*(-source.w/2. + .5));
    വേണ്ടി(k = 0; k < source.c; ++k){
        വേണ്ടി(y = 0; y < source.h; ++y){
            വേണ്ടി(x = 0; x < source.w; ++x){
                ദശാംശം dist = sqrt((x - source.w/2. + .5)*(x - source.w/2. + .5) + (y - source.h/2. + .5)*(y - source.h/2. + .5));
                ദശാംശം alpha = (1 - dist/max_dist);
                എങ്കിൽ(alpha < 0) alpha = 0;
                ദശാംശം v1 = get_pixel(source, x,y,k);
                ദശാംശം v2 = get_pixel(dest, dx+x,dy+y,k);
                ദശാംശം val = alpha*v1 + (1-alpha)*v2;
                set_pixel(dest, dx+x, dy+y, k, val);
            }
        }
    }
}

ശൂന്യം blocky_image(image im, പൂർണ്ണസംഖ്യ s)
{
    പൂർണ്ണസംഖ്യ i,j,k;
    വേണ്ടി(k = 0; k < im.c; ++k){
        വേണ്ടി(j = 0; j < im.h; ++j){
            വേണ്ടി(i = 0; i < im.w; ++i){
                im.data[i + im.w*(j + im.h*k)] = im.data[i/s*s + im.w*(j/s*s + im.h*k)];
            }
        }
    }
}

ശൂന്യം censor_image(image im, പൂർണ്ണസംഖ്യ dx, പൂർണ്ണസംഖ്യ dy, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h)
{
    പൂർണ്ണസംഖ്യ i,j,k;
    പൂർണ്ണസംഖ്യ s = 32;
    എങ്കിൽ(dx < 0) dx = 0;
    എങ്കിൽ(dy < 0) dy = 0;

    വേണ്ടി(k = 0; k < im.c; ++k){
        വേണ്ടി(j = dy; j < dy + h && j < im.h; ++j){
            വേണ്ടി(i = dx; i < dx + w && i < im.w; ++i){
                im.data[i + im.w*(j + im.h*k)] = im.data[i/s*s + im.w*(j/s*s + im.h*k)];
                //im.data[i + j*im.w + k*im.w*im.h] = 0;
            }
        }
    }
}

ശൂന്യം embed_image(image source, image dest, പൂർണ്ണസംഖ്യ dx, പൂർണ്ണസംഖ്യ dy)
{
    പൂർണ്ണസംഖ്യ x,y,k;
    വേണ്ടി(k = 0; k < source.c; ++k){
        വേണ്ടി(y = 0; y < source.h; ++y){
            വേണ്ടി(x = 0; x < source.w; ++x){
                ദശാംശം val = get_pixel(source, x,y,k);
                set_pixel(dest, dx+x, dy+y, k, val);
            }
        }
    }
}

image collapse_image_layers(image source, പൂർണ്ണസംഖ്യ border)
{
    പൂർണ്ണസംഖ്യ h = source.h;
    h = (h+border)*source.c - border;
    image dest = make_image(source.w, h, 1);
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < source.c; ++i){
        image layer = get_image_layer(source, i);
        പൂർണ്ണസംഖ്യ h_offset = i*(source.h+border);
        embed_image(layer, dest, 0, h_offset);
        free_image(layer);
    }
    തിരികെ dest;
}

ശൂന്യം constrain_image(image im)
{
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < im.w*im.h*im.c; ++i){
        എങ്കിൽ(im.data[i] < 0) im.data[i] = 0;
        എങ്കിൽ(im.data[i] > 1) im.data[i] = 1;
    }
}

ശൂന്യം normalize_image(image p)
{
    പൂർണ്ണസംഖ്യ i;
    ദശാംശം min = 9999999;
    ദശാംശം max = -999999;

    വേണ്ടി(i = 0; i < p.h*p.w*p.c; ++i){
        ദശാംശം v = p.data[i];
        എങ്കിൽ(v < min) min = v;
        എങ്കിൽ(v > max) max = v;
    }
    എങ്കിൽ(max - min < .000000001){
        min = 0;
        max = 1;
    }
    വേണ്ടി(i = 0; i < p.c*p.w*p.h; ++i){
        p.data[i] = (p.data[i] - min)/(max-min);
    }
}

ശൂന്യം normalize_image2(image p)
{
    ദശാംശം *min = calloc(p.c, വലുപ്പം(ദശാംശം));
    ദശാംശം *max = calloc(p.c, വലുപ്പം(ദശാംശം));
    പൂർണ്ണസംഖ്യ i,j;
    വേണ്ടി(i = 0; i < p.c; ++i) min[i] = max[i] = p.data[i*p.h*p.w];

    വേണ്ടി(j = 0; j < p.c; ++j){
        വേണ്ടി(i = 0; i < p.h*p.w; ++i){
            ദശാംശം v = p.data[i+j*p.h*p.w];
            എങ്കിൽ(v < min[j]) min[j] = v;
            എങ്കിൽ(v > max[j]) max[j] = v;
        }
    }
    വേണ്ടി(i = 0; i < p.c; ++i){
        എങ്കിൽ(max[i] - min[i] < .000000001){
            min[i] = 0;
            max[i] = 1;
        }
    }
    വേണ്ടി(j = 0; j < p.c; ++j){
        വേണ്ടി(i = 0; i < p.w*p.h; ++i){
            p.data[i+j*p.h*p.w] = (p.data[i+j*p.h*p.w] - min[j])/(max[j]-min[j]);
        }
    }
    സ്വതന്ത്രം(min);
    സ്വതന്ത്രം(max);
}

ശൂന്യം copy_image_into(image src, image dest)
{
    memcpy(dest.data, src.data, src.h*src.w*src.c*വലുപ്പം(ദശാംശം));
}

image copy_image(image p)
{
    image copy = p;
    copy.data = calloc(p.h*p.w*p.c, വലുപ്പം(ദശാംശം));
    memcpy(copy.data, p.data, p.h*p.w*p.c*വലുപ്പം(ദശാംശം));
    തിരികെ copy;
}

ശൂന്യം rgbgr_image(image im)
{
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < im.w*im.h; ++i){
        ദശാംശം swap = im.data[i];
        im.data[i] = im.data[i+im.w*im.h*2];
        im.data[i+im.w*im.h*2] = swap;
    }
}

പൂർണ്ണസംഖ്യ show_image(image p, സ്ഥിരം അക്ഷരം *name, പൂർണ്ണസംഖ്യ ms)
{
#ifdef OPENCV
    പൂർണ്ണസംഖ്യ c = show_image_cv(p, name, ms);
    തിരികെ c;
#അല്ലെങ്കിൽ
    fprintf(stderr, "Not compiled with OpenCV, saving to %s.png instead\n", name);
    save_image(p, name);
    തിരികെ -1;
#endif
}

ശൂന്യം save_image_options(image im, സ്ഥിരം അക്ഷരം *name, IMTYPE f, പൂർണ്ണസംഖ്യ quality)
{
    അക്ഷരം buff[256];
    //sprintf(buff, "%s (%d)", name, windows);
    എങ്കിൽ(f == PNG)       sprintf(buff, "%s.png", name);
    അല്ലെങ്കിൽ എങ്കിൽ (f == BMP) sprintf(buff, "%s.bmp", name);
    അല്ലെങ്കിൽ എങ്കിൽ (f == TGA) sprintf(buff, "%s.tga", name);
    അല്ലെങ്കിൽ എങ്കിൽ (f == JPG) sprintf(buff, "%s.jpg", name);
    അല്ലെങ്കിൽ               sprintf(buff, "%s.png", name);
    unsigned അക്ഷരം *data = calloc(im.w*im.h*im.c, വലുപ്പം(അക്ഷരം));
    പൂർണ്ണസംഖ്യ i,k;
    വേണ്ടി(k = 0; k < im.c; ++k){
        വേണ്ടി(i = 0; i < im.w*im.h; ++i){
            data[i*im.c+k] = (unsigned അക്ഷരം) (255*im.data[i + k*im.w*im.h]);
        }
    }
    പൂർണ്ണസംഖ്യ success = 0;
    എങ്കിൽ(f == PNG)       success = stbi_write_png(buff, im.w, im.h, im.c, data, im.w*im.c);
    അല്ലെങ്കിൽ എങ്കിൽ (f == BMP) success = stbi_write_bmp(buff, im.w, im.h, im.c, data);
    അല്ലെങ്കിൽ എങ്കിൽ (f == TGA) success = stbi_write_tga(buff, im.w, im.h, im.c, data);
    അല്ലെങ്കിൽ എങ്കിൽ (f == JPG) success = stbi_write_jpg(buff, im.w, im.h, im.c, data, quality);
    സ്വതന്ത്രം(data);
    എങ്കിൽ(!success) fprintf(stderr, "Failed to write image %s\n", buff);
}

ശൂന്യം save_image(image im, സ്ഥിരം അക്ഷരം *name)
{
    save_image_options(im, name, JPG, 80);
}

ശൂന്യം show_image_layers(image p, അക്ഷരം *name)
{
    പൂർണ്ണസംഖ്യ i;
    അക്ഷരം buff[256];
    വേണ്ടി(i = 0; i < p.c; ++i){
        sprintf(buff, "%s - Layer %d", name, i);
        image layer = get_image_layer(p, i);
        show_image(layer, buff, 1);
        free_image(layer);
    }
}

ശൂന്യം show_image_collapsed(image p, അക്ഷരം *name)
{
    image c = collapse_image_layers(p, 1);
    show_image(c, name, 1);
    free_image(c);
}

image make_empty_image(പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h, പൂർണ്ണസംഖ്യ c)
{
    image out;
    out.data = 0;
    out.h = h;
    out.w = w;
    out.c = c;
    തിരികെ out;
}

image make_image(പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h, പൂർണ്ണസംഖ്യ c)
{
    image out = make_empty_image(w,h,c);
    out.data = calloc(h*w*c, വലുപ്പം(ദശാംശം));
    തിരികെ out;
}

image make_random_image(പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h, പൂർണ്ണസംഖ്യ c)
{
    image out = make_empty_image(w,h,c);
    out.data = calloc(h*w*c, വലുപ്പം(ദശാംശം));
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < w*h*c; ++i){
        out.data[i] = (rand_normal() * .25) + .5;
    }
    തിരികെ out;
}

image float_to_image(പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h, പൂർണ്ണസംഖ്യ c, ദശാംശം *data)
{
    image out = make_empty_image(w,h,c);
    out.data = data;
    തിരികെ out;
}

ശൂന്യം place_image(image im, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h, പൂർണ്ണസംഖ്യ dx, പൂർണ്ണസംഖ്യ dy, image canvas)
{
    പൂർണ്ണസംഖ്യ x, y, c;
    വേണ്ടി(c = 0; c < im.c; ++c){
        വേണ്ടി(y = 0; y < h; ++y){
            വേണ്ടി(x = 0; x < w; ++x){
                ദശാംശം rx = ((ദശാംശം)x / w) * im.w;
                ദശാംശം ry = ((ദശാംശം)y / h) * im.h;
                ദശാംശം val = bilinear_interpolate(im, rx, ry, c);
                set_pixel(canvas, x + dx, y + dy, c, val);
            }
        }
    }
}

image center_crop_image(image im, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h)
{
    പൂർണ്ണസംഖ്യ m = (im.w < im.h) ? im.w : im.h;   
    image c = crop_image(im, (im.w - m) / 2, (im.h - m)/2, m, m);
    image r = resize_image(c, w, h);
    free_image(c);
    തിരികെ r;
}

image rotate_crop_image(image im, ദശാംശം rad, ദശാംശം s, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h, ദശാംശം dx, ദശാംശം dy, ദശാംശം aspect)
{
    പൂർണ്ണസംഖ്യ x, y, c;
    ദശാംശം cx = im.w/2.;
    ദശാംശം cy = im.h/2.;
    image rot = make_image(w, h, im.c);
    വേണ്ടി(c = 0; c < im.c; ++c){
        വേണ്ടി(y = 0; y < h; ++y){
            വേണ്ടി(x = 0; x < w; ++x){
                ദശാംശം rx = cos(rad)*((x - w/2.)/s*aspect + dx/s*aspect) - sin(rad)*((y - h/2.)/s + dy/s) + cx;
                ദശാംശം ry = sin(rad)*((x - w/2.)/s*aspect + dx/s*aspect) + cos(rad)*((y - h/2.)/s + dy/s) + cy;
                ദശാംശം val = bilinear_interpolate(im, rx, ry, c);
                set_pixel(rot, x, y, c, val);
            }
        }
    }
    തിരികെ rot;
}

image rotate_image(image im, ദശാംശം rad)
{
    പൂർണ്ണസംഖ്യ x, y, c;
    ദശാംശം cx = im.w/2.;
    ദശാംശം cy = im.h/2.;
    image rot = make_image(im.w, im.h, im.c);
    വേണ്ടി(c = 0; c < im.c; ++c){
        വേണ്ടി(y = 0; y < im.h; ++y){
            വേണ്ടി(x = 0; x < im.w; ++x){
                ദശാംശം rx = cos(rad)*(x-cx) - sin(rad)*(y-cy) + cx;
                ദശാംശം ry = sin(rad)*(x-cx) + cos(rad)*(y-cy) + cy;
                ദശാംശം val = bilinear_interpolate(im, rx, ry, c);
                set_pixel(rot, x, y, c, val);
            }
        }
    }
    തിരികെ rot;
}

ശൂന്യം fill_image(image m, ദശാംശം s)
{
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < m.h*m.w*m.c; ++i) m.data[i] = s;
}

ശൂന്യം translate_image(image m, ദശാംശം s)
{
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < m.h*m.w*m.c; ++i) m.data[i] += s;
}

ശൂന്യം scale_image(image m, ദശാംശം s)
{
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < m.h*m.w*m.c; ++i) m.data[i] *= s;
}

image crop_image(image im, പൂർണ്ണസംഖ്യ dx, പൂർണ്ണസംഖ്യ dy, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h)
{
    image cropped = make_image(w, h, im.c);
    പൂർണ്ണസംഖ്യ i, j, k;
    വേണ്ടി(k = 0; k < im.c; ++k){
        വേണ്ടി(j = 0; j < h; ++j){
            വേണ്ടി(i = 0; i < w; ++i){
                പൂർണ്ണസംഖ്യ r = j + dy;
                പൂർണ്ണസംഖ്യ c = i + dx;
                ദശാംശം val = 0;
                r = constrain_int(r, 0, im.h-1);
                c = constrain_int(c, 0, im.w-1);
                val = get_pixel(im, c, r, k);
                set_pixel(cropped, i, j, k, val);
            }
        }
    }
    തിരികെ cropped;
}

പൂർണ്ണസംഖ്യ best_3d_shift_r(image a, image b, പൂർണ്ണസംഖ്യ min, പൂർണ്ണസംഖ്യ max)
{
    എങ്കിൽ(min == max) തിരികെ min;
    പൂർണ്ണസംഖ്യ mid = floor((min + max) / 2.);
    image c1 = crop_image(b, 0, mid, b.w, b.h);
    image c2 = crop_image(b, 0, mid+1, b.w, b.h);
    ദശാംശം d1 = dist_array(c1.data, a.data, a.w*a.h*a.c, 10);
    ദശാംശം d2 = dist_array(c2.data, a.data, a.w*a.h*a.c, 10);
    free_image(c1);
    free_image(c2);
    എങ്കിൽ(d1 < d2) തിരികെ best_3d_shift_r(a, b, min, mid);
    അല്ലെങ്കിൽ തിരികെ best_3d_shift_r(a, b, mid+1, max);
}

പൂർണ്ണസംഖ്യ best_3d_shift(image a, image b, പൂർണ്ണസംഖ്യ min, പൂർണ്ണസംഖ്യ max)
{
    പൂർണ്ണസംഖ്യ i;
    പൂർണ്ണസംഖ്യ best = 0;
    ദശാംശം best_distance = FLT_MAX;
    വേണ്ടി(i = min; i <= max; i += 2){
        image c = crop_image(b, 0, i, b.w, b.h);
        ദശാംശം d = dist_array(c.data, a.data, a.w*a.h*a.c, 100);
        എങ്കിൽ(d < best_distance){
            best_distance = d;
            best = i;
        }
        printf("%d %f\n", i, d);
        free_image(c);
    }
    തിരികെ best;
}

ശൂന്യം composite_3d(അക്ഷരം *f1, അക്ഷരം *f2, അക്ഷരം *out, പൂർണ്ണസംഖ്യ delta)
{
    എങ്കിൽ(!out) out = "out";
    image a = load_image(f1, 0,0,0);
    image b = load_image(f2, 0,0,0);
    പൂർണ്ണസംഖ്യ shift = best_3d_shift_r(a, b, -a.h/100, a.h/100);

    image c1 = crop_image(b, 10, shift, b.w, b.h);
    ദശാംശം d1 = dist_array(c1.data, a.data, a.w*a.h*a.c, 100);
    image c2 = crop_image(b, -10, shift, b.w, b.h);
    ദശാംശം d2 = dist_array(c2.data, a.data, a.w*a.h*a.c, 100);

    എങ്കിൽ(d2 < d1 && 0){
        image swap = a;
        a = b;
        b = swap;
        shift = -shift;
        printf("swapped, %d\n", shift);
    }
    അല്ലെങ്കിൽ{
        printf("%d\n", shift);
    }

    image c = crop_image(b, delta, shift, a.w, a.h);
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < c.w*c.h; ++i){
        c.data[i] = a.data[i];
    }
    save_image(c, out);
}

ശൂന്യം letterbox_image_into(image im, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h, image boxed)
{
    പൂർണ്ണസംഖ്യ new_w = im.w;
    പൂർണ്ണസംഖ്യ new_h = im.h;
    എങ്കിൽ (((ദശാംശം)w/im.w) < ((ദശാംശം)h/im.h)) {
        new_w = w;
        new_h = (im.h * w)/im.w;
    } അല്ലെങ്കിൽ {
        new_h = h;
        new_w = (im.w * h)/im.h;
    }
    image resized = resize_image(im, new_w, new_h);
    embed_image(resized, boxed, (w-new_w)/2, (h-new_h)/2); 
    free_image(resized);
}

image letterbox_image(image im, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h)
{
    പൂർണ്ണസംഖ്യ new_w = im.w;
    പൂർണ്ണസംഖ്യ new_h = im.h;
    എങ്കിൽ (((ദശാംശം)w/im.w) < ((ദശാംശം)h/im.h)) {
        new_w = w;
        new_h = (im.h * w)/im.w;
    } അല്ലെങ്കിൽ {
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
    തിരികെ boxed;
}

image resize_max(image im, പൂർണ്ണസംഖ്യ max)
{
    പൂർണ്ണസംഖ്യ w = im.w;
    പൂർണ്ണസംഖ്യ h = im.h;
    എങ്കിൽ(w > h){
        h = (h * max) / w;
        w = max;
    } അല്ലെങ്കിൽ {
        w = (w * max) / h;
        h = max;
    }
    എങ്കിൽ(w == im.w && h == im.h) തിരികെ im;
    image resized = resize_image(im, w, h);
    തിരികെ resized;
}

image resize_min(image im, പൂർണ്ണസംഖ്യ min)
{
    പൂർണ്ണസംഖ്യ w = im.w;
    പൂർണ്ണസംഖ്യ h = im.h;
    എങ്കിൽ(w < h){
        h = (h * min) / w;
        w = min;
    } അല്ലെങ്കിൽ {
        w = (w * min) / h;
        h = min;
    }
    എങ്കിൽ(w == im.w && h == im.h) തിരികെ im;
    image resized = resize_image(im, w, h);
    തിരികെ resized;
}

image random_crop_image(image im, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h)
{
    പൂർണ്ണസംഖ്യ dx = rand_int(0, im.w - w);
    പൂർണ്ണസംഖ്യ dy = rand_int(0, im.h - h);
    image crop = crop_image(im, dx, dy, w, h);
    തിരികെ crop;
}

augment_args random_augment_args(image im, ദശാംശം angle, ദശാംശം aspect, പൂർണ്ണസംഖ്യ താഴെയുള്ള, പൂർണ്ണസംഖ്യ മുകളിലെ, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h)
{
    augment_args a = {0};
    aspect = rand_scale(aspect);
    പൂർണ്ണസംഖ്യ r = rand_int(താഴെയുള്ള, മുകളിലെ);
    പൂർണ്ണസംഖ്യ min = (im.h < im.w*aspect) ? im.h : im.w*aspect;
    ദശാംശം scale = (ദശാംശം)r / min;

    ദശാംശം rad = rand_uniform(-angle, angle) * TWO_PI / 360.;

    ദശാംശം dx = (im.w*scale/aspect - w) / 2.;
    ദശാംശം dy = (im.h*scale - w) / 2.;
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
    തിരികെ a;
}

image random_augment_image(image im, ദശാംശം angle, ദശാംശം aspect, പൂർണ്ണസംഖ്യ താഴെയുള്ള, പൂർണ്ണസംഖ്യ മുകളിലെ, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h)
{
    augment_args a = random_augment_args(im, angle, aspect, താഴെയുള്ള, മുകളിലെ, w, h);
    image crop = rotate_crop_image(im, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);
    തിരികെ crop;
}

ദശാംശം three_way_max(ദശാംശം a, ദശാംശം b, ദശാംശം c)
{
    തിരികെ (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

ദശാംശം three_way_min(ദശാംശം a, ദശാംശം b, ദശാംശം c)
{
    തിരികെ (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

ശൂന്യം yuv_to_rgb(image im)
{
    പ്രതിജ്ഞാ(im.c == 3);
    പൂർണ്ണസംഖ്യ i, j;
    ദശാംശം r, g, b;
    ദശാംശം y, u, v;
    വേണ്ടി(j = 0; j < im.h; ++j){
        വേണ്ടി(i = 0; i < im.w; ++i){
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

ശൂന്യം rgb_to_yuv(image im)
{
    പ്രതിജ്ഞാ(im.c == 3);
    പൂർണ്ണസംഖ്യ i, j;
    ദശാംശം r, g, b;
    ദശാംശം y, u, v;
    വേണ്ടി(j = 0; j < im.h; ++j){
        വേണ്ടി(i = 0; i < im.w; ++i){
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
ശൂന്യം rgb_to_hsv(image im)
{
    പ്രതിജ്ഞാ(im.c == 3);
    പൂർണ്ണസംഖ്യ i, j;
    ദശാംശം r, g, b;
    ദശാംശം h, s, v;
    വേണ്ടി(j = 0; j < im.h; ++j){
        വേണ്ടി(i = 0; i < im.w; ++i){
            r = get_pixel(im, i , j, 0);
            g = get_pixel(im, i , j, 1);
            b = get_pixel(im, i , j, 2);
            ദശാംശം max = three_way_max(r,g,b);
            ദശാംശം min = three_way_min(r,g,b);
            ദശാംശം delta = max - min;
            v = max;
            എങ്കിൽ(max == 0){
                s = 0;
                h = 0;
            }അല്ലെങ്കിൽ{
                s = delta/max;
                എങ്കിൽ(r == max){
                    h = (g - b) / delta;
                } അല്ലെങ്കിൽ എങ്കിൽ (g == max) {
                    h = 2 + (b - r) / delta;
                } അല്ലെങ്കിൽ {
                    h = 4 + (r - g) / delta;
                }
                എങ്കിൽ (h < 0) h += 6;
                h = h/6.;
            }
            set_pixel(im, i, j, 0, h);
            set_pixel(im, i, j, 1, s);
            set_pixel(im, i, j, 2, v);
        }
    }
}

ശൂന്യം hsv_to_rgb(image im)
{
    പ്രതിജ്ഞാ(im.c == 3);
    പൂർണ്ണസംഖ്യ i, j;
    ദശാംശം r, g, b;
    ദശാംശം h, s, v;
    ദശാംശം f, p, q, t;
    വേണ്ടി(j = 0; j < im.h; ++j){
        വേണ്ടി(i = 0; i < im.w; ++i){
            h = 6 * get_pixel(im, i , j, 0);
            s = get_pixel(im, i , j, 1);
            v = get_pixel(im, i , j, 2);
            എങ്കിൽ (s == 0) {
                r = g = b = v;
            } അല്ലെങ്കിൽ {
                പൂർണ്ണസംഖ്യ index = floor(h);
                f = h - index;
                p = v*(1-s);
                q = v*(1-s*f);
                t = v*(1-s*(1-f));
                എങ്കിൽ(index == 0){
                    r = v; g = t; b = p;
                } അല്ലെങ്കിൽ എങ്കിൽ(index == 1){
                    r = q; g = v; b = p;
                } അല്ലെങ്കിൽ എങ്കിൽ(index == 2){
                    r = p; g = v; b = t;
                } അല്ലെങ്കിൽ എങ്കിൽ(index == 3){
                    r = p; g = q; b = v;
                } അല്ലെങ്കിൽ എങ്കിൽ(index == 4){
                    r = t; g = p; b = v;
                } അല്ലെങ്കിൽ {
                    r = v; g = p; b = q;
                }
            }
            set_pixel(im, i, j, 0, r);
            set_pixel(im, i, j, 1, g);
            set_pixel(im, i, j, 2, b);
        }
    }
}

ശൂന്യം grayscale_image_3c(image im)
{
    പ്രതിജ്ഞാ(im.c == 3);
    പൂർണ്ണസംഖ്യ i, j, k;
    ദശാംശം scale[] = {0.299, 0.587, 0.114};
    വേണ്ടി(j = 0; j < im.h; ++j){
        വേണ്ടി(i = 0; i < im.w; ++i){
            ദശാംശം val = 0;
            വേണ്ടി(k = 0; k < 3; ++k){
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
    പ്രതിജ്ഞാ(im.c == 3);
    പൂർണ്ണസംഖ്യ i, j, k;
    image gray = make_image(im.w, im.h, 1);
    ദശാംശം scale[] = {0.299, 0.587, 0.114};
    വേണ്ടി(k = 0; k < im.c; ++k){
        വേണ്ടി(j = 0; j < im.h; ++j){
            വേണ്ടി(i = 0; i < im.w; ++i){
                gray.data[i+im.w*j] += scale[k]*get_pixel(im, i, j, k);
            }
        }
    }
    തിരികെ gray;
}

image threshold_image(image im, ദശാംശം thresh)
{
    പൂർണ്ണസംഖ്യ i;
    image t = make_image(im.w, im.h, im.c);
    വേണ്ടി(i = 0; i < im.w*im.h*im.c; ++i){
        t.data[i] = im.data[i]>thresh ? 1 : 0;
    }
    തിരികെ t;
}

image blend_image(image fore, image back, ദശാംശം alpha)
{
    പ്രതിജ്ഞാ(fore.w == back.w && fore.h == back.h && fore.c == back.c);
    image blend = make_image(fore.w, fore.h, fore.c);
    പൂർണ്ണസംഖ്യ i, j, k;
    വേണ്ടി(k = 0; k < fore.c; ++k){
        വേണ്ടി(j = 0; j < fore.h; ++j){
            വേണ്ടി(i = 0; i < fore.w; ++i){
                ദശാംശം val = alpha * get_pixel(fore, i, j, k) + 
                    (1 - alpha)* get_pixel(back, i, j, k);
                set_pixel(blend, i, j, k, val);
            }
        }
    }
    തിരികെ blend;
}

ശൂന്യം scale_image_channel(image im, പൂർണ്ണസംഖ്യ c, ദശാംശം v)
{
    പൂർണ്ണസംഖ്യ i, j;
    വേണ്ടി(j = 0; j < im.h; ++j){
        വേണ്ടി(i = 0; i < im.w; ++i){
            ദശാംശം pix = get_pixel(im, i, j, c);
            pix = pix*v;
            set_pixel(im, i, j, c, pix);
        }
    }
}

ശൂന്യം translate_image_channel(image im, പൂർണ്ണസംഖ്യ c, ദശാംശം v)
{
    പൂർണ്ണസംഖ്യ i, j;
    വേണ്ടി(j = 0; j < im.h; ++j){
        വേണ്ടി(i = 0; i < im.w; ++i){
            ദശാംശം pix = get_pixel(im, i, j, c);
            pix = pix+v;
            set_pixel(im, i, j, c, pix);
        }
    }
}

image binarize_image(image im)
{
    image c = copy_image(im);
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < im.w * im.h * im.c; ++i){
        എങ്കിൽ(c.data[i] > .5) c.data[i] = 1;
        അല്ലെങ്കിൽ c.data[i] = 0;
    }
    തിരികെ c;
}

ശൂന്യം saturate_image(image im, ദശാംശം sat)
{
    rgb_to_hsv(im);
    scale_image_channel(im, 1, sat);
    hsv_to_rgb(im);
    constrain_image(im);
}

ശൂന്യം hue_image(image im, ദശാംശം hue)
{
    rgb_to_hsv(im);
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < im.w*im.h; ++i){
        im.data[i] = im.data[i] + hue;
        എങ്കിൽ (im.data[i] > 1) im.data[i] -= 1;
        എങ്കിൽ (im.data[i] < 0) im.data[i] += 1;
    }
    hsv_to_rgb(im);
    constrain_image(im);
}

ശൂന്യം exposure_image(image im, ദശാംശം sat)
{
    rgb_to_hsv(im);
    scale_image_channel(im, 2, sat);
    hsv_to_rgb(im);
    constrain_image(im);
}

ശൂന്യം distort_image(image im, ദശാംശം hue, ദശാംശം sat, ദശാംശം val)
{
    rgb_to_hsv(im);
    scale_image_channel(im, 1, sat);
    scale_image_channel(im, 2, val);
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < im.w*im.h; ++i){
        im.data[i] = im.data[i] + hue;
        എങ്കിൽ (im.data[i] > 1) im.data[i] -= 1;
        എങ്കിൽ (im.data[i] < 0) im.data[i] += 1;
    }
    hsv_to_rgb(im);
    constrain_image(im);
}

ശൂന്യം random_distort_image(image im, ദശാംശം hue, ദശാംശം saturation, ദശാംശം exposure)
{
    ദശാംശം dhue = rand_uniform(-hue, hue);
    ദശാംശം dsat = rand_scale(saturation);
    ദശാംശം dexp = rand_scale(exposure);
    distort_image(im, dhue, dsat, dexp);
}

ശൂന്യം saturate_exposure_image(image im, ദശാംശം sat, ദശാംശം exposure)
{
    rgb_to_hsv(im);
    scale_image_channel(im, 1, sat);
    scale_image_channel(im, 2, exposure);
    hsv_to_rgb(im);
    constrain_image(im);
}

image resize_image(image im, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h)
{
    image resized = make_image(w, h, im.c);   
    image part = make_image(w, im.h, im.c);
    പൂർണ്ണസംഖ്യ r, c, k;
    ദശാംശം w_scale = (ദശാംശം)(im.w - 1) / (w - 1);
    ദശാംശം h_scale = (ദശാംശം)(im.h - 1) / (h - 1);
    വേണ്ടി(k = 0; k < im.c; ++k){
        വേണ്ടി(r = 0; r < im.h; ++r){
            വേണ്ടി(c = 0; c < w; ++c){
                ദശാംശം val = 0;
                എങ്കിൽ(c == w-1 || im.w == 1){
                    val = get_pixel(im, im.w-1, r, k);
                } അല്ലെങ്കിൽ {
                    ദശാംശം sx = c*w_scale;
                    പൂർണ്ണസംഖ്യ ix = (പൂർണ്ണസംഖ്യ) sx;
                    ദശാംശം dx = sx - ix;
                    val = (1 - dx) * get_pixel(im, ix, r, k) + dx * get_pixel(im, ix+1, r, k);
                }
                set_pixel(part, c, r, k, val);
            }
        }
    }
    വേണ്ടി(k = 0; k < im.c; ++k){
        വേണ്ടി(r = 0; r < h; ++r){
            ദശാംശം sy = r*h_scale;
            പൂർണ്ണസംഖ്യ iy = (പൂർണ്ണസംഖ്യ) sy;
            ദശാംശം dy = sy - iy;
            വേണ്ടി(c = 0; c < w; ++c){
                ദശാംശം val = (1-dy) * get_pixel(part, c, iy, k);
                set_pixel(resized, c, r, k, val);
            }
            എങ്കിൽ(r == h-1 || im.h == 1) തുടരുക;
            വേണ്ടി(c = 0; c < w; ++c){
                ദശാംശം val = dy * get_pixel(part, c, iy+1, k);
                add_pixel(resized, c, r, k, val);
            }
        }
    }

    free_image(part);
    തിരികെ resized;
}


ശൂന്യം test_resize(അക്ഷരം *filename)
{
    image im = load_image(filename, 0,0, 3);
    ദശാംശം mag = mag_array(im.data, im.w*im.h*im.c);
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
    ആകുമ്പോൾ(1){
        image aug = random_augment_image(im, 0, .75, 320, 448, 320, 320);
        show_image(aug, "aug", 1);
        free_image(aug);


        ദശാംശം exposure = 1.15;
        ദശാംശം saturation = 1.15;
        ദശാംശം hue = .05;

        image c = copy_image(im);

        ദശാംശം dexp = rand_scale(exposure);
        ദശാംശം dsat = rand_scale(saturation);
        ദശാംശം dhue = rand_uniform(-hue, hue);

        distort_image(c, dhue, dsat, dexp);
        show_image(c, "rand", 1);
        printf("%f %f %f\n", dhue, dsat, dexp);
        free_image(c);
    }
#endif
}


image load_image_stb(അക്ഷരം *filename, പൂർണ്ണസംഖ്യ channels)
{
    പൂർണ്ണസംഖ്യ w, h, c;
    unsigned അക്ഷരം *data = stbi_load(filename, &w, &h, &c, channels);
    എങ്കിൽ (!data) {
        fprintf(stderr, "Cannot load image \"%s\"\nSTB Reason: %s\n", filename, stbi_failure_reason());
        exit(0);
    }
    എങ്കിൽ(channels) c = channels;
    പൂർണ്ണസംഖ്യ i,j,k;
    image im = make_image(w, h, c);
    വേണ്ടി(k = 0; k < c; ++k){
        വേണ്ടി(j = 0; j < h; ++j){
            വേണ്ടി(i = 0; i < w; ++i){
                പൂർണ്ണസംഖ്യ dst_index = i + w*j + w*h*k;
                പൂർണ്ണസംഖ്യ src_index = k + c*i + c*w*j;
                im.data[dst_index] = (ദശാംശം)data[src_index]/255.;
            }
        }
    }
    സ്വതന്ത്രം(data);
    തിരികെ im;
}

image load_image(അക്ഷരം *filename, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h, പൂർണ്ണസംഖ്യ c)
{
#ifdef OPENCV
    image out = load_image_cv(filename, c);
#അല്ലെങ്കിൽ
    image out = load_image_stb(filename, c);
#endif

    എങ്കിൽ((h && w) && (h != out.h || w != out.w)){
        image resized = resize_image(out, w, h);
        free_image(out);
        out = resized;
    }
    തിരികെ out;
}

image load_image_color(അക്ഷരം *filename, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h)
{
    തിരികെ load_image(filename, w, h, 3);
}

image get_image_layer(image m, പൂർണ്ണസംഖ്യ l)
{
    image out = make_image(m.w, m.h, 1);
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < m.h*m.w; ++i){
        out.data[i] = m.data[i+l*m.h*m.w];
    }
    തിരികെ out;
}
ശൂന്യം print_image(image m)
{
    പൂർണ്ണസംഖ്യ i, j, k;
    വേണ്ടി(i =0 ; i < m.c; ++i){
        വേണ്ടി(j =0 ; j < m.h; ++j){
            വേണ്ടി(k = 0; k < m.w; ++k){
                printf("%.2lf, ", m.data[i*m.h*m.w + j*m.w + k]);
                എങ്കിൽ(k > 30) നിർത്തുക;
            }
            printf("\n");
            എങ്കിൽ(j > 30) നിർത്തുക;
        }
        printf("\n");
    }
    printf("\n");
}

image collapse_images_vert(image *ims, പൂർണ്ണസംഖ്യ n)
{
    പൂർണ്ണസംഖ്യ color = 1;
    പൂർണ്ണസംഖ്യ border = 1;
    പൂർണ്ണസംഖ്യ h,w,c;
    w = ims[0].w;
    h = (ims[0].h + border) * n - border;
    c = ims[0].c;
    എങ്കിൽ(c != 3 || !color){
        w = (w+border)*c - border;
        c = 1;
    }

    image filters = make_image(w, h, c);
    പൂർണ്ണസംഖ്യ i,j;
    വേണ്ടി(i = 0; i < n; ++i){
        പൂർണ്ണസംഖ്യ h_offset = i*(ims[0].h+border);
        image copy = copy_image(ims[i]);
        //normalize_image(copy);
        എങ്കിൽ(c == 3 && color){
            embed_image(copy, filters, 0, h_offset);
        }
        അല്ലെങ്കിൽ{
            വേണ്ടി(j = 0; j < copy.c; ++j){
                പൂർണ്ണസംഖ്യ w_offset = j*(ims[0].w+border);
                image layer = get_image_layer(copy, j);
                embed_image(layer, filters, w_offset, h_offset);
                free_image(layer);
            }
        }
        free_image(copy);
    }
    തിരികെ filters;
} 

image collapse_images_horz(image *ims, പൂർണ്ണസംഖ്യ n)
{
    പൂർണ്ണസംഖ്യ color = 1;
    പൂർണ്ണസംഖ്യ border = 1;
    പൂർണ്ണസംഖ്യ h,w,c;
    പൂർണ്ണസംഖ്യ size = ims[0].h;
    h = size;
    w = (ims[0].w + border) * n - border;
    c = ims[0].c;
    എങ്കിൽ(c != 3 || !color){
        h = (h+border)*c - border;
        c = 1;
    }

    image filters = make_image(w, h, c);
    പൂർണ്ണസംഖ്യ i,j;
    വേണ്ടി(i = 0; i < n; ++i){
        പൂർണ്ണസംഖ്യ w_offset = i*(size+border);
        image copy = copy_image(ims[i]);
        //normalize_image(copy);
        എങ്കിൽ(c == 3 && color){
            embed_image(copy, filters, w_offset, 0);
        }
        അല്ലെങ്കിൽ{
            വേണ്ടി(j = 0; j < copy.c; ++j){
                പൂർണ്ണസംഖ്യ h_offset = j*(size+border);
                image layer = get_image_layer(copy, j);
                embed_image(layer, filters, w_offset, h_offset);
                free_image(layer);
            }
        }
        free_image(copy);
    }
    തിരികെ filters;
} 

ശൂന്യം show_image_normalized(image im, സ്ഥിരം അക്ഷരം *name)
{
    image c = copy_image(im);
    normalize_image(c);
    show_image(c, name, 1);
    free_image(c);
}

ശൂന്യം show_images(image *ims, പൂർണ്ണസംഖ്യ n, അക്ഷരം *window)
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

ശൂന്യം free_image(image m)
{
    എങ്കിൽ(m.data){
        സ്വതന്ത്രം(m.data);
    }
}
