#અન્તર્ભાવ "image.h"
#અન્તર્ભાવ "utils.h"
#અન્તર્ભાવ "blas.h"
#અન્તર્ભાવ "cuda.h"
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <math.h>

#define STB_IMAGE_IMPLEMENTATION
#અન્તર્ભાવ "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#અન્તર્ભાવ "stb_image_write.h"

પૂર્ણાંક windows = 0;

દશાંશ colors[6][3] = { {1,0,1}, {0,0,1},{0,1,1},{0,1,0},{1,1,0},{1,0,0} };

દશાંશ get_color(પૂર્ણાંક c, પૂર્ણાંક x, પૂર્ણાંક max)
{
    દશાંશ ratio = ((દશાંશ)x/max)*5;
    પૂર્ણાંક i = floor(ratio);
    પૂર્ણાંક j = ceil(ratio);
    ratio -= i;
    દશાંશ r = (1-ratio) * colors[i][c] + ratio*colors[j][c];
    //printf("%f\n", r);
    પાછા r;
}

image mask_to_rgb(image mask)
{
    પૂર્ણાંક n = mask.c;
    image im = make_image(mask.w, mask.h, 3);
    પૂર્ણાંક i, j;
    માટે(j = 0; j < n; ++j){
        પૂર્ણાંક offset = j*123457 % n;
        દશાંશ red = get_color(2,offset,n);
        દશાંશ green = get_color(1,offset,n);
        દશાંશ blue = get_color(0,offset,n);
        માટે(i = 0; i < im.w*im.h; ++i){
            im.data[i + 0*im.w*im.h] += mask.data[j*im.h*im.w + i]*red;
            im.data[i + 1*im.w*im.h] += mask.data[j*im.h*im.w + i]*green;
            im.data[i + 2*im.w*im.h] += mask.data[j*im.h*im.w + i]*blue;
        }
    }
    પાછા im;
}

સ્થિર દશાંશ get_pixel(image m, પૂર્ણાંક x, પૂર્ણાંક y, પૂર્ણાંક c)
{
    પ્રતિજ્ઞા(x < m.w && y < m.h && c < m.c);
    પાછા m.data[c*m.h*m.w + y*m.w + x];
}
સ્થિર દશાંશ get_pixel_extend(image m, પૂર્ણાંક x, પૂર્ણાંક y, પૂર્ણાંક c)
{
    જો(x < 0 || x >= m.w || y < 0 || y >= m.h) પાછા 0;
    /*
    if(x < 0) x = 0;
    if(x >= m.w) x = m.w-1;
    if(y < 0) y = 0;
    if(y >= m.h) y = m.h-1;
    */
    જો(c < 0 || c >= m.c) પાછા 0;
    પાછા get_pixel(m, x, y, c);
}
સ્થિર શૂન્ય set_pixel(image m, પૂર્ણાંક x, પૂર્ણાંક y, પૂર્ણાંક c, દશાંશ val)
{
    જો (x < 0 || y < 0 || c < 0 || x >= m.w || y >= m.h || c >= m.c) પાછા;
    પ્રતિજ્ઞા(x < m.w && y < m.h && c < m.c);
    m.data[c*m.h*m.w + y*m.w + x] = val;
}
સ્થિર શૂન્ય add_pixel(image m, પૂર્ણાંક x, પૂર્ણાંક y, પૂર્ણાંક c, દશાંશ val)
{
    પ્રતિજ્ઞા(x < m.w && y < m.h && c < m.c);
    m.data[c*m.h*m.w + y*m.w + x] += val;
}

સ્થિર દશાંશ bilinear_interpolate(image im, દશાંશ x, દશાંશ y, પૂર્ણાંક c)
{
    પૂર્ણાંક ix = (પૂર્ણાંક) floorf(x);
    પૂર્ણાંક iy = (પૂર્ણાંક) floorf(y);

    દશાંશ dx = x - ix;
    દશાંશ dy = y - iy;

    દશાંશ val = (1-dy) * (1-dx) * get_pixel_extend(im, ix, iy, c) + 
        dy     * (1-dx) * get_pixel_extend(im, ix, iy+1, c) + 
        (1-dy) *   dx   * get_pixel_extend(im, ix+1, iy, c) +
        dy     *   dx   * get_pixel_extend(im, ix+1, iy+1, c);
    પાછા val;
}


શૂન્ય composite_image(image source, image dest, પૂર્ણાંક dx, પૂર્ણાંક dy)
{
    પૂર્ણાંક x,y,k;
    માટે(k = 0; k < source.c; ++k){
        માટે(y = 0; y < source.h; ++y){
            માટે(x = 0; x < source.w; ++x){
                દશાંશ val = get_pixel(source, x, y, k);
                દશાંશ val2 = get_pixel_extend(dest, dx+x, dy+y, k);
                set_pixel(dest, dx+x, dy+y, k, val * val2);
            }
        }
    }
}

image border_image(image a, પૂર્ણાંક border)
{
    image b = make_image(a.w + 2*border, a.h + 2*border, a.c);
    પૂર્ણાંક x,y,k;
    માટે(k = 0; k < b.c; ++k){
        માટે(y = 0; y < b.h; ++y){
            માટે(x = 0; x < b.w; ++x){
                દશાંશ val = get_pixel_extend(a, x - border, y - border, k);
                જો(x - border < 0 || x - border >= a.w || y - border < 0 || y - border >= a.h) val = 1;
                set_pixel(b, x, y, k, val);
            }
        }
    }
    પાછા b;
}

image tile_images(image a, image b, પૂર્ણાંક dx)
{
    જો(a.w == 0) પાછા copy_image(b);
    image c = make_image(a.w + b.w + dx, (a.h > b.h) ? a.h : b.h, (a.c > b.c) ? a.c : b.c);
    fill_cpu(c.w*c.h*c.c, 1, c.data, 1);
    embed_image(a, c, 0, 0); 
    composite_image(b, c, a.w + dx, 0);
    પાછા c;
}

image get_label(image **characters, અક્ષર *string, પૂર્ણાંક size)
{
    size = size/10;
    જો(size > 7) size = 7;
    image label = make_empty_image(0,0,0);
    જ્યારે(*string){
        image l = characters[size][(પૂર્ણાંક)*string];
        image n = tile_images(label, l, -size - 1 + (size+1)/2);
        free_image(label);
        label = n;
        ++string;
    }
    image b = border_image(label, label.h*.25);
    free_image(label);
    પાછા b;
}

