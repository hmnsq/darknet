#ಅನ್ತರ್ಭಾವ "image.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "blas.h"
#ಅನ್ತರ್ಭಾವ "cuda.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <math.h>

#define STB_IMAGE_IMPLEMENTATION
#ಅನ್ತರ್ಭಾವ "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#ಅನ್ತರ್ಭಾವ "stb_image_write.h"

ಪೂರ್ಣಾಙ್ಕ windows = 0;

ಭಿನ್ನ colors[6][3] = { {1,0,1}, {0,0,1},{0,1,1},{0,1,0},{1,1,0},{1,0,0} };

ಭಿನ್ನ get_color(ಪೂರ್ಣಾಙ್ಕ c, ಪೂರ್ಣಾಙ್ಕ x, ಪೂರ್ಣಾಙ್ಕ max)
{
    ಭಿನ್ನ ratio = ((ಭಿನ್ನ)x/max)*5;
    ಪೂರ್ಣಾಙ್ಕ i = floor(ratio);
    ಪೂರ್ಣಾಙ್ಕ j = ceil(ratio);
    ratio -= i;
    ಭಿನ್ನ r = (1-ratio) * colors[i][c] + ratio*colors[j][c];
    //printf("%f\n", r);
    ಪ್ರತ್ಯಾಗಮ r;
}

image mask_to_rgb(image mask)
{
    ಪೂರ್ಣಾಙ್ಕ n = mask.c;
    image im = make_image(mask.w, mask.h, 3);
    ಪೂರ್ಣಾಙ್ಕ i, j;
    ಪ್ರತಿ(j = 0; j < n; ++j){
        ಪೂರ್ಣಾಙ್ಕ offset = j*123457 % n;
        ಭಿನ್ನ red = get_color(2,offset,n);
        ಭಿನ್ನ green = get_color(1,offset,n);
        ಭಿನ್ನ blue = get_color(0,offset,n);
        ಪ್ರತಿ(i = 0; i < im.w*im.h; ++i){
            im.data[i + 0*im.w*im.h] += mask.data[j*im.h*im.w + i]*red;
            im.data[i + 1*im.w*im.h] += mask.data[j*im.h*im.w + i]*green;
            im.data[i + 2*im.w*im.h] += mask.data[j*im.h*im.w + i]*blue;
        }
    }
    ಪ್ರತ್ಯಾಗಮ im;
}

ಸ್ಥಿರ ಭಿನ್ನ get_pixel(image m, ಪೂರ್ಣಾಙ್ಕ x, ಪೂರ್ಣಾಙ್ಕ y, ಪೂರ್ಣಾಙ್ಕ c)
{
    ಪ್ರತಿಜ್ಞಾ(x < m.w && y < m.h && c < m.c);
    ಪ್ರತ್ಯಾಗಮ m.data[c*m.h*m.w + y*m.w + x];
}
ಸ್ಥಿರ ಭಿನ್ನ get_pixel_extend(image m, ಪೂರ್ಣಾಙ್ಕ x, ಪೂರ್ಣಾಙ್ಕ y, ಪೂರ್ಣಾಙ್ಕ c)
{
    ಯದಿ(x < 0 || x >= m.w || y < 0 || y >= m.h) ಪ್ರತ್ಯಾಗಮ 0;
    /*
    if(x < 0) x = 0;
    if(x >= m.w) x = m.w-1;
    if(y < 0) y = 0;
    if(y >= m.h) y = m.h-1;
    */
    ಯದಿ(c < 0 || c >= m.c) ಪ್ರತ್ಯಾಗಮ 0;
    ಪ್ರತ್ಯಾಗಮ get_pixel(m, x, y, c);
}
ಸ್ಥಿರ ಶೂನ್ಯ set_pixel(image m, ಪೂರ್ಣಾಙ್ಕ x, ಪೂರ್ಣಾಙ್ಕ y, ಪೂರ್ಣಾಙ್ಕ c, ಭಿನ್ನ val)
{
    ಯದಿ (x < 0 || y < 0 || c < 0 || x >= m.w || y >= m.h || c >= m.c) ಪ್ರತ್ಯಾಗಮ;
    ಪ್ರತಿಜ್ಞಾ(x < m.w && y < m.h && c < m.c);
    m.data[c*m.h*m.w + y*m.w + x] = val;
}
ಸ್ಥಿರ ಶೂನ್ಯ add_pixel(image m, ಪೂರ್ಣಾಙ್ಕ x, ಪೂರ್ಣಾಙ್ಕ y, ಪೂರ್ಣಾಙ್ಕ c, ಭಿನ್ನ val)
{
    ಪ್ರತಿಜ್ಞಾ(x < m.w && y < m.h && c < m.c);
    m.data[c*m.h*m.w + y*m.w + x] += val;
}

ಸ್ಥಿರ ಭಿನ್ನ bilinear_interpolate(image im, ಭಿನ್ನ x, ಭಿನ್ನ y, ಪೂರ್ಣಾಙ್ಕ c)
{
    ಪೂರ್ಣಾಙ್ಕ ix = (ಪೂರ್ಣಾಙ್ಕ) floorf(x);
    ಪೂರ್ಣಾಙ್ಕ iy = (ಪೂರ್ಣಾಙ್ಕ) floorf(y);

    ಭಿನ್ನ dx = x - ix;
    ಭಿನ್ನ dy = y - iy;

    ಭಿನ್ನ val = (1-dy) * (1-dx) * get_pixel_extend(im, ix, iy, c) + 
        dy     * (1-dx) * get_pixel_extend(im, ix, iy+1, c) + 
        (1-dy) *   dx   * get_pixel_extend(im, ix+1, iy, c) +
        dy     *   dx   * get_pixel_extend(im, ix+1, iy+1, c);
    ಪ್ರತ್ಯಾಗಮ val;
}


ಶೂನ್ಯ composite_image(image source, image dest, ಪೂರ್ಣಾಙ್ಕ dx, ಪೂರ್ಣಾಙ್ಕ dy)
{
    ಪೂರ್ಣಾಙ್ಕ x,y,k;
    ಪ್ರತಿ(k = 0; k < source.c; ++k){
        ಪ್ರತಿ(y = 0; y < source.h; ++y){
            ಪ್ರತಿ(x = 0; x < source.w; ++x){
                ಭಿನ್ನ val = get_pixel(source, x, y, k);
                ಭಿನ್ನ val2 = get_pixel_extend(dest, dx+x, dy+y, k);
                set_pixel(dest, dx+x, dy+y, k, val * val2);
            }
        }
    }
}

image border_image(image a, ಪೂರ್ಣಾಙ್ಕ border)
{
    image b = make_image(a.w + 2*border, a.h + 2*border, a.c);
    ಪೂರ್ಣಾಙ್ಕ x,y,k;
    ಪ್ರತಿ(k = 0; k < b.c; ++k){
        ಪ್ರತಿ(y = 0; y < b.h; ++y){
            ಪ್ರತಿ(x = 0; x < b.w; ++x){
                ಭಿನ್ನ val = get_pixel_extend(a, x - border, y - border, k);
                ಯದಿ(x - border < 0 || x - border >= a.w || y - border < 0 || y - border >= a.h) val = 1;
                set_pixel(b, x, y, k, val);
            }
        }
    }
    ಪ್ರತ್ಯಾಗಮ b;
}

image tile_images(image a, image b, ಪೂರ್ಣಾಙ್ಕ dx)
{
    ಯದಿ(a.w == 0) ಪ್ರತ್ಯಾಗಮ copy_image(b);
    image c = make_image(a.w + b.w + dx, (a.h > b.h) ? a.h : b.h, (a.c > b.c) ? a.c : b.c);
    fill_cpu(c.w*c.h*c.c, 1, c.data, 1);
    embed_image(a, c, 0, 0); 
    composite_image(b, c, a.w + dx, 0);
    ಪ್ರತ್ಯಾಗಮ c;
}

