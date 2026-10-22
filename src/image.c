#include "image.h"
#include "utils.h"
#include "blas.h"
#include "cuda.h"
#include <stdio.h>
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

ຈຳນວນເຕັມ windows = 0;

float colors[6][3] = { {1,0,1}, {0,0,1},{0,1,1},{0,1,0},{1,1,0},{1,0,0} };

float get_color(ຈຳນວນເຕັມ c, ຈຳນວນເຕັມ x, ຈຳນວນເຕັມ max)
{
    float ratio = ((float)x/max)*5;
    ຈຳນວນເຕັມ i = floor(ratio);
    ຈຳນວນເຕັມ j = ceil(ratio);
    ratio -= i;
    float r = (1-ratio) * colors[i][c] + ratio*colors[j][c];
    //printf("%f\n", r);
    ສົ່ງຄືນ r;
}

image mask_to_rgb(image mask)
{
    ຈຳນວນເຕັມ n = mask.c;
    image im = make_image(mask.w, mask.h, 3);
    ຈຳນວນເຕັມ i, j;
    ສຳລັບ(j = 0; j < n; ++j){
        ຈຳນວນເຕັມ offset = j*123457 % n;
        float red = get_color(2,offset,n);
        float green = get_color(1,offset,n);
        float blue = get_color(0,offset,n);
        ສຳລັບ(i = 0; i < im.w*im.h; ++i){
            im.data[i + 0*im.w*im.h] += mask.data[j*im.h*im.w + i]*red;
            im.data[i + 1*im.w*im.h] += mask.data[j*im.h*im.w + i]*green;
            im.data[i + 2*im.w*im.h] += mask.data[j*im.h*im.w + i]*blue;
        }
    }
    ສົ່ງຄືນ im;
}

static float get_pixel(image m, ຈຳນວນເຕັມ x, ຈຳນວນເຕັມ y, ຈຳນວນເຕັມ c)
{
    assert(x < m.w && y < m.h && c < m.c);
    ສົ່ງຄືນ m.data[c*m.h*m.w + y*m.w + x];
}
static float get_pixel_extend(image m, ຈຳນວນເຕັມ x, ຈຳນວນເຕັມ y, ຈຳນວນເຕັມ c)
{
    ຖ້າ(x < 0 || x >= m.w || y < 0 || y >= m.h) ສົ່ງຄືນ 0;
    /*
    if(x < 0) x = 0;
    if(x >= m.w) x = m.w-1;
    if(y < 0) y = 0;
    if(y >= m.h) y = m.h-1;
    */
    ຖ້າ(c < 0 || c >= m.c) ສົ່ງຄືນ 0;
    ສົ່ງຄືນ get_pixel(m, x, y, c);
}
static void set_pixel(image m, ຈຳນວນເຕັມ x, ຈຳນວນເຕັມ y, ຈຳນວນເຕັມ c, float val)
{
    ຖ້າ (x < 0 || y < 0 || c < 0 || x >= m.w || y >= m.h || c >= m.c) ສົ່ງຄືນ;
    assert(x < m.w && y < m.h && c < m.c);
    m.data[c*m.h*m.w + y*m.w + x] = val;
}
static void add_pixel(image m, ຈຳນວນເຕັມ x, ຈຳນວນເຕັມ y, ຈຳນວນເຕັມ c, float val)
{
    assert(x < m.w && y < m.h && c < m.c);
    m.data[c*m.h*m.w + y*m.w + x] += val;
}

static float bilinear_interpolate(image im, float x, float y, ຈຳນວນເຕັມ c)
{
    ຈຳນວນເຕັມ ix = (ຈຳນວນເຕັມ) floorf(x);
    ຈຳນວນເຕັມ iy = (ຈຳນວນເຕັມ) floorf(y);

    float dx = x - ix;
    float dy = y - iy;

    float val = (1-dy) * (1-dx) * get_pixel_extend(im, ix, iy, c) + 
        dy     * (1-dx) * get_pixel_extend(im, ix, iy+1, c) + 
        (1-dy) *   dx   * get_pixel_extend(im, ix+1, iy, c) +
        dy     *   dx   * get_pixel_extend(im, ix+1, iy+1, c);
    ສົ່ງຄືນ val;
}


void composite_image(image source, image dest, ຈຳນວນເຕັມ dx, ຈຳນວນເຕັມ dy)
{
    ຈຳນວນເຕັມ x,y,k;
    ສຳລັບ(k = 0; k < source.c; ++k){
        ສຳລັບ(y = 0; y < source.h; ++y){
            ສຳລັບ(x = 0; x < source.w; ++x){
                float val = get_pixel(source, x, y, k);
                float val2 = get_pixel_extend(dest, dx+x, dy+y, k);
                set_pixel(dest, dx+x, dy+y, k, val * val2);
            }
        }
    }
}

image border_image(image a, ຈຳນວນເຕັມ border)
{
    image b = make_image(a.w + 2*border, a.h + 2*border, a.c);
    ຈຳນວນເຕັມ x,y,k;
    ສຳລັບ(k = 0; k < b.c; ++k){
        ສຳລັບ(y = 0; y < b.h; ++y){
            ສຳລັບ(x = 0; x < b.w; ++x){
                float val = get_pixel_extend(a, x - border, y - border, k);
                ຖ້າ(x - border < 0 || x - border >= a.w || y - border < 0 || y - border >= a.h) val = 1;
                set_pixel(b, x, y, k, val);
            }
        }
    }
    ສົ່ງຄືນ b;
}

image tile_images(image a, image b, ຈຳນວນເຕັມ dx)
{
    ຖ້າ(a.w == 0) ສົ່ງຄືນ copy_image(b);
    image c = make_image(a.w + b.w + dx, (a.h > b.h) ? a.h : b.h, (a.c > b.c) ? a.c : b.c);
    fill_cpu(c.w*c.h*c.c, 1, c.data, 1);
    embed_image(a, c, 0, 0); 
    composite_image(b, c, a.w + dx, 0);
    ສົ່ງຄືນ c;
}

image get_label(image **characters, var_na *string, ຈຳນວນເຕັມ size)
{
    size = size/10;
    ຖ້າ(size > 7) size = 7;
    image label = make_empty_image(0,0,0);
    ໃນຂະນະທີ່(*string){
        image l = characters[size][(ຈຳນວນເຕັມ)*string];
        image n = tile_images(label, l, -size - 1 + (size+1)/2);
        free_image(label);
        label = n;
        ++string;
    }
    image b = border_image(label, label.h*.25);
    free_image(label);
    ສົ່ງຄືນ b;
}

void draw_label(image a, ຈຳນວນເຕັມ r, ຈຳນວນເຕັມ c, image label, const float *rgb)
{
    ຈຳນວນເຕັມ w = label.w;
    ຈຳນວນເຕັມ h = label.h;
    ຖ້າ (r - h >= 0) r = r - h;

    ຈຳນວນເຕັມ i, j, k;
    ສຳລັບ(j = 0; j < h && j + r < a.h; ++j){
        ສຳລັບ(i = 0; i < w && i + c < a.w; ++i){
            ສຳລັບ(k = 0; k < label.c; ++k){
                float val = get_pixel(label, i, j, k);
                set_pixel(a, i+c, j+r, k, rgb[k] * val);
            }
        }
    }
}

