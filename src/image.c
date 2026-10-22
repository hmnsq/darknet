#அந்தர்பாவ "image.h"
#அந்தர்பாவ "utils.h"
#அந்தர்பாவ "blas.h"
#அந்தர்பாவ "cuda.h"
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <math.h>

#define STB_IMAGE_IMPLEMENTATION
#அந்தர்பாவ "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#அந்தர்பாவ "stb_image_write.h"

முழு windows = 0;

தசம colors[6][3] = { {1,0,1}, {0,0,1},{0,1,1},{0,1,0},{1,1,0},{1,0,0} };

தசம get_color(முழு c, முழு x, முழு max)
{
    தசம ratio = ((தசம)x/max)*5;
    முழு i = floor(ratio);
    முழு j = ceil(ratio);
    ratio -= i;
    தசம r = (1-ratio) * colors[i][c] + ratio*colors[j][c];
    //printf("%f\n", r);
    திரும்பு r;
}

image mask_to_rgb(image mask)
{
    முழு n = mask.c;
    image im = make_image(mask.w, mask.h, 3);
    முழு i, j;
    க்கு(j = 0; j < n; ++j){
        முழு offset = j*123457 % n;
        தசம red = get_color(2,offset,n);
        தசம green = get_color(1,offset,n);
        தசம blue = get_color(0,offset,n);
        க்கு(i = 0; i < im.w*im.h; ++i){
            im.data[i + 0*im.w*im.h] += mask.data[j*im.h*im.w + i]*red;
            im.data[i + 1*im.w*im.h] += mask.data[j*im.h*im.w + i]*green;
            im.data[i + 2*im.w*im.h] += mask.data[j*im.h*im.w + i]*blue;
        }
    }
    திரும்பு im;
}

நிலையான தசம get_pixel(image m, முழு x, முழு y, முழு c)
{
    ப்ரதிஜ்ஞா(x < m.w && y < m.h && c < m.c);
    திரும்பு m.data[c*m.h*m.w + y*m.w + x];
}
நிலையான தசம get_pixel_extend(image m, முழு x, முழு y, முழு c)
{
    எனில்(x < 0 || x >= m.w || y < 0 || y >= m.h) திரும்பு 0;
    /*
    if(x < 0) x = 0;
    if(x >= m.w) x = m.w-1;
    if(y < 0) y = 0;
    if(y >= m.h) y = m.h-1;
    */
    எனில்(c < 0 || c >= m.c) திரும்பு 0;
    திரும்பு get_pixel(m, x, y, c);
}
நிலையான வெறுமை set_pixel(image m, முழு x, முழு y, முழு c, தசம val)
{
    எனில் (x < 0 || y < 0 || c < 0 || x >= m.w || y >= m.h || c >= m.c) திரும்பு;
    ப்ரதிஜ்ஞா(x < m.w && y < m.h && c < m.c);
    m.data[c*m.h*m.w + y*m.w + x] = val;
}
நிலையான வெறுமை add_pixel(image m, முழு x, முழு y, முழு c, தசம val)
{
    ப்ரதிஜ்ஞா(x < m.w && y < m.h && c < m.c);
    m.data[c*m.h*m.w + y*m.w + x] += val;
}

நிலையான தசம bilinear_interpolate(image im, தசம x, தசம y, முழு c)
{
    முழு ix = (முழு) floorf(x);
    முழு iy = (முழு) floorf(y);

    தசம dx = x - ix;
    தசம dy = y - iy;

    தசம val = (1-dy) * (1-dx) * get_pixel_extend(im, ix, iy, c) + 
        dy     * (1-dx) * get_pixel_extend(im, ix, iy+1, c) + 
        (1-dy) *   dx   * get_pixel_extend(im, ix+1, iy, c) +
        dy     *   dx   * get_pixel_extend(im, ix+1, iy+1, c);
    திரும்பு val;
}


வெறுமை composite_image(image source, image dest, முழு dx, முழு dy)
{
    முழு x,y,k;
    க்கு(k = 0; k < source.c; ++k){
        க்கு(y = 0; y < source.h; ++y){
            க்கு(x = 0; x < source.w; ++x){
                தசம val = get_pixel(source, x, y, k);
                தசம val2 = get_pixel_extend(dest, dx+x, dy+y, k);
                set_pixel(dest, dx+x, dy+y, k, val * val2);
            }
        }
    }
}

image border_image(image a, முழு border)
{
    image b = make_image(a.w + 2*border, a.h + 2*border, a.c);
    முழு x,y,k;
    க்கு(k = 0; k < b.c; ++k){
        க்கு(y = 0; y < b.h; ++y){
            க்கு(x = 0; x < b.w; ++x){
                தசம val = get_pixel_extend(a, x - border, y - border, k);
                எனில்(x - border < 0 || x - border >= a.w || y - border < 0 || y - border >= a.h) val = 1;
                set_pixel(b, x, y, k, val);
            }
        }
    }
    திரும்பு b;
}

image tile_images(image a, image b, முழு dx)
{
    எனில்(a.w == 0) திரும்பு copy_image(b);
    image c = make_image(a.w + b.w + dx, (a.h > b.h) ? a.h : b.h, (a.c > b.c) ? a.c : b.c);
    fill_cpu(c.w*c.h*c.c, 1, c.data, 1);
    embed_image(a, c, 0, 0); 
    composite_image(b, c, a.w + dx, 0);
    திரும்பு c;
}

image get_label(image **characters, எழுத்து *string, முழு size)
{
    size = size/10;
    எனில்(size > 7) size = 7;
    image label = make_empty_image(0,0,0);
    போது(*string){
        image l = characters[size][(முழு)*string];
        image n = tile_images(label, l, -size - 1 + (size+1)/2);
        free_image(label);
        label = n;
        ++string;
    }
    image b = border_image(label, label.h*.25);
    free_image(label);
    திரும்பு b;
}

