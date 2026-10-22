#شامل "image.h"
#شامل "utils.h"
#شامل "blas.h"
#شامل "cuda.h"
#شامل <stdio.h>
#شامل <math.h>

#تعریف_کرو STB_IMAGE_IMPLEMENTATION
#شامل "stb_image.h"
#تعریف_کرو STB_IMAGE_WRITE_IMPLEMENTATION
#شامل "stb_image_write.h"

صحیح windows = 0;

اعشاری colors[6][3] = { {1,0,1}, {0,0,1},{0,1,1},{0,1,0},{1,1,0},{1,0,0} };

اعشاری get_color(صحیح c, صحیح x, صحیح max)
{
    اعشاری تناسب = ((اعشاری)x/max)*5;
    صحیح i = floor(تناسب);
    صحیح j = ceil(تناسب);
    تناسب -= i;
    اعشاری r = (1-تناسب) * colors[i][c] + تناسب*colors[j][c];
    //printf("%f\n", r);
    واپس r;
}

image نقاب_مقصد_rgb(image نقاب)
{
    صحیح n = نقاب.c;
    image im = make_image(نقاب.w, نقاب.h, 3);
    صحیح i, j;
    برائے(j = 0; j < n; ++j){
        صحیح انحراف = j*123457 % n;
        اعشاری red = get_color(2,انحراف,n);
        اعشاری green = get_color(1,انحراف,n);
        اعشاری blue = get_color(0,انحراف,n);
        برائے(i = 0; i < im.w*im.h; ++i){
            im.ڈیٹا[i + 0*im.w*im.h] += نقاب.ڈیٹا[j*im.h*im.w + i]*red;
            im.ڈیٹا[i + 1*im.w*im.h] += نقاب.ڈیٹا[j*im.h*im.w + i]*green;
            im.ڈیٹا[i + 2*im.w*im.h] += نقاب.ڈیٹا[j*im.h*im.w + i]*blue;
        }
    }
    واپس im;
}

ساکن اعشاری get_pixel(image m, صحیح x, صحیح y, صحیح c)
{
    أكد(x < m.w && y < m.h && c < m.c);
    واپس m.ڈیٹا[c*m.h*m.w + y*m.w + x];
}
ساکن اعشاری get_pixel_extend(image m, صحیح x, صحیح y, صحیح c)
{
    اگر(x < 0 || x >= m.w || y < 0 || y >= m.h) واپس 0;
    /*
    if(x < 0) x = 0;
    if(x >= m.w) x = m.w-1;
    if(y < 0) y = 0;
    if(y >= m.h) y = m.h-1;
    */
    اگر(c < 0 || c >= m.c) واپس 0;
    واپس get_pixel(m, x, y, c);
}
ساکن خالی سیٹ_pixel(image m, صحیح x, صحیح y, صحیح c, اعشاری val)
{
    اگر (x < 0 || y < 0 || c < 0 || x >= m.w || y >= m.h || c >= m.c) واپس;
    أكد(x < m.w && y < m.h && c < m.c);
    m.ڈیٹا[c*m.h*m.w + y*m.w + x] = val;
}
ساکن خالی جوڑو_pixel(image m, صحیح x, صحیح y, صحیح c, اعشاری val)
{
    أكد(x < m.w && y < m.h && c < m.c);
    m.ڈیٹا[c*m.h*m.w + y*m.w + x] += val;
}

ساکن اعشاری bilinear_interpolate(image im, اعشاری x, اعشاری y, صحیح c)
{
    صحیح ix = (صحیح) floorf(x);
    صحیح iy = (صحیح) floorf(y);

    اعشاری dx = x - ix;
    اعشاری dy = y - iy;

    اعشاری val = (1-dy) * (1-dx) * get_pixel_extend(im, ix, iy, c) + 
        dy     * (1-dx) * get_pixel_extend(im, ix, iy+1, c) + 
        (1-dy) *   dx   * get_pixel_extend(im, ix+1, iy, c) +
        dy     *   dx   * get_pixel_extend(im, ix+1, iy+1, c);
    واپس val;
}


خالی مرکب_image(image ماخذ, image dest, صحیح dx, صحیح dy)
{
    صحیح x,y,k;
    برائے(k = 0; k < ماخذ.c; ++k){
        برائے(y = 0; y < ماخذ.h; ++y){
            برائے(x = 0; x < ماخذ.w; ++x){
                اعشاری val = get_pixel(ماخذ, x, y, k);
                اعشاری val2 = get_pixel_extend(dest, dx+x, dy+y, k);
                سیٹ_pixel(dest, dx+x, dy+y, k, val * val2);
            }
        }
    }
}

image border_image(image a, صحیح border)
{
    image b = make_image(a.w + 2*border, a.h + 2*border, a.c);
    صحیح x,y,k;
    برائے(k = 0; k < b.c; ++k){
        برائے(y = 0; y < b.h; ++y){
            برائے(x = 0; x < b.w; ++x){
                اعشاری val = get_pixel_extend(a, x - border, y - border, k);
                اگر(x - border < 0 || x - border >= a.w || y - border < 0 || y - border >= a.h) val = 1;
                سیٹ_pixel(b, x, y, k, val);
            }
        }
    }
    واپس b;
}

image tile_images(image a, image b, صحیح dx)
{
    اگر(a.w == 0) واپس copy_image(b);
    image c = make_image(a.w + b.w + dx, (a.h > b.h) ? a.h : b.h, (a.c > b.c) ? a.c : b.c);
    fill_cpu(c.w*c.h*c.c, 1, c.ڈیٹا, 1);
    embed_image(a, c, 0, 0); 
    مرکب_image(b, c, a.w + dx, 0);
    واپس c;
}

image get_label(image **characters, حرف *string, صحیح سائز)
{
    سائز = سائز/10;
    اگر(سائز > 7) سائز = 7;
    image label = make_خالی_خانے_image(0,0,0);
    جب(*string){
        image l = characters[سائز][(صحیح)*string];
        image n = tile_images(label, l, -سائز - 1 + (سائز+1)/2);
        free_image(label);
        label = n;
        ++string;
    }
    image b = border_image(label, label.h*.25);
    free_image(label);
    واپس b;
}

خالی draw_label(image a, صحیح r, صحیح c, image label, ثابت اعشاری *rgb)
{
    صحیح w = label.w;
    صحیح h = label.h;
    اگر (r - h >= 0) r = r - h;

    صحیح i, j, k;
    برائے(j = 0; j < h && j + r < a.h; ++j){
        برائے(i = 0; i < w && i + c < a.w; ++i){
            برائے(k = 0; k < label.c; ++k){
                اعشاری val = get_pixel(label, i, j, k);
                سیٹ_pixel(a, i+c, j+r, k, rgb[k] * val);
            }
        }
    }
}

