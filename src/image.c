#شامل "image.h"
#شامل "utils.h"
#شامل "blas.h"
#شامل "cuda.h"
#شامل <stdio.h>
#شامل <math.h>

#define STB_IMAGE_IMPLEMENTATION
#شامل "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#شامل "stb_image_write.h"

صحیح windows = 0;

اعشاری colors[6][3] = { {1,0,1}, {0,0,1},{0,1,1},{0,1,0},{1,1,0},{1,0,0} };

اعشاری get_color(صحیح c, صحیح x, صحیح max)
{
    اعشاری ratio = ((اعشاری)x/max)*5;
    صحیح i = floor(ratio);
    صحیح j = ceil(ratio);
    ratio -= i;
    اعشاری r = (1-ratio) * colors[i][c] + ratio*colors[j][c];
    //printf("%f\n", r);
    بازگشت r;
}

image mask_to_rgb(image mask)
{
    صحیح n = mask.c;
    image im = make_image(mask.w, mask.h, 3);
    صحیح i, j;
    برای(j = 0; j < n; ++j){
        صحیح offset = j*123457 % n;
        اعشاری red = get_color(2,offset,n);
        اعشاری green = get_color(1,offset,n);
        اعشاری blue = get_color(0,offset,n);
        برای(i = 0; i < im.w*im.h; ++i){
            im.data[i + 0*im.w*im.h] += mask.data[j*im.h*im.w + i]*red;
            im.data[i + 1*im.w*im.h] += mask.data[j*im.h*im.w + i]*green;
            im.data[i + 2*im.w*im.h] += mask.data[j*im.h*im.w + i]*blue;
        }
    }
    بازگشت im;
}

ایستا اعشاری get_pixel(image m, صحیح x, صحیح y, صحیح c)
{
    أكد(x < m.w && y < m.h && c < m.c);
    بازگشت m.data[c*m.h*m.w + y*m.w + x];
}
ایستا اعشاری get_pixel_extend(image m, صحیح x, صحیح y, صحیح c)
{
    اگر(x < 0 || x >= m.w || y < 0 || y >= m.h) بازگشت 0;
    /*
    if(x < 0) x = 0;
    if(x >= m.w) x = m.w-1;
    if(y < 0) y = 0;
    if(y >= m.h) y = m.h-1;
    */
    اگر(c < 0 || c >= m.c) بازگشت 0;
    بازگشت get_pixel(m, x, y, c);
}
ایستا تهی set_pixel(image m, صحیح x, صحیح y, صحیح c, اعشاری val)
{
    اگر (x < 0 || y < 0 || c < 0 || x >= m.w || y >= m.h || c >= m.c) بازگشت;
    أكد(x < m.w && y < m.h && c < m.c);
    m.data[c*m.h*m.w + y*m.w + x] = val;
}
ایستا تهی add_pixel(image m, صحیح x, صحیح y, صحیح c, اعشاری val)
{
    أكد(x < m.w && y < m.h && c < m.c);
    m.data[c*m.h*m.w + y*m.w + x] += val;
}

ایستا اعشاری bilinear_interpolate(image im, اعشاری x, اعشاری y, صحیح c)
{
    صحیح ix = (صحیح) floorf(x);
    صحیح iy = (صحیح) floorf(y);

    اعشاری dx = x - ix;
    اعشاری dy = y - iy;

    اعشاری val = (1-dy) * (1-dx) * get_pixel_extend(im, ix, iy, c) + 
        dy     * (1-dx) * get_pixel_extend(im, ix, iy+1, c) + 
        (1-dy) *   dx   * get_pixel_extend(im, ix+1, iy, c) +
        dy     *   dx   * get_pixel_extend(im, ix+1, iy+1, c);
    بازگشت val;
}


تهی composite_image(image source, image dest, صحیح dx, صحیح dy)
{
    صحیح x,y,k;
    برای(k = 0; k < source.c; ++k){
        برای(y = 0; y < source.h; ++y){
            برای(x = 0; x < source.w; ++x){
                اعشاری val = get_pixel(source, x, y, k);
                اعشاری val2 = get_pixel_extend(dest, dx+x, dy+y, k);
                set_pixel(dest, dx+x, dy+y, k, val * val2);
            }
        }
    }
}

image border_image(image a, صحیح border)
{
    image b = make_image(a.w + 2*border, a.h + 2*border, a.c);
    صحیح x,y,k;
    برای(k = 0; k < b.c; ++k){
        برای(y = 0; y < b.h; ++y){
            برای(x = 0; x < b.w; ++x){
                اعشاری val = get_pixel_extend(a, x - border, y - border, k);
                اگر(x - border < 0 || x - border >= a.w || y - border < 0 || y - border >= a.h) val = 1;
                set_pixel(b, x, y, k, val);
            }
        }
    }
    بازگشت b;
}

image tile_images(image a, image b, صحیح dx)
{
    اگر(a.w == 0) بازگشت copy_image(b);
    image c = make_image(a.w + b.w + dx, (a.h > b.h) ? a.h : b.h, (a.c > b.c) ? a.c : b.c);
    fill_cpu(c.w*c.h*c.c, 1, c.data, 1);
    embed_image(a, c, 0, 0); 
    composite_image(b, c, a.w + dx, 0);
    بازگشت c;
}

image get_label(image **characters, نویسه *string, صحیح size)
{
    size = size/10;
    اگر(size > 7) size = 7;
    image label = make_empty_image(0,0,0);
    وقتی(*string){
        image l = characters[size][(صحیح)*string];
        image n = tile_images(label, l, -size - 1 + (size+1)/2);
        free_image(label);
        label = n;
        ++string;
    }
    image b = border_image(label, label.h*.25);
    free_image(label);
    بازگشت b;
}