શૂન્ય draw_label(image a, પૂર્ણાંક r, પૂર્ણાંક c, image label, અચલ દશાંશ *rgb)
{
    પૂર્ણાંક w = label.w;
    પૂર્ણાંક h = label.h;
    જો (r - h >= 0) r = r - h;

    પૂર્ણાંક i, j, k;
    માટે(j = 0; j < h && j + r < a.h; ++j){
        માટે(i = 0; i < w && i + c < a.w; ++i){
            માટે(k = 0; k < label.c; ++k){
                દશાંશ val = get_pixel(label, i, j, k);
                set_pixel(a, i+c, j+r, k, rgb[k] * val);
            }
        }
    }
}

શૂન્ય draw_box(image a, પૂર્ણાંક x1, પૂર્ણાંક y1, પૂર્ણાંક x2, પૂર્ણાંક y2, દશાંશ r, દશાંશ g, દશાંશ b)
{
    //normalize_image(a);
    પૂર્ણાંક i;
    જો(x1 < 0) x1 = 0;
    જો(x1 >= a.w) x1 = a.w-1;
    જો(x2 < 0) x2 = 0;
    જો(x2 >= a.w) x2 = a.w-1;

    જો(y1 < 0) y1 = 0;
    જો(y1 >= a.h) y1 = a.h-1;
    જો(y2 < 0) y2 = 0;
    જો(y2 >= a.h) y2 = a.h-1;

    માટે(i = x1; i <= x2; ++i){
        a.data[i + y1*a.w + 0*a.w*a.h] = r;
        a.data[i + y2*a.w + 0*a.w*a.h] = r;

        a.data[i + y1*a.w + 1*a.w*a.h] = g;
        a.data[i + y2*a.w + 1*a.w*a.h] = g;

        a.data[i + y1*a.w + 2*a.w*a.h] = b;
        a.data[i + y2*a.w + 2*a.w*a.h] = b;
    }
    માટે(i = y1; i <= y2; ++i){
        a.data[x1 + i*a.w + 0*a.w*a.h] = r;
        a.data[x2 + i*a.w + 0*a.w*a.h] = r;

        a.data[x1 + i*a.w + 1*a.w*a.h] = g;
        a.data[x2 + i*a.w + 1*a.w*a.h] = g;

        a.data[x1 + i*a.w + 2*a.w*a.h] = b;
        a.data[x2 + i*a.w + 2*a.w*a.h] = b;
    }
}

શૂન્ય draw_box_width(image a, પૂર્ણાંક x1, પૂર્ણાંક y1, પૂર્ણાંક x2, પૂર્ણાંક y2, પૂર્ણાંક w, દશાંશ r, દશાંશ g, દશાંશ b)
{
    પૂર્ણાંક i;
    માટે(i = 0; i < w; ++i){
        draw_box(a, x1+i, y1+i, x2-i, y2-i, r, g, b);
    }
}

શૂન્ય draw_bbox(image a, box bbox, પૂર્ણાંક w, દશાંશ r, દશાંશ g, દશાંશ b)
{
    પૂર્ણાંક ડાબું  = (bbox.x-bbox.w/2)*a.w;
    પૂર્ણાંક જમણું = (bbox.x+bbox.w/2)*a.w;
    પૂર્ણાંક ટોચ   = (bbox.y-bbox.h/2)*a.h;
    પૂર્ણાંક bot   = (bbox.y+bbox.h/2)*a.h;

    પૂર્ણાંક i;
    માટે(i = 0; i < w; ++i){
        draw_box(a, ડાબું+i, ટોચ+i, જમણું-i, bot-i, r, g, b);
    }
}

image **load_alphabet()
{
    પૂર્ણાંક i, j;
    અચલ પૂર્ણાંક nsize = 8;
    image **alphabets = calloc(nsize, કદ(image));
    માટે(j = 0; j < nsize; ++j){
        alphabets[j] = calloc(128, કદ(image));
        માટે(i = 32; i < 127; ++i){
            અક્ષર buff[256];
            sprintf(buff, "data/labels/%d_%d.png", i, j);
            alphabets[j][i] = load_image_color(buff, 0, 0);
        }
    }
    પાછા alphabets;
}

શૂન્ય draw_detections(image im, detection *dets, પૂર્ણાંક અંક, દશાંશ thresh, અક્ષર **names, image **alphabet, પૂર્ણાંક classes)
{
    પૂર્ણાંક i,j;

    માટે(i = 0; i < અંક; ++i){
        અક્ષર labelstr[4096] = {0};
        પૂર્ણાંક class = -1;
        માટે(j = 0; j < classes; ++j){
            જો (dets[i].prob[j] > thresh){
                જો (class < 0) {
                    strcat(labelstr, names[j]);
                    class = j;
                } નહીંતર {
                    strcat(labelstr, ", ");
                    strcat(labelstr, names[j]);
                }
                printf("%s: %.0f%%\n", names[j], dets[i].prob[j]*100);
            }
        }
        જો(class >= 0){
            પૂર્ણાંક width = im.h * .006;

            /*
               if(0){
               width = pow(prob, 1./2.)*10+1;
               alphabet = 0;
               }
             */

            //printf("%d %s: %.0f%%\n", i, names[class], prob*100);
            પૂર્ણાંક offset = class*123457 % classes;
            દશાંશ red = get_color(2,offset,classes);
            દશાંશ green = get_color(1,offset,classes);
            દશાંશ blue = get_color(0,offset,classes);
            દશાંશ rgb[3];

            //width = prob*20+2;

            rgb[0] = red;
            rgb[1] = green;
            rgb[2] = blue;
            box b = dets[i].bbox;
            //printf("%f %f %f %f\n", b.x, b.y, b.w, b.h);

            પૂર્ણાંક ડાબું  = (b.x-b.w/2.)*im.w;
            પૂર્ણાંક જમણું = (b.x+b.w/2.)*im.w;
            પૂર્ણાંક ટોચ   = (b.y-b.h/2.)*im.h;
            પૂર્ણાંક bot   = (b.y+b.h/2.)*im.h;

            જો(ડાબું < 0) ડાબું = 0;
            જો(જમણું > im.w-1) જમણું = im.w-1;
            જો(ટોચ < 0) ટોચ = 0;
            જો(bot > im.h-1) bot = im.h-1;

            draw_box_width(im, ડાબું, ટોચ, જમણું, bot, width, red, green, blue);
            જો (alphabet) {
                image label = get_label(alphabet, labelstr, (im.h*.03));
                draw_label(im, ટોચ + width, ડાબું, label, rgb);
                free_image(label);
            }
            જો (dets[i].mask){
                image mask = float_to_image(14, 14, 1, dets[i].mask);
                image resized_mask = resize_image(mask, b.w*im.w, b.h*im.h);
                image tmask = threshold_image(resized_mask, .5);
                embed_image(tmask, im, ડાબું, ટોચ);
                free_image(mask);
                free_image(resized_mask);
                free_image(tmask);
            }
        }
    }
}

