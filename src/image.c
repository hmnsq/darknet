#অন্তর্ভাৱ "image.h"
#অন্তর্ভাৱ "utils.h"
#অন্তর্ভাৱ "blas.h"
#অন্তর্ভাৱ "cuda.h"
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <math.h>

#define STB_IMAGE_IMPLEMENTATION
#অন্তর্ভাৱ "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#অন্তর্ভাৱ "stb_image_write.h"

পূর্ণাঙ্ক windows = 0;

ভিন্ন colors[6][3] = { {1,0,1}, {0,0,1},{0,1,1},{0,1,0},{1,1,0},{1,0,0} };

ভিন্ন get_color(পূর্ণাঙ্ক c, পূর্ণাঙ্ক x, পূর্ণাঙ্ক max)
{
    ভিন্ন ratio = ((ভিন্ন)x/max)*5;
    পূর্ণাঙ্ক i = floor(ratio);
    পূর্ণাঙ্ক j = ceil(ratio);
    ratio -= i;
    ভিন্ন r = (1-ratio) * colors[i][c] + ratio*colors[j][c];
    //printf("%f\n", r);
    ঘূৰাই r;
}

image mask_to_rgb(image mask)
{
    পূর্ণাঙ্ক n = mask.c;
    image im = make_image(mask.w, mask.h, 3);
    পূর্ণাঙ্ক i, j;
    বাবে(j = 0; j < n; ++j){
        পূর্ণাঙ্ক offset = j*123457 % n;
        ভিন্ন red = get_color(2,offset,n);
        ভিন্ন green = get_color(1,offset,n);
        ভিন্ন blue = get_color(0,offset,n);
        বাবে(i = 0; i < im.w*im.h; ++i){
            im.data[i + 0*im.w*im.h] += mask.data[j*im.h*im.w + i]*red;
            im.data[i + 1*im.w*im.h] += mask.data[j*im.h*im.w + i]*green;
            im.data[i + 2*im.w*im.h] += mask.data[j*im.h*im.w + i]*blue;
        }
    }
    ঘূৰাই im;
}

স্থির ভিন্ন get_pixel(image m, পূর্ণাঙ্ক x, পূর্ণাঙ্ক y, পূর্ণাঙ্ক c)
{
    প্রতিজ্ঞা(x < m.w && y < m.h && c < m.c);
    ঘূৰাই m.data[c*m.h*m.w + y*m.w + x];
}
স্থির ভিন্ন get_pixel_extend(image m, পূর্ণাঙ্ক x, পূর্ণাঙ্ক y, পূর্ণাঙ্ক c)
{
    যদি(x < 0 || x >= m.w || y < 0 || y >= m.h) ঘূৰাই 0;
    /*
    if(x < 0) x = 0;
    if(x >= m.w) x = m.w-1;
    if(y < 0) y = 0;
    if(y >= m.h) y = m.h-1;
    */
    যদি(c < 0 || c >= m.c) ঘূৰাই 0;
    ঘূৰাই get_pixel(m, x, y, c);
}
স্থির শূন্য set_pixel(image m, পূর্ণাঙ্ক x, পূর্ণাঙ্ক y, পূর্ণাঙ্ক c, ভিন্ন val)
{
    যদি (x < 0 || y < 0 || c < 0 || x >= m.w || y >= m.h || c >= m.c) ঘূৰাই;
    প্রতিজ্ঞা(x < m.w && y < m.h && c < m.c);
    m.data[c*m.h*m.w + y*m.w + x] = val;
}
স্থির শূন্য add_pixel(image m, পূর্ণাঙ্ক x, পূর্ণাঙ্ক y, পূর্ণাঙ্ক c, ভিন্ন val)
{
    প্রতিজ্ঞা(x < m.w && y < m.h && c < m.c);
    m.data[c*m.h*m.w + y*m.w + x] += val;
}

স্থির ভিন্ন bilinear_interpolate(image im, ভিন্ন x, ভিন্ন y, পূর্ণাঙ্ক c)
{
    পূর্ণাঙ্ক ix = (পূর্ণাঙ্ক) floorf(x);
    পূর্ণাঙ্ক iy = (পূর্ণাঙ্ক) floorf(y);

    ভিন্ন dx = x - ix;
    ভিন্ন dy = y - iy;

    ভিন্ন val = (1-dy) * (1-dx) * get_pixel_extend(im, ix, iy, c) + 
        dy     * (1-dx) * get_pixel_extend(im, ix, iy+1, c) + 
        (1-dy) *   dx   * get_pixel_extend(im, ix+1, iy, c) +
        dy     *   dx   * get_pixel_extend(im, ix+1, iy+1, c);
    ঘূৰাই val;
}


শূন্য composite_image(image source, image dest, পূর্ণাঙ্ক dx, পূর্ণাঙ্ক dy)
{
    পূর্ণাঙ্ক x,y,k;
    বাবে(k = 0; k < source.c; ++k){
        বাবে(y = 0; y < source.h; ++y){
            বাবে(x = 0; x < source.w; ++x){
                ভিন্ন val = get_pixel(source, x, y, k);
                ভিন্ন val2 = get_pixel_extend(dest, dx+x, dy+y, k);
                set_pixel(dest, dx+x, dy+y, k, val * val2);
            }
        }
    }
}

image border_image(image a, পূর্ণাঙ্ক border)
{
    image b = make_image(a.w + 2*border, a.h + 2*border, a.c);
    পূর্ণাঙ্ক x,y,k;
    বাবে(k = 0; k < b.c; ++k){
        বাবে(y = 0; y < b.h; ++y){
            বাবে(x = 0; x < b.w; ++x){
                ভিন্ন val = get_pixel_extend(a, x - border, y - border, k);
                যদি(x - border < 0 || x - border >= a.w || y - border < 0 || y - border >= a.h) val = 1;
                set_pixel(b, x, y, k, val);
            }
        }
    }
    ঘূৰাই b;
}

image tile_images(image a, image b, পূর্ণাঙ্ক dx)
{
    যদি(a.w == 0) ঘূৰাই copy_image(b);
    image c = make_image(a.w + b.w + dx, (a.h > b.h) ? a.h : b.h, (a.c > b.c) ? a.c : b.c);
    fill_cpu(c.w*c.h*c.c, 1, c.data, 1);
    embed_image(a, c, 0, 0); 
    composite_image(b, c, a.w + dx, 0);
    ঘূৰাই c;
}

image get_label(image **characters, ৱর্ণ *string, পূর্ণাঙ্ক size)
{
    size = size/10;
    যদি(size > 7) size = 7;
    image label = make_empty_image(0,0,0);
    যেতিয়ালৈ(*string){
        image l = characters[size][(পূর্ণাঙ্ক)*string];
        image n = tile_images(label, l, -size - 1 + (size+1)/2);
        free_image(label);
        label = n;
        ++string;
    }
    image b = border_image(label, label.h*.25);
    free_image(label);
    ঘূৰাই b;
}