void draw_box(image a, ຈຳນວນເຕັມ x1, ຈຳນວນເຕັມ y1, ຈຳນວນເຕັມ x2, ຈຳນວນເຕັມ y2, float r, float g, float b)
{
    //normalize_image(a);
    ຈຳນວນເຕັມ i;
    ຖ້າ(x1 < 0) x1 = 0;
    ຖ້າ(x1 >= a.w) x1 = a.w-1;
    ຖ້າ(x2 < 0) x2 = 0;
    ຖ້າ(x2 >= a.w) x2 = a.w-1;

    ຖ້າ(y1 < 0) y1 = 0;
    ຖ້າ(y1 >= a.h) y1 = a.h-1;
    ຖ້າ(y2 < 0) y2 = 0;
    ຖ້າ(y2 >= a.h) y2 = a.h-1;

    ສຳລັບ(i = x1; i <= x2; ++i){
        a.data[i + y1*a.w + 0*a.w*a.h] = r;
        a.data[i + y2*a.w + 0*a.w*a.h] = r;

        a.data[i + y1*a.w + 1*a.w*a.h] = g;
        a.data[i + y2*a.w + 1*a.w*a.h] = g;

        a.data[i + y1*a.w + 2*a.w*a.h] = b;
        a.data[i + y2*a.w + 2*a.w*a.h] = b;
    }
    ສຳລັບ(i = y1; i <= y2; ++i){
        a.data[x1 + i*a.w + 0*a.w*a.h] = r;
        a.data[x2 + i*a.w + 0*a.w*a.h] = r;

        a.data[x1 + i*a.w + 1*a.w*a.h] = g;
        a.data[x2 + i*a.w + 1*a.w*a.h] = g;

        a.data[x1 + i*a.w + 2*a.w*a.h] = b;
        a.data[x2 + i*a.w + 2*a.w*a.h] = b;
    }
}

void draw_box_width(image a, ຈຳນວນເຕັມ x1, ຈຳນວນເຕັມ y1, ຈຳນວນເຕັມ x2, ຈຳນວນເຕັມ y2, ຈຳນວນເຕັມ w, float r, float g, float b)
{
    ຈຳນວນເຕັມ i;
    ສຳລັບ(i = 0; i < w; ++i){
        draw_box(a, x1+i, y1+i, x2-i, y2-i, r, g, b);
    }
}

void draw_bbox(image a, box bbox, ຈຳນວນເຕັມ w, float r, float g, float b)
{
    ຈຳນວນເຕັມ left  = (bbox.x-bbox.w/2)*a.w;
    ຈຳນວນເຕັມ right = (bbox.x+bbox.w/2)*a.w;
    ຈຳນວນເຕັມ top   = (bbox.y-bbox.h/2)*a.h;
    ຈຳນວນເຕັມ bot   = (bbox.y+bbox.h/2)*a.h;

    ຈຳນວນເຕັມ i;
    ສຳລັບ(i = 0; i < w; ++i){
        draw_box(a, left+i, top+i, right-i, bot-i, r, g, b);
    }
}

image **load_alphabet()
{
    ຈຳນວນເຕັມ i, j;
    const ຈຳນວນເຕັມ nsize = 8;
    image **alphabets = calloc(nsize, parimaa_na(image));
    ສຳລັບ(j = 0; j < nsize; ++j){
        alphabets[j] = calloc(128, parimaa_na(image));
        ສຳລັບ(i = 32; i < 127; ++i){
            var_na buff[256];
            sprintf(buff, "data/labels/%d_%d.png", i, j);
            alphabets[j][i] = load_image_color(buff, 0, 0);
        }
    }
    ສົ່ງຄືນ alphabets;
}

void draw_detections(image im, detection *dets, ຈຳນວນເຕັມ num, float thresh, var_na **names, image **alphabet, ຈຳນວນເຕັມ classes)
{
    ຈຳນວນເຕັມ i,j;

    ສຳລັບ(i = 0; i < num; ++i){
        var_na labelstr[4096] = {0};
        ຈຳນວນເຕັມ class = -1;
        ສຳລັບ(j = 0; j < classes; ++j){
            ຖ້າ (dets[i].prob[j] > thresh){
                ຖ້າ (class < 0) {
                    strcat(labelstr, names[j]);
                    class = j;
                } ບໍ່ດັ່ງນັ້ນ {
                    strcat(labelstr, ", ");
                    strcat(labelstr, names[j]);
                }
                printf("%s: %.0f%%\n", names[j], dets[i].prob[j]*100);
            }
        }
        ຖ້າ(class >= 0){
            ຈຳນວນເຕັມ width = im.h * .006;

            /*
               if(0){
               width = pow(prob, 1./2.)*10+1;
               alphabet = 0;
               }
             */

            //printf("%d %s: %.0f%%\n", i, names[class], prob*100);
            ຈຳນວນເຕັມ offset = class*123457 % classes;
            float red = get_color(2,offset,classes);
            float green = get_color(1,offset,classes);
            float blue = get_color(0,offset,classes);
            float rgb[3];

            //width = prob*20+2;

            rgb[0] = red;
            rgb[1] = green;
            rgb[2] = blue;
            box b = dets[i].bbox;
            //printf("%f %f %f %f\n", b.x, b.y, b.w, b.h);

            ຈຳນວນເຕັມ left  = (b.x-b.w/2.)*im.w;
            ຈຳນວນເຕັມ right = (b.x+b.w/2.)*im.w;
            ຈຳນວນເຕັມ top   = (b.y-b.h/2.)*im.h;
            ຈຳນວນເຕັມ bot   = (b.y+b.h/2.)*im.h;

            ຖ້າ(left < 0) left = 0;
            ຖ້າ(right > im.w-1) right = im.w-1;
            ຖ້າ(top < 0) top = 0;
            ຖ້າ(bot > im.h-1) bot = im.h-1;

            draw_box_width(im, left, top, right, bot, width, red, green, blue);
            ຖ້າ (alphabet) {
                image label = get_label(alphabet, labelstr, (im.h*.03));
                draw_label(im, top + width, left, label, rgb);
                free_image(label);
            }
            ຖ້າ (dets[i].mask){
                image mask = float_to_image(14, 14, 1, dets[i].mask);
                image resized_mask = resize_image(mask, b.w*im.w, b.h*im.h);
                image tmask = threshold_image(resized_mask, .5);
                embed_image(tmask, im, left, top);
                free_image(mask);
                free_image(resized_mask);
                free_image(tmask);
            }
        }
    }
}

