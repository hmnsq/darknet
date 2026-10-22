#ଅନ୍ତର୍ଭାଵ "image.h"
#ଅନ୍ତର୍ଭାଵ "utils.h"
#ଅନ୍ତର୍ଭାଵ "blas.h"
#ଅନ୍ତର୍ଭାଵ "cuda.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <math.h>

#define STB_IMAGE_IMPLEMENTATION
#ଅନ୍ତର୍ଭାଵ "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#ଅନ୍ତର୍ଭାଵ "stb_image_write.h"

ପୂର୍ଣ୍ଣସଂଖ୍ୟା windows = 0;

ଦଶମିକ colors[6][3] = { {1,0,1}, {0,0,1},{0,1,1},{0,1,0},{1,1,0},{1,0,0} };

ଦଶମିକ get_color(ପୂର୍ଣ୍ଣସଂଖ୍ୟା c, ପୂର୍ଣ୍ଣସଂଖ୍ୟା x, ପୂର୍ଣ୍ଣସଂଖ୍ୟା max)
{
    ଦଶମିକ ratio = ((ଦଶମିକ)x/max)*5;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = floor(ratio);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା j = ceil(ratio);
    ratio -= i;
    ଦଶମିକ r = (1-ratio) * colors[i][c] + ratio*colors[j][c];
    //printf("%f\n", r);
    ଫେରନ୍ତୁ r;
}

image mask_to_rgb(image mask)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = mask.c;
    image im = make_image(mask.w, mask.h, 3);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j;
    ପାଇଁ(j = 0; j < n; ++j){
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା offset = j*123457 % n;
        ଦଶମିକ red = get_color(2,offset,n);
        ଦଶମିକ green = get_color(1,offset,n);
        ଦଶମିକ blue = get_color(0,offset,n);
        ପାଇଁ(i = 0; i < im.w*im.h; ++i){
            im.data[i + 0*im.w*im.h] += mask.data[j*im.h*im.w + i]*red;
            im.data[i + 1*im.w*im.h] += mask.data[j*im.h*im.w + i]*green;
            im.data[i + 2*im.w*im.h] += mask.data[j*im.h*im.w + i]*blue;
        }
    }
    ଫେରନ୍ତୁ im;
}

ସ୍ଥିର ଦଶମିକ get_pixel(image m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା x, ପୂର୍ଣ୍ଣସଂଖ୍ୟା y, ପୂର୍ଣ୍ଣସଂଖ୍ୟା c)
{
    ପ୍ରତିଜ୍ଞା(x < m.w && y < m.h && c < m.c);
    ଫେରନ୍ତୁ m.data[c*m.h*m.w + y*m.w + x];
}
ସ୍ଥିର ଦଶମିକ get_pixel_extend(image m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା x, ପୂର୍ଣ୍ଣସଂଖ୍ୟା y, ପୂର୍ଣ୍ଣସଂଖ୍ୟା c)
{
    ଯଦି(x < 0 || x >= m.w || y < 0 || y >= m.h) ଫେରନ୍ତୁ 0;
    /*
    if(x < 0) x = 0;
    if(x >= m.w) x = m.w-1;
    if(y < 0) y = 0;
    if(y >= m.h) y = m.h-1;
    */
    ଯଦି(c < 0 || c >= m.c) ଫେରନ୍ତୁ 0;
    ଫେରନ୍ତୁ get_pixel(m, x, y, c);
}
ସ୍ଥିର ଶୂନ୍ୟ set_pixel(image m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା x, ପୂର୍ଣ୍ଣସଂଖ୍ୟା y, ପୂର୍ଣ୍ଣସଂଖ୍ୟା c, ଦଶମିକ val)
{
    ଯଦି (x < 0 || y < 0 || c < 0 || x >= m.w || y >= m.h || c >= m.c) ଫେରନ୍ତୁ;
    ପ୍ରତିଜ୍ଞା(x < m.w && y < m.h && c < m.c);
    m.data[c*m.h*m.w + y*m.w + x] = val;
}
ସ୍ଥିର ଶୂନ୍ୟ add_pixel(image m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା x, ପୂର୍ଣ୍ଣସଂଖ୍ୟା y, ପୂର୍ଣ୍ଣସଂଖ୍ୟା c, ଦଶମିକ val)
{
    ପ୍ରତିଜ୍ଞା(x < m.w && y < m.h && c < m.c);
    m.data[c*m.h*m.w + y*m.w + x] += val;
}

ସ୍ଥିର ଦଶମିକ bilinear_interpolate(image im, ଦଶମିକ x, ଦଶମିକ y, ପୂର୍ଣ୍ଣସଂଖ୍ୟା c)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ix = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) floorf(x);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା iy = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) floorf(y);

    ଦଶମିକ dx = x - ix;
    ଦଶମିକ dy = y - iy;

    ଦଶମିକ val = (1-dy) * (1-dx) * get_pixel_extend(im, ix, iy, c) + 
        dy     * (1-dx) * get_pixel_extend(im, ix, iy+1, c) + 
        (1-dy) *   dx   * get_pixel_extend(im, ix+1, iy, c) +
        dy     *   dx   * get_pixel_extend(im, ix+1, iy+1, c);
    ଫେରନ୍ତୁ val;
}


ଶୂନ୍ୟ composite_image(image source, image dest, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dx, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dy)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା x,y,k;
    ପାଇଁ(k = 0; k < source.c; ++k){
        ପାଇଁ(y = 0; y < source.h; ++y){
            ପାଇଁ(x = 0; x < source.w; ++x){
                ଦଶମିକ val = get_pixel(source, x, y, k);
                ଦଶମିକ val2 = get_pixel_extend(dest, dx+x, dy+y, k);
                set_pixel(dest, dx+x, dy+y, k, val * val2);
            }
        }
    }
}

image border_image(image a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା border)
{
    image b = make_image(a.w + 2*border, a.h + 2*border, a.c);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା x,y,k;
    ପାଇଁ(k = 0; k < b.c; ++k){
        ପାଇଁ(y = 0; y < b.h; ++y){
            ପାଇଁ(x = 0; x < b.w; ++x){
                ଦଶମିକ val = get_pixel_extend(a, x - border, y - border, k);
                ଯଦି(x - border < 0 || x - border >= a.w || y - border < 0 || y - border >= a.h) val = 1;
                set_pixel(b, x, y, k, val);
            }
        }
    }
    ଫେରନ୍ତୁ b;
}

image tile_images(image a, image b, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dx)
{
    ଯଦି(a.w == 0) ଫେରନ୍ତୁ copy_image(b);
    image c = make_image(a.w + b.w + dx, (a.h > b.h) ? a.h : b.h, (a.c > b.c) ? a.c : b.c);
    fill_cpu(c.w*c.h*c.c, 1, c.data, 1);
    embed_image(a, c, 0, 0); 
    composite_image(b, c, a.w + dx, 0);
    ଫେରନ୍ତୁ c;
}