image get_label(image **characters, ವರ್ಣ *string, ಪೂರ್ಣಾಙ್ಕ size)
{
    size = size/10;
    ಯದಿ(size > 7) size = 7;
    image label = make_empty_image(0,0,0);
    ಯಾವತ್(*string){
        image l = characters[size][(ಪೂರ್ಣಾಙ್ಕ)*string];
        image n = tile_images(label, l, -size - 1 + (size+1)/2);
        free_image(label);
        label = n;
        ++string;
    }
    image b = border_image(label, label.h*.25);
    free_image(label);
    ಪ್ರತ್ಯಾಗಮ b;
}

ಶೂನ್ಯ draw_label(image a, ಪೂರ್ಣಾಙ್ಕ r, ಪೂರ್ಣಾಙ್ಕ c, image label, ಧ್ರುವ ಭಿನ್ನ *rgb)
{
    ಪೂರ್ಣಾಙ್ಕ w = label.w;
    ಪೂರ್ಣಾಙ್ಕ h = label.h;
    ಯದಿ (r - h >= 0) r = r - h;

    ಪೂರ್ಣಾಙ್ಕ i, j, k;
    ಪ್ರತಿ(j = 0; j < h && j + r < a.h; ++j){
        ಪ್ರತಿ(i = 0; i < w && i + c < a.w; ++i){
            ಪ್ರತಿ(k = 0; k < label.c; ++k){
                ಭಿನ್ನ val = get_pixel(label, i, j, k);
                set_pixel(a, i+c, j+r, k, rgb[k] * val);
            }
        }
    }
}

ಶೂನ್ಯ draw_box(image a, ಪೂರ್ಣಾಙ್ಕ x1, ಪೂರ್ಣಾಙ್ಕ y1, ಪೂರ್ಣಾಙ್ಕ x2, ಪೂರ್ಣಾಙ್ಕ y2, ಭಿನ್ನ r, ಭಿನ್ನ g, ಭಿನ್ನ b)
{
    //normalize_image(a);
    ಪೂರ್ಣಾಙ್ಕ i;
    ಯದಿ(x1 < 0) x1 = 0;
    ಯದಿ(x1 >= a.w) x1 = a.w-1;
    ಯದಿ(x2 < 0) x2 = 0;
    ಯದಿ(x2 >= a.w) x2 = a.w-1;

    ಯದಿ(y1 < 0) y1 = 0;
    ಯದಿ(y1 >= a.h) y1 = a.h-1;
    ಯದಿ(y2 < 0) y2 = 0;
    ಯದಿ(y2 >= a.h) y2 = a.h-1;

    ಪ್ರತಿ(i = x1; i <= x2; ++i){
        a.data[i + y1*a.w + 0*a.w*a.h] = r;
        a.data[i + y2*a.w + 0*a.w*a.h] = r;

        a.data[i + y1*a.w + 1*a.w*a.h] = g;
        a.data[i + y2*a.w + 1*a.w*a.h] = g;

        a.data[i + y1*a.w + 2*a.w*a.h] = b;
        a.data[i + y2*a.w + 2*a.w*a.h] = b;
    }
    ಪ್ರತಿ(i = y1; i <= y2; ++i){
        a.data[x1 + i*a.w + 0*a.w*a.h] = r;
        a.data[x2 + i*a.w + 0*a.w*a.h] = r;

        a.data[x1 + i*a.w + 1*a.w*a.h] = g;
        a.data[x2 + i*a.w + 1*a.w*a.h] = g;

        a.data[x1 + i*a.w + 2*a.w*a.h] = b;
        a.data[x2 + i*a.w + 2*a.w*a.h] = b;
    }
}

ಶೂನ್ಯ draw_box_width(image a, ಪೂರ್ಣಾಙ್ಕ x1, ಪೂರ್ಣಾಙ್ಕ y1, ಪೂರ್ಣಾಙ್ಕ x2, ಪೂರ್ಣಾಙ್ಕ y2, ಪೂರ್ಣಾಙ್ಕ w, ಭಿನ್ನ r, ಭಿನ್ನ g, ಭಿನ್ನ b)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < w; ++i){
        draw_box(a, x1+i, y1+i, x2-i, y2-i, r, g, b);
    }
}

ಶೂನ್ಯ draw_bbox(image a, box bbox, ಪೂರ್ಣಾಙ್ಕ w, ಭಿನ್ನ r, ಭಿನ್ನ g, ಭಿನ್ನ b)
{
    ಪೂರ್ಣಾಙ್ಕ ಎಡ  = (bbox.x-bbox.w/2)*a.w;
    ಪೂರ್ಣಾಙ್ಕ ಬಲ = (bbox.x+bbox.w/2)*a.w;
    ಪೂರ್ಣಾಙ್ಕ ತುದಿ   = (bbox.y-bbox.h/2)*a.h;
    ಪೂರ್ಣಾಙ್ಕ bot   = (bbox.y+bbox.h/2)*a.h;

    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < w; ++i){
        draw_box(a, ಎಡ+i, ತುದಿ+i, ಬಲ-i, bot-i, r, g, b);
    }
}

image **load_alphabet()
{
    ಪೂರ್ಣಾಙ್ಕ i, j;
    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ nsize = 8;
    image **alphabets = calloc(nsize, ಪರಿಮಾಣ(image));
    ಪ್ರತಿ(j = 0; j < nsize; ++j){
        alphabets[j] = calloc(128, ಪರಿಮಾಣ(image));
        ಪ್ರತಿ(i = 32; i < 127; ++i){
            ವರ್ಣ buff[256];
            sprintf(buff, "data/labels/%d_%d.png", i, j);
            alphabets[j][i] = load_image_color(buff, 0, 0);
        }
    }
    ಪ್ರತ್ಯಾಗಮ alphabets;
}

ಶೂನ್ಯ draw_detections(image im, detection *dets, ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ, ಭಿನ್ನ thresh, ವರ್ಣ **names, image **alphabet, ಪೂರ್ಣಾಙ್ಕ classes)
{
    ಪೂರ್ಣಾಙ್ಕ i,j;

    ಪ್ರತಿ(i = 0; i < ಅಂಕೆ; ++i){
        ವರ್ಣ labelstr[4096] = {0};
        ಪೂರ್ಣಾಙ್ಕ class = -1;
        ಪ್ರತಿ(j = 0; j < classes; ++j){
            ಯದಿ (dets[i].prob[j] > thresh){
                ಯದಿ (class < 0) {
                    strcat(labelstr, names[j]);
                    class = j;
                } ಅನ್ಯಥಾ {
                    strcat(labelstr, ", ");
                    strcat(labelstr, names[j]);
                }
                printf("%s: %.0f%%\n", names[j], dets[i].prob[j]*100);
            }
        }
        ಯದಿ(class >= 0){
            ಪೂರ್ಣಾಙ್ಕ width = im.h * .006;

            /*
               if(0){
               width = pow(prob, 1./2.)*10+1;
               alphabet = 0;
               }
             */

            //printf("%d %s: %.0f%%\n", i, names[class], prob*100);
            ಪೂರ್ಣಾಙ್ಕ offset = class*123457 % classes;
            ಭಿನ್ನ red = get_color(2,offset,classes);
            ಭಿನ್ನ green = get_color(1,offset,classes);
            ಭಿನ್ನ blue = get_color(0,offset,classes);
            ಭಿನ್ನ rgb[3];

            //width = prob*20+2;

            rgb[0] = red;
            rgb[1] = green;
            rgb[2] = blue;
            box b = dets[i].bbox;
            //printf("%f %f %f %f\n", b.x, b.y, b.w, b.h);

            ಪೂರ್ಣಾಙ್ಕ ಎಡ  = (b.x-b.w/2.)*im.w;
            ಪೂರ್ಣಾಙ್ಕ ಬಲ = (b.x+b.w/2.)*im.w;
            ಪೂರ್ಣಾಙ್ಕ ತುದಿ   = (b.y-b.h/2.)*im.h;
            ಪೂರ್ಣಾಙ್ಕ bot   = (b.y+b.h/2.)*im.h;

            ಯದಿ(ಎಡ < 0) ಎಡ = 0;
            ಯದಿ(ಬಲ > im.w-1) ಬಲ = im.w-1;
            ಯದಿ(ತುದಿ < 0) ತುದಿ = 0;
            ಯದಿ(bot > im.h-1) bot = im.h-1;

            draw_box_width(im, ಎಡ, ತುದಿ, ಬಲ, bot, width, red, green, blue);
            ಯದಿ (alphabet) {
                image label = get_label(alphabet, labelstr, (im.h*.03));
                draw_label(im, ತುದಿ + width, ಎಡ, label, rgb);
                free_image(label);
            }
            ಯದಿ (dets[i].mask){
                image mask = float_to_image(14, 14, 1, dets[i].mask);
                image resized_mask = resize_image(mask, b.w*im.w, b.h*im.h);
                image tmask = threshold_image(resized_mask, .5);
                embed_image(tmask, im, ಎಡ, ತುದಿ);
                free_image(mask);
                free_image(resized_mask);
                free_image(tmask);
            }
        }
    }
}