શૂન્ય transpose_image(image im)
{
    પ્રતિજ્ઞા(im.w == im.h);
    પૂર્ણાંક n, m;
    પૂર્ણાંક c;
    માટે(c = 0; c < im.c; ++c){
        માટે(n = 0; n < im.w-1; ++n){
            માટે(m = n + 1; m < im.w; ++m){
                દશાંશ swap = im.data[m + im.w*(n + im.h*c)];
                im.data[m + im.w*(n + im.h*c)] = im.data[n + im.w*(m + im.h*c)];
                im.data[n + im.w*(m + im.h*c)] = swap;
            }
        }
    }
}

શૂન્ય rotate_image_cw(image im, પૂર્ણાંક times)
{
    પ્રતિજ્ઞા(im.w == im.h);
    times = (times + 400) % 4;
    પૂર્ણાંક i, x, y, c;
    પૂર્ણાંક n = im.w;
    માટે(i = 0; i < times; ++i){
        માટે(c = 0; c < im.c; ++c){
            માટે(x = 0; x < n/2; ++x){
                માટે(y = 0; y < (n-1)/2 + 1; ++y){
                    દશાંશ temp = im.data[y + im.w*(x + im.h*c)];
                    im.data[y + im.w*(x + im.h*c)] = im.data[n-1-x + im.w*(y + im.h*c)];
                    im.data[n-1-x + im.w*(y + im.h*c)] = im.data[n-1-y + im.w*(n-1-x + im.h*c)];
                    im.data[n-1-y + im.w*(n-1-x + im.h*c)] = im.data[x + im.w*(n-1-y + im.h*c)];
                    im.data[x + im.w*(n-1-y + im.h*c)] = temp;
                }
            }
        }
    }
}

શૂન્ય flip_image(image a)
{
    પૂર્ણાંક i,j,k;
    માટે(k = 0; k < a.c; ++k){
        માટે(i = 0; i < a.h; ++i){
            માટે(j = 0; j < a.w/2; ++j){
                પૂર્ણાંક index = j + a.w*(i + a.h*(k));
                પૂર્ણાંક flip = (a.w - j - 1) + a.w*(i + a.h*(k));
                દશાંશ swap = a.data[flip];
                a.data[flip] = a.data[index];
                a.data[index] = swap;
            }
        }
    }
}

image image_distance(image a, image b)
{
    પૂર્ણાંક i,j;
    image dist = make_image(a.w, a.h, 1);
    માટે(i = 0; i < a.c; ++i){
        માટે(j = 0; j < a.h*a.w; ++j){
            dist.data[j] += pow(a.data[i*a.h*a.w+j]-b.data[i*a.h*a.w+j],2);
        }
    }
    માટે(j = 0; j < a.h*a.w; ++j){
        dist.data[j] = sqrt(dist.data[j]);
    }
    પાછા dist;
}

શૂન્ય ghost_image(image source, image dest, પૂર્ણાંક dx, પૂર્ણાંક dy)
{
    પૂર્ણાંક x,y,k;
    દશાંશ max_dist = sqrt((-source.w/2. + .5)*(-source.w/2. + .5));
    માટે(k = 0; k < source.c; ++k){
        માટે(y = 0; y < source.h; ++y){
            માટે(x = 0; x < source.w; ++x){
                દશાંશ dist = sqrt((x - source.w/2. + .5)*(x - source.w/2. + .5) + (y - source.h/2. + .5)*(y - source.h/2. + .5));
                દશાંશ alpha = (1 - dist/max_dist);
                જો(alpha < 0) alpha = 0;
                દશાંશ v1 = get_pixel(source, x,y,k);
                દશાંશ v2 = get_pixel(dest, dx+x,dy+y,k);
                દશાંશ val = alpha*v1 + (1-alpha)*v2;
                set_pixel(dest, dx+x, dy+y, k, val);
            }
        }
    }
}

શૂન્ય blocky_image(image im, પૂર્ણાંક s)
{
    પૂર્ણાંક i,j,k;
    માટે(k = 0; k < im.c; ++k){
        માટે(j = 0; j < im.h; ++j){
            માટે(i = 0; i < im.w; ++i){
                im.data[i + im.w*(j + im.h*k)] = im.data[i/s*s + im.w*(j/s*s + im.h*k)];
            }
        }
    }
}

શૂન્ય censor_image(image im, પૂર્ણાંક dx, પૂર્ણાંક dy, પૂર્ણાંક w, પૂર્ણાંક h)
{
    પૂર્ણાંક i,j,k;
    પૂર્ણાંક s = 32;
    જો(dx < 0) dx = 0;
    જો(dy < 0) dy = 0;

    માટે(k = 0; k < im.c; ++k){
        માટે(j = dy; j < dy + h && j < im.h; ++j){
            માટે(i = dx; i < dx + w && i < im.w; ++i){
                im.data[i + im.w*(j + im.h*k)] = im.data[i/s*s + im.w*(j/s*s + im.h*k)];
                //im.data[i + j*im.w + k*im.w*im.h] = 0;
            }
        }
    }
}

શૂન્ય embed_image(image source, image dest, પૂર્ણાંક dx, પૂર્ણાંક dy)
{
    પૂર્ણાંક x,y,k;
    માટે(k = 0; k < source.c; ++k){
        માટે(y = 0; y < source.h; ++y){
            માટે(x = 0; x < source.w; ++x){
                દશાંશ val = get_pixel(source, x,y,k);
                set_pixel(dest, dx+x, dy+y, k, val);
            }
        }
    }
}

image collapse_image_layers(image source, પૂર્ણાંક border)
{
    પૂર્ણાંક h = source.h;
    h = (h+border)*source.c - border;
    image dest = make_image(source.w, h, 1);
    પૂર્ણાંક i;
    માટે(i = 0; i < source.c; ++i){
        image layer = get_image_layer(source, i);
        પૂર્ણાંક h_offset = i*(source.h+border);
        embed_image(layer, dest, 0, h_offset);
        free_image(layer);
    }
    પાછા dest;
}

શૂન્ય constrain_image(image im)
{
    પૂર્ણાંક i;
    માટે(i = 0; i < im.w*im.h*im.c; ++i){
        જો(im.data[i] < 0) im.data[i] = 0;
        જો(im.data[i] > 1) im.data[i] = 1;
    }
}

