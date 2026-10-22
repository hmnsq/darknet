#ضمن "image.h"
#ضمن "utils.h"
#ضمن "blas.h"
#ضمن "cuda.h"
#ضمن <stdio.h>
#ضمن <math.h>

#define STB_IMAGE_IMPLEMENTATION
#ضمن "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#ضمن "stb_image_write.h"

صحيح windows = 0;

عائم colors[6][3] = { {1,0,1}, {0,0,1},{0,1,1},{0,1,0},{1,1,0},{1,0,0} };

عائم get_color(صحيح c, صحيح x, صحيح max)
{
    عائم ratio = ((عائم)x/max)*5;
    صحيح i = floor(ratio);
    صحيح j = ceil(ratio);
    ratio -= i;
    عائم r = (1-ratio) * colors[i][c] + ratio*colors[j][c];
    //printf("%f\n", r);
    أرجع r;
}

image mask_to_rgb(image mask)
{
    صحيح n = mask.c;
    image im = make_image(mask.w, mask.h, 3);
    صحيح i, j;
    لكل(j = 0; j < n; ++j){
        صحيح offset = j*123457 % n;
        عائم red = get_color(2,offset,n);
        عائم green = get_color(1,offset,n);
        عائم blue = get_color(0,offset,n);
        لكل(i = 0; i < im.w*im.h; ++i){
            im.data[i + 0*im.w*im.h] += mask.data[j*im.h*im.w + i]*red;
            im.data[i + 1*im.w*im.h] += mask.data[j*im.h*im.w + i]*green;
            im.data[i + 2*im.w*im.h] += mask.data[j*im.h*im.w + i]*blue;
        }
    }
    أرجع im;
}

ساكن عائم get_pixel(image m, صحيح x, صحيح y, صحيح c)
{
    أكد(x < m.w && y < m.h && c < m.c);
    أرجع m.data[c*m.h*m.w + y*m.w + x];
}
ساكن عائم get_pixel_extend(image m, صحيح x, صحيح y, صحيح c)
{
    إذا(x < 0 || x >= m.w || y < 0 || y >= m.h) أرجع 0;
    /*
    if(x < 0) x = 0;
    if(x >= m.w) x = m.w-1;
    if(y < 0) y = 0;
    if(y >= m.h) y = m.h-1;
    */
    إذا(c < 0 || c >= m.c) أرجع 0;
    أرجع get_pixel(m, x, y, c);
}
ساكن فراغ set_pixel(image m, صحيح x, صحيح y, صحيح c, عائم val)
{
    إذا (x < 0 || y < 0 || c < 0 || x >= m.w || y >= m.h || c >= m.c) أرجع;
    أكد(x < m.w && y < m.h && c < m.c);
    m.data[c*m.h*m.w + y*m.w + x] = val;
}
ساكن فراغ add_pixel(image m, صحيح x, صحيح y, صحيح c, عائم val)
{
    أكد(x < m.w && y < m.h && c < m.c);
    m.data[c*m.h*m.w + y*m.w + x] += val;
}

ساكن عائم bilinear_interpolate(image im, عائم x, عائم y, صحيح c)
{
    صحيح ix = (صحيح) floorf(x);
    صحيح iy = (صحيح) floorf(y);

    عائم dx = x - ix;
    عائم dy = y - iy;

    عائم val = (1-dy) * (1-dx) * get_pixel_extend(im, ix, iy, c) + 
        dy     * (1-dx) * get_pixel_extend(im, ix, iy+1, c) + 
        (1-dy) *   dx   * get_pixel_extend(im, ix+1, iy, c) +
        dy     *   dx   * get_pixel_extend(im, ix+1, iy+1, c);
    أرجع val;
}


فراغ composite_image(image source, image dest, صحيح dx, صحيح dy)
{
    صحيح x,y,k;
    لكل(k = 0; k < source.c; ++k){
        لكل(y = 0; y < source.h; ++y){
            لكل(x = 0; x < source.w; ++x){
                عائم val = get_pixel(source, x, y, k);
                عائم val2 = get_pixel_extend(dest, dx+x, dy+y, k);
                set_pixel(dest, dx+x, dy+y, k, val * val2);
            }
        }
    }
}

image border_image(image a, صحيح border)
{
    image b = make_image(a.w + 2*border, a.h + 2*border, a.c);
    صحيح x,y,k;
    لكل(k = 0; k < b.c; ++k){
        لكل(y = 0; y < b.h; ++y){
            لكل(x = 0; x < b.w; ++x){
                عائم val = get_pixel_extend(a, x - border, y - border, k);
                إذا(x - border < 0 || x - border >= a.w || y - border < 0 || y - border >= a.h) val = 1;
                set_pixel(b, x, y, k, val);
            }
        }
    }
    أرجع b;
}

image tile_images(image a, image b, صحيح dx)
{
    إذا(a.w == 0) أرجع copy_image(b);
    image c = make_image(a.w + b.w + dx, (a.h > b.h) ? a.h : b.h, (a.c > b.c) ? a.c : b.c);
    fill_cpu(c.w*c.h*c.c, 1, c.data, 1);
    embed_image(a, c, 0, 0); 
    composite_image(b, c, a.w + dx, 0);
    أرجع c;
}

image get_label(image **characters, حرف *string, صحيح size)
{
    size = size/10;
    إذا(size > 7) size = 7;
    image label = make_empty_image(0,0,0);
    طالما(*string){
        image l = characters[size][(صحيح)*string];
        image n = tile_images(label, l, -size - 1 + (size+1)/2);
        free_image(label);
        label = n;
        ++string;
    }
    image b = border_image(label, label.h*.25);
    free_image(label);
    أرجع b;
}