تهی draw_label(image a, صحیح r, صحیح c, image label, ثابت اعشاری *rgb)
{
    صحیح w = label.w;
    صحیح h = label.h;
    اگر (r - h >= 0) r = r - h;

    صحیح i, j, k;
    برای(j = 0; j < h && j + r < a.h; ++j){
        برای(i = 0; i < w && i + c < a.w; ++i){
            برای(k = 0; k < label.c; ++k){
                اعشاری val = get_pixel(label, i, j, k);
                set_pixel(a, i+c, j+r, k, rgb[k] * val);
            }
        }
    }
}

تهی draw_box(image a, صحیح x1, صحیح y1, صحیح x2, صحیح y2, اعشاری r, اعشاری g, اعشاری b)
{
    //normalize_image(a);
    صحیح i;
    اگر(x1 < 0) x1 = 0;
    اگر(x1 >= a.w) x1 = a.w-1;
    اگر(x2 < 0) x2 = 0;
    اگر(x2 >= a.w) x2 = a.w-1;

    اگر(y1 < 0) y1 = 0;
    اگر(y1 >= a.h) y1 = a.h-1;
    اگر(y2 < 0) y2 = 0;
    اگر(y2 >= a.h) y2 = a.h-1;

    برای(i = x1; i <= x2; ++i){
        a.data[i + y1*a.w + 0*a.w*a.h] = r;
        a.data[i + y2*a.w + 0*a.w*a.h] = r;

        a.data[i + y1*a.w + 1*a.w*a.h] = g;
        a.data[i + y2*a.w + 1*a.w*a.h] = g;

        a.data[i + y1*a.w + 2*a.w*a.h] = b;
        a.data[i + y2*a.w + 2*a.w*a.h] = b;
    }
    برای(i = y1; i <= y2; ++i){
        a.data[x1 + i*a.w + 0*a.w*a.h] = r;
        a.data[x2 + i*a.w + 0*a.w*a.h] = r;

        a.data[x1 + i*a.w + 1*a.w*a.h] = g;
        a.data[x2 + i*a.w + 1*a.w*a.h] = g;

        a.data[x1 + i*a.w + 2*a.w*a.h] = b;
        a.data[x2 + i*a.w + 2*a.w*a.h] = b;
    }
}

تهی draw_box_width(image a, صحیح x1, صحیح y1, صحیح x2, صحیح y2, صحیح w, اعشاری r, اعشاری g, اعشاری b)
{
    صحیح i;
    برای(i = 0; i < w; ++i){
        draw_box(a, x1+i, y1+i, x2-i, y2-i, r, g, b);
    }
}

تهی draw_bbox(image a, box bbox, صحیح w, اعشاری r, اعشاری g, اعشاری b)
{
    صحیح چپ  = (bbox.x-bbox.w/2)*a.w;
    صحیح راست = (bbox.x+bbox.w/2)*a.w;
    صحیح بالاترین   = (bbox.y-bbox.h/2)*a.h;
    صحیح bot   = (bbox.y+bbox.h/2)*a.h;

    صحیح i;
    برای(i = 0; i < w; ++i){
        draw_box(a, چپ+i, بالاترین+i, راست-i, bot-i, r, g, b);
    }
}

image **load_alphabet()
{
    صحیح i, j;
    ثابت صحیح nsize = 8;
    image **alphabets = calloc(nsize, اندازه(image));
    برای(j = 0; j < nsize; ++j){
        alphabets[j] = calloc(128, اندازه(image));
        برای(i = 32; i < 127; ++i){
            نویسه buff[256];
            sprintf(buff, "data/labels/%d_%d.png", i, j);
            alphabets[j][i] = load_image_color(buff, 0, 0);
        }
    }
    بازگشت alphabets;
}

تهی draw_detections(image im, detection *dets, صحیح رقم, اعشاری thresh, نویسه **names, image **alphabet, صحیح classes)
{
    صحیح i,j;

    برای(i = 0; i < رقم; ++i){
        نویسه labelstr[4096] = {0};
        صحیح class = -1;
        برای(j = 0; j < classes; ++j){
            اگر (dets[i].prob[j] > thresh){
                اگر (class < 0) {
                    strcat(labelstr, names[j]);
                    class = j;
                } وگرنه {
                    strcat(labelstr, ", ");
                    strcat(labelstr, names[j]);
                }
                printf("%s: %.0f%%\n", names[j], dets[i].prob[j]*100);
            }
        }
        اگر(class >= 0){
            صحیح width = im.h * .006;

            /*
               if(0){
               width = pow(prob, 1./2.)*10+1;
               alphabet = 0;
               }
             */

            //printf("%d %s: %.0f%%\n", i, names[class], prob*100);
            صحیح offset = class*123457 % classes;
            اعشاری red = get_color(2,offset,classes);
            اعشاری green = get_color(1,offset,classes);
            اعشاری blue = get_color(0,offset,classes);
            اعشاری rgb[3];

            //width = prob*20+2;

            rgb[0] = red;
            rgb[1] = green;
            rgb[2] = blue;
            box b = dets[i].bbox;
            //printf("%f %f %f %f\n", b.x, b.y, b.w, b.h);

            صحیح چپ  = (b.x-b.w/2.)*im.w;
            صحیح راست = (b.x+b.w/2.)*im.w;
            صحیح بالاترین   = (b.y-b.h/2.)*im.h;
            صحیح bot   = (b.y+b.h/2.)*im.h;

            اگر(چپ < 0) چپ = 0;
            اگر(راست > im.w-1) راست = im.w-1;
            اگر(بالاترین < 0) بالاترین = 0;
            اگر(bot > im.h-1) bot = im.h-1;

            draw_box_width(im, چپ, بالاترین, راست, bot, width, red, green, blue);
            اگر (alphabet) {
                image label = get_label(alphabet, labelstr, (im.h*.03));
                draw_label(im, بالاترین + width, چپ, label, rgb);
                free_image(label);
            }
            اگر (dets[i].mask){
                image mask = float_to_image(14, 14, 1, dets[i].mask);
                image resized_mask = resize_image(mask, b.w*im.w, b.h*im.h);
                image tmask = threshold_image(resized_mask, .5);
                embed_image(tmask, im, چپ, بالاترین);
                free_image(mask);
                free_image(resized_mask);
                free_image(tmask);
            }
        }
    }
}