image get_label(image **characters, ଅକ୍ଷର *string, ପୂର୍ଣ୍ଣସଂଖ୍ୟା size)
{
    size = size/10;
    ଯଦି(size > 7) size = 7;
    image label = make_empty_image(0,0,0);
    ଯେତେବେଳେ(*string){
        image l = characters[size][(ପୂର୍ଣ୍ଣସଂଖ୍ୟା)*string];
        image n = tile_images(label, l, -size - 1 + (size+1)/2);
        free_image(label);
        label = n;
        ++string;
    }
    image b = border_image(label, label.h*.25);
    free_image(label);
    ଫେରନ୍ତୁ b;
}

ଶୂନ୍ୟ draw_label(image a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା r, ପୂର୍ଣ୍ଣସଂଖ୍ୟା c, image label, ଅଚଳ ଦଶମିକ *rgb)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା w = label.w;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା h = label.h;
    ଯଦି (r - h >= 0) r = r - h;

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j, k;
    ପାଇଁ(j = 0; j < h && j + r < a.h; ++j){
        ପାଇଁ(i = 0; i < w && i + c < a.w; ++i){
            ପାଇଁ(k = 0; k < label.c; ++k){
                ଦଶମିକ val = get_pixel(label, i, j, k);
                set_pixel(a, i+c, j+r, k, rgb[k] * val);
            }
        }
    }
}

ଶୂନ୍ୟ draw_box(image a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା x1, ପୂର୍ଣ୍ଣସଂଖ୍ୟା y1, ପୂର୍ଣ୍ଣସଂଖ୍ୟା x2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା y2, ଦଶମିକ r, ଦଶମିକ g, ଦଶମିକ b)
{
    //normalize_image(a);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ଯଦି(x1 < 0) x1 = 0;
    ଯଦି(x1 >= a.w) x1 = a.w-1;
    ଯଦି(x2 < 0) x2 = 0;
    ଯଦି(x2 >= a.w) x2 = a.w-1;

    ଯଦି(y1 < 0) y1 = 0;
    ଯଦି(y1 >= a.h) y1 = a.h-1;
    ଯଦି(y2 < 0) y2 = 0;
    ଯଦି(y2 >= a.h) y2 = a.h-1;

    ପାଇଁ(i = x1; i <= x2; ++i){
        a.data[i + y1*a.w + 0*a.w*a.h] = r;
        a.data[i + y2*a.w + 0*a.w*a.h] = r;

        a.data[i + y1*a.w + 1*a.w*a.h] = g;
        a.data[i + y2*a.w + 1*a.w*a.h] = g;

        a.data[i + y1*a.w + 2*a.w*a.h] = b;
        a.data[i + y2*a.w + 2*a.w*a.h] = b;
    }
    ପାଇଁ(i = y1; i <= y2; ++i){
        a.data[x1 + i*a.w + 0*a.w*a.h] = r;
        a.data[x2 + i*a.w + 0*a.w*a.h] = r;

        a.data[x1 + i*a.w + 1*a.w*a.h] = g;
        a.data[x2 + i*a.w + 1*a.w*a.h] = g;

        a.data[x1 + i*a.w + 2*a.w*a.h] = b;
        a.data[x2 + i*a.w + 2*a.w*a.h] = b;
    }
}

ଶୂନ୍ୟ draw_box_width(image a, ପୂର୍ଣ୍ଣସଂଖ୍ୟା x1, ପୂର୍ଣ୍ଣସଂଖ୍ୟା y1, ପୂର୍ଣ୍ଣସଂଖ୍ୟା x2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା y2, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ଦଶମିକ r, ଦଶମିକ g, ଦଶମିକ b)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < w; ++i){
        draw_box(a, x1+i, y1+i, x2-i, y2-i, r, g, b);
    }
}

ଶୂନ୍ୟ draw_bbox(image a, box bbox, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ଦଶମିକ r, ଦଶମିକ g, ଦଶମିକ b)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ବାମ  = (bbox.x-bbox.w/2)*a.w;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଡାହାଣ = (bbox.x+bbox.w/2)*a.w;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଶୀର୍ଷ   = (bbox.y-bbox.h/2)*a.h;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା bot   = (bbox.y+bbox.h/2)*a.h;

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < w; ++i){
        draw_box(a, ବାମ+i, ଶୀର୍ଷ+i, ଡାହାଣ-i, bot-i, r, g, b);
    }
}

image **load_alphabet()
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j;
    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା nsize = 8;
    image **alphabets = calloc(nsize, ଆକାର(image));
    ପାଇଁ(j = 0; j < nsize; ++j){
        alphabets[j] = calloc(128, ଆକାର(image));
        ପାଇଁ(i = 32; i < 127; ++i){
            ଅକ୍ଷର buff[256];
            sprintf(buff, "data/labels/%d_%d.png", i, j);
            alphabets[j][i] = load_image_color(buff, 0, 0);
        }
    }
    ଫେରନ୍ତୁ alphabets;
}

ଶୂନ୍ୟ draw_detections(image im, detection *dets, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ, ଦଶମିକ thresh, ଅକ୍ଷର **names, image **alphabet, ପୂର୍ଣ୍ଣସଂଖ୍ୟା classes)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i,j;

    ପାଇଁ(i = 0; i < ଅଙ୍କ; ++i){
        ଅକ୍ଷର labelstr[4096] = {0};
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା class = -1;
        ପାଇଁ(j = 0; j < classes; ++j){
            ଯଦି (dets[i].prob[j] > thresh){
                ଯଦି (class < 0) {
                    strcat(labelstr, names[j]);
                    class = j;
                } ଅନ୍ୟଥା {
                    strcat(labelstr, ", ");
                    strcat(labelstr, names[j]);
                }
                printf("%s: %.0f%%\n", names[j], dets[i].prob[j]*100);
            }
        }
        ଯଦି(class >= 0){
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା width = im.h * .006;

            /*
               if(0){
               width = pow(prob, 1./2.)*10+1;
               alphabet = 0;
               }
             */

            //printf("%d %s: %.0f%%\n", i, names[class], prob*100);
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା offset = class*123457 % classes;
            ଦଶମିକ red = get_color(2,offset,classes);
            ଦଶମିକ green = get_color(1,offset,classes);
            ଦଶମିକ blue = get_color(0,offset,classes);
            ଦଶମିକ rgb[3];

            //width = prob*20+2;

            rgb[0] = red;
            rgb[1] = green;
            rgb[2] = blue;
            box b = dets[i].bbox;
            //printf("%f %f %f %f\n", b.x, b.y, b.w, b.h);

            ପୂର୍ଣ୍ଣସଂଖ୍ୟା ବାମ  = (b.x-b.w/2.)*im.w;
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଡାହାଣ = (b.x+b.w/2.)*im.w;
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଶୀର୍ଷ   = (b.y-b.h/2.)*im.h;
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା bot   = (b.y+b.h/2.)*im.h;

            ଯଦି(ବାମ < 0) ବାମ = 0;
            ଯଦି(ଡାହାଣ > im.w-1) ଡାହାଣ = im.w-1;
            ଯଦି(ଶୀର୍ଷ < 0) ଶୀର୍ଷ = 0;
            ଯଦି(bot > im.h-1) bot = im.h-1;

            draw_box_width(im, ବାମ, ଶୀର୍ଷ, ଡାହାଣ, bot, width, red, green, blue);
            ଯଦି (alphabet) {
                image label = get_label(alphabet, labelstr, (im.h*.03));
                draw_label(im, ଶୀର୍ଷ + width, ବାମ, label, rgb);
                free_image(label);
            }
            ଯଦି (dets[i].mask){
                image mask = float_to_image(14, 14, 1, dets[i].mask);
                image resized_mask = resize_image(mask, b.w*im.w, b.h*im.h);
                image tmask = threshold_image(resized_mask, .5);
                embed_image(tmask, im, ବାମ, ଶୀର୍ଷ);
                free_image(mask);
                free_image(resized_mask);
                free_image(tmask);
            }
        }
    }
}

