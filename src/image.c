#অন্তর্ভাব "image.h"
#অন্তর্ভাব "utils.h"
#অন্তর্ভাব "blas.h"
#অন্তর্ভাব "cuda.h"
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <math.h>

#define STB_IMAGE_IMPLEMENTATION
#অন্তর্ভাব "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#অন্তর্ভাব "stb_image_write.h"

পূর্ণসংখ্যা windows = 0;

দশমিক colors[6][3] = { {1,0,1}, {0,0,1},{0,1,1},{0,1,0},{1,1,0},{1,0,0} };

দশমিক get_color(পূর্ণসংখ্যা c, পূর্ণসংখ্যা x, পূর্ণসংখ্যা max)
{
    দশমিক ratio = ((দশমিক)x/max)*5;
    পূর্ণসংখ্যা i = floor(ratio);
    পূর্ণসংখ্যা j = ceil(ratio);
    ratio -= i;
    দশমিক r = (1-ratio) * colors[i][c] + ratio*colors[j][c];
    //printf("%f\n", r);
    ফেরত r;
}

image mask_to_rgb(image mask)
{
    পূর্ণসংখ্যা n = mask.c;
    image im = make_image(mask.w, mask.h, 3);
    পূর্ণসংখ্যা i, j;
    জন্য(j = 0; j < n; ++j){
        পূর্ণসংখ্যা offset = j*123457 % n;
        দশমিক red = get_color(2,offset,n);
        দশমিক green = get_color(1,offset,n);
        দশমিক blue = get_color(0,offset,n);
        জন্য(i = 0; i < im.w*im.h; ++i){
            im.তথ্য[i + 0*im.w*im.h] += mask.তথ্য[j*im.h*im.w + i]*red;
            im.তথ্য[i + 1*im.w*im.h] += mask.তথ্য[j*im.h*im.w + i]*green;
            im.তথ্য[i + 2*im.w*im.h] += mask.তথ্য[j*im.h*im.w + i]*blue;
        }
    }
    ফেরত im;
}

স্থির দশমিক get_pixel(image m, পূর্ণসংখ্যা x, পূর্ণসংখ্যা y, পূর্ণসংখ্যা c)
{
    প্রতিজ্ঞা(x < m.w && y < m.h && c < m.c);
    ফেরত m.তথ্য[c*m.h*m.w + y*m.w + x];
}
স্থির দশমিক get_pixel_extend(image m, পূর্ণসংখ্যা x, পূর্ণসংখ্যা y, পূর্ণসংখ্যা c)
{
    যদি(x < 0 || x >= m.w || y < 0 || y >= m.h) ফেরত 0;
    /*
    if(x < 0) x = 0;
    if(x >= m.w) x = m.w-1;
    if(y < 0) y = 0;
    if(y >= m.h) y = m.h-1;
    */
    যদি(c < 0 || c >= m.c) ফেরত 0;
    ফেরত get_pixel(m, x, y, c);
}
স্থির শূন্য set_pixel(image m, পূর্ণসংখ্যা x, পূর্ণসংখ্যা y, পূর্ণসংখ্যা c, দশমিক val)
{
    যদি (x < 0 || y < 0 || c < 0 || x >= m.w || y >= m.h || c >= m.c) ফেরত;
    প্রতিজ্ঞা(x < m.w && y < m.h && c < m.c);
    m.তথ্য[c*m.h*m.w + y*m.w + x] = val;
}
স্থির শূন্য add_pixel(image m, পূর্ণসংখ্যা x, পূর্ণসংখ্যা y, পূর্ণসংখ্যা c, দশমিক val)
{
    প্রতিজ্ঞা(x < m.w && y < m.h && c < m.c);
    m.তথ্য[c*m.h*m.w + y*m.w + x] += val;
}

স্থির দশমিক bilinear_interpolate(image im, দশমিক x, দশমিক y, পূর্ণসংখ্যা c)
{
    পূর্ণসংখ্যা ix = (পূর্ণসংখ্যা) floorf(x);
    পূর্ণসংখ্যা iy = (পূর্ণসংখ্যা) floorf(y);

    দশমিক dx = x - ix;
    দশমিক dy = y - iy;

    দশমিক val = (1-dy) * (1-dx) * get_pixel_extend(im, ix, iy, c) + 
        dy     * (1-dx) * get_pixel_extend(im, ix, iy+1, c) + 
        (1-dy) *   dx   * get_pixel_extend(im, ix+1, iy, c) +
        dy     *   dx   * get_pixel_extend(im, ix+1, iy+1, c);
    ফেরত val;
}


শূন্য composite_image(image source, image dest, পূর্ণসংখ্যা dx, পূর্ণসংখ্যা dy)
{
    পূর্ণসংখ্যা x,y,k;
    জন্য(k = 0; k < source.c; ++k){
        জন্য(y = 0; y < source.h; ++y){
            জন্য(x = 0; x < source.w; ++x){
                দশমিক val = get_pixel(source, x, y, k);
                দশমিক val2 = get_pixel_extend(dest, dx+x, dy+y, k);
                set_pixel(dest, dx+x, dy+y, k, val * val2);
            }
        }
    }
}

image border_image(image a, পূর্ণসংখ্যা border)
{
    image b = make_image(a.w + 2*border, a.h + 2*border, a.c);
    পূর্ণসংখ্যা x,y,k;
    জন্য(k = 0; k < b.c; ++k){
        জন্য(y = 0; y < b.h; ++y){
            জন্য(x = 0; x < b.w; ++x){
                দশমিক val = get_pixel_extend(a, x - border, y - border, k);
                যদি(x - border < 0 || x - border >= a.w || y - border < 0 || y - border >= a.h) val = 1;
                set_pixel(b, x, y, k, val);
            }
        }
    }
    ফেরত b;
}

image tile_images(image a, image b, পূর্ণসংখ্যা dx)
{
    যদি(a.w == 0) ফেরত copy_image(b);
    image c = make_image(a.w + b.w + dx, (a.h > b.h) ? a.h : b.h, (a.c > b.c) ? a.c : b.c);
    fill_cpu(c.w*c.h*c.c, 1, c.তথ্য, 1);
    embed_image(a, c, 0, 0); 
    composite_image(b, c, a.w + dx, 0);
    ফেরত c;
}