ಶೂನ್ಯ transpose_image(image im)
{
    ಪ್ರತಿಜ್ಞಾ(im.w == im.h);
    ಪೂರ್ಣಾಙ್ಕ n, m;
    ಪೂರ್ಣಾಙ್ಕ c;
    ಪ್ರತಿ(c = 0; c < im.c; ++c){
        ಪ್ರತಿ(n = 0; n < im.w-1; ++n){
            ಪ್ರತಿ(m = n + 1; m < im.w; ++m){
                ಭಿನ್ನ swap = im.data[m + im.w*(n + im.h*c)];
                im.data[m + im.w*(n + im.h*c)] = im.data[n + im.w*(m + im.h*c)];
                im.data[n + im.w*(m + im.h*c)] = swap;
            }
        }
    }
}

ಶೂನ್ಯ rotate_image_cw(image im, ಪೂರ್ಣಾಙ್ಕ times)
{
    ಪ್ರತಿಜ್ಞಾ(im.w == im.h);
    times = (times + 400) % 4;
    ಪೂರ್ಣಾಙ್ಕ i, x, y, c;
    ಪೂರ್ಣಾಙ್ಕ n = im.w;
    ಪ್ರತಿ(i = 0; i < times; ++i){
        ಪ್ರತಿ(c = 0; c < im.c; ++c){
            ಪ್ರತಿ(x = 0; x < n/2; ++x){
                ಪ್ರತಿ(y = 0; y < (n-1)/2 + 1; ++y){
                    ಭಿನ್ನ temp = im.data[y + im.w*(x + im.h*c)];
                    im.data[y + im.w*(x + im.h*c)] = im.data[n-1-x + im.w*(y + im.h*c)];
                    im.data[n-1-x + im.w*(y + im.h*c)] = im.data[n-1-y + im.w*(n-1-x + im.h*c)];
                    im.data[n-1-y + im.w*(n-1-x + im.h*c)] = im.data[x + im.w*(n-1-y + im.h*c)];
                    im.data[x + im.w*(n-1-y + im.h*c)] = temp;
                }
            }
        }
    }
}

ಶೂನ್ಯ flip_image(image a)
{
    ಪೂರ್ಣಾಙ್ಕ i,j,k;
    ಪ್ರತಿ(k = 0; k < a.c; ++k){
        ಪ್ರತಿ(i = 0; i < a.h; ++i){
            ಪ್ರತಿ(j = 0; j < a.w/2; ++j){
                ಪೂರ್ಣಾಙ್ಕ index = j + a.w*(i + a.h*(k));
                ಪೂರ್ಣಾಙ್ಕ flip = (a.w - j - 1) + a.w*(i + a.h*(k));
                ಭಿನ್ನ swap = a.data[flip];
                a.data[flip] = a.data[index];
                a.data[index] = swap;
            }
        }
    }
}

image image_distance(image a, image b)
{
    ಪೂರ್ಣಾಙ್ಕ i,j;
    image dist = make_image(a.w, a.h, 1);
    ಪ್ರತಿ(i = 0; i < a.c; ++i){
        ಪ್ರತಿ(j = 0; j < a.h*a.w; ++j){
            dist.data[j] += pow(a.data[i*a.h*a.w+j]-b.data[i*a.h*a.w+j],2);
        }
    }
    ಪ್ರತಿ(j = 0; j < a.h*a.w; ++j){
        dist.data[j] = sqrt(dist.data[j]);
    }
    ಪ್ರತ್ಯಾಗಮ dist;
}

ಶೂನ್ಯ ghost_image(image source, image dest, ಪೂರ್ಣಾಙ್ಕ dx, ಪೂರ್ಣಾಙ್ಕ dy)
{
    ಪೂರ್ಣಾಙ್ಕ x,y,k;
    ಭಿನ್ನ max_dist = sqrt((-source.w/2. + .5)*(-source.w/2. + .5));
    ಪ್ರತಿ(k = 0; k < source.c; ++k){
        ಪ್ರತಿ(y = 0; y < source.h; ++y){
            ಪ್ರತಿ(x = 0; x < source.w; ++x){
                ಭಿನ್ನ dist = sqrt((x - source.w/2. + .5)*(x - source.w/2. + .5) + (y - source.h/2. + .5)*(y - source.h/2. + .5));
                ಭಿನ್ನ alpha = (1 - dist/max_dist);
                ಯದಿ(alpha < 0) alpha = 0;
                ಭಿನ್ನ v1 = get_pixel(source, x,y,k);
                ಭಿನ್ನ v2 = get_pixel(dest, dx+x,dy+y,k);
                ಭಿನ್ನ val = alpha*v1 + (1-alpha)*v2;
                set_pixel(dest, dx+x, dy+y, k, val);
            }
        }
    }
}

ಶೂನ್ಯ blocky_image(image im, ಪೂರ್ಣಾಙ್ಕ s)
{
    ಪೂರ್ಣಾಙ್ಕ i,j,k;
    ಪ್ರತಿ(k = 0; k < im.c; ++k){
        ಪ್ರತಿ(j = 0; j < im.h; ++j){
            ಪ್ರತಿ(i = 0; i < im.w; ++i){
                im.data[i + im.w*(j + im.h*k)] = im.data[i/s*s + im.w*(j/s*s + im.h*k)];
            }
        }
    }
}

ಶೂನ್ಯ censor_image(image im, ಪೂರ್ಣಾಙ್ಕ dx, ಪೂರ್ಣಾಙ್ಕ dy, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h)
{
    ಪೂರ್ಣಾಙ್ಕ i,j,k;
    ಪೂರ್ಣಾಙ್ಕ s = 32;
    ಯದಿ(dx < 0) dx = 0;
    ಯದಿ(dy < 0) dy = 0;

    ಪ್ರತಿ(k = 0; k < im.c; ++k){
        ಪ್ರತಿ(j = dy; j < dy + h && j < im.h; ++j){
            ಪ್ರತಿ(i = dx; i < dx + w && i < im.w; ++i){
                im.data[i + im.w*(j + im.h*k)] = im.data[i/s*s + im.w*(j/s*s + im.h*k)];
                //im.data[i + j*im.w + k*im.w*im.h] = 0;
            }
        }
    }
}

ಶೂನ್ಯ embed_image(image source, image dest, ಪೂರ್ಣಾಙ್ಕ dx, ಪೂರ್ಣಾಙ್ಕ dy)
{
    ಪೂರ್ಣಾಙ್ಕ x,y,k;
    ಪ್ರತಿ(k = 0; k < source.c; ++k){
        ಪ್ರತಿ(y = 0; y < source.h; ++y){
            ಪ್ರತಿ(x = 0; x < source.w; ++x){
                ಭಿನ್ನ val = get_pixel(source, x,y,k);
                set_pixel(dest, dx+x, dy+y, k, val);
            }
        }
    }
}

image collapse_image_layers(image source, ಪೂರ್ಣಾಙ್ಕ border)
{
    ಪೂರ್ಣಾಙ್ಕ h = source.h;
    h = (h+border)*source.c - border;
    image dest = make_image(source.w, h, 1);
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < source.c; ++i){
        image layer = get_image_layer(source, i);
        ಪೂರ್ಣಾಙ್ಕ h_offset = i*(source.h+border);
        embed_image(layer, dest, 0, h_offset);
        free_image(layer);
    }
    ಪ್ರತ್ಯಾಗಮ dest;
}