تهی transpose_image(image im)
{
    أكد(im.w == im.h);
    صحیح n, m;
    صحیح c;
    برای(c = 0; c < im.c; ++c){
        برای(n = 0; n < im.w-1; ++n){
            برای(m = n + 1; m < im.w; ++m){
                اعشاری swap = im.data[m + im.w*(n + im.h*c)];
                im.data[m + im.w*(n + im.h*c)] = im.data[n + im.w*(m + im.h*c)];
                im.data[n + im.w*(m + im.h*c)] = swap;
            }
        }
    }
}

تهی rotate_image_cw(image im, صحیح times)
{
    أكد(im.w == im.h);
    times = (times + 400) % 4;
    صحیح i, x, y, c;
    صحیح n = im.w;
    برای(i = 0; i < times; ++i){
        برای(c = 0; c < im.c; ++c){
            برای(x = 0; x < n/2; ++x){
                برای(y = 0; y < (n-1)/2 + 1; ++y){
                    اعشاری temp = im.data[y + im.w*(x + im.h*c)];
                    im.data[y + im.w*(x + im.h*c)] = im.data[n-1-x + im.w*(y + im.h*c)];
                    im.data[n-1-x + im.w*(y + im.h*c)] = im.data[n-1-y + im.w*(n-1-x + im.h*c)];
                    im.data[n-1-y + im.w*(n-1-x + im.h*c)] = im.data[x + im.w*(n-1-y + im.h*c)];
                    im.data[x + im.w*(n-1-y + im.h*c)] = temp;
                }
            }
        }
    }
}

تهی flip_image(image a)
{
    صحیح i,j,k;
    برای(k = 0; k < a.c; ++k){
        برای(i = 0; i < a.h; ++i){
            برای(j = 0; j < a.w/2; ++j){
                صحیح index = j + a.w*(i + a.h*(k));
                صحیح flip = (a.w - j - 1) + a.w*(i + a.h*(k));
                اعشاری swap = a.data[flip];
                a.data[flip] = a.data[index];
                a.data[index] = swap;
            }
        }
    }
}

image image_distance(image a, image b)
{
    صحیح i,j;
    image dist = make_image(a.w, a.h, 1);
    برای(i = 0; i < a.c; ++i){
        برای(j = 0; j < a.h*a.w; ++j){
            dist.data[j] += pow(a.data[i*a.h*a.w+j]-b.data[i*a.h*a.w+j],2);
        }
    }
    برای(j = 0; j < a.h*a.w; ++j){
        dist.data[j] = sqrt(dist.data[j]);
    }
    بازگشت dist;
}

تهی ghost_image(image source, image dest, صحیح dx, صحیح dy)
{
    صحیح x,y,k;
    اعشاری max_dist = sqrt((-source.w/2. + .5)*(-source.w/2. + .5));
    برای(k = 0; k < source.c; ++k){
        برای(y = 0; y < source.h; ++y){
            برای(x = 0; x < source.w; ++x){
                اعشاری dist = sqrt((x - source.w/2. + .5)*(x - source.w/2. + .5) + (y - source.h/2. + .5)*(y - source.h/2. + .5));
                اعشاری alpha = (1 - dist/max_dist);
                اگر(alpha < 0) alpha = 0;
                اعشاری v1 = get_pixel(source, x,y,k);
                اعشاری v2 = get_pixel(dest, dx+x,dy+y,k);
                اعشاری val = alpha*v1 + (1-alpha)*v2;
                set_pixel(dest, dx+x, dy+y, k, val);
            }
        }
    }
}

تهی blocky_image(image im, صحیح s)
{
    صحیح i,j,k;
    برای(k = 0; k < im.c; ++k){
        برای(j = 0; j < im.h; ++j){
            برای(i = 0; i < im.w; ++i){
                im.data[i + im.w*(j + im.h*k)] = im.data[i/s*s + im.w*(j/s*s + im.h*k)];
            }
        }
    }
}

تهی censor_image(image im, صحیح dx, صحیح dy, صحیح w, صحیح h)
{
    صحیح i,j,k;
    صحیح s = 32;
    اگر(dx < 0) dx = 0;
    اگر(dy < 0) dy = 0;

    برای(k = 0; k < im.c; ++k){
        برای(j = dy; j < dy + h && j < im.h; ++j){
            برای(i = dx; i < dx + w && i < im.w; ++i){
                im.data[i + im.w*(j + im.h*k)] = im.data[i/s*s + im.w*(j/s*s + im.h*k)];
                //im.data[i + j*im.w + k*im.w*im.h] = 0;
            }
        }
    }
}

تهی embed_image(image source, image dest, صحیح dx, صحیح dy)
{
    صحیح x,y,k;
    برای(k = 0; k < source.c; ++k){
        برای(y = 0; y < source.h; ++y){
            برای(x = 0; x < source.w; ++x){
                اعشاری val = get_pixel(source, x,y,k);
                set_pixel(dest, dx+x, dy+y, k, val);
            }
        }
    }
}

image collapse_image_layers(image source, صحیح border)
{
    صحیح h = source.h;
    h = (h+border)*source.c - border;
    image dest = make_image(source.w, h, 1);
    صحیح i;
    برای(i = 0; i < source.c; ++i){
        image layer = get_image_layer(source, i);
        صحیح h_offset = i*(source.h+border);
        embed_image(layer, dest, 0, h_offset);
        free_image(layer);
    }
    بازگشت dest;
}

تهی constrain_image(image im)
{
    صحیح i;
    برای(i = 0; i < im.w*im.h*im.c; ++i){
        اگر(im.data[i] < 0) im.data[i] = 0;
        اگر(im.data[i] > 1) im.data[i] = 1;
    }
}