خالی draw_box(image a, صحیح x1, صحیح y1, صحیح x2, صحیح y2, اعشاری r, اعشاری g, اعشاری b)
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

    برائے(i = x1; i <= x2; ++i){
        a.ڈیٹا[i + y1*a.w + 0*a.w*a.h] = r;
        a.ڈیٹا[i + y2*a.w + 0*a.w*a.h] = r;

        a.ڈیٹا[i + y1*a.w + 1*a.w*a.h] = g;
        a.ڈیٹا[i + y2*a.w + 1*a.w*a.h] = g;

        a.ڈیٹا[i + y1*a.w + 2*a.w*a.h] = b;
        a.ڈیٹا[i + y2*a.w + 2*a.w*a.h] = b;
    }
    برائے(i = y1; i <= y2; ++i){
        a.ڈیٹا[x1 + i*a.w + 0*a.w*a.h] = r;
        a.ڈیٹا[x2 + i*a.w + 0*a.w*a.h] = r;

        a.ڈیٹا[x1 + i*a.w + 1*a.w*a.h] = g;
        a.ڈیٹا[x2 + i*a.w + 1*a.w*a.h] = g;

        a.ڈیٹا[x1 + i*a.w + 2*a.w*a.h] = b;
        a.ڈیٹا[x2 + i*a.w + 2*a.w*a.h] = b;
    }
}

خالی draw_box_چوڑائی(image a, صحیح x1, صحیح y1, صحیح x2, صحیح y2, صحیح w, اعشاری r, اعشاری g, اعشاری b)
{
    صحیح i;
    برائے(i = 0; i < w; ++i){
        draw_box(a, x1+i, y1+i, x2-i, y2-i, r, g, b);
    }
}

خالی draw_bbox(image a, box bbox, صحیح w, اعشاری r, اعشاری g, اعشاری b)
{
    صحیح بایاں  = (bbox.x-bbox.w/2)*a.w;
    صحیح دایاں = (bbox.x+bbox.w/2)*a.w;
    صحیح چوٹی   = (bbox.y-bbox.h/2)*a.h;
    صحیح bot   = (bbox.y+bbox.h/2)*a.h;

    صحیح i;
    برائے(i = 0; i < w; ++i){
        draw_box(a, بایاں+i, چوٹی+i, دایاں-i, bot-i, r, g, b);
    }
}

image **بھرو_alphabet()
{
    صحیح i, j;
    ثابت صحیح nsize = 8;
    image **alphabets = صفر_حافظہ_دو(nsize, حجم(image));
    برائے(j = 0; j < nsize; ++j){
        alphabets[j] = صفر_حافظہ_دو(128, حجم(image));
        برائے(i = 32; i < 127; ++i){
            حرف buff[256];
            sprintf(buff, "data/labels/%d_%d.png", i, j);
            alphabets[j][i] = بھرو_image_color(buff, 0, 0);
        }
    }
    واپس alphabets;
}

خالی draw_detections(image im, detection *dets, صحیح عدد, اعشاری thresh, حرف **نام, image **alphabet, صحیح classes)
{
    صحیح i,j;

    برائے(i = 0; i < عدد; ++i){
        حرف labelstr[4096] = {0};
        صحیح class = -1;
        برائے(j = 0; j < classes; ++j){
            اگر (dets[i].prob[j] > thresh){
                اگر (class < 0) {
                    strcat(labelstr, نام[j]);
                    class = j;
                } ورنہ {
                    strcat(labelstr, ", ");
                    strcat(labelstr, نام[j]);
                }
                لکھو("%s: %.0f%%\n", نام[j], dets[i].prob[j]*100);
            }
        }
        اگر(class >= 0){
            صحیح چوڑائی = im.h * .006;

            /*
               if(0){
               width = pow(prob, 1./2.)*10+1;
               alphabet = 0;
               }
             */

            //printf("%d %s: %.0f%%\n", i, names[class], prob*100);
            صحیح انحراف = class*123457 % classes;
            اعشاری red = get_color(2,انحراف,classes);
            اعشاری green = get_color(1,انحراف,classes);
            اعشاری blue = get_color(0,انحراف,classes);
            اعشاری rgb[3];

            //width = prob*20+2;

            rgb[0] = red;
            rgb[1] = green;
            rgb[2] = blue;
            box b = dets[i].bbox;
            //printf("%f %f %f %f\n", b.x, b.y, b.w, b.h);

            صحیح بایاں  = (b.x-b.w/2.)*im.w;
            صحیح دایاں = (b.x+b.w/2.)*im.w;
            صحیح چوٹی   = (b.y-b.h/2.)*im.h;
            صحیح bot   = (b.y+b.h/2.)*im.h;

            اگر(بایاں < 0) بایاں = 0;
            اگر(دایاں > im.w-1) دایاں = im.w-1;
            اگر(چوٹی < 0) چوٹی = 0;
            اگر(bot > im.h-1) bot = im.h-1;

            draw_box_چوڑائی(im, بایاں, چوٹی, دایاں, bot, چوڑائی, red, green, blue);
            اگر (alphabet) {
                image label = get_label(alphabet, labelstr, (im.h*.03));
                draw_label(im, چوٹی + چوڑائی, بایاں, label, rgb);
                free_image(label);
            }
            اگر (dets[i].نقاب){
                image نقاب = float_مقصد_image(14, 14, 1, dets[i].نقاب);
                image resized_نقاب = resize_image(نقاب, b.w*im.w, b.h*im.h);
                image tmask = حد_image(resized_نقاب, .5);
                embed_image(tmask, im, بایاں, چوٹی);
                free_image(نقاب);
                free_image(resized_نقاب);
                free_image(tmask);
            }
        }
    }
}

خالی transpose_image(image im)
{
    أكد(im.w == im.h);
    صحیح n, m;
    صحیح c;
    برائے(c = 0; c < im.c; ++c){
        برائے(n = 0; n < im.w-1; ++n){
            برائے(m = n + 1; m < im.w; ++m){
                اعشاری تبادلہ = im.ڈیٹا[m + im.w*(n + im.h*c)];
                im.ڈیٹا[m + im.w*(n + im.h*c)] = im.ڈیٹا[n + im.w*(m + im.h*c)];
                im.ڈیٹا[n + im.w*(m + im.h*c)] = تبادلہ;
            }
        }
    }
}