فراغ draw_label(image a, صحيح r, صحيح c, image label, ثابت عائم *rgb)
{
    صحيح w = label.w;
    صحيح h = label.h;
    إذا (r - h >= 0) r = r - h;

    صحيح i, j, k;
    لكل(j = 0; j < h && j + r < a.h; ++j){
        لكل(i = 0; i < w && i + c < a.w; ++i){
            لكل(k = 0; k < label.c; ++k){
                عائم val = get_pixel(label, i, j, k);
                set_pixel(a, i+c, j+r, k, rgb[k] * val);
            }
        }
    }
}

فراغ draw_box(image a, صحيح x1, صحيح y1, صحيح x2, صحيح y2, عائم r, عائم g, عائم b)
{
    //normalize_image(a);
    صحيح i;
    إذا(x1 < 0) x1 = 0;
    إذا(x1 >= a.w) x1 = a.w-1;
    إذا(x2 < 0) x2 = 0;
    إذا(x2 >= a.w) x2 = a.w-1;

    إذا(y1 < 0) y1 = 0;
    إذا(y1 >= a.h) y1 = a.h-1;
    إذا(y2 < 0) y2 = 0;
    إذا(y2 >= a.h) y2 = a.h-1;

    لكل(i = x1; i <= x2; ++i){
        a.data[i + y1*a.w + 0*a.w*a.h] = r;
        a.data[i + y2*a.w + 0*a.w*a.h] = r;

        a.data[i + y1*a.w + 1*a.w*a.h] = g;
        a.data[i + y2*a.w + 1*a.w*a.h] = g;

        a.data[i + y1*a.w + 2*a.w*a.h] = b;
        a.data[i + y2*a.w + 2*a.w*a.h] = b;
    }
    لكل(i = y1; i <= y2; ++i){
        a.data[x1 + i*a.w + 0*a.w*a.h] = r;
        a.data[x2 + i*a.w + 0*a.w*a.h] = r;

        a.data[x1 + i*a.w + 1*a.w*a.h] = g;
        a.data[x2 + i*a.w + 1*a.w*a.h] = g;

        a.data[x1 + i*a.w + 2*a.w*a.h] = b;
        a.data[x2 + i*a.w + 2*a.w*a.h] = b;
    }
}

فراغ draw_box_width(image a, صحيح x1, صحيح y1, صحيح x2, صحيح y2, صحيح w, عائم r, عائم g, عائم b)
{
    صحيح i;
    لكل(i = 0; i < w; ++i){
        draw_box(a, x1+i, y1+i, x2-i, y2-i, r, g, b);
    }
}

فراغ draw_bbox(image a, box bbox, صحيح w, عائم r, عائم g, عائم b)
{
    صحيح چەپ  = (bbox.x-bbox.w/2)*a.w;
    صحيح ڕاست = (bbox.x+bbox.w/2)*a.w;
    صحيح لووتکە   = (bbox.y-bbox.h/2)*a.h;
    صحيح bot   = (bbox.y+bbox.h/2)*a.h;

    صحيح i;
    لكل(i = 0; i < w; ++i){
        draw_box(a, چەپ+i, لووتکە+i, ڕاست-i, bot-i, r, g, b);
    }
}

image **load_alphabet()
{
    صحيح i, j;
    ثابت صحيح nsize = 8;
    image **alphabets = calloc(nsize, حجم(image));
    لكل(j = 0; j < nsize; ++j){
        alphabets[j] = calloc(128, حجم(image));
        لكل(i = 32; i < 127; ++i){
            حرف buff[256];
            sprintf(buff, "data/labels/%d_%d.png", i, j);
            alphabets[j][i] = load_image_color(buff, 0, 0);
        }
    }
    أرجع alphabets;
}

فراغ draw_detections(image im, detection *dets, صحيح ژمارە, عائم thresh, حرف **names, image **alphabet, صحيح classes)
{
    صحيح i,j;

    لكل(i = 0; i < ژمارە; ++i){
        حرف labelstr[4096] = {0};
        صحيح class = -1;
        لكل(j = 0; j < classes; ++j){
            إذا (dets[i].prob[j] > thresh){
                إذا (class < 0) {
                    strcat(labelstr, names[j]);
                    class = j;
                } وإلا {
                    strcat(labelstr, ", ");
                    strcat(labelstr, names[j]);
                }
                printf("%s: %.0f%%\n", names[j], dets[i].prob[j]*100);
            }
        }
        إذا(class >= 0){
            صحيح width = im.h * .006;

            /*
               if(0){
               width = pow(prob, 1./2.)*10+1;
               alphabet = 0;
               }
             */

            //printf("%d %s: %.0f%%\n", i, names[class], prob*100);
            صحيح offset = class*123457 % classes;
            عائم red = get_color(2,offset,classes);
            عائم green = get_color(1,offset,classes);
            عائم blue = get_color(0,offset,classes);
            عائم rgb[3];

            //width = prob*20+2;

            rgb[0] = red;
            rgb[1] = green;
            rgb[2] = blue;
            box b = dets[i].bbox;
            //printf("%f %f %f %f\n", b.x, b.y, b.w, b.h);

            صحيح چەپ  = (b.x-b.w/2.)*im.w;
            صحيح ڕاست = (b.x+b.w/2.)*im.w;
            صحيح لووتکە   = (b.y-b.h/2.)*im.h;
            صحيح bot   = (b.y+b.h/2.)*im.h;

            إذا(چەپ < 0) چەپ = 0;
            إذا(ڕاست > im.w-1) ڕاست = im.w-1;
            إذا(لووتکە < 0) لووتکە = 0;
            إذا(bot > im.h-1) bot = im.h-1;

            draw_box_width(im, چەپ, لووتکە, ڕاست, bot, width, red, green, blue);
            إذا (alphabet) {
                image label = get_label(alphabet, labelstr, (im.h*.03));
                draw_label(im, لووتکە + width, چەپ, label, rgb);
                free_image(label);
            }
            إذا (dets[i].mask){
                image mask = float_to_image(14, 14, 1, dets[i].mask);
                image resized_mask = resize_image(mask, b.w*im.w, b.h*im.h);
                image tmask = threshold_image(resized_mask, .5);
                embed_image(tmask, im, چەپ, لووتکە);
                free_image(mask);
                free_image(resized_mask);
                free_image(tmask);
            }
        }
    }
}