ଶୂନ୍ୟ transpose_image(image im)
{
    ପ୍ରତିଜ୍ଞା(im.w == im.h);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, m;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା c;
    ପାଇଁ(c = 0; c < im.c; ++c){
        ପାଇଁ(n = 0; n < im.w-1; ++n){
            ପାଇଁ(m = n + 1; m < im.w; ++m){
                ଦଶମିକ swap = im.data[m + im.w*(n + im.h*c)];
                im.data[m + im.w*(n + im.h*c)] = im.data[n + im.w*(m + im.h*c)];
                im.data[n + im.w*(m + im.h*c)] = swap;
            }
        }
    }
}

ଶୂନ୍ୟ rotate_image_cw(image im, ପୂର୍ଣ୍ଣସଂଖ୍ୟା times)
{
    ପ୍ରତିଜ୍ଞା(im.w == im.h);
    times = (times + 400) % 4;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, x, y, c;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = im.w;
    ପାଇଁ(i = 0; i < times; ++i){
        ପାଇଁ(c = 0; c < im.c; ++c){
            ପାଇଁ(x = 0; x < n/2; ++x){
                ପାଇଁ(y = 0; y < (n-1)/2 + 1; ++y){
                    ଦଶମିକ temp = im.data[y + im.w*(x + im.h*c)];
                    im.data[y + im.w*(x + im.h*c)] = im.data[n-1-x + im.w*(y + im.h*c)];
                    im.data[n-1-x + im.w*(y + im.h*c)] = im.data[n-1-y + im.w*(n-1-x + im.h*c)];
                    im.data[n-1-y + im.w*(n-1-x + im.h*c)] = im.data[x + im.w*(n-1-y + im.h*c)];
                    im.data[x + im.w*(n-1-y + im.h*c)] = temp;
                }
            }
        }
    }
}

ଶୂନ୍ୟ flip_image(image a)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i,j,k;
    ପାଇଁ(k = 0; k < a.c; ++k){
        ପାଇଁ(i = 0; i < a.h; ++i){
            ପାଇଁ(j = 0; j < a.w/2; ++j){
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = j + a.w*(i + a.h*(k));
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା flip = (a.w - j - 1) + a.w*(i + a.h*(k));
                ଦଶମିକ swap = a.data[flip];
                a.data[flip] = a.data[index];
                a.data[index] = swap;
            }
        }
    }
}

image image_distance(image a, image b)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i,j;
    image dist = make_image(a.w, a.h, 1);
    ପାଇଁ(i = 0; i < a.c; ++i){
        ପାଇଁ(j = 0; j < a.h*a.w; ++j){
            dist.data[j] += pow(a.data[i*a.h*a.w+j]-b.data[i*a.h*a.w+j],2);
        }
    }
    ପାଇଁ(j = 0; j < a.h*a.w; ++j){
        dist.data[j] = sqrt(dist.data[j]);
    }
    ଫେରନ୍ତୁ dist;
}

ଶୂନ୍ୟ ghost_image(image source, image dest, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dx, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dy)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା x,y,k;
    ଦଶମିକ max_dist = sqrt((-source.w/2. + .5)*(-source.w/2. + .5));
    ପାଇଁ(k = 0; k < source.c; ++k){
        ପାଇଁ(y = 0; y < source.h; ++y){
            ପାଇଁ(x = 0; x < source.w; ++x){
                ଦଶମିକ dist = sqrt((x - source.w/2. + .5)*(x - source.w/2. + .5) + (y - source.h/2. + .5)*(y - source.h/2. + .5));
                ଦଶମିକ alpha = (1 - dist/max_dist);
                ଯଦି(alpha < 0) alpha = 0;
                ଦଶମିକ v1 = get_pixel(source, x,y,k);
                ଦଶମିକ v2 = get_pixel(dest, dx+x,dy+y,k);
                ଦଶମିକ val = alpha*v1 + (1-alpha)*v2;
                set_pixel(dest, dx+x, dy+y, k, val);
            }
        }
    }
}

ଶୂନ୍ୟ blocky_image(image im, ପୂର୍ଣ୍ଣସଂଖ୍ୟା s)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i,j,k;
    ପାଇଁ(k = 0; k < im.c; ++k){
        ପାଇଁ(j = 0; j < im.h; ++j){
            ପାଇଁ(i = 0; i < im.w; ++i){
                im.data[i + im.w*(j + im.h*k)] = im.data[i/s*s + im.w*(j/s*s + im.h*k)];
            }
        }
    }
}

ଶୂନ୍ୟ censor_image(image im, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dx, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dy, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i,j,k;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା s = 32;
    ଯଦି(dx < 0) dx = 0;
    ଯଦି(dy < 0) dy = 0;

    ପାଇଁ(k = 0; k < im.c; ++k){
        ପାଇଁ(j = dy; j < dy + h && j < im.h; ++j){
            ପାଇଁ(i = dx; i < dx + w && i < im.w; ++i){
                im.data[i + im.w*(j + im.h*k)] = im.data[i/s*s + im.w*(j/s*s + im.h*k)];
                //im.data[i + j*im.w + k*im.w*im.h] = 0;
            }
        }
    }
}

ଶୂନ୍ୟ embed_image(image source, image dest, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dx, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dy)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା x,y,k;
    ପାଇଁ(k = 0; k < source.c; ++k){
        ପାଇଁ(y = 0; y < source.h; ++y){
            ପାଇଁ(x = 0; x < source.w; ++x){
                ଦଶମିକ val = get_pixel(source, x,y,k);
                set_pixel(dest, dx+x, dy+y, k, val);
            }
        }
    }
}

image collapse_image_layers(image source, ପୂର୍ଣ୍ଣସଂଖ୍ୟା border)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା h = source.h;
    h = (h+border)*source.c - border;
    image dest = make_image(source.w, h, 1);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < source.c; ++i){
        image layer = get_image_layer(source, i);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା h_offset = i*(source.h+border);
        embed_image(layer, dest, 0, h_offset);
        free_image(layer);
    }
    ଫେରନ୍ତୁ dest;
}