خالی rotate_image_cw(image im, صحیح times)
{
    أكد(im.w == im.h);
    times = (times + 400) % 4;
    صحیح i, x, y, c;
    صحیح n = im.w;
    برائے(i = 0; i < times; ++i){
        برائے(c = 0; c < im.c; ++c){
            برائے(x = 0; x < n/2; ++x){
                برائے(y = 0; y < (n-1)/2 + 1; ++y){
                    اعشاری عارضی = im.ڈیٹا[y + im.w*(x + im.h*c)];
                    im.ڈیٹا[y + im.w*(x + im.h*c)] = im.ڈیٹا[n-1-x + im.w*(y + im.h*c)];
                    im.ڈیٹا[n-1-x + im.w*(y + im.h*c)] = im.ڈیٹا[n-1-y + im.w*(n-1-x + im.h*c)];
                    im.ڈیٹا[n-1-y + im.w*(n-1-x + im.h*c)] = im.ڈیٹا[x + im.w*(n-1-y + im.h*c)];
                    im.ڈیٹا[x + im.w*(n-1-y + im.h*c)] = عارضی;
                }
            }
        }
    }
}

خالی flip_image(image a)
{
    صحیح i,j,k;
    برائے(k = 0; k < a.c; ++k){
        برائے(i = 0; i < a.h; ++i){
            برائے(j = 0; j < a.w/2; ++j){
                صحیح اشاریہ = j + a.w*(i + a.h*(k));
                صحیح flip = (a.w - j - 1) + a.w*(i + a.h*(k));
                اعشاری تبادلہ = a.ڈیٹا[flip];
                a.ڈیٹا[flip] = a.ڈیٹا[اشاریہ];
                a.ڈیٹا[اشاریہ] = تبادلہ;
            }
        }
    }
}

image image_فاصلہ(image a, image b)
{
    صحیح i,j;
    image dist = make_image(a.w, a.h, 1);
    برائے(i = 0; i < a.c; ++i){
        برائے(j = 0; j < a.h*a.w; ++j){
            dist.ڈیٹا[j] += قوت(a.ڈیٹا[i*a.h*a.w+j]-b.ڈیٹا[i*a.h*a.w+j],2);
        }
    }
    برائے(j = 0; j < a.h*a.w; ++j){
        dist.ڈیٹا[j] = جذر(dist.ڈیٹا[j]);
    }
    واپس dist;
}

خالی ghost_image(image ماخذ, image dest, صحیح dx, صحیح dy)
{
    صحیح x,y,k;
    اعشاری max_dist = جذر((-ماخذ.w/2. + .5)*(-ماخذ.w/2. + .5));
    برائے(k = 0; k < ماخذ.c; ++k){
        برائے(y = 0; y < ماخذ.h; ++y){
            برائے(x = 0; x < ماخذ.w; ++x){
                اعشاری dist = جذر((x - ماخذ.w/2. + .5)*(x - ماخذ.w/2. + .5) + (y - ماخذ.h/2. + .5)*(y - ماخذ.h/2. + .5));
                اعشاری alpha = (1 - dist/max_dist);
                اگر(alpha < 0) alpha = 0;
                اعشاری v1 = get_pixel(ماخذ, x,y,k);
                اعشاری v2 = get_pixel(dest, dx+x,dy+y,k);
                اعشاری val = alpha*v1 + (1-alpha)*v2;
                سیٹ_pixel(dest, dx+x, dy+y, k, val);
            }
        }
    }
}

خالی blocky_image(image im, صحیح s)
{
    صحیح i,j,k;
    برائے(k = 0; k < im.c; ++k){
        برائے(j = 0; j < im.h; ++j){
            برائے(i = 0; i < im.w; ++i){
                im.ڈیٹا[i + im.w*(j + im.h*k)] = im.ڈیٹا[i/s*s + im.w*(j/s*s + im.h*k)];
            }
        }
    }
}

خالی censor_image(image im, صحیح dx, صحیح dy, صحیح w, صحیح h)
{
    صحیح i,j,k;
    صحیح s = 32;
    اگر(dx < 0) dx = 0;
    اگر(dy < 0) dy = 0;

    برائے(k = 0; k < im.c; ++k){
        برائے(j = dy; j < dy + h && j < im.h; ++j){
            برائے(i = dx; i < dx + w && i < im.w; ++i){
                im.ڈیٹا[i + im.w*(j + im.h*k)] = im.ڈیٹا[i/s*s + im.w*(j/s*s + im.h*k)];
                //im.data[i + j*im.w + k*im.w*im.h] = 0;
            }
        }
    }
}

خالی embed_image(image ماخذ, image dest, صحیح dx, صحیح dy)
{
    صحیح x,y,k;
    برائے(k = 0; k < ماخذ.c; ++k){
        برائے(y = 0; y < ماخذ.h; ++y){
            برائے(x = 0; x < ماخذ.w; ++x){
                اعشاری val = get_pixel(ماخذ, x,y,k);
                سیٹ_pixel(dest, dx+x, dy+y, k, val);
            }
        }
    }
}

image collapse_image_layers(image ماخذ, صحیح border)
{
    صحیح h = ماخذ.h;
    h = (h+border)*ماخذ.c - border;
    image dest = make_image(ماخذ.w, h, 1);
    صحیح i;
    برائے(i = 0; i < ماخذ.c; ++i){
        image layer = get_image_layer(ماخذ, i);
        صحیح h_انحراف = i*(ماخذ.h+border);
        embed_image(layer, dest, 0, h_انحراف);
        free_image(layer);
    }
    واپس dest;
}

خالی constrain_image(image im)
{
    صحیح i;
    برائے(i = 0; i < im.w*im.h*im.c; ++i){
        اگر(im.ڈیٹا[i] < 0) im.ڈیٹا[i] = 0;
        اگر(im.ڈیٹا[i] > 1) im.ڈیٹا[i] = 1;
    }
}

خالی normalize_image(image p)
{
    صحیح i;
    اعشاری min = 9999999;
    اعشاری max = -999999;

    برائے(i = 0; i < p.h*p.w*p.c; ++i){
        اعشاری v = p.ڈیٹا[i];
        اگر(v < min) min = v;
        اگر(v > max) max = v;
    }
    اگر(max - min < .000000001){
        min = 0;
        max = 1;
    }
    برائے(i = 0; i < p.c*p.w*p.h; ++i){
        p.ڈیٹا[i] = (p.ڈیٹا[i] - min)/(max-min);
    }
}