வெறுமை draw_label(image a, முழு r, முழு c, image label, மாறிலி தசம *rgb)
{
    முழு w = label.w;
    முழு h = label.h;
    எனில் (r - h >= 0) r = r - h;

    முழு i, j, k;
    க்கு(j = 0; j < h && j + r < a.h; ++j){
        க்கு(i = 0; i < w && i + c < a.w; ++i){
            க்கு(k = 0; k < label.c; ++k){
                தசம val = get_pixel(label, i, j, k);
                set_pixel(a, i+c, j+r, k, rgb[k] * val);
            }
        }
    }
}

வெறுமை draw_box(image a, முழு x1, முழு y1, முழு x2, முழு y2, தசம r, தசம g, தசம b)
{
    //normalize_image(a);
    முழு i;
    எனில்(x1 < 0) x1 = 0;
    எனில்(x1 >= a.w) x1 = a.w-1;
    எனில்(x2 < 0) x2 = 0;
    எனில்(x2 >= a.w) x2 = a.w-1;

    எனில்(y1 < 0) y1 = 0;
    எனில்(y1 >= a.h) y1 = a.h-1;
    எனில்(y2 < 0) y2 = 0;
    எனில்(y2 >= a.h) y2 = a.h-1;

    க்கு(i = x1; i <= x2; ++i){
        a.data[i + y1*a.w + 0*a.w*a.h] = r;
        a.data[i + y2*a.w + 0*a.w*a.h] = r;

        a.data[i + y1*a.w + 1*a.w*a.h] = g;
        a.data[i + y2*a.w + 1*a.w*a.h] = g;

        a.data[i + y1*a.w + 2*a.w*a.h] = b;
        a.data[i + y2*a.w + 2*a.w*a.h] = b;
    }
    க்கு(i = y1; i <= y2; ++i){
        a.data[x1 + i*a.w + 0*a.w*a.h] = r;
        a.data[x2 + i*a.w + 0*a.w*a.h] = r;

        a.data[x1 + i*a.w + 1*a.w*a.h] = g;
        a.data[x2 + i*a.w + 1*a.w*a.h] = g;

        a.data[x1 + i*a.w + 2*a.w*a.h] = b;
        a.data[x2 + i*a.w + 2*a.w*a.h] = b;
    }
}

வெறுமை draw_box_width(image a, முழு x1, முழு y1, முழு x2, முழு y2, முழு w, தசம r, தசம g, தசம b)
{
    முழு i;
    க்கு(i = 0; i < w; ++i){
        draw_box(a, x1+i, y1+i, x2-i, y2-i, r, g, b);
    }
}

வெறுமை draw_bbox(image a, box bbox, முழு w, தசம r, தசம g, தசம b)
{
    முழு இடது  = (bbox.x-bbox.w/2)*a.w;
    முழு வலது = (bbox.x+bbox.w/2)*a.w;
    முழு உச்சி   = (bbox.y-bbox.h/2)*a.h;
    முழு bot   = (bbox.y+bbox.h/2)*a.h;

    முழு i;
    க்கு(i = 0; i < w; ++i){
        draw_box(a, இடது+i, உச்சி+i, வலது-i, bot-i, r, g, b);
    }
}

image **load_alphabet()
{
    முழு i, j;
    மாறிலி முழு nsize = 8;
    image **alphabets = calloc(nsize, அளவு(image));
    க்கு(j = 0; j < nsize; ++j){
        alphabets[j] = calloc(128, அளவு(image));
        க்கு(i = 32; i < 127; ++i){
            எழுத்து buff[256];
            sprintf(buff, "data/labels/%d_%d.png", i, j);
            alphabets[j][i] = load_image_color(buff, 0, 0);
        }
    }
    திரும்பு alphabets;
}

வெறுமை draw_detections(image im, detection *dets, முழு இலக்கம், தசம thresh, எழுத்து **names, image **alphabet, முழு classes)
{
    முழு i,j;

    க்கு(i = 0; i < இலக்கம்; ++i){
        எழுத்து labelstr[4096] = {0};
        முழு class = -1;
        க்கு(j = 0; j < classes; ++j){
            எனில் (dets[i].prob[j] > thresh){
                எனில் (class < 0) {
                    strcat(labelstr, names[j]);
                    class = j;
                } இல்லையெனில் {
                    strcat(labelstr, ", ");
                    strcat(labelstr, names[j]);
                }
                printf("%s: %.0f%%\n", names[j], dets[i].prob[j]*100);
            }
        }
        எனில்(class >= 0){
            முழு width = im.h * .006;

            /*
               if(0){
               width = pow(prob, 1./2.)*10+1;
               alphabet = 0;
               }
             */

            //printf("%d %s: %.0f%%\n", i, names[class], prob*100);
            முழு offset = class*123457 % classes;
            தசம red = get_color(2,offset,classes);
            தசம green = get_color(1,offset,classes);
            தசம blue = get_color(0,offset,classes);
            தசம rgb[3];

            //width = prob*20+2;

            rgb[0] = red;
            rgb[1] = green;
            rgb[2] = blue;
            box b = dets[i].bbox;
            //printf("%f %f %f %f\n", b.x, b.y, b.w, b.h);

            முழு இடது  = (b.x-b.w/2.)*im.w;
            முழு வலது = (b.x+b.w/2.)*im.w;
            முழு உச்சி   = (b.y-b.h/2.)*im.h;
            முழு bot   = (b.y+b.h/2.)*im.h;

            எனில்(இடது < 0) இடது = 0;
            எனில்(வலது > im.w-1) வலது = im.w-1;
            எனில்(உச்சி < 0) உச்சி = 0;
            எனில்(bot > im.h-1) bot = im.h-1;

            draw_box_width(im, இடது, உச்சி, வலது, bot, width, red, green, blue);
            எனில் (alphabet) {
                image label = get_label(alphabet, labelstr, (im.h*.03));
                draw_label(im, உச்சி + width, இடது, label, rgb);
                free_image(label);
            }
            எனில் (dets[i].mask){
                image mask = float_to_image(14, 14, 1, dets[i].mask);
                image resized_mask = resize_image(mask, b.w*im.w, b.h*im.h);
                image tmask = threshold_image(resized_mask, .5);
                embed_image(tmask, im, இடது, உச்சி);
                free_image(mask);
                free_image(resized_mask);
                free_image(tmask);
            }
        }
    }
}

