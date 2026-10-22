#ਅਨ੍ਤਰ੍ਭਾਵ "image.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "blas.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cuda.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <math.h>

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ STB_IMAGE_IMPLEMENTATION
#ਅਨ੍ਤਰ੍ਭਾਵ "stb_image.h"
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ STB_IMAGE_WRITE_IMPLEMENTATION
#ਅਨ੍ਤਰ੍ਭਾਵ "stb_image_write.h"

ਪੂਰਨ windows = 0;

ਦਸ਼ਮਲਵ colors[6][3] = { {1,0,1}, {0,0,1},{0,1,1},{0,1,0},{1,1,0},{1,0,0} };

ਦਸ਼ਮਲਵ get_color(ਪੂਰਨ c, ਪੂਰਨ x, ਪੂਰਨ max)
{
    ਦਸ਼ਮਲਵ ਅਨੁਪਾਤ = ((ਦਸ਼ਮਲਵ)x/max)*5;
    ਪੂਰਨ i = floor(ਅਨੁਪਾਤ);
    ਪੂਰਨ j = ceil(ਅਨੁਪਾਤ);
    ਅਨੁਪਾਤ -= i;
    ਦਸ਼ਮਲਵ r = (1-ਅਨੁਪਾਤ) * colors[i][c] + ਅਨੁਪਾਤ*colors[j][c];
    //printf("%f\n", r);
    ਵਾਪਸ r;
}

image ਨਕਾਬ_ਮੰਜ਼ਿਲ_rgb(image ਨਕਾਬ)
{
    ਪੂਰਨ n = ਨਕਾਬ.c;
    image im = make_image(ਨਕਾਬ.w, ਨਕਾਬ.h, 3);
    ਪੂਰਨ i, j;
    ਲਈ(j = 0; j < n; ++j){
        ਪੂਰਨ ਵਿੱਥ = j*123457 % n;
        ਦਸ਼ਮਲਵ red = get_color(2,ਵਿੱਥ,n);
        ਦਸ਼ਮਲਵ green = get_color(1,ਵਿੱਥ,n);
        ਦਸ਼ਮਲਵ blue = get_color(0,ਵਿੱਥ,n);
        ਲਈ(i = 0; i < im.w*im.h; ++i){
            im.ਅੰਕੜੇ[i + 0*im.w*im.h] += ਨਕਾਬ.ਅੰਕੜੇ[j*im.h*im.w + i]*red;
            im.ਅੰਕੜੇ[i + 1*im.w*im.h] += ਨਕਾਬ.ਅੰਕੜੇ[j*im.h*im.w + i]*green;
            im.ਅੰਕੜੇ[i + 2*im.w*im.h] += ਨਕਾਬ.ਅੰਕੜੇ[j*im.h*im.w + i]*blue;
        }
    }
    ਵਾਪਸ im;
}

ਸਥਿਰ ਦਸ਼ਮਲਵ get_pixel(image m, ਪੂਰਨ x, ਪੂਰਨ y, ਪੂਰਨ c)
{
    ਪ੍ਰਤਿਜ੍ਞਾ(x < m.w && y < m.h && c < m.c);
    ਵਾਪਸ m.ਅੰਕੜੇ[c*m.h*m.w + y*m.w + x];
}
ਸਥਿਰ ਦਸ਼ਮਲਵ get_pixel_extend(image m, ਪੂਰਨ x, ਪੂਰਨ y, ਪੂਰਨ c)
{
    ਜੇ(x < 0 || x >= m.w || y < 0 || y >= m.h) ਵਾਪਸ 0;
    /*
    if(x < 0) x = 0;
    if(x >= m.w) x = m.w-1;
    if(y < 0) y = 0;
    if(y >= m.h) y = m.h-1;
    */
    ਜੇ(c < 0 || c >= m.c) ਵਾਪਸ 0;
    ਵਾਪਸ get_pixel(m, x, y, c);
}
ਸਥਿਰ ਖਾਲੀ ਸੈੱਟ_pixel(image m, ਪੂਰਨ x, ਪੂਰਨ y, ਪੂਰਨ c, ਦਸ਼ਮਲਵ val)
{
    ਜੇ (x < 0 || y < 0 || c < 0 || x >= m.w || y >= m.h || c >= m.c) ਵਾਪਸ;
    ਪ੍ਰਤਿਜ੍ਞਾ(x < m.w && y < m.h && c < m.c);
    m.ਅੰਕੜੇ[c*m.h*m.w + y*m.w + x] = val;
}
ਸਥਿਰ ਖਾਲੀ ਜੋੜੋ_pixel(image m, ਪੂਰਨ x, ਪੂਰਨ y, ਪੂਰਨ c, ਦਸ਼ਮਲਵ val)
{
    ਪ੍ਰਤਿਜ੍ਞਾ(x < m.w && y < m.h && c < m.c);
    m.ਅੰਕੜੇ[c*m.h*m.w + y*m.w + x] += val;
}

ਸਥਿਰ ਦਸ਼ਮਲਵ bilinear_interpolate(image im, ਦਸ਼ਮਲਵ x, ਦਸ਼ਮਲਵ y, ਪੂਰਨ c)
{
    ਪੂਰਨ ix = (ਪੂਰਨ) floorf(x);
    ਪੂਰਨ iy = (ਪੂਰਨ) floorf(y);

    ਦਸ਼ਮਲਵ dx = x - ix;
    ਦਸ਼ਮਲਵ dy = y - iy;

    ਦਸ਼ਮਲਵ val = (1-dy) * (1-dx) * get_pixel_extend(im, ix, iy, c) + 
        dy     * (1-dx) * get_pixel_extend(im, ix, iy+1, c) + 
        (1-dy) *   dx   * get_pixel_extend(im, ix+1, iy, c) +
        dy     *   dx   * get_pixel_extend(im, ix+1, iy+1, c);
    ਵਾਪਸ val;
}


ਖਾਲੀ ਸੰਯੁਕਤ_image(image ਸਰੋਤ, image dest, ਪੂਰਨ dx, ਪੂਰਨ dy)
{
    ਪੂਰਨ x,y,k;
    ਲਈ(k = 0; k < ਸਰੋਤ.c; ++k){
        ਲਈ(y = 0; y < ਸਰੋਤ.h; ++y){
            ਲਈ(x = 0; x < ਸਰੋਤ.w; ++x){
                ਦਸ਼ਮਲਵ val = get_pixel(ਸਰੋਤ, x, y, k);
                ਦਸ਼ਮਲਵ val2 = get_pixel_extend(dest, dx+x, dy+y, k);
                ਸੈੱਟ_pixel(dest, dx+x, dy+y, k, val * val2);
            }
        }
    }
}

image border_image(image a, ਪੂਰਨ border)
{
    image b = make_image(a.w + 2*border, a.h + 2*border, a.c);
    ਪੂਰਨ x,y,k;
    ਲਈ(k = 0; k < b.c; ++k){
        ਲਈ(y = 0; y < b.h; ++y){
            ਲਈ(x = 0; x < b.w; ++x){
                ਦਸ਼ਮਲਵ val = get_pixel_extend(a, x - border, y - border, k);
                ਜੇ(x - border < 0 || x - border >= a.w || y - border < 0 || y - border >= a.h) val = 1;
                ਸੈੱਟ_pixel(b, x, y, k, val);
            }
        }
    }
    ਵਾਪਸ b;
}

image tile_images(image a, image b, ਪੂਰਨ dx)
{
    ਜੇ(a.w == 0) ਵਾਪਸ copy_image(b);
    image c = make_image(a.w + b.w + dx, (a.h > b.h) ? a.h : b.h, (a.c > b.c) ? a.c : b.c);
    fill_cpu(c.w*c.h*c.c, 1, c.ਅੰਕੜੇ, 1);
    embed_image(a, c, 0, 0); 
    ਸੰਯੁਕਤ_image(b, c, a.w + dx, 0);
    ਵਾਪਸ c;
}

image get_label(image **characters, ਅੱਖਰ *string, ਪੂਰਨ ਮਾਪ)
{
    ਮਾਪ = ਮਾਪ/10;
    ਜੇ(ਮਾਪ > 7) ਮਾਪ = 7;
    image label = make_ਖਾਲੀ_ਥਾਵਾਂ_image(0,0,0);
    ਜਦੋਂ(*string){
        image l = characters[ਮਾਪ][(ਪੂਰਨ)*string];
        image n = tile_images(label, l, -ਮਾਪ - 1 + (ਮਾਪ+1)/2);
        free_image(label);
        label = n;
        ++string;
    }
    image b = border_image(label, label.h*.25);
    free_image(label);
    ਵਾਪਸ b;
}

ਖਾਲੀ draw_label(image a, ਪੂਰਨ r, ਪੂਰਨ c, image label, ਅਚਲ ਦਸ਼ਮਲਵ *rgb)
{
    ਪੂਰਨ w = label.w;
    ਪੂਰਨ h = label.h;
    ਜੇ (r - h >= 0) r = r - h;

    ਪੂਰਨ i, j, k;
    ਲਈ(j = 0; j < h && j + r < a.h; ++j){
        ਲਈ(i = 0; i < w && i + c < a.w; ++i){
            ਲਈ(k = 0; k < label.c; ++k){
                ਦਸ਼ਮਲਵ val = get_pixel(label, i, j, k);
                ਸੈੱਟ_pixel(a, i+c, j+r, k, rgb[k] * val);
            }
        }
    }
}