শূন্য draw_label(image a, পূর্ণাঙ্ক r, পূর্ণাঙ্ক c, image label, ধ্রুৱ ভিন্ন *rgb)
{
    পূর্ণাঙ্ক w = label.w;
    পূর্ণাঙ্ক h = label.h;
    যদি (r - h >= 0) r = r - h;

    পূর্ণাঙ্ক i, j, k;
    বাবে(j = 0; j < h && j + r < a.h; ++j){
        বাবে(i = 0; i < w && i + c < a.w; ++i){
            বাবে(k = 0; k < label.c; ++k){
                ভিন্ন val = get_pixel(label, i, j, k);
                set_pixel(a, i+c, j+r, k, rgb[k] * val);
            }
        }
    }
}

শূন্য draw_box(image a, পূর্ণাঙ্ক x1, পূর্ণাঙ্ক y1, পূর্ণাঙ্ক x2, পূর্ণাঙ্ক y2, ভিন্ন r, ভিন্ন g, ভিন্ন b)
{
    //normalize_image(a);
    পূর্ণাঙ্ক i;
    যদি(x1 < 0) x1 = 0;
    যদি(x1 >= a.w) x1 = a.w-1;
    যদি(x2 < 0) x2 = 0;
    যদি(x2 >= a.w) x2 = a.w-1;

    যদি(y1 < 0) y1 = 0;
    যদি(y1 >= a.h) y1 = a.h-1;
    যদি(y2 < 0) y2 = 0;
    যদি(y2 >= a.h) y2 = a.h-1;

    বাবে(i = x1; i <= x2; ++i){
        a.data[i + y1*a.w + 0*a.w*a.h] = r;
        a.data[i + y2*a.w + 0*a.w*a.h] = r;

        a.data[i + y1*a.w + 1*a.w*a.h] = g;
        a.data[i + y2*a.w + 1*a.w*a.h] = g;

        a.data[i + y1*a.w + 2*a.w*a.h] = b;
        a.data[i + y2*a.w + 2*a.w*a.h] = b;
    }
    বাবে(i = y1; i <= y2; ++i){
        a.data[x1 + i*a.w + 0*a.w*a.h] = r;
        a.data[x2 + i*a.w + 0*a.w*a.h] = r;

        a.data[x1 + i*a.w + 1*a.w*a.h] = g;
        a.data[x2 + i*a.w + 1*a.w*a.h] = g;

        a.data[x1 + i*a.w + 2*a.w*a.h] = b;
        a.data[x2 + i*a.w + 2*a.w*a.h] = b;
    }
}

শূন্য draw_box_width(image a, পূর্ণাঙ্ক x1, পূর্ণাঙ্ক y1, পূর্ণাঙ্ক x2, পূর্ণাঙ্ক y2, পূর্ণাঙ্ক w, ভিন্ন r, ভিন্ন g, ভিন্ন b)
{
    পূর্ণাঙ্ক i;
    বাবে(i = 0; i < w; ++i){
        draw_box(a, x1+i, y1+i, x2-i, y2-i, r, g, b);
    }
}

শূন্য draw_bbox(image a, box bbox, পূর্ণাঙ্ক w, ভিন্ন r, ভিন্ন g, ভিন্ন b)
{
    পূর্ণাঙ্ক বাম  = (bbox.x-bbox.w/2)*a.w;
    পূর্ণাঙ্ক ডান = (bbox.x+bbox.w/2)*a.w;
    পূর্ণাঙ্ক শীৰ্ষ   = (bbox.y-bbox.h/2)*a.h;
    পূর্ণাঙ্ক bot   = (bbox.y+bbox.h/2)*a.h;

    পূর্ণাঙ্ক i;
    বাবে(i = 0; i < w; ++i){
        draw_box(a, বাম+i, শীৰ্ষ+i, ডান-i, bot-i, r, g, b);
    }
}

image **load_alphabet()
{
    পূর্ণাঙ্ক i, j;
    ধ্রুৱ পূর্ণাঙ্ক nsize = 8;
    image **alphabets = calloc(nsize, পরিমাণ(image));
    বাবে(j = 0; j < nsize; ++j){
        alphabets[j] = calloc(128, পরিমাণ(image));
        বাবে(i = 32; i < 127; ++i){
            ৱর্ণ buff[256];
            sprintf(buff, "data/labels/%d_%d.png", i, j);
            alphabets[j][i] = load_image_color(buff, 0, 0);
        }
    }
    ঘূৰাই alphabets;
}

শূন্য draw_detections(image im, detection *dets, পূর্ণাঙ্ক অঙ্ক, ভিন্ন thresh, ৱর্ণ **names, image **alphabet, পূর্ণাঙ্ক classes)
{
    পূর্ণাঙ্ক i,j;

    বাবে(i = 0; i < অঙ্ক; ++i){
        ৱর্ণ labelstr[4096] = {0};
        পূর্ণাঙ্ক class = -1;
        বাবে(j = 0; j < classes; ++j){
            যদি (dets[i].prob[j] > thresh){
                যদি (class < 0) {
                    strcat(labelstr, names[j]);
                    class = j;
                } অন্যথা {
                    strcat(labelstr, ", ");
                    strcat(labelstr, names[j]);
                }
                printf("%s: %.0f%%\n", names[j], dets[i].prob[j]*100);
            }
        }
        যদি(class >= 0){
            পূর্ণাঙ্ক width = im.h * .006;

            /*
               if(0){
               width = pow(prob, 1./2.)*10+1;
               alphabet = 0;
               }
             */

            //printf("%d %s: %.0f%%\n", i, names[class], prob*100);
            পূর্ণাঙ্ক offset = class*123457 % classes;
            ভিন্ন red = get_color(2,offset,classes);
            ভিন্ন green = get_color(1,offset,classes);
            ভিন্ন blue = get_color(0,offset,classes);
            ভিন্ন rgb[3];

            //width = prob*20+2;

            rgb[0] = red;
            rgb[1] = green;
            rgb[2] = blue;
            box b = dets[i].bbox;
            //printf("%f %f %f %f\n", b.x, b.y, b.w, b.h);

            পূর্ণাঙ্ক বাম  = (b.x-b.w/2.)*im.w;
            পূর্ণাঙ্ক ডান = (b.x+b.w/2.)*im.w;
            পূর্ণাঙ্ক শীৰ্ষ   = (b.y-b.h/2.)*im.h;
            পূর্ণাঙ্ক bot   = (b.y+b.h/2.)*im.h;

            যদি(বাম < 0) বাম = 0;
            যদি(ডান > im.w-1) ডান = im.w-1;
            যদি(শীৰ্ষ < 0) শীৰ্ষ = 0;
            যদি(bot > im.h-1) bot = im.h-1;

            draw_box_width(im, বাম, শীৰ্ষ, ডান, bot, width, red, green, blue);
            যদি (alphabet) {
                image label = get_label(alphabet, labelstr, (im.h*.03));
                draw_label(im, শীৰ্ষ + width, বাম, label, rgb);
                free_image(label);
            }
            যদি (dets[i].mask){
                image mask = float_to_image(14, 14, 1, dets[i].mask);
                image resized_mask = resize_image(mask, b.w*im.w, b.h*im.h);
                image tmask = threshold_image(resized_mask, .5);
                embed_image(tmask, im, বাম, শীৰ্ষ);
                free_image(mask);
                free_image(resized_mask);
                free_image(tmask);
            }
        }
    }
}