ଶୂନ୍ୟ constrain_image(image im)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < im.w*im.h*im.c; ++i){
        ଯଦି(im.data[i] < 0) im.data[i] = 0;
        ଯଦି(im.data[i] > 1) im.data[i] = 1;
    }
}

ଶୂନ୍ୟ normalize_image(image p)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ଦଶମିକ min = 9999999;
    ଦଶମିକ max = -999999;

    ପାଇଁ(i = 0; i < p.h*p.w*p.c; ++i){
        ଦଶମିକ v = p.data[i];
        ଯଦି(v < min) min = v;
        ଯଦି(v > max) max = v;
    }
    ଯଦି(max - min < .000000001){
        min = 0;
        max = 1;
    }
    ପାଇଁ(i = 0; i < p.c*p.w*p.h; ++i){
        p.data[i] = (p.data[i] - min)/(max-min);
    }
}

ଶୂନ୍ୟ normalize_image2(image p)
{
    ଦଶମିକ *min = calloc(p.c, ଆକାର(ଦଶମିକ));
    ଦଶମିକ *max = calloc(p.c, ଆକାର(ଦଶମିକ));
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i,j;
    ପାଇଁ(i = 0; i < p.c; ++i) min[i] = max[i] = p.data[i*p.h*p.w];

    ପାଇଁ(j = 0; j < p.c; ++j){
        ପାଇଁ(i = 0; i < p.h*p.w; ++i){
            ଦଶମିକ v = p.data[i+j*p.h*p.w];
            ଯଦି(v < min[j]) min[j] = v;
            ଯଦି(v > max[j]) max[j] = v;
        }
    }
    ପାଇଁ(i = 0; i < p.c; ++i){
        ଯଦି(max[i] - min[i] < .000000001){
            min[i] = 0;
            max[i] = 1;
        }
    }
    ପାଇଁ(j = 0; j < p.c; ++j){
        ପାଇଁ(i = 0; i < p.w*p.h; ++i){
            p.data[i+j*p.h*p.w] = (p.data[i+j*p.h*p.w] - min[j])/(max[j]-min[j]);
        }
    }
    ମୁକ୍ତ(min);
    ମୁକ୍ତ(max);
}

ଶୂନ୍ୟ copy_image_into(image src, image dest)
{
    memcpy(dest.data, src.data, src.h*src.w*src.c*ଆକାର(ଦଶମିକ));
}

image copy_image(image p)
{
    image copy = p;
    copy.data = calloc(p.h*p.w*p.c, ଆକାର(ଦଶମିକ));
    memcpy(copy.data, p.data, p.h*p.w*p.c*ଆକାର(ଦଶମିକ));
    ଫେରନ୍ତୁ copy;
}

ଶୂନ୍ୟ rgbgr_image(image im)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < im.w*im.h; ++i){
        ଦଶମିକ swap = im.data[i];
        im.data[i] = im.data[i+im.w*im.h*2];
        im.data[i+im.w*im.h*2] = swap;
    }
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା show_image(image p, ଅଚଳ ଅକ୍ଷର *name, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ms)
{
#ifdef OPENCV
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା c = show_image_cv(p, name, ms);
    ଫେରନ୍ତୁ c;
#ଅନ୍ୟଥା
    fprintf(stderr, "Not compiled with OpenCV, saving to %s.png instead\n", name);
    save_image(p, name);
    ଫେରନ୍ତୁ -1;
#endif
}

ଶୂନ୍ୟ save_image_options(image im, ଅଚଳ ଅକ୍ଷର *name, IMTYPE f, ପୂର୍ଣ୍ଣସଂଖ୍ୟା quality)
{
    ଅକ୍ଷର buff[256];
    //sprintf(buff, "%s (%d)", name, windows);
    ଯଦି(f == PNG)       sprintf(buff, "%s.png", name);
    ଅନ୍ୟଥା ଯଦି (f == BMP) sprintf(buff, "%s.bmp", name);
    ଅନ୍ୟଥା ଯଦି (f == TGA) sprintf(buff, "%s.tga", name);
    ଅନ୍ୟଥା ଯଦି (f == JPG) sprintf(buff, "%s.jpg", name);
    ଅନ୍ୟଥା               sprintf(buff, "%s.png", name);
    unsigned ଅକ୍ଷର *data = calloc(im.w*im.h*im.c, ଆକାର(ଅକ୍ଷର));
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i,k;
    ପାଇଁ(k = 0; k < im.c; ++k){
        ପାଇଁ(i = 0; i < im.w*im.h; ++i){
            data[i*im.c+k] = (unsigned ଅକ୍ଷର) (255*im.data[i + k*im.w*im.h]);
        }
    }
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା success = 0;
    ଯଦି(f == PNG)       success = stbi_write_png(buff, im.w, im.h, im.c, data, im.w*im.c);
    ଅନ୍ୟଥା ଯଦି (f == BMP) success = stbi_write_bmp(buff, im.w, im.h, im.c, data);
    ଅନ୍ୟଥା ଯଦି (f == TGA) success = stbi_write_tga(buff, im.w, im.h, im.c, data);
    ଅନ୍ୟଥା ଯଦି (f == JPG) success = stbi_write_jpg(buff, im.w, im.h, im.c, data, quality);
    ମୁକ୍ତ(data);
    ଯଦି(!success) fprintf(stderr, "Failed to write image %s\n", buff);
}

ଶୂନ୍ୟ save_image(image im, ଅଚଳ ଅକ୍ଷର *name)
{
    save_image_options(im, name, JPG, 80);
}

ଶୂନ୍ୟ show_image_layers(image p, ଅକ୍ଷର *name)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ଅକ୍ଷର buff[256];
    ପାଇଁ(i = 0; i < p.c; ++i){
        sprintf(buff, "%s - Layer %d", name, i);
        image layer = get_image_layer(p, i);
        show_image(layer, buff, 1);
        free_image(layer);
    }
}

ଶୂନ୍ୟ show_image_collapsed(image p, ଅକ୍ଷର *name)
{
    image c = collapse_image_layers(p, 1);
    show_image(c, name, 1);
    free_image(c);
}

image make_empty_image(ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ପୂର୍ଣ୍ଣସଂଖ୍ୟା c)
{
    image out;
    out.data = 0;
    out.h = h;
    out.w = w;
    out.c = c;
    ଫେରନ୍ତୁ out;
}

image make_image(ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ପୂର୍ଣ୍ଣସଂଖ୍ୟା c)
{
    image out = make_empty_image(w,h,c);
    out.data = calloc(h*w*c, ଆକାର(ଦଶମିକ));
    ଫେରନ୍ତୁ out;
}

image make_random_image(ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ପୂର୍ଣ୍ଣସଂଖ୍ୟା c)
{
    image out = make_empty_image(w,h,c);
    out.data = calloc(h*w*c, ଆକାର(ଦଶମିକ));
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < w*h*c; ++i){
        out.data[i] = (rand_normal() * .25) + .5;
    }
    ଫେରନ୍ତୁ out;
}