خالی normalize_image2(image p)
{
    اعشاری *min = صفر_حافظہ_دو(p.c, حجم(اعشاری));
    اعشاری *max = صفر_حافظہ_دو(p.c, حجم(اعشاری));
    صحیح i,j;
    برائے(i = 0; i < p.c; ++i) min[i] = max[i] = p.ڈیٹا[i*p.h*p.w];

    برائے(j = 0; j < p.c; ++j){
        برائے(i = 0; i < p.h*p.w; ++i){
            اعشاری v = p.ڈیٹا[i+j*p.h*p.w];
            اگر(v < min[j]) min[j] = v;
            اگر(v > max[j]) max[j] = v;
        }
    }
    برائے(i = 0; i < p.c; ++i){
        اگر(max[i] - min[i] < .000000001){
            min[i] = 0;
            max[i] = 1;
        }
    }
    برائے(j = 0; j < p.c; ++j){
        برائے(i = 0; i < p.w*p.h; ++i){
            p.ڈیٹا[i+j*p.h*p.w] = (p.ڈیٹا[i+j*p.h*p.w] - min[j])/(max[j]-min[j]);
        }
    }
    آزاد(min);
    آزاد(max);
}

خالی copy_image_into(image src, image dest)
{
    حافظہ_نقل(dest.ڈیٹا, src.ڈیٹا, src.h*src.w*src.c*حجم(اعشاری));
}

image copy_image(image p)
{
    image copy = p;
    copy.ڈیٹا = صفر_حافظہ_دو(p.h*p.w*p.c, حجم(اعشاری));
    حافظہ_نقل(copy.ڈیٹا, p.ڈیٹا, p.h*p.w*p.c*حجم(اعشاری));
    واپس copy;
}

خالی rgbgr_image(image im)
{
    صحیح i;
    برائے(i = 0; i < im.w*im.h; ++i){
        اعشاری تبادلہ = im.ڈیٹا[i];
        im.ڈیٹا[i] = im.ڈیٹا[i+im.w*im.h*2];
        im.ڈیٹا[i+im.w*im.h*2] = تبادلہ;
    }
}

صحیح show_image(image p, ثابت حرف *name, صحیح ms)
{
#اگر_تعریف_شدہ OPENCV
    صحیح c = show_image_cv(p, name, ms);
    واپس c;
#ورنہ
    fprintf(stderr, "Not compiled with OpenCV, saving to %s.png instead\n", name);
    save_image(p, name);
    واپس -1;
#اگر_ختم
}

خالی save_image_options(image im, ثابت حرف *name, IMTYPE f, صحیح quality)
{
    حرف buff[256];
    //sprintf(buff, "%s (%d)", name, windows);
    اگر(f == PNG)       sprintf(buff, "%s.png", name);
    ورنہ اگر (f == BMP) sprintf(buff, "%s.bmp", name);
    ورنہ اگر (f == TGA) sprintf(buff, "%s.tga", name);
    ورنہ اگر (f == JPG) sprintf(buff, "%s.jpg", name);
    ورنہ               sprintf(buff, "%s.png", name);
    غیرعلامتی حرف *ڈیٹا = صفر_حافظہ_دو(im.w*im.h*im.c, حجم(حرف));
    صحیح i,k;
    برائے(k = 0; k < im.c; ++k){
        برائے(i = 0; i < im.w*im.h; ++i){
            ڈیٹا[i*im.c+k] = (غیرعلامتی حرف) (255*im.ڈیٹا[i + k*im.w*im.h]);
        }
    }
    صحیح success = 0;
    اگر(f == PNG)       success = stbi_write_png(buff, im.w, im.h, im.c, ڈیٹا, im.w*im.c);
    ورنہ اگر (f == BMP) success = stbi_write_bmp(buff, im.w, im.h, im.c, ڈیٹا);
    ورنہ اگر (f == TGA) success = stbi_write_tga(buff, im.w, im.h, im.c, ڈیٹا);
    ورنہ اگر (f == JPG) success = stbi_write_jpg(buff, im.w, im.h, im.c, ڈیٹا, quality);
    آزاد(ڈیٹا);
    اگر(!success) fprintf(stderr, "Failed to write image %s\n", buff);
}

خالی save_image(image im, ثابت حرف *name)
{
    save_image_options(im, name, JPG, 80);
}

خالی show_image_layers(image p, حرف *name)
{
    صحیح i;
    حرف buff[256];
    برائے(i = 0; i < p.c; ++i){
        sprintf(buff, "%s - Layer %d", name, i);
        image layer = get_image_layer(p, i);
        show_image(layer, buff, 1);
        free_image(layer);
    }
}

خالی show_image_collapsed(image p, حرف *name)
{
    image c = collapse_image_layers(p, 1);
    show_image(c, name, 1);
    free_image(c);
}

image make_خالی_خانے_image(صحیح w, صحیح h, صحیح c)
{
    image برآمد;
    برآمد.ڈیٹا = 0;
    برآمد.h = h;
    برآمد.w = w;
    برآمد.c = c;
    واپس برآمد;
}

image make_image(صحیح w, صحیح h, صحیح c)
{
    image برآمد = make_خالی_خانے_image(w,h,c);
    برآمد.ڈیٹا = صفر_حافظہ_دو(h*w*c, حجم(اعشاری));
    واپس برآمد;
}

image make_random_image(صحیح w, صحیح h, صحیح c)
{
    image برآمد = make_خالی_خانے_image(w,h,c);
    برآمد.ڈیٹا = صفر_حافظہ_دو(h*w*c, حجم(اعشاری));
    صحیح i;
    برائے(i = 0; i < w*h*c; ++i){
        برآمد.ڈیٹا[i] = (rand_معمول() * .25) + .5;
    }
    واپس برآمد;
}

image float_مقصد_image(صحیح w, صحیح h, صحیح c, اعشاری *ڈیٹا)
{
    image برآمد = make_خالی_خانے_image(w,h,c);
    برآمد.ڈیٹا = ڈیٹا;
    واپس برآمد;
}