શૂન્ય normalize_image(image p)
{
    પૂર્ણાંક i;
    દશાંશ min = 9999999;
    દશાંશ max = -999999;

    માટે(i = 0; i < p.h*p.w*p.c; ++i){
        દશાંશ v = p.data[i];
        જો(v < min) min = v;
        જો(v > max) max = v;
    }
    જો(max - min < .000000001){
        min = 0;
        max = 1;
    }
    માટે(i = 0; i < p.c*p.w*p.h; ++i){
        p.data[i] = (p.data[i] - min)/(max-min);
    }
}

શૂન્ય normalize_image2(image p)
{
    દશાંશ *min = calloc(p.c, કદ(દશાંશ));
    દશાંશ *max = calloc(p.c, કદ(દશાંશ));
    પૂર્ણાંક i,j;
    માટે(i = 0; i < p.c; ++i) min[i] = max[i] = p.data[i*p.h*p.w];

    માટે(j = 0; j < p.c; ++j){
        માટે(i = 0; i < p.h*p.w; ++i){
            દશાંશ v = p.data[i+j*p.h*p.w];
            જો(v < min[j]) min[j] = v;
            જો(v > max[j]) max[j] = v;
        }
    }
    માટે(i = 0; i < p.c; ++i){
        જો(max[i] - min[i] < .000000001){
            min[i] = 0;
            max[i] = 1;
        }
    }
    માટે(j = 0; j < p.c; ++j){
        માટે(i = 0; i < p.w*p.h; ++i){
            p.data[i+j*p.h*p.w] = (p.data[i+j*p.h*p.w] - min[j])/(max[j]-min[j]);
        }
    }
    મુક્ત(min);
    મુક્ત(max);
}

શૂન્ય copy_image_into(image src, image dest)
{
    memcpy(dest.data, src.data, src.h*src.w*src.c*કદ(દશાંશ));
}

image copy_image(image p)
{
    image copy = p;
    copy.data = calloc(p.h*p.w*p.c, કદ(દશાંશ));
    memcpy(copy.data, p.data, p.h*p.w*p.c*કદ(દશાંશ));
    પાછા copy;
}

શૂન્ય rgbgr_image(image im)
{
    પૂર્ણાંક i;
    માટે(i = 0; i < im.w*im.h; ++i){
        દશાંશ swap = im.data[i];
        im.data[i] = im.data[i+im.w*im.h*2];
        im.data[i+im.w*im.h*2] = swap;
    }
}

પૂર્ણાંક show_image(image p, અચલ અક્ષર *name, પૂર્ણાંક ms)
{
#ifdef OPENCV
    પૂર્ણાંક c = show_image_cv(p, name, ms);
    પાછા c;
#નહીંતર
    fprintf(stderr, "Not compiled with OpenCV, saving to %s.png instead\n", name);
    save_image(p, name);
    પાછા -1;
#endif
}

શૂન્ય save_image_options(image im, અચલ અક્ષર *name, IMTYPE f, પૂર્ણાંક quality)
{
    અક્ષર buff[256];
    //sprintf(buff, "%s (%d)", name, windows);
    જો(f == PNG)       sprintf(buff, "%s.png", name);
    નહીંતર જો (f == BMP) sprintf(buff, "%s.bmp", name);
    નહીંતર જો (f == TGA) sprintf(buff, "%s.tga", name);
    નહીંતર જો (f == JPG) sprintf(buff, "%s.jpg", name);
    નહીંતર               sprintf(buff, "%s.png", name);
    unsigned અક્ષર *data = calloc(im.w*im.h*im.c, કદ(અક્ષર));
    પૂર્ણાંક i,k;
    માટે(k = 0; k < im.c; ++k){
        માટે(i = 0; i < im.w*im.h; ++i){
            data[i*im.c+k] = (unsigned અક્ષર) (255*im.data[i + k*im.w*im.h]);
        }
    }
    પૂર્ણાંક success = 0;
    જો(f == PNG)       success = stbi_write_png(buff, im.w, im.h, im.c, data, im.w*im.c);
    નહીંતર જો (f == BMP) success = stbi_write_bmp(buff, im.w, im.h, im.c, data);
    નહીંતર જો (f == TGA) success = stbi_write_tga(buff, im.w, im.h, im.c, data);
    નહીંતર જો (f == JPG) success = stbi_write_jpg(buff, im.w, im.h, im.c, data, quality);
    મુક્ત(data);
    જો(!success) fprintf(stderr, "Failed to write image %s\n", buff);
}

શૂન્ય save_image(image im, અચલ અક્ષર *name)
{
    save_image_options(im, name, JPG, 80);
}

શૂન્ય show_image_layers(image p, અક્ષર *name)
{
    પૂર્ણાંક i;
    અક્ષર buff[256];
    માટે(i = 0; i < p.c; ++i){
        sprintf(buff, "%s - Layer %d", name, i);
        image layer = get_image_layer(p, i);
        show_image(layer, buff, 1);
        free_image(layer);
    }
}

શૂન્ય show_image_collapsed(image p, અક્ષર *name)
{
    image c = collapse_image_layers(p, 1);
    show_image(c, name, 1);
    free_image(c);
}

image make_empty_image(પૂર્ણાંક w, પૂર્ણાંક h, પૂર્ણાંક c)
{
    image out;
    out.data = 0;
    out.h = h;
    out.w = w;
    out.c = c;
    પાછા out;
}

image make_image(પૂર્ણાંક w, પૂર્ણાંક h, પૂર્ણાંક c)
{
    image out = make_empty_image(w,h,c);
    out.data = calloc(h*w*c, કદ(દશાંશ));
    પાછા out;
}

image make_random_image(પૂર્ણાંક w, પૂર્ણાંક h, પૂર્ણાંક c)
{
    image out = make_empty_image(w,h,c);
    out.data = calloc(h*w*c, કદ(દશાંશ));
    પૂર્ણાંક i;
    માટે(i = 0; i < w*h*c; ++i){
        out.data[i] = (rand_normal() * .25) + .5;
    }
    પાછા out;
}

image float_to_image(પૂર્ણાંક w, પૂર્ણાંક h, પૂર્ણાંક c, દશાંશ *data)
{
    image out = make_empty_image(w,h,c);
    out.data = data;
    પાછા out;
}