تهی normalize_image(image p)
{
    صحیح i;
    اعشاری min = 9999999;
    اعشاری max = -999999;

    برای(i = 0; i < p.h*p.w*p.c; ++i){
        اعشاری v = p.data[i];
        اگر(v < min) min = v;
        اگر(v > max) max = v;
    }
    اگر(max - min < .000000001){
        min = 0;
        max = 1;
    }
    برای(i = 0; i < p.c*p.w*p.h; ++i){
        p.data[i] = (p.data[i] - min)/(max-min);
    }
}

تهی normalize_image2(image p)
{
    اعشاری *min = calloc(p.c, اندازه(اعشاری));
    اعشاری *max = calloc(p.c, اندازه(اعشاری));
    صحیح i,j;
    برای(i = 0; i < p.c; ++i) min[i] = max[i] = p.data[i*p.h*p.w];

    برای(j = 0; j < p.c; ++j){
        برای(i = 0; i < p.h*p.w; ++i){
            اعشاری v = p.data[i+j*p.h*p.w];
            اگر(v < min[j]) min[j] = v;
            اگر(v > max[j]) max[j] = v;
        }
    }
    برای(i = 0; i < p.c; ++i){
        اگر(max[i] - min[i] < .000000001){
            min[i] = 0;
            max[i] = 1;
        }
    }
    برای(j = 0; j < p.c; ++j){
        برای(i = 0; i < p.w*p.h; ++i){
            p.data[i+j*p.h*p.w] = (p.data[i+j*p.h*p.w] - min[j])/(max[j]-min[j]);
        }
    }
    آزاد(min);
    آزاد(max);
}

تهی copy_image_into(image src, image dest)
{
    memcpy(dest.data, src.data, src.h*src.w*src.c*اندازه(اعشاری));
}

image copy_image(image p)
{
    image copy = p;
    copy.data = calloc(p.h*p.w*p.c, اندازه(اعشاری));
    memcpy(copy.data, p.data, p.h*p.w*p.c*اندازه(اعشاری));
    بازگشت copy;
}

تهی rgbgr_image(image im)
{
    صحیح i;
    برای(i = 0; i < im.w*im.h; ++i){
        اعشاری swap = im.data[i];
        im.data[i] = im.data[i+im.w*im.h*2];
        im.data[i+im.w*im.h*2] = swap;
    }
}

صحیح show_image(image p, ثابت نویسه *name, صحیح ms)
{
#ifdef OPENCV
    صحیح c = show_image_cv(p, name, ms);
    بازگشت c;
#وگرنه
    fprintf(stderr, "Not compiled with OpenCV, saving to %s.png instead\n", name);
    save_image(p, name);
    بازگشت -1;
#endif
}

تهی save_image_options(image im, ثابت نویسه *name, IMTYPE f, صحیح quality)
{
    نویسه buff[256];
    //sprintf(buff, "%s (%d)", name, windows);
    اگر(f == PNG)       sprintf(buff, "%s.png", name);
    وگرنه اگر (f == BMP) sprintf(buff, "%s.bmp", name);
    وگرنه اگر (f == TGA) sprintf(buff, "%s.tga", name);
    وگرنه اگر (f == JPG) sprintf(buff, "%s.jpg", name);
    وگرنه               sprintf(buff, "%s.png", name);
    unsigned نویسه *data = calloc(im.w*im.h*im.c, اندازه(نویسه));
    صحیح i,k;
    برای(k = 0; k < im.c; ++k){
        برای(i = 0; i < im.w*im.h; ++i){
            data[i*im.c+k] = (unsigned نویسه) (255*im.data[i + k*im.w*im.h]);
        }
    }
    صحیح success = 0;
    اگر(f == PNG)       success = stbi_write_png(buff, im.w, im.h, im.c, data, im.w*im.c);
    وگرنه اگر (f == BMP) success = stbi_write_bmp(buff, im.w, im.h, im.c, data);
    وگرنه اگر (f == TGA) success = stbi_write_tga(buff, im.w, im.h, im.c, data);
    وگرنه اگر (f == JPG) success = stbi_write_jpg(buff, im.w, im.h, im.c, data, quality);
    آزاد(data);
    اگر(!success) fprintf(stderr, "Failed to write image %s\n", buff);
}

تهی save_image(image im, ثابت نویسه *name)
{
    save_image_options(im, name, JPG, 80);
}

تهی show_image_layers(image p, نویسه *name)
{
    صحیح i;
    نویسه buff[256];
    برای(i = 0; i < p.c; ++i){
        sprintf(buff, "%s - Layer %d", name, i);
        image layer = get_image_layer(p, i);
        show_image(layer, buff, 1);
        free_image(layer);
    }
}

تهی show_image_collapsed(image p, نویسه *name)
{
    image c = collapse_image_layers(p, 1);
    show_image(c, name, 1);
    free_image(c);
}

image make_empty_image(صحیح w, صحیح h, صحیح c)
{
    image out;
    out.data = 0;
    out.h = h;
    out.w = w;
    out.c = c;
    بازگشت out;
}

image make_image(صحیح w, صحیح h, صحیح c)
{
    image out = make_empty_image(w,h,c);
    out.data = calloc(h*w*c, اندازه(اعشاری));
    بازگشت out;
}

image make_random_image(صحیح w, صحیح h, صحیح c)
{
    image out = make_empty_image(w,h,c);
    out.data = calloc(h*w*c, اندازه(اعشاری));
    صحیح i;
    برای(i = 0; i < w*h*c; ++i){
        out.data[i] = (rand_normal() * .25) + .5;
    }
    بازگشت out;
}

image float_to_image(صحیح w, صحیح h, صحیح c, اعشاری *data)
{
    image out = make_empty_image(w,h,c);
    out.data = data;
    بازگشت out;
}

تهی place_image(image im, صحیح w, صحیح h, صحیح dx, صحیح dy, image canvas)
{
    صحیح x, y, c;
    برای(c = 0; c < im.c; ++c){
        برای(y = 0; y < h; ++y){
            برای(x = 0; x < w; ++x){
                اعشاری rx = ((اعشاری)x / w) * im.w;
                اعشاری ry = ((اعشاری)y / h) * im.h;
                اعشاری val = bilinear_interpolate(im, rx, ry, c);
                set_pixel(canvas, x + dx, y + dy, c, val);
            }
        }
    }
}