শূন্য transpose_image(image im)
{
    প্রতিজ্ঞা(im.w == im.h);
    পূর্ণাঙ্ক n, m;
    পূর্ণাঙ্ক c;
    বাবে(c = 0; c < im.c; ++c){
        বাবে(n = 0; n < im.w-1; ++n){
            বাবে(m = n + 1; m < im.w; ++m){
                ভিন্ন swap = im.data[m + im.w*(n + im.h*c)];
                im.data[m + im.w*(n + im.h*c)] = im.data[n + im.w*(m + im.h*c)];
                im.data[n + im.w*(m + im.h*c)] = swap;
            }
        }
    }
}

শূন্য rotate_image_cw(image im, পূর্ণাঙ্ক times)
{
    প্রতিজ্ঞা(im.w == im.h);
    times = (times + 400) % 4;
    পূর্ণাঙ্ক i, x, y, c;
    পূর্ণাঙ্ক n = im.w;
    বাবে(i = 0; i < times; ++i){
        বাবে(c = 0; c < im.c; ++c){
            বাবে(x = 0; x < n/2; ++x){
                বাবে(y = 0; y < (n-1)/2 + 1; ++y){
                    ভিন্ন temp = im.data[y + im.w*(x + im.h*c)];
                    im.data[y + im.w*(x + im.h*c)] = im.data[n-1-x + im.w*(y + im.h*c)];
                    im.data[n-1-x + im.w*(y + im.h*c)] = im.data[n-1-y + im.w*(n-1-x + im.h*c)];
                    im.data[n-1-y + im.w*(n-1-x + im.h*c)] = im.data[x + im.w*(n-1-y + im.h*c)];
                    im.data[x + im.w*(n-1-y + im.h*c)] = temp;
                }
            }
        }
    }
}

শূন্য flip_image(image a)
{
    পূর্ণাঙ্ক i,j,k;
    বাবে(k = 0; k < a.c; ++k){
        বাবে(i = 0; i < a.h; ++i){
            বাবে(j = 0; j < a.w/2; ++j){
                পূর্ণাঙ্ক index = j + a.w*(i + a.h*(k));
                পূর্ণাঙ্ক flip = (a.w - j - 1) + a.w*(i + a.h*(k));
                ভিন্ন swap = a.data[flip];
                a.data[flip] = a.data[index];
                a.data[index] = swap;
            }
        }
    }
}

image image_distance(image a, image b)
{
    পূর্ণাঙ্ক i,j;
    image dist = make_image(a.w, a.h, 1);
    বাবে(i = 0; i < a.c; ++i){
        বাবে(j = 0; j < a.h*a.w; ++j){
            dist.data[j] += pow(a.data[i*a.h*a.w+j]-b.data[i*a.h*a.w+j],2);
        }
    }
    বাবে(j = 0; j < a.h*a.w; ++j){
        dist.data[j] = sqrt(dist.data[j]);
    }
    ঘূৰাই dist;
}

শূন্য ghost_image(image source, image dest, পূর্ণাঙ্ক dx, পূর্ণাঙ্ক dy)
{
    পূর্ণাঙ্ক x,y,k;
    ভিন্ন max_dist = sqrt((-source.w/2. + .5)*(-source.w/2. + .5));
    বাবে(k = 0; k < source.c; ++k){
        বাবে(y = 0; y < source.h; ++y){
            বাবে(x = 0; x < source.w; ++x){
                ভিন্ন dist = sqrt((x - source.w/2. + .5)*(x - source.w/2. + .5) + (y - source.h/2. + .5)*(y - source.h/2. + .5));
                ভিন্ন alpha = (1 - dist/max_dist);
                যদি(alpha < 0) alpha = 0;
                ভিন্ন v1 = get_pixel(source, x,y,k);
                ভিন্ন v2 = get_pixel(dest, dx+x,dy+y,k);
                ভিন্ন val = alpha*v1 + (1-alpha)*v2;
                set_pixel(dest, dx+x, dy+y, k, val);
            }
        }
    }
}

শূন্য blocky_image(image im, পূর্ণাঙ্ক s)
{
    পূর্ণাঙ্ক i,j,k;
    বাবে(k = 0; k < im.c; ++k){
        বাবে(j = 0; j < im.h; ++j){
            বাবে(i = 0; i < im.w; ++i){
                im.data[i + im.w*(j + im.h*k)] = im.data[i/s*s + im.w*(j/s*s + im.h*k)];
            }
        }
    }
}

শূন্য censor_image(image im, পূর্ণাঙ্ক dx, পূর্ণাঙ্ক dy, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h)
{
    পূর্ণাঙ্ক i,j,k;
    পূর্ণাঙ্ক s = 32;
    যদি(dx < 0) dx = 0;
    যদি(dy < 0) dy = 0;

    বাবে(k = 0; k < im.c; ++k){
        বাবে(j = dy; j < dy + h && j < im.h; ++j){
            বাবে(i = dx; i < dx + w && i < im.w; ++i){
                im.data[i + im.w*(j + im.h*k)] = im.data[i/s*s + im.w*(j/s*s + im.h*k)];
                //im.data[i + j*im.w + k*im.w*im.h] = 0;
            }
        }
    }
}

শূন্য embed_image(image source, image dest, পূর্ণাঙ্ক dx, পূর্ণাঙ্ক dy)
{
    পূর্ণাঙ্ক x,y,k;
    বাবে(k = 0; k < source.c; ++k){
        বাবে(y = 0; y < source.h; ++y){
            বাবে(x = 0; x < source.w; ++x){
                ভিন্ন val = get_pixel(source, x,y,k);
                set_pixel(dest, dx+x, dy+y, k, val);
            }
        }
    }
}

image collapse_image_layers(image source, পূর্ণাঙ্ক border)
{
    পূর্ণাঙ্ক h = source.h;
    h = (h+border)*source.c - border;
    image dest = make_image(source.w, h, 1);
    পূর্ণাঙ্ক i;
    বাবে(i = 0; i < source.c; ++i){
        image layer = get_image_layer(source, i);
        পূর্ণাঙ্ক h_offset = i*(source.h+border);
        embed_image(layer, dest, 0, h_offset);
        free_image(layer);
    }
    ঘূৰাই dest;
}