ಶೂನ್ಯ constrain_image(image im)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < im.w*im.h*im.c; ++i){
        ಯದಿ(im.data[i] < 0) im.data[i] = 0;
        ಯದಿ(im.data[i] > 1) im.data[i] = 1;
    }
}

ಶೂನ್ಯ normalize_image(image p)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಭಿನ್ನ min = 9999999;
    ಭಿನ್ನ max = -999999;

    ಪ್ರತಿ(i = 0; i < p.h*p.w*p.c; ++i){
        ಭಿನ್ನ v = p.data[i];
        ಯದಿ(v < min) min = v;
        ಯದಿ(v > max) max = v;
    }
    ಯದಿ(max - min < .000000001){
        min = 0;
        max = 1;
    }
    ಪ್ರತಿ(i = 0; i < p.c*p.w*p.h; ++i){
        p.data[i] = (p.data[i] - min)/(max-min);
    }
}

ಶೂನ್ಯ normalize_image2(image p)
{
    ಭಿನ್ನ *min = calloc(p.c, ಪರಿಮಾಣ(ಭಿನ್ನ));
    ಭಿನ್ನ *max = calloc(p.c, ಪರಿಮಾಣ(ಭಿನ್ನ));
    ಪೂರ್ಣಾಙ್ಕ i,j;
    ಪ್ರತಿ(i = 0; i < p.c; ++i) min[i] = max[i] = p.data[i*p.h*p.w];

    ಪ್ರತಿ(j = 0; j < p.c; ++j){
        ಪ್ರತಿ(i = 0; i < p.h*p.w; ++i){
            ಭಿನ್ನ v = p.data[i+j*p.h*p.w];
            ಯದಿ(v < min[j]) min[j] = v;
            ಯದಿ(v > max[j]) max[j] = v;
        }
    }
    ಪ್ರತಿ(i = 0; i < p.c; ++i){
        ಯದಿ(max[i] - min[i] < .000000001){
            min[i] = 0;
            max[i] = 1;
        }
    }
    ಪ್ರತಿ(j = 0; j < p.c; ++j){
        ಪ್ರತಿ(i = 0; i < p.w*p.h; ++i){
            p.data[i+j*p.h*p.w] = (p.data[i+j*p.h*p.w] - min[j])/(max[j]-min[j]);
        }
    }
    ಮುಕ್ತ(min);
    ಮುಕ್ತ(max);
}

ಶೂನ್ಯ copy_image_into(image src, image dest)
{
    memcpy(dest.data, src.data, src.h*src.w*src.c*ಪರಿಮಾಣ(ಭಿನ್ನ));
}

image copy_image(image p)
{
    image copy = p;
    copy.data = calloc(p.h*p.w*p.c, ಪರಿಮಾಣ(ಭಿನ್ನ));
    memcpy(copy.data, p.data, p.h*p.w*p.c*ಪರಿಮಾಣ(ಭಿನ್ನ));
    ಪ್ರತ್ಯಾಗಮ copy;
}

ಶೂನ್ಯ rgbgr_image(image im)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < im.w*im.h; ++i){
        ಭಿನ್ನ swap = im.data[i];
        im.data[i] = im.data[i+im.w*im.h*2];
        im.data[i+im.w*im.h*2] = swap;
    }
}

ಪೂರ್ಣಾಙ್ಕ show_image(image p, ಧ್ರುವ ವರ್ಣ *name, ಪೂರ್ಣಾಙ್ಕ ms)
{
#ifdef OPENCV
    ಪೂರ್ಣಾಙ್ಕ c = show_image_cv(p, name, ms);
    ಪ್ರತ್ಯಾಗಮ c;
#ಅನ್ಯಥಾ
    fprintf(stderr, "Not compiled with OpenCV, saving to %s.png instead\n", name);
    save_image(p, name);
    ಪ್ರತ್ಯಾಗಮ -1;
#endif
}

ಶೂನ್ಯ save_image_options(image im, ಧ್ರುವ ವರ್ಣ *name, IMTYPE f, ಪೂರ್ಣಾಙ್ಕ quality)
{
    ವರ್ಣ buff[256];
    //sprintf(buff, "%s (%d)", name, windows);
    ಯದಿ(f == PNG)       sprintf(buff, "%s.png", name);
    ಅನ್ಯಥಾ ಯದಿ (f == BMP) sprintf(buff, "%s.bmp", name);
    ಅನ್ಯಥಾ ಯದಿ (f == TGA) sprintf(buff, "%s.tga", name);
    ಅನ್ಯಥಾ ಯದಿ (f == JPG) sprintf(buff, "%s.jpg", name);
    ಅನ್ಯಥಾ               sprintf(buff, "%s.png", name);
    unsigned ವರ್ಣ *data = calloc(im.w*im.h*im.c, ಪರಿಮಾಣ(ವರ್ಣ));
    ಪೂರ್ಣಾಙ್ಕ i,k;
    ಪ್ರತಿ(k = 0; k < im.c; ++k){
        ಪ್ರತಿ(i = 0; i < im.w*im.h; ++i){
            data[i*im.c+k] = (unsigned ವರ್ಣ) (255*im.data[i + k*im.w*im.h]);
        }
    }
    ಪೂರ್ಣಾಙ್ಕ success = 0;
    ಯದಿ(f == PNG)       success = stbi_write_png(buff, im.w, im.h, im.c, data, im.w*im.c);
    ಅನ್ಯಥಾ ಯದಿ (f == BMP) success = stbi_write_bmp(buff, im.w, im.h, im.c, data);
    ಅನ್ಯಥಾ ಯದಿ (f == TGA) success = stbi_write_tga(buff, im.w, im.h, im.c, data);
    ಅನ್ಯಥಾ ಯದಿ (f == JPG) success = stbi_write_jpg(buff, im.w, im.h, im.c, data, quality);
    ಮುಕ್ತ(data);
    ಯದಿ(!success) fprintf(stderr, "Failed to write image %s\n", buff);
}

ಶೂನ್ಯ save_image(image im, ಧ್ರುವ ವರ್ಣ *name)
{
    save_image_options(im, name, JPG, 80);
}

ಶೂನ್ಯ show_image_layers(image p, ವರ್ಣ *name)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ವರ್ಣ buff[256];
    ಪ್ರತಿ(i = 0; i < p.c; ++i){
        sprintf(buff, "%s - Layer %d", name, i);
        image layer = get_image_layer(p, i);
        show_image(layer, buff, 1);
        free_image(layer);
    }
}

ಶೂನ್ಯ show_image_collapsed(image p, ವರ್ಣ *name)
{
    image c = collapse_image_layers(p, 1);
    show_image(c, name, 1);
    free_image(c);
}

image make_empty_image(ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h, ಪೂರ್ಣಾಙ್ಕ c)
{
    image out;
    out.data = 0;
    out.h = h;
    out.w = w;
    out.c = c;
    ಪ್ರತ್ಯಾಗಮ out;
}

image make_image(ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h, ಪೂರ್ಣಾಙ್ಕ c)
{
    image out = make_empty_image(w,h,c);
    out.data = calloc(h*w*c, ಪರಿಮಾಣ(ಭಿನ್ನ));
    ಪ್ರತ್ಯಾಗಮ out;
}

image make_random_image(ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h, ಪೂರ್ಣಾಙ್ಕ c)
{
    image out = make_empty_image(w,h,c);
    out.data = calloc(h*w*c, ಪರಿಮಾಣ(ಭಿನ್ನ));
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < w*h*c; ++i){
        out.data[i] = (rand_normal() * .25) + .5;
    }
    ಪ್ರತ್ಯಾಗಮ out;
}

image float_to_image(ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h, ಪೂರ್ಣಾಙ್ಕ c, ಭಿನ್ನ *data)
{
    image out = make_empty_image(w,h,c);
    out.data = data;
    ಪ್ರತ್ಯಾಗಮ out;
}