فراغ transpose_image(image im)
{
    أكد(im.w == im.h);
    صحيح n, m;
    صحيح c;
    لكل(c = 0; c < im.c; ++c){
        لكل(n = 0; n < im.w-1; ++n){
            لكل(m = n + 1; m < im.w; ++m){
                عائم swap = im.data[m + im.w*(n + im.h*c)];
                im.data[m + im.w*(n + im.h*c)] = im.data[n + im.w*(m + im.h*c)];
                im.data[n + im.w*(m + im.h*c)] = swap;
            }
        }
    }
}

فراغ rotate_image_cw(image im, صحيح times)
{
    أكد(im.w == im.h);
    times = (times + 400) % 4;
    صحيح i, x, y, c;
    صحيح n = im.w;
    لكل(i = 0; i < times; ++i){
        لكل(c = 0; c < im.c; ++c){
            لكل(x = 0; x < n/2; ++x){
                لكل(y = 0; y < (n-1)/2 + 1; ++y){
                    عائم temp = im.data[y + im.w*(x + im.h*c)];
                    im.data[y + im.w*(x + im.h*c)] = im.data[n-1-x + im.w*(y + im.h*c)];
                    im.data[n-1-x + im.w*(y + im.h*c)] = im.data[n-1-y + im.w*(n-1-x + im.h*c)];
                    im.data[n-1-y + im.w*(n-1-x + im.h*c)] = im.data[x + im.w*(n-1-y + im.h*c)];
                    im.data[x + im.w*(n-1-y + im.h*c)] = temp;
                }
            }
        }
    }
}

فراغ flip_image(image a)
{
    صحيح i,j,k;
    لكل(k = 0; k < a.c; ++k){
        لكل(i = 0; i < a.h; ++i){
            لكل(j = 0; j < a.w/2; ++j){
                صحيح index = j + a.w*(i + a.h*(k));
                صحيح flip = (a.w - j - 1) + a.w*(i + a.h*(k));
                عائم swap = a.data[flip];
                a.data[flip] = a.data[index];
                a.data[index] = swap;
            }
        }
    }
}

image image_distance(image a, image b)
{
    صحيح i,j;
    image dist = make_image(a.w, a.h, 1);
    لكل(i = 0; i < a.c; ++i){
        لكل(j = 0; j < a.h*a.w; ++j){
            dist.data[j] += pow(a.data[i*a.h*a.w+j]-b.data[i*a.h*a.w+j],2);
        }
    }
    لكل(j = 0; j < a.h*a.w; ++j){
        dist.data[j] = sqrt(dist.data[j]);
    }
    أرجع dist;
}

فراغ ghost_image(image source, image dest, صحيح dx, صحيح dy)
{
    صحيح x,y,k;
    عائم max_dist = sqrt((-source.w/2. + .5)*(-source.w/2. + .5));
    لكل(k = 0; k < source.c; ++k){
        لكل(y = 0; y < source.h; ++y){
            لكل(x = 0; x < source.w; ++x){
                عائم dist = sqrt((x - source.w/2. + .5)*(x - source.w/2. + .5) + (y - source.h/2. + .5)*(y - source.h/2. + .5));
                عائم alpha = (1 - dist/max_dist);
                إذا(alpha < 0) alpha = 0;
                عائم v1 = get_pixel(source, x,y,k);
                عائم v2 = get_pixel(dest, dx+x,dy+y,k);
                عائم val = alpha*v1 + (1-alpha)*v2;
                set_pixel(dest, dx+x, dy+y, k, val);
            }
        }
    }
}

فراغ blocky_image(image im, صحيح s)
{
    صحيح i,j,k;
    لكل(k = 0; k < im.c; ++k){
        لكل(j = 0; j < im.h; ++j){
            لكل(i = 0; i < im.w; ++i){
                im.data[i + im.w*(j + im.h*k)] = im.data[i/s*s + im.w*(j/s*s + im.h*k)];
            }
        }
    }
}

فراغ censor_image(image im, صحيح dx, صحيح dy, صحيح w, صحيح h)
{
    صحيح i,j,k;
    صحيح s = 32;
    إذا(dx < 0) dx = 0;
    إذا(dy < 0) dy = 0;

    لكل(k = 0; k < im.c; ++k){
        لكل(j = dy; j < dy + h && j < im.h; ++j){
            لكل(i = dx; i < dx + w && i < im.w; ++i){
                im.data[i + im.w*(j + im.h*k)] = im.data[i/s*s + im.w*(j/s*s + im.h*k)];
                //im.data[i + j*im.w + k*im.w*im.h] = 0;
            }
        }
    }
}

فراغ embed_image(image source, image dest, صحيح dx, صحيح dy)
{
    صحيح x,y,k;
    لكل(k = 0; k < source.c; ++k){
        لكل(y = 0; y < source.h; ++y){
            لكل(x = 0; x < source.w; ++x){
                عائم val = get_pixel(source, x,y,k);
                set_pixel(dest, dx+x, dy+y, k, val);
            }
        }
    }
}

image collapse_image_layers(image source, صحيح border)
{
    صحيح h = source.h;
    h = (h+border)*source.c - border;
    image dest = make_image(source.w, h, 1);
    صحيح i;
    لكل(i = 0; i < source.c; ++i){
        image layer = get_image_layer(source, i);
        صحيح h_offset = i*(source.h+border);
        embed_image(layer, dest, 0, h_offset);
        free_image(layer);
    }
    أرجع dest;
}

فراغ constrain_image(image im)
{
    صحيح i;
    لكل(i = 0; i < im.w*im.h*im.c; ++i){
        إذا(im.data[i] < 0) im.data[i] = 0;
        إذا(im.data[i] > 1) im.data[i] = 1;
    }
}