image center_crop_image(image im, صحیح w, صحیح h)
{
    صحیح m = (im.w < im.h) ? im.w : im.h;   
    image c = crop_image(im, (im.w - m) / 2, (im.h - m)/2, m, m);
    image r = resize_image(c, w, h);
    free_image(c);
    بازگشت r;
}

image rotate_crop_image(image im, اعشاری rad, اعشاری s, صحیح w, صحیح h, اعشاری dx, اعشاری dy, اعشاری aspect)
{
    صحیح x, y, c;
    اعشاری cx = im.w/2.;
    اعشاری cy = im.h/2.;
    image rot = make_image(w, h, im.c);
    برای(c = 0; c < im.c; ++c){
        برای(y = 0; y < h; ++y){
            برای(x = 0; x < w; ++x){
                اعشاری rx = cos(rad)*((x - w/2.)/s*aspect + dx/s*aspect) - sin(rad)*((y - h/2.)/s + dy/s) + cx;
                اعشاری ry = sin(rad)*((x - w/2.)/s*aspect + dx/s*aspect) + cos(rad)*((y - h/2.)/s + dy/s) + cy;
                اعشاری val = bilinear_interpolate(im, rx, ry, c);
                set_pixel(rot, x, y, c, val);
            }
        }
    }
    بازگشت rot;
}

image rotate_image(image im, اعشاری rad)
{
    صحیح x, y, c;
    اعشاری cx = im.w/2.;
    اعشاری cy = im.h/2.;
    image rot = make_image(im.w, im.h, im.c);
    برای(c = 0; c < im.c; ++c){
        برای(y = 0; y < im.h; ++y){
            برای(x = 0; x < im.w; ++x){
                اعشاری rx = cos(rad)*(x-cx) - sin(rad)*(y-cy) + cx;
                اعشاری ry = sin(rad)*(x-cx) + cos(rad)*(y-cy) + cy;
                اعشاری val = bilinear_interpolate(im, rx, ry, c);
                set_pixel(rot, x, y, c, val);
            }
        }
    }
    بازگشت rot;
}

تهی fill_image(image m, اعشاری s)
{
    صحیح i;
    برای(i = 0; i < m.h*m.w*m.c; ++i) m.data[i] = s;
}

تهی translate_image(image m, اعشاری s)
{
    صحیح i;
    برای(i = 0; i < m.h*m.w*m.c; ++i) m.data[i] += s;
}

تهی scale_image(image m, اعشاری s)
{
    صحیح i;
    برای(i = 0; i < m.h*m.w*m.c; ++i) m.data[i] *= s;
}

image crop_image(image im, صحیح dx, صحیح dy, صحیح w, صحیح h)
{
    image cropped = make_image(w, h, im.c);
    صحیح i, j, k;
    برای(k = 0; k < im.c; ++k){
        برای(j = 0; j < h; ++j){
            برای(i = 0; i < w; ++i){
                صحیح r = j + dy;
                صحیح c = i + dx;
                اعشاری val = 0;
                r = constrain_int(r, 0, im.h-1);
                c = constrain_int(c, 0, im.w-1);
                val = get_pixel(im, c, r, k);
                set_pixel(cropped, i, j, k, val);
            }
        }
    }
    بازگشت cropped;
}

صحیح best_3d_shift_r(image a, image b, صحیح min, صحیح max)
{
    اگر(min == max) بازگشت min;
    صحیح mid = floor((min + max) / 2.);
    image c1 = crop_image(b, 0, mid, b.w, b.h);
    image c2 = crop_image(b, 0, mid+1, b.w, b.h);
    اعشاری d1 = dist_array(c1.data, a.data, a.w*a.h*a.c, 10);
    اعشاری d2 = dist_array(c2.data, a.data, a.w*a.h*a.c, 10);
    free_image(c1);
    free_image(c2);
    اگر(d1 < d2) بازگشت best_3d_shift_r(a, b, min, mid);
    وگرنه بازگشت best_3d_shift_r(a, b, mid+1, max);
}

صحیح best_3d_shift(image a, image b, صحیح min, صحیح max)
{
    صحیح i;
    صحیح best = 0;
    اعشاری best_distance = FLT_MAX;
    برای(i = min; i <= max; i += 2){
        image c = crop_image(b, 0, i, b.w, b.h);
        اعشاری d = dist_array(c.data, a.data, a.w*a.h*a.c, 100);
        اگر(d < best_distance){
            best_distance = d;
            best = i;
        }
        printf("%d %f\n", i, d);
        free_image(c);
    }
    بازگشت best;
}

تهی composite_3d(نویسه *f1, نویسه *f2, نویسه *out, صحیح delta)
{
    اگر(!out) out = "out";
    image a = load_image(f1, 0,0,0);
    image b = load_image(f2, 0,0,0);
    صحیح shift = best_3d_shift_r(a, b, -a.h/100, a.h/100);

    image c1 = crop_image(b, 10, shift, b.w, b.h);
    اعشاری d1 = dist_array(c1.data, a.data, a.w*a.h*a.c, 100);
    image c2 = crop_image(b, -10, shift, b.w, b.h);
    اعشاری d2 = dist_array(c2.data, a.data, a.w*a.h*a.c, 100);

    اگر(d2 < d1 && 0){
        image swap = a;
        a = b;
        b = swap;
        shift = -shift;
        printf("swapped, %d\n", shift);
    }
    وگرنه{
        printf("%d\n", shift);
    }

    image c = crop_image(b, delta, shift, a.w, a.h);
    صحیح i;
    برای(i = 0; i < c.w*c.h; ++i){
        c.data[i] = a.data[i];
    }
    save_image(c, out);
}