void transpose_image(image im)
{
    assert(im.w == im.h);
    ຈຳນວນເຕັມ n, m;
    ຈຳນວນເຕັມ c;
    ສຳລັບ(c = 0; c < im.c; ++c){
        ສຳລັບ(n = 0; n < im.w-1; ++n){
            ສຳລັບ(m = n + 1; m < im.w; ++m){
                float swap = im.data[m + im.w*(n + im.h*c)];
                im.data[m + im.w*(n + im.h*c)] = im.data[n + im.w*(m + im.h*c)];
                im.data[n + im.w*(m + im.h*c)] = swap;
            }
        }
    }
}

void rotate_image_cw(image im, ຈຳນວນເຕັມ times)
{
    assert(im.w == im.h);
    times = (times + 400) % 4;
    ຈຳນວນເຕັມ i, x, y, c;
    ຈຳນວນເຕັມ n = im.w;
    ສຳລັບ(i = 0; i < times; ++i){
        ສຳລັບ(c = 0; c < im.c; ++c){
            ສຳລັບ(x = 0; x < n/2; ++x){
                ສຳລັບ(y = 0; y < (n-1)/2 + 1; ++y){
                    float temp = im.data[y + im.w*(x + im.h*c)];
                    im.data[y + im.w*(x + im.h*c)] = im.data[n-1-x + im.w*(y + im.h*c)];
                    im.data[n-1-x + im.w*(y + im.h*c)] = im.data[n-1-y + im.w*(n-1-x + im.h*c)];
                    im.data[n-1-y + im.w*(n-1-x + im.h*c)] = im.data[x + im.w*(n-1-y + im.h*c)];
                    im.data[x + im.w*(n-1-y + im.h*c)] = temp;
                }
            }
        }
    }
}

void flip_image(image a)
{
    ຈຳນວນເຕັມ i,j,k;
    ສຳລັບ(k = 0; k < a.c; ++k){
        ສຳລັບ(i = 0; i < a.h; ++i){
            ສຳລັບ(j = 0; j < a.w/2; ++j){
                ຈຳນວນເຕັມ index = j + a.w*(i + a.h*(k));
                ຈຳນວນເຕັມ flip = (a.w - j - 1) + a.w*(i + a.h*(k));
                float swap = a.data[flip];
                a.data[flip] = a.data[index];
                a.data[index] = swap;
            }
        }
    }
}

image image_distance(image a, image b)
{
    ຈຳນວນເຕັມ i,j;
    image dist = make_image(a.w, a.h, 1);
    ສຳລັບ(i = 0; i < a.c; ++i){
        ສຳລັບ(j = 0; j < a.h*a.w; ++j){
            dist.data[j] += pow(a.data[i*a.h*a.w+j]-b.data[i*a.h*a.w+j],2);
        }
    }
    ສຳລັບ(j = 0; j < a.h*a.w; ++j){
        dist.data[j] = sqrt(dist.data[j]);
    }
    ສົ່ງຄືນ dist;
}

void ghost_image(image source, image dest, ຈຳນວນເຕັມ dx, ຈຳນວນເຕັມ dy)
{
    ຈຳນວນເຕັມ x,y,k;
    float max_dist = sqrt((-source.w/2. + .5)*(-source.w/2. + .5));
    ສຳລັບ(k = 0; k < source.c; ++k){
        ສຳລັບ(y = 0; y < source.h; ++y){
            ສຳລັບ(x = 0; x < source.w; ++x){
                float dist = sqrt((x - source.w/2. + .5)*(x - source.w/2. + .5) + (y - source.h/2. + .5)*(y - source.h/2. + .5));
                float alpha = (1 - dist/max_dist);
                ຖ້າ(alpha < 0) alpha = 0;
                float v1 = get_pixel(source, x,y,k);
                float v2 = get_pixel(dest, dx+x,dy+y,k);
                float val = alpha*v1 + (1-alpha)*v2;
                set_pixel(dest, dx+x, dy+y, k, val);
            }
        }
    }
}

void blocky_image(image im, ຈຳນວນເຕັມ s)
{
    ຈຳນວນເຕັມ i,j,k;
    ສຳລັບ(k = 0; k < im.c; ++k){
        ສຳລັບ(j = 0; j < im.h; ++j){
            ສຳລັບ(i = 0; i < im.w; ++i){
                im.data[i + im.w*(j + im.h*k)] = im.data[i/s*s + im.w*(j/s*s + im.h*k)];
            }
        }
    }
}

void censor_image(image im, ຈຳນວນເຕັມ dx, ຈຳນວນເຕັມ dy, ຈຳນວນເຕັມ w, ຈຳນວນເຕັມ h)
{
    ຈຳນວນເຕັມ i,j,k;
    ຈຳນວນເຕັມ s = 32;
    ຖ້າ(dx < 0) dx = 0;
    ຖ້າ(dy < 0) dy = 0;

    ສຳລັບ(k = 0; k < im.c; ++k){
        ສຳລັບ(j = dy; j < dy + h && j < im.h; ++j){
            ສຳລັບ(i = dx; i < dx + w && i < im.w; ++i){
                im.data[i + im.w*(j + im.h*k)] = im.data[i/s*s + im.w*(j/s*s + im.h*k)];
                //im.data[i + j*im.w + k*im.w*im.h] = 0;
            }
        }
    }
}

void embed_image(image source, image dest, ຈຳນວນເຕັມ dx, ຈຳນວນເຕັມ dy)
{
    ຈຳນວນເຕັມ x,y,k;
    ສຳລັບ(k = 0; k < source.c; ++k){
        ສຳລັບ(y = 0; y < source.h; ++y){
            ສຳລັບ(x = 0; x < source.w; ++x){
                float val = get_pixel(source, x,y,k);
                set_pixel(dest, dx+x, dy+y, k, val);
            }
        }
    }
}

image collapse_image_layers(image source, ຈຳນວນເຕັມ border)
{
    ຈຳນວນເຕັມ h = source.h;
    h = (h+border)*source.c - border;
    image dest = make_image(source.w, h, 1);
    ຈຳນວນເຕັມ i;
    ສຳລັບ(i = 0; i < source.c; ++i){
        image layer = get_image_layer(source, i);
        ຈຳນວນເຕັມ h_offset = i*(source.h+border);
        embed_image(layer, dest, 0, h_offset);
        free_image(layer);
    }
    ສົ່ງຄືນ dest;
}

void constrain_image(image im)
{
    ຈຳນວນເຕັມ i;
    ສຳລັບ(i = 0; i < im.w*im.h*im.c; ++i){
        ຖ້າ(im.data[i] < 0) im.data[i] = 0;
        ຖ້າ(im.data[i] > 1) im.data[i] = 1;
    }
}

void normalize_image(image p)
{
    ຈຳນວນເຕັມ i;
    float min = 9999999;
    float max = -999999;

    ສຳລັບ(i = 0; i < p.h*p.w*p.c; ++i){
        float v = p.data[i];
        ຖ້າ(v < min) min = v;
        ຖ້າ(v > max) max = v;
    }
    ຖ້າ(max - min < .000000001){
        min = 0;
        max = 1;
    }
    ສຳລັບ(i = 0; i < p.c*p.w*p.h; ++i){
        p.data[i] = (p.data[i] - min)/(max-min);
    }
}