வெறுமை transpose_image(image im)
{
    ப்ரதிஜ்ஞா(im.w == im.h);
    முழு n, m;
    முழு c;
    க்கு(c = 0; c < im.c; ++c){
        க்கு(n = 0; n < im.w-1; ++n){
            க்கு(m = n + 1; m < im.w; ++m){
                தசம swap = im.data[m + im.w*(n + im.h*c)];
                im.data[m + im.w*(n + im.h*c)] = im.data[n + im.w*(m + im.h*c)];
                im.data[n + im.w*(m + im.h*c)] = swap;
            }
        }
    }
}

வெறுமை rotate_image_cw(image im, முழு times)
{
    ப்ரதிஜ்ஞா(im.w == im.h);
    times = (times + 400) % 4;
    முழு i, x, y, c;
    முழு n = im.w;
    க்கு(i = 0; i < times; ++i){
        க்கு(c = 0; c < im.c; ++c){
            க்கு(x = 0; x < n/2; ++x){
                க்கு(y = 0; y < (n-1)/2 + 1; ++y){
                    தசம temp = im.data[y + im.w*(x + im.h*c)];
                    im.data[y + im.w*(x + im.h*c)] = im.data[n-1-x + im.w*(y + im.h*c)];
                    im.data[n-1-x + im.w*(y + im.h*c)] = im.data[n-1-y + im.w*(n-1-x + im.h*c)];
                    im.data[n-1-y + im.w*(n-1-x + im.h*c)] = im.data[x + im.w*(n-1-y + im.h*c)];
                    im.data[x + im.w*(n-1-y + im.h*c)] = temp;
                }
            }
        }
    }
}

வெறுமை flip_image(image a)
{
    முழு i,j,k;
    க்கு(k = 0; k < a.c; ++k){
        க்கு(i = 0; i < a.h; ++i){
            க்கு(j = 0; j < a.w/2; ++j){
                முழு index = j + a.w*(i + a.h*(k));
                முழு flip = (a.w - j - 1) + a.w*(i + a.h*(k));
                தசம swap = a.data[flip];
                a.data[flip] = a.data[index];
                a.data[index] = swap;
            }
        }
    }
}

image image_distance(image a, image b)
{
    முழு i,j;
    image dist = make_image(a.w, a.h, 1);
    க்கு(i = 0; i < a.c; ++i){
        க்கு(j = 0; j < a.h*a.w; ++j){
            dist.data[j] += pow(a.data[i*a.h*a.w+j]-b.data[i*a.h*a.w+j],2);
        }
    }
    க்கு(j = 0; j < a.h*a.w; ++j){
        dist.data[j] = sqrt(dist.data[j]);
    }
    திரும்பு dist;
}

வெறுமை ghost_image(image source, image dest, முழு dx, முழு dy)
{
    முழு x,y,k;
    தசம max_dist = sqrt((-source.w/2. + .5)*(-source.w/2. + .5));
    க்கு(k = 0; k < source.c; ++k){
        க்கு(y = 0; y < source.h; ++y){
            க்கு(x = 0; x < source.w; ++x){
                தசம dist = sqrt((x - source.w/2. + .5)*(x - source.w/2. + .5) + (y - source.h/2. + .5)*(y - source.h/2. + .5));
                தசம alpha = (1 - dist/max_dist);
                எனில்(alpha < 0) alpha = 0;
                தசம v1 = get_pixel(source, x,y,k);
                தசம v2 = get_pixel(dest, dx+x,dy+y,k);
                தசம val = alpha*v1 + (1-alpha)*v2;
                set_pixel(dest, dx+x, dy+y, k, val);
            }
        }
    }
}

வெறுமை blocky_image(image im, முழு s)
{
    முழு i,j,k;
    க்கு(k = 0; k < im.c; ++k){
        க்கு(j = 0; j < im.h; ++j){
            க்கு(i = 0; i < im.w; ++i){
                im.data[i + im.w*(j + im.h*k)] = im.data[i/s*s + im.w*(j/s*s + im.h*k)];
            }
        }
    }
}

வெறுமை censor_image(image im, முழு dx, முழு dy, முழு w, முழு h)
{
    முழு i,j,k;
    முழு s = 32;
    எனில்(dx < 0) dx = 0;
    எனில்(dy < 0) dy = 0;

    க்கு(k = 0; k < im.c; ++k){
        க்கு(j = dy; j < dy + h && j < im.h; ++j){
            க்கு(i = dx; i < dx + w && i < im.w; ++i){
                im.data[i + im.w*(j + im.h*k)] = im.data[i/s*s + im.w*(j/s*s + im.h*k)];
                //im.data[i + j*im.w + k*im.w*im.h] = 0;
            }
        }
    }
}

வெறுமை embed_image(image source, image dest, முழு dx, முழு dy)
{
    முழு x,y,k;
    க்கு(k = 0; k < source.c; ++k){
        க்கு(y = 0; y < source.h; ++y){
            க்கு(x = 0; x < source.w; ++x){
                தசம val = get_pixel(source, x,y,k);
                set_pixel(dest, dx+x, dy+y, k, val);
            }
        }
    }
}

image collapse_image_layers(image source, முழு border)
{
    முழு h = source.h;
    h = (h+border)*source.c - border;
    image dest = make_image(source.w, h, 1);
    முழு i;
    க்கு(i = 0; i < source.c; ++i){
        image layer = get_image_layer(source, i);
        முழு h_offset = i*(source.h+border);
        embed_image(layer, dest, 0, h_offset);
        free_image(layer);
    }
    திரும்பு dest;
}

வெறுமை constrain_image(image im)
{
    முழு i;
    க்கு(i = 0; i < im.w*im.h*im.c; ++i){
        எனில்(im.data[i] < 0) im.data[i] = 0;
        எனில்(im.data[i] > 1) im.data[i] = 1;
    }
}