خالی رکھو_image(image im, صحیح w, صحیح h, صحیح dx, صحیح dy, image canvas)
{
    صحیح x, y, c;
    برائے(c = 0; c < im.c; ++c){
        برائے(y = 0; y < h; ++y){
            برائے(x = 0; x < w; ++x){
                اعشاری rx = ((اعشاری)x / w) * im.w;
                اعشاری ry = ((اعشاری)y / h) * im.h;
                اعشاری val = bilinear_interpolate(im, rx, ry, c);
                سیٹ_pixel(canvas, x + dx, y + dy, c, val);
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
    واپس r;
}

image rotate_crop_image(image im, اعشاری rad, اعشاری s, صحیح w, صحیح h, اعشاری dx, اعشاری dy, اعشاری aspect)
{
    صحیح x, y, c;
    اعشاری cx = im.w/2.;
    اعشاری cy = im.h/2.;
    image rot = make_image(w, h, im.c);
    برائے(c = 0; c < im.c; ++c){
        برائے(y = 0; y < h; ++y){
            برائے(x = 0; x < w; ++x){
                اعشاری rx = جیب_تمام(rad)*((x - w/2.)/s*aspect + dx/s*aspect) - جیب(rad)*((y - h/2.)/s + dy/s) + cx;
                اعشاری ry = جیب(rad)*((x - w/2.)/s*aspect + dx/s*aspect) + جیب_تمام(rad)*((y - h/2.)/s + dy/s) + cy;
                اعشاری val = bilinear_interpolate(im, rx, ry, c);
                سیٹ_pixel(rot, x, y, c, val);
            }
        }
    }
    واپس rot;
}

image rotate_image(image im, اعشاری rad)
{
    صحیح x, y, c;
    اعشاری cx = im.w/2.;
    اعشاری cy = im.h/2.;
    image rot = make_image(im.w, im.h, im.c);
    برائے(c = 0; c < im.c; ++c){
        برائے(y = 0; y < im.h; ++y){
            برائے(x = 0; x < im.w; ++x){
                اعشاری rx = جیب_تمام(rad)*(x-cx) - جیب(rad)*(y-cy) + cx;
                اعشاری ry = جیب(rad)*(x-cx) + جیب_تمام(rad)*(y-cy) + cy;
                اعشاری val = bilinear_interpolate(im, rx, ry, c);
                سیٹ_pixel(rot, x, y, c, val);
            }
        }
    }
    واپس rot;
}

خالی fill_image(image m, اعشاری s)
{
    صحیح i;
    برائے(i = 0; i < m.h*m.w*m.c; ++i) m.ڈیٹا[i] = s;
}

خالی translate_image(image m, اعشاری s)
{
    صحیح i;
    برائے(i = 0; i < m.h*m.w*m.c; ++i) m.ڈیٹا[i] += s;
}

خالی scale_image(image m, اعشاری s)
{
    صحیح i;
    برائے(i = 0; i < m.h*m.w*m.c; ++i) m.ڈیٹا[i] *= s;
}

image crop_image(image im, صحیح dx, صحیح dy, صحیح w, صحیح h)
{
    image cropped = make_image(w, h, im.c);
    صحیح i, j, k;
    برائے(k = 0; k < im.c; ++k){
        برائے(j = 0; j < h; ++j){
            برائے(i = 0; i < w; ++i){
                صحیح r = j + dy;
                صحیح c = i + dx;
                اعشاری val = 0;
                r = constrain_int(r, 0, im.h-1);
                c = constrain_int(c, 0, im.w-1);
                val = get_pixel(im, c, r, k);
                سیٹ_pixel(cropped, i, j, k, val);
            }
        }
    }
    واپس cropped;
}

صحیح بہترین_3d_سرکاؤ_r(image a, image b, صحیح min, صحیح max)
{
    اگر(min == max) واپس min;
    صحیح mid = floor((min + max) / 2.);
    image c1 = crop_image(b, 0, mid, b.w, b.h);
    image c2 = crop_image(b, 0, mid+1, b.w, b.h);
    اعشاری d1 = dist_array(c1.ڈیٹا, a.ڈیٹا, a.w*a.h*a.c, 10);
    اعشاری d2 = dist_array(c2.ڈیٹا, a.ڈیٹا, a.w*a.h*a.c, 10);
    free_image(c1);
    free_image(c2);
    اگر(d1 < d2) واپس بہترین_3d_سرکاؤ_r(a, b, min, mid);
    ورنہ واپس بہترین_3d_سرکاؤ_r(a, b, mid+1, max);
}

صحیح بہترین_3d_سرکاؤ(image a, image b, صحیح min, صحیح max)
{
    صحیح i;
    صحیح بہترین = 0;
    اعشاری بہترین_فاصلہ = FLT_MAX;
    برائے(i = min; i <= max; i += 2){
        image c = crop_image(b, 0, i, b.w, b.h);
        اعشاری d = dist_array(c.ڈیٹا, a.ڈیٹا, a.w*a.h*a.c, 100);
        اگر(d < بہترین_فاصلہ){
            بہترین_فاصلہ = d;
            بہترین = i;
        }
        لکھو("%d %f\n", i, d);
        free_image(c);
    }
    واپس بہترین;
}

خالی مرکب_3d(حرف *f1, حرف *f2, حرف *برآمد, صحیح delta)
{
    اگر(!برآمد) برآمد = "out";
    image a = بھرو_image(f1, 0,0,0);
    image b = بھرو_image(f2, 0,0,0);
    صحیح سرکاؤ = بہترین_3d_سرکاؤ_r(a, b, -a.h/100, a.h/100);

    image c1 = crop_image(b, 10, سرکاؤ, b.w, b.h);
    اعشاری d1 = dist_array(c1.ڈیٹا, a.ڈیٹا, a.w*a.h*a.c, 100);
    image c2 = crop_image(b, -10, سرکاؤ, b.w, b.h);
    اعشاری d2 = dist_array(c2.ڈیٹا, a.ڈیٹا, a.w*a.h*a.c, 100);

    اگر(d2 < d1 && 0){
        image تبادلہ = a;
        a = b;
        b = تبادلہ;
        سرکاؤ = -سرکاؤ;
        لکھو("swapped, %d\n", سرکاؤ);
    }
    ورنہ{
        لکھو("%d\n", سرکاؤ);
    }

    image c = crop_image(b, delta, سرکاؤ, a.w, a.h);
    صحیح i;
    برائے(i = 0; i < c.w*c.h; ++i){
        c.ڈیٹا[i] = a.ڈیٹا[i];
    }
    save_image(c, برآمد);
}

خالی letterbox_image_into(image im, صحیح w, صحیح h, image boxed)
{
    صحیح new_w = im.w;
    صحیح new_h = im.h;
    اگر (((اعشاری)w/im.w) < ((اعشاری)h/im.h)) {
        new_w = w;
        new_h = (im.h * w)/im.w;
    } ورنہ {
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
    } ورنہ {
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
    واپس boxed;
}

image resize_max(image im, صحیح max)
{
    صحیح w = im.w;
    صحیح h = im.h;
    اگر(w > h){
        h = (h * max) / w;
        w = max;
    } ورنہ {
        w = (w * max) / h;
        h = max;
    }
    اگر(w == im.w && h == im.h) واپس im;
    image resized = resize_image(im, w, h);
    واپس resized;
}

image resize_min(image im, صحیح min)
{
    صحیح w = im.w;
    صحیح h = im.h;
    اگر(w < h){
        h = (h * min) / w;
        w = min;
    } ورنہ {
        w = (w * min) / h;
        h = min;
    }
    اگر(w == im.w && h == im.h) واپس im;
    image resized = resize_image(im, w, h);
    واپس resized;
}

image random_crop_image(image im, صحیح w, صحیح h)
{
    صحیح dx = rand_int(0, im.w - w);
    صحیح dy = rand_int(0, im.h - h);
    image crop = crop_image(im, dx, dy, w, h);
    واپس crop;
}

augment_دلائل random_augment_دلائل(image im, اعشاری angle, اعشاری aspect, صحیح کم, صحیح بلند, صحیح w, صحیح h)
{
    augment_دلائل a = {0};
    aspect = rand_scale(aspect);
    صحیح r = rand_int(کم, بلند);
    صحیح min = (im.h < im.w*aspect) ? im.h : im.w*aspect;
    اعشاری scale = (اعشاری)r / min;

    اعشاری rad = rand_یکساں(-angle, angle) * TWO_PI / 360.;

    اعشاری dx = (im.w*scale/aspect - w) / 2.;
    اعشاری dy = (im.h*scale - w) / 2.;
    //if(dx < 0) dx = 0;
    //if(dy < 0) dy = 0;
    dx = rand_یکساں(-dx, dx);
    dy = rand_یکساں(-dy, dy);

    a.rad = rad;
    a.scale = scale;
    a.w = w;
    a.h = h;
    a.dx = dx;
    a.dy = dy;
    a.aspect = aspect;
    واپس a;
}

image random_augment_image(image im, اعشاری angle, اعشاری aspect, صحیح کم, صحیح بلند, صحیح w, صحیح h)
{
    augment_دلائل a = random_augment_دلائل(im, angle, aspect, کم, بلند, w, h);
    image crop = rotate_crop_image(im, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);
    واپس crop;
}

اعشاری three_way_max(اعشاری a, اعشاری b, اعشاری c)
{
    واپس (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

اعشاری three_way_min(اعشاری a, اعشاری b, اعشاری c)
{
    واپس (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

خالی yuv_مقصد_rgb(image im)
{
    أكد(im.c == 3);
    صحیح i, j;
    اعشاری r, g, b;
    اعشاری y, u, v;
    برائے(j = 0; j < im.h; ++j){
        برائے(i = 0; i < im.w; ++i){
            y = get_pixel(im, i , j, 0);
            u = get_pixel(im, i , j, 1);
            v = get_pixel(im, i , j, 2);

            r = y + 1.13983*v;
            g = y + -.39465*u + -.58060*v;
            b = y + 2.03211*u;

            سیٹ_pixel(im, i, j, 0, r);
            سیٹ_pixel(im, i, j, 1, g);
            سیٹ_pixel(im, i, j, 2, b);
        }
    }
}

خالی rgb_مقصد_yuv(image im)
{
    أكد(im.c == 3);
    صحیح i, j;
    اعشاری r, g, b;
    اعشاری y, u, v;
    برائے(j = 0; j < im.h; ++j){
        برائے(i = 0; i < im.w; ++i){
            r = get_pixel(im, i , j, 0);
            g = get_pixel(im, i , j, 1);
            b = get_pixel(im, i , j, 2);

            y = .299*r + .587*g + .114*b;
            u = -.14713*r + -.28886*g + .436*b;
            v = .615*r + -.51499*g + -.10001*b;

            سیٹ_pixel(im, i, j, 0, y);
            سیٹ_pixel(im, i, j, 1, u);
            سیٹ_pixel(im, i, j, 2, v);
        }
    }
}

// http://www.cs.rit.edu/~ncs/color/t_convert.html
خالی rgb_مقصد_hsv(image im)
{
    أكد(im.c == 3);
    صحیح i, j;
    اعشاری r, g, b;
    اعشاری h, s, v;
    برائے(j = 0; j < im.h; ++j){
        برائے(i = 0; i < im.w; ++i){
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
            }ورنہ{
                s = delta/max;
                اگر(r == max){
                    h = (g - b) / delta;
                } ورنہ اگر (g == max) {
                    h = 2 + (b - r) / delta;
                } ورنہ {
                    h = 4 + (r - g) / delta;
                }
                اگر (h < 0) h += 6;
                h = h/6.;
            }
            سیٹ_pixel(im, i, j, 0, h);
            سیٹ_pixel(im, i, j, 1, s);
            سیٹ_pixel(im, i, j, 2, v);
        }
    }
}

خالی hsv_مقصد_rgb(image im)
{
    أكد(im.c == 3);
    صحیح i, j;
    اعشاری r, g, b;
    اعشاری h, s, v;
    اعشاری f, p, q, t;
    برائے(j = 0; j < im.h; ++j){
        برائے(i = 0; i < im.w; ++i){
            h = 6 * get_pixel(im, i , j, 0);
            s = get_pixel(im, i , j, 1);
            v = get_pixel(im, i , j, 2);
            اگر (s == 0) {
                r = g = b = v;
            } ورنہ {
                صحیح اشاریہ = floor(h);
                f = h - اشاریہ;
                p = v*(1-s);
                q = v*(1-s*f);
                t = v*(1-s*(1-f));
                اگر(اشاریہ == 0){
                    r = v; g = t; b = p;
                } ورنہ اگر(اشاریہ == 1){
                    r = q; g = v; b = p;
                } ورنہ اگر(اشاریہ == 2){
                    r = p; g = v; b = t;
                } ورنہ اگر(اشاریہ == 3){
                    r = p; g = q; b = v;
                } ورنہ اگر(اشاریہ == 4){
                    r = t; g = p; b = v;
                } ورنہ {
                    r = v; g = p; b = q;
                }
            }
            سیٹ_pixel(im, i, j, 0, r);
            سیٹ_pixel(im, i, j, 1, g);
            سیٹ_pixel(im, i, j, 2, b);
        }
    }
}

خالی grayscale_image_3c(image im)
{
    أكد(im.c == 3);
    صحیح i, j, k;
    اعشاری scale[] = {0.299, 0.587, 0.114};
    برائے(j = 0; j < im.h; ++j){
        برائے(i = 0; i < im.w; ++i){
            اعشاری val = 0;
            برائے(k = 0; k < 3; ++k){
                val += scale[k]*get_pixel(im, i, j, k);
            }
            im.ڈیٹا[0*im.h*im.w + im.w*j + i] = val;
            im.ڈیٹا[1*im.h*im.w + im.w*j + i] = val;
            im.ڈیٹا[2*im.h*im.w + im.w*j + i] = val;
        }
    }
}

image grayscale_image(image im)
{
    أكد(im.c == 3);
    صحیح i, j, k;
    image سرمئی = make_image(im.w, im.h, 1);
    اعشاری scale[] = {0.299, 0.587, 0.114};
    برائے(k = 0; k < im.c; ++k){
        برائے(j = 0; j < im.h; ++j){
            برائے(i = 0; i < im.w; ++i){
                سرمئی.ڈیٹا[i+im.w*j] += scale[k]*get_pixel(im, i, j, k);
            }
        }
    }
    واپس سرمئی;
}

image حد_image(image im, اعشاری thresh)
{
    صحیح i;
    image t = make_image(im.w, im.h, im.c);
    برائے(i = 0; i < im.w*im.h*im.c; ++i){
        t.ڈیٹا[i] = im.ڈیٹا[i]>thresh ? 1 : 0;
    }
    واپس t;
}

image blend_image(image fore, image عقب, اعشاری alpha)
{
    أكد(fore.w == عقب.w && fore.h == عقب.h && fore.c == عقب.c);
    image blend = make_image(fore.w, fore.h, fore.c);
    صحیح i, j, k;
    برائے(k = 0; k < fore.c; ++k){
        برائے(j = 0; j < fore.h; ++j){
            برائے(i = 0; i < fore.w; ++i){
                اعشاری val = alpha * get_pixel(fore, i, j, k) + 
                    (1 - alpha)* get_pixel(عقب, i, j, k);
                سیٹ_pixel(blend, i, j, k, val);
            }
        }
    }
    واپس blend;
}

خالی scale_image_channel(image im, صحیح c, اعشاری v)
{
    صحیح i, j;
    برائے(j = 0; j < im.h; ++j){
        برائے(i = 0; i < im.w; ++i){
            اعشاری pix = get_pixel(im, i, j, c);
            pix = pix*v;
            سیٹ_pixel(im, i, j, c, pix);
        }
    }
}

خالی translate_image_channel(image im, صحیح c, اعشاری v)
{
    صحیح i, j;
    برائے(j = 0; j < im.h; ++j){
        برائے(i = 0; i < im.w; ++i){
            اعشاری pix = get_pixel(im, i, j, c);
            pix = pix+v;
            سیٹ_pixel(im, i, j, c, pix);
        }
    }
}

image binarize_image(image im)
{
    image c = copy_image(im);
    صحیح i;
    برائے(i = 0; i < im.w * im.h * im.c; ++i){
        اگر(c.ڈیٹا[i] > .5) c.ڈیٹا[i] = 1;
        ورنہ c.ڈیٹا[i] = 0;
    }
    واپس c;
}

خالی saturate_image(image im, اعشاری sat)
{
    rgb_مقصد_hsv(im);
    scale_image_channel(im, 1, sat);
    hsv_مقصد_rgb(im);
    constrain_image(im);
}

خالی hue_image(image im, اعشاری hue)
{
    rgb_مقصد_hsv(im);
    صحیح i;
    برائے(i = 0; i < im.w*im.h; ++i){
        im.ڈیٹا[i] = im.ڈیٹا[i] + hue;
        اگر (im.ڈیٹا[i] > 1) im.ڈیٹا[i] -= 1;
        اگر (im.ڈیٹا[i] < 0) im.ڈیٹا[i] += 1;
    }
    hsv_مقصد_rgb(im);
    constrain_image(im);
}

خالی exposure_image(image im, اعشاری sat)
{
    rgb_مقصد_hsv(im);
    scale_image_channel(im, 2, sat);
    hsv_مقصد_rgb(im);
    constrain_image(im);
}

خالی distort_image(image im, اعشاری hue, اعشاری sat, اعشاری val)
{
    rgb_مقصد_hsv(im);
    scale_image_channel(im, 1, sat);
    scale_image_channel(im, 2, val);
    صحیح i;
    برائے(i = 0; i < im.w*im.h; ++i){
        im.ڈیٹا[i] = im.ڈیٹا[i] + hue;
        اگر (im.ڈیٹا[i] > 1) im.ڈیٹا[i] -= 1;
        اگر (im.ڈیٹا[i] < 0) im.ڈیٹا[i] += 1;
    }
    hsv_مقصد_rgb(im);
    constrain_image(im);
}

خالی random_distort_image(image im, اعشاری hue, اعشاری saturation, اعشاری exposure)
{
    اعشاری dhue = rand_یکساں(-hue, hue);
    اعشاری dsat = rand_scale(saturation);
    اعشاری dexp = rand_scale(exposure);
    distort_image(im, dhue, dsat, dexp);
}

خالی saturate_exposure_image(image im, اعشاری sat, اعشاری exposure)
{
    rgb_مقصد_hsv(im);
    scale_image_channel(im, 1, sat);
    scale_image_channel(im, 2, exposure);
    hsv_مقصد_rgb(im);
    constrain_image(im);
}

image resize_image(image im, صحیح w, صحیح h)
{
    image resized = make_image(w, h, im.c);   
    image part = make_image(w, im.h, im.c);
    صحیح r, c, k;
    اعشاری w_scale = (اعشاری)(im.w - 1) / (w - 1);
    اعشاری h_scale = (اعشاری)(im.h - 1) / (h - 1);
    برائے(k = 0; k < im.c; ++k){
        برائے(r = 0; r < im.h; ++r){
            برائے(c = 0; c < w; ++c){
                اعشاری val = 0;
                اگر(c == w-1 || im.w == 1){
                    val = get_pixel(im, im.w-1, r, k);
                } ورنہ {
                    اعشاری sx = c*w_scale;
                    صحیح ix = (صحیح) sx;
                    اعشاری dx = sx - ix;
                    val = (1 - dx) * get_pixel(im, ix, r, k) + dx * get_pixel(im, ix+1, r, k);
                }
                سیٹ_pixel(part, c, r, k, val);
            }
        }
    }
    برائے(k = 0; k < im.c; ++k){
        برائے(r = 0; r < h; ++r){
            اعشاری sy = r*h_scale;
            صحیح iy = (صحیح) sy;
            اعشاری dy = sy - iy;
            برائے(c = 0; c < w; ++c){
                اعشاری val = (1-dy) * get_pixel(part, c, iy, k);
                سیٹ_pixel(resized, c, r, k, val);
            }
            اگر(r == h-1 || im.h == 1) جاری;
            برائے(c = 0; c < w; ++c){
                اعشاری val = dy * get_pixel(part, c, iy+1, k);
                جوڑو_pixel(resized, c, r, k, val);
            }
        }
    }

    free_image(part);
    واپس resized;
}


خالی test_resize(حرف *filename)
{
    image im = بھرو_image(filename, 0,0, 3);
    اعشاری mag = mag_array(im.ڈیٹا, im.w*im.h*im.c);
    لکھو("L2 Norm: %f\n", mag);
    image سرمئی = grayscale_image(im);

    image c1 = copy_image(im);
    image c2 = copy_image(im);
    image c3 = copy_image(im);
    image c4 = copy_image(im);
    distort_image(c1, .1, 1.5, 1.5);
    distort_image(c2, -.1, .66666, .66666);
    distort_image(c3, .1, 1.5, .66666);
    distort_image(c4, .1, .66666, 1.5);


    show_image(im,   "Original", 1);
    show_image(سرمئی, "Gray", 1);
    show_image(c1, "C1", 1);
    show_image(c2, "C2", 1);
    show_image(c3, "C3", 1);
    show_image(c4, "C4", 1);
#اگر_تعریف_شدہ OPENCV
    جب(1){
        image aug = random_augment_image(im, 0, .75, 320, 448, 320, 320);
        show_image(aug, "aug", 1);
        free_image(aug);


        اعشاری exposure = 1.15;
        اعشاری saturation = 1.15;
        اعشاری hue = .05;

        image c = copy_image(im);

        اعشاری dexp = rand_scale(exposure);
        اعشاری dsat = rand_scale(saturation);
        اعشاری dhue = rand_یکساں(-hue, hue);

        distort_image(c, dhue, dsat, dexp);
        show_image(c, "rand", 1);
        لکھو("%f %f %f\n", dhue, dsat, dexp);
        free_image(c);
    }
#اگر_ختم
}


image بھرو_image_stb(حرف *filename, صحیح channels)
{
    صحیح w, h, c;
    غیرعلامتی حرف *ڈیٹا = stbi_بھرو(filename, &w, &h, &c, channels);
    اگر (!ڈیٹا) {
        fprintf(stderr, "Cannot load image \"%s\"\nSTB Reason: %s\n", filename, stbi_failure_reason());
        باہر_نکلو(0);
    }
    اگر(channels) c = channels;
    صحیح i,j,k;
    image im = make_image(w, h, c);
    برائے(k = 0; k < c; ++k){
        برائے(j = 0; j < h; ++j){
            برائے(i = 0; i < w; ++i){
                صحیح dst_اشاریہ = i + w*j + w*h*k;
                صحیح src_اشاریہ = k + c*i + c*w*j;
                im.ڈیٹا[dst_اشاریہ] = (اعشاری)ڈیٹا[src_اشاریہ]/255.;
            }
        }
    }
    آزاد(ڈیٹا);
    واپس im;
}

image بھرو_image(حرف *filename, صحیح w, صحیح h, صحیح c)
{
#اگر_تعریف_شدہ OPENCV
    image برآمد = بھرو_image_cv(filename, c);
#ورنہ
    image برآمد = بھرو_image_stb(filename, c);
#اگر_ختم

    اگر((h && w) && (h != برآمد.h || w != برآمد.w)){
        image resized = resize_image(برآمد, w, h);
        free_image(برآمد);
        برآمد = resized;
    }
    واپس برآمد;
}

image بھرو_image_color(حرف *filename, صحیح w, صحیح h)
{
    واپس بھرو_image(filename, w, h, 3);
}

image get_image_layer(image m, صحیح l)
{
    image برآمد = make_image(m.w, m.h, 1);
    صحیح i;
    برائے(i = 0; i < m.h*m.w; ++i){
        برآمد.ڈیٹا[i] = m.ڈیٹا[i+l*m.h*m.w];
    }
    واپس برآمد;
}
خالی print_image(image m)
{
    صحیح i, j, k;
    برائے(i =0 ; i < m.c; ++i){
        برائے(j =0 ; j < m.h; ++j){
            برائے(k = 0; k < m.w; ++k){
                لکھو("%.2lf, ", m.ڈیٹا[i*m.h*m.w + j*m.w + k]);
                اگر(k > 30) توڑو;
            }
            لکھو("\n");
            اگر(j > 30) توڑو;
        }
        لکھو("\n");
    }
    لکھو("\n");
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
    برائے(i = 0; i < n; ++i){
        صحیح h_انحراف = i*(ims[0].h+border);
        image copy = copy_image(ims[i]);
        //normalize_image(copy);
        اگر(c == 3 && color){
            embed_image(copy, filters, 0, h_انحراف);
        }
        ورنہ{
            برائے(j = 0; j < copy.c; ++j){
                صحیح w_انحراف = j*(ims[0].w+border);
                image layer = get_image_layer(copy, j);
                embed_image(layer, filters, w_انحراف, h_انحراف);
                free_image(layer);
            }
        }
        free_image(copy);
    }
    واپس filters;
} 

image collapse_images_horz(image *ims, صحیح n)
{
    صحیح color = 1;
    صحیح border = 1;
    صحیح h,w,c;
    صحیح سائز = ims[0].h;
    h = سائز;
    w = (ims[0].w + border) * n - border;
    c = ims[0].c;
    اگر(c != 3 || !color){
        h = (h+border)*c - border;
        c = 1;
    }

    image filters = make_image(w, h, c);
    صحیح i,j;
    برائے(i = 0; i < n; ++i){
        صحیح w_انحراف = i*(سائز+border);
        image copy = copy_image(ims[i]);
        //normalize_image(copy);
        اگر(c == 3 && color){
            embed_image(copy, filters, w_انحراف, 0);
        }
        ورنہ{
            برائے(j = 0; j < copy.c; ++j){
                صحیح h_انحراف = j*(سائز+border);
                image layer = get_image_layer(copy, j);
                embed_image(layer, filters, w_انحراف, h_انحراف);
                free_image(layer);
            }
        }
        free_image(copy);
    }
    واپس filters;
} 

خالی show_image_normalized(image im, ثابت حرف *name)
{
    image c = copy_image(im);
    normalize_image(c);
    show_image(c, name, 1);
    free_image(c);
}

خالی show_images(image *ims, صحیح n, حرف *کھڑکی)
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
    save_image(m, کھڑکی);
    show_image(m, کھڑکی, 1);
    free_image(m);
}

خالی free_image(image m)
{
    اگر(m.ڈیٹا){
        آزاد(m.ڈیٹا);
    }
}