void normalize_image2(image p)
{
    float *min = calloc(p.c, parimaa_na(float));
    float *max = calloc(p.c, parimaa_na(float));
    ຈຳນວນເຕັມ i,j;
    ສຳລັບ(i = 0; i < p.c; ++i) min[i] = max[i] = p.data[i*p.h*p.w];

    ສຳລັບ(j = 0; j < p.c; ++j){
        ສຳລັບ(i = 0; i < p.h*p.w; ++i){
            float v = p.data[i+j*p.h*p.w];
            ຖ້າ(v < min[j]) min[j] = v;
            ຖ້າ(v > max[j]) max[j] = v;
        }
    }
    ສຳລັບ(i = 0; i < p.c; ++i){
        ຖ້າ(max[i] - min[i] < .000000001){
            min[i] = 0;
            max[i] = 1;
        }
    }
    ສຳລັບ(j = 0; j < p.c; ++j){
        ສຳລັບ(i = 0; i < p.w*p.h; ++i){
            p.data[i+j*p.h*p.w] = (p.data[i+j*p.h*p.w] - min[j])/(max[j]-min[j]);
        }
    }
    free(min);
    free(max);
}

void copy_image_into(image src, image dest)
{
    memcpy(dest.data, src.data, src.h*src.w*src.c*parimaa_na(float));
}

image copy_image(image p)
{
    image copy = p;
    copy.data = calloc(p.h*p.w*p.c, parimaa_na(float));
    memcpy(copy.data, p.data, p.h*p.w*p.c*parimaa_na(float));
    ສົ່ງຄືນ copy;
}

void rgbgr_image(image im)
{
    ຈຳນວນເຕັມ i;
    ສຳລັບ(i = 0; i < im.w*im.h; ++i){
        float swap = im.data[i];
        im.data[i] = im.data[i+im.w*im.h*2];
        im.data[i+im.w*im.h*2] = swap;
    }
}

ຈຳນວນເຕັມ show_image(image p, const var_na *name, ຈຳນວນເຕັມ ms)
{
#ifdef OPENCV
    ຈຳນວນເຕັມ c = show_image_cv(p, name, ms);
    ສົ່ງຄືນ c;
#ບໍ່ດັ່ງນັ້ນ
    fprintf(stderr, "Not compiled with OpenCV, saving to %s.png instead\n", name);
    save_image(p, name);
    ສົ່ງຄືນ -1;
#endif
}

void save_image_options(image im, const var_na *name, IMTYPE f, ຈຳນວນເຕັມ quality)
{
    var_na buff[256];
    //sprintf(buff, "%s (%d)", name, windows);
    ຖ້າ(f == PNG)       sprintf(buff, "%s.png", name);
    ບໍ່ດັ່ງນັ້ນ ຖ້າ (f == BMP) sprintf(buff, "%s.bmp", name);
    ບໍ່ດັ່ງນັ້ນ ຖ້າ (f == TGA) sprintf(buff, "%s.tga", name);
    ບໍ່ດັ່ງນັ້ນ ຖ້າ (f == JPG) sprintf(buff, "%s.jpg", name);
    ບໍ່ດັ່ງນັ້ນ               sprintf(buff, "%s.png", name);
    unsigned var_na *data = calloc(im.w*im.h*im.c, parimaa_na(var_na));
    ຈຳນວນເຕັມ i,k;
    ສຳລັບ(k = 0; k < im.c; ++k){
        ສຳລັບ(i = 0; i < im.w*im.h; ++i){
            data[i*im.c+k] = (unsigned var_na) (255*im.data[i + k*im.w*im.h]);
        }
    }
    ຈຳນວນເຕັມ success = 0;
    ຖ້າ(f == PNG)       success = stbi_write_png(buff, im.w, im.h, im.c, data, im.w*im.c);
    ບໍ່ດັ່ງນັ້ນ ຖ້າ (f == BMP) success = stbi_write_bmp(buff, im.w, im.h, im.c, data);
    ບໍ່ດັ່ງນັ້ນ ຖ້າ (f == TGA) success = stbi_write_tga(buff, im.w, im.h, im.c, data);
    ບໍ່ດັ່ງນັ້ນ ຖ້າ (f == JPG) success = stbi_write_jpg(buff, im.w, im.h, im.c, data, quality);
    free(data);
    ຖ້າ(!success) fprintf(stderr, "Failed to write image %s\n", buff);
}

void save_image(image im, const var_na *name)
{
    save_image_options(im, name, JPG, 80);
}

void show_image_layers(image p, var_na *name)
{
    ຈຳນວນເຕັມ i;
    var_na buff[256];
    ສຳລັບ(i = 0; i < p.c; ++i){
        sprintf(buff, "%s - Layer %d", name, i);
        image layer = get_image_layer(p, i);
        show_image(layer, buff, 1);
        free_image(layer);
    }
}

void show_image_collapsed(image p, var_na *name)
{
    image c = collapse_image_layers(p, 1);
    show_image(c, name, 1);
    free_image(c);
}

image make_empty_image(ຈຳນວນເຕັມ w, ຈຳນວນເຕັມ h, ຈຳນວນເຕັມ c)
{
    image out;
    out.data = 0;
    out.h = h;
    out.w = w;
    out.c = c;
    ສົ່ງຄືນ out;
}

image make_image(ຈຳນວນເຕັມ w, ຈຳນວນເຕັມ h, ຈຳນວນເຕັມ c)
{
    image out = make_empty_image(w,h,c);
    out.data = calloc(h*w*c, parimaa_na(float));
    ສົ່ງຄືນ out;
}

image make_random_image(ຈຳນວນເຕັມ w, ຈຳນວນເຕັມ h, ຈຳນວນເຕັມ c)
{
    image out = make_empty_image(w,h,c);
    out.data = calloc(h*w*c, parimaa_na(float));
    ຈຳນວນເຕັມ i;
    ສຳລັບ(i = 0; i < w*h*c; ++i){
        out.data[i] = (rand_normal() * .25) + .5;
    }
    ສົ່ງຄືນ out;
}

image float_to_image(ຈຳນວນເຕັມ w, ຈຳນວນເຕັມ h, ຈຳນວນເຕັມ c, float *data)
{
    image out = make_empty_image(w,h,c);
    out.data = data;
    ສົ່ງຄືນ out;
}

void place_image(image im, ຈຳນວນເຕັມ w, ຈຳນວນເຕັມ h, ຈຳນວນເຕັມ dx, ຈຳນວນເຕັມ dy, image canvas)
{
    ຈຳນວນເຕັມ x, y, c;
    ສຳລັບ(c = 0; c < im.c; ++c){
        ສຳລັບ(y = 0; y < h; ++y){
            ສຳລັບ(x = 0; x < w; ++x){
                float rx = ((float)x / w) * im.w;
                float ry = ((float)y / h) * im.h;
                float val = bilinear_interpolate(im, rx, ry, c);
                set_pixel(canvas, x + dx, y + dy, c, val);
            }
        }
    }
}