ਖਾਲੀ draw_box(image a, ਪੂਰਨ x1, ਪੂਰਨ y1, ਪੂਰਨ x2, ਪੂਰਨ y2, ਦਸ਼ਮਲਵ r, ਦਸ਼ਮਲਵ g, ਦਸ਼ਮਲਵ b)
{
    //normalize_image(a);
    ਪੂਰਨ i;
    ਜੇ(x1 < 0) x1 = 0;
    ਜੇ(x1 >= a.w) x1 = a.w-1;
    ਜੇ(x2 < 0) x2 = 0;
    ਜੇ(x2 >= a.w) x2 = a.w-1;

    ਜੇ(y1 < 0) y1 = 0;
    ਜੇ(y1 >= a.h) y1 = a.h-1;
    ਜੇ(y2 < 0) y2 = 0;
    ਜੇ(y2 >= a.h) y2 = a.h-1;

    ਲਈ(i = x1; i <= x2; ++i){
        a.ਅੰਕੜੇ[i + y1*a.w + 0*a.w*a.h] = r;
        a.ਅੰਕੜੇ[i + y2*a.w + 0*a.w*a.h] = r;

        a.ਅੰਕੜੇ[i + y1*a.w + 1*a.w*a.h] = g;
        a.ਅੰਕੜੇ[i + y2*a.w + 1*a.w*a.h] = g;

        a.ਅੰਕੜੇ[i + y1*a.w + 2*a.w*a.h] = b;
        a.ਅੰਕੜੇ[i + y2*a.w + 2*a.w*a.h] = b;
    }
    ਲਈ(i = y1; i <= y2; ++i){
        a.ਅੰਕੜੇ[x1 + i*a.w + 0*a.w*a.h] = r;
        a.ਅੰਕੜੇ[x2 + i*a.w + 0*a.w*a.h] = r;

        a.ਅੰਕੜੇ[x1 + i*a.w + 1*a.w*a.h] = g;
        a.ਅੰਕੜੇ[x2 + i*a.w + 1*a.w*a.h] = g;

        a.ਅੰਕੜੇ[x1 + i*a.w + 2*a.w*a.h] = b;
        a.ਅੰਕੜੇ[x2 + i*a.w + 2*a.w*a.h] = b;
    }
}

ਖਾਲੀ draw_box_ਚੌੜਾਈ(image a, ਪੂਰਨ x1, ਪੂਰਨ y1, ਪੂਰਨ x2, ਪੂਰਨ y2, ਪੂਰਨ w, ਦਸ਼ਮਲਵ r, ਦਸ਼ਮਲਵ g, ਦਸ਼ਮਲਵ b)
{
    ਪੂਰਨ i;
    ਲਈ(i = 0; i < w; ++i){
        draw_box(a, x1+i, y1+i, x2-i, y2-i, r, g, b);
    }
}

ਖਾਲੀ draw_bbox(image a, box bbox, ਪੂਰਨ w, ਦਸ਼ਮਲਵ r, ਦਸ਼ਮਲਵ g, ਦਸ਼ਮਲਵ b)
{
    ਪੂਰਨ ਖੱਬਾ  = (bbox.x-bbox.w/2)*a.w;
    ਪੂਰਨ ਸੱਜਾ = (bbox.x+bbox.w/2)*a.w;
    ਪੂਰਨ ਸਿਖਰ   = (bbox.y-bbox.h/2)*a.h;
    ਪੂਰਨ bot   = (bbox.y+bbox.h/2)*a.h;

    ਪੂਰਨ i;
    ਲਈ(i = 0; i < w; ++i){
        draw_box(a, ਖੱਬਾ+i, ਸਿਖਰ+i, ਸੱਜਾ-i, bot-i, r, g, b);
    }
}

image **ਲੱਦੋ_alphabet()
{
    ਪੂਰਨ i, j;
    ਅਚਲ ਪੂਰਨ nsize = 8;
    image **alphabets = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(nsize, ਆਕਾਰ(image));
    ਲਈ(j = 0; j < nsize; ++j){
        alphabets[j] = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(128, ਆਕਾਰ(image));
        ਲਈ(i = 32; i < 127; ++i){
            ਅੱਖਰ buff[256];
            sprintf(buff, "data/labels/%d_%d.png", i, j);
            alphabets[j][i] = ਲੱਦੋ_image_color(buff, 0, 0);
        }
    }
    ਵਾਪਸ alphabets;
}

ਖਾਲੀ draw_detections(image im, detection *dets, ਪੂਰਨ ਅੰਕ, ਦਸ਼ਮਲਵ thresh, ਅੱਖਰ **ਨਾਂ, image **alphabet, ਪੂਰਨ classes)
{
    ਪੂਰਨ i,j;

    ਲਈ(i = 0; i < ਅੰਕ; ++i){
        ਅੱਖਰ labelstr[4096] = {0};
        ਪੂਰਨ class = -1;
        ਲਈ(j = 0; j < classes; ++j){
            ਜੇ (dets[i].prob[j] > thresh){
                ਜੇ (class < 0) {
                    strcat(labelstr, ਨਾਂ[j]);
                    class = j;
                } ਨਹੀਂਤਾਂ {
                    strcat(labelstr, ", ");
                    strcat(labelstr, ਨਾਂ[j]);
                }
                ਲਿਖੋ("%s: %.0f%%\n", ਨਾਂ[j], dets[i].prob[j]*100);
            }
        }
        ਜੇ(class >= 0){
            ਪੂਰਨ ਚੌੜਾਈ = im.h * .006;

            /*
               if(0){
               width = pow(prob, 1./2.)*10+1;
               alphabet = 0;
               }
             */

            //printf("%d %s: %.0f%%\n", i, names[class], prob*100);
            ਪੂਰਨ ਵਿੱਥ = class*123457 % classes;
            ਦਸ਼ਮਲਵ red = get_color(2,ਵਿੱਥ,classes);
            ਦਸ਼ਮਲਵ green = get_color(1,ਵਿੱਥ,classes);
            ਦਸ਼ਮਲਵ blue = get_color(0,ਵਿੱਥ,classes);
            ਦਸ਼ਮਲਵ rgb[3];

            //width = prob*20+2;

            rgb[0] = red;
            rgb[1] = green;
            rgb[2] = blue;
            box b = dets[i].bbox;
            //printf("%f %f %f %f\n", b.x, b.y, b.w, b.h);

            ਪੂਰਨ ਖੱਬਾ  = (b.x-b.w/2.)*im.w;
            ਪੂਰਨ ਸੱਜਾ = (b.x+b.w/2.)*im.w;
            ਪੂਰਨ ਸਿਖਰ   = (b.y-b.h/2.)*im.h;
            ਪੂਰਨ bot   = (b.y+b.h/2.)*im.h;

            ਜੇ(ਖੱਬਾ < 0) ਖੱਬਾ = 0;
            ਜੇ(ਸੱਜਾ > im.w-1) ਸੱਜਾ = im.w-1;
            ਜੇ(ਸਿਖਰ < 0) ਸਿਖਰ = 0;
            ਜੇ(bot > im.h-1) bot = im.h-1;

            draw_box_ਚੌੜਾਈ(im, ਖੱਬਾ, ਸਿਖਰ, ਸੱਜਾ, bot, ਚੌੜਾਈ, red, green, blue);
            ਜੇ (alphabet) {
                image label = get_label(alphabet, labelstr, (im.h*.03));
                draw_label(im, ਸਿਖਰ + ਚੌੜਾਈ, ਖੱਬਾ, label, rgb);
                free_image(label);
            }
            ਜੇ (dets[i].ਨਕਾਬ){
                image ਨਕਾਬ = float_ਮੰਜ਼ਿਲ_image(14, 14, 1, dets[i].ਨਕਾਬ);
                image resized_ਨਕਾਬ = resize_image(ਨਕਾਬ, b.w*im.w, b.h*im.h);
                image tmask = ਹੱਦ_image(resized_ਨਕਾਬ, .5);
                embed_image(tmask, im, ਖੱਬਾ, ਸਿਖਰ);
                free_image(ਨਕਾਬ);
                free_image(resized_ਨਕਾਬ);
                free_image(tmask);
            }
        }
    }
}

ਖਾਲੀ transpose_image(image im)
{
    ਪ੍ਰਤਿਜ੍ਞਾ(im.w == im.h);
    ਪੂਰਨ n, m;
    ਪੂਰਨ c;
    ਲਈ(c = 0; c < im.c; ++c){
        ਲਈ(n = 0; n < im.w-1; ++n){
            ਲਈ(m = n + 1; m < im.w; ++m){
                ਦਸ਼ਮਲਵ ਅਦਲਾ_ਬਦਲੀ = im.ਅੰਕੜੇ[m + im.w*(n + im.h*c)];
                im.ਅੰਕੜੇ[m + im.w*(n + im.h*c)] = im.ਅੰਕੜੇ[n + im.w*(m + im.h*c)];
                im.ਅੰਕੜੇ[n + im.w*(m + im.h*c)] = ਅਦਲਾ_ਬਦਲੀ;
            }
        }
    }
}