image get_label(image **characters, অক্ষর *string, পূর্ণসংখ্যা আকার_2)
{
    আকার_2 = আকার_2/10;
    যদি(আকার_2 > 7) আকার_2 = 7;
    image label = make_empty_image(0,0,0);
    যখন(*string){
        image l = characters[আকার_2][(পূর্ণসংখ্যা)*string];
        image n = tile_images(label, l, -আকার_2 - 1 + (আকার_2+1)/2);
        free_image(label);
        label = n;
        ++string;
    }
    image b = border_image(label, label.h*.25);
    free_image(label);
    ফেরত b;
}

শূন্য draw_label(image a, পূর্ণসংখ্যা r, পূর্ণসংখ্যা c, image label, ধ্রুবক দশমিক *rgb)
{
    পূর্ণসংখ্যা w = label.w;
    পূর্ণসংখ্যা h = label.h;
    যদি (r - h >= 0) r = r - h;

    পূর্ণসংখ্যা i, j, k;
    জন্য(j = 0; j < h && j + r < a.h; ++j){
        জন্য(i = 0; i < w && i + c < a.w; ++i){
            জন্য(k = 0; k < label.c; ++k){
                দশমিক val = get_pixel(label, i, j, k);
                set_pixel(a, i+c, j+r, k, rgb[k] * val);
            }
        }
    }
}

শূন্য draw_box(image a, পূর্ণসংখ্যা x1, পূর্ণসংখ্যা y1, পূর্ণসংখ্যা x2, পূর্ণসংখ্যা y2, দশমিক r, দশমিক g, দশমিক b)
{
    //normalize_image(a);
    পূর্ণসংখ্যা i;
    যদি(x1 < 0) x1 = 0;
    যদি(x1 >= a.w) x1 = a.w-1;
    যদি(x2 < 0) x2 = 0;
    যদি(x2 >= a.w) x2 = a.w-1;

    যদি(y1 < 0) y1 = 0;
    যদি(y1 >= a.h) y1 = a.h-1;
    যদি(y2 < 0) y2 = 0;
    যদি(y2 >= a.h) y2 = a.h-1;

    জন্য(i = x1; i <= x2; ++i){
        a.তথ্য[i + y1*a.w + 0*a.w*a.h] = r;
        a.তথ্য[i + y2*a.w + 0*a.w*a.h] = r;

        a.তথ্য[i + y1*a.w + 1*a.w*a.h] = g;
        a.তথ্য[i + y2*a.w + 1*a.w*a.h] = g;

        a.তথ্য[i + y1*a.w + 2*a.w*a.h] = b;
        a.তথ্য[i + y2*a.w + 2*a.w*a.h] = b;
    }
    জন্য(i = y1; i <= y2; ++i){
        a.তথ্য[x1 + i*a.w + 0*a.w*a.h] = r;
        a.তথ্য[x2 + i*a.w + 0*a.w*a.h] = r;

        a.তথ্য[x1 + i*a.w + 1*a.w*a.h] = g;
        a.তথ্য[x2 + i*a.w + 1*a.w*a.h] = g;

        a.তথ্য[x1 + i*a.w + 2*a.w*a.h] = b;
        a.তথ্য[x2 + i*a.w + 2*a.w*a.h] = b;
    }
}

শূন্য draw_box_width(image a, পূর্ণসংখ্যা x1, পূর্ণসংখ্যা y1, পূর্ণসংখ্যা x2, পূর্ণসংখ্যা y2, পূর্ণসংখ্যা w, দশমিক r, দশমিক g, দশমিক b)
{
    পূর্ণসংখ্যা i;
    জন্য(i = 0; i < w; ++i){
        draw_box(a, x1+i, y1+i, x2-i, y2-i, r, g, b);
    }
}

শূন্য draw_bbox(image a, box bbox, পূর্ণসংখ্যা w, দশমিক r, দশমিক g, দশমিক b)
{
    পূর্ণসংখ্যা বাম  = (bbox.x-bbox.w/2)*a.w;
    পূর্ণসংখ্যা ডান = (bbox.x+bbox.w/2)*a.w;
    পূর্ণসংখ্যা শীর্ষ   = (bbox.y-bbox.h/2)*a.h;
    পূর্ণসংখ্যা bot   = (bbox.y+bbox.h/2)*a.h;

    পূর্ণসংখ্যা i;
    জন্য(i = 0; i < w; ++i){
        draw_box(a, বাম+i, শীর্ষ+i, ডান-i, bot-i, r, g, b);
    }
}

image **load_alphabet()
{
    পূর্ণসংখ্যা i, j;
    ধ্রুবক পূর্ণসংখ্যা nsize = 8;
    image **alphabets = calloc(nsize, আকার(image));
    জন্য(j = 0; j < nsize; ++j){
        alphabets[j] = calloc(128, আকার(image));
        জন্য(i = 32; i < 127; ++i){
            অক্ষর buff[256];
            sprintf(buff, "data/labels/%d_%d.png", i, j);
            alphabets[j][i] = load_image_color(buff, 0, 0);
        }
    }
    ফেরত alphabets;
}