image center_crop_image(image im, ຈຳນວນເຕັມ w, ຈຳນວນເຕັມ h)
{
    ຈຳນວນເຕັມ m = (im.w < im.h) ? im.w : im.h;   
    image c = crop_image(im, (im.w - m) / 2, (im.h - m)/2, m, m);
    image r = resize_image(c, w, h);
    free_image(c);
    ສົ່ງຄືນ r;
}

image rotate_crop_image(image im, float rad, float s, ຈຳນວນເຕັມ w, ຈຳນວນເຕັມ h, float dx, float dy, float aspect)
{
    ຈຳນວນເຕັມ x, y, c;
    float cx = im.w/2.;
    float cy = im.h/2.;
    image rot = make_image(w, h, im.c);
    ສຳລັບ(c = 0; c < im.c; ++c){
        ສຳລັບ(y = 0; y < h; ++y){
            ສຳລັບ(x = 0; x < w; ++x){
                float rx = cos(rad)*((x - w/2.)/s*aspect + dx/s*aspect) - sin(rad)*((y - h/2.)/s + dy/s) + cx;
                float ry = sin(rad)*((x - w/2.)/s*aspect + dx/s*aspect) + cos(rad)*((y - h/2.)/s + dy/s) + cy;
                float val = bilinear_interpolate(im, rx, ry, c);
                set_pixel(rot, x, y, c, val);
            }
        }
    }
    ສົ່ງຄືນ rot;
}

image rotate_image(image im, float rad)
{
    ຈຳນວນເຕັມ x, y, c;
    float cx = im.w/2.;
    float cy = im.h/2.;
    image rot = make_image(im.w, im.h, im.c);
    ສຳລັບ(c = 0; c < im.c; ++c){
        ສຳລັບ(y = 0; y < im.h; ++y){
            ສຳລັບ(x = 0; x < im.w; ++x){
                float rx = cos(rad)*(x-cx) - sin(rad)*(y-cy) + cx;
                float ry = sin(rad)*(x-cx) + cos(rad)*(y-cy) + cy;
                float val = bilinear_interpolate(im, rx, ry, c);
                set_pixel(rot, x, y, c, val);
            }
        }
    }
    ສົ່ງຄືນ rot;
}

void fill_image(image m, float s)
{
    ຈຳນວນເຕັມ i;
    ສຳລັບ(i = 0; i < m.h*m.w*m.c; ++i) m.data[i] = s;
}

void translate_image(image m, float s)
{
    ຈຳນວນເຕັມ i;
    ສຳລັບ(i = 0; i < m.h*m.w*m.c; ++i) m.data[i] += s;
}

void scale_image(image m, float s)
{
    ຈຳນວນເຕັມ i;
    ສຳລັບ(i = 0; i < m.h*m.w*m.c; ++i) m.data[i] *= s;
}

image crop_image(image im, ຈຳນວນເຕັມ dx, ຈຳນວນເຕັມ dy, ຈຳນວນເຕັມ w, ຈຳນວນເຕັມ h)
{
    image cropped = make_image(w, h, im.c);
    ຈຳນວນເຕັມ i, j, k;
    ສຳລັບ(k = 0; k < im.c; ++k){
        ສຳລັບ(j = 0; j < h; ++j){
            ສຳລັບ(i = 0; i < w; ++i){
                ຈຳນວນເຕັມ r = j + dy;
                ຈຳນວນເຕັມ c = i + dx;
                float val = 0;
                r = constrain_int(r, 0, im.h-1);
                c = constrain_int(c, 0, im.w-1);
                val = get_pixel(im, c, r, k);
                set_pixel(cropped, i, j, k, val);
            }
        }
    }
    ສົ່ງຄືນ cropped;
}

ຈຳນວນເຕັມ best_3d_shift_r(image a, image b, ຈຳນວນເຕັມ min, ຈຳນວນເຕັມ max)
{
    ຖ້າ(min == max) ສົ່ງຄືນ min;
    ຈຳນວນເຕັມ mid = floor((min + max) / 2.);
    image c1 = crop_image(b, 0, mid, b.w, b.h);
    image c2 = crop_image(b, 0, mid+1, b.w, b.h);
    float d1 = dist_array(c1.data, a.data, a.w*a.h*a.c, 10);
    float d2 = dist_array(c2.data, a.data, a.w*a.h*a.c, 10);
    free_image(c1);
    free_image(c2);
    ຖ້າ(d1 < d2) ສົ່ງຄືນ best_3d_shift_r(a, b, min, mid);
    ບໍ່ດັ່ງນັ້ນ ສົ່ງຄືນ best_3d_shift_r(a, b, mid+1, max);
}

ຈຳນວນເຕັມ best_3d_shift(image a, image b, ຈຳນວນເຕັມ min, ຈຳນວນເຕັມ max)
{
    ຈຳນວນເຕັມ i;
    ຈຳນວນເຕັມ best = 0;
    float best_distance = FLT_MAX;
    ສຳລັບ(i = min; i <= max; i += 2){
        image c = crop_image(b, 0, i, b.w, b.h);
        float d = dist_array(c.data, a.data, a.w*a.h*a.c, 100);
        ຖ້າ(d < best_distance){
            best_distance = d;
            best = i;
        }
        printf("%d %f\n", i, d);
        free_image(c);
    }
    ສົ່ງຄືນ best;
}

void composite_3d(var_na *f1, var_na *f2, var_na *out, ຈຳນວນເຕັມ delta)
{
    ຖ້າ(!out) out = "out";
    image a = load_image(f1, 0,0,0);
    image b = load_image(f2, 0,0,0);
    ຈຳນວນເຕັມ shift = best_3d_shift_r(a, b, -a.h/100, a.h/100);

    image c1 = crop_image(b, 10, shift, b.w, b.h);
    float d1 = dist_array(c1.data, a.data, a.w*a.h*a.c, 100);
    image c2 = crop_image(b, -10, shift, b.w, b.h);
    float d2 = dist_array(c2.data, a.data, a.w*a.h*a.c, 100);

    ຖ້າ(d2 < d1 && 0){
        image swap = a;
        a = b;
        b = swap;
        shift = -shift;
        printf("swapped, %d\n", shift);
    }
    ບໍ່ດັ່ງນັ້ນ{
        printf("%d\n", shift);
    }

    image c = crop_image(b, delta, shift, a.w, a.h);
    ຈຳນວນເຕັມ i;
    ສຳລັບ(i = 0; i < c.w*c.h; ++i){
        c.data[i] = a.data[i];
    }
    save_image(c, out);
}