শূন্য constrain_image(image im)
{
    পূর্ণাঙ্ক i;
    বাবে(i = 0; i < im.w*im.h*im.c; ++i){
        যদি(im.data[i] < 0) im.data[i] = 0;
        যদি(im.data[i] > 1) im.data[i] = 1;
    }
}

শূন্য normalize_image(image p)
{
    পূর্ণাঙ্ক i;
    ভিন্ন min = 9999999;
    ভিন্ন max = -999999;

    বাবে(i = 0; i < p.h*p.w*p.c; ++i){
        ভিন্ন v = p.data[i];
        যদি(v < min) min = v;
        যদি(v > max) max = v;
    }
    যদি(max - min < .000000001){
        min = 0;
        max = 1;
    }
    বাবে(i = 0; i < p.c*p.w*p.h; ++i){
        p.data[i] = (p.data[i] - min)/(max-min);
    }
}

শূন্য normalize_image2(image p)
{
    ভিন্ন *min = calloc(p.c, পরিমাণ(ভিন্ন));
    ভিন্ন *max = calloc(p.c, পরিমাণ(ভিন্ন));
    পূর্ণাঙ্ক i,j;
    বাবে(i = 0; i < p.c; ++i) min[i] = max[i] = p.data[i*p.h*p.w];

    বাবে(j = 0; j < p.c; ++j){
        বাবে(i = 0; i < p.h*p.w; ++i){
            ভিন্ন v = p.data[i+j*p.h*p.w];
            যদি(v < min[j]) min[j] = v;
            যদি(v > max[j]) max[j] = v;
        }
    }
    বাবে(i = 0; i < p.c; ++i){
        যদি(max[i] - min[i] < .000000001){
            min[i] = 0;
            max[i] = 1;
        }
    }
    বাবে(j = 0; j < p.c; ++j){
        বাবে(i = 0; i < p.w*p.h; ++i){
            p.data[i+j*p.h*p.w] = (p.data[i+j*p.h*p.w] - min[j])/(max[j]-min[j]);
        }
    }
    মুক্ত(min);
    মুক্ত(max);
}

শূন্য copy_image_into(image src, image dest)
{
    memcpy(dest.data, src.data, src.h*src.w*src.c*পরিমাণ(ভিন্ন));
}

image copy_image(image p)
{
    image copy = p;
    copy.data = calloc(p.h*p.w*p.c, পরিমাণ(ভিন্ন));
    memcpy(copy.data, p.data, p.h*p.w*p.c*পরিমাণ(ভিন্ন));
    ঘূৰাই copy;
}

শূন্য rgbgr_image(image im)
{
    পূর্ণাঙ্ক i;
    বাবে(i = 0; i < im.w*im.h; ++i){
        ভিন্ন swap = im.data[i];
        im.data[i] = im.data[i+im.w*im.h*2];
        im.data[i+im.w*im.h*2] = swap;
    }
}

পূর্ণাঙ্ক show_image(image p, ধ্রুৱ ৱর্ণ *name, পূর্ণাঙ্ক ms)
{
#ifdef OPENCV
    পূর্ণাঙ্ক c = show_image_cv(p, name, ms);
    ঘূৰাই c;
#অন্যথা
    fprintf(stderr, "Not compiled with OpenCV, saving to %s.png instead\n", name);
    save_image(p, name);
    ঘূৰাই -1;
#endif
}

শূন্য save_image_options(image im, ধ্রুৱ ৱর্ণ *name, IMTYPE f, পূর্ণাঙ্ক quality)
{
    ৱর্ণ buff[256];
    //sprintf(buff, "%s (%d)", name, windows);
    যদি(f == PNG)       sprintf(buff, "%s.png", name);
    অন্যথা যদি (f == BMP) sprintf(buff, "%s.bmp", name);
    অন্যথা যদি (f == TGA) sprintf(buff, "%s.tga", name);
    অন্যথা যদি (f == JPG) sprintf(buff, "%s.jpg", name);
    অন্যথা               sprintf(buff, "%s.png", name);
    unsigned ৱর্ণ *data = calloc(im.w*im.h*im.c, পরিমাণ(ৱর্ণ));
    পূর্ণাঙ্ক i,k;
    বাবে(k = 0; k < im.c; ++k){
        বাবে(i = 0; i < im.w*im.h; ++i){
            data[i*im.c+k] = (unsigned ৱর্ণ) (255*im.data[i + k*im.w*im.h]);
        }
    }
    পূর্ণাঙ্ক success = 0;
    যদি(f == PNG)       success = stbi_write_png(buff, im.w, im.h, im.c, data, im.w*im.c);
    অন্যথা যদি (f == BMP) success = stbi_write_bmp(buff, im.w, im.h, im.c, data);
    অন্যথা যদি (f == TGA) success = stbi_write_tga(buff, im.w, im.h, im.c, data);
    অন্যথা যদি (f == JPG) success = stbi_write_jpg(buff, im.w, im.h, im.c, data, quality);
    মুক্ত(data);
    যদি(!success) fprintf(stderr, "Failed to write image %s\n", buff);
}

শূন্য save_image(image im, ধ্রুৱ ৱর্ণ *name)
{
    save_image_options(im, name, JPG, 80);
}

শূন্য show_image_layers(image p, ৱর্ণ *name)
{
    পূর্ণাঙ্ক i;
    ৱর্ণ buff[256];
    বাবে(i = 0; i < p.c; ++i){
        sprintf(buff, "%s - Layer %d", name, i);
        image layer = get_image_layer(p, i);
        show_image(layer, buff, 1);
        free_image(layer);
    }
}

শূন্য show_image_collapsed(image p, ৱর্ণ *name)
{
    image c = collapse_image_layers(p, 1);
    show_image(c, name, 1);
    free_image(c);
}

image make_empty_image(পূর্ণাঙ্ক w, পূর্ণাঙ্ক h, পূর্ণাঙ্ক c)
{
    image out;
    out.data = 0;
    out.h = h;
    out.w = w;
    out.c = c;
    ঘূৰাই out;
}

image make_image(পূর্ণাঙ্ক w, পূর্ণাঙ্ক h, পূর্ণাঙ্ক c)
{
    image out = make_empty_image(w,h,c);
    out.data = calloc(h*w*c, পরিমাণ(ভিন্ন));
    ঘূৰাই out;
}

image make_random_image(পূর্ণাঙ্ক w, পূর্ণাঙ্ক h, পূর্ণাঙ্ক c)
{
    image out = make_empty_image(w,h,c);
    out.data = calloc(h*w*c, পরিমাণ(ভিন্ন));
    পূর্ণাঙ্ক i;
    বাবে(i = 0; i < w*h*c; ++i){
        out.data[i] = (rand_normal() * .25) + .5;
    }
    ঘূৰাই out;
}

image float_to_image(পূর্ণাঙ্ক w, পূর্ণাঙ্ক h, পূর্ণাঙ্ক c, ভিন্ন *data)
{
    image out = make_empty_image(w,h,c);
    out.data = data;
    ঘূৰাই out;
}