فراغ normalize_image(image p)
{
    صحيح i;
    عائم min = 9999999;
    عائم max = -999999;

    لكل(i = 0; i < p.h*p.w*p.c; ++i){
        عائم v = p.data[i];
        إذا(v < min) min = v;
        إذا(v > max) max = v;
    }
    إذا(max - min < .000000001){
        min = 0;
        max = 1;
    }
    لكل(i = 0; i < p.c*p.w*p.h; ++i){
        p.data[i] = (p.data[i] - min)/(max-min);
    }
}

فراغ normalize_image2(image p)
{
    عائم *min = calloc(p.c, حجم(عائم));
    عائم *max = calloc(p.c, حجم(عائم));
    صحيح i,j;
    لكل(i = 0; i < p.c; ++i) min[i] = max[i] = p.data[i*p.h*p.w];

    لكل(j = 0; j < p.c; ++j){
        لكل(i = 0; i < p.h*p.w; ++i){
            عائم v = p.data[i+j*p.h*p.w];
            إذا(v < min[j]) min[j] = v;
            إذا(v > max[j]) max[j] = v;
        }
    }
    لكل(i = 0; i < p.c; ++i){
        إذا(max[i] - min[i] < .000000001){
            min[i] = 0;
            max[i] = 1;
        }
    }
    لكل(j = 0; j < p.c; ++j){
        لكل(i = 0; i < p.w*p.h; ++i){
            p.data[i+j*p.h*p.w] = (p.data[i+j*p.h*p.w] - min[j])/(max[j]-min[j]);
        }
    }
    حرر(min);
    حرر(max);
}

فراغ copy_image_into(image src, image dest)
{
    memcpy(dest.data, src.data, src.h*src.w*src.c*حجم(عائم));
}

image copy_image(image p)
{
    image copy = p;
    copy.data = calloc(p.h*p.w*p.c, حجم(عائم));
    memcpy(copy.data, p.data, p.h*p.w*p.c*حجم(عائم));
    أرجع copy;
}

فراغ rgbgr_image(image im)
{
    صحيح i;
    لكل(i = 0; i < im.w*im.h; ++i){
        عائم swap = im.data[i];
        im.data[i] = im.data[i+im.w*im.h*2];
        im.data[i+im.w*im.h*2] = swap;
    }
}

صحيح show_image(image p, ثابت حرف *name, صحيح ms)
{
#ifdef OPENCV
    صحيح c = show_image_cv(p, name, ms);
    أرجع c;
#وإلا
    fprintf(stderr, "Not compiled with OpenCV, saving to %s.png instead\n", name);
    save_image(p, name);
    أرجع -1;
#endif
}

فراغ save_image_options(image im, ثابت حرف *name, IMTYPE f, صحيح quality)
{
    حرف buff[256];
    //sprintf(buff, "%s (%d)", name, windows);
    إذا(f == PNG)       sprintf(buff, "%s.png", name);
    وإلا إذا (f == BMP) sprintf(buff, "%s.bmp", name);
    وإلا إذا (f == TGA) sprintf(buff, "%s.tga", name);
    وإلا إذا (f == JPG) sprintf(buff, "%s.jpg", name);
    وإلا               sprintf(buff, "%s.png", name);
    unsigned حرف *data = calloc(im.w*im.h*im.c, حجم(حرف));
    صحيح i,k;
    لكل(k = 0; k < im.c; ++k){
        لكل(i = 0; i < im.w*im.h; ++i){
            data[i*im.c+k] = (unsigned حرف) (255*im.data[i + k*im.w*im.h]);
        }
    }
    صحيح success = 0;
    إذا(f == PNG)       success = stbi_write_png(buff, im.w, im.h, im.c, data, im.w*im.c);
    وإلا إذا (f == BMP) success = stbi_write_bmp(buff, im.w, im.h, im.c, data);
    وإلا إذا (f == TGA) success = stbi_write_tga(buff, im.w, im.h, im.c, data);
    وإلا إذا (f == JPG) success = stbi_write_jpg(buff, im.w, im.h, im.c, data, quality);
    حرر(data);
    إذا(!success) fprintf(stderr, "Failed to write image %s\n", buff);
}

فراغ save_image(image im, ثابت حرف *name)
{
    save_image_options(im, name, JPG, 80);
}

فراغ show_image_layers(image p, حرف *name)
{
    صحيح i;
    حرف buff[256];
    لكل(i = 0; i < p.c; ++i){
        sprintf(buff, "%s - Layer %d", name, i);
        image layer = get_image_layer(p, i);
        show_image(layer, buff, 1);
        free_image(layer);
    }
}

فراغ show_image_collapsed(image p, حرف *name)
{
    image c = collapse_image_layers(p, 1);
    show_image(c, name, 1);
    free_image(c);
}

image make_empty_image(صحيح w, صحيح h, صحيح c)
{
    image out;
    out.data = 0;
    out.h = h;
    out.w = w;
    out.c = c;
    أرجع out;
}

image make_image(صحيح w, صحيح h, صحيح c)
{
    image out = make_empty_image(w,h,c);
    out.data = calloc(h*w*c, حجم(عائم));
    أرجع out;
}

image make_random_image(صحيح w, صحيح h, صحيح c)
{
    image out = make_empty_image(w,h,c);
    out.data = calloc(h*w*c, حجم(عائم));
    صحيح i;
    لكل(i = 0; i < w*h*c; ++i){
        out.data[i] = (rand_normal() * .25) + .5;
    }
    أرجع out;
}

image float_to_image(صحيح w, صحيح h, صحيح c, عائم *data)
{
    image out = make_empty_image(w,h,c);
    out.data = data;
    أرجع out;
}