void letterbox_image_into(image im, ຈຳນວນເຕັມ w, ຈຳນວນເຕັມ h, image boxed)
{
    ຈຳນວນເຕັມ new_w = im.w;
    ຈຳນວນເຕັມ new_h = im.h;
    ຖ້າ (((float)w/im.w) < ((float)h/im.h)) {
        new_w = w;
        new_h = (im.h * w)/im.w;
    } ບໍ່ດັ່ງນັ້ນ {
        new_h = h;
        new_w = (im.w * h)/im.h;
    }
    image resized = resize_image(im, new_w, new_h);
    embed_image(resized, boxed, (w-new_w)/2, (h-new_h)/2); 
    free_image(resized);
}

image letterbox_image(image im, ຈຳນວນເຕັມ w, ຈຳນວນເຕັມ h)
{
    ຈຳນວນເຕັມ new_w = im.w;
    ຈຳນວນເຕັມ new_h = im.h;
    ຖ້າ (((float)w/im.w) < ((float)h/im.h)) {
        new_w = w;
        new_h = (im.h * w)/im.w;
    } ບໍ່ດັ່ງນັ້ນ {
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
    ສົ່ງຄືນ boxed;
}

image resize_max(image im, ຈຳນວນເຕັມ max)
{
    ຈຳນວນເຕັມ w = im.w;
    ຈຳນວນເຕັມ h = im.h;
    ຖ້າ(w > h){
        h = (h * max) / w;
        w = max;
    } ບໍ່ດັ່ງນັ້ນ {
        w = (w * max) / h;
        h = max;
    }
    ຖ້າ(w == im.w && h == im.h) ສົ່ງຄືນ im;
    image resized = resize_image(im, w, h);
    ສົ່ງຄືນ resized;
}

image resize_min(image im, ຈຳນວນເຕັມ min)
{
    ຈຳນວນເຕັມ w = im.w;
    ຈຳນວນເຕັມ h = im.h;
    ຖ້າ(w < h){
        h = (h * min) / w;
        w = min;
    } ບໍ່ດັ່ງນັ້ນ {
        w = (w * min) / h;
        h = min;
    }
    ຖ້າ(w == im.w && h == im.h) ສົ່ງຄືນ im;
    image resized = resize_image(im, w, h);
    ສົ່ງຄືນ resized;
}

image random_crop_image(image im, ຈຳນວນເຕັມ w, ຈຳນວນເຕັມ h)
{
    ຈຳນວນເຕັມ dx = rand_int(0, im.w - w);
    ຈຳນວນເຕັມ dy = rand_int(0, im.h - h);
    image crop = crop_image(im, dx, dy, w, h);
    ສົ່ງຄືນ crop;
}

augment_args random_augment_args(image im, float angle, float aspect, ຈຳນວນເຕັມ low, ຈຳນວນເຕັມ high, ຈຳນວນເຕັມ w, ຈຳນວນເຕັມ h)
{
    augment_args a = {0};
    aspect = rand_scale(aspect);
    ຈຳນວນເຕັມ r = rand_int(low, high);
    ຈຳນວນເຕັມ min = (im.h < im.w*aspect) ? im.h : im.w*aspect;
    float scale = (float)r / min;

    float rad = rand_uniform(-angle, angle) * TWO_PI / 360.;

    float dx = (im.w*scale/aspect - w) / 2.;
    float dy = (im.h*scale - w) / 2.;
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
    ສົ່ງຄືນ a;
}

image random_augment_image(image im, float angle, float aspect, ຈຳນວນເຕັມ low, ຈຳນວນເຕັມ high, ຈຳນວນເຕັມ w, ຈຳນວນເຕັມ h)
{
    augment_args a = random_augment_args(im, angle, aspect, low, high, w, h);
    image crop = rotate_crop_image(im, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);
    ສົ່ງຄືນ crop;
}

float three_way_max(float a, float b, float c)
{
    ສົ່ງຄືນ (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

float three_way_min(float a, float b, float c)
{
    ສົ່ງຄືນ (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

void yuv_to_rgb(image im)
{
    assert(im.c == 3);
    ຈຳນວນເຕັມ i, j;
    float r, g, b;
    float y, u, v;
    ສຳລັບ(j = 0; j < im.h; ++j){
        ສຳລັບ(i = 0; i < im.w; ++i){
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

void rgb_to_yuv(image im)
{
    assert(im.c == 3);
    ຈຳນວນເຕັມ i, j;
    float r, g, b;
    float y, u, v;
    ສຳລັບ(j = 0; j < im.h; ++j){
        ສຳລັບ(i = 0; i < im.w; ++i){
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
void rgb_to_hsv(image im)
{
    assert(im.c == 3);
    ຈຳນວນເຕັມ i, j;
    float r, g, b;
    float h, s, v;
    ສຳລັບ(j = 0; j < im.h; ++j){
        ສຳລັບ(i = 0; i < im.w; ++i){
            r = get_pixel(im, i , j, 0);
            g = get_pixel(im, i , j, 1);
            b = get_pixel(im, i , j, 2);
            float max = three_way_max(r,g,b);
            float min = three_way_min(r,g,b);
            float delta = max - min;
            v = max;
            ຖ້າ(max == 0){
                s = 0;
                h = 0;
            }ບໍ່ດັ່ງນັ້ນ{
                s = delta/max;
                ຖ້າ(r == max){
                    h = (g - b) / delta;
                } ບໍ່ດັ່ງນັ້ນ ຖ້າ (g == max) {
                    h = 2 + (b - r) / delta;
                } ບໍ່ດັ່ງນັ້ນ {
                    h = 4 + (r - g) / delta;
                }
                ຖ້າ (h < 0) h += 6;
                h = h/6.;
            }
            set_pixel(im, i, j, 0, h);
            set_pixel(im, i, j, 1, s);
            set_pixel(im, i, j, 2, v);
        }
    }
}

void hsv_to_rgb(image im)
{
    assert(im.c == 3);
    ຈຳນວນເຕັມ i, j;
    float r, g, b;
    float h, s, v;
    float f, p, q, t;
    ສຳລັບ(j = 0; j < im.h; ++j){
        ສຳລັບ(i = 0; i < im.w; ++i){
            h = 6 * get_pixel(im, i , j, 0);
            s = get_pixel(im, i , j, 1);
            v = get_pixel(im, i , j, 2);
            ຖ້າ (s == 0) {
                r = g = b = v;
            } ບໍ່ດັ່ງນັ້ນ {
                ຈຳນວນເຕັມ index = floor(h);
                f = h - index;
                p = v*(1-s);
                q = v*(1-s*f);
                t = v*(1-s*(1-f));
                ຖ້າ(index == 0){
                    r = v; g = t; b = p;
                } ບໍ່ດັ່ງນັ້ນ ຖ້າ(index == 1){
                    r = q; g = v; b = p;
                } ບໍ່ດັ່ງນັ້ນ ຖ້າ(index == 2){
                    r = p; g = v; b = t;
                } ບໍ່ດັ່ງນັ້ນ ຖ້າ(index == 3){
                    r = p; g = q; b = v;
                } ບໍ່ດັ່ງນັ້ນ ຖ້າ(index == 4){
                    r = t; g = p; b = v;
                } ບໍ່ດັ່ງນັ້ນ {
                    r = v; g = p; b = q;
                }
            }
            set_pixel(im, i, j, 0, r);
            set_pixel(im, i, j, 1, g);
            set_pixel(im, i, j, 2, b);
        }
    }
}

void grayscale_image_3c(image im)
{
    assert(im.c == 3);
    ຈຳນວນເຕັມ i, j, k;
    float scale[] = {0.299, 0.587, 0.114};
    ສຳລັບ(j = 0; j < im.h; ++j){
        ສຳລັບ(i = 0; i < im.w; ++i){
            float val = 0;
            ສຳລັບ(k = 0; k < 3; ++k){
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
    assert(im.c == 3);
    ຈຳນວນເຕັມ i, j, k;
    image gray = make_image(im.w, im.h, 1);
    float scale[] = {0.299, 0.587, 0.114};
    ສຳລັບ(k = 0; k < im.c; ++k){
        ສຳລັບ(j = 0; j < im.h; ++j){
            ສຳລັບ(i = 0; i < im.w; ++i){
                gray.data[i+im.w*j] += scale[k]*get_pixel(im, i, j, k);
            }
        }
    }
    ສົ່ງຄືນ gray;
}

image threshold_image(image im, float thresh)
{
    ຈຳນວນເຕັມ i;
    image t = make_image(im.w, im.h, im.c);
    ສຳລັບ(i = 0; i < im.w*im.h*im.c; ++i){
        t.data[i] = im.data[i]>thresh ? 1 : 0;
    }
    ສົ່ງຄືນ t;
}

image blend_image(image fore, image back, float alpha)
{
    assert(fore.w == back.w && fore.h == back.h && fore.c == back.c);
    image blend = make_image(fore.w, fore.h, fore.c);
    ຈຳນວນເຕັມ i, j, k;
    ສຳລັບ(k = 0; k < fore.c; ++k){
        ສຳລັບ(j = 0; j < fore.h; ++j){
            ສຳລັບ(i = 0; i < fore.w; ++i){
                float val = alpha * get_pixel(fore, i, j, k) + 
                    (1 - alpha)* get_pixel(back, i, j, k);
                set_pixel(blend, i, j, k, val);
            }
        }
    }
    ສົ່ງຄືນ blend;
}

void scale_image_channel(image im, ຈຳນວນເຕັມ c, float v)
{
    ຈຳນວນເຕັມ i, j;
    ສຳລັບ(j = 0; j < im.h; ++j){
        ສຳລັບ(i = 0; i < im.w; ++i){
            float pix = get_pixel(im, i, j, c);
            pix = pix*v;
            set_pixel(im, i, j, c, pix);
        }
    }
}

void translate_image_channel(image im, ຈຳນວນເຕັມ c, float v)
{
    ຈຳນວນເຕັມ i, j;
    ສຳລັບ(j = 0; j < im.h; ++j){
        ສຳລັບ(i = 0; i < im.w; ++i){
            float pix = get_pixel(im, i, j, c);
            pix = pix+v;
            set_pixel(im, i, j, c, pix);
        }
    }
}

image binarize_image(image im)
{
    image c = copy_image(im);
    ຈຳນວນເຕັມ i;
    ສຳລັບ(i = 0; i < im.w * im.h * im.c; ++i){
        ຖ້າ(c.data[i] > .5) c.data[i] = 1;
        ບໍ່ດັ່ງນັ້ນ c.data[i] = 0;
    }
    ສົ່ງຄືນ c;
}

void saturate_image(image im, float sat)
{
    rgb_to_hsv(im);
    scale_image_channel(im, 1, sat);
    hsv_to_rgb(im);
    constrain_image(im);
}

void hue_image(image im, float hue)
{
    rgb_to_hsv(im);
    ຈຳນວນເຕັມ i;
    ສຳລັບ(i = 0; i < im.w*im.h; ++i){
        im.data[i] = im.data[i] + hue;
        ຖ້າ (im.data[i] > 1) im.data[i] -= 1;
        ຖ້າ (im.data[i] < 0) im.data[i] += 1;
    }
    hsv_to_rgb(im);
    constrain_image(im);
}

void exposure_image(image im, float sat)
{
    rgb_to_hsv(im);
    scale_image_channel(im, 2, sat);
    hsv_to_rgb(im);
    constrain_image(im);
}

void distort_image(image im, float hue, float sat, float val)
{
    rgb_to_hsv(im);
    scale_image_channel(im, 1, sat);
    scale_image_channel(im, 2, val);
    ຈຳນວນເຕັມ i;
    ສຳລັບ(i = 0; i < im.w*im.h; ++i){
        im.data[i] = im.data[i] + hue;
        ຖ້າ (im.data[i] > 1) im.data[i] -= 1;
        ຖ້າ (im.data[i] < 0) im.data[i] += 1;
    }
    hsv_to_rgb(im);
    constrain_image(im);
}

void random_distort_image(image im, float hue, float saturation, float exposure)
{
    float dhue = rand_uniform(-hue, hue);
    float dsat = rand_scale(saturation);
    float dexp = rand_scale(exposure);
    distort_image(im, dhue, dsat, dexp);
}

void saturate_exposure_image(image im, float sat, float exposure)
{
    rgb_to_hsv(im);
    scale_image_channel(im, 1, sat);
    scale_image_channel(im, 2, exposure);
    hsv_to_rgb(im);
    constrain_image(im);
}

image resize_image(image im, ຈຳນວນເຕັມ w, ຈຳນວນເຕັມ h)
{
    image resized = make_image(w, h, im.c);   
    image part = make_image(w, im.h, im.c);
    ຈຳນວນເຕັມ r, c, k;
    float w_scale = (float)(im.w - 1) / (w - 1);
    float h_scale = (float)(im.h - 1) / (h - 1);
    ສຳລັບ(k = 0; k < im.c; ++k){
        ສຳລັບ(r = 0; r < im.h; ++r){
            ສຳລັບ(c = 0; c < w; ++c){
                float val = 0;
                ຖ້າ(c == w-1 || im.w == 1){
                    val = get_pixel(im, im.w-1, r, k);
                } ບໍ່ດັ່ງນັ້ນ {
                    float sx = c*w_scale;
                    ຈຳນວນເຕັມ ix = (ຈຳນວນເຕັມ) sx;
                    float dx = sx - ix;
                    val = (1 - dx) * get_pixel(im, ix, r, k) + dx * get_pixel(im, ix+1, r, k);
                }
                set_pixel(part, c, r, k, val);
            }
        }
    }
    ສຳລັບ(k = 0; k < im.c; ++k){
        ສຳລັບ(r = 0; r < h; ++r){
            float sy = r*h_scale;
            ຈຳນວນເຕັມ iy = (ຈຳນວນເຕັມ) sy;
            float dy = sy - iy;
            ສຳລັບ(c = 0; c < w; ++c){
                float val = (1-dy) * get_pixel(part, c, iy, k);
                set_pixel(resized, c, r, k, val);
            }
            ຖ້າ(r == h-1 || im.h == 1) continue;
            ສຳລັບ(c = 0; c < w; ++c){
                float val = dy * get_pixel(part, c, iy+1, k);
                add_pixel(resized, c, r, k, val);
            }
        }
    }

    free_image(part);
    ສົ່ງຄືນ resized;
}


void test_resize(var_na *filename)
{
    image im = load_image(filename, 0,0, 3);
    float mag = mag_array(im.data, im.w*im.h*im.c);
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
    ໃນຂະນະທີ່(1){
        image aug = random_augment_image(im, 0, .75, 320, 448, 320, 320);
        show_image(aug, "aug", 1);
        free_image(aug);


        float exposure = 1.15;
        float saturation = 1.15;
        float hue = .05;

        image c = copy_image(im);

        float dexp = rand_scale(exposure);
        float dsat = rand_scale(saturation);
        float dhue = rand_uniform(-hue, hue);

        distort_image(c, dhue, dsat, dexp);
        show_image(c, "rand", 1);
        printf("%f %f %f\n", dhue, dsat, dexp);
        free_image(c);
    }
#endif
}


image load_image_stb(var_na *filename, ຈຳນວນເຕັມ channels)
{
    ຈຳນວນເຕັມ w, h, c;
    unsigned var_na *data = stbi_load(filename, &w, &h, &c, channels);
    ຖ້າ (!data) {
        fprintf(stderr, "Cannot load image \"%s\"\nSTB Reason: %s\n", filename, stbi_failure_reason());
        exit(0);
    }
    ຖ້າ(channels) c = channels;
    ຈຳນວນເຕັມ i,j,k;
    image im = make_image(w, h, c);
    ສຳລັບ(k = 0; k < c; ++k){
        ສຳລັບ(j = 0; j < h; ++j){
            ສຳລັບ(i = 0; i < w; ++i){
                ຈຳນວນເຕັມ dst_index = i + w*j + w*h*k;
                ຈຳນວນເຕັມ src_index = k + c*i + c*w*j;
                im.data[dst_index] = (float)data[src_index]/255.;
            }
        }
    }
    free(data);
    ສົ່ງຄືນ im;
}

image load_image(var_na *filename, ຈຳນວນເຕັມ w, ຈຳນວນເຕັມ h, ຈຳນວນເຕັມ c)
{
#ifdef OPENCV
    image out = load_image_cv(filename, c);
#ບໍ່ດັ່ງນັ້ນ
    image out = load_image_stb(filename, c);
#endif

    ຖ້າ((h && w) && (h != out.h || w != out.w)){
        image resized = resize_image(out, w, h);
        free_image(out);
        out = resized;
    }
    ສົ່ງຄືນ out;
}

image load_image_color(var_na *filename, ຈຳນວນເຕັມ w, ຈຳນວນເຕັມ h)
{
    ສົ່ງຄືນ load_image(filename, w, h, 3);
}

image get_image_layer(image m, ຈຳນວນເຕັມ l)
{
    image out = make_image(m.w, m.h, 1);
    ຈຳນວນເຕັມ i;
    ສຳລັບ(i = 0; i < m.h*m.w; ++i){
        out.data[i] = m.data[i+l*m.h*m.w];
    }
    ສົ່ງຄືນ out;
}
void print_image(image m)
{
    ຈຳນວນເຕັມ i, j, k;
    ສຳລັບ(i =0 ; i < m.c; ++i){
        ສຳລັບ(j =0 ; j < m.h; ++j){
            ສຳລັບ(k = 0; k < m.w; ++k){
                printf("%.2lf, ", m.data[i*m.h*m.w + j*m.w + k]);
                ຖ້າ(k > 30) bha_nga;
            }
            printf("\n");
            ຖ້າ(j > 30) bha_nga;
        }
        printf("\n");
    }
    printf("\n");
}

image collapse_images_vert(image *ims, ຈຳນວນເຕັມ n)
{
    ຈຳນວນເຕັມ color = 1;
    ຈຳນວນເຕັມ border = 1;
    ຈຳນວນເຕັມ h,w,c;
    w = ims[0].w;
    h = (ims[0].h + border) * n - border;
    c = ims[0].c;
    ຖ້າ(c != 3 || !color){
        w = (w+border)*c - border;
        c = 1;
    }

    image filters = make_image(w, h, c);
    ຈຳນວນເຕັມ i,j;
    ສຳລັບ(i = 0; i < n; ++i){
        ຈຳນວນເຕັມ h_offset = i*(ims[0].h+border);
        image copy = copy_image(ims[i]);
        //normalize_image(copy);
        ຖ້າ(c == 3 && color){
            embed_image(copy, filters, 0, h_offset);
        }
        ບໍ່ດັ່ງນັ້ນ{
            ສຳລັບ(j = 0; j < copy.c; ++j){
                ຈຳນວນເຕັມ w_offset = j*(ims[0].w+border);
                image layer = get_image_layer(copy, j);
                embed_image(layer, filters, w_offset, h_offset);
                free_image(layer);
            }
        }
        free_image(copy);
    }
    ສົ່ງຄືນ filters;
} 

image collapse_images_horz(image *ims, ຈຳນວນເຕັມ n)
{
    ຈຳນວນເຕັມ color = 1;
    ຈຳນວນເຕັມ border = 1;
    ຈຳນວນເຕັມ h,w,c;
    ຈຳນວນເຕັມ size = ims[0].h;
    h = size;
    w = (ims[0].w + border) * n - border;
    c = ims[0].c;
    ຖ້າ(c != 3 || !color){
        h = (h+border)*c - border;
        c = 1;
    }

    image filters = make_image(w, h, c);
    ຈຳນວນເຕັມ i,j;
    ສຳລັບ(i = 0; i < n; ++i){
        ຈຳນວນເຕັມ w_offset = i*(size+border);
        image copy = copy_image(ims[i]);
        //normalize_image(copy);
        ຖ້າ(c == 3 && color){
            embed_image(copy, filters, w_offset, 0);
        }
        ບໍ່ດັ່ງນັ້ນ{
            ສຳລັບ(j = 0; j < copy.c; ++j){
                ຈຳນວນເຕັມ h_offset = j*(size+border);
                image layer = get_image_layer(copy, j);
                embed_image(layer, filters, w_offset, h_offset);
                free_image(layer);
            }
        }
        free_image(copy);
    }
    ສົ່ງຄືນ filters;
} 

void show_image_normalized(image im, const var_na *name)
{
    image c = copy_image(im);
    normalize_image(c);
    show_image(c, name, 1);
    free_image(c);
}

void show_images(image *ims, ຈຳນວນເຕັມ n, var_na *window)
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

void free_image(image m)
{
    ຖ້າ(m.data){
        free(m.data);
    }
}