শূন্য place_image(image im, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h, পূর্ণাঙ্ক dx, পূর্ণাঙ্ক dy, image canvas)
{
    পূর্ণাঙ্ক x, y, c;
    বাবে(c = 0; c < im.c; ++c){
        বাবে(y = 0; y < h; ++y){
            বাবে(x = 0; x < w; ++x){
                ভিন্ন rx = ((ভিন্ন)x / w) * im.w;
                ভিন্ন ry = ((ভিন্ন)y / h) * im.h;
                ভিন্ন val = bilinear_interpolate(im, rx, ry, c);
                set_pixel(canvas, x + dx, y + dy, c, val);
            }
        }
    }
}

image center_crop_image(image im, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h)
{
    পূর্ণাঙ্ক m = (im.w < im.h) ? im.w : im.h;   
    image c = crop_image(im, (im.w - m) / 2, (im.h - m)/2, m, m);
    image r = resize_image(c, w, h);
    free_image(c);
    ঘূৰাই r;
}

image rotate_crop_image(image im, ভিন্ন rad, ভিন্ন s, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h, ভিন্ন dx, ভিন্ন dy, ভিন্ন aspect)
{
    পূর্ণাঙ্ক x, y, c;
    ভিন্ন cx = im.w/2.;
    ভিন্ন cy = im.h/2.;
    image rot = make_image(w, h, im.c);
    বাবে(c = 0; c < im.c; ++c){
        বাবে(y = 0; y < h; ++y){
            বাবে(x = 0; x < w; ++x){
                ভিন্ন rx = cos(rad)*((x - w/2.)/s*aspect + dx/s*aspect) - sin(rad)*((y - h/2.)/s + dy/s) + cx;
                ভিন্ন ry = sin(rad)*((x - w/2.)/s*aspect + dx/s*aspect) + cos(rad)*((y - h/2.)/s + dy/s) + cy;
                ভিন্ন val = bilinear_interpolate(im, rx, ry, c);
                set_pixel(rot, x, y, c, val);
            }
        }
    }
    ঘূৰাই rot;
}

image rotate_image(image im, ভিন্ন rad)
{
    পূর্ণাঙ্ক x, y, c;
    ভিন্ন cx = im.w/2.;
    ভিন্ন cy = im.h/2.;
    image rot = make_image(im.w, im.h, im.c);
    বাবে(c = 0; c < im.c; ++c){
        বাবে(y = 0; y < im.h; ++y){
            বাবে(x = 0; x < im.w; ++x){
                ভিন্ন rx = cos(rad)*(x-cx) - sin(rad)*(y-cy) + cx;
                ভিন্ন ry = sin(rad)*(x-cx) + cos(rad)*(y-cy) + cy;
                ভিন্ন val = bilinear_interpolate(im, rx, ry, c);
                set_pixel(rot, x, y, c, val);
            }
        }
    }
    ঘূৰাই rot;
}

শূন্য fill_image(image m, ভিন্ন s)
{
    পূর্ণাঙ্ক i;
    বাবে(i = 0; i < m.h*m.w*m.c; ++i) m.data[i] = s;
}

শূন্য translate_image(image m, ভিন্ন s)
{
    পূর্ণাঙ্ক i;
    বাবে(i = 0; i < m.h*m.w*m.c; ++i) m.data[i] += s;
}

শূন্য scale_image(image m, ভিন্ন s)
{
    পূর্ণাঙ্ক i;
    বাবে(i = 0; i < m.h*m.w*m.c; ++i) m.data[i] *= s;
}

image crop_image(image im, পূর্ণাঙ্ক dx, পূর্ণাঙ্ক dy, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h)
{
    image cropped = make_image(w, h, im.c);
    পূর্ণাঙ্ক i, j, k;
    বাবে(k = 0; k < im.c; ++k){
        বাবে(j = 0; j < h; ++j){
            বাবে(i = 0; i < w; ++i){
                পূর্ণাঙ্ক r = j + dy;
                পূর্ণাঙ্ক c = i + dx;
                ভিন্ন val = 0;
                r = constrain_int(r, 0, im.h-1);
                c = constrain_int(c, 0, im.w-1);
                val = get_pixel(im, c, r, k);
                set_pixel(cropped, i, j, k, val);
            }
        }
    }
    ঘূৰাই cropped;
}

পূর্ণাঙ্ক best_3d_shift_r(image a, image b, পূর্ণাঙ্ক min, পূর্ণাঙ্ক max)
{
    যদি(min == max) ঘূৰাই min;
    পূর্ণাঙ্ক mid = floor((min + max) / 2.);
    image c1 = crop_image(b, 0, mid, b.w, b.h);
    image c2 = crop_image(b, 0, mid+1, b.w, b.h);
    ভিন্ন d1 = dist_array(c1.data, a.data, a.w*a.h*a.c, 10);
    ভিন্ন d2 = dist_array(c2.data, a.data, a.w*a.h*a.c, 10);
    free_image(c1);
    free_image(c2);
    যদি(d1 < d2) ঘূৰাই best_3d_shift_r(a, b, min, mid);
    অন্যথা ঘূৰাই best_3d_shift_r(a, b, mid+1, max);
}

পূর্ণাঙ্ক best_3d_shift(image a, image b, পূর্ণাঙ্ক min, পূর্ণাঙ্ক max)
{
    পূর্ণাঙ্ক i;
    পূর্ণাঙ্ক best = 0;
    ভিন্ন best_distance = FLT_MAX;
    বাবে(i = min; i <= max; i += 2){
        image c = crop_image(b, 0, i, b.w, b.h);
        ভিন্ন d = dist_array(c.data, a.data, a.w*a.h*a.c, 100);
        যদি(d < best_distance){
            best_distance = d;
            best = i;
        }
        printf("%d %f\n", i, d);
        free_image(c);
    }
    ঘূৰাই best;
}

শূন্য composite_3d(ৱর্ণ *f1, ৱর্ণ *f2, ৱর্ণ *out, পূর্ণাঙ্ক delta)
{
    যদি(!out) out = "out";
    image a = load_image(f1, 0,0,0);
    image b = load_image(f2, 0,0,0);
    পূর্ণাঙ্ক shift = best_3d_shift_r(a, b, -a.h/100, a.h/100);

    image c1 = crop_image(b, 10, shift, b.w, b.h);
    ভিন্ন d1 = dist_array(c1.data, a.data, a.w*a.h*a.c, 100);
    image c2 = crop_image(b, -10, shift, b.w, b.h);
    ভিন্ন d2 = dist_array(c2.data, a.data, a.w*a.h*a.c, 100);

    যদি(d2 < d1 && 0){
        image swap = a;
        a = b;
        b = swap;
        shift = -shift;
        printf("swapped, %d\n", shift);
    }
    অন্যথা{
        printf("%d\n", shift);
    }

    image c = crop_image(b, delta, shift, a.w, a.h);
    পূর্ণাঙ্ক i;
    বাবে(i = 0; i < c.w*c.h; ++i){
        c.data[i] = a.data[i];
    }
    save_image(c, out);
}