image float_to_image(ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ପୂର୍ଣ୍ଣସଂଖ୍ୟା c, ଦଶମିକ *data)
{
    image out = make_empty_image(w,h,c);
    out.data = data;
    ଫେରନ୍ତୁ out;
}

ଶୂନ୍ୟ place_image(image im, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dx, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dy, image canvas)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା x, y, c;
    ପାଇଁ(c = 0; c < im.c; ++c){
        ପାଇଁ(y = 0; y < h; ++y){
            ପାଇଁ(x = 0; x < w; ++x){
                ଦଶମିକ rx = ((ଦଶମିକ)x / w) * im.w;
                ଦଶମିକ ry = ((ଦଶମିକ)y / h) * im.h;
                ଦଶମିକ val = bilinear_interpolate(im, rx, ry, c);
                set_pixel(canvas, x + dx, y + dy, c, val);
            }
        }
    }
}

image center_crop_image(image im, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା m = (im.w < im.h) ? im.w : im.h;   
    image c = crop_image(im, (im.w - m) / 2, (im.h - m)/2, m, m);
    image r = resize_image(c, w, h);
    free_image(c);
    ଫେରନ୍ତୁ r;
}

image rotate_crop_image(image im, ଦଶମିକ rad, ଦଶମିକ s, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ଦଶମିକ dx, ଦଶମିକ dy, ଦଶମିକ aspect)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା x, y, c;
    ଦଶମିକ cx = im.w/2.;
    ଦଶମିକ cy = im.h/2.;
    image rot = make_image(w, h, im.c);
    ପାଇଁ(c = 0; c < im.c; ++c){
        ପାଇଁ(y = 0; y < h; ++y){
            ପାଇଁ(x = 0; x < w; ++x){
                ଦଶମିକ rx = cos(rad)*((x - w/2.)/s*aspect + dx/s*aspect) - sin(rad)*((y - h/2.)/s + dy/s) + cx;
                ଦଶମିକ ry = sin(rad)*((x - w/2.)/s*aspect + dx/s*aspect) + cos(rad)*((y - h/2.)/s + dy/s) + cy;
                ଦଶମିକ val = bilinear_interpolate(im, rx, ry, c);
                set_pixel(rot, x, y, c, val);
            }
        }
    }
    ଫେରନ୍ତୁ rot;
}

image rotate_image(image im, ଦଶମିକ rad)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା x, y, c;
    ଦଶମିକ cx = im.w/2.;
    ଦଶମିକ cy = im.h/2.;
    image rot = make_image(im.w, im.h, im.c);
    ପାଇଁ(c = 0; c < im.c; ++c){
        ପାଇଁ(y = 0; y < im.h; ++y){
            ପାଇଁ(x = 0; x < im.w; ++x){
                ଦଶମିକ rx = cos(rad)*(x-cx) - sin(rad)*(y-cy) + cx;
                ଦଶମିକ ry = sin(rad)*(x-cx) + cos(rad)*(y-cy) + cy;
                ଦଶମିକ val = bilinear_interpolate(im, rx, ry, c);
                set_pixel(rot, x, y, c, val);
            }
        }
    }
    ଫେରନ୍ତୁ rot;
}

ଶୂନ୍ୟ fill_image(image m, ଦଶମିକ s)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < m.h*m.w*m.c; ++i) m.data[i] = s;
}

ଶୂନ୍ୟ translate_image(image m, ଦଶମିକ s)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < m.h*m.w*m.c; ++i) m.data[i] += s;
}

ଶୂନ୍ୟ scale_image(image m, ଦଶମିକ s)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < m.h*m.w*m.c; ++i) m.data[i] *= s;
}

image crop_image(image im, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dx, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dy, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h)
{
    image cropped = make_image(w, h, im.c);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j, k;
    ପାଇଁ(k = 0; k < im.c; ++k){
        ପାଇଁ(j = 0; j < h; ++j){
            ପାଇଁ(i = 0; i < w; ++i){
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା r = j + dy;
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା c = i + dx;
                ଦଶମିକ val = 0;
                r = constrain_int(r, 0, im.h-1);
                c = constrain_int(c, 0, im.w-1);
                val = get_pixel(im, c, r, k);
                set_pixel(cropped, i, j, k, val);
            }
        }
    }
    ଫେରନ୍ତୁ cropped;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା best_3d_shift_r(image a, image b, ପୂର୍ଣ୍ଣସଂଖ୍ୟା min, ପୂର୍ଣ୍ଣସଂଖ୍ୟା max)
{
    ଯଦି(min == max) ଫେରନ୍ତୁ min;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା mid = floor((min + max) / 2.);
    image c1 = crop_image(b, 0, mid, b.w, b.h);
    image c2 = crop_image(b, 0, mid+1, b.w, b.h);
    ଦଶମିକ d1 = dist_array(c1.data, a.data, a.w*a.h*a.c, 10);
    ଦଶମିକ d2 = dist_array(c2.data, a.data, a.w*a.h*a.c, 10);
    free_image(c1);
    free_image(c2);
    ଯଦି(d1 < d2) ଫେରନ୍ତୁ best_3d_shift_r(a, b, min, mid);
    ଅନ୍ୟଥା ଫେରନ୍ତୁ best_3d_shift_r(a, b, mid+1, max);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା best_3d_shift(image a, image b, ପୂର୍ଣ୍ଣସଂଖ୍ୟା min, ପୂର୍ଣ୍ଣସଂଖ୍ୟା max)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା best = 0;
    ଦଶମିକ best_distance = FLT_MAX;
    ପାଇଁ(i = min; i <= max; i += 2){
        image c = crop_image(b, 0, i, b.w, b.h);
        ଦଶମିକ d = dist_array(c.data, a.data, a.w*a.h*a.c, 100);
        ଯଦି(d < best_distance){
            best_distance = d;
            best = i;
        }
        printf("%d %f\n", i, d);
        free_image(c);
    }
    ଫେରନ୍ତୁ best;
}

ଶୂନ୍ୟ composite_3d(ଅକ୍ଷର *f1, ଅକ୍ଷର *f2, ଅକ୍ଷର *out, ପୂର୍ଣ୍ଣସଂଖ୍ୟା delta)
{
    ଯଦି(!out) out = "out";
    image a = load_image(f1, 0,0,0);
    image b = load_image(f2, 0,0,0);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା shift = best_3d_shift_r(a, b, -a.h/100, a.h/100);

    image c1 = crop_image(b, 10, shift, b.w, b.h);
    ଦଶମିକ d1 = dist_array(c1.data, a.data, a.w*a.h*a.c, 100);
    image c2 = crop_image(b, -10, shift, b.w, b.h);
    ଦଶମିକ d2 = dist_array(c2.data, a.data, a.w*a.h*a.c, 100);

    ଯଦି(d2 < d1 && 0){
        image swap = a;
        a = b;
        b = swap;
        shift = -shift;
        printf("swapped, %d\n", shift);
    }
    ଅନ୍ୟଥା{
        printf("%d\n", shift);
    }

    image c = crop_image(b, delta, shift, a.w, a.h);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < c.w*c.h; ++i){
        c.data[i] = a.data[i];
    }
    save_image(c, out);
}