வெறுமை normalize_image(image p)
{
    முழு i;
    தசம min = 9999999;
    தசம max = -999999;

    க்கு(i = 0; i < p.h*p.w*p.c; ++i){
        தசம v = p.data[i];
        எனில்(v < min) min = v;
        எனில்(v > max) max = v;
    }
    எனில்(max - min < .000000001){
        min = 0;
        max = 1;
    }
    க்கு(i = 0; i < p.c*p.w*p.h; ++i){
        p.data[i] = (p.data[i] - min)/(max-min);
    }
}

வெறுமை normalize_image2(image p)
{
    தசம *min = calloc(p.c, அளவு(தசம));
    தசம *max = calloc(p.c, அளவு(தசம));
    முழு i,j;
    க்கு(i = 0; i < p.c; ++i) min[i] = max[i] = p.data[i*p.h*p.w];

    க்கு(j = 0; j < p.c; ++j){
        க்கு(i = 0; i < p.h*p.w; ++i){
            தசம v = p.data[i+j*p.h*p.w];
            எனில்(v < min[j]) min[j] = v;
            எனில்(v > max[j]) max[j] = v;
        }
    }
    க்கு(i = 0; i < p.c; ++i){
        எனில்(max[i] - min[i] < .000000001){
            min[i] = 0;
            max[i] = 1;
        }
    }
    க்கு(j = 0; j < p.c; ++j){
        க்கு(i = 0; i < p.w*p.h; ++i){
            p.data[i+j*p.h*p.w] = (p.data[i+j*p.h*p.w] - min[j])/(max[j]-min[j]);
        }
    }
    விடுவி(min);
    விடுவி(max);
}

வெறுமை copy_image_into(image src, image dest)
{
    memcpy(dest.data, src.data, src.h*src.w*src.c*அளவு(தசம));
}

image copy_image(image p)
{
    image copy = p;
    copy.data = calloc(p.h*p.w*p.c, அளவு(தசம));
    memcpy(copy.data, p.data, p.h*p.w*p.c*அளவு(தசம));
    திரும்பு copy;
}

வெறுமை rgbgr_image(image im)
{
    முழு i;
    க்கு(i = 0; i < im.w*im.h; ++i){
        தசம swap = im.data[i];
        im.data[i] = im.data[i+im.w*im.h*2];
        im.data[i+im.w*im.h*2] = swap;
    }
}

முழு show_image(image p, மாறிலி எழுத்து *name, முழு ms)
{
#ifdef OPENCV
    முழு c = show_image_cv(p, name, ms);
    திரும்பு c;
#இல்லையெனில்
    fprintf(stderr, "Not compiled with OpenCV, saving to %s.png instead\n", name);
    save_image(p, name);
    திரும்பு -1;
#endif
}

வெறுமை save_image_options(image im, மாறிலி எழுத்து *name, IMTYPE f, முழு quality)
{
    எழுத்து buff[256];
    //sprintf(buff, "%s (%d)", name, windows);
    எனில்(f == PNG)       sprintf(buff, "%s.png", name);
    இல்லையெனில் எனில் (f == BMP) sprintf(buff, "%s.bmp", name);
    இல்லையெனில் எனில் (f == TGA) sprintf(buff, "%s.tga", name);
    இல்லையெனில் எனில் (f == JPG) sprintf(buff, "%s.jpg", name);
    இல்லையெனில்               sprintf(buff, "%s.png", name);
    unsigned எழுத்து *data = calloc(im.w*im.h*im.c, அளவு(எழுத்து));
    முழு i,k;
    க்கு(k = 0; k < im.c; ++k){
        க்கு(i = 0; i < im.w*im.h; ++i){
            data[i*im.c+k] = (unsigned எழுத்து) (255*im.data[i + k*im.w*im.h]);
        }
    }
    முழு success = 0;
    எனில்(f == PNG)       success = stbi_write_png(buff, im.w, im.h, im.c, data, im.w*im.c);
    இல்லையெனில் எனில் (f == BMP) success = stbi_write_bmp(buff, im.w, im.h, im.c, data);
    இல்லையெனில் எனில் (f == TGA) success = stbi_write_tga(buff, im.w, im.h, im.c, data);
    இல்லையெனில் எனில் (f == JPG) success = stbi_write_jpg(buff, im.w, im.h, im.c, data, quality);
    விடுவி(data);
    எனில்(!success) fprintf(stderr, "Failed to write image %s\n", buff);
}

வெறுமை save_image(image im, மாறிலி எழுத்து *name)
{
    save_image_options(im, name, JPG, 80);
}

வெறுமை show_image_layers(image p, எழுத்து *name)
{
    முழு i;
    எழுத்து buff[256];
    க்கு(i = 0; i < p.c; ++i){
        sprintf(buff, "%s - Layer %d", name, i);
        image layer = get_image_layer(p, i);
        show_image(layer, buff, 1);
        free_image(layer);
    }
}

வெறுமை show_image_collapsed(image p, எழுத்து *name)
{
    image c = collapse_image_layers(p, 1);
    show_image(c, name, 1);
    free_image(c);
}

image make_empty_image(முழு w, முழு h, முழு c)
{
    image out;
    out.data = 0;
    out.h = h;
    out.w = w;
    out.c = c;
    திரும்பு out;
}

image make_image(முழு w, முழு h, முழு c)
{
    image out = make_empty_image(w,h,c);
    out.data = calloc(h*w*c, அளவு(தசம));
    திரும்பு out;
}

image make_random_image(முழு w, முழு h, முழு c)
{
    image out = make_empty_image(w,h,c);
    out.data = calloc(h*w*c, அளவு(தசம));
    முழு i;
    க்கு(i = 0; i < w*h*c; ++i){
        out.data[i] = (rand_normal() * .25) + .5;
    }
    திரும்பு out;
}

image float_to_image(முழு w, முழு h, முழு c, தசம *data)
{
    image out = make_empty_image(w,h,c);
    out.data = data;
    திரும்பு out;
}