শূন্য letterbox_image_into(image im, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h, image boxed)
{
    পূর্ণাঙ্ক new_w = im.w;
    পূর্ণাঙ্ক new_h = im.h;
    যদি (((ভিন্ন)w/im.w) < ((ভিন্ন)h/im.h)) {
        new_w = w;
        new_h = (im.h * w)/im.w;
    } অন্যথা {
        new_h = h;
        new_w = (im.w * h)/im.h;
    }
    image resized = resize_image(im, new_w, new_h);
    embed_image(resized, boxed, (w-new_w)/2, (h-new_h)/2); 
    free_image(resized);
}

image letterbox_image(image im, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h)
{
    পূর্ণাঙ্ক new_w = im.w;
    পূর্ণাঙ্ক new_h = im.h;
    যদি (((ভিন্ন)w/im.w) < ((ভিন্ন)h/im.h)) {
        new_w = w;
        new_h = (im.h * w)/im.w;
    } অন্যথা {
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
    ঘূৰাই boxed;
}

image resize_max(image im, পূর্ণাঙ্ক max)
{
    পূর্ণাঙ্ক w = im.w;
    পূর্ণাঙ্ক h = im.h;
    যদি(w > h){
        h = (h * max) / w;
        w = max;
    } অন্যথা {
        w = (w * max) / h;
        h = max;
    }
    যদি(w == im.w && h == im.h) ঘূৰাই im;
    image resized = resize_image(im, w, h);
    ঘূৰাই resized;
}

image resize_min(image im, পূর্ণাঙ্ক min)
{
    পূর্ণাঙ্ক w = im.w;
    পূর্ণাঙ্ক h = im.h;
    যদি(w < h){
        h = (h * min) / w;
        w = min;
    } অন্যথা {
        w = (w * min) / h;
        h = min;
    }
    যদি(w == im.w && h == im.h) ঘূৰাই im;
    image resized = resize_image(im, w, h);
    ঘূৰাই resized;
}

image random_crop_image(image im, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h)
{
    পূর্ণাঙ্ক dx = rand_int(0, im.w - w);
    পূর্ণাঙ্ক dy = rand_int(0, im.h - h);
    image crop = crop_image(im, dx, dy, w, h);
    ঘূৰাই crop;
}

augment_args random_augment_args(image im, ভিন্ন angle, ভিন্ন aspect, পূর্ণাঙ্ক নিচেৰ, পূর্ণাঙ্ক উপৰেৰ, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h)
{
    augment_args a = {0};
    aspect = rand_scale(aspect);
    পূর্ণাঙ্ক r = rand_int(নিচেৰ, উপৰেৰ);
    পূর্ণাঙ্ক min = (im.h < im.w*aspect) ? im.h : im.w*aspect;
    ভিন্ন scale = (ভিন্ন)r / min;

    ভিন্ন rad = rand_uniform(-angle, angle) * TWO_PI / 360.;

    ভিন্ন dx = (im.w*scale/aspect - w) / 2.;
    ভিন্ন dy = (im.h*scale - w) / 2.;
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
    ঘূৰাই a;
}

image random_augment_image(image im, ভিন্ন angle, ভিন্ন aspect, পূর্ণাঙ্ক নিচেৰ, পূর্ণাঙ্ক উপৰেৰ, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h)
{
    augment_args a = random_augment_args(im, angle, aspect, নিচেৰ, উপৰেৰ, w, h);
    image crop = rotate_crop_image(im, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);
    ঘূৰাই crop;
}

ভিন্ন three_way_max(ভিন্ন a, ভিন্ন b, ভিন্ন c)
{
    ঘূৰাই (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

ভিন্ন three_way_min(ভিন্ন a, ভিন্ন b, ভিন্ন c)
{
    ঘূৰাই (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

শূন্য yuv_to_rgb(image im)
{
    প্রতিজ্ঞা(im.c == 3);
    পূর্ণাঙ্ক i, j;
    ভিন্ন r, g, b;
    ভিন্ন y, u, v;
    বাবে(j = 0; j < im.h; ++j){
        বাবে(i = 0; i < im.w; ++i){
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

শূন্য rgb_to_yuv(image im)
{
    প্রতিজ্ঞা(im.c == 3);
    পূর্ণাঙ্ক i, j;
    ভিন্ন r, g, b;
    ভিন্ন y, u, v;
    বাবে(j = 0; j < im.h; ++j){
        বাবে(i = 0; i < im.w; ++i){
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
শূন্য rgb_to_hsv(image im)
{
    প্রতিজ্ঞা(im.c == 3);
    পূর্ণাঙ্ক i, j;
    ভিন্ন r, g, b;
    ভিন্ন h, s, v;
    বাবে(j = 0; j < im.h; ++j){
        বাবে(i = 0; i < im.w; ++i){
            r = get_pixel(im, i , j, 0);
            g = get_pixel(im, i , j, 1);
            b = get_pixel(im, i , j, 2);
            ভিন্ন max = three_way_max(r,g,b);
            ভিন্ন min = three_way_min(r,g,b);
            ভিন্ন delta = max - min;
            v = max;
            যদি(max == 0){
                s = 0;
                h = 0;
            }অন্যথা{
                s = delta/max;
                যদি(r == max){
                    h = (g - b) / delta;
                } অন্যথা যদি (g == max) {
                    h = 2 + (b - r) / delta;
                } অন্যথা {
                    h = 4 + (r - g) / delta;
                }
                যদি (h < 0) h += 6;
                h = h/6.;
            }
            set_pixel(im, i, j, 0, h);
            set_pixel(im, i, j, 1, s);
            set_pixel(im, i, j, 2, v);
        }
    }
}

শূন্য hsv_to_rgb(image im)
{
    প্রতিজ্ঞা(im.c == 3);
    পূর্ণাঙ্ক i, j;
    ভিন্ন r, g, b;
    ভিন্ন h, s, v;
    ভিন্ন f, p, q, t;
    বাবে(j = 0; j < im.h; ++j){
        বাবে(i = 0; i < im.w; ++i){
            h = 6 * get_pixel(im, i , j, 0);
            s = get_pixel(im, i , j, 1);
            v = get_pixel(im, i , j, 2);
            যদি (s == 0) {
                r = g = b = v;
            } অন্যথা {
                পূর্ণাঙ্ক index = floor(h);
                f = h - index;
                p = v*(1-s);
                q = v*(1-s*f);
                t = v*(1-s*(1-f));
                যদি(index == 0){
                    r = v; g = t; b = p;
                } অন্যথা যদি(index == 1){
                    r = q; g = v; b = p;
                } অন্যথা যদি(index == 2){
                    r = p; g = v; b = t;
                } অন্যথা যদি(index == 3){
                    r = p; g = q; b = v;
                } অন্যথা যদি(index == 4){
                    r = t; g = p; b = v;
                } অন্যথা {
                    r = v; g = p; b = q;
                }
            }
            set_pixel(im, i, j, 0, r);
            set_pixel(im, i, j, 1, g);
            set_pixel(im, i, j, 2, b);
        }
    }
}

শূন্য grayscale_image_3c(image im)
{
    প্রতিজ্ঞা(im.c == 3);
    পূর্ণাঙ্ক i, j, k;
    ভিন্ন scale[] = {0.299, 0.587, 0.114};
    বাবে(j = 0; j < im.h; ++j){
        বাবে(i = 0; i < im.w; ++i){
            ভিন্ন val = 0;
            বাবে(k = 0; k < 3; ++k){
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
    প্রতিজ্ঞা(im.c == 3);
    পূর্ণাঙ্ক i, j, k;
    image gray = make_image(im.w, im.h, 1);
    ভিন্ন scale[] = {0.299, 0.587, 0.114};
    বাবে(k = 0; k < im.c; ++k){
        বাবে(j = 0; j < im.h; ++j){
            বাবে(i = 0; i < im.w; ++i){
                gray.data[i+im.w*j] += scale[k]*get_pixel(im, i, j, k);
            }
        }
    }
    ঘূৰাই gray;
}

image threshold_image(image im, ভিন্ন thresh)
{
    পূর্ণাঙ্ক i;
    image t = make_image(im.w, im.h, im.c);
    বাবে(i = 0; i < im.w*im.h*im.c; ++i){
        t.data[i] = im.data[i]>thresh ? 1 : 0;
    }
    ঘূৰাই t;
}

image blend_image(image fore, image back, ভিন্ন alpha)
{
    প্রতিজ্ঞা(fore.w == back.w && fore.h == back.h && fore.c == back.c);
    image blend = make_image(fore.w, fore.h, fore.c);
    পূর্ণাঙ্ক i, j, k;
    বাবে(k = 0; k < fore.c; ++k){
        বাবে(j = 0; j < fore.h; ++j){
            বাবে(i = 0; i < fore.w; ++i){
                ভিন্ন val = alpha * get_pixel(fore, i, j, k) + 
                    (1 - alpha)* get_pixel(back, i, j, k);
                set_pixel(blend, i, j, k, val);
            }
        }
    }
    ঘূৰাই blend;
}

শূন্য scale_image_channel(image im, পূর্ণাঙ্ক c, ভিন্ন v)
{
    পূর্ণাঙ্ক i, j;
    বাবে(j = 0; j < im.h; ++j){
        বাবে(i = 0; i < im.w; ++i){
            ভিন্ন pix = get_pixel(im, i, j, c);
            pix = pix*v;
            set_pixel(im, i, j, c, pix);
        }
    }
}

শূন্য translate_image_channel(image im, পূর্ণাঙ্ক c, ভিন্ন v)
{
    পূর্ণাঙ্ক i, j;
    বাবে(j = 0; j < im.h; ++j){
        বাবে(i = 0; i < im.w; ++i){
            ভিন্ন pix = get_pixel(im, i, j, c);
            pix = pix+v;
            set_pixel(im, i, j, c, pix);
        }
    }
}

image binarize_image(image im)
{
    image c = copy_image(im);
    পূর্ণাঙ্ক i;
    বাবে(i = 0; i < im.w * im.h * im.c; ++i){
        যদি(c.data[i] > .5) c.data[i] = 1;
        অন্যথা c.data[i] = 0;
    }
    ঘূৰাই c;
}

শূন্য saturate_image(image im, ভিন্ন sat)
{
    rgb_to_hsv(im);
    scale_image_channel(im, 1, sat);
    hsv_to_rgb(im);
    constrain_image(im);
}

শূন্য hue_image(image im, ভিন্ন hue)
{
    rgb_to_hsv(im);
    পূর্ণাঙ্ক i;
    বাবে(i = 0; i < im.w*im.h; ++i){
        im.data[i] = im.data[i] + hue;
        যদি (im.data[i] > 1) im.data[i] -= 1;
        যদি (im.data[i] < 0) im.data[i] += 1;
    }
    hsv_to_rgb(im);
    constrain_image(im);
}

শূন্য exposure_image(image im, ভিন্ন sat)
{
    rgb_to_hsv(im);
    scale_image_channel(im, 2, sat);
    hsv_to_rgb(im);
    constrain_image(im);
}

শূন্য distort_image(image im, ভিন্ন hue, ভিন্ন sat, ভিন্ন val)
{
    rgb_to_hsv(im);
    scale_image_channel(im, 1, sat);
    scale_image_channel(im, 2, val);
    পূর্ণাঙ্ক i;
    বাবে(i = 0; i < im.w*im.h; ++i){
        im.data[i] = im.data[i] + hue;
        যদি (im.data[i] > 1) im.data[i] -= 1;
        যদি (im.data[i] < 0) im.data[i] += 1;
    }
    hsv_to_rgb(im);
    constrain_image(im);
}

শূন্য random_distort_image(image im, ভিন্ন hue, ভিন্ন saturation, ভিন্ন exposure)
{
    ভিন্ন dhue = rand_uniform(-hue, hue);
    ভিন্ন dsat = rand_scale(saturation);
    ভিন্ন dexp = rand_scale(exposure);
    distort_image(im, dhue, dsat, dexp);
}

শূন্য saturate_exposure_image(image im, ভিন্ন sat, ভিন্ন exposure)
{
    rgb_to_hsv(im);
    scale_image_channel(im, 1, sat);
    scale_image_channel(im, 2, exposure);
    hsv_to_rgb(im);
    constrain_image(im);
}

image resize_image(image im, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h)
{
    image resized = make_image(w, h, im.c);   
    image part = make_image(w, im.h, im.c);
    পূর্ণাঙ্ক r, c, k;
    ভিন্ন w_scale = (ভিন্ন)(im.w - 1) / (w - 1);
    ভিন্ন h_scale = (ভিন্ন)(im.h - 1) / (h - 1);
    বাবে(k = 0; k < im.c; ++k){
        বাবে(r = 0; r < im.h; ++r){
            বাবে(c = 0; c < w; ++c){
                ভিন্ন val = 0;
                যদি(c == w-1 || im.w == 1){
                    val = get_pixel(im, im.w-1, r, k);
                } অন্যথা {
                    ভিন্ন sx = c*w_scale;
                    পূর্ণাঙ্ক ix = (পূর্ণাঙ্ক) sx;
                    ভিন্ন dx = sx - ix;
                    val = (1 - dx) * get_pixel(im, ix, r, k) + dx * get_pixel(im, ix+1, r, k);
                }
                set_pixel(part, c, r, k, val);
            }
        }
    }
    বাবে(k = 0; k < im.c; ++k){
        বাবে(r = 0; r < h; ++r){
            ভিন্ন sy = r*h_scale;
            পূর্ণাঙ্ক iy = (পূর্ণাঙ্ক) sy;
            ভিন্ন dy = sy - iy;
            বাবে(c = 0; c < w; ++c){
                ভিন্ন val = (1-dy) * get_pixel(part, c, iy, k);
                set_pixel(resized, c, r, k, val);
            }
            যদি(r == h-1 || im.h == 1) অনুৱর্ত;
            বাবে(c = 0; c < w; ++c){
                ভিন্ন val = dy * get_pixel(part, c, iy+1, k);
                add_pixel(resized, c, r, k, val);
            }
        }
    }

    free_image(part);
    ঘূৰাই resized;
}


শূন্য test_resize(ৱর্ণ *filename)
{
    image im = load_image(filename, 0,0, 3);
    ভিন্ন mag = mag_array(im.data, im.w*im.h*im.c);
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
    যেতিয়ালৈ(1){
        image aug = random_augment_image(im, 0, .75, 320, 448, 320, 320);
        show_image(aug, "aug", 1);
        free_image(aug);


        ভিন্ন exposure = 1.15;
        ভিন্ন saturation = 1.15;
        ভিন্ন hue = .05;

        image c = copy_image(im);

        ভিন্ন dexp = rand_scale(exposure);
        ভিন্ন dsat = rand_scale(saturation);
        ভিন্ন dhue = rand_uniform(-hue, hue);

        distort_image(c, dhue, dsat, dexp);
        show_image(c, "rand", 1);
        printf("%f %f %f\n", dhue, dsat, dexp);
        free_image(c);
    }
#endif
}


image load_image_stb(ৱর্ণ *filename, পূর্ণাঙ্ক channels)
{
    পূর্ণাঙ্ক w, h, c;
    unsigned ৱর্ণ *data = stbi_load(filename, &w, &h, &c, channels);
    যদি (!data) {
        fprintf(stderr, "Cannot load image \"%s\"\nSTB Reason: %s\n", filename, stbi_failure_reason());
        exit(0);
    }
    যদি(channels) c = channels;
    পূর্ণাঙ্ক i,j,k;
    image im = make_image(w, h, c);
    বাবে(k = 0; k < c; ++k){
        বাবে(j = 0; j < h; ++j){
            বাবে(i = 0; i < w; ++i){
                পূর্ণাঙ্ক dst_index = i + w*j + w*h*k;
                পূর্ণাঙ্ক src_index = k + c*i + c*w*j;
                im.data[dst_index] = (ভিন্ন)data[src_index]/255.;
            }
        }
    }
    মুক্ত(data);
    ঘূৰাই im;
}

image load_image(ৱর্ণ *filename, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h, পূর্ণাঙ্ক c)
{
#ifdef OPENCV
    image out = load_image_cv(filename, c);
#অন্যথা
    image out = load_image_stb(filename, c);
#endif

    যদি((h && w) && (h != out.h || w != out.w)){
        image resized = resize_image(out, w, h);
        free_image(out);
        out = resized;
    }
    ঘূৰাই out;
}

image load_image_color(ৱর্ণ *filename, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h)
{
    ঘূৰাই load_image(filename, w, h, 3);
}

image get_image_layer(image m, পূর্ণাঙ্ক l)
{
    image out = make_image(m.w, m.h, 1);
    পূর্ণাঙ্ক i;
    বাবে(i = 0; i < m.h*m.w; ++i){
        out.data[i] = m.data[i+l*m.h*m.w];
    }
    ঘূৰাই out;
}
শূন্য print_image(image m)
{
    পূর্ণাঙ্ক i, j, k;
    বাবে(i =0 ; i < m.c; ++i){
        বাবে(j =0 ; j < m.h; ++j){
            বাবে(k = 0; k < m.w; ++k){
                printf("%.2lf, ", m.data[i*m.h*m.w + j*m.w + k]);
                যদি(k > 30) ভঙ্গ;
            }
            printf("\n");
            যদি(j > 30) ভঙ্গ;
        }
        printf("\n");
    }
    printf("\n");
}

image collapse_images_vert(image *ims, পূর্ণাঙ্ক n)
{
    পূর্ণাঙ্ক color = 1;
    পূর্ণাঙ্ক border = 1;
    পূর্ণাঙ্ক h,w,c;
    w = ims[0].w;
    h = (ims[0].h + border) * n - border;
    c = ims[0].c;
    যদি(c != 3 || !color){
        w = (w+border)*c - border;
        c = 1;
    }

    image filters = make_image(w, h, c);
    পূর্ণাঙ্ক i,j;
    বাবে(i = 0; i < n; ++i){
        পূর্ণাঙ্ক h_offset = i*(ims[0].h+border);
        image copy = copy_image(ims[i]);
        //normalize_image(copy);
        যদি(c == 3 && color){
            embed_image(copy, filters, 0, h_offset);
        }
        অন্যথা{
            বাবে(j = 0; j < copy.c; ++j){
                পূর্ণাঙ্ক w_offset = j*(ims[0].w+border);
                image layer = get_image_layer(copy, j);
                embed_image(layer, filters, w_offset, h_offset);
                free_image(layer);
            }
        }
        free_image(copy);
    }
    ঘূৰাই filters;
} 

image collapse_images_horz(image *ims, পূর্ণাঙ্ক n)
{
    পূর্ণাঙ্ক color = 1;
    পূর্ণাঙ্ক border = 1;
    পূর্ণাঙ্ক h,w,c;
    পূর্ণাঙ্ক size = ims[0].h;
    h = size;
    w = (ims[0].w + border) * n - border;
    c = ims[0].c;
    যদি(c != 3 || !color){
        h = (h+border)*c - border;
        c = 1;
    }

    image filters = make_image(w, h, c);
    পূর্ণাঙ্ক i,j;
    বাবে(i = 0; i < n; ++i){
        পূর্ণাঙ্ক w_offset = i*(size+border);
        image copy = copy_image(ims[i]);
        //normalize_image(copy);
        যদি(c == 3 && color){
            embed_image(copy, filters, w_offset, 0);
        }
        অন্যথা{
            বাবে(j = 0; j < copy.c; ++j){
                পূর্ণাঙ্ক h_offset = j*(size+border);
                image layer = get_image_layer(copy, j);
                embed_image(layer, filters, w_offset, h_offset);
                free_image(layer);
            }
        }
        free_image(copy);
    }
    ঘূৰাই filters;
} 

শূন্য show_image_normalized(image im, ধ্রুৱ ৱর্ণ *name)
{
    image c = copy_image(im);
    normalize_image(c);
    show_image(c, name, 1);
    free_image(c);
}

শূন্য show_images(image *ims, পূর্ণাঙ্ক n, ৱর্ণ *window)
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

শূন্য free_image(image m)
{
    যদি(m.data){
        মুক্ত(m.data);
    }
}