শূন্য draw_detections(image im, detection *dets, পূর্ণসংখ্যা অঙ্ক, দশমিক thresh, অক্ষর **names, image **alphabet, পূর্ণসংখ্যা classes)
{
    পূর্ণসংখ্যা i,j;

    জন্য(i = 0; i < অঙ্ক; ++i){
        অক্ষর labelstr[4096] = {0};
        পূর্ণসংখ্যা class = -1;
        জন্য(j = 0; j < classes; ++j){
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
            পূর্ণসংখ্যা width = im.h * .006;

            /*
               if(0){
               width = pow(prob, 1./2.)*10+1;
               alphabet = 0;
               }
             */

            //printf("%d %s: %.0f%%\n", i, names[class], prob*100);
            পূর্ণসংখ্যা offset = class*123457 % classes;
            দশমিক red = get_color(2,offset,classes);
            দশমিক green = get_color(1,offset,classes);
            দশমিক blue = get_color(0,offset,classes);
            দশমিক rgb[3];

            //width = prob*20+2;

            rgb[0] = red;
            rgb[1] = green;
            rgb[2] = blue;
            box b = dets[i].bbox;
            //printf("%f %f %f %f\n", b.x, b.y, b.w, b.h);

            পূর্ণসংখ্যা বাম  = (b.x-b.w/2.)*im.w;
            পূর্ণসংখ্যা ডান = (b.x+b.w/2.)*im.w;
            পূর্ণসংখ্যা শীর্ষ   = (b.y-b.h/2.)*im.h;
            পূর্ণসংখ্যা bot   = (b.y+b.h/2.)*im.h;

            যদি(বাম < 0) বাম = 0;
            যদি(ডান > im.w-1) ডান = im.w-1;
            যদি(শীর্ষ < 0) শীর্ষ = 0;
            যদি(bot > im.h-1) bot = im.h-1;

            draw_box_width(im, বাম, শীর্ষ, ডান, bot, width, red, green, blue);
            যদি (alphabet) {
                image label = get_label(alphabet, labelstr, (im.h*.03));
                draw_label(im, শীর্ষ + width, বাম, label, rgb);
                free_image(label);
            }
            যদি (dets[i].mask){
                image mask = float_to_image(14, 14, 1, dets[i].mask);
                image resized_mask = resize_image(mask, b.w*im.w, b.h*im.h);
                image tmask = threshold_image(resized_mask, .5);
                embed_image(tmask, im, বাম, শীর্ষ);
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
    পূর্ণসংখ্যা n, m;
    পূর্ণসংখ্যা c;
    জন্য(c = 0; c < im.c; ++c){
        জন্য(n = 0; n < im.w-1; ++n){
            জন্য(m = n + 1; m < im.w; ++m){
                দশমিক swap = im.তথ্য[m + im.w*(n + im.h*c)];
                im.তথ্য[m + im.w*(n + im.h*c)] = im.তথ্য[n + im.w*(m + im.h*c)];
                im.তথ্য[n + im.w*(m + im.h*c)] = swap;
            }
        }
    }
}

শূন্য rotate_image_cw(image im, পূর্ণসংখ্যা times)
{
    প্রতিজ্ঞা(im.w == im.h);
    times = (times + 400) % 4;
    পূর্ণসংখ্যা i, x, y, c;
    পূর্ণসংখ্যা n = im.w;
    জন্য(i = 0; i < times; ++i){
        জন্য(c = 0; c < im.c; ++c){
            জন্য(x = 0; x < n/2; ++x){
                জন্য(y = 0; y < (n-1)/2 + 1; ++y){
                    দশমিক অস্থায়ী = im.তথ্য[y + im.w*(x + im.h*c)];
                    im.তথ্য[y + im.w*(x + im.h*c)] = im.তথ্য[n-1-x + im.w*(y + im.h*c)];
                    im.তথ্য[n-1-x + im.w*(y + im.h*c)] = im.তথ্য[n-1-y + im.w*(n-1-x + im.h*c)];
                    im.তথ্য[n-1-y + im.w*(n-1-x + im.h*c)] = im.তথ্য[x + im.w*(n-1-y + im.h*c)];
                    im.তথ্য[x + im.w*(n-1-y + im.h*c)] = অস্থায়ী;
                }
            }
        }
    }
}

শূন্য flip_image(image a)
{
    পূর্ণসংখ্যা i,j,k;
    জন্য(k = 0; k < a.c; ++k){
        জন্য(i = 0; i < a.h; ++i){
            জন্য(j = 0; j < a.w/2; ++j){
                পূর্ণসংখ্যা সূচক = j + a.w*(i + a.h*(k));
                পূর্ণসংখ্যা flip = (a.w - j - 1) + a.w*(i + a.h*(k));
                দশমিক swap = a.তথ্য[flip];
                a.তথ্য[flip] = a.তথ্য[সূচক];
                a.তথ্য[সূচক] = swap;
            }
        }
    }
}

image image_distance(image a, image b)
{
    পূর্ণসংখ্যা i,j;
    image dist = make_image(a.w, a.h, 1);
    জন্য(i = 0; i < a.c; ++i){
        জন্য(j = 0; j < a.h*a.w; ++j){
            dist.তথ্য[j] += pow(a.তথ্য[i*a.h*a.w+j]-b.তথ্য[i*a.h*a.w+j],2);
        }
    }
    জন্য(j = 0; j < a.h*a.w; ++j){
        dist.তথ্য[j] = sqrt(dist.তথ্য[j]);
    }
    ফেরত dist;
}

শূন্য ghost_image(image source, image dest, পূর্ণসংখ্যা dx, পূর্ণসংখ্যা dy)
{
    পূর্ণসংখ্যা x,y,k;
    দশমিক max_dist = sqrt((-source.w/2. + .5)*(-source.w/2. + .5));
    জন্য(k = 0; k < source.c; ++k){
        জন্য(y = 0; y < source.h; ++y){
            জন্য(x = 0; x < source.w; ++x){
                দশমিক dist = sqrt((x - source.w/2. + .5)*(x - source.w/2. + .5) + (y - source.h/2. + .5)*(y - source.h/2. + .5));
                দশমিক alpha = (1 - dist/max_dist);
                যদি(alpha < 0) alpha = 0;
                দশমিক v1 = get_pixel(source, x,y,k);
                দশমিক v2 = get_pixel(dest, dx+x,dy+y,k);
                দশমিক val = alpha*v1 + (1-alpha)*v2;
                set_pixel(dest, dx+x, dy+y, k, val);
            }
        }
    }
}

শূন্য blocky_image(image im, পূর্ণসংখ্যা s)
{
    পূর্ণসংখ্যা i,j,k;
    জন্য(k = 0; k < im.c; ++k){
        জন্য(j = 0; j < im.h; ++j){
            জন্য(i = 0; i < im.w; ++i){
                im.তথ্য[i + im.w*(j + im.h*k)] = im.তথ্য[i/s*s + im.w*(j/s*s + im.h*k)];
            }
        }
    }
}

শূন্য censor_image(image im, পূর্ণসংখ্যা dx, পূর্ণসংখ্যা dy, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h)
{
    পূর্ণসংখ্যা i,j,k;
    পূর্ণসংখ্যা s = 32;
    যদি(dx < 0) dx = 0;
    যদি(dy < 0) dy = 0;

    জন্য(k = 0; k < im.c; ++k){
        জন্য(j = dy; j < dy + h && j < im.h; ++j){
            জন্য(i = dx; i < dx + w && i < im.w; ++i){
                im.তথ্য[i + im.w*(j + im.h*k)] = im.তথ্য[i/s*s + im.w*(j/s*s + im.h*k)];
                //im.data[i + j*im.w + k*im.w*im.h] = 0;
            }
        }
    }
}

শূন্য embed_image(image source, image dest, পূর্ণসংখ্যা dx, পূর্ণসংখ্যা dy)
{
    পূর্ণসংখ্যা x,y,k;
    জন্য(k = 0; k < source.c; ++k){
        জন্য(y = 0; y < source.h; ++y){
            জন্য(x = 0; x < source.w; ++x){
                দশমিক val = get_pixel(source, x,y,k);
                set_pixel(dest, dx+x, dy+y, k, val);
            }
        }
    }
}

image collapse_image_layers(image source, পূর্ণসংখ্যা border)
{
    পূর্ণসংখ্যা h = source.h;
    h = (h+border)*source.c - border;
    image dest = make_image(source.w, h, 1);
    পূর্ণসংখ্যা i;
    জন্য(i = 0; i < source.c; ++i){
        image layer = get_image_layer(source, i);
        পূর্ণসংখ্যা h_offset = i*(source.h+border);
        embed_image(layer, dest, 0, h_offset);
        free_image(layer);
    }
    ফেরত dest;
}

শূন্য constrain_image(image im)
{
    পূর্ণসংখ্যা i;
    জন্য(i = 0; i < im.w*im.h*im.c; ++i){
        যদি(im.তথ্য[i] < 0) im.তথ্য[i] = 0;
        যদি(im.তথ্য[i] > 1) im.তথ্য[i] = 1;
    }
}

শূন্য normalize_image(image p)
{
    পূর্ণসংখ্যা i;
    দশমিক min = 9999999;
    দশমিক max = -999999;

    জন্য(i = 0; i < p.h*p.w*p.c; ++i){
        দশমিক v = p.তথ্য[i];
        যদি(v < min) min = v;
        যদি(v > max) max = v;
    }
    যদি(max - min < .000000001){
        min = 0;
        max = 1;
    }
    জন্য(i = 0; i < p.c*p.w*p.h; ++i){
        p.তথ্য[i] = (p.তথ্য[i] - min)/(max-min);
    }
}

শূন্য normalize_image2(image p)
{
    দশমিক *min = calloc(p.c, আকার(দশমিক));
    দশমিক *max = calloc(p.c, আকার(দশমিক));
    পূর্ণসংখ্যা i,j;
    জন্য(i = 0; i < p.c; ++i) min[i] = max[i] = p.তথ্য[i*p.h*p.w];

    জন্য(j = 0; j < p.c; ++j){
        জন্য(i = 0; i < p.h*p.w; ++i){
            দশমিক v = p.তথ্য[i+j*p.h*p.w];
            যদি(v < min[j]) min[j] = v;
            যদি(v > max[j]) max[j] = v;
        }
    }
    জন্য(i = 0; i < p.c; ++i){
        যদি(max[i] - min[i] < .000000001){
            min[i] = 0;
            max[i] = 1;
        }
    }
    জন্য(j = 0; j < p.c; ++j){
        জন্য(i = 0; i < p.w*p.h; ++i){
            p.তথ্য[i+j*p.h*p.w] = (p.তথ্য[i+j*p.h*p.w] - min[j])/(max[j]-min[j]);
        }
    }
    মুক্ত(min);
    মুক্ত(max);
}

শূন্য copy_image_into(image src, image dest)
{
    memcpy(dest.তথ্য, src.তথ্য, src.h*src.w*src.c*আকার(দশমিক));
}

image copy_image(image p)
{
    image copy = p;
    copy.তথ্য = calloc(p.h*p.w*p.c, আকার(দশমিক));
    memcpy(copy.তথ্য, p.তথ্য, p.h*p.w*p.c*আকার(দশমিক));
    ফেরত copy;
}

শূন্য rgbgr_image(image im)
{
    পূর্ণসংখ্যা i;
    জন্য(i = 0; i < im.w*im.h; ++i){
        দশমিক swap = im.তথ্য[i];
        im.তথ্য[i] = im.তথ্য[i+im.w*im.h*2];
        im.তথ্য[i+im.w*im.h*2] = swap;
    }
}

পূর্ণসংখ্যা show_image(image p, ধ্রুবক অক্ষর *name, পূর্ণসংখ্যা ms)
{
#ifdef OPENCV
    পূর্ণসংখ্যা c = show_image_cv(p, name, ms);
    ফেরত c;
#অন্যথা
    fprintf(stderr, "Not compiled with OpenCV, saving to %s.png instead\n", name);
    save_image(p, name);
    ফেরত -1;
#endif
}

শূন্য save_image_options(image im, ধ্রুবক অক্ষর *name, IMTYPE f, পূর্ণসংখ্যা quality)
{
    অক্ষর buff[256];
    //sprintf(buff, "%s (%d)", name, windows);
    যদি(f == PNG)       sprintf(buff, "%s.png", name);
    অন্যথা যদি (f == BMP) sprintf(buff, "%s.bmp", name);
    অন্যথা যদি (f == TGA) sprintf(buff, "%s.tga", name);
    অন্যথা যদি (f == JPG) sprintf(buff, "%s.jpg", name);
    অন্যথা               sprintf(buff, "%s.png", name);
    unsigned অক্ষর *তথ্য = calloc(im.w*im.h*im.c, আকার(অক্ষর));
    পূর্ণসংখ্যা i,k;
    জন্য(k = 0; k < im.c; ++k){
        জন্য(i = 0; i < im.w*im.h; ++i){
            তথ্য[i*im.c+k] = (unsigned অক্ষর) (255*im.তথ্য[i + k*im.w*im.h]);
        }
    }
    পূর্ণসংখ্যা success = 0;
    যদি(f == PNG)       success = stbi_write_png(buff, im.w, im.h, im.c, তথ্য, im.w*im.c);
    অন্যথা যদি (f == BMP) success = stbi_write_bmp(buff, im.w, im.h, im.c, তথ্য);
    অন্যথা যদি (f == TGA) success = stbi_write_tga(buff, im.w, im.h, im.c, তথ্য);
    অন্যথা যদি (f == JPG) success = stbi_write_jpg(buff, im.w, im.h, im.c, তথ্য, quality);
    মুক্ত(তথ্য);
    যদি(!success) fprintf(stderr, "Failed to write image %s\n", buff);
}

শূন্য save_image(image im, ধ্রুবক অক্ষর *name)
{
    save_image_options(im, name, JPG, 80);
}

শূন্য show_image_layers(image p, অক্ষর *name)
{
    পূর্ণসংখ্যা i;
    অক্ষর buff[256];
    জন্য(i = 0; i < p.c; ++i){
        sprintf(buff, "%s - Layer %d", name, i);
        image layer = get_image_layer(p, i);
        show_image(layer, buff, 1);
        free_image(layer);
    }
}

শূন্য show_image_collapsed(image p, অক্ষর *name)
{
    image c = collapse_image_layers(p, 1);
    show_image(c, name, 1);
    free_image(c);
}

image make_empty_image(পূর্ণসংখ্যা w, পূর্ণসংখ্যা h, পূর্ণসংখ্যা c)
{
    image out;
    out.তথ্য = 0;
    out.h = h;
    out.w = w;
    out.c = c;
    ফেরত out;
}

image make_image(পূর্ণসংখ্যা w, পূর্ণসংখ্যা h, পূর্ণসংখ্যা c)
{
    image out = make_empty_image(w,h,c);
    out.তথ্য = calloc(h*w*c, আকার(দশমিক));
    ফেরত out;
}

image make_random_image(পূর্ণসংখ্যা w, পূর্ণসংখ্যা h, পূর্ণসংখ্যা c)
{
    image out = make_empty_image(w,h,c);
    out.তথ্য = calloc(h*w*c, আকার(দশমিক));
    পূর্ণসংখ্যা i;
    জন্য(i = 0; i < w*h*c; ++i){
        out.তথ্য[i] = (rand_normal() * .25) + .5;
    }
    ফেরত out;
}

image float_to_image(পূর্ণসংখ্যা w, পূর্ণসংখ্যা h, পূর্ণসংখ্যা c, দশমিক *তথ্য)
{
    image out = make_empty_image(w,h,c);
    out.তথ্য = তথ্য;
    ফেরত out;
}

শূন্য place_image(image im, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h, পূর্ণসংখ্যা dx, পূর্ণসংখ্যা dy, image canvas)
{
    পূর্ণসংখ্যা x, y, c;
    জন্য(c = 0; c < im.c; ++c){
        জন্য(y = 0; y < h; ++y){
            জন্য(x = 0; x < w; ++x){
                দশমিক rx = ((দশমিক)x / w) * im.w;
                দশমিক ry = ((দশমিক)y / h) * im.h;
                দশমিক val = bilinear_interpolate(im, rx, ry, c);
                set_pixel(canvas, x + dx, y + dy, c, val);
            }
        }
    }
}

image center_crop_image(image im, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h)
{
    পূর্ণসংখ্যা m = (im.w < im.h) ? im.w : im.h;   
    image c = crop_image(im, (im.w - m) / 2, (im.h - m)/2, m, m);
    image r = resize_image(c, w, h);
    free_image(c);
    ফেরত r;
}

image rotate_crop_image(image im, দশমিক rad, দশমিক s, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h, দশমিক dx, দশমিক dy, দশমিক aspect)
{
    পূর্ণসংখ্যা x, y, c;
    দশমিক cx = im.w/2.;
    দশমিক cy = im.h/2.;
    image rot = make_image(w, h, im.c);
    জন্য(c = 0; c < im.c; ++c){
        জন্য(y = 0; y < h; ++y){
            জন্য(x = 0; x < w; ++x){
                দশমিক rx = cos(rad)*((x - w/2.)/s*aspect + dx/s*aspect) - sin(rad)*((y - h/2.)/s + dy/s) + cx;
                দশমিক ry = sin(rad)*((x - w/2.)/s*aspect + dx/s*aspect) + cos(rad)*((y - h/2.)/s + dy/s) + cy;
                দশমিক val = bilinear_interpolate(im, rx, ry, c);
                set_pixel(rot, x, y, c, val);
            }
        }
    }
    ফেরত rot;
}

image rotate_image(image im, দশমিক rad)
{
    পূর্ণসংখ্যা x, y, c;
    দশমিক cx = im.w/2.;
    দশমিক cy = im.h/2.;
    image rot = make_image(im.w, im.h, im.c);
    জন্য(c = 0; c < im.c; ++c){
        জন্য(y = 0; y < im.h; ++y){
            জন্য(x = 0; x < im.w; ++x){
                দশমিক rx = cos(rad)*(x-cx) - sin(rad)*(y-cy) + cx;
                দশমিক ry = sin(rad)*(x-cx) + cos(rad)*(y-cy) + cy;
                দশমিক val = bilinear_interpolate(im, rx, ry, c);
                set_pixel(rot, x, y, c, val);
            }
        }
    }
    ফেরত rot;
}

শূন্য fill_image(image m, দশমিক s)
{
    পূর্ণসংখ্যা i;
    জন্য(i = 0; i < m.h*m.w*m.c; ++i) m.তথ্য[i] = s;
}

শূন্য translate_image(image m, দশমিক s)
{
    পূর্ণসংখ্যা i;
    জন্য(i = 0; i < m.h*m.w*m.c; ++i) m.তথ্য[i] += s;
}

শূন্য scale_image(image m, দশমিক s)
{
    পূর্ণসংখ্যা i;
    জন্য(i = 0; i < m.h*m.w*m.c; ++i) m.তথ্য[i] *= s;
}

image crop_image(image im, পূর্ণসংখ্যা dx, পূর্ণসংখ্যা dy, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h)
{
    image cropped = make_image(w, h, im.c);
    পূর্ণসংখ্যা i, j, k;
    জন্য(k = 0; k < im.c; ++k){
        জন্য(j = 0; j < h; ++j){
            জন্য(i = 0; i < w; ++i){
                পূর্ণসংখ্যা r = j + dy;
                পূর্ণসংখ্যা c = i + dx;
                দশমিক val = 0;
                r = constrain_int(r, 0, im.h-1);
                c = constrain_int(c, 0, im.w-1);
                val = get_pixel(im, c, r, k);
                set_pixel(cropped, i, j, k, val);
            }
        }
    }
    ফেরত cropped;
}

পূর্ণসংখ্যা best_3d_shift_r(image a, image b, পূর্ণসংখ্যা min, পূর্ণসংখ্যা max)
{
    যদি(min == max) ফেরত min;
    পূর্ণসংখ্যা mid = floor((min + max) / 2.);
    image c1 = crop_image(b, 0, mid, b.w, b.h);
    image c2 = crop_image(b, 0, mid+1, b.w, b.h);
    দশমিক d1 = dist_array(c1.তথ্য, a.তথ্য, a.w*a.h*a.c, 10);
    দশমিক d2 = dist_array(c2.তথ্য, a.তথ্য, a.w*a.h*a.c, 10);
    free_image(c1);
    free_image(c2);
    যদি(d1 < d2) ফেরত best_3d_shift_r(a, b, min, mid);
    অন্যথা ফেরত best_3d_shift_r(a, b, mid+1, max);
}

পূর্ণসংখ্যা best_3d_shift(image a, image b, পূর্ণসংখ্যা min, পূর্ণসংখ্যা max)
{
    পূর্ণসংখ্যা i;
    পূর্ণসংখ্যা best = 0;
    দশমিক best_distance = FLT_MAX;
    জন্য(i = min; i <= max; i += 2){
        image c = crop_image(b, 0, i, b.w, b.h);
        দশমিক d = dist_array(c.তথ্য, a.তথ্য, a.w*a.h*a.c, 100);
        যদি(d < best_distance){
            best_distance = d;
            best = i;
        }
        printf("%d %f\n", i, d);
        free_image(c);
    }
    ফেরত best;
}

শূন্য composite_3d(অক্ষর *f1, অক্ষর *f2, অক্ষর *out, পূর্ণসংখ্যা delta)
{
    যদি(!out) out = "out";
    image a = load_image(f1, 0,0,0);
    image b = load_image(f2, 0,0,0);
    পূর্ণসংখ্যা shift = best_3d_shift_r(a, b, -a.h/100, a.h/100);

    image c1 = crop_image(b, 10, shift, b.w, b.h);
    দশমিক d1 = dist_array(c1.তথ্য, a.তথ্য, a.w*a.h*a.c, 100);
    image c2 = crop_image(b, -10, shift, b.w, b.h);
    দশমিক d2 = dist_array(c2.তথ্য, a.তথ্য, a.w*a.h*a.c, 100);

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
    পূর্ণসংখ্যা i;
    জন্য(i = 0; i < c.w*c.h; ++i){
        c.তথ্য[i] = a.তথ্য[i];
    }
    save_image(c, out);
}

শূন্য letterbox_image_into(image im, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h, image boxed)
{
    পূর্ণসংখ্যা new_w = im.w;
    পূর্ণসংখ্যা new_h = im.h;
    যদি (((দশমিক)w/im.w) < ((দশমিক)h/im.h)) {
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

image letterbox_image(image im, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h)
{
    পূর্ণসংখ্যা new_w = im.w;
    পূর্ণসংখ্যা new_h = im.h;
    যদি (((দশমিক)w/im.w) < ((দশমিক)h/im.h)) {
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
    ফেরত boxed;
}

image resize_max(image im, পূর্ণসংখ্যা max)
{
    পূর্ণসংখ্যা w = im.w;
    পূর্ণসংখ্যা h = im.h;
    যদি(w > h){
        h = (h * max) / w;
        w = max;
    } অন্যথা {
        w = (w * max) / h;
        h = max;
    }
    যদি(w == im.w && h == im.h) ফেরত im;
    image resized = resize_image(im, w, h);
    ফেরত resized;
}

image resize_min(image im, পূর্ণসংখ্যা min)
{
    পূর্ণসংখ্যা w = im.w;
    পূর্ণসংখ্যা h = im.h;
    যদি(w < h){
        h = (h * min) / w;
        w = min;
    } অন্যথা {
        w = (w * min) / h;
        h = min;
    }
    যদি(w == im.w && h == im.h) ফেরত im;
    image resized = resize_image(im, w, h);
    ফেরত resized;
}

image random_crop_image(image im, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h)
{
    পূর্ণসংখ্যা dx = rand_int(0, im.w - w);
    পূর্ণসংখ্যা dy = rand_int(0, im.h - h);
    image crop = crop_image(im, dx, dy, w, h);
    ফেরত crop;
}

augment_args random_augment_args(image im, দশমিক angle, দশমিক aspect, পূর্ণসংখ্যা নিচের, পূর্ণসংখ্যা উপরের, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h)
{
    augment_args a = {0};
    aspect = rand_scale(aspect);
    পূর্ণসংখ্যা r = rand_int(নিচের, উপরের);
    পূর্ণসংখ্যা min = (im.h < im.w*aspect) ? im.h : im.w*aspect;
    দশমিক scale = (দশমিক)r / min;

    দশমিক rad = rand_uniform(-angle, angle) * TWO_PI / 360.;

    দশমিক dx = (im.w*scale/aspect - w) / 2.;
    দশমিক dy = (im.h*scale - w) / 2.;
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
    ফেরত a;
}

image random_augment_image(image im, দশমিক angle, দশমিক aspect, পূর্ণসংখ্যা নিচের, পূর্ণসংখ্যা উপরের, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h)
{
    augment_args a = random_augment_args(im, angle, aspect, নিচের, উপরের, w, h);
    image crop = rotate_crop_image(im, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);
    ফেরত crop;
}

দশমিক three_way_max(দশমিক a, দশমিক b, দশমিক c)
{
    ফেরত (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

দশমিক three_way_min(দশমিক a, দশমিক b, দশমিক c)
{
    ফেরত (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

শূন্য yuv_to_rgb(image im)
{
    প্রতিজ্ঞা(im.c == 3);
    পূর্ণসংখ্যা i, j;
    দশমিক r, g, b;
    দশমিক y, u, v;
    জন্য(j = 0; j < im.h; ++j){
        জন্য(i = 0; i < im.w; ++i){
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
    পূর্ণসংখ্যা i, j;
    দশমিক r, g, b;
    দশমিক y, u, v;
    জন্য(j = 0; j < im.h; ++j){
        জন্য(i = 0; i < im.w; ++i){
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
    পূর্ণসংখ্যা i, j;
    দশমিক r, g, b;
    দশমিক h, s, v;
    জন্য(j = 0; j < im.h; ++j){
        জন্য(i = 0; i < im.w; ++i){
            r = get_pixel(im, i , j, 0);
            g = get_pixel(im, i , j, 1);
            b = get_pixel(im, i , j, 2);
            দশমিক max = three_way_max(r,g,b);
            দশমিক min = three_way_min(r,g,b);
            দশমিক delta = max - min;
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
    পূর্ণসংখ্যা i, j;
    দশমিক r, g, b;
    দশমিক h, s, v;
    দশমিক f, p, q, t;
    জন্য(j = 0; j < im.h; ++j){
        জন্য(i = 0; i < im.w; ++i){
            h = 6 * get_pixel(im, i , j, 0);
            s = get_pixel(im, i , j, 1);
            v = get_pixel(im, i , j, 2);
            যদি (s == 0) {
                r = g = b = v;
            } অন্যথা {
                পূর্ণসংখ্যা সূচক = floor(h);
                f = h - সূচক;
                p = v*(1-s);
                q = v*(1-s*f);
                t = v*(1-s*(1-f));
                যদি(সূচক == 0){
                    r = v; g = t; b = p;
                } অন্যথা যদি(সূচক == 1){
                    r = q; g = v; b = p;
                } অন্যথা যদি(সূচক == 2){
                    r = p; g = v; b = t;
                } অন্যথা যদি(সূচক == 3){
                    r = p; g = q; b = v;
                } অন্যথা যদি(সূচক == 4){
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
    পূর্ণসংখ্যা i, j, k;
    দশমিক scale[] = {0.299, 0.587, 0.114};
    জন্য(j = 0; j < im.h; ++j){
        জন্য(i = 0; i < im.w; ++i){
            দশমিক val = 0;
            জন্য(k = 0; k < 3; ++k){
                val += scale[k]*get_pixel(im, i, j, k);
            }
            im.তথ্য[0*im.h*im.w + im.w*j + i] = val;
            im.তথ্য[1*im.h*im.w + im.w*j + i] = val;
            im.তথ্য[2*im.h*im.w + im.w*j + i] = val;
        }
    }
}

image grayscale_image(image im)
{
    প্রতিজ্ঞা(im.c == 3);
    পূর্ণসংখ্যা i, j, k;
    image gray = make_image(im.w, im.h, 1);
    দশমিক scale[] = {0.299, 0.587, 0.114};
    জন্য(k = 0; k < im.c; ++k){
        জন্য(j = 0; j < im.h; ++j){
            জন্য(i = 0; i < im.w; ++i){
                gray.তথ্য[i+im.w*j] += scale[k]*get_pixel(im, i, j, k);
            }
        }
    }
    ফেরত gray;
}

image threshold_image(image im, দশমিক thresh)
{
    পূর্ণসংখ্যা i;
    image t = make_image(im.w, im.h, im.c);
    জন্য(i = 0; i < im.w*im.h*im.c; ++i){
        t.তথ্য[i] = im.তথ্য[i]>thresh ? 1 : 0;
    }
    ফেরত t;
}

image blend_image(image fore, image back, দশমিক alpha)
{
    প্রতিজ্ঞা(fore.w == back.w && fore.h == back.h && fore.c == back.c);
    image blend = make_image(fore.w, fore.h, fore.c);
    পূর্ণসংখ্যা i, j, k;
    জন্য(k = 0; k < fore.c; ++k){
        জন্য(j = 0; j < fore.h; ++j){
            জন্য(i = 0; i < fore.w; ++i){
                দশমিক val = alpha * get_pixel(fore, i, j, k) + 
                    (1 - alpha)* get_pixel(back, i, j, k);
                set_pixel(blend, i, j, k, val);
            }
        }
    }
    ফেরত blend;
}

শূন্য scale_image_channel(image im, পূর্ণসংখ্যা c, দশমিক v)
{
    পূর্ণসংখ্যা i, j;
    জন্য(j = 0; j < im.h; ++j){
        জন্য(i = 0; i < im.w; ++i){
            দশমিক pix = get_pixel(im, i, j, c);
            pix = pix*v;
            set_pixel(im, i, j, c, pix);
        }
    }
}

শূন্য translate_image_channel(image im, পূর্ণসংখ্যা c, দশমিক v)
{
    পূর্ণসংখ্যা i, j;
    জন্য(j = 0; j < im.h; ++j){
        জন্য(i = 0; i < im.w; ++i){
            দশমিক pix = get_pixel(im, i, j, c);
            pix = pix+v;
            set_pixel(im, i, j, c, pix);
        }
    }
}

image binarize_image(image im)
{
    image c = copy_image(im);
    পূর্ণসংখ্যা i;
    জন্য(i = 0; i < im.w * im.h * im.c; ++i){
        যদি(c.তথ্য[i] > .5) c.তথ্য[i] = 1;
        অন্যথা c.তথ্য[i] = 0;
    }
    ফেরত c;
}

শূন্য saturate_image(image im, দশমিক sat)
{
    rgb_to_hsv(im);
    scale_image_channel(im, 1, sat);
    hsv_to_rgb(im);
    constrain_image(im);
}

শূন্য hue_image(image im, দশমিক hue)
{
    rgb_to_hsv(im);
    পূর্ণসংখ্যা i;
    জন্য(i = 0; i < im.w*im.h; ++i){
        im.তথ্য[i] = im.তথ্য[i] + hue;
        যদি (im.তথ্য[i] > 1) im.তথ্য[i] -= 1;
        যদি (im.তথ্য[i] < 0) im.তথ্য[i] += 1;
    }
    hsv_to_rgb(im);
    constrain_image(im);
}

শূন্য exposure_image(image im, দশমিক sat)
{
    rgb_to_hsv(im);
    scale_image_channel(im, 2, sat);
    hsv_to_rgb(im);
    constrain_image(im);
}

শূন্য distort_image(image im, দশমিক hue, দশমিক sat, দশমিক val)
{
    rgb_to_hsv(im);
    scale_image_channel(im, 1, sat);
    scale_image_channel(im, 2, val);
    পূর্ণসংখ্যা i;
    জন্য(i = 0; i < im.w*im.h; ++i){
        im.তথ্য[i] = im.তথ্য[i] + hue;
        যদি (im.তথ্য[i] > 1) im.তথ্য[i] -= 1;
        যদি (im.তথ্য[i] < 0) im.তথ্য[i] += 1;
    }
    hsv_to_rgb(im);
    constrain_image(im);
}

শূন্য random_distort_image(image im, দশমিক hue, দশমিক saturation, দশমিক exposure)
{
    দশমিক dhue = rand_uniform(-hue, hue);
    দশমিক dsat = rand_scale(saturation);
    দশমিক dexp = rand_scale(exposure);
    distort_image(im, dhue, dsat, dexp);
}

শূন্য saturate_exposure_image(image im, দশমিক sat, দশমিক exposure)
{
    rgb_to_hsv(im);
    scale_image_channel(im, 1, sat);
    scale_image_channel(im, 2, exposure);
    hsv_to_rgb(im);
    constrain_image(im);
}

image resize_image(image im, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h)
{
    image resized = make_image(w, h, im.c);   
    image part = make_image(w, im.h, im.c);
    পূর্ণসংখ্যা r, c, k;
    দশমিক w_scale = (দশমিক)(im.w - 1) / (w - 1);
    দশমিক h_scale = (দশমিক)(im.h - 1) / (h - 1);
    জন্য(k = 0; k < im.c; ++k){
        জন্য(r = 0; r < im.h; ++r){
            জন্য(c = 0; c < w; ++c){
                দশমিক val = 0;
                যদি(c == w-1 || im.w == 1){
                    val = get_pixel(im, im.w-1, r, k);
                } অন্যথা {
                    দশমিক sx = c*w_scale;
                    পূর্ণসংখ্যা ix = (পূর্ণসংখ্যা) sx;
                    দশমিক dx = sx - ix;
                    val = (1 - dx) * get_pixel(im, ix, r, k) + dx * get_pixel(im, ix+1, r, k);
                }
                set_pixel(part, c, r, k, val);
            }
        }
    }
    জন্য(k = 0; k < im.c; ++k){
        জন্য(r = 0; r < h; ++r){
            দশমিক sy = r*h_scale;
            পূর্ণসংখ্যা iy = (পূর্ণসংখ্যা) sy;
            দশমিক dy = sy - iy;
            জন্য(c = 0; c < w; ++c){
                দশমিক val = (1-dy) * get_pixel(part, c, iy, k);
                set_pixel(resized, c, r, k, val);
            }
            যদি(r == h-1 || im.h == 1) চালু;
            জন্য(c = 0; c < w; ++c){
                দশমিক val = dy * get_pixel(part, c, iy+1, k);
                add_pixel(resized, c, r, k, val);
            }
        }
    }

    free_image(part);
    ফেরত resized;
}


শূন্য test_resize(অক্ষর *filename)
{
    image im = load_image(filename, 0,0, 3);
    দশমিক mag = mag_array(im.তথ্য, im.w*im.h*im.c);
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
    যখন(1){
        image aug = random_augment_image(im, 0, .75, 320, 448, 320, 320);
        show_image(aug, "aug", 1);
        free_image(aug);


        দশমিক exposure = 1.15;
        দশমিক saturation = 1.15;
        দশমিক hue = .05;

        image c = copy_image(im);

        দশমিক dexp = rand_scale(exposure);
        দশমিক dsat = rand_scale(saturation);
        দশমিক dhue = rand_uniform(-hue, hue);

        distort_image(c, dhue, dsat, dexp);
        show_image(c, "rand", 1);
        printf("%f %f %f\n", dhue, dsat, dexp);
        free_image(c);
    }
#endif
}


image load_image_stb(অক্ষর *filename, পূর্ণসংখ্যা channels)
{
    পূর্ণসংখ্যা w, h, c;
    unsigned অক্ষর *তথ্য = stbi_load(filename, &w, &h, &c, channels);
    যদি (!তথ্য) {
        fprintf(stderr, "Cannot load image \"%s\"\nSTB Reason: %s\n", filename, stbi_failure_reason());
        exit(0);
    }
    যদি(channels) c = channels;
    পূর্ণসংখ্যা i,j,k;
    image im = make_image(w, h, c);
    জন্য(k = 0; k < c; ++k){
        জন্য(j = 0; j < h; ++j){
            জন্য(i = 0; i < w; ++i){
                পূর্ণসংখ্যা dst_সূচক = i + w*j + w*h*k;
                পূর্ণসংখ্যা src_সূচক = k + c*i + c*w*j;
                im.তথ্য[dst_সূচক] = (দশমিক)তথ্য[src_সূচক]/255.;
            }
        }
    }
    মুক্ত(তথ্য);
    ফেরত im;
}

image load_image(অক্ষর *filename, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h, পূর্ণসংখ্যা c)
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
    ফেরত out;
}

image load_image_color(অক্ষর *filename, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h)
{
    ফেরত load_image(filename, w, h, 3);
}

image get_image_layer(image m, পূর্ণসংখ্যা l)
{
    image out = make_image(m.w, m.h, 1);
    পূর্ণসংখ্যা i;
    জন্য(i = 0; i < m.h*m.w; ++i){
        out.তথ্য[i] = m.তথ্য[i+l*m.h*m.w];
    }
    ফেরত out;
}
শূন্য print_image(image m)
{
    পূর্ণসংখ্যা i, j, k;
    জন্য(i =0 ; i < m.c; ++i){
        জন্য(j =0 ; j < m.h; ++j){
            জন্য(k = 0; k < m.w; ++k){
                printf("%.2lf, ", m.তথ্য[i*m.h*m.w + j*m.w + k]);
                যদি(k > 30) বিরতি;
            }
            printf("\n");
            যদি(j > 30) বিরতি;
        }
        printf("\n");
    }
    printf("\n");
}

image collapse_images_vert(image *ims, পূর্ণসংখ্যা n)
{
    পূর্ণসংখ্যা color = 1;
    পূর্ণসংখ্যা border = 1;
    পূর্ণসংখ্যা h,w,c;
    w = ims[0].w;
    h = (ims[0].h + border) * n - border;
    c = ims[0].c;
    যদি(c != 3 || !color){
        w = (w+border)*c - border;
        c = 1;
    }

    image filters = make_image(w, h, c);
    পূর্ণসংখ্যা i,j;
    জন্য(i = 0; i < n; ++i){
        পূর্ণসংখ্যা h_offset = i*(ims[0].h+border);
        image copy = copy_image(ims[i]);
        //normalize_image(copy);
        যদি(c == 3 && color){
            embed_image(copy, filters, 0, h_offset);
        }
        অন্যথা{
            জন্য(j = 0; j < copy.c; ++j){
                পূর্ণসংখ্যা w_offset = j*(ims[0].w+border);
                image layer = get_image_layer(copy, j);
                embed_image(layer, filters, w_offset, h_offset);
                free_image(layer);
            }
        }
        free_image(copy);
    }
    ফেরত filters;
} 

image collapse_images_horz(image *ims, পূর্ণসংখ্যা n)
{
    পূর্ণসংখ্যা color = 1;
    পূর্ণসংখ্যা border = 1;
    পূর্ণসংখ্যা h,w,c;
    পূর্ণসংখ্যা আকার_2 = ims[0].h;
    h = আকার_2;
    w = (ims[0].w + border) * n - border;
    c = ims[0].c;
    যদি(c != 3 || !color){
        h = (h+border)*c - border;
        c = 1;
    }

    image filters = make_image(w, h, c);
    পূর্ণসংখ্যা i,j;
    জন্য(i = 0; i < n; ++i){
        পূর্ণসংখ্যা w_offset = i*(আকার_2+border);
        image copy = copy_image(ims[i]);
        //normalize_image(copy);
        যদি(c == 3 && color){
            embed_image(copy, filters, w_offset, 0);
        }
        অন্যথা{
            জন্য(j = 0; j < copy.c; ++j){
                পূর্ণসংখ্যা h_offset = j*(আকার_2+border);
                image layer = get_image_layer(copy, j);
                embed_image(layer, filters, w_offset, h_offset);
                free_image(layer);
            }
        }
        free_image(copy);
    }
    ফেরত filters;
} 

শূন্য show_image_normalized(image im, ধ্রুবক অক্ষর *name)
{
    image c = copy_image(im);
    normalize_image(c);
    show_image(c, name, 1);
    free_image(c);
}

শূন্য show_images(image *ims, পূর্ণসংখ্যা n, অক্ষর *window)
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
    যদি(m.তথ্য){
        মুক্ত(m.তথ্য);
    }
}