تهی letterbox_image_into(image im, صحیح w, صحیح h, image boxed)
{
    صحیح new_w = im.w;
    صحیح new_h = im.h;
    اگر (((اعشاری)w/im.w) < ((اعشاری)h/im.h)) {
        new_w = w;
        new_h = (im.h * w)/im.w;
    } وگرنه {
        new_h = h;
        new_w = (im.w * h)/im.h;
    }
    image resized = resize_image(im, new_w, new_h);
    embed_image(resized, boxed, (w-new_w)/2, (h-new_h)/2); 
    free_image(resized);
}

image letterbox_image(image im, صحیح w, صحیح h)
{
    صحیح new_w = im.w;
    صحیح new_h = im.h;
    اگر (((اعشاری)w/im.w) < ((اعشاری)h/im.h)) {
        new_w = w;
        new_h = (im.h * w)/im.w;
    } وگرنه {
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
    بازگشت boxed;
}

image resize_max(image im, صحیح max)
{
    صحیح w = im.w;
    صحیح h = im.h;
    اگر(w > h){
        h = (h * max) / w;
        w = max;
    } وگرنه {
        w = (w * max) / h;
        h = max;
    }
    اگر(w == im.w && h == im.h) بازگشت im;
    image resized = resize_image(im, w, h);
    بازگشت resized;
}

image resize_min(image im, صحیح min)
{
    صحیح w = im.w;
    صحیح h = im.h;
    اگر(w < h){
        h = (h * min) / w;
        w = min;
    } وگرنه {
        w = (w * min) / h;
        h = min;
    }
    اگر(w == im.w && h == im.h) بازگشت im;
    image resized = resize_image(im, w, h);
    بازگشت resized;
}

image random_crop_image(image im, صحیح w, صحیح h)
{
    صحیح dx = rand_int(0, im.w - w);
    صحیح dy = rand_int(0, im.h - h);
    image crop = crop_image(im, dx, dy, w, h);
    بازگشت crop;
}

augment_args random_augment_args(image im, اعشاری angle, اعشاری aspect, صحیح کم, صحیح بالا, صحیح w, صحیح h)
{
    augment_args a = {0};
    aspect = rand_scale(aspect);
    صحیح r = rand_int(کم, بالا);
    صحیح min = (im.h < im.w*aspect) ? im.h : im.w*aspect;
    اعشاری scale = (اعشاری)r / min;

    اعشاری rad = rand_uniform(-angle, angle) * TWO_PI / 360.;

    اعشاری dx = (im.w*scale/aspect - w) / 2.;
    اعشاری dy = (im.h*scale - w) / 2.;
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
    بازگشت a;
}

image random_augment_image(image im, اعشاری angle, اعشاری aspect, صحیح کم, صحیح بالا, صحیح w, صحیح h)
{
    augment_args a = random_augment_args(im, angle, aspect, کم, بالا, w, h);
    image crop = rotate_crop_image(im, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);
    بازگشت crop;
}

اعشاری three_way_max(اعشاری a, اعشاری b, اعشاری c)
{
    بازگشت (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

اعشاری three_way_min(اعشاری a, اعشاری b, اعشاری c)
{
    بازگشت (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

تهی yuv_to_rgb(image im)
{
    أكد(im.c == 3);
    صحیح i, j;
    اعشاری r, g, b;
    اعشاری y, u, v;
    برای(j = 0; j < im.h; ++j){
        برای(i = 0; i < im.w; ++i){
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

تهی rgb_to_yuv(image im)
{
    أكد(im.c == 3);
    صحیح i, j;
    اعشاری r, g, b;
    اعشاری y, u, v;
    برای(j = 0; j < im.h; ++j){
        برای(i = 0; i < im.w; ++i){
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
تهی rgb_to_hsv(image im)
{
    أكد(im.c == 3);
    صحیح i, j;
    اعشاری r, g, b;
    اعشاری h, s, v;
    برای(j = 0; j < im.h; ++j){
        برای(i = 0; i < im.w; ++i){
            r = get_pixel(im, i , j, 0);
            g = get_pixel(im, i , j, 1);
            b = get_pixel(im, i , j, 2);
            اعشاری max = three_way_max(r,g,b);
            اعشاری min = three_way_min(r,g,b);
            اعشاری delta = max - min;
            v = max;
            اگر(max == 0){
                s = 0;
                h = 0;
            }وگرنه{
                s = delta/max;
                اگر(r == max){
                    h = (g - b) / delta;
                } وگرنه اگر (g == max) {
                    h = 2 + (b - r) / delta;
                } وگرنه {
                    h = 4 + (r - g) / delta;
                }
                اگر (h < 0) h += 6;
                h = h/6.;
            }
            set_pixel(im, i, j, 0, h);
            set_pixel(im, i, j, 1, s);
            set_pixel(im, i, j, 2, v);
        }
    }
}

تهی hsv_to_rgb(image im)
{
    أكد(im.c == 3);
    صحیح i, j;
    اعشاری r, g, b;
    اعشاری h, s, v;
    اعشاری f, p, q, t;
    برای(j = 0; j < im.h; ++j){
        برای(i = 0; i < im.w; ++i){
            h = 6 * get_pixel(im, i , j, 0);
            s = get_pixel(im, i , j, 1);
            v = get_pixel(im, i , j, 2);
            اگر (s == 0) {
                r = g = b = v;
            } وگرنه {
                صحیح index = floor(h);
                f = h - index;
                p = v*(1-s);
                q = v*(1-s*f);
                t = v*(1-s*(1-f));
                اگر(index == 0){
                    r = v; g = t; b = p;
                } وگرنه اگر(index == 1){
                    r = q; g = v; b = p;
                } وگرنه اگر(index == 2){
                    r = p; g = v; b = t;
                } وگرنه اگر(index == 3){
                    r = p; g = q; b = v;
                } وگرنه اگر(index == 4){
                    r = t; g = p; b = v;
                } وگرنه {
                    r = v; g = p; b = q;
                }
            }
            set_pixel(im, i, j, 0, r);
            set_pixel(im, i, j, 1, g);
            set_pixel(im, i, j, 2, b);
        }
    }
}

تهی grayscale_image_3c(image im)
{
    أكد(im.c == 3);
    صحیح i, j, k;
    اعشاری scale[] = {0.299, 0.587, 0.114};
    برای(j = 0; j < im.h; ++j){
        برای(i = 0; i < im.w; ++i){
            اعشاری val = 0;
            برای(k = 0; k < 3; ++k){
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
    أكد(im.c == 3);
    صحیح i, j, k;
    image gray = make_image(im.w, im.h, 1);
    اعشاری scale[] = {0.299, 0.587, 0.114};
    برای(k = 0; k < im.c; ++k){
        برای(j = 0; j < im.h; ++j){
            برای(i = 0; i < im.w; ++i){
                gray.data[i+im.w*j] += scale[k]*get_pixel(im, i, j, k);
            }
        }
    }
    بازگشت gray;
}

image threshold_image(image im, اعشاری thresh)
{
    صحیح i;
    image t = make_image(im.w, im.h, im.c);
    برای(i = 0; i < im.w*im.h*im.c; ++i){
        t.data[i] = im.data[i]>thresh ? 1 : 0;
    }
    بازگشت t;
}

image blend_image(image fore, image back, اعشاری alpha)
{
    أكد(fore.w == back.w && fore.h == back.h && fore.c == back.c);
    image blend = make_image(fore.w, fore.h, fore.c);
    صحیح i, j, k;
    برای(k = 0; k < fore.c; ++k){
        برای(j = 0; j < fore.h; ++j){
            برای(i = 0; i < fore.w; ++i){
                اعشاری val = alpha * get_pixel(fore, i, j, k) + 
                    (1 - alpha)* get_pixel(back, i, j, k);
                set_pixel(blend, i, j, k, val);
            }
        }
    }
    بازگشت blend;
}

تهی scale_image_channel(image im, صحیح c, اعشاری v)
{
    صحیح i, j;
    برای(j = 0; j < im.h; ++j){
        برای(i = 0; i < im.w; ++i){
            اعشاری pix = get_pixel(im, i, j, c);
            pix = pix*v;
            set_pixel(im, i, j, c, pix);
        }
    }
}

تهی translate_image_channel(image im, صحیح c, اعشاری v)
{
    صحیح i, j;
    برای(j = 0; j < im.h; ++j){
        برای(i = 0; i < im.w; ++i){
            اعشاری pix = get_pixel(im, i, j, c);
            pix = pix+v;
            set_pixel(im, i, j, c, pix);
        }
    }
}

image binarize_image(image im)
{
    image c = copy_image(im);
    صحیح i;
    برای(i = 0; i < im.w * im.h * im.c; ++i){
        اگر(c.data[i] > .5) c.data[i] = 1;
        وگرنه c.data[i] = 0;
    }
    بازگشت c;
}

تهی saturate_image(image im, اعشاری sat)
{
    rgb_to_hsv(im);
    scale_image_channel(im, 1, sat);
    hsv_to_rgb(im);
    constrain_image(im);
}

تهی hue_image(image im, اعشاری hue)
{
    rgb_to_hsv(im);
    صحیح i;
    برای(i = 0; i < im.w*im.h; ++i){
        im.data[i] = im.data[i] + hue;
        اگر (im.data[i] > 1) im.data[i] -= 1;
        اگر (im.data[i] < 0) im.data[i] += 1;
    }
    hsv_to_rgb(im);
    constrain_image(im);
}

تهی exposure_image(image im, اعشاری sat)
{
    rgb_to_hsv(im);
    scale_image_channel(im, 2, sat);
    hsv_to_rgb(im);
    constrain_image(im);
}

تهی distort_image(image im, اعشاری hue, اعشاری sat, اعشاری val)
{
    rgb_to_hsv(im);
    scale_image_channel(im, 1, sat);
    scale_image_channel(im, 2, val);
    صحیح i;
    برای(i = 0; i < im.w*im.h; ++i){
        im.data[i] = im.data[i] + hue;
        اگر (im.data[i] > 1) im.data[i] -= 1;
        اگر (im.data[i] < 0) im.data[i] += 1;
    }
    hsv_to_rgb(im);
    constrain_image(im);
}

تهی random_distort_image(image im, اعشاری hue, اعشاری saturation, اعشاری exposure)
{
    اعشاری dhue = rand_uniform(-hue, hue);
    اعشاری dsat = rand_scale(saturation);
    اعشاری dexp = rand_scale(exposure);
    distort_image(im, dhue, dsat, dexp);
}

تهی saturate_exposure_image(image im, اعشاری sat, اعشاری exposure)
{
    rgb_to_hsv(im);
    scale_image_channel(im, 1, sat);
    scale_image_channel(im, 2, exposure);
    hsv_to_rgb(im);
    constrain_image(im);
}

image resize_image(image im, صحیح w, صحیح h)
{
    image resized = make_image(w, h, im.c);   
    image part = make_image(w, im.h, im.c);
    صحیح r, c, k;
    اعشاری w_scale = (اعشاری)(im.w - 1) / (w - 1);
    اعشاری h_scale = (اعشاری)(im.h - 1) / (h - 1);
    برای(k = 0; k < im.c; ++k){
        برای(r = 0; r < im.h; ++r){
            برای(c = 0; c < w; ++c){
                اعشاری val = 0;
                اگر(c == w-1 || im.w == 1){
                    val = get_pixel(im, im.w-1, r, k);
                } وگرنه {
                    اعشاری sx = c*w_scale;
                    صحیح ix = (صحیح) sx;
                    اعشاری dx = sx - ix;
                    val = (1 - dx) * get_pixel(im, ix, r, k) + dx * get_pixel(im, ix+1, r, k);
                }
                set_pixel(part, c, r, k, val);
            }
        }
    }
    برای(k = 0; k < im.c; ++k){
        برای(r = 0; r < h; ++r){
            اعشاری sy = r*h_scale;
            صحیح iy = (صحیح) sy;
            اعشاری dy = sy - iy;
            برای(c = 0; c < w; ++c){
                اعشاری val = (1-dy) * get_pixel(part, c, iy, k);
                set_pixel(resized, c, r, k, val);
            }
            اگر(r == h-1 || im.h == 1) ادامه;
            برای(c = 0; c < w; ++c){
                اعشاری val = dy * get_pixel(part, c, iy+1, k);
                add_pixel(resized, c, r, k, val);
            }
        }
    }

    free_image(part);
    بازگشت resized;
}


تهی test_resize(نویسه *filename)
{
    image im = load_image(filename, 0,0, 3);
    اعشاری mag = mag_array(im.data, im.w*im.h*im.c);
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
    وقتی(1){
        image aug = random_augment_image(im, 0, .75, 320, 448, 320, 320);
        show_image(aug, "aug", 1);
        free_image(aug);


        اعشاری exposure = 1.15;
        اعشاری saturation = 1.15;
        اعشاری hue = .05;

        image c = copy_image(im);

        اعشاری dexp = rand_scale(exposure);
        اعشاری dsat = rand_scale(saturation);
        اعشاری dhue = rand_uniform(-hue, hue);

        distort_image(c, dhue, dsat, dexp);
        show_image(c, "rand", 1);
        printf("%f %f %f\n", dhue, dsat, dexp);
        free_image(c);
    }
#endif
}


image load_image_stb(نویسه *filename, صحیح channels)
{
    صحیح w, h, c;
    unsigned نویسه *data = stbi_load(filename, &w, &h, &c, channels);
    اگر (!data) {
        fprintf(stderr, "Cannot load image \"%s\"\nSTB Reason: %s\n", filename, stbi_failure_reason());
        exit(0);
    }
    اگر(channels) c = channels;
    صحیح i,j,k;
    image im = make_image(w, h, c);
    برای(k = 0; k < c; ++k){
        برای(j = 0; j < h; ++j){
            برای(i = 0; i < w; ++i){
                صحیح dst_index = i + w*j + w*h*k;
                صحیح src_index = k + c*i + c*w*j;
                im.data[dst_index] = (اعشاری)data[src_index]/255.;
            }
        }
    }
    آزاد(data);
    بازگشت im;
}

image load_image(نویسه *filename, صحیح w, صحیح h, صحیح c)
{
#ifdef OPENCV
    image out = load_image_cv(filename, c);
#وگرنه
    image out = load_image_stb(filename, c);
#endif

    اگر((h && w) && (h != out.h || w != out.w)){
        image resized = resize_image(out, w, h);
        free_image(out);
        out = resized;
    }
    بازگشت out;
}

image load_image_color(نویسه *filename, صحیح w, صحیح h)
{
    بازگشت load_image(filename, w, h, 3);
}

image get_image_layer(image m, صحیح l)
{
    image out = make_image(m.w, m.h, 1);
    صحیح i;
    برای(i = 0; i < m.h*m.w; ++i){
        out.data[i] = m.data[i+l*m.h*m.w];
    }
    بازگشت out;
}
تهی print_image(image m)
{
    صحیح i, j, k;
    برای(i =0 ; i < m.c; ++i){
        برای(j =0 ; j < m.h; ++j){
            برای(k = 0; k < m.w; ++k){
                printf("%.2lf, ", m.data[i*m.h*m.w + j*m.w + k]);
                اگر(k > 30) بشکن;
            }
            printf("\n");
            اگر(j > 30) بشکن;
        }
        printf("\n");
    }
    printf("\n");
}

image collapse_images_vert(image *ims, صحیح n)
{
    صحیح color = 1;
    صحیح border = 1;
    صحیح h,w,c;
    w = ims[0].w;
    h = (ims[0].h + border) * n - border;
    c = ims[0].c;
    اگر(c != 3 || !color){
        w = (w+border)*c - border;
        c = 1;
    }

    image filters = make_image(w, h, c);
    صحیح i,j;
    برای(i = 0; i < n; ++i){
        صحیح h_offset = i*(ims[0].h+border);
        image copy = copy_image(ims[i]);
        //normalize_image(copy);
        اگر(c == 3 && color){
            embed_image(copy, filters, 0, h_offset);
        }
        وگرنه{
            برای(j = 0; j < copy.c; ++j){
                صحیح w_offset = j*(ims[0].w+border);
                image layer = get_image_layer(copy, j);
                embed_image(layer, filters, w_offset, h_offset);
                free_image(layer);
            }
        }
        free_image(copy);
    }
    بازگشت filters;
} 

image collapse_images_horz(image *ims, صحیح n)
{
    صحیح color = 1;
    صحیح border = 1;
    صحیح h,w,c;
    صحیح size = ims[0].h;
    h = size;
    w = (ims[0].w + border) * n - border;
    c = ims[0].c;
    اگر(c != 3 || !color){
        h = (h+border)*c - border;
        c = 1;
    }

    image filters = make_image(w, h, c);
    صحیح i,j;
    برای(i = 0; i < n; ++i){
        صحیح w_offset = i*(size+border);
        image copy = copy_image(ims[i]);
        //normalize_image(copy);
        اگر(c == 3 && color){
            embed_image(copy, filters, w_offset, 0);
        }
        وگرنه{
            برای(j = 0; j < copy.c; ++j){
                صحیح h_offset = j*(size+border);
                image layer = get_image_layer(copy, j);
                embed_image(layer, filters, w_offset, h_offset);
                free_image(layer);
            }
        }
        free_image(copy);
    }
    بازگشت filters;
} 

تهی show_image_normalized(image im, ثابت نویسه *name)
{
    image c = copy_image(im);
    normalize_image(c);
    show_image(c, name, 1);
    free_image(c);
}

تهی show_images(image *ims, صحیح n, نویسه *window)
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

تهی free_image(image m)
{
    اگر(m.data){
        آزاد(m.data);
    }
}