ਖਾਲੀ rotate_image_cw(image im, ਪੂਰਨ times)
{
    ਪ੍ਰਤਿਜ੍ਞਾ(im.w == im.h);
    times = (times + 400) % 4;
    ਪੂਰਨ i, x, y, c;
    ਪੂਰਨ n = im.w;
    ਲਈ(i = 0; i < times; ++i){
        ਲਈ(c = 0; c < im.c; ++c){
            ਲਈ(x = 0; x < n/2; ++x){
                ਲਈ(y = 0; y < (n-1)/2 + 1; ++y){
                    ਦਸ਼ਮਲਵ ਆਰਜ਼ੀ = im.ਅੰਕੜੇ[y + im.w*(x + im.h*c)];
                    im.ਅੰਕੜੇ[y + im.w*(x + im.h*c)] = im.ਅੰਕੜੇ[n-1-x + im.w*(y + im.h*c)];
                    im.ਅੰਕੜੇ[n-1-x + im.w*(y + im.h*c)] = im.ਅੰਕੜੇ[n-1-y + im.w*(n-1-x + im.h*c)];
                    im.ਅੰਕੜੇ[n-1-y + im.w*(n-1-x + im.h*c)] = im.ਅੰਕੜੇ[x + im.w*(n-1-y + im.h*c)];
                    im.ਅੰਕੜੇ[x + im.w*(n-1-y + im.h*c)] = ਆਰਜ਼ੀ;
                }
            }
        }
    }
}

ਖਾਲੀ flip_image(image a)
{
    ਪੂਰਨ i,j,k;
    ਲਈ(k = 0; k < a.c; ++k){
        ਲਈ(i = 0; i < a.h; ++i){
            ਲਈ(j = 0; j < a.w/2; ++j){
                ਪੂਰਨ ਸੂਚਕਾਂਕ = j + a.w*(i + a.h*(k));
                ਪੂਰਨ flip = (a.w - j - 1) + a.w*(i + a.h*(k));
                ਦਸ਼ਮਲਵ ਅਦਲਾ_ਬਦਲੀ = a.ਅੰਕੜੇ[flip];
                a.ਅੰਕੜੇ[flip] = a.ਅੰਕੜੇ[ਸੂਚਕਾਂਕ];
                a.ਅੰਕੜੇ[ਸੂਚਕਾਂਕ] = ਅਦਲਾ_ਬਦਲੀ;
            }
        }
    }
}

image image_ਦੂਰੀ(image a, image b)
{
    ਪੂਰਨ i,j;
    image dist = make_image(a.w, a.h, 1);
    ਲਈ(i = 0; i < a.c; ++i){
        ਲਈ(j = 0; j < a.h*a.w; ++j){
            dist.ਅੰਕੜੇ[j] += ਘਾਤ(a.ਅੰਕੜੇ[i*a.h*a.w+j]-b.ਅੰਕੜੇ[i*a.h*a.w+j],2);
        }
    }
    ਲਈ(j = 0; j < a.h*a.w; ++j){
        dist.ਅੰਕੜੇ[j] = ਵਰਗਮੂਲ(dist.ਅੰਕੜੇ[j]);
    }
    ਵਾਪਸ dist;
}

ਖਾਲੀ ghost_image(image ਸਰੋਤ, image dest, ਪੂਰਨ dx, ਪੂਰਨ dy)
{
    ਪੂਰਨ x,y,k;
    ਦਸ਼ਮਲਵ max_dist = ਵਰਗਮੂਲ((-ਸਰੋਤ.w/2. + .5)*(-ਸਰੋਤ.w/2. + .5));
    ਲਈ(k = 0; k < ਸਰੋਤ.c; ++k){
        ਲਈ(y = 0; y < ਸਰੋਤ.h; ++y){
            ਲਈ(x = 0; x < ਸਰੋਤ.w; ++x){
                ਦਸ਼ਮਲਵ dist = ਵਰਗਮੂਲ((x - ਸਰੋਤ.w/2. + .5)*(x - ਸਰੋਤ.w/2. + .5) + (y - ਸਰੋਤ.h/2. + .5)*(y - ਸਰੋਤ.h/2. + .5));
                ਦਸ਼ਮਲਵ alpha = (1 - dist/max_dist);
                ਜੇ(alpha < 0) alpha = 0;
                ਦਸ਼ਮਲਵ v1 = get_pixel(ਸਰੋਤ, x,y,k);
                ਦਸ਼ਮਲਵ v2 = get_pixel(dest, dx+x,dy+y,k);
                ਦਸ਼ਮਲਵ val = alpha*v1 + (1-alpha)*v2;
                ਸੈੱਟ_pixel(dest, dx+x, dy+y, k, val);
            }
        }
    }
}

ਖਾਲੀ blocky_image(image im, ਪੂਰਨ s)
{
    ਪੂਰਨ i,j,k;
    ਲਈ(k = 0; k < im.c; ++k){
        ਲਈ(j = 0; j < im.h; ++j){
            ਲਈ(i = 0; i < im.w; ++i){
                im.ਅੰਕੜੇ[i + im.w*(j + im.h*k)] = im.ਅੰਕੜੇ[i/s*s + im.w*(j/s*s + im.h*k)];
            }
        }
    }
}

ਖਾਲੀ censor_image(image im, ਪੂਰਨ dx, ਪੂਰਨ dy, ਪੂਰਨ w, ਪੂਰਨ h)
{
    ਪੂਰਨ i,j,k;
    ਪੂਰਨ s = 32;
    ਜੇ(dx < 0) dx = 0;
    ਜੇ(dy < 0) dy = 0;

    ਲਈ(k = 0; k < im.c; ++k){
        ਲਈ(j = dy; j < dy + h && j < im.h; ++j){
            ਲਈ(i = dx; i < dx + w && i < im.w; ++i){
                im.ਅੰਕੜੇ[i + im.w*(j + im.h*k)] = im.ਅੰਕੜੇ[i/s*s + im.w*(j/s*s + im.h*k)];
                //im.data[i + j*im.w + k*im.w*im.h] = 0;
            }
        }
    }
}

ਖਾਲੀ embed_image(image ਸਰੋਤ, image dest, ਪੂਰਨ dx, ਪੂਰਨ dy)
{
    ਪੂਰਨ x,y,k;
    ਲਈ(k = 0; k < ਸਰੋਤ.c; ++k){
        ਲਈ(y = 0; y < ਸਰੋਤ.h; ++y){
            ਲਈ(x = 0; x < ਸਰੋਤ.w; ++x){
                ਦਸ਼ਮਲਵ val = get_pixel(ਸਰੋਤ, x,y,k);
                ਸੈੱਟ_pixel(dest, dx+x, dy+y, k, val);
            }
        }
    }
}

image collapse_image_layers(image ਸਰੋਤ, ਪੂਰਨ border)
{
    ਪੂਰਨ h = ਸਰੋਤ.h;
    h = (h+border)*ਸਰੋਤ.c - border;
    image dest = make_image(ਸਰੋਤ.w, h, 1);
    ਪੂਰਨ i;
    ਲਈ(i = 0; i < ਸਰੋਤ.c; ++i){
        image layer = get_image_layer(ਸਰੋਤ, i);
        ਪੂਰਨ h_ਵਿੱਥ = i*(ਸਰੋਤ.h+border);
        embed_image(layer, dest, 0, h_ਵਿੱਥ);
        free_image(layer);
    }
    ਵਾਪਸ dest;
}

ਖਾਲੀ constrain_image(image im)
{
    ਪੂਰਨ i;
    ਲਈ(i = 0; i < im.w*im.h*im.c; ++i){
        ਜੇ(im.ਅੰਕੜੇ[i] < 0) im.ਅੰਕੜੇ[i] = 0;
        ਜੇ(im.ਅੰਕੜੇ[i] > 1) im.ਅੰਕੜੇ[i] = 1;
    }
}

ਖਾਲੀ normalize_image(image p)
{
    ਪੂਰਨ i;
    ਦਸ਼ਮਲਵ min = 9999999;
    ਦਸ਼ਮਲਵ max = -999999;

    ਲਈ(i = 0; i < p.h*p.w*p.c; ++i){
        ਦਸ਼ਮਲਵ v = p.ਅੰਕੜੇ[i];
        ਜੇ(v < min) min = v;
        ਜੇ(v > max) max = v;
    }
    ਜੇ(max - min < .000000001){
        min = 0;
        max = 1;
    }
    ਲਈ(i = 0; i < p.c*p.w*p.h; ++i){
        p.ਅੰਕੜੇ[i] = (p.ਅੰਕੜੇ[i] - min)/(max-min);
    }
}