வெறுமை place_image(image im, முழு w, முழு h, முழு dx, முழு dy, image canvas)
{
    முழு x, y, c;
    க்கு(c = 0; c < im.c; ++c){
        க்கு(y = 0; y < h; ++y){
            க்கு(x = 0; x < w; ++x){
                தசம rx = ((தசம)x / w) * im.w;
                தசம ry = ((தசம)y / h) * im.h;
                தசம val = bilinear_interpolate(im, rx, ry, c);
                set_pixel(canvas, x + dx, y + dy, c, val);
            }
        }
    }
}

image center_crop_image(image im, முழு w, முழு h)
{
    முழு m = (im.w < im.h) ? im.w : im.h;   
    image c = crop_image(im, (im.w - m) / 2, (im.h - m)/2, m, m);
    image r = resize_image(c, w, h);
    free_image(c);
    திரும்பு r;
}

image rotate_crop_image(image im, தசம rad, தசம s, முழு w, முழு h, தசம dx, தசம dy, தசம aspect)
{
    முழு x, y, c;
    தசம cx = im.w/2.;
    தசம cy = im.h/2.;
    image rot = make_image(w, h, im.c);
    க்கு(c = 0; c < im.c; ++c){
        க்கு(y = 0; y < h; ++y){
            க்கு(x = 0; x < w; ++x){
                தசம rx = cos(rad)*((x - w/2.)/s*aspect + dx/s*aspect) - sin(rad)*((y - h/2.)/s + dy/s) + cx;
                தசம ry = sin(rad)*((x - w/2.)/s*aspect + dx/s*aspect) + cos(rad)*((y - h/2.)/s + dy/s) + cy;
                தசம val = bilinear_interpolate(im, rx, ry, c);
                set_pixel(rot, x, y, c, val);
            }
        }
    }
    திரும்பு rot;
}

image rotate_image(image im, தசம rad)
{
    முழு x, y, c;
    தசம cx = im.w/2.;
    தசம cy = im.h/2.;
    image rot = make_image(im.w, im.h, im.c);
    க்கு(c = 0; c < im.c; ++c){
        க்கு(y = 0; y < im.h; ++y){
            க்கு(x = 0; x < im.w; ++x){
                தசம rx = cos(rad)*(x-cx) - sin(rad)*(y-cy) + cx;
                தசம ry = sin(rad)*(x-cx) + cos(rad)*(y-cy) + cy;
                தசம val = bilinear_interpolate(im, rx, ry, c);
                set_pixel(rot, x, y, c, val);
            }
        }
    }
    திரும்பு rot;
}

வெறுமை fill_image(image m, தசம s)
{
    முழு i;
    க்கு(i = 0; i < m.h*m.w*m.c; ++i) m.data[i] = s;
}

வெறுமை translate_image(image m, தசம s)
{
    முழு i;
    க்கு(i = 0; i < m.h*m.w*m.c; ++i) m.data[i] += s;
}

வெறுமை scale_image(image m, தசம s)
{
    முழு i;
    க்கு(i = 0; i < m.h*m.w*m.c; ++i) m.data[i] *= s;
}

image crop_image(image im, முழு dx, முழு dy, முழு w, முழு h)
{
    image cropped = make_image(w, h, im.c);
    முழு i, j, k;
    க்கு(k = 0; k < im.c; ++k){
        க்கு(j = 0; j < h; ++j){
            க்கு(i = 0; i < w; ++i){
                முழு r = j + dy;
                முழு c = i + dx;
                தசம val = 0;
                r = constrain_int(r, 0, im.h-1);
                c = constrain_int(c, 0, im.w-1);
                val = get_pixel(im, c, r, k);
                set_pixel(cropped, i, j, k, val);
            }
        }
    }
    திரும்பு cropped;
}

முழு best_3d_shift_r(image a, image b, முழு min, முழு max)
{
    எனில்(min == max) திரும்பு min;
    முழு mid = floor((min + max) / 2.);
    image c1 = crop_image(b, 0, mid, b.w, b.h);
    image c2 = crop_image(b, 0, mid+1, b.w, b.h);
    தசம d1 = dist_array(c1.data, a.data, a.w*a.h*a.c, 10);
    தசம d2 = dist_array(c2.data, a.data, a.w*a.h*a.c, 10);
    free_image(c1);
    free_image(c2);
    எனில்(d1 < d2) திரும்பு best_3d_shift_r(a, b, min, mid);
    இல்லையெனில் திரும்பு best_3d_shift_r(a, b, mid+1, max);
}

முழு best_3d_shift(image a, image b, முழு min, முழு max)
{
    முழு i;
    முழு best = 0;
    தசம best_distance = FLT_MAX;
    க்கு(i = min; i <= max; i += 2){
        image c = crop_image(b, 0, i, b.w, b.h);
        தசம d = dist_array(c.data, a.data, a.w*a.h*a.c, 100);
        எனில்(d < best_distance){
            best_distance = d;
            best = i;
        }
        printf("%d %f\n", i, d);
        free_image(c);
    }
    திரும்பு best;
}

வெறுமை composite_3d(எழுத்து *f1, எழுத்து *f2, எழுத்து *out, முழு delta)
{
    எனில்(!out) out = "out";
    image a = load_image(f1, 0,0,0);
    image b = load_image(f2, 0,0,0);
    முழு shift = best_3d_shift_r(a, b, -a.h/100, a.h/100);

    image c1 = crop_image(b, 10, shift, b.w, b.h);
    தசம d1 = dist_array(c1.data, a.data, a.w*a.h*a.c, 100);
    image c2 = crop_image(b, -10, shift, b.w, b.h);
    தசம d2 = dist_array(c2.data, a.data, a.w*a.h*a.c, 100);

    எனில்(d2 < d1 && 0){
        image swap = a;
        a = b;
        b = swap;
        shift = -shift;
        printf("swapped, %d\n", shift);
    }
    இல்லையெனில்{
        printf("%d\n", shift);
    }

    image c = crop_image(b, delta, shift, a.w, a.h);
    முழு i;
    க்கு(i = 0; i < c.w*c.h; ++i){
        c.data[i] = a.data[i];
    }
    save_image(c, out);
}