ଶୂନ୍ୟ letterbox_image_into(image im, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, image boxed)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା new_w = im.w;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା new_h = im.h;
    ଯଦି (((ଦଶମିକ)w/im.w) < ((ଦଶମିକ)h/im.h)) {
        new_w = w;
        new_h = (im.h * w)/im.w;
    } ଅନ୍ୟଥା {
        new_h = h;
        new_w = (im.w * h)/im.h;
    }
    image resized = resize_image(im, new_w, new_h);
    embed_image(resized, boxed, (w-new_w)/2, (h-new_h)/2); 
    free_image(resized);
}

image letterbox_image(image im, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା new_w = im.w;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା new_h = im.h;
    ଯଦି (((ଦଶମିକ)w/im.w) < ((ଦଶମିକ)h/im.h)) {
        new_w = w;
        new_h = (im.h * w)/im.w;
    } ଅନ୍ୟଥା {
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
    ଫେରନ୍ତୁ boxed;
}

image resize_max(image im, ପୂର୍ଣ୍ଣସଂଖ୍ୟା max)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା w = im.w;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା h = im.h;
    ଯଦି(w > h){
        h = (h * max) / w;
        w = max;
    } ଅନ୍ୟଥା {
        w = (w * max) / h;
        h = max;
    }
    ଯଦି(w == im.w && h == im.h) ଫେରନ୍ତୁ im;
    image resized = resize_image(im, w, h);
    ଫେରନ୍ତୁ resized;
}

image resize_min(image im, ପୂର୍ଣ୍ଣସଂଖ୍ୟା min)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା w = im.w;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା h = im.h;
    ଯଦି(w < h){
        h = (h * min) / w;
        w = min;
    } ଅନ୍ୟଥା {
        w = (w * min) / h;
        h = min;
    }
    ଯଦି(w == im.w && h == im.h) ଫେରନ୍ତୁ im;
    image resized = resize_image(im, w, h);
    ଫେରନ୍ତୁ resized;
}

image random_crop_image(image im, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା dx = rand_int(0, im.w - w);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା dy = rand_int(0, im.h - h);
    image crop = crop_image(im, dx, dy, w, h);
    ଫେରନ୍ତୁ crop;
}

augment_args random_augment_args(image im, ଦଶମିକ angle, ଦଶମିକ aspect, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ତଳ, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଉପର, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h)
{
    augment_args a = {0};
    aspect = rand_scale(aspect);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା r = rand_int(ତଳ, ଉପର);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା min = (im.h < im.w*aspect) ? im.h : im.w*aspect;
    ଦଶମିକ scale = (ଦଶମିକ)r / min;

    ଦଶମିକ rad = rand_uniform(-angle, angle) * TWO_PI / 360.;

    ଦଶମିକ dx = (im.w*scale/aspect - w) / 2.;
    ଦଶମିକ dy = (im.h*scale - w) / 2.;
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
    ଫେରନ୍ତୁ a;
}

image random_augment_image(image im, ଦଶମିକ angle, ଦଶମିକ aspect, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ତଳ, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଉପର, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h)
{
    augment_args a = random_augment_args(im, angle, aspect, ତଳ, ଉପର, w, h);
    image crop = rotate_crop_image(im, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);
    ଫେରନ୍ତୁ crop;
}