શૂન્ય place_image(image im, પૂર્ણાંક w, પૂર્ણાંક h, પૂર્ણાંક dx, પૂર્ણાંક dy, image canvas)
{
    પૂર્ણાંક x, y, c;
    માટે(c = 0; c < im.c; ++c){
        માટે(y = 0; y < h; ++y){
            માટે(x = 0; x < w; ++x){
                દશાંશ rx = ((દશાંશ)x / w) * im.w;
                દશાંશ ry = ((દશાંશ)y / h) * im.h;
                દશાંશ val = bilinear_interpolate(im, rx, ry, c);
                set_pixel(canvas, x + dx, y + dy, c, val);
            }
        }
    }
}

image center_crop_image(image im, પૂર્ણાંક w, પૂર્ણાંક h)
{
    પૂર્ણાંક m = (im.w < im.h) ? im.w : im.h;   
    image c = crop_image(im, (im.w - m) / 2, (im.h - m)/2, m, m);
    image r = resize_image(c, w, h);
    free_image(c);
    પાછા r;
}

image rotate_crop_image(image im, દશાંશ rad, દશાંશ s, પૂર્ણાંક w, પૂર્ણાંક h, દશાંશ dx, દશાંશ dy, દશાંશ aspect)
{
    પૂર્ણાંક x, y, c;
    દશાંશ cx = im.w/2.;
    દશાંશ cy = im.h/2.;
    image rot = make_image(w, h, im.c);
    માટે(c = 0; c < im.c; ++c){
        માટે(y = 0; y < h; ++y){
            માટે(x = 0; x < w; ++x){
                દશાંશ rx = cos(rad)*((x - w/2.)/s*aspect + dx/s*aspect) - sin(rad)*((y - h/2.)/s + dy/s) + cx;
                દશાંશ ry = sin(rad)*((x - w/2.)/s*aspect + dx/s*aspect) + cos(rad)*((y - h/2.)/s + dy/s) + cy;
                દશાંશ val = bilinear_interpolate(im, rx, ry, c);
                set_pixel(rot, x, y, c, val);
            }
        }
    }
    પાછા rot;
}

image rotate_image(image im, દશાંશ rad)
{
    પૂર્ણાંક x, y, c;
    દશાંશ cx = im.w/2.;
    દશાંશ cy = im.h/2.;
    image rot = make_image(im.w, im.h, im.c);
    માટે(c = 0; c < im.c; ++c){
        માટે(y = 0; y < im.h; ++y){
            માટે(x = 0; x < im.w; ++x){
                દશાંશ rx = cos(rad)*(x-cx) - sin(rad)*(y-cy) + cx;
                દશાંશ ry = sin(rad)*(x-cx) + cos(rad)*(y-cy) + cy;
                દશાંશ val = bilinear_interpolate(im, rx, ry, c);
                set_pixel(rot, x, y, c, val);
            }
        }
    }
    પાછા rot;
}

શૂન્ય fill_image(image m, દશાંશ s)
{
    પૂર્ણાંક i;
    માટે(i = 0; i < m.h*m.w*m.c; ++i) m.data[i] = s;
}

શૂન્ય translate_image(image m, દશાંશ s)
{
    પૂર્ણાંક i;
    માટે(i = 0; i < m.h*m.w*m.c; ++i) m.data[i] += s;
}

શૂન્ય scale_image(image m, દશાંશ s)
{
    પૂર્ણાંક i;
    માટે(i = 0; i < m.h*m.w*m.c; ++i) m.data[i] *= s;
}

image crop_image(image im, પૂર્ણાંક dx, પૂર્ણાંક dy, પૂર્ણાંક w, પૂર્ણાંક h)
{
    image cropped = make_image(w, h, im.c);
    પૂર્ણાંક i, j, k;
    માટે(k = 0; k < im.c; ++k){
        માટે(j = 0; j < h; ++j){
            માટે(i = 0; i < w; ++i){
                પૂર્ણાંક r = j + dy;
                પૂર્ણાંક c = i + dx;
                દશાંશ val = 0;
                r = constrain_int(r, 0, im.h-1);
                c = constrain_int(c, 0, im.w-1);
                val = get_pixel(im, c, r, k);
                set_pixel(cropped, i, j, k, val);
            }
        }
    }
    પાછા cropped;
}

પૂર્ણાંક best_3d_shift_r(image a, image b, પૂર્ણાંક min, પૂર્ણાંક max)
{
    જો(min == max) પાછા min;
    પૂર્ણાંક mid = floor((min + max) / 2.);
    image c1 = crop_image(b, 0, mid, b.w, b.h);
    image c2 = crop_image(b, 0, mid+1, b.w, b.h);
    દશાંશ d1 = dist_array(c1.data, a.data, a.w*a.h*a.c, 10);
    દશાંશ d2 = dist_array(c2.data, a.data, a.w*a.h*a.c, 10);
    free_image(c1);
    free_image(c2);
    જો(d1 < d2) પાછા best_3d_shift_r(a, b, min, mid);
    નહીંતર પાછા best_3d_shift_r(a, b, mid+1, max);
}

પૂર્ણાંક best_3d_shift(image a, image b, પૂર્ણાંક min, પૂર્ણાંક max)
{
    પૂર્ણાંક i;
    પૂર્ણાંક best = 0;
    દશાંશ best_distance = FLT_MAX;
    માટે(i = min; i <= max; i += 2){
        image c = crop_image(b, 0, i, b.w, b.h);
        દશાંશ d = dist_array(c.data, a.data, a.w*a.h*a.c, 100);
        જો(d < best_distance){
            best_distance = d;
            best = i;
        }
        printf("%d %f\n", i, d);
        free_image(c);
    }
    પાછા best;
}

શૂન્ય composite_3d(અક્ષર *f1, અક્ષર *f2, અક્ષર *out, પૂર્ણાંક delta)
{
    જો(!out) out = "out";
    image a = load_image(f1, 0,0,0);
    image b = load_image(f2, 0,0,0);
    પૂર્ણાંક shift = best_3d_shift_r(a, b, -a.h/100, a.h/100);

    image c1 = crop_image(b, 10, shift, b.w, b.h);
    દશાંશ d1 = dist_array(c1.data, a.data, a.w*a.h*a.c, 100);
    image c2 = crop_image(b, -10, shift, b.w, b.h);
    દશાંશ d2 = dist_array(c2.data, a.data, a.w*a.h*a.c, 100);

    જો(d2 < d1 && 0){
        image swap = a;
        a = b;
        b = swap;
        shift = -shift;
        printf("swapped, %d\n", shift);
    }
    નહીંતર{
        printf("%d\n", shift);
    }

    image c = crop_image(b, delta, shift, a.w, a.h);
    પૂર્ણાંક i;
    માટે(i = 0; i < c.w*c.h; ++i){
        c.data[i] = a.data[i];
    }
    save_image(c, out);
}