வெறுமை letterbox_image_into(image im, முழு w, முழு h, image boxed)
{
    முழு new_w = im.w;
    முழு new_h = im.h;
    எனில் (((தசம)w/im.w) < ((தசம)h/im.h)) {
        new_w = w;
        new_h = (im.h * w)/im.w;
    } இல்லையெனில் {
        new_h = h;
        new_w = (im.w * h)/im.h;
    }
    image resized = resize_image(im, new_w, new_h);
    embed_image(resized, boxed, (w-new_w)/2, (h-new_h)/2); 
    free_image(resized);
}

image letterbox_image(image im, முழு w, முழு h)
{
    முழு new_w = im.w;
    முழு new_h = im.h;
    எனில் (((தசம)w/im.w) < ((தசம)h/im.h)) {
        new_w = w;
        new_h = (im.h * w)/im.w;
    } இல்லையெனில் {
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
    திரும்பு boxed;
}

image resize_max(image im, முழு max)
{
    முழு w = im.w;
    முழு h = im.h;
    எனில்(w > h){
        h = (h * max) / w;
        w = max;
    } இல்லையெனில் {
        w = (w * max) / h;
        h = max;
    }
    எனில்(w == im.w && h == im.h) திரும்பு im;
    image resized = resize_image(im, w, h);
    திரும்பு resized;
}

image resize_min(image im, முழு min)
{
    முழு w = im.w;
    முழு h = im.h;
    எனில்(w < h){
        h = (h * min) / w;
        w = min;
    } இல்லையெனில் {
        w = (w * min) / h;
        h = min;
    }
    எனில்(w == im.w && h == im.h) திரும்பு im;
    image resized = resize_image(im, w, h);
    திரும்பு resized;
}

image random_crop_image(image im, முழு w, முழு h)
{
    முழு dx = rand_int(0, im.w - w);
    முழு dy = rand_int(0, im.h - h);
    image crop = crop_image(im, dx, dy, w, h);
    திரும்பு crop;
}

augment_args random_augment_args(image im, தசம angle, தசம aspect, முழு கீழ், முழு உயர்வு, முழு w, முழு h)
{
    augment_args a = {0};
    aspect = rand_scale(aspect);
    முழு r = rand_int(கீழ், உயர்வு);
    முழு min = (im.h < im.w*aspect) ? im.h : im.w*aspect;
    தசம scale = (தசம)r / min;

    தசம rad = rand_uniform(-angle, angle) * TWO_PI / 360.;

    தசம dx = (im.w*scale/aspect - w) / 2.;
    தசம dy = (im.h*scale - w) / 2.;
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
    திரும்பு a;
}

image random_augment_image(image im, தசம angle, தசம aspect, முழு கீழ், முழு உயர்வு, முழு w, முழு h)
{
    augment_args a = random_augment_args(im, angle, aspect, கீழ், உயர்வு, w, h);
    image crop = rotate_crop_image(im, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);
    திரும்பு crop;
}

தசம three_way_max(தசம a, தசம b, தசம c)
{
    திரும்பு (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

தசம three_way_min(தசம a, தசம b, தசம c)
{
    திரும்பு (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

வெறுமை yuv_to_rgb(image im)
{
    ப்ரதிஜ்ஞா(im.c == 3);
    முழு i, j;
    தசம r, g, b;
    தசம y, u, v;
    க்கு(j = 0; j < im.h; ++j){
        க்கு(i = 0; i < im.w; ++i){
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

வெறுமை rgb_to_yuv(image im)
{
    ப்ரதிஜ்ஞா(im.c == 3);
    முழு i, j;
    தசம r, g, b;
    தசம y, u, v;
    க்கு(j = 0; j < im.h; ++j){
        க்கு(i = 0; i < im.w; ++i){
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
வெறுமை rgb_to_hsv(image im)
{
    ப்ரதிஜ்ஞா(im.c == 3);
    முழு i, j;
    தசம r, g, b;
    தசம h, s, v;
    க்கு(j = 0; j < im.h; ++j){
        க்கு(i = 0; i < im.w; ++i){
            r = get_pixel(im, i , j, 0);
            g = get_pixel(im, i , j, 1);
            b = get_pixel(im, i , j, 2);
            தசம max = three_way_max(r,g,b);
            தசம min = three_way_min(r,g,b);
            தசம delta = max - min;
            v = max;
            எனில்(max == 0){
                s = 0;
                h = 0;
            }இல்லையெனில்{
                s = delta/max;
                எனில்(r == max){
                    h = (g - b) / delta;
                } இல்லையெனில் எனில் (g == max) {
                    h = 2 + (b - r) / delta;
                } இல்லையெனில் {
                    h = 4 + (r - g) / delta;
                }
                எனில் (h < 0) h += 6;
                h = h/6.;
            }
            set_pixel(im, i, j, 0, h);
            set_pixel(im, i, j, 1, s);
            set_pixel(im, i, j, 2, v);
        }
    }
}

வெறுமை hsv_to_rgb(image im)
{
    ப்ரதிஜ்ஞா(im.c == 3);
    முழு i, j;
    தசம r, g, b;
    தசம h, s, v;
    தசம f, p, q, t;
    க்கு(j = 0; j < im.h; ++j){
        க்கு(i = 0; i < im.w; ++i){
            h = 6 * get_pixel(im, i , j, 0);
            s = get_pixel(im, i , j, 1);
            v = get_pixel(im, i , j, 2);
            எனில் (s == 0) {
                r = g = b = v;
            } இல்லையெனில் {
                முழு index = floor(h);
                f = h - index;
                p = v*(1-s);
                q = v*(1-s*f);
                t = v*(1-s*(1-f));
                எனில்(index == 0){
                    r = v; g = t; b = p;
                } இல்லையெனில் எனில்(index == 1){
                    r = q; g = v; b = p;
                } இல்லையெனில் எனில்(index == 2){
                    r = p; g = v; b = t;
                } இல்லையெனில் எனில்(index == 3){
                    r = p; g = q; b = v;
                } இல்லையெனில் எனில்(index == 4){
                    r = t; g = p; b = v;
                } இல்லையெனில் {
                    r = v; g = p; b = q;
                }
            }
            set_pixel(im, i, j, 0, r);
            set_pixel(im, i, j, 1, g);
            set_pixel(im, i, j, 2, b);
        }
    }
}

வெறுமை grayscale_image_3c(image im)
{
    ப்ரதிஜ்ஞா(im.c == 3);
    முழு i, j, k;
    தசம scale[] = {0.299, 0.587, 0.114};
    க்கு(j = 0; j < im.h; ++j){
        க்கு(i = 0; i < im.w; ++i){
            தசம val = 0;
            க்கு(k = 0; k < 3; ++k){
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
    ப்ரதிஜ்ஞா(im.c == 3);
    முழு i, j, k;
    image gray = make_image(im.w, im.h, 1);
    தசம scale[] = {0.299, 0.587, 0.114};
    க்கு(k = 0; k < im.c; ++k){
        க்கு(j = 0; j < im.h; ++j){
            க்கு(i = 0; i < im.w; ++i){
                gray.data[i+im.w*j] += scale[k]*get_pixel(im, i, j, k);
            }
        }
    }
    திரும்பு gray;
}

image threshold_image(image im, தசம thresh)
{
    முழு i;
    image t = make_image(im.w, im.h, im.c);
    க்கு(i = 0; i < im.w*im.h*im.c; ++i){
        t.data[i] = im.data[i]>thresh ? 1 : 0;
    }
    திரும்பு t;
}

image blend_image(image fore, image back, தசம alpha)
{
    ப்ரதிஜ்ஞா(fore.w == back.w && fore.h == back.h && fore.c == back.c);
    image blend = make_image(fore.w, fore.h, fore.c);
    முழு i, j, k;
    க்கு(k = 0; k < fore.c; ++k){
        க்கு(j = 0; j < fore.h; ++j){
            க்கு(i = 0; i < fore.w; ++i){
                தசம val = alpha * get_pixel(fore, i, j, k) + 
                    (1 - alpha)* get_pixel(back, i, j, k);
                set_pixel(blend, i, j, k, val);
            }
        }
    }
    திரும்பு blend;
}

வெறுமை scale_image_channel(image im, முழு c, தசம v)
{
    முழு i, j;
    க்கு(j = 0; j < im.h; ++j){
        க்கு(i = 0; i < im.w; ++i){
            தசம pix = get_pixel(im, i, j, c);
            pix = pix*v;
            set_pixel(im, i, j, c, pix);
        }
    }
}

வெறுமை translate_image_channel(image im, முழு c, தசம v)
{
    முழு i, j;
    க்கு(j = 0; j < im.h; ++j){
        க்கு(i = 0; i < im.w; ++i){
            தசம pix = get_pixel(im, i, j, c);
            pix = pix+v;
            set_pixel(im, i, j, c, pix);
        }
    }
}

image binarize_image(image im)
{
    image c = copy_image(im);
    முழு i;
    க்கு(i = 0; i < im.w * im.h * im.c; ++i){
        எனில்(c.data[i] > .5) c.data[i] = 1;
        இல்லையெனில் c.data[i] = 0;
    }
    திரும்பு c;
}

வெறுமை saturate_image(image im, தசம sat)
{
    rgb_to_hsv(im);
    scale_image_channel(im, 1, sat);
    hsv_to_rgb(im);
    constrain_image(im);
}

வெறுமை hue_image(image im, தசம hue)
{
    rgb_to_hsv(im);
    முழு i;
    க்கு(i = 0; i < im.w*im.h; ++i){
        im.data[i] = im.data[i] + hue;
        எனில் (im.data[i] > 1) im.data[i] -= 1;
        எனில் (im.data[i] < 0) im.data[i] += 1;
    }
    hsv_to_rgb(im);
    constrain_image(im);
}

வெறுமை exposure_image(image im, தசம sat)
{
    rgb_to_hsv(im);
    scale_image_channel(im, 2, sat);
    hsv_to_rgb(im);
    constrain_image(im);
}

வெறுமை distort_image(image im, தசம hue, தசம sat, தசம val)
{
    rgb_to_hsv(im);
    scale_image_channel(im, 1, sat);
    scale_image_channel(im, 2, val);
    முழு i;
    க்கு(i = 0; i < im.w*im.h; ++i){
        im.data[i] = im.data[i] + hue;
        எனில் (im.data[i] > 1) im.data[i] -= 1;
        எனில் (im.data[i] < 0) im.data[i] += 1;
    }
    hsv_to_rgb(im);
    constrain_image(im);
}

வெறுமை random_distort_image(image im, தசம hue, தசம saturation, தசம exposure)
{
    தசம dhue = rand_uniform(-hue, hue);
    தசம dsat = rand_scale(saturation);
    தசம dexp = rand_scale(exposure);
    distort_image(im, dhue, dsat, dexp);
}

வெறுமை saturate_exposure_image(image im, தசம sat, தசம exposure)
{
    rgb_to_hsv(im);
    scale_image_channel(im, 1, sat);
    scale_image_channel(im, 2, exposure);
    hsv_to_rgb(im);
    constrain_image(im);
}

image resize_image(image im, முழு w, முழு h)
{
    image resized = make_image(w, h, im.c);   
    image part = make_image(w, im.h, im.c);
    முழு r, c, k;
    தசம w_scale = (தசம)(im.w - 1) / (w - 1);
    தசம h_scale = (தசம)(im.h - 1) / (h - 1);
    க்கு(k = 0; k < im.c; ++k){
        க்கு(r = 0; r < im.h; ++r){
            க்கு(c = 0; c < w; ++c){
                தசம val = 0;
                எனில்(c == w-1 || im.w == 1){
                    val = get_pixel(im, im.w-1, r, k);
                } இல்லையெனில் {
                    தசம sx = c*w_scale;
                    முழு ix = (முழு) sx;
                    தசம dx = sx - ix;
                    val = (1 - dx) * get_pixel(im, ix, r, k) + dx * get_pixel(im, ix+1, r, k);
                }
                set_pixel(part, c, r, k, val);
            }
        }
    }
    க்கு(k = 0; k < im.c; ++k){
        க்கு(r = 0; r < h; ++r){
            தசம sy = r*h_scale;
            முழு iy = (முழு) sy;
            தசம dy = sy - iy;
            க்கு(c = 0; c < w; ++c){
                தசம val = (1-dy) * get_pixel(part, c, iy, k);
                set_pixel(resized, c, r, k, val);
            }
            எனில்(r == h-1 || im.h == 1) தொடர்;
            க்கு(c = 0; c < w; ++c){
                தசம val = dy * get_pixel(part, c, iy+1, k);
                add_pixel(resized, c, r, k, val);
            }
        }
    }

    free_image(part);
    திரும்பு resized;
}


வெறுமை test_resize(எழுத்து *filename)
{
    image im = load_image(filename, 0,0, 3);
    தசம mag = mag_array(im.data, im.w*im.h*im.c);
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
    போது(1){
        image aug = random_augment_image(im, 0, .75, 320, 448, 320, 320);
        show_image(aug, "aug", 1);
        free_image(aug);


        தசம exposure = 1.15;
        தசம saturation = 1.15;
        தசம hue = .05;

        image c = copy_image(im);

        தசம dexp = rand_scale(exposure);
        தசம dsat = rand_scale(saturation);
        தசம dhue = rand_uniform(-hue, hue);

        distort_image(c, dhue, dsat, dexp);
        show_image(c, "rand", 1);
        printf("%f %f %f\n", dhue, dsat, dexp);
        free_image(c);
    }
#endif
}


image load_image_stb(எழுத்து *filename, முழு channels)
{
    முழு w, h, c;
    unsigned எழுத்து *data = stbi_load(filename, &w, &h, &c, channels);
    எனில் (!data) {
        fprintf(stderr, "Cannot load image \"%s\"\nSTB Reason: %s\n", filename, stbi_failure_reason());
        exit(0);
    }
    எனில்(channels) c = channels;
    முழு i,j,k;
    image im = make_image(w, h, c);
    க்கு(k = 0; k < c; ++k){
        க்கு(j = 0; j < h; ++j){
            க்கு(i = 0; i < w; ++i){
                முழு dst_index = i + w*j + w*h*k;
                முழு src_index = k + c*i + c*w*j;
                im.data[dst_index] = (தசம)data[src_index]/255.;
            }
        }
    }
    விடுவி(data);
    திரும்பு im;
}

image load_image(எழுத்து *filename, முழு w, முழு h, முழு c)
{
#ifdef OPENCV
    image out = load_image_cv(filename, c);
#இல்லையெனில்
    image out = load_image_stb(filename, c);
#endif

    எனில்((h && w) && (h != out.h || w != out.w)){
        image resized = resize_image(out, w, h);
        free_image(out);
        out = resized;
    }
    திரும்பு out;
}

image load_image_color(எழுத்து *filename, முழு w, முழு h)
{
    திரும்பு load_image(filename, w, h, 3);
}

image get_image_layer(image m, முழு l)
{
    image out = make_image(m.w, m.h, 1);
    முழு i;
    க்கு(i = 0; i < m.h*m.w; ++i){
        out.data[i] = m.data[i+l*m.h*m.w];
    }
    திரும்பு out;
}
வெறுமை print_image(image m)
{
    முழு i, j, k;
    க்கு(i =0 ; i < m.c; ++i){
        க்கு(j =0 ; j < m.h; ++j){
            க்கு(k = 0; k < m.w; ++k){
                printf("%.2lf, ", m.data[i*m.h*m.w + j*m.w + k]);
                எனில்(k > 30) நிறுத்து;
            }
            printf("\n");
            எனில்(j > 30) நிறுத்து;
        }
        printf("\n");
    }
    printf("\n");
}

image collapse_images_vert(image *ims, முழு n)
{
    முழு color = 1;
    முழு border = 1;
    முழு h,w,c;
    w = ims[0].w;
    h = (ims[0].h + border) * n - border;
    c = ims[0].c;
    எனில்(c != 3 || !color){
        w = (w+border)*c - border;
        c = 1;
    }

    image filters = make_image(w, h, c);
    முழு i,j;
    க்கு(i = 0; i < n; ++i){
        முழு h_offset = i*(ims[0].h+border);
        image copy = copy_image(ims[i]);
        //normalize_image(copy);
        எனில்(c == 3 && color){
            embed_image(copy, filters, 0, h_offset);
        }
        இல்லையெனில்{
            க்கு(j = 0; j < copy.c; ++j){
                முழு w_offset = j*(ims[0].w+border);
                image layer = get_image_layer(copy, j);
                embed_image(layer, filters, w_offset, h_offset);
                free_image(layer);
            }
        }
        free_image(copy);
    }
    திரும்பு filters;
} 

image collapse_images_horz(image *ims, முழு n)
{
    முழு color = 1;
    முழு border = 1;
    முழு h,w,c;
    முழு size = ims[0].h;
    h = size;
    w = (ims[0].w + border) * n - border;
    c = ims[0].c;
    எனில்(c != 3 || !color){
        h = (h+border)*c - border;
        c = 1;
    }

    image filters = make_image(w, h, c);
    முழு i,j;
    க்கு(i = 0; i < n; ++i){
        முழு w_offset = i*(size+border);
        image copy = copy_image(ims[i]);
        //normalize_image(copy);
        எனில்(c == 3 && color){
            embed_image(copy, filters, w_offset, 0);
        }
        இல்லையெனில்{
            க்கு(j = 0; j < copy.c; ++j){
                முழு h_offset = j*(size+border);
                image layer = get_image_layer(copy, j);
                embed_image(layer, filters, w_offset, h_offset);
                free_image(layer);
            }
        }
        free_image(copy);
    }
    திரும்பு filters;
} 

வெறுமை show_image_normalized(image im, மாறிலி எழுத்து *name)
{
    image c = copy_image(im);
    normalize_image(c);
    show_image(c, name, 1);
    free_image(c);
}

வெறுமை show_images(image *ims, முழு n, எழுத்து *window)
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

வெறுமை free_image(image m)
{
    எனில்(m.data){
        விடுவி(m.data);
    }
}