ಶೂನ್ಯ place_image(image im, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h, ಪೂರ್ಣಾಙ್ಕ dx, ಪೂರ್ಣಾಙ್ಕ dy, image canvas)
{
    ಪೂರ್ಣಾಙ್ಕ x, y, c;
    ಪ್ರತಿ(c = 0; c < im.c; ++c){
        ಪ್ರತಿ(y = 0; y < h; ++y){
            ಪ್ರತಿ(x = 0; x < w; ++x){
                ಭಿನ್ನ rx = ((ಭಿನ್ನ)x / w) * im.w;
                ಭಿನ್ನ ry = ((ಭಿನ್ನ)y / h) * im.h;
                ಭಿನ್ನ val = bilinear_interpolate(im, rx, ry, c);
                set_pixel(canvas, x + dx, y + dy, c, val);
            }
        }
    }
}

image center_crop_image(image im, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h)
{
    ಪೂರ್ಣಾಙ್ಕ m = (im.w < im.h) ? im.w : im.h;   
    image c = crop_image(im, (im.w - m) / 2, (im.h - m)/2, m, m);
    image r = resize_image(c, w, h);
    free_image(c);
    ಪ್ರತ್ಯಾಗಮ r;
}

image rotate_crop_image(image im, ಭಿನ್ನ rad, ಭಿನ್ನ s, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h, ಭಿನ್ನ dx, ಭಿನ್ನ dy, ಭಿನ್ನ aspect)
{
    ಪೂರ್ಣಾಙ್ಕ x, y, c;
    ಭಿನ್ನ cx = im.w/2.;
    ಭಿನ್ನ cy = im.h/2.;
    image rot = make_image(w, h, im.c);
    ಪ್ರತಿ(c = 0; c < im.c; ++c){
        ಪ್ರತಿ(y = 0; y < h; ++y){
            ಪ್ರತಿ(x = 0; x < w; ++x){
                ಭಿನ್ನ rx = cos(rad)*((x - w/2.)/s*aspect + dx/s*aspect) - sin(rad)*((y - h/2.)/s + dy/s) + cx;
                ಭಿನ್ನ ry = sin(rad)*((x - w/2.)/s*aspect + dx/s*aspect) + cos(rad)*((y - h/2.)/s + dy/s) + cy;
                ಭಿನ್ನ val = bilinear_interpolate(im, rx, ry, c);
                set_pixel(rot, x, y, c, val);
            }
        }
    }
    ಪ್ರತ್ಯಾಗಮ rot;
}

image rotate_image(image im, ಭಿನ್ನ rad)
{
    ಪೂರ್ಣಾಙ್ಕ x, y, c;
    ಭಿನ್ನ cx = im.w/2.;
    ಭಿನ್ನ cy = im.h/2.;
    image rot = make_image(im.w, im.h, im.c);
    ಪ್ರತಿ(c = 0; c < im.c; ++c){
        ಪ್ರತಿ(y = 0; y < im.h; ++y){
            ಪ್ರತಿ(x = 0; x < im.w; ++x){
                ಭಿನ್ನ rx = cos(rad)*(x-cx) - sin(rad)*(y-cy) + cx;
                ಭಿನ್ನ ry = sin(rad)*(x-cx) + cos(rad)*(y-cy) + cy;
                ಭಿನ್ನ val = bilinear_interpolate(im, rx, ry, c);
                set_pixel(rot, x, y, c, val);
            }
        }
    }
    ಪ್ರತ್ಯಾಗಮ rot;
}

ಶೂನ್ಯ fill_image(image m, ಭಿನ್ನ s)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < m.h*m.w*m.c; ++i) m.data[i] = s;
}

ಶೂನ್ಯ translate_image(image m, ಭಿನ್ನ s)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < m.h*m.w*m.c; ++i) m.data[i] += s;
}

ಶೂನ್ಯ scale_image(image m, ಭಿನ್ನ s)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < m.h*m.w*m.c; ++i) m.data[i] *= s;
}

image crop_image(image im, ಪೂರ್ಣಾಙ್ಕ dx, ಪೂರ್ಣಾಙ್ಕ dy, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h)
{
    image cropped = make_image(w, h, im.c);
    ಪೂರ್ಣಾಙ್ಕ i, j, k;
    ಪ್ರತಿ(k = 0; k < im.c; ++k){
        ಪ್ರತಿ(j = 0; j < h; ++j){
            ಪ್ರತಿ(i = 0; i < w; ++i){
                ಪೂರ್ಣಾಙ್ಕ r = j + dy;
                ಪೂರ್ಣಾಙ್ಕ c = i + dx;
                ಭಿನ್ನ val = 0;
                r = constrain_int(r, 0, im.h-1);
                c = constrain_int(c, 0, im.w-1);
                val = get_pixel(im, c, r, k);
                set_pixel(cropped, i, j, k, val);
            }
        }
    }
    ಪ್ರತ್ಯಾಗಮ cropped;
}

ಪೂರ್ಣಾಙ್ಕ best_3d_shift_r(image a, image b, ಪೂರ್ಣಾಙ್ಕ min, ಪೂರ್ಣಾಙ್ಕ max)
{
    ಯದಿ(min == max) ಪ್ರತ್ಯಾಗಮ min;
    ಪೂರ್ಣಾಙ್ಕ mid = floor((min + max) / 2.);
    image c1 = crop_image(b, 0, mid, b.w, b.h);
    image c2 = crop_image(b, 0, mid+1, b.w, b.h);
    ಭಿನ್ನ d1 = dist_array(c1.data, a.data, a.w*a.h*a.c, 10);
    ಭಿನ್ನ d2 = dist_array(c2.data, a.data, a.w*a.h*a.c, 10);
    free_image(c1);
    free_image(c2);
    ಯದಿ(d1 < d2) ಪ್ರತ್ಯಾಗಮ best_3d_shift_r(a, b, min, mid);
    ಅನ್ಯಥಾ ಪ್ರತ್ಯಾಗಮ best_3d_shift_r(a, b, mid+1, max);
}

ಪೂರ್ಣಾಙ್ಕ best_3d_shift(image a, image b, ಪೂರ್ಣಾಙ್ಕ min, ಪೂರ್ಣಾಙ್ಕ max)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪೂರ್ಣಾಙ್ಕ best = 0;
    ಭಿನ್ನ best_distance = FLT_MAX;
    ಪ್ರತಿ(i = min; i <= max; i += 2){
        image c = crop_image(b, 0, i, b.w, b.h);
        ಭಿನ್ನ d = dist_array(c.data, a.data, a.w*a.h*a.c, 100);
        ಯದಿ(d < best_distance){
            best_distance = d;
            best = i;
        }
        printf("%d %f\n", i, d);
        free_image(c);
    }
    ಪ್ರತ್ಯಾಗಮ best;
}

ಶೂನ್ಯ composite_3d(ವರ್ಣ *f1, ವರ್ಣ *f2, ವರ್ಣ *out, ಪೂರ್ಣಾಙ್ಕ delta)
{
    ಯದಿ(!out) out = "out";
    image a = load_image(f1, 0,0,0);
    image b = load_image(f2, 0,0,0);
    ಪೂರ್ಣಾಙ್ಕ shift = best_3d_shift_r(a, b, -a.h/100, a.h/100);

    image c1 = crop_image(b, 10, shift, b.w, b.h);
    ಭಿನ್ನ d1 = dist_array(c1.data, a.data, a.w*a.h*a.c, 100);
    image c2 = crop_image(b, -10, shift, b.w, b.h);
    ಭಿನ್ನ d2 = dist_array(c2.data, a.data, a.w*a.h*a.c, 100);

    ಯದಿ(d2 < d1 && 0){
        image swap = a;
        a = b;
        b = swap;
        shift = -shift;
        printf("swapped, %d\n", shift);
    }
    ಅನ್ಯಥಾ{
        printf("%d\n", shift);
    }

    image c = crop_image(b, delta, shift, a.w, a.h);
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < c.w*c.h; ++i){
        c.data[i] = a.data[i];
    }
    save_image(c, out);
}