فراغ place_image(image im, صحيح w, صحيح h, صحيح dx, صحيح dy, image canvas)
{
    صحيح x, y, c;
    لكل(c = 0; c < im.c; ++c){
        لكل(y = 0; y < h; ++y){
            لكل(x = 0; x < w; ++x){
                عائم rx = ((عائم)x / w) * im.w;
                عائم ry = ((عائم)y / h) * im.h;
                عائم val = bilinear_interpolate(im, rx, ry, c);
                set_pixel(canvas, x + dx, y + dy, c, val);
            }
        }
    }
}

image center_crop_image(image im, صحيح w, صحيح h)
{
    صحيح m = (im.w < im.h) ? im.w : im.h;   
    image c = crop_image(im, (im.w - m) / 2, (im.h - m)/2, m, m);
    image r = resize_image(c, w, h);
    free_image(c);
    أرجع r;
}

image rotate_crop_image(image im, عائم rad, عائم s, صحيح w, صحيح h, عائم dx, عائم dy, عائم aspect)
{
    صحيح x, y, c;
    عائم cx = im.w/2.;
    عائم cy = im.h/2.;
    image rot = make_image(w, h, im.c);
    لكل(c = 0; c < im.c; ++c){
        لكل(y = 0; y < h; ++y){
            لكل(x = 0; x < w; ++x){
                عائم rx = cos(rad)*((x - w/2.)/s*aspect + dx/s*aspect) - sin(rad)*((y - h/2.)/s + dy/s) + cx;
                عائم ry = sin(rad)*((x - w/2.)/s*aspect + dx/s*aspect) + cos(rad)*((y - h/2.)/s + dy/s) + cy;
                عائم val = bilinear_interpolate(im, rx, ry, c);
                set_pixel(rot, x, y, c, val);
            }
        }
    }
    أرجع rot;
}

image rotate_image(image im, عائم rad)
{
    صحيح x, y, c;
    عائم cx = im.w/2.;
    عائم cy = im.h/2.;
    image rot = make_image(im.w, im.h, im.c);
    لكل(c = 0; c < im.c; ++c){
        لكل(y = 0; y < im.h; ++y){
            لكل(x = 0; x < im.w; ++x){
                عائم rx = cos(rad)*(x-cx) - sin(rad)*(y-cy) + cx;
                عائم ry = sin(rad)*(x-cx) + cos(rad)*(y-cy) + cy;
                عائم val = bilinear_interpolate(im, rx, ry, c);
                set_pixel(rot, x, y, c, val);
            }
        }
    }
    أرجع rot;
}

فراغ fill_image(image m, عائم s)
{
    صحيح i;
    لكل(i = 0; i < m.h*m.w*m.c; ++i) m.data[i] = s;
}

فراغ translate_image(image m, عائم s)
{
    صحيح i;
    لكل(i = 0; i < m.h*m.w*m.c; ++i) m.data[i] += s;
}

فراغ scale_image(image m, عائم s)
{
    صحيح i;
    لكل(i = 0; i < m.h*m.w*m.c; ++i) m.data[i] *= s;
}

image crop_image(image im, صحيح dx, صحيح dy, صحيح w, صحيح h)
{
    image cropped = make_image(w, h, im.c);
    صحيح i, j, k;
    لكل(k = 0; k < im.c; ++k){
        لكل(j = 0; j < h; ++j){
            لكل(i = 0; i < w; ++i){
                صحيح r = j + dy;
                صحيح c = i + dx;
                عائم val = 0;
                r = constrain_int(r, 0, im.h-1);
                c = constrain_int(c, 0, im.w-1);
                val = get_pixel(im, c, r, k);
                set_pixel(cropped, i, j, k, val);
            }
        }
    }
    أرجع cropped;
}

صحيح best_3d_shift_r(image a, image b, صحيح min, صحيح max)
{
    إذا(min == max) أرجع min;
    صحيح mid = floor((min + max) / 2.);
    image c1 = crop_image(b, 0, mid, b.w, b.h);
    image c2 = crop_image(b, 0, mid+1, b.w, b.h);
    عائم d1 = dist_array(c1.data, a.data, a.w*a.h*a.c, 10);
    عائم d2 = dist_array(c2.data, a.data, a.w*a.h*a.c, 10);
    free_image(c1);
    free_image(c2);
    إذا(d1 < d2) أرجع best_3d_shift_r(a, b, min, mid);
    وإلا أرجع best_3d_shift_r(a, b, mid+1, max);
}

صحيح best_3d_shift(image a, image b, صحيح min, صحيح max)
{
    صحيح i;
    صحيح best = 0;
    عائم best_distance = FLT_MAX;
    لكل(i = min; i <= max; i += 2){
        image c = crop_image(b, 0, i, b.w, b.h);
        عائم d = dist_array(c.data, a.data, a.w*a.h*a.c, 100);
        إذا(d < best_distance){
            best_distance = d;
            best = i;
        }
        printf("%d %f\n", i, d);
        free_image(c);
    }
    أرجع best;
}

فراغ composite_3d(حرف *f1, حرف *f2, حرف *out, صحيح delta)
{
    إذا(!out) out = "out";
    image a = load_image(f1, 0,0,0);
    image b = load_image(f2, 0,0,0);
    صحيح shift = best_3d_shift_r(a, b, -a.h/100, a.h/100);

    image c1 = crop_image(b, 10, shift, b.w, b.h);
    عائم d1 = dist_array(c1.data, a.data, a.w*a.h*a.c, 100);
    image c2 = crop_image(b, -10, shift, b.w, b.h);
    عائم d2 = dist_array(c2.data, a.data, a.w*a.h*a.c, 100);

    إذا(d2 < d1 && 0){
        image swap = a;
        a = b;
        b = swap;
        shift = -shift;
        printf("swapped, %d\n", shift);
    }
    وإلا{
        printf("%d\n", shift);
    }

    image c = crop_image(b, delta, shift, a.w, a.h);
    صحيح i;
    لكل(i = 0; i < c.w*c.h; ++i){
        c.data[i] = a.data[i];
    }
    save_image(c, out);
}