ଦଶମିକ three_way_max(ଦଶମିକ a, ଦଶମିକ b, ଦଶମିକ c)
{
    ଫେରନ୍ତୁ (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

ଦଶମିକ three_way_min(ଦଶମିକ a, ଦଶମିକ b, ଦଶମିକ c)
{
    ଫେରନ୍ତୁ (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

ଶୂନ୍ୟ yuv_to_rgb(image im)
{
    ପ୍ରତିଜ୍ଞା(im.c == 3);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j;
    ଦଶମିକ r, g, b;
    ଦଶମିକ y, u, v;
    ପାଇଁ(j = 0; j < im.h; ++j){
        ପାଇଁ(i = 0; i < im.w; ++i){
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

ଶୂନ୍ୟ rgb_to_yuv(image im)
{
    ପ୍ରତିଜ୍ଞା(im.c == 3);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j;
    ଦଶମିକ r, g, b;
    ଦଶମିକ y, u, v;
    ପାଇଁ(j = 0; j < im.h; ++j){
        ପାଇଁ(i = 0; i < im.w; ++i){
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
ଶୂନ୍ୟ rgb_to_hsv(image im)
{
    ପ୍ରତିଜ୍ଞା(im.c == 3);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j;
    ଦଶମିକ r, g, b;
    ଦଶମିକ h, s, v;
    ପାଇଁ(j = 0; j < im.h; ++j){
        ପାଇଁ(i = 0; i < im.w; ++i){
            r = get_pixel(im, i , j, 0);
            g = get_pixel(im, i , j, 1);
            b = get_pixel(im, i , j, 2);
            ଦଶମିକ max = three_way_max(r,g,b);
            ଦଶମିକ min = three_way_min(r,g,b);
            ଦଶମିକ delta = max - min;
            v = max;
            ଯଦି(max == 0){
                s = 0;
                h = 0;
            }ଅନ୍ୟଥା{
                s = delta/max;
                ଯଦି(r == max){
                    h = (g - b) / delta;
                } ଅନ୍ୟଥା ଯଦି (g == max) {
                    h = 2 + (b - r) / delta;
                } ଅନ୍ୟଥା {
                    h = 4 + (r - g) / delta;
                }
                ଯଦି (h < 0) h += 6;
                h = h/6.;
            }
            set_pixel(im, i, j, 0, h);
            set_pixel(im, i, j, 1, s);
            set_pixel(im, i, j, 2, v);
        }
    }
}

ଶୂନ୍ୟ hsv_to_rgb(image im)
{
    ପ୍ରତିଜ୍ଞା(im.c == 3);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j;
    ଦଶମିକ r, g, b;
    ଦଶମିକ h, s, v;
    ଦଶମିକ f, p, q, t;
    ପାଇଁ(j = 0; j < im.h; ++j){
        ପାଇଁ(i = 0; i < im.w; ++i){
            h = 6 * get_pixel(im, i , j, 0);
            s = get_pixel(im, i , j, 1);
            v = get_pixel(im, i , j, 2);
            ଯଦି (s == 0) {
                r = g = b = v;
            } ଅନ୍ୟଥା {
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = floor(h);
                f = h - index;
                p = v*(1-s);
                q = v*(1-s*f);
                t = v*(1-s*(1-f));
                ଯଦି(index == 0){
                    r = v; g = t; b = p;
                } ଅନ୍ୟଥା ଯଦି(index == 1){
                    r = q; g = v; b = p;
                } ଅନ୍ୟଥା ଯଦି(index == 2){
                    r = p; g = v; b = t;
                } ଅନ୍ୟଥା ଯଦି(index == 3){
                    r = p; g = q; b = v;
                } ଅନ୍ୟଥା ଯଦି(index == 4){
                    r = t; g = p; b = v;
                } ଅନ୍ୟଥା {
                    r = v; g = p; b = q;
                }
            }
            set_pixel(im, i, j, 0, r);
            set_pixel(im, i, j, 1, g);
            set_pixel(im, i, j, 2, b);
        }
    }
}

ଶୂନ୍ୟ grayscale_image_3c(image im)
{
    ପ୍ରତିଜ୍ଞା(im.c == 3);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j, k;
    ଦଶମିକ scale[] = {0.299, 0.587, 0.114};
    ପାଇଁ(j = 0; j < im.h; ++j){
        ପାଇଁ(i = 0; i < im.w; ++i){
            ଦଶମିକ val = 0;
            ପାଇଁ(k = 0; k < 3; ++k){
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
    ପ୍ରତିଜ୍ଞା(im.c == 3);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j, k;
    image gray = make_image(im.w, im.h, 1);
    ଦଶମିକ scale[] = {0.299, 0.587, 0.114};
    ପାଇଁ(k = 0; k < im.c; ++k){
        ପାଇଁ(j = 0; j < im.h; ++j){
            ପାଇଁ(i = 0; i < im.w; ++i){
                gray.data[i+im.w*j] += scale[k]*get_pixel(im, i, j, k);
            }
        }
    }
    ଫେରନ୍ତୁ gray;
}

image threshold_image(image im, ଦଶମିକ thresh)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    image t = make_image(im.w, im.h, im.c);
    ପାଇଁ(i = 0; i < im.w*im.h*im.c; ++i){
        t.data[i] = im.data[i]>thresh ? 1 : 0;
    }
    ଫେରନ୍ତୁ t;
}

image blend_image(image fore, image back, ଦଶମିକ alpha)
{
    ପ୍ରତିଜ୍ଞା(fore.w == back.w && fore.h == back.h && fore.c == back.c);
    image blend = make_image(fore.w, fore.h, fore.c);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j, k;
    ପାଇଁ(k = 0; k < fore.c; ++k){
        ପାଇଁ(j = 0; j < fore.h; ++j){
            ପାଇଁ(i = 0; i < fore.w; ++i){
                ଦଶମିକ val = alpha * get_pixel(fore, i, j, k) + 
                    (1 - alpha)* get_pixel(back, i, j, k);
                set_pixel(blend, i, j, k, val);
            }
        }
    }
    ଫେରନ୍ତୁ blend;
}

ଶୂନ୍ୟ scale_image_channel(image im, ପୂର୍ଣ୍ଣସଂଖ୍ୟା c, ଦଶମିକ v)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j;
    ପାଇଁ(j = 0; j < im.h; ++j){
        ପାଇଁ(i = 0; i < im.w; ++i){
            ଦଶମିକ pix = get_pixel(im, i, j, c);
            pix = pix*v;
            set_pixel(im, i, j, c, pix);
        }
    }
}

ଶୂନ୍ୟ translate_image_channel(image im, ପୂର୍ଣ୍ଣସଂଖ୍ୟା c, ଦଶମିକ v)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j;
    ପାଇଁ(j = 0; j < im.h; ++j){
        ପାଇଁ(i = 0; i < im.w; ++i){
            ଦଶମିକ pix = get_pixel(im, i, j, c);
            pix = pix+v;
            set_pixel(im, i, j, c, pix);
        }
    }
}

image binarize_image(image im)
{
    image c = copy_image(im);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < im.w * im.h * im.c; ++i){
        ଯଦି(c.data[i] > .5) c.data[i] = 1;
        ଅନ୍ୟଥା c.data[i] = 0;
    }
    ଫେରନ୍ତୁ c;
}

ଶୂନ୍ୟ saturate_image(image im, ଦଶମିକ sat)
{
    rgb_to_hsv(im);
    scale_image_channel(im, 1, sat);
    hsv_to_rgb(im);
    constrain_image(im);
}

ଶୂନ୍ୟ hue_image(image im, ଦଶମିକ hue)
{
    rgb_to_hsv(im);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < im.w*im.h; ++i){
        im.data[i] = im.data[i] + hue;
        ଯଦି (im.data[i] > 1) im.data[i] -= 1;
        ଯଦି (im.data[i] < 0) im.data[i] += 1;
    }
    hsv_to_rgb(im);
    constrain_image(im);
}

ଶୂନ୍ୟ exposure_image(image im, ଦଶମିକ sat)
{
    rgb_to_hsv(im);
    scale_image_channel(im, 2, sat);
    hsv_to_rgb(im);
    constrain_image(im);
}

ଶୂନ୍ୟ distort_image(image im, ଦଶମିକ hue, ଦଶମିକ sat, ଦଶମିକ val)
{
    rgb_to_hsv(im);
    scale_image_channel(im, 1, sat);
    scale_image_channel(im, 2, val);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < im.w*im.h; ++i){
        im.data[i] = im.data[i] + hue;
        ଯଦି (im.data[i] > 1) im.data[i] -= 1;
        ଯଦି (im.data[i] < 0) im.data[i] += 1;
    }
    hsv_to_rgb(im);
    constrain_image(im);
}

ଶୂନ୍ୟ random_distort_image(image im, ଦଶମିକ hue, ଦଶମିକ saturation, ଦଶମିକ exposure)
{
    ଦଶମିକ dhue = rand_uniform(-hue, hue);
    ଦଶମିକ dsat = rand_scale(saturation);
    ଦଶମିକ dexp = rand_scale(exposure);
    distort_image(im, dhue, dsat, dexp);
}

ଶୂନ୍ୟ saturate_exposure_image(image im, ଦଶମିକ sat, ଦଶମିକ exposure)
{
    rgb_to_hsv(im);
    scale_image_channel(im, 1, sat);
    scale_image_channel(im, 2, exposure);
    hsv_to_rgb(im);
    constrain_image(im);
}

image resize_image(image im, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h)
{
    image resized = make_image(w, h, im.c);   
    image part = make_image(w, im.h, im.c);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା r, c, k;
    ଦଶମିକ w_scale = (ଦଶମିକ)(im.w - 1) / (w - 1);
    ଦଶମିକ h_scale = (ଦଶମିକ)(im.h - 1) / (h - 1);
    ପାଇଁ(k = 0; k < im.c; ++k){
        ପାଇଁ(r = 0; r < im.h; ++r){
            ପାଇଁ(c = 0; c < w; ++c){
                ଦଶମିକ val = 0;
                ଯଦି(c == w-1 || im.w == 1){
                    val = get_pixel(im, im.w-1, r, k);
                } ଅନ୍ୟଥା {
                    ଦଶମିକ sx = c*w_scale;
                    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ix = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) sx;
                    ଦଶମିକ dx = sx - ix;
                    val = (1 - dx) * get_pixel(im, ix, r, k) + dx * get_pixel(im, ix+1, r, k);
                }
                set_pixel(part, c, r, k, val);
            }
        }
    }
    ପାଇଁ(k = 0; k < im.c; ++k){
        ପାଇଁ(r = 0; r < h; ++r){
            ଦଶମିକ sy = r*h_scale;
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା iy = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) sy;
            ଦଶମିକ dy = sy - iy;
            ପାଇଁ(c = 0; c < w; ++c){
                ଦଶମିକ val = (1-dy) * get_pixel(part, c, iy, k);
                set_pixel(resized, c, r, k, val);
            }
            ଯଦି(r == h-1 || im.h == 1) ଜାରି;
            ପାଇଁ(c = 0; c < w; ++c){
                ଦଶମିକ val = dy * get_pixel(part, c, iy+1, k);
                add_pixel(resized, c, r, k, val);
            }
        }
    }

    free_image(part);
    ଫେରନ୍ତୁ resized;
}


ଶୂନ୍ୟ test_resize(ଅକ୍ଷର *filename)
{
    image im = load_image(filename, 0,0, 3);
    ଦଶମିକ mag = mag_array(im.data, im.w*im.h*im.c);
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
    ଯେତେବେଳେ(1){
        image aug = random_augment_image(im, 0, .75, 320, 448, 320, 320);
        show_image(aug, "aug", 1);
        free_image(aug);


        ଦଶମିକ exposure = 1.15;
        ଦଶମିକ saturation = 1.15;
        ଦଶମିକ hue = .05;

        image c = copy_image(im);

        ଦଶମିକ dexp = rand_scale(exposure);
        ଦଶମିକ dsat = rand_scale(saturation);
        ଦଶମିକ dhue = rand_uniform(-hue, hue);

        distort_image(c, dhue, dsat, dexp);
        show_image(c, "rand", 1);
        printf("%f %f %f\n", dhue, dsat, dexp);
        free_image(c);
    }
#endif
}


image load_image_stb(ଅକ୍ଷର *filename, ପୂର୍ଣ୍ଣସଂଖ୍ୟା channels)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, h, c;
    unsigned ଅକ୍ଷର *data = stbi_load(filename, &w, &h, &c, channels);
    ଯଦି (!data) {
        fprintf(stderr, "Cannot load image \"%s\"\nSTB Reason: %s\n", filename, stbi_failure_reason());
        exit(0);
    }
    ଯଦି(channels) c = channels;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i,j,k;
    image im = make_image(w, h, c);
    ପାଇଁ(k = 0; k < c; ++k){
        ପାଇଁ(j = 0; j < h; ++j){
            ପାଇଁ(i = 0; i < w; ++i){
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା dst_index = i + w*j + w*h*k;
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା src_index = k + c*i + c*w*j;
                im.data[dst_index] = (ଦଶମିକ)data[src_index]/255.;
            }
        }
    }
    ମୁକ୍ତ(data);
    ଫେରନ୍ତୁ im;
}

image load_image(ଅକ୍ଷର *filename, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ପୂର୍ଣ୍ଣସଂଖ୍ୟା c)
{
#ifdef OPENCV
    image out = load_image_cv(filename, c);
#ଅନ୍ୟଥା
    image out = load_image_stb(filename, c);
#endif

    ଯଦି((h && w) && (h != out.h || w != out.w)){
        image resized = resize_image(out, w, h);
        free_image(out);
        out = resized;
    }
    ଫେରନ୍ତୁ out;
}

image load_image_color(ଅକ୍ଷର *filename, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h)
{
    ଫେରନ୍ତୁ load_image(filename, w, h, 3);
}

image get_image_layer(image m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା l)
{
    image out = make_image(m.w, m.h, 1);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < m.h*m.w; ++i){
        out.data[i] = m.data[i+l*m.h*m.w];
    }
    ଫେରନ୍ତୁ out;
}
ଶୂନ୍ୟ print_image(image m)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j, k;
    ପାଇଁ(i =0 ; i < m.c; ++i){
        ପାଇଁ(j =0 ; j < m.h; ++j){
            ପାଇଁ(k = 0; k < m.w; ++k){
                printf("%.2lf, ", m.data[i*m.h*m.w + j*m.w + k]);
                ଯଦି(k > 30) ଭାଙ୍ଗ;
            }
            printf("\n");
            ଯଦି(j > 30) ଭାଙ୍ଗ;
        }
        printf("\n");
    }
    printf("\n");
}

image collapse_images_vert(image *ims, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା color = 1;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା border = 1;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା h,w,c;
    w = ims[0].w;
    h = (ims[0].h + border) * n - border;
    c = ims[0].c;
    ଯଦି(c != 3 || !color){
        w = (w+border)*c - border;
        c = 1;
    }

    image filters = make_image(w, h, c);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i,j;
    ପାଇଁ(i = 0; i < n; ++i){
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା h_offset = i*(ims[0].h+border);
        image copy = copy_image(ims[i]);
        //normalize_image(copy);
        ଯଦି(c == 3 && color){
            embed_image(copy, filters, 0, h_offset);
        }
        ଅନ୍ୟଥା{
            ପାଇଁ(j = 0; j < copy.c; ++j){
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା w_offset = j*(ims[0].w+border);
                image layer = get_image_layer(copy, j);
                embed_image(layer, filters, w_offset, h_offset);
                free_image(layer);
            }
        }
        free_image(copy);
    }
    ଫେରନ୍ତୁ filters;
} 

image collapse_images_horz(image *ims, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା color = 1;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା border = 1;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା h,w,c;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା size = ims[0].h;
    h = size;
    w = (ims[0].w + border) * n - border;
    c = ims[0].c;
    ଯଦି(c != 3 || !color){
        h = (h+border)*c - border;
        c = 1;
    }

    image filters = make_image(w, h, c);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i,j;
    ପାଇଁ(i = 0; i < n; ++i){
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା w_offset = i*(size+border);
        image copy = copy_image(ims[i]);
        //normalize_image(copy);
        ଯଦି(c == 3 && color){
            embed_image(copy, filters, w_offset, 0);
        }
        ଅନ୍ୟଥା{
            ପାଇଁ(j = 0; j < copy.c; ++j){
                ପୂର୍ଣ୍ଣସଂଖ୍ୟା h_offset = j*(size+border);
                image layer = get_image_layer(copy, j);
                embed_image(layer, filters, w_offset, h_offset);
                free_image(layer);
            }
        }
        free_image(copy);
    }
    ଫେରନ୍ତୁ filters;
} 

ଶୂନ୍ୟ show_image_normalized(image im, ଅଚଳ ଅକ୍ଷର *name)
{
    image c = copy_image(im);
    normalize_image(c);
    show_image(c, name, 1);
    free_image(c);
}

ଶୂନ୍ୟ show_images(image *ims, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ଅକ୍ଷର *window)
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

ଶୂନ୍ୟ free_image(image m)
{
    ଯଦି(m.data){
        ମୁକ୍ତ(m.data);
    }
}