ಶೂನ್ಯ letterbox_image_into(image im, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h, image boxed)
{
    ಪೂರ್ಣಾಙ್ಕ new_w = im.w;
    ಪೂರ್ಣಾಙ್ಕ new_h = im.h;
    ಯದಿ (((ಭಿನ್ನ)w/im.w) < ((ಭಿನ್ನ)h/im.h)) {
        new_w = w;
        new_h = (im.h * w)/im.w;
    } ಅನ್ಯಥಾ {
        new_h = h;
        new_w = (im.w * h)/im.h;
    }
    image resized = resize_image(im, new_w, new_h);
    embed_image(resized, boxed, (w-new_w)/2, (h-new_h)/2); 
    free_image(resized);
}

image letterbox_image(image im, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h)
{
    ಪೂರ್ಣಾಙ್ಕ new_w = im.w;
    ಪೂರ್ಣಾಙ್ಕ new_h = im.h;
    ಯದಿ (((ಭಿನ್ನ)w/im.w) < ((ಭಿನ್ನ)h/im.h)) {
        new_w = w;
        new_h = (im.h * w)/im.w;
    } ಅನ್ಯಥಾ {
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
    ಪ್ರತ್ಯಾಗಮ boxed;
}

image resize_max(image im, ಪೂರ್ಣಾಙ್ಕ max)
{
    ಪೂರ್ಣಾಙ್ಕ w = im.w;
    ಪೂರ್ಣಾಙ್ಕ h = im.h;
    ಯದಿ(w > h){
        h = (h * max) / w;
        w = max;
    } ಅನ್ಯಥಾ {
        w = (w * max) / h;
        h = max;
    }
    ಯದಿ(w == im.w && h == im.h) ಪ್ರತ್ಯಾಗಮ im;
    image resized = resize_image(im, w, h);
    ಪ್ರತ್ಯಾಗಮ resized;
}

image resize_min(image im, ಪೂರ್ಣಾಙ್ಕ min)
{
    ಪೂರ್ಣಾಙ್ಕ w = im.w;
    ಪೂರ್ಣಾಙ್ಕ h = im.h;
    ಯದಿ(w < h){
        h = (h * min) / w;
        w = min;
    } ಅನ್ಯಥಾ {
        w = (w * min) / h;
        h = min;
    }
    ಯದಿ(w == im.w && h == im.h) ಪ್ರತ್ಯಾಗಮ im;
    image resized = resize_image(im, w, h);
    ಪ್ರತ್ಯಾಗಮ resized;
}

image random_crop_image(image im, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h)
{
    ಪೂರ್ಣಾಙ್ಕ dx = rand_int(0, im.w - w);
    ಪೂರ್ಣಾಙ್ಕ dy = rand_int(0, im.h - h);
    image crop = crop_image(im, dx, dy, w, h);
    ಪ್ರತ್ಯಾಗಮ crop;
}

augment_args random_augment_args(image im, ಭಿನ್ನ angle, ಭಿನ್ನ aspect, ಪೂರ್ಣಾಙ್ಕ ಕೆಳಗಿನ, ಪೂರ್ಣಾಙ್ಕ ಮೇಲಿನ, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h)
{
    augment_args a = {0};
    aspect = rand_scale(aspect);
    ಪೂರ್ಣಾಙ್ಕ r = rand_int(ಕೆಳಗಿನ, ಮೇಲಿನ);
    ಪೂರ್ಣಾಙ್ಕ min = (im.h < im.w*aspect) ? im.h : im.w*aspect;
    ಭಿನ್ನ scale = (ಭಿನ್ನ)r / min;

    ಭಿನ್ನ rad = rand_uniform(-angle, angle) * TWO_PI / 360.;

    ಭಿನ್ನ dx = (im.w*scale/aspect - w) / 2.;
    ಭಿನ್ನ dy = (im.h*scale - w) / 2.;
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
    ಪ್ರತ್ಯಾಗಮ a;
}

image random_augment_image(image im, ಭಿನ್ನ angle, ಭಿನ್ನ aspect, ಪೂರ್ಣಾಙ್ಕ ಕೆಳಗಿನ, ಪೂರ್ಣಾಙ್ಕ ಮೇಲಿನ, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h)
{
    augment_args a = random_augment_args(im, angle, aspect, ಕೆಳಗಿನ, ಮೇಲಿನ, w, h);
    image crop = rotate_crop_image(im, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);
    ಪ್ರತ್ಯಾಗಮ crop;
}

ಭಿನ್ನ three_way_max(ಭಿನ್ನ a, ಭಿನ್ನ b, ಭಿನ್ನ c)
{
    ಪ್ರತ್ಯಾಗಮ (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

ಭಿನ್ನ three_way_min(ಭಿನ್ನ a, ಭಿನ್ನ b, ಭಿನ್ನ c)
{
    ಪ್ರತ್ಯಾಗಮ (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

ಶೂನ್ಯ yuv_to_rgb(image im)
{
    ಪ್ರತಿಜ್ಞಾ(im.c == 3);
    ಪೂರ್ಣಾಙ್ಕ i, j;
    ಭಿನ್ನ r, g, b;
    ಭಿನ್ನ y, u, v;
    ಪ್ರತಿ(j = 0; j < im.h; ++j){
        ಪ್ರತಿ(i = 0; i < im.w; ++i){
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

ಶೂನ್ಯ rgb_to_yuv(image im)
{
    ಪ್ರತಿಜ್ಞಾ(im.c == 3);
    ಪೂರ್ಣಾಙ್ಕ i, j;
    ಭಿನ್ನ r, g, b;
    ಭಿನ್ನ y, u, v;
    ಪ್ರತಿ(j = 0; j < im.h; ++j){
        ಪ್ರತಿ(i = 0; i < im.w; ++i){
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
ಶೂನ್ಯ rgb_to_hsv(image im)
{
    ಪ್ರತಿಜ್ಞಾ(im.c == 3);
    ಪೂರ್ಣಾಙ್ಕ i, j;
    ಭಿನ್ನ r, g, b;
    ಭಿನ್ನ h, s, v;
    ಪ್ರತಿ(j = 0; j < im.h; ++j){
        ಪ್ರತಿ(i = 0; i < im.w; ++i){
            r = get_pixel(im, i , j, 0);
            g = get_pixel(im, i , j, 1);
            b = get_pixel(im, i , j, 2);
            ಭಿನ್ನ max = three_way_max(r,g,b);
            ಭಿನ್ನ min = three_way_min(r,g,b);
            ಭಿನ್ನ delta = max - min;
            v = max;
            ಯದಿ(max == 0){
                s = 0;
                h = 0;
            }ಅನ್ಯಥಾ{
                s = delta/max;
                ಯದಿ(r == max){
                    h = (g - b) / delta;
                } ಅನ್ಯಥಾ ಯದಿ (g == max) {
                    h = 2 + (b - r) / delta;
                } ಅನ್ಯಥಾ {
                    h = 4 + (r - g) / delta;
                }
                ಯದಿ (h < 0) h += 6;
                h = h/6.;
            }
            set_pixel(im, i, j, 0, h);
            set_pixel(im, i, j, 1, s);
            set_pixel(im, i, j, 2, v);
        }
    }
}

ಶೂನ್ಯ hsv_to_rgb(image im)
{
    ಪ್ರತಿಜ್ಞಾ(im.c == 3);
    ಪೂರ್ಣಾಙ್ಕ i, j;
    ಭಿನ್ನ r, g, b;
    ಭಿನ್ನ h, s, v;
    ಭಿನ್ನ f, p, q, t;
    ಪ್ರತಿ(j = 0; j < im.h; ++j){
        ಪ್ರತಿ(i = 0; i < im.w; ++i){
            h = 6 * get_pixel(im, i , j, 0);
            s = get_pixel(im, i , j, 1);
            v = get_pixel(im, i , j, 2);
            ಯದಿ (s == 0) {
                r = g = b = v;
            } ಅನ್ಯಥಾ {
                ಪೂರ್ಣಾಙ್ಕ index = floor(h);
                f = h - index;
                p = v*(1-s);
                q = v*(1-s*f);
                t = v*(1-s*(1-f));
                ಯದಿ(index == 0){
                    r = v; g = t; b = p;
                } ಅನ್ಯಥಾ ಯದಿ(index == 1){
                    r = q; g = v; b = p;
                } ಅನ್ಯಥಾ ಯದಿ(index == 2){
                    r = p; g = v; b = t;
                } ಅನ್ಯಥಾ ಯದಿ(index == 3){
                    r = p; g = q; b = v;
                } ಅನ್ಯಥಾ ಯದಿ(index == 4){
                    r = t; g = p; b = v;
                } ಅನ್ಯಥಾ {
                    r = v; g = p; b = q;
                }
            }
            set_pixel(im, i, j, 0, r);
            set_pixel(im, i, j, 1, g);
            set_pixel(im, i, j, 2, b);
        }
    }
}

ಶೂನ್ಯ grayscale_image_3c(image im)
{
    ಪ್ರತಿಜ್ಞಾ(im.c == 3);
    ಪೂರ್ಣಾಙ್ಕ i, j, k;
    ಭಿನ್ನ scale[] = {0.299, 0.587, 0.114};
    ಪ್ರತಿ(j = 0; j < im.h; ++j){
        ಪ್ರತಿ(i = 0; i < im.w; ++i){
            ಭಿನ್ನ val = 0;
            ಪ್ರತಿ(k = 0; k < 3; ++k){
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
    ಪ್ರತಿಜ್ಞಾ(im.c == 3);
    ಪೂರ್ಣಾಙ್ಕ i, j, k;
    image gray = make_image(im.w, im.h, 1);
    ಭಿನ್ನ scale[] = {0.299, 0.587, 0.114};
    ಪ್ರತಿ(k = 0; k < im.c; ++k){
        ಪ್ರತಿ(j = 0; j < im.h; ++j){
            ಪ್ರತಿ(i = 0; i < im.w; ++i){
                gray.data[i+im.w*j] += scale[k]*get_pixel(im, i, j, k);
            }
        }
    }
    ಪ್ರತ್ಯಾಗಮ gray;
}

image threshold_image(image im, ಭಿನ್ನ thresh)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    image t = make_image(im.w, im.h, im.c);
    ಪ್ರತಿ(i = 0; i < im.w*im.h*im.c; ++i){
        t.data[i] = im.data[i]>thresh ? 1 : 0;
    }
    ಪ್ರತ್ಯಾಗಮ t;
}

image blend_image(image fore, image back, ಭಿನ್ನ alpha)
{
    ಪ್ರತಿಜ್ಞಾ(fore.w == back.w && fore.h == back.h && fore.c == back.c);
    image blend = make_image(fore.w, fore.h, fore.c);
    ಪೂರ್ಣಾಙ್ಕ i, j, k;
    ಪ್ರತಿ(k = 0; k < fore.c; ++k){
        ಪ್ರತಿ(j = 0; j < fore.h; ++j){
            ಪ್ರತಿ(i = 0; i < fore.w; ++i){
                ಭಿನ್ನ val = alpha * get_pixel(fore, i, j, k) + 
                    (1 - alpha)* get_pixel(back, i, j, k);
                set_pixel(blend, i, j, k, val);
            }
        }
    }
    ಪ್ರತ್ಯಾಗಮ blend;
}

ಶೂನ್ಯ scale_image_channel(image im, ಪೂರ್ಣಾಙ್ಕ c, ಭಿನ್ನ v)
{
    ಪೂರ್ಣಾಙ್ಕ i, j;
    ಪ್ರತಿ(j = 0; j < im.h; ++j){
        ಪ್ರತಿ(i = 0; i < im.w; ++i){
            ಭಿನ್ನ pix = get_pixel(im, i, j, c);
            pix = pix*v;
            set_pixel(im, i, j, c, pix);
        }
    }
}

ಶೂನ್ಯ translate_image_channel(image im, ಪೂರ್ಣಾಙ್ಕ c, ಭಿನ್ನ v)
{
    ಪೂರ್ಣಾಙ್ಕ i, j;
    ಪ್ರತಿ(j = 0; j < im.h; ++j){
        ಪ್ರತಿ(i = 0; i < im.w; ++i){
            ಭಿನ್ನ pix = get_pixel(im, i, j, c);
            pix = pix+v;
            set_pixel(im, i, j, c, pix);
        }
    }
}

image binarize_image(image im)
{
    image c = copy_image(im);
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < im.w * im.h * im.c; ++i){
        ಯದಿ(c.data[i] > .5) c.data[i] = 1;
        ಅನ್ಯಥಾ c.data[i] = 0;
    }
    ಪ್ರತ್ಯಾಗಮ c;
}

ಶೂನ್ಯ saturate_image(image im, ಭಿನ್ನ sat)
{
    rgb_to_hsv(im);
    scale_image_channel(im, 1, sat);
    hsv_to_rgb(im);
    constrain_image(im);
}

ಶೂನ್ಯ hue_image(image im, ಭಿನ್ನ hue)
{
    rgb_to_hsv(im);
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < im.w*im.h; ++i){
        im.data[i] = im.data[i] + hue;
        ಯದಿ (im.data[i] > 1) im.data[i] -= 1;
        ಯದಿ (im.data[i] < 0) im.data[i] += 1;
    }
    hsv_to_rgb(im);
    constrain_image(im);
}

ಶೂನ್ಯ exposure_image(image im, ಭಿನ್ನ sat)
{
    rgb_to_hsv(im);
    scale_image_channel(im, 2, sat);
    hsv_to_rgb(im);
    constrain_image(im);
}

ಶೂನ್ಯ distort_image(image im, ಭಿನ್ನ hue, ಭಿನ್ನ sat, ಭಿನ್ನ val)
{
    rgb_to_hsv(im);
    scale_image_channel(im, 1, sat);
    scale_image_channel(im, 2, val);
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < im.w*im.h; ++i){
        im.data[i] = im.data[i] + hue;
        ಯದಿ (im.data[i] > 1) im.data[i] -= 1;
        ಯದಿ (im.data[i] < 0) im.data[i] += 1;
    }
    hsv_to_rgb(im);
    constrain_image(im);
}

ಶೂನ್ಯ random_distort_image(image im, ಭಿನ್ನ hue, ಭಿನ್ನ saturation, ಭಿನ್ನ exposure)
{
    ಭಿನ್ನ dhue = rand_uniform(-hue, hue);
    ಭಿನ್ನ dsat = rand_scale(saturation);
    ಭಿನ್ನ dexp = rand_scale(exposure);
    distort_image(im, dhue, dsat, dexp);
}

ಶೂನ್ಯ saturate_exposure_image(image im, ಭಿನ್ನ sat, ಭಿನ್ನ exposure)
{
    rgb_to_hsv(im);
    scale_image_channel(im, 1, sat);
    scale_image_channel(im, 2, exposure);
    hsv_to_rgb(im);
    constrain_image(im);
}

image resize_image(image im, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h)
{
    image resized = make_image(w, h, im.c);   
    image part = make_image(w, im.h, im.c);
    ಪೂರ್ಣಾಙ್ಕ r, c, k;
    ಭಿನ್ನ w_scale = (ಭಿನ್ನ)(im.w - 1) / (w - 1);
    ಭಿನ್ನ h_scale = (ಭಿನ್ನ)(im.h - 1) / (h - 1);
    ಪ್ರತಿ(k = 0; k < im.c; ++k){
        ಪ್ರತಿ(r = 0; r < im.h; ++r){
            ಪ್ರತಿ(c = 0; c < w; ++c){
                ಭಿನ್ನ val = 0;
                ಯದಿ(c == w-1 || im.w == 1){
                    val = get_pixel(im, im.w-1, r, k);
                } ಅನ್ಯಥಾ {
                    ಭಿನ್ನ sx = c*w_scale;
                    ಪೂರ್ಣಾಙ್ಕ ix = (ಪೂರ್ಣಾಙ್ಕ) sx;
                    ಭಿನ್ನ dx = sx - ix;
                    val = (1 - dx) * get_pixel(im, ix, r, k) + dx * get_pixel(im, ix+1, r, k);
                }
                set_pixel(part, c, r, k, val);
            }
        }
    }
    ಪ್ರತಿ(k = 0; k < im.c; ++k){
        ಪ್ರತಿ(r = 0; r < h; ++r){
            ಭಿನ್ನ sy = r*h_scale;
            ಪೂರ್ಣಾಙ್ಕ iy = (ಪೂರ್ಣಾಙ್ಕ) sy;
            ಭಿನ್ನ dy = sy - iy;
            ಪ್ರತಿ(c = 0; c < w; ++c){
                ಭಿನ್ನ val = (1-dy) * get_pixel(part, c, iy, k);
                set_pixel(resized, c, r, k, val);
            }
            ಯದಿ(r == h-1 || im.h == 1) ಅನುವರ್ತ;
            ಪ್ರತಿ(c = 0; c < w; ++c){
                ಭಿನ್ನ val = dy * get_pixel(part, c, iy+1, k);
                add_pixel(resized, c, r, k, val);
            }
        }
    }

    free_image(part);
    ಪ್ರತ್ಯಾಗಮ resized;
}


ಶೂನ್ಯ test_resize(ವರ್ಣ *filename)
{
    image im = load_image(filename, 0,0, 3);
    ಭಿನ್ನ mag = mag_array(im.data, im.w*im.h*im.c);
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
    ಯಾವತ್(1){
        image aug = random_augment_image(im, 0, .75, 320, 448, 320, 320);
        show_image(aug, "aug", 1);
        free_image(aug);


        ಭಿನ್ನ exposure = 1.15;
        ಭಿನ್ನ saturation = 1.15;
        ಭಿನ್ನ hue = .05;

        image c = copy_image(im);

        ಭಿನ್ನ dexp = rand_scale(exposure);
        ಭಿನ್ನ dsat = rand_scale(saturation);
        ಭಿನ್ನ dhue = rand_uniform(-hue, hue);

        distort_image(c, dhue, dsat, dexp);
        show_image(c, "rand", 1);
        printf("%f %f %f\n", dhue, dsat, dexp);
        free_image(c);
    }
#endif
}


image load_image_stb(ವರ್ಣ *filename, ಪೂರ್ಣಾಙ್ಕ channels)
{
    ಪೂರ್ಣಾಙ್ಕ w, h, c;
    unsigned ವರ್ಣ *data = stbi_load(filename, &w, &h, &c, channels);
    ಯದಿ (!data) {
        fprintf(stderr, "Cannot load image \"%s\"\nSTB Reason: %s\n", filename, stbi_failure_reason());
        exit(0);
    }
    ಯದಿ(channels) c = channels;
    ಪೂರ್ಣಾಙ್ಕ i,j,k;
    image im = make_image(w, h, c);
    ಪ್ರತಿ(k = 0; k < c; ++k){
        ಪ್ರತಿ(j = 0; j < h; ++j){
            ಪ್ರತಿ(i = 0; i < w; ++i){
                ಪೂರ್ಣಾಙ್ಕ dst_index = i + w*j + w*h*k;
                ಪೂರ್ಣಾಙ್ಕ src_index = k + c*i + c*w*j;
                im.data[dst_index] = (ಭಿನ್ನ)data[src_index]/255.;
            }
        }
    }
    ಮುಕ್ತ(data);
    ಪ್ರತ್ಯಾಗಮ im;
}

image load_image(ವರ್ಣ *filename, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h, ಪೂರ್ಣಾಙ್ಕ c)
{
#ifdef OPENCV
    image out = load_image_cv(filename, c);
#ಅನ್ಯಥಾ
    image out = load_image_stb(filename, c);
#endif

    ಯದಿ((h && w) && (h != out.h || w != out.w)){
        image resized = resize_image(out, w, h);
        free_image(out);
        out = resized;
    }
    ಪ್ರತ್ಯಾಗಮ out;
}

image load_image_color(ವರ್ಣ *filename, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h)
{
    ಪ್ರತ್ಯಾಗಮ load_image(filename, w, h, 3);
}

image get_image_layer(image m, ಪೂರ್ಣಾಙ್ಕ l)
{
    image out = make_image(m.w, m.h, 1);
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < m.h*m.w; ++i){
        out.data[i] = m.data[i+l*m.h*m.w];
    }
    ಪ್ರತ್ಯಾಗಮ out;
}
ಶೂನ್ಯ print_image(image m)
{
    ಪೂರ್ಣಾಙ್ಕ i, j, k;
    ಪ್ರತಿ(i =0 ; i < m.c; ++i){
        ಪ್ರತಿ(j =0 ; j < m.h; ++j){
            ಪ್ರತಿ(k = 0; k < m.w; ++k){
                printf("%.2lf, ", m.data[i*m.h*m.w + j*m.w + k]);
                ಯದಿ(k > 30) ಭಙ್ಗ;
            }
            printf("\n");
            ಯದಿ(j > 30) ಭಙ್ಗ;
        }
        printf("\n");
    }
    printf("\n");
}

image collapse_images_vert(image *ims, ಪೂರ್ಣಾಙ್ಕ n)
{
    ಪೂರ್ಣಾಙ್ಕ color = 1;
    ಪೂರ್ಣಾಙ್ಕ border = 1;
    ಪೂರ್ಣಾಙ್ಕ h,w,c;
    w = ims[0].w;
    h = (ims[0].h + border) * n - border;
    c = ims[0].c;
    ಯದಿ(c != 3 || !color){
        w = (w+border)*c - border;
        c = 1;
    }

    image filters = make_image(w, h, c);
    ಪೂರ್ಣಾಙ್ಕ i,j;
    ಪ್ರತಿ(i = 0; i < n; ++i){
        ಪೂರ್ಣಾಙ್ಕ h_offset = i*(ims[0].h+border);
        image copy = copy_image(ims[i]);
        //normalize_image(copy);
        ಯದಿ(c == 3 && color){
            embed_image(copy, filters, 0, h_offset);
        }
        ಅನ್ಯಥಾ{
            ಪ್ರತಿ(j = 0; j < copy.c; ++j){
                ಪೂರ್ಣಾಙ್ಕ w_offset = j*(ims[0].w+border);
                image layer = get_image_layer(copy, j);
                embed_image(layer, filters, w_offset, h_offset);
                free_image(layer);
            }
        }
        free_image(copy);
    }
    ಪ್ರತ್ಯಾಗಮ filters;
} 

image collapse_images_horz(image *ims, ಪೂರ್ಣಾಙ್ಕ n)
{
    ಪೂರ್ಣಾಙ್ಕ color = 1;
    ಪೂರ್ಣಾಙ್ಕ border = 1;
    ಪೂರ್ಣಾಙ್ಕ h,w,c;
    ಪೂರ್ಣಾಙ್ಕ size = ims[0].h;
    h = size;
    w = (ims[0].w + border) * n - border;
    c = ims[0].c;
    ಯದಿ(c != 3 || !color){
        h = (h+border)*c - border;
        c = 1;
    }

    image filters = make_image(w, h, c);
    ಪೂರ್ಣಾಙ್ಕ i,j;
    ಪ್ರತಿ(i = 0; i < n; ++i){
        ಪೂರ್ಣಾಙ್ಕ w_offset = i*(size+border);
        image copy = copy_image(ims[i]);
        //normalize_image(copy);
        ಯದಿ(c == 3 && color){
            embed_image(copy, filters, w_offset, 0);
        }
        ಅನ್ಯಥಾ{
            ಪ್ರತಿ(j = 0; j < copy.c; ++j){
                ಪೂರ್ಣಾಙ್ಕ h_offset = j*(size+border);
                image layer = get_image_layer(copy, j);
                embed_image(layer, filters, w_offset, h_offset);
                free_image(layer);
            }
        }
        free_image(copy);
    }
    ಪ್ರತ್ಯಾಗಮ filters;
} 

ಶೂನ್ಯ show_image_normalized(image im, ಧ್ರುವ ವರ್ಣ *name)
{
    image c = copy_image(im);
    normalize_image(c);
    show_image(c, name, 1);
    free_image(c);
}

ಶೂನ್ಯ show_images(image *ims, ಪೂರ್ಣಾಙ್ಕ n, ವರ್ಣ *window)
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

ಶೂನ್ಯ free_image(image m)
{
    ಯದಿ(m.data){
        ಮುಕ್ತ(m.data);
    }
}