શૂન્ય letterbox_image_into(image im, પૂર્ણાંક w, પૂર્ણાંક h, image boxed)
{
    પૂર્ણાંક new_w = im.w;
    પૂર્ણાંક new_h = im.h;
    જો (((દશાંશ)w/im.w) < ((દશાંશ)h/im.h)) {
        new_w = w;
        new_h = (im.h * w)/im.w;
    } નહીંતર {
        new_h = h;
        new_w = (im.w * h)/im.h;
    }
    image resized = resize_image(im, new_w, new_h);
    embed_image(resized, boxed, (w-new_w)/2, (h-new_h)/2); 
    free_image(resized);
}

image letterbox_image(image im, પૂર્ણાંક w, પૂર્ણાંક h)
{
    પૂર્ણાંક new_w = im.w;
    પૂર્ણાંક new_h = im.h;
    જો (((દશાંશ)w/im.w) < ((દશાંશ)h/im.h)) {
        new_w = w;
        new_h = (im.h * w)/im.w;
    } નહીંતર {
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
    પાછા boxed;
}

image resize_max(image im, પૂર્ણાંક max)
{
    પૂર્ણાંક w = im.w;
    પૂર્ણાંક h = im.h;
    જો(w > h){
        h = (h * max) / w;
        w = max;
    } નહીંતર {
        w = (w * max) / h;
        h = max;
    }
    જો(w == im.w && h == im.h) પાછા im;
    image resized = resize_image(im, w, h);
    પાછા resized;
}

image resize_min(image im, પૂર્ણાંક min)
{
    પૂર્ણાંક w = im.w;
    પૂર્ણાંક h = im.h;
    જો(w < h){
        h = (h * min) / w;
        w = min;
    } નહીંતર {
        w = (w * min) / h;
        h = min;
    }
    જો(w == im.w && h == im.h) પાછા im;
    image resized = resize_image(im, w, h);
    પાછા resized;
}

image random_crop_image(image im, પૂર્ણાંક w, પૂર્ણાંક h)
{
    પૂર્ણાંક dx = rand_int(0, im.w - w);
    પૂર્ણાંક dy = rand_int(0, im.h - h);
    image crop = crop_image(im, dx, dy, w, h);
    પાછા crop;
}

augment_args random_augment_args(image im, દશાંશ angle, દશાંશ aspect, પૂર્ણાંક નીચલું, પૂર્ણાંક ઉપલું, પૂર્ણાંક w, પૂર્ણાંક h)
{
    augment_args a = {0};
    aspect = rand_scale(aspect);
    પૂર્ણાંક r = rand_int(નીચલું, ઉપલું);
    પૂર્ણાંક min = (im.h < im.w*aspect) ? im.h : im.w*aspect;
    દશાંશ scale = (દશાંશ)r / min;

    દશાંશ rad = rand_uniform(-angle, angle) * TWO_PI / 360.;

    દશાંશ dx = (im.w*scale/aspect - w) / 2.;
    દશાંશ dy = (im.h*scale - w) / 2.;
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
    પાછા a;
}

image random_augment_image(image im, દશાંશ angle, દશાંશ aspect, પૂર્ણાંક નીચલું, પૂર્ણાંક ઉપલું, પૂર્ણાંક w, પૂર્ણાંક h)
{
    augment_args a = random_augment_args(im, angle, aspect, નીચલું, ઉપલું, w, h);
    image crop = rotate_crop_image(im, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);
    પાછા crop;
}

દશાંશ three_way_max(દશાંશ a, દશાંશ b, દશાંશ c)
{
    પાછા (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

દશાંશ three_way_min(દશાંશ a, દશાંશ b, દશાંશ c)
{
    પાછા (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

શૂન્ય yuv_to_rgb(image im)
{
    પ્રતિજ્ઞા(im.c == 3);
    પૂર્ણાંક i, j;
    દશાંશ r, g, b;
    દશાંશ y, u, v;
    માટે(j = 0; j < im.h; ++j){
        માટે(i = 0; i < im.w; ++i){
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

શૂન્ય rgb_to_yuv(image im)
{
    પ્રતિજ્ઞા(im.c == 3);
    પૂર્ણાંક i, j;
    દશાંશ r, g, b;
    દશાંશ y, u, v;
    માટે(j = 0; j < im.h; ++j){
        માટે(i = 0; i < im.w; ++i){
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
શૂન્ય rgb_to_hsv(image im)
{
    પ્રતિજ્ઞા(im.c == 3);
    પૂર્ણાંક i, j;
    દશાંશ r, g, b;
    દશાંશ h, s, v;
    માટે(j = 0; j < im.h; ++j){
        માટે(i = 0; i < im.w; ++i){
            r = get_pixel(im, i , j, 0);
            g = get_pixel(im, i , j, 1);
            b = get_pixel(im, i , j, 2);
            દશાંશ max = three_way_max(r,g,b);
            દશાંશ min = three_way_min(r,g,b);
            દશાંશ delta = max - min;
            v = max;
            જો(max == 0){
                s = 0;
                h = 0;
            }નહીંતર{
                s = delta/max;
                જો(r == max){
                    h = (g - b) / delta;
                } નહીંતર જો (g == max) {
                    h = 2 + (b - r) / delta;
                } નહીંતર {
                    h = 4 + (r - g) / delta;
                }
                જો (h < 0) h += 6;
                h = h/6.;
            }
            set_pixel(im, i, j, 0, h);
            set_pixel(im, i, j, 1, s);
            set_pixel(im, i, j, 2, v);
        }
    }
}

શૂન્ય hsv_to_rgb(image im)
{
    પ્રતિજ્ઞા(im.c == 3);
    પૂર્ણાંક i, j;
    દશાંશ r, g, b;
    દશાંશ h, s, v;
    દશાંશ f, p, q, t;
    માટે(j = 0; j < im.h; ++j){
        માટે(i = 0; i < im.w; ++i){
            h = 6 * get_pixel(im, i , j, 0);
            s = get_pixel(im, i , j, 1);
            v = get_pixel(im, i , j, 2);
            જો (s == 0) {
                r = g = b = v;
            } નહીંતર {
                પૂર્ણાંક index = floor(h);
                f = h - index;
                p = v*(1-s);
                q = v*(1-s*f);
                t = v*(1-s*(1-f));
                જો(index == 0){
                    r = v; g = t; b = p;
                } નહીંતર જો(index == 1){
                    r = q; g = v; b = p;
                } નહીંતર જો(index == 2){
                    r = p; g = v; b = t;
                } નહીંતર જો(index == 3){
                    r = p; g = q; b = v;
                } નહીંતર જો(index == 4){
                    r = t; g = p; b = v;
                } નહીંતર {
                    r = v; g = p; b = q;
                }
            }
            set_pixel(im, i, j, 0, r);
            set_pixel(im, i, j, 1, g);
            set_pixel(im, i, j, 2, b);
        }
    }
}

શૂન્ય grayscale_image_3c(image im)
{
    પ્રતિજ્ઞા(im.c == 3);
    પૂર્ણાંક i, j, k;
    દશાંશ scale[] = {0.299, 0.587, 0.114};
    માટે(j = 0; j < im.h; ++j){
        માટે(i = 0; i < im.w; ++i){
            દશાંશ val = 0;
            માટે(k = 0; k < 3; ++k){
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
    પ્રતિજ્ઞા(im.c == 3);
    પૂર્ણાંક i, j, k;
    image gray = make_image(im.w, im.h, 1);
    દશાંશ scale[] = {0.299, 0.587, 0.114};
    માટે(k = 0; k < im.c; ++k){
        માટે(j = 0; j < im.h; ++j){
            માટે(i = 0; i < im.w; ++i){
                gray.data[i+im.w*j] += scale[k]*get_pixel(im, i, j, k);
            }
        }
    }
    પાછા gray;
}

image threshold_image(image im, દશાંશ thresh)
{
    પૂર્ણાંક i;
    image t = make_image(im.w, im.h, im.c);
    માટે(i = 0; i < im.w*im.h*im.c; ++i){
        t.data[i] = im.data[i]>thresh ? 1 : 0;
    }
    પાછા t;
}

image blend_image(image fore, image back, દશાંશ alpha)
{
    પ્રતિજ્ઞા(fore.w == back.w && fore.h == back.h && fore.c == back.c);
    image blend = make_image(fore.w, fore.h, fore.c);
    પૂર્ણાંક i, j, k;
    માટે(k = 0; k < fore.c; ++k){
        માટે(j = 0; j < fore.h; ++j){
            માટે(i = 0; i < fore.w; ++i){
                દશાંશ val = alpha * get_pixel(fore, i, j, k) + 
                    (1 - alpha)* get_pixel(back, i, j, k);
                set_pixel(blend, i, j, k, val);
            }
        }
    }
    પાછા blend;
}

શૂન્ય scale_image_channel(image im, પૂર્ણાંક c, દશાંશ v)
{
    પૂર્ણાંક i, j;
    માટે(j = 0; j < im.h; ++j){
        માટે(i = 0; i < im.w; ++i){
            દશાંશ pix = get_pixel(im, i, j, c);
            pix = pix*v;
            set_pixel(im, i, j, c, pix);
        }
    }
}

શૂન્ય translate_image_channel(image im, પૂર્ણાંક c, દશાંશ v)
{
    પૂર્ણાંક i, j;
    માટે(j = 0; j < im.h; ++j){
        માટે(i = 0; i < im.w; ++i){
            દશાંશ pix = get_pixel(im, i, j, c);
            pix = pix+v;
            set_pixel(im, i, j, c, pix);
        }
    }
}

image binarize_image(image im)
{
    image c = copy_image(im);
    પૂર્ણાંક i;
    માટે(i = 0; i < im.w * im.h * im.c; ++i){
        જો(c.data[i] > .5) c.data[i] = 1;
        નહીંતર c.data[i] = 0;
    }
    પાછા c;
}

શૂન્ય saturate_image(image im, દશાંશ sat)
{
    rgb_to_hsv(im);
    scale_image_channel(im, 1, sat);
    hsv_to_rgb(im);
    constrain_image(im);
}

શૂન્ય hue_image(image im, દશાંશ hue)
{
    rgb_to_hsv(im);
    પૂર્ણાંક i;
    માટે(i = 0; i < im.w*im.h; ++i){
        im.data[i] = im.data[i] + hue;
        જો (im.data[i] > 1) im.data[i] -= 1;
        જો (im.data[i] < 0) im.data[i] += 1;
    }
    hsv_to_rgb(im);
    constrain_image(im);
}

શૂન્ય exposure_image(image im, દશાંશ sat)
{
    rgb_to_hsv(im);
    scale_image_channel(im, 2, sat);
    hsv_to_rgb(im);
    constrain_image(im);
}

શૂન્ય distort_image(image im, દશાંશ hue, દશાંશ sat, દશાંશ val)
{
    rgb_to_hsv(im);
    scale_image_channel(im, 1, sat);
    scale_image_channel(im, 2, val);
    પૂર્ણાંક i;
    માટે(i = 0; i < im.w*im.h; ++i){
        im.data[i] = im.data[i] + hue;
        જો (im.data[i] > 1) im.data[i] -= 1;
        જો (im.data[i] < 0) im.data[i] += 1;
    }
    hsv_to_rgb(im);
    constrain_image(im);
}

શૂન્ય random_distort_image(image im, દશાંશ hue, દશાંશ saturation, દશાંશ exposure)
{
    દશાંશ dhue = rand_uniform(-hue, hue);
    દશાંશ dsat = rand_scale(saturation);
    દશાંશ dexp = rand_scale(exposure);
    distort_image(im, dhue, dsat, dexp);
}

શૂન્ય saturate_exposure_image(image im, દશાંશ sat, દશાંશ exposure)
{
    rgb_to_hsv(im);
    scale_image_channel(im, 1, sat);
    scale_image_channel(im, 2, exposure);
    hsv_to_rgb(im);
    constrain_image(im);
}

image resize_image(image im, પૂર્ણાંક w, પૂર્ણાંક h)
{
    image resized = make_image(w, h, im.c);   
    image part = make_image(w, im.h, im.c);
    પૂર્ણાંક r, c, k;
    દશાંશ w_scale = (દશાંશ)(im.w - 1) / (w - 1);
    દશાંશ h_scale = (દશાંશ)(im.h - 1) / (h - 1);
    માટે(k = 0; k < im.c; ++k){
        માટે(r = 0; r < im.h; ++r){
            માટે(c = 0; c < w; ++c){
                દશાંશ val = 0;
                જો(c == w-1 || im.w == 1){
                    val = get_pixel(im, im.w-1, r, k);
                } નહીંતર {
                    દશાંશ sx = c*w_scale;
                    પૂર્ણાંક ix = (પૂર્ણાંક) sx;
                    દશાંશ dx = sx - ix;
                    val = (1 - dx) * get_pixel(im, ix, r, k) + dx * get_pixel(im, ix+1, r, k);
                }
                set_pixel(part, c, r, k, val);
            }
        }
    }
    માટે(k = 0; k < im.c; ++k){
        માટે(r = 0; r < h; ++r){
            દશાંશ sy = r*h_scale;
            પૂર્ણાંક iy = (પૂર્ણાંક) sy;
            દશાંશ dy = sy - iy;
            માટે(c = 0; c < w; ++c){
                દશાંશ val = (1-dy) * get_pixel(part, c, iy, k);
                set_pixel(resized, c, r, k, val);
            }
            જો(r == h-1 || im.h == 1) ચાલુ;
            માટે(c = 0; c < w; ++c){
                દશાંશ val = dy * get_pixel(part, c, iy+1, k);
                add_pixel(resized, c, r, k, val);
            }
        }
    }

    free_image(part);
    પાછા resized;
}


શૂન્ય test_resize(અક્ષર *filename)
{
    image im = load_image(filename, 0,0, 3);
    દશાંશ mag = mag_array(im.data, im.w*im.h*im.c);
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
    જ્યારે(1){
        image aug = random_augment_image(im, 0, .75, 320, 448, 320, 320);
        show_image(aug, "aug", 1);
        free_image(aug);


        દશાંશ exposure = 1.15;
        દશાંશ saturation = 1.15;
        દશાંશ hue = .05;

        image c = copy_image(im);

        દશાંશ dexp = rand_scale(exposure);
        દશાંશ dsat = rand_scale(saturation);
        દશાંશ dhue = rand_uniform(-hue, hue);

        distort_image(c, dhue, dsat, dexp);
        show_image(c, "rand", 1);
        printf("%f %f %f\n", dhue, dsat, dexp);
        free_image(c);
    }
#endif
}


image load_image_stb(અક્ષર *filename, પૂર્ણાંક channels)
{
    પૂર્ણાંક w, h, c;
    unsigned અક્ષર *data = stbi_load(filename, &w, &h, &c, channels);
    જો (!data) {
        fprintf(stderr, "Cannot load image \"%s\"\nSTB Reason: %s\n", filename, stbi_failure_reason());
        exit(0);
    }
    જો(channels) c = channels;
    પૂર્ણાંક i,j,k;
    image im = make_image(w, h, c);
    માટે(k = 0; k < c; ++k){
        માટે(j = 0; j < h; ++j){
            માટે(i = 0; i < w; ++i){
                પૂર્ણાંક dst_index = i + w*j + w*h*k;
                પૂર્ણાંક src_index = k + c*i + c*w*j;
                im.data[dst_index] = (દશાંશ)data[src_index]/255.;
            }
        }
    }
    મુક્ત(data);
    પાછા im;
}

image load_image(અક્ષર *filename, પૂર્ણાંક w, પૂર્ણાંક h, પૂર્ણાંક c)
{
#ifdef OPENCV
    image out = load_image_cv(filename, c);
#નહીંતર
    image out = load_image_stb(filename, c);
#endif

    જો((h && w) && (h != out.h || w != out.w)){
        image resized = resize_image(out, w, h);
        free_image(out);
        out = resized;
    }
    પાછા out;
}

image load_image_color(અક્ષર *filename, પૂર્ણાંક w, પૂર્ણાંક h)
{
    પાછા load_image(filename, w, h, 3);
}

image get_image_layer(image m, પૂર્ણાંક l)
{
    image out = make_image(m.w, m.h, 1);
    પૂર્ણાંક i;
    માટે(i = 0; i < m.h*m.w; ++i){
        out.data[i] = m.data[i+l*m.h*m.w];
    }
    પાછા out;
}
શૂન્ય print_image(image m)
{
    પૂર્ણાંક i, j, k;
    માટે(i =0 ; i < m.c; ++i){
        માટે(j =0 ; j < m.h; ++j){
            માટે(k = 0; k < m.w; ++k){
                printf("%.2lf, ", m.data[i*m.h*m.w + j*m.w + k]);
                જો(k > 30) તોડો;
            }
            printf("\n");
            જો(j > 30) તોડો;
        }
        printf("\n");
    }
    printf("\n");
}

image collapse_images_vert(image *ims, પૂર્ણાંક n)
{
    પૂર્ણાંક color = 1;
    પૂર્ણાંક border = 1;
    પૂર્ણાંક h,w,c;
    w = ims[0].w;
    h = (ims[0].h + border) * n - border;
    c = ims[0].c;
    જો(c != 3 || !color){
        w = (w+border)*c - border;
        c = 1;
    }

    image filters = make_image(w, h, c);
    પૂર્ણાંક i,j;
    માટે(i = 0; i < n; ++i){
        પૂર્ણાંક h_offset = i*(ims[0].h+border);
        image copy = copy_image(ims[i]);
        //normalize_image(copy);
        જો(c == 3 && color){
            embed_image(copy, filters, 0, h_offset);
        }
        નહીંતર{
            માટે(j = 0; j < copy.c; ++j){
                પૂર્ણાંક w_offset = j*(ims[0].w+border);
                image layer = get_image_layer(copy, j);
                embed_image(layer, filters, w_offset, h_offset);
                free_image(layer);
            }
        }
        free_image(copy);
    }
    પાછા filters;
} 

image collapse_images_horz(image *ims, પૂર્ણાંક n)
{
    પૂર્ણાંક color = 1;
    પૂર્ણાંક border = 1;
    પૂર્ણાંક h,w,c;
    પૂર્ણાંક size = ims[0].h;
    h = size;
    w = (ims[0].w + border) * n - border;
    c = ims[0].c;
    જો(c != 3 || !color){
        h = (h+border)*c - border;
        c = 1;
    }

    image filters = make_image(w, h, c);
    પૂર્ણાંક i,j;
    માટે(i = 0; i < n; ++i){
        પૂર્ણાંક w_offset = i*(size+border);
        image copy = copy_image(ims[i]);
        //normalize_image(copy);
        જો(c == 3 && color){
            embed_image(copy, filters, w_offset, 0);
        }
        નહીંતર{
            માટે(j = 0; j < copy.c; ++j){
                પૂર્ણાંક h_offset = j*(size+border);
                image layer = get_image_layer(copy, j);
                embed_image(layer, filters, w_offset, h_offset);
                free_image(layer);
            }
        }
        free_image(copy);
    }
    પાછા filters;
} 

શૂન્ય show_image_normalized(image im, અચલ અક્ષર *name)
{
    image c = copy_image(im);
    normalize_image(c);
    show_image(c, name, 1);
    free_image(c);
}

શૂન્ય show_images(image *ims, પૂર્ણાંક n, અક્ષર *window)
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

શૂન્ય free_image(image m)
{
    જો(m.data){
        મુક્ત(m.data);
    }
}