فراغ letterbox_image_into(image im, صحيح w, صحيح h, image boxed)
{
    صحيح new_w = im.w;
    صحيح new_h = im.h;
    إذا (((عائم)w/im.w) < ((عائم)h/im.h)) {
        new_w = w;
        new_h = (im.h * w)/im.w;
    } وإلا {
        new_h = h;
        new_w = (im.w * h)/im.h;
    }
    image resized = resize_image(im, new_w, new_h);
    embed_image(resized, boxed, (w-new_w)/2, (h-new_h)/2); 
    free_image(resized);
}

image letterbox_image(image im, صحيح w, صحيح h)
{
    صحيح new_w = im.w;
    صحيح new_h = im.h;
    إذا (((عائم)w/im.w) < ((عائم)h/im.h)) {
        new_w = w;
        new_h = (im.h * w)/im.w;
    } وإلا {
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
    أرجع boxed;
}

image resize_max(image im, صحيح max)
{
    صحيح w = im.w;
    صحيح h = im.h;
    إذا(w > h){
        h = (h * max) / w;
        w = max;
    } وإلا {
        w = (w * max) / h;
        h = max;
    }
    إذا(w == im.w && h == im.h) أرجع im;
    image resized = resize_image(im, w, h);
    أرجع resized;
}

image resize_min(image im, صحيح min)
{
    صحيح w = im.w;
    صحيح h = im.h;
    إذا(w < h){
        h = (h * min) / w;
        w = min;
    } وإلا {
        w = (w * min) / h;
        h = min;
    }
    إذا(w == im.w && h == im.h) أرجع im;
    image resized = resize_image(im, w, h);
    أرجع resized;
}

image random_crop_image(image im, صحيح w, صحيح h)
{
    صحيح dx = rand_int(0, im.w - w);
    صحيح dy = rand_int(0, im.h - h);
    image crop = crop_image(im, dx, dy, w, h);
    أرجع crop;
}

augment_args random_augment_args(image im, عائم angle, عائم aspect, صحيح نزم, صحيح بەرز, صحيح w, صحيح h)
{
    augment_args a = {0};
    aspect = rand_scale(aspect);
    صحيح r = rand_int(نزم, بەرز);
    صحيح min = (im.h < im.w*aspect) ? im.h : im.w*aspect;
    عائم scale = (عائم)r / min;

    عائم rad = rand_uniform(-angle, angle) * TWO_PI / 360.;

    عائم dx = (im.w*scale/aspect - w) / 2.;
    عائم dy = (im.h*scale - w) / 2.;
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
    أرجع a;
}

image random_augment_image(image im, عائم angle, عائم aspect, صحيح نزم, صحيح بەرز, صحيح w, صحيح h)
{
    augment_args a = random_augment_args(im, angle, aspect, نزم, بەرز, w, h);
    image crop = rotate_crop_image(im, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);
    أرجع crop;
}

عائم three_way_max(عائم a, عائم b, عائم c)
{
    أرجع (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

عائم three_way_min(عائم a, عائم b, عائم c)
{
    أرجع (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

فراغ yuv_to_rgb(image im)
{
    أكد(im.c == 3);
    صحيح i, j;
    عائم r, g, b;
    عائم y, u, v;
    لكل(j = 0; j < im.h; ++j){
        لكل(i = 0; i < im.w; ++i){
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

فراغ rgb_to_yuv(image im)
{
    أكد(im.c == 3);
    صحيح i, j;
    عائم r, g, b;
    عائم y, u, v;
    لكل(j = 0; j < im.h; ++j){
        لكل(i = 0; i < im.w; ++i){
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
فراغ rgb_to_hsv(image im)
{
    أكد(im.c == 3);
    صحيح i, j;
    عائم r, g, b;
    عائم h, s, v;
    لكل(j = 0; j < im.h; ++j){
        لكل(i = 0; i < im.w; ++i){
            r = get_pixel(im, i , j, 0);
            g = get_pixel(im, i , j, 1);
            b = get_pixel(im, i , j, 2);
            عائم max = three_way_max(r,g,b);
            عائم min = three_way_min(r,g,b);
            عائم delta = max - min;
            v = max;
            إذا(max == 0){
                s = 0;
                h = 0;
            }وإلا{
                s = delta/max;
                إذا(r == max){
                    h = (g - b) / delta;
                } وإلا إذا (g == max) {
                    h = 2 + (b - r) / delta;
                } وإلا {
                    h = 4 + (r - g) / delta;
                }
                إذا (h < 0) h += 6;
                h = h/6.;
            }
            set_pixel(im, i, j, 0, h);
            set_pixel(im, i, j, 1, s);
            set_pixel(im, i, j, 2, v);
        }
    }
}

فراغ hsv_to_rgb(image im)
{
    أكد(im.c == 3);
    صحيح i, j;
    عائم r, g, b;
    عائم h, s, v;
    عائم f, p, q, t;
    لكل(j = 0; j < im.h; ++j){
        لكل(i = 0; i < im.w; ++i){
            h = 6 * get_pixel(im, i , j, 0);
            s = get_pixel(im, i , j, 1);
            v = get_pixel(im, i , j, 2);
            إذا (s == 0) {
                r = g = b = v;
            } وإلا {
                صحيح index = floor(h);
                f = h - index;
                p = v*(1-s);
                q = v*(1-s*f);
                t = v*(1-s*(1-f));
                إذا(index == 0){
                    r = v; g = t; b = p;
                } وإلا إذا(index == 1){
                    r = q; g = v; b = p;
                } وإلا إذا(index == 2){
                    r = p; g = v; b = t;
                } وإلا إذا(index == 3){
                    r = p; g = q; b = v;
                } وإلا إذا(index == 4){
                    r = t; g = p; b = v;
                } وإلا {
                    r = v; g = p; b = q;
                }
            }
            set_pixel(im, i, j, 0, r);
            set_pixel(im, i, j, 1, g);
            set_pixel(im, i, j, 2, b);
        }
    }
}

فراغ grayscale_image_3c(image im)
{
    أكد(im.c == 3);
    صحيح i, j, k;
    عائم scale[] = {0.299, 0.587, 0.114};
    لكل(j = 0; j < im.h; ++j){
        لكل(i = 0; i < im.w; ++i){
            عائم val = 0;
            لكل(k = 0; k < 3; ++k){
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
    صحيح i, j, k;
    image gray = make_image(im.w, im.h, 1);
    عائم scale[] = {0.299, 0.587, 0.114};
    لكل(k = 0; k < im.c; ++k){
        لكل(j = 0; j < im.h; ++j){
            لكل(i = 0; i < im.w; ++i){
                gray.data[i+im.w*j] += scale[k]*get_pixel(im, i, j, k);
            }
        }
    }
    أرجع gray;
}

image threshold_image(image im, عائم thresh)
{
    صحيح i;
    image t = make_image(im.w, im.h, im.c);
    لكل(i = 0; i < im.w*im.h*im.c; ++i){
        t.data[i] = im.data[i]>thresh ? 1 : 0;
    }
    أرجع t;
}

image blend_image(image fore, image back, عائم alpha)
{
    أكد(fore.w == back.w && fore.h == back.h && fore.c == back.c);
    image blend = make_image(fore.w, fore.h, fore.c);
    صحيح i, j, k;
    لكل(k = 0; k < fore.c; ++k){
        لكل(j = 0; j < fore.h; ++j){
            لكل(i = 0; i < fore.w; ++i){
                عائم val = alpha * get_pixel(fore, i, j, k) + 
                    (1 - alpha)* get_pixel(back, i, j, k);
                set_pixel(blend, i, j, k, val);
            }
        }
    }
    أرجع blend;
}

فراغ scale_image_channel(image im, صحيح c, عائم v)
{
    صحيح i, j;
    لكل(j = 0; j < im.h; ++j){
        لكل(i = 0; i < im.w; ++i){
            عائم pix = get_pixel(im, i, j, c);
            pix = pix*v;
            set_pixel(im, i, j, c, pix);
        }
    }
}

فراغ translate_image_channel(image im, صحيح c, عائم v)
{
    صحيح i, j;
    لكل(j = 0; j < im.h; ++j){
        لكل(i = 0; i < im.w; ++i){
            عائم pix = get_pixel(im, i, j, c);
            pix = pix+v;
            set_pixel(im, i, j, c, pix);
        }
    }
}

image binarize_image(image im)
{
    image c = copy_image(im);
    صحيح i;
    لكل(i = 0; i < im.w * im.h * im.c; ++i){
        إذا(c.data[i] > .5) c.data[i] = 1;
        وإلا c.data[i] = 0;
    }
    أرجع c;
}

فراغ saturate_image(image im, عائم sat)
{
    rgb_to_hsv(im);
    scale_image_channel(im, 1, sat);
    hsv_to_rgb(im);
    constrain_image(im);
}

فراغ hue_image(image im, عائم hue)
{
    rgb_to_hsv(im);
    صحيح i;
    لكل(i = 0; i < im.w*im.h; ++i){
        im.data[i] = im.data[i] + hue;
        إذا (im.data[i] > 1) im.data[i] -= 1;
        إذا (im.data[i] < 0) im.data[i] += 1;
    }
    hsv_to_rgb(im);
    constrain_image(im);
}

فراغ exposure_image(image im, عائم sat)
{
    rgb_to_hsv(im);
    scale_image_channel(im, 2, sat);
    hsv_to_rgb(im);
    constrain_image(im);
}

فراغ distort_image(image im, عائم hue, عائم sat, عائم val)
{
    rgb_to_hsv(im);
    scale_image_channel(im, 1, sat);
    scale_image_channel(im, 2, val);
    صحيح i;
    لكل(i = 0; i < im.w*im.h; ++i){
        im.data[i] = im.data[i] + hue;
        إذا (im.data[i] > 1) im.data[i] -= 1;
        إذا (im.data[i] < 0) im.data[i] += 1;
    }
    hsv_to_rgb(im);
    constrain_image(im);
}

فراغ random_distort_image(image im, عائم hue, عائم saturation, عائم exposure)
{
    عائم dhue = rand_uniform(-hue, hue);
    عائم dsat = rand_scale(saturation);
    عائم dexp = rand_scale(exposure);
    distort_image(im, dhue, dsat, dexp);
}

فراغ saturate_exposure_image(image im, عائم sat, عائم exposure)
{
    rgb_to_hsv(im);
    scale_image_channel(im, 1, sat);
    scale_image_channel(im, 2, exposure);
    hsv_to_rgb(im);
    constrain_image(im);
}

image resize_image(image im, صحيح w, صحيح h)
{
    image resized = make_image(w, h, im.c);   
    image part = make_image(w, im.h, im.c);
    صحيح r, c, k;
    عائم w_scale = (عائم)(im.w - 1) / (w - 1);
    عائم h_scale = (عائم)(im.h - 1) / (h - 1);
    لكل(k = 0; k < im.c; ++k){
        لكل(r = 0; r < im.h; ++r){
            لكل(c = 0; c < w; ++c){
                عائم val = 0;
                إذا(c == w-1 || im.w == 1){
                    val = get_pixel(im, im.w-1, r, k);
                } وإلا {
                    عائم sx = c*w_scale;
                    صحيح ix = (صحيح) sx;
                    عائم dx = sx - ix;
                    val = (1 - dx) * get_pixel(im, ix, r, k) + dx * get_pixel(im, ix+1, r, k);
                }
                set_pixel(part, c, r, k, val);
            }
        }
    }
    لكل(k = 0; k < im.c; ++k){
        لكل(r = 0; r < h; ++r){
            عائم sy = r*h_scale;
            صحيح iy = (صحيح) sy;
            عائم dy = sy - iy;
            لكل(c = 0; c < w; ++c){
                عائم val = (1-dy) * get_pixel(part, c, iy, k);
                set_pixel(resized, c, r, k, val);
            }
            إذا(r == h-1 || im.h == 1) استمر;
            لكل(c = 0; c < w; ++c){
                عائم val = dy * get_pixel(part, c, iy+1, k);
                add_pixel(resized, c, r, k, val);
            }
        }
    }

    free_image(part);
    أرجع resized;
}


فراغ test_resize(حرف *filename)
{
    image im = load_image(filename, 0,0, 3);
    عائم mag = mag_array(im.data, im.w*im.h*im.c);
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
    طالما(1){
        image aug = random_augment_image(im, 0, .75, 320, 448, 320, 320);
        show_image(aug, "aug", 1);
        free_image(aug);


        عائم exposure = 1.15;
        عائم saturation = 1.15;
        عائم hue = .05;

        image c = copy_image(im);

        عائم dexp = rand_scale(exposure);
        عائم dsat = rand_scale(saturation);
        عائم dhue = rand_uniform(-hue, hue);

        distort_image(c, dhue, dsat, dexp);
        show_image(c, "rand", 1);
        printf("%f %f %f\n", dhue, dsat, dexp);
        free_image(c);
    }
#endif
}


image load_image_stb(حرف *filename, صحيح channels)
{
    صحيح w, h, c;
    unsigned حرف *data = stbi_load(filename, &w, &h, &c, channels);
    إذا (!data) {
        fprintf(stderr, "Cannot load image \"%s\"\nSTB Reason: %s\n", filename, stbi_failure_reason());
        exit(0);
    }
    إذا(channels) c = channels;
    صحيح i,j,k;
    image im = make_image(w, h, c);
    لكل(k = 0; k < c; ++k){
        لكل(j = 0; j < h; ++j){
            لكل(i = 0; i < w; ++i){
                صحيح dst_index = i + w*j + w*h*k;
                صحيح src_index = k + c*i + c*w*j;
                im.data[dst_index] = (عائم)data[src_index]/255.;
            }
        }
    }
    حرر(data);
    أرجع im;
}

image load_image(حرف *filename, صحيح w, صحيح h, صحيح c)
{
#ifdef OPENCV
    image out = load_image_cv(filename, c);
#وإلا
    image out = load_image_stb(filename, c);
#endif

    إذا((h && w) && (h != out.h || w != out.w)){
        image resized = resize_image(out, w, h);
        free_image(out);
        out = resized;
    }
    أرجع out;
}

image load_image_color(حرف *filename, صحيح w, صحيح h)
{
    أرجع load_image(filename, w, h, 3);
}

image get_image_layer(image m, صحيح l)
{
    image out = make_image(m.w, m.h, 1);
    صحيح i;
    لكل(i = 0; i < m.h*m.w; ++i){
        out.data[i] = m.data[i+l*m.h*m.w];
    }
    أرجع out;
}
فراغ print_image(image m)
{
    صحيح i, j, k;
    لكل(i =0 ; i < m.c; ++i){
        لكل(j =0 ; j < m.h; ++j){
            لكل(k = 0; k < m.w; ++k){
                printf("%.2lf, ", m.data[i*m.h*m.w + j*m.w + k]);
                إذا(k > 30) اكسر;
            }
            printf("\n");
            إذا(j > 30) اكسر;
        }
        printf("\n");
    }
    printf("\n");
}

image collapse_images_vert(image *ims, صحيح n)
{
    صحيح color = 1;
    صحيح border = 1;
    صحيح h,w,c;
    w = ims[0].w;
    h = (ims[0].h + border) * n - border;
    c = ims[0].c;
    إذا(c != 3 || !color){
        w = (w+border)*c - border;
        c = 1;
    }

    image filters = make_image(w, h, c);
    صحيح i,j;
    لكل(i = 0; i < n; ++i){
        صحيح h_offset = i*(ims[0].h+border);
        image copy = copy_image(ims[i]);
        //normalize_image(copy);
        إذا(c == 3 && color){
            embed_image(copy, filters, 0, h_offset);
        }
        وإلا{
            لكل(j = 0; j < copy.c; ++j){
                صحيح w_offset = j*(ims[0].w+border);
                image layer = get_image_layer(copy, j);
                embed_image(layer, filters, w_offset, h_offset);
                free_image(layer);
            }
        }
        free_image(copy);
    }
    أرجع filters;
} 

image collapse_images_horz(image *ims, صحيح n)
{
    صحيح color = 1;
    صحيح border = 1;
    صحيح h,w,c;
    صحيح size = ims[0].h;
    h = size;
    w = (ims[0].w + border) * n - border;
    c = ims[0].c;
    إذا(c != 3 || !color){
        h = (h+border)*c - border;
        c = 1;
    }

    image filters = make_image(w, h, c);
    صحيح i,j;
    لكل(i = 0; i < n; ++i){
        صحيح w_offset = i*(size+border);
        image copy = copy_image(ims[i]);
        //normalize_image(copy);
        إذا(c == 3 && color){
            embed_image(copy, filters, w_offset, 0);
        }
        وإلا{
            لكل(j = 0; j < copy.c; ++j){
                صحيح h_offset = j*(size+border);
                image layer = get_image_layer(copy, j);
                embed_image(layer, filters, w_offset, h_offset);
                free_image(layer);
            }
        }
        free_image(copy);
    }
    أرجع filters;
} 

فراغ show_image_normalized(image im, ثابت حرف *name)
{
    image c = copy_image(im);
    normalize_image(c);
    show_image(c, name, 1);
    free_image(c);
}

فراغ show_images(image *ims, صحيح n, حرف *window)
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

فراغ free_image(image m)
{
    إذا(m.data){
        حرر(m.data);
    }
}