ਖਾਲੀ normalize_image2(image p)
{
    ਦਸ਼ਮਲਵ *min = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(p.c, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    ਦਸ਼ਮਲਵ *max = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(p.c, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    ਪੂਰਨ i,j;
    ਲਈ(i = 0; i < p.c; ++i) min[i] = max[i] = p.ਅੰਕੜੇ[i*p.h*p.w];

    ਲਈ(j = 0; j < p.c; ++j){
        ਲਈ(i = 0; i < p.h*p.w; ++i){
            ਦਸ਼ਮਲਵ v = p.ਅੰਕੜੇ[i+j*p.h*p.w];
            ਜੇ(v < min[j]) min[j] = v;
            ਜੇ(v > max[j]) max[j] = v;
        }
    }
    ਲਈ(i = 0; i < p.c; ++i){
        ਜੇ(max[i] - min[i] < .000000001){
            min[i] = 0;
            max[i] = 1;
        }
    }
    ਲਈ(j = 0; j < p.c; ++j){
        ਲਈ(i = 0; i < p.w*p.h; ++i){
            p.ਅੰਕੜੇ[i+j*p.h*p.w] = (p.ਅੰਕੜੇ[i+j*p.h*p.w] - min[j])/(max[j]-min[j]);
        }
    }
    ਮੁਕਤ(min);
    ਮੁਕਤ(max);
}

ਖਾਲੀ copy_image_into(image src, image dest)
{
    ਸਮ੍ਰਿਤੀ_ਨਕਲ(dest.ਅੰਕੜੇ, src.ਅੰਕੜੇ, src.h*src.w*src.c*ਆਕਾਰ(ਦਸ਼ਮਲਵ));
}

image copy_image(image p)
{
    image copy = p;
    copy.ਅੰਕੜੇ = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(p.h*p.w*p.c, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    ਸਮ੍ਰਿਤੀ_ਨਕਲ(copy.ਅੰਕੜੇ, p.ਅੰਕੜੇ, p.h*p.w*p.c*ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    ਵਾਪਸ copy;
}

ਖਾਲੀ rgbgr_image(image im)
{
    ਪੂਰਨ i;
    ਲਈ(i = 0; i < im.w*im.h; ++i){
        ਦਸ਼ਮਲਵ ਅਦਲਾ_ਬਦਲੀ = im.ਅੰਕੜੇ[i];
        im.ਅੰਕੜੇ[i] = im.ਅੰਕੜੇ[i+im.w*im.h*2];
        im.ਅੰਕੜੇ[i+im.w*im.h*2] = ਅਦਲਾ_ਬਦਲੀ;
    }
}

ਪੂਰਨ show_image(image p, ਅਚਲ ਅੱਖਰ *name, ਪੂਰਨ ms)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ OPENCV
    ਪੂਰਨ c = show_image_cv(p, name, ms);
    ਵਾਪਸ c;
#ਨਹੀਂਤਾਂ
    fprintf(stderr, "Not compiled with OpenCV, saving to %s.png instead\n", name);
    save_image(p, name);
    ਵਾਪਸ -1;
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ save_image_options(image im, ਅਚਲ ਅੱਖਰ *name, IMTYPE f, ਪੂਰਨ quality)
{
    ਅੱਖਰ buff[256];
    //sprintf(buff, "%s (%d)", name, windows);
    ਜੇ(f == PNG)       sprintf(buff, "%s.png", name);
    ਨਹੀਂਤਾਂ ਜੇ (f == BMP) sprintf(buff, "%s.bmp", name);
    ਨਹੀਂਤਾਂ ਜੇ (f == TGA) sprintf(buff, "%s.tga", name);
    ਨਹੀਂਤਾਂ ਜੇ (f == JPG) sprintf(buff, "%s.jpg", name);
    ਨਹੀਂਤਾਂ               sprintf(buff, "%s.png", name);
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *ਅੰਕੜੇ = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(im.w*im.h*im.c, ਆਕਾਰ(ਅੱਖਰ));
    ਪੂਰਨ i,k;
    ਲਈ(k = 0; k < im.c; ++k){
        ਲਈ(i = 0; i < im.w*im.h; ++i){
            ਅੰਕੜੇ[i*im.c+k] = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ) (255*im.ਅੰਕੜੇ[i + k*im.w*im.h]);
        }
    }
    ਪੂਰਨ success = 0;
    ਜੇ(f == PNG)       success = stbi_write_png(buff, im.w, im.h, im.c, ਅੰਕੜੇ, im.w*im.c);
    ਨਹੀਂਤਾਂ ਜੇ (f == BMP) success = stbi_write_bmp(buff, im.w, im.h, im.c, ਅੰਕੜੇ);
    ਨਹੀਂਤਾਂ ਜੇ (f == TGA) success = stbi_write_tga(buff, im.w, im.h, im.c, ਅੰਕੜੇ);
    ਨਹੀਂਤਾਂ ਜੇ (f == JPG) success = stbi_write_jpg(buff, im.w, im.h, im.c, ਅੰਕੜੇ, quality);
    ਮੁਕਤ(ਅੰਕੜੇ);
    ਜੇ(!success) fprintf(stderr, "Failed to write image %s\n", buff);
}

ਖਾਲੀ save_image(image im, ਅਚਲ ਅੱਖਰ *name)
{
    save_image_options(im, name, JPG, 80);
}

ਖਾਲੀ show_image_layers(image p, ਅੱਖਰ *name)
{
    ਪੂਰਨ i;
    ਅੱਖਰ buff[256];
    ਲਈ(i = 0; i < p.c; ++i){
        sprintf(buff, "%s - Layer %d", name, i);
        image layer = get_image_layer(p, i);
        show_image(layer, buff, 1);
        free_image(layer);
    }
}

ਖਾਲੀ show_image_collapsed(image p, ਅੱਖਰ *name)
{
    image c = collapse_image_layers(p, 1);
    show_image(c, name, 1);
    free_image(c);
}

image make_ਖਾਲੀ_ਥਾਵਾਂ_image(ਪੂਰਨ w, ਪੂਰਨ h, ਪੂਰਨ c)
{
    image ਬਾਹਰਲਾ;
    ਬਾਹਰਲਾ.ਅੰਕੜੇ = 0;
    ਬਾਹਰਲਾ.h = h;
    ਬਾਹਰਲਾ.w = w;
    ਬਾਹਰਲਾ.c = c;
    ਵਾਪਸ ਬਾਹਰਲਾ;
}

image make_image(ਪੂਰਨ w, ਪੂਰਨ h, ਪੂਰਨ c)
{
    image ਬਾਹਰਲਾ = make_ਖਾਲੀ_ਥਾਵਾਂ_image(w,h,c);
    ਬਾਹਰਲਾ.ਅੰਕੜੇ = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(h*w*c, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    ਵਾਪਸ ਬਾਹਰਲਾ;
}

image make_random_image(ਪੂਰਨ w, ਪੂਰਨ h, ਪੂਰਨ c)
{
    image ਬਾਹਰਲਾ = make_ਖਾਲੀ_ਥਾਵਾਂ_image(w,h,c);
    ਬਾਹਰਲਾ.ਅੰਕੜੇ = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(h*w*c, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    ਪੂਰਨ i;
    ਲਈ(i = 0; i < w*h*c; ++i){
        ਬਾਹਰਲਾ.ਅੰਕੜੇ[i] = (rand_ਸਾਧਾਰਨ() * .25) + .5;
    }
    ਵਾਪਸ ਬਾਹਰਲਾ;
}

image float_ਮੰਜ਼ਿਲ_image(ਪੂਰਨ w, ਪੂਰਨ h, ਪੂਰਨ c, ਦਸ਼ਮਲਵ *ਅੰਕੜੇ)
{
    image ਬਾਹਰਲਾ = make_ਖਾਲੀ_ਥਾਵਾਂ_image(w,h,c);
    ਬਾਹਰਲਾ.ਅੰਕੜੇ = ਅੰਕੜੇ;
    ਵਾਪਸ ਬਾਹਰਲਾ;
}

ਖਾਲੀ ਰੱਖੋ_image(image im, ਪੂਰਨ w, ਪੂਰਨ h, ਪੂਰਨ dx, ਪੂਰਨ dy, image canvas)
{
    ਪੂਰਨ x, y, c;
    ਲਈ(c = 0; c < im.c; ++c){
        ਲਈ(y = 0; y < h; ++y){
            ਲਈ(x = 0; x < w; ++x){
                ਦਸ਼ਮਲਵ rx = ((ਦਸ਼ਮਲਵ)x / w) * im.w;
                ਦਸ਼ਮਲਵ ry = ((ਦਸ਼ਮਲਵ)y / h) * im.h;
                ਦਸ਼ਮਲਵ val = bilinear_interpolate(im, rx, ry, c);
                ਸੈੱਟ_pixel(canvas, x + dx, y + dy, c, val);
            }
        }
    }
}

image center_crop_image(image im, ਪੂਰਨ w, ਪੂਰਨ h)
{
    ਪੂਰਨ m = (im.w < im.h) ? im.w : im.h;   
    image c = crop_image(im, (im.w - m) / 2, (im.h - m)/2, m, m);
    image r = resize_image(c, w, h);
    free_image(c);
    ਵਾਪਸ r;
}

image rotate_crop_image(image im, ਦਸ਼ਮਲਵ rad, ਦਸ਼ਮਲਵ s, ਪੂਰਨ w, ਪੂਰਨ h, ਦਸ਼ਮਲਵ dx, ਦਸ਼ਮਲਵ dy, ਦਸ਼ਮਲਵ aspect)
{
    ਪੂਰਨ x, y, c;
    ਦਸ਼ਮਲਵ cx = im.w/2.;
    ਦਸ਼ਮਲਵ cy = im.h/2.;
    image rot = make_image(w, h, im.c);
    ਲਈ(c = 0; c < im.c; ++c){
        ਲਈ(y = 0; y < h; ++y){
            ਲਈ(x = 0; x < w; ++x){
                ਦਸ਼ਮਲਵ rx = ਕੋਜੀਵਾ(rad)*((x - w/2.)/s*aspect + dx/s*aspect) - ਜੀਵਾ(rad)*((y - h/2.)/s + dy/s) + cx;
                ਦਸ਼ਮਲਵ ry = ਜੀਵਾ(rad)*((x - w/2.)/s*aspect + dx/s*aspect) + ਕੋਜੀਵਾ(rad)*((y - h/2.)/s + dy/s) + cy;
                ਦਸ਼ਮਲਵ val = bilinear_interpolate(im, rx, ry, c);
                ਸੈੱਟ_pixel(rot, x, y, c, val);
            }
        }
    }
    ਵਾਪਸ rot;
}

image rotate_image(image im, ਦਸ਼ਮਲਵ rad)
{
    ਪੂਰਨ x, y, c;
    ਦਸ਼ਮਲਵ cx = im.w/2.;
    ਦਸ਼ਮਲਵ cy = im.h/2.;
    image rot = make_image(im.w, im.h, im.c);
    ਲਈ(c = 0; c < im.c; ++c){
        ਲਈ(y = 0; y < im.h; ++y){
            ਲਈ(x = 0; x < im.w; ++x){
                ਦਸ਼ਮਲਵ rx = ਕੋਜੀਵਾ(rad)*(x-cx) - ਜੀਵਾ(rad)*(y-cy) + cx;
                ਦਸ਼ਮਲਵ ry = ਜੀਵਾ(rad)*(x-cx) + ਕੋਜੀਵਾ(rad)*(y-cy) + cy;
                ਦਸ਼ਮਲਵ val = bilinear_interpolate(im, rx, ry, c);
                ਸੈੱਟ_pixel(rot, x, y, c, val);
            }
        }
    }
    ਵਾਪਸ rot;
}

ਖਾਲੀ fill_image(image m, ਦਸ਼ਮਲਵ s)
{
    ਪੂਰਨ i;
    ਲਈ(i = 0; i < m.h*m.w*m.c; ++i) m.ਅੰਕੜੇ[i] = s;
}

ਖਾਲੀ translate_image(image m, ਦਸ਼ਮਲਵ s)
{
    ਪੂਰਨ i;
    ਲਈ(i = 0; i < m.h*m.w*m.c; ++i) m.ਅੰਕੜੇ[i] += s;
}

ਖਾਲੀ scale_image(image m, ਦਸ਼ਮਲਵ s)
{
    ਪੂਰਨ i;
    ਲਈ(i = 0; i < m.h*m.w*m.c; ++i) m.ਅੰਕੜੇ[i] *= s;
}

image crop_image(image im, ਪੂਰਨ dx, ਪੂਰਨ dy, ਪੂਰਨ w, ਪੂਰਨ h)
{
    image cropped = make_image(w, h, im.c);
    ਪੂਰਨ i, j, k;
    ਲਈ(k = 0; k < im.c; ++k){
        ਲਈ(j = 0; j < h; ++j){
            ਲਈ(i = 0; i < w; ++i){
                ਪੂਰਨ r = j + dy;
                ਪੂਰਨ c = i + dx;
                ਦਸ਼ਮਲਵ val = 0;
                r = constrain_int(r, 0, im.h-1);
                c = constrain_int(c, 0, im.w-1);
                val = get_pixel(im, c, r, k);
                ਸੈੱਟ_pixel(cropped, i, j, k, val);
            }
        }
    }
    ਵਾਪਸ cropped;
}

ਪੂਰਨ ਉੱਤਮ_3d_ਖਿਸਕਾਓ_r(image a, image b, ਪੂਰਨ min, ਪੂਰਨ max)
{
    ਜੇ(min == max) ਵਾਪਸ min;
    ਪੂਰਨ mid = floor((min + max) / 2.);
    image c1 = crop_image(b, 0, mid, b.w, b.h);
    image c2 = crop_image(b, 0, mid+1, b.w, b.h);
    ਦਸ਼ਮਲਵ d1 = dist_array(c1.ਅੰਕੜੇ, a.ਅੰਕੜੇ, a.w*a.h*a.c, 10);
    ਦਸ਼ਮਲਵ d2 = dist_array(c2.ਅੰਕੜੇ, a.ਅੰਕੜੇ, a.w*a.h*a.c, 10);
    free_image(c1);
    free_image(c2);
    ਜੇ(d1 < d2) ਵਾਪਸ ਉੱਤਮ_3d_ਖਿਸਕਾਓ_r(a, b, min, mid);
    ਨਹੀਂਤਾਂ ਵਾਪਸ ਉੱਤਮ_3d_ਖਿਸਕਾਓ_r(a, b, mid+1, max);
}

ਪੂਰਨ ਉੱਤਮ_3d_ਖਿਸਕਾਓ(image a, image b, ਪੂਰਨ min, ਪੂਰਨ max)
{
    ਪੂਰਨ i;
    ਪੂਰਨ ਉੱਤਮ = 0;
    ਦਸ਼ਮਲਵ ਉੱਤਮ_ਦੂਰੀ = FLT_MAX;
    ਲਈ(i = min; i <= max; i += 2){
        image c = crop_image(b, 0, i, b.w, b.h);
        ਦਸ਼ਮਲਵ d = dist_array(c.ਅੰਕੜੇ, a.ਅੰਕੜੇ, a.w*a.h*a.c, 100);
        ਜੇ(d < ਉੱਤਮ_ਦੂਰੀ){
            ਉੱਤਮ_ਦੂਰੀ = d;
            ਉੱਤਮ = i;
        }
        ਲਿਖੋ("%d %f\n", i, d);
        free_image(c);
    }
    ਵਾਪਸ ਉੱਤਮ;
}

ਖਾਲੀ ਸੰਯੁਕਤ_3d(ਅੱਖਰ *f1, ਅੱਖਰ *f2, ਅੱਖਰ *ਬਾਹਰਲਾ, ਪੂਰਨ delta)
{
    ਜੇ(!ਬਾਹਰਲਾ) ਬਾਹਰਲਾ = "out";
    image a = ਲੱਦੋ_image(f1, 0,0,0);
    image b = ਲੱਦੋ_image(f2, 0,0,0);
    ਪੂਰਨ ਖਿਸਕਾਓ = ਉੱਤਮ_3d_ਖਿਸਕਾਓ_r(a, b, -a.h/100, a.h/100);

    image c1 = crop_image(b, 10, ਖਿਸਕਾਓ, b.w, b.h);
    ਦਸ਼ਮਲਵ d1 = dist_array(c1.ਅੰਕੜੇ, a.ਅੰਕੜੇ, a.w*a.h*a.c, 100);
    image c2 = crop_image(b, -10, ਖਿਸਕਾਓ, b.w, b.h);
    ਦਸ਼ਮਲਵ d2 = dist_array(c2.ਅੰਕੜੇ, a.ਅੰਕੜੇ, a.w*a.h*a.c, 100);

    ਜੇ(d2 < d1 && 0){
        image ਅਦਲਾ_ਬਦਲੀ = a;
        a = b;
        b = ਅਦਲਾ_ਬਦਲੀ;
        ਖਿਸਕਾਓ = -ਖਿਸਕਾਓ;
        ਲਿਖੋ("swapped, %d\n", ਖਿਸਕਾਓ);
    }
    ਨਹੀਂਤਾਂ{
        ਲਿਖੋ("%d\n", ਖਿਸਕਾਓ);
    }

    image c = crop_image(b, delta, ਖਿਸਕਾਓ, a.w, a.h);
    ਪੂਰਨ i;
    ਲਈ(i = 0; i < c.w*c.h; ++i){
        c.ਅੰਕੜੇ[i] = a.ਅੰਕੜੇ[i];
    }
    save_image(c, ਬਾਹਰਲਾ);
}

ਖਾਲੀ letterbox_image_into(image im, ਪੂਰਨ w, ਪੂਰਨ h, image boxed)
{
    ਪੂਰਨ new_w = im.w;
    ਪੂਰਨ new_h = im.h;
    ਜੇ (((ਦਸ਼ਮਲਵ)w/im.w) < ((ਦਸ਼ਮਲਵ)h/im.h)) {
        new_w = w;
        new_h = (im.h * w)/im.w;
    } ਨਹੀਂਤਾਂ {
        new_h = h;
        new_w = (im.w * h)/im.h;
    }
    image resized = resize_image(im, new_w, new_h);
    embed_image(resized, boxed, (w-new_w)/2, (h-new_h)/2); 
    free_image(resized);
}

image letterbox_image(image im, ਪੂਰਨ w, ਪੂਰਨ h)
{
    ਪੂਰਨ new_w = im.w;
    ਪੂਰਨ new_h = im.h;
    ਜੇ (((ਦਸ਼ਮਲਵ)w/im.w) < ((ਦਸ਼ਮਲਵ)h/im.h)) {
        new_w = w;
        new_h = (im.h * w)/im.w;
    } ਨਹੀਂਤਾਂ {
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
    ਵਾਪਸ boxed;
}

image resize_max(image im, ਪੂਰਨ max)
{
    ਪੂਰਨ w = im.w;
    ਪੂਰਨ h = im.h;
    ਜੇ(w > h){
        h = (h * max) / w;
        w = max;
    } ਨਹੀਂਤਾਂ {
        w = (w * max) / h;
        h = max;
    }
    ਜੇ(w == im.w && h == im.h) ਵਾਪਸ im;
    image resized = resize_image(im, w, h);
    ਵਾਪਸ resized;
}

image resize_min(image im, ਪੂਰਨ min)
{
    ਪੂਰਨ w = im.w;
    ਪੂਰਨ h = im.h;
    ਜੇ(w < h){
        h = (h * min) / w;
        w = min;
    } ਨਹੀਂਤਾਂ {
        w = (w * min) / h;
        h = min;
    }
    ਜੇ(w == im.w && h == im.h) ਵਾਪਸ im;
    image resized = resize_image(im, w, h);
    ਵਾਪਸ resized;
}

image random_crop_image(image im, ਪੂਰਨ w, ਪੂਰਨ h)
{
    ਪੂਰਨ dx = rand_int(0, im.w - w);
    ਪੂਰਨ dy = rand_int(0, im.h - h);
    image crop = crop_image(im, dx, dy, w, h);
    ਵਾਪਸ crop;
}

augment_ਦਲੀਲਾਂ random_augment_ਦਲੀਲਾਂ(image im, ਦਸ਼ਮਲਵ angle, ਦਸ਼ਮਲਵ aspect, ਪੂਰਨ ਨੀਵਾਂ, ਪੂਰਨ ਉੱਚਾ, ਪੂਰਨ w, ਪੂਰਨ h)
{
    augment_ਦਲੀਲਾਂ a = {0};
    aspect = rand_scale(aspect);
    ਪੂਰਨ r = rand_int(ਨੀਵਾਂ, ਉੱਚਾ);
    ਪੂਰਨ min = (im.h < im.w*aspect) ? im.h : im.w*aspect;
    ਦਸ਼ਮਲਵ scale = (ਦਸ਼ਮਲਵ)r / min;

    ਦਸ਼ਮਲਵ rad = rand_ਇਕਸਾਰ(-angle, angle) * TWO_PI / 360.;

    ਦਸ਼ਮਲਵ dx = (im.w*scale/aspect - w) / 2.;
    ਦਸ਼ਮਲਵ dy = (im.h*scale - w) / 2.;
    //if(dx < 0) dx = 0;
    //if(dy < 0) dy = 0;
    dx = rand_ਇਕਸਾਰ(-dx, dx);
    dy = rand_ਇਕਸਾਰ(-dy, dy);

    a.rad = rad;
    a.scale = scale;
    a.w = w;
    a.h = h;
    a.dx = dx;
    a.dy = dy;
    a.aspect = aspect;
    ਵਾਪਸ a;
}

image random_augment_image(image im, ਦਸ਼ਮਲਵ angle, ਦਸ਼ਮਲਵ aspect, ਪੂਰਨ ਨੀਵਾਂ, ਪੂਰਨ ਉੱਚਾ, ਪੂਰਨ w, ਪੂਰਨ h)
{
    augment_ਦਲੀਲਾਂ a = random_augment_ਦਲੀਲਾਂ(im, angle, aspect, ਨੀਵਾਂ, ਉੱਚਾ, w, h);
    image crop = rotate_crop_image(im, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);
    ਵਾਪਸ crop;
}

ਦਸ਼ਮਲਵ three_way_max(ਦਸ਼ਮਲਵ a, ਦਸ਼ਮਲਵ b, ਦਸ਼ਮਲਵ c)
{
    ਵਾਪਸ (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

ਦਸ਼ਮਲਵ three_way_min(ਦਸ਼ਮਲਵ a, ਦਸ਼ਮਲਵ b, ਦਸ਼ਮਲਵ c)
{
    ਵਾਪਸ (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

ਖਾਲੀ yuv_ਮੰਜ਼ਿਲ_rgb(image im)
{
    ਪ੍ਰਤਿਜ੍ਞਾ(im.c == 3);
    ਪੂਰਨ i, j;
    ਦਸ਼ਮਲਵ r, g, b;
    ਦਸ਼ਮਲਵ y, u, v;
    ਲਈ(j = 0; j < im.h; ++j){
        ਲਈ(i = 0; i < im.w; ++i){
            y = get_pixel(im, i , j, 0);
            u = get_pixel(im, i , j, 1);
            v = get_pixel(im, i , j, 2);

            r = y + 1.13983*v;
            g = y + -.39465*u + -.58060*v;
            b = y + 2.03211*u;

            ਸੈੱਟ_pixel(im, i, j, 0, r);
            ਸੈੱਟ_pixel(im, i, j, 1, g);
            ਸੈੱਟ_pixel(im, i, j, 2, b);
        }
    }
}

ਖਾਲੀ rgb_ਮੰਜ਼ਿਲ_yuv(image im)
{
    ਪ੍ਰਤਿਜ੍ਞਾ(im.c == 3);
    ਪੂਰਨ i, j;
    ਦਸ਼ਮਲਵ r, g, b;
    ਦਸ਼ਮਲਵ y, u, v;
    ਲਈ(j = 0; j < im.h; ++j){
        ਲਈ(i = 0; i < im.w; ++i){
            r = get_pixel(im, i , j, 0);
            g = get_pixel(im, i , j, 1);
            b = get_pixel(im, i , j, 2);

            y = .299*r + .587*g + .114*b;
            u = -.14713*r + -.28886*g + .436*b;
            v = .615*r + -.51499*g + -.10001*b;

            ਸੈੱਟ_pixel(im, i, j, 0, y);
            ਸੈੱਟ_pixel(im, i, j, 1, u);
            ਸੈੱਟ_pixel(im, i, j, 2, v);
        }
    }
}

// http://www.cs.rit.edu/~ncs/color/t_convert.html
ਖਾਲੀ rgb_ਮੰਜ਼ਿਲ_hsv(image im)
{
    ਪ੍ਰਤਿਜ੍ਞਾ(im.c == 3);
    ਪੂਰਨ i, j;
    ਦਸ਼ਮਲਵ r, g, b;
    ਦਸ਼ਮਲਵ h, s, v;
    ਲਈ(j = 0; j < im.h; ++j){
        ਲਈ(i = 0; i < im.w; ++i){
            r = get_pixel(im, i , j, 0);
            g = get_pixel(im, i , j, 1);
            b = get_pixel(im, i , j, 2);
            ਦਸ਼ਮਲਵ max = three_way_max(r,g,b);
            ਦਸ਼ਮਲਵ min = three_way_min(r,g,b);
            ਦਸ਼ਮਲਵ delta = max - min;
            v = max;
            ਜੇ(max == 0){
                s = 0;
                h = 0;
            }ਨਹੀਂਤਾਂ{
                s = delta/max;
                ਜੇ(r == max){
                    h = (g - b) / delta;
                } ਨਹੀਂਤਾਂ ਜੇ (g == max) {
                    h = 2 + (b - r) / delta;
                } ਨਹੀਂਤਾਂ {
                    h = 4 + (r - g) / delta;
                }
                ਜੇ (h < 0) h += 6;
                h = h/6.;
            }
            ਸੈੱਟ_pixel(im, i, j, 0, h);
            ਸੈੱਟ_pixel(im, i, j, 1, s);
            ਸੈੱਟ_pixel(im, i, j, 2, v);
        }
    }
}

ਖਾਲੀ hsv_ਮੰਜ਼ਿਲ_rgb(image im)
{
    ਪ੍ਰਤਿਜ੍ਞਾ(im.c == 3);
    ਪੂਰਨ i, j;
    ਦਸ਼ਮਲਵ r, g, b;
    ਦਸ਼ਮਲਵ h, s, v;
    ਦਸ਼ਮਲਵ f, p, q, t;
    ਲਈ(j = 0; j < im.h; ++j){
        ਲਈ(i = 0; i < im.w; ++i){
            h = 6 * get_pixel(im, i , j, 0);
            s = get_pixel(im, i , j, 1);
            v = get_pixel(im, i , j, 2);
            ਜੇ (s == 0) {
                r = g = b = v;
            } ਨਹੀਂਤਾਂ {
                ਪੂਰਨ ਸੂਚਕਾਂਕ = floor(h);
                f = h - ਸੂਚਕਾਂਕ;
                p = v*(1-s);
                q = v*(1-s*f);
                t = v*(1-s*(1-f));
                ਜੇ(ਸੂਚਕਾਂਕ == 0){
                    r = v; g = t; b = p;
                } ਨਹੀਂਤਾਂ ਜੇ(ਸੂਚਕਾਂਕ == 1){
                    r = q; g = v; b = p;
                } ਨਹੀਂਤਾਂ ਜੇ(ਸੂਚਕਾਂਕ == 2){
                    r = p; g = v; b = t;
                } ਨਹੀਂਤਾਂ ਜੇ(ਸੂਚਕਾਂਕ == 3){
                    r = p; g = q; b = v;
                } ਨਹੀਂਤਾਂ ਜੇ(ਸੂਚਕਾਂਕ == 4){
                    r = t; g = p; b = v;
                } ਨਹੀਂਤਾਂ {
                    r = v; g = p; b = q;
                }
            }
            ਸੈੱਟ_pixel(im, i, j, 0, r);
            ਸੈੱਟ_pixel(im, i, j, 1, g);
            ਸੈੱਟ_pixel(im, i, j, 2, b);
        }
    }
}

ਖਾਲੀ grayscale_image_3c(image im)
{
    ਪ੍ਰਤਿਜ੍ਞਾ(im.c == 3);
    ਪੂਰਨ i, j, k;
    ਦਸ਼ਮਲਵ scale[] = {0.299, 0.587, 0.114};
    ਲਈ(j = 0; j < im.h; ++j){
        ਲਈ(i = 0; i < im.w; ++i){
            ਦਸ਼ਮਲਵ val = 0;
            ਲਈ(k = 0; k < 3; ++k){
                val += scale[k]*get_pixel(im, i, j, k);
            }
            im.ਅੰਕੜੇ[0*im.h*im.w + im.w*j + i] = val;
            im.ਅੰਕੜੇ[1*im.h*im.w + im.w*j + i] = val;
            im.ਅੰਕੜੇ[2*im.h*im.w + im.w*j + i] = val;
        }
    }
}

image grayscale_image(image im)
{
    ਪ੍ਰਤਿਜ੍ਞਾ(im.c == 3);
    ਪੂਰਨ i, j, k;
    image ਸਲੇਟੀ = make_image(im.w, im.h, 1);
    ਦਸ਼ਮਲਵ scale[] = {0.299, 0.587, 0.114};
    ਲਈ(k = 0; k < im.c; ++k){
        ਲਈ(j = 0; j < im.h; ++j){
            ਲਈ(i = 0; i < im.w; ++i){
                ਸਲੇਟੀ.ਅੰਕੜੇ[i+im.w*j] += scale[k]*get_pixel(im, i, j, k);
            }
        }
    }
    ਵਾਪਸ ਸਲੇਟੀ;
}

image ਹੱਦ_image(image im, ਦਸ਼ਮਲਵ thresh)
{
    ਪੂਰਨ i;
    image t = make_image(im.w, im.h, im.c);
    ਲਈ(i = 0; i < im.w*im.h*im.c; ++i){
        t.ਅੰਕੜੇ[i] = im.ਅੰਕੜੇ[i]>thresh ? 1 : 0;
    }
    ਵਾਪਸ t;
}

image blend_image(image fore, image ਪਿੱਠ, ਦਸ਼ਮਲਵ alpha)
{
    ਪ੍ਰਤਿਜ੍ਞਾ(fore.w == ਪਿੱਠ.w && fore.h == ਪਿੱਠ.h && fore.c == ਪਿੱਠ.c);
    image blend = make_image(fore.w, fore.h, fore.c);
    ਪੂਰਨ i, j, k;
    ਲਈ(k = 0; k < fore.c; ++k){
        ਲਈ(j = 0; j < fore.h; ++j){
            ਲਈ(i = 0; i < fore.w; ++i){
                ਦਸ਼ਮਲਵ val = alpha * get_pixel(fore, i, j, k) + 
                    (1 - alpha)* get_pixel(ਪਿੱਠ, i, j, k);
                ਸੈੱਟ_pixel(blend, i, j, k, val);
            }
        }
    }
    ਵਾਪਸ blend;
}

ਖਾਲੀ scale_image_channel(image im, ਪੂਰਨ c, ਦਸ਼ਮਲਵ v)
{
    ਪੂਰਨ i, j;
    ਲਈ(j = 0; j < im.h; ++j){
        ਲਈ(i = 0; i < im.w; ++i){
            ਦਸ਼ਮਲਵ pix = get_pixel(im, i, j, c);
            pix = pix*v;
            ਸੈੱਟ_pixel(im, i, j, c, pix);
        }
    }
}

ਖਾਲੀ translate_image_channel(image im, ਪੂਰਨ c, ਦਸ਼ਮਲਵ v)
{
    ਪੂਰਨ i, j;
    ਲਈ(j = 0; j < im.h; ++j){
        ਲਈ(i = 0; i < im.w; ++i){
            ਦਸ਼ਮਲਵ pix = get_pixel(im, i, j, c);
            pix = pix+v;
            ਸੈੱਟ_pixel(im, i, j, c, pix);
        }
    }
}

image binarize_image(image im)
{
    image c = copy_image(im);
    ਪੂਰਨ i;
    ਲਈ(i = 0; i < im.w * im.h * im.c; ++i){
        ਜੇ(c.ਅੰਕੜੇ[i] > .5) c.ਅੰਕੜੇ[i] = 1;
        ਨਹੀਂਤਾਂ c.ਅੰਕੜੇ[i] = 0;
    }
    ਵਾਪਸ c;
}

ਖਾਲੀ saturate_image(image im, ਦਸ਼ਮਲਵ sat)
{
    rgb_ਮੰਜ਼ਿਲ_hsv(im);
    scale_image_channel(im, 1, sat);
    hsv_ਮੰਜ਼ਿਲ_rgb(im);
    constrain_image(im);
}

ਖਾਲੀ hue_image(image im, ਦਸ਼ਮਲਵ hue)
{
    rgb_ਮੰਜ਼ਿਲ_hsv(im);
    ਪੂਰਨ i;
    ਲਈ(i = 0; i < im.w*im.h; ++i){
        im.ਅੰਕੜੇ[i] = im.ਅੰਕੜੇ[i] + hue;
        ਜੇ (im.ਅੰਕੜੇ[i] > 1) im.ਅੰਕੜੇ[i] -= 1;
        ਜੇ (im.ਅੰਕੜੇ[i] < 0) im.ਅੰਕੜੇ[i] += 1;
    }
    hsv_ਮੰਜ਼ਿਲ_rgb(im);
    constrain_image(im);
}

ਖਾਲੀ exposure_image(image im, ਦਸ਼ਮਲਵ sat)
{
    rgb_ਮੰਜ਼ਿਲ_hsv(im);
    scale_image_channel(im, 2, sat);
    hsv_ਮੰਜ਼ਿਲ_rgb(im);
    constrain_image(im);
}

ਖਾਲੀ distort_image(image im, ਦਸ਼ਮਲਵ hue, ਦਸ਼ਮਲਵ sat, ਦਸ਼ਮਲਵ val)
{
    rgb_ਮੰਜ਼ਿਲ_hsv(im);
    scale_image_channel(im, 1, sat);
    scale_image_channel(im, 2, val);
    ਪੂਰਨ i;
    ਲਈ(i = 0; i < im.w*im.h; ++i){
        im.ਅੰਕੜੇ[i] = im.ਅੰਕੜੇ[i] + hue;
        ਜੇ (im.ਅੰਕੜੇ[i] > 1) im.ਅੰਕੜੇ[i] -= 1;
        ਜੇ (im.ਅੰਕੜੇ[i] < 0) im.ਅੰਕੜੇ[i] += 1;
    }
    hsv_ਮੰਜ਼ਿਲ_rgb(im);
    constrain_image(im);
}

ਖਾਲੀ random_distort_image(image im, ਦਸ਼ਮਲਵ hue, ਦਸ਼ਮਲਵ saturation, ਦਸ਼ਮਲਵ exposure)
{
    ਦਸ਼ਮਲਵ dhue = rand_ਇਕਸਾਰ(-hue, hue);
    ਦਸ਼ਮਲਵ dsat = rand_scale(saturation);
    ਦਸ਼ਮਲਵ dexp = rand_scale(exposure);
    distort_image(im, dhue, dsat, dexp);
}

ਖਾਲੀ saturate_exposure_image(image im, ਦਸ਼ਮਲਵ sat, ਦਸ਼ਮਲਵ exposure)
{
    rgb_ਮੰਜ਼ਿਲ_hsv(im);
    scale_image_channel(im, 1, sat);
    scale_image_channel(im, 2, exposure);
    hsv_ਮੰਜ਼ਿਲ_rgb(im);
    constrain_image(im);
}

image resize_image(image im, ਪੂਰਨ w, ਪੂਰਨ h)
{
    image resized = make_image(w, h, im.c);   
    image part = make_image(w, im.h, im.c);
    ਪੂਰਨ r, c, k;
    ਦਸ਼ਮਲਵ w_scale = (ਦਸ਼ਮਲਵ)(im.w - 1) / (w - 1);
    ਦਸ਼ਮਲਵ h_scale = (ਦਸ਼ਮਲਵ)(im.h - 1) / (h - 1);
    ਲਈ(k = 0; k < im.c; ++k){
        ਲਈ(r = 0; r < im.h; ++r){
            ਲਈ(c = 0; c < w; ++c){
                ਦਸ਼ਮਲਵ val = 0;
                ਜੇ(c == w-1 || im.w == 1){
                    val = get_pixel(im, im.w-1, r, k);
                } ਨਹੀਂਤਾਂ {
                    ਦਸ਼ਮਲਵ sx = c*w_scale;
                    ਪੂਰਨ ix = (ਪੂਰਨ) sx;
                    ਦਸ਼ਮਲਵ dx = sx - ix;
                    val = (1 - dx) * get_pixel(im, ix, r, k) + dx * get_pixel(im, ix+1, r, k);
                }
                ਸੈੱਟ_pixel(part, c, r, k, val);
            }
        }
    }
    ਲਈ(k = 0; k < im.c; ++k){
        ਲਈ(r = 0; r < h; ++r){
            ਦਸ਼ਮਲਵ sy = r*h_scale;
            ਪੂਰਨ iy = (ਪੂਰਨ) sy;
            ਦਸ਼ਮਲਵ dy = sy - iy;
            ਲਈ(c = 0; c < w; ++c){
                ਦਸ਼ਮਲਵ val = (1-dy) * get_pixel(part, c, iy, k);
                ਸੈੱਟ_pixel(resized, c, r, k, val);
            }
            ਜੇ(r == h-1 || im.h == 1) ਜਾਰੀ;
            ਲਈ(c = 0; c < w; ++c){
                ਦਸ਼ਮਲਵ val = dy * get_pixel(part, c, iy+1, k);
                ਜੋੜੋ_pixel(resized, c, r, k, val);
            }
        }
    }

    free_image(part);
    ਵਾਪਸ resized;
}


ਖਾਲੀ test_resize(ਅੱਖਰ *filename)
{
    image im = ਲੱਦੋ_image(filename, 0,0, 3);
    ਦਸ਼ਮਲਵ mag = mag_array(im.ਅੰਕੜੇ, im.w*im.h*im.c);
    ਲਿਖੋ("L2 Norm: %f\n", mag);
    image ਸਲੇਟੀ = grayscale_image(im);

    image c1 = copy_image(im);
    image c2 = copy_image(im);
    image c3 = copy_image(im);
    image c4 = copy_image(im);
    distort_image(c1, .1, 1.5, 1.5);
    distort_image(c2, -.1, .66666, .66666);
    distort_image(c3, .1, 1.5, .66666);
    distort_image(c4, .1, .66666, 1.5);


    show_image(im,   "Original", 1);
    show_image(ਸਲੇਟੀ, "Gray", 1);
    show_image(c1, "C1", 1);
    show_image(c2, "C2", 1);
    show_image(c3, "C3", 1);
    show_image(c4, "C4", 1);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ OPENCV
    ਜਦੋਂ(1){
        image aug = random_augment_image(im, 0, .75, 320, 448, 320, 320);
        show_image(aug, "aug", 1);
        free_image(aug);


        ਦਸ਼ਮਲਵ exposure = 1.15;
        ਦਸ਼ਮਲਵ saturation = 1.15;
        ਦਸ਼ਮਲਵ hue = .05;

        image c = copy_image(im);

        ਦਸ਼ਮਲਵ dexp = rand_scale(exposure);
        ਦਸ਼ਮਲਵ dsat = rand_scale(saturation);
        ਦਸ਼ਮਲਵ dhue = rand_ਇਕਸਾਰ(-hue, hue);

        distort_image(c, dhue, dsat, dexp);
        show_image(c, "rand", 1);
        ਲਿਖੋ("%f %f %f\n", dhue, dsat, dexp);
        free_image(c);
    }
#ਜੇ_ਅੰਤ
}


image ਲੱਦੋ_image_stb(ਅੱਖਰ *filename, ਪੂਰਨ channels)
{
    ਪੂਰਨ w, h, c;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *ਅੰਕੜੇ = stbi_ਲੱਦੋ(filename, &w, &h, &c, channels);
    ਜੇ (!ਅੰਕੜੇ) {
        fprintf(stderr, "Cannot load image \"%s\"\nSTB Reason: %s\n", filename, stbi_failure_reason());
        ਬਾਹਰ_ਨਿਕਲੋ(0);
    }
    ਜੇ(channels) c = channels;
    ਪੂਰਨ i,j,k;
    image im = make_image(w, h, c);
    ਲਈ(k = 0; k < c; ++k){
        ਲਈ(j = 0; j < h; ++j){
            ਲਈ(i = 0; i < w; ++i){
                ਪੂਰਨ dst_ਸੂਚਕਾਂਕ = i + w*j + w*h*k;
                ਪੂਰਨ src_ਸੂਚਕਾਂਕ = k + c*i + c*w*j;
                im.ਅੰਕੜੇ[dst_ਸੂਚਕਾਂਕ] = (ਦਸ਼ਮਲਵ)ਅੰਕੜੇ[src_ਸੂਚਕਾਂਕ]/255.;
            }
        }
    }
    ਮੁਕਤ(ਅੰਕੜੇ);
    ਵਾਪਸ im;
}

image ਲੱਦੋ_image(ਅੱਖਰ *filename, ਪੂਰਨ w, ਪੂਰਨ h, ਪੂਰਨ c)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ OPENCV
    image ਬਾਹਰਲਾ = ਲੱਦੋ_image_cv(filename, c);
#ਨਹੀਂਤਾਂ
    image ਬਾਹਰਲਾ = ਲੱਦੋ_image_stb(filename, c);
#ਜੇ_ਅੰਤ

    ਜੇ((h && w) && (h != ਬਾਹਰਲਾ.h || w != ਬਾਹਰਲਾ.w)){
        image resized = resize_image(ਬਾਹਰਲਾ, w, h);
        free_image(ਬਾਹਰਲਾ);
        ਬਾਹਰਲਾ = resized;
    }
    ਵਾਪਸ ਬਾਹਰਲਾ;
}

image ਲੱਦੋ_image_color(ਅੱਖਰ *filename, ਪੂਰਨ w, ਪੂਰਨ h)
{
    ਵਾਪਸ ਲੱਦੋ_image(filename, w, h, 3);
}

image get_image_layer(image m, ਪੂਰਨ l)
{
    image ਬਾਹਰਲਾ = make_image(m.w, m.h, 1);
    ਪੂਰਨ i;
    ਲਈ(i = 0; i < m.h*m.w; ++i){
        ਬਾਹਰਲਾ.ਅੰਕੜੇ[i] = m.ਅੰਕੜੇ[i+l*m.h*m.w];
    }
    ਵਾਪਸ ਬਾਹਰਲਾ;
}
ਖਾਲੀ print_image(image m)
{
    ਪੂਰਨ i, j, k;
    ਲਈ(i =0 ; i < m.c; ++i){
        ਲਈ(j =0 ; j < m.h; ++j){
            ਲਈ(k = 0; k < m.w; ++k){
                ਲਿਖੋ("%.2lf, ", m.ਅੰਕੜੇ[i*m.h*m.w + j*m.w + k]);
                ਜੇ(k > 30) ਰੋਕੋ;
            }
            ਲਿਖੋ("\n");
            ਜੇ(j > 30) ਰੋਕੋ;
        }
        ਲਿਖੋ("\n");
    }
    ਲਿਖੋ("\n");
}

image collapse_images_vert(image *ims, ਪੂਰਨ n)
{
    ਪੂਰਨ color = 1;
    ਪੂਰਨ border = 1;
    ਪੂਰਨ h,w,c;
    w = ims[0].w;
    h = (ims[0].h + border) * n - border;
    c = ims[0].c;
    ਜੇ(c != 3 || !color){
        w = (w+border)*c - border;
        c = 1;
    }

    image filters = make_image(w, h, c);
    ਪੂਰਨ i,j;
    ਲਈ(i = 0; i < n; ++i){
        ਪੂਰਨ h_ਵਿੱਥ = i*(ims[0].h+border);
        image copy = copy_image(ims[i]);
        //normalize_image(copy);
        ਜੇ(c == 3 && color){
            embed_image(copy, filters, 0, h_ਵਿੱਥ);
        }
        ਨਹੀਂਤਾਂ{
            ਲਈ(j = 0; j < copy.c; ++j){
                ਪੂਰਨ w_ਵਿੱਥ = j*(ims[0].w+border);
                image layer = get_image_layer(copy, j);
                embed_image(layer, filters, w_ਵਿੱਥ, h_ਵਿੱਥ);
                free_image(layer);
            }
        }
        free_image(copy);
    }
    ਵਾਪਸ filters;
} 

image collapse_images_horz(image *ims, ਪੂਰਨ n)
{
    ਪੂਰਨ color = 1;
    ਪੂਰਨ border = 1;
    ਪੂਰਨ h,w,c;
    ਪੂਰਨ ਮਾਪ = ims[0].h;
    h = ਮਾਪ;
    w = (ims[0].w + border) * n - border;
    c = ims[0].c;
    ਜੇ(c != 3 || !color){
        h = (h+border)*c - border;
        c = 1;
    }

    image filters = make_image(w, h, c);
    ਪੂਰਨ i,j;
    ਲਈ(i = 0; i < n; ++i){
        ਪੂਰਨ w_ਵਿੱਥ = i*(ਮਾਪ+border);
        image copy = copy_image(ims[i]);
        //normalize_image(copy);
        ਜੇ(c == 3 && color){
            embed_image(copy, filters, w_ਵਿੱਥ, 0);
        }
        ਨਹੀਂਤਾਂ{
            ਲਈ(j = 0; j < copy.c; ++j){
                ਪੂਰਨ h_ਵਿੱਥ = j*(ਮਾਪ+border);
                image layer = get_image_layer(copy, j);
                embed_image(layer, filters, w_ਵਿੱਥ, h_ਵਿੱਥ);
                free_image(layer);
            }
        }
        free_image(copy);
    }
    ਵਾਪਸ filters;
} 

ਖਾਲੀ show_image_normalized(image im, ਅਚਲ ਅੱਖਰ *name)
{
    image c = copy_image(im);
    normalize_image(c);
    show_image(c, name, 1);
    free_image(c);
}

ਖਾਲੀ show_images(image *ims, ਪੂਰਨ n, ਅੱਖਰ *ਖਿੜਕੀ)
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
    save_image(m, ਖਿੜਕੀ);
    show_image(m, ਖਿੜਕੀ, 1);
    free_image(m);
}

ਖਾਲੀ free_image(image m)
{
    ਜੇ(m.ਅੰਕੜੇ){
        ਮੁਕਤ(m.ਅੰਕੜੇ);
    }
}
