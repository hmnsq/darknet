#ಅನ್ತರ್ಭಾವ "image.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "blas.h"
#ಅನ್ತರ್ಭಾವ "cuda.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <math.h>

#ವ್ಯಾಖ್ಯಾನಿಸು STB_IMAGE_IMPLEMENTATION
#ಅನ್ತರ್ಭಾವ "stb_image.h"
#ವ್ಯಾಖ್ಯಾನಿಸು STB_IMAGE_WRITE_IMPLEMENTATION
#ಅನ್ತರ್ಭಾವ "stb_image_write.h"

ಪೂರ್ಣಾಂಕ windows = 0;

ದಶಮಾಂಶ colors[6][3] = { {1,0,1}, {0,0,1},{0,1,1},{0,1,0},{1,1,0},{1,0,0} };

ದಶಮಾಂಶ get_color(ಪೂರ್ಣಾಂಕ c, ಪೂರ್ಣಾಂಕ x, ಪೂರ್ಣಾಂಕ max)
{
    ದಶಮಾಂಶ ಅನುಪಾತ = ((ದಶಮಾಂಶ)x/max)*5;
    ಪೂರ್ಣಾಂಕ i = floor(ಅನುಪಾತ);
    ಪೂರ್ಣಾಂಕ j = ceil(ಅನುಪಾತ);
    ಅನುಪಾತ -= i;
    ದಶಮಾಂಶ r = (1-ಅನುಪಾತ) * colors[i][c] + ಅನುಪಾತ*colors[j][c];
    //printf("%f\n", r);
    ಹಿಂತಿರುಗಿ r;
}

image ಮುಖವಾಡ_ಗಮ್ಯ_rgb(image ಮುಖವಾಡ)
{
    ಪೂರ್ಣಾಂಕ n = ಮುಖವಾಡ.c;
    image im = make_image(ಮುಖವಾಡ.w, ಮುಖವಾಡ.h, 3);
    ಪೂರ್ಣಾಂಕ i, j;
    ಗಾಗಿ(j = 0; j < n; ++j){
        ಪೂರ್ಣಾಂಕ ಸ್ಥಾನಾಂತರ = j*123457 % n;
        ದಶಮಾಂಶ red = get_color(2,ಸ್ಥಾನಾಂತರ,n);
        ದಶಮಾಂಶ green = get_color(1,ಸ್ಥಾನಾಂತರ,n);
        ದಶಮಾಂಶ blue = get_color(0,ಸ್ಥಾನಾಂತರ,n);
        ಗಾಗಿ(i = 0; i < im.w*im.h; ++i){
            im.ದತ್ತಾಂಶ[i + 0*im.w*im.h] += ಮುಖವಾಡ.ದತ್ತಾಂಶ[j*im.h*im.w + i]*red;
            im.ದತ್ತಾಂಶ[i + 1*im.w*im.h] += ಮುಖವಾಡ.ದತ್ತಾಂಶ[j*im.h*im.w + i]*green;
            im.ದತ್ತಾಂಶ[i + 2*im.w*im.h] += ಮುಖವಾಡ.ದತ್ತಾಂಶ[j*im.h*im.w + i]*blue;
        }
    }
    ಹಿಂತಿರುಗಿ im;
}

ಸ್ಥಿರ ದಶಮಾಂಶ get_pixel(image m, ಪೂರ್ಣಾಂಕ x, ಪೂರ್ಣಾಂಕ y, ಪೂರ್ಣಾಂಕ c)
{
    ಪ್ರತಿಜ್ಞಾ(x < m.w && y < m.h && c < m.c);
    ಹಿಂತಿರುಗಿ m.ದತ್ತಾಂಶ[c*m.h*m.w + y*m.w + x];
}
ಸ್ಥಿರ ದಶಮಾಂಶ get_pixel_extend(image m, ಪೂರ್ಣಾಂಕ x, ಪೂರ್ಣಾಂಕ y, ಪೂರ್ಣಾಂಕ c)
{
    ಆದರೆ(x < 0 || x >= m.w || y < 0 || y >= m.h) ಹಿಂತಿರುಗಿ 0;
    /*
    if(x < 0) x = 0;
    if(x >= m.w) x = m.w-1;
    if(y < 0) y = 0;
    if(y >= m.h) y = m.h-1;
    */
    ಆದರೆ(c < 0 || c >= m.c) ಹಿಂತಿರುಗಿ 0;
    ಹಿಂತಿರುಗಿ get_pixel(m, x, y, c);
}
ಸ್ಥಿರ ಶೂನ್ಯ ಗಣ_pixel(image m, ಪೂರ್ಣಾಂಕ x, ಪೂರ್ಣಾಂಕ y, ಪೂರ್ಣಾಂಕ c, ದಶಮಾಂಶ val)
{
    ಆದರೆ (x < 0 || y < 0 || c < 0 || x >= m.w || y >= m.h || c >= m.c) ಹಿಂತಿರುಗಿ;
    ಪ್ರತಿಜ್ಞಾ(x < m.w && y < m.h && c < m.c);
    m.ದತ್ತಾಂಶ[c*m.h*m.w + y*m.w + x] = val;
}
ಸ್ಥಿರ ಶೂನ್ಯ ಕೂಡಿಸು_pixel(image m, ಪೂರ್ಣಾಂಕ x, ಪೂರ್ಣಾಂಕ y, ಪೂರ್ಣಾಂಕ c, ದಶಮಾಂಶ val)
{
    ಪ್ರತಿಜ್ಞಾ(x < m.w && y < m.h && c < m.c);
    m.ದತ್ತಾಂಶ[c*m.h*m.w + y*m.w + x] += val;
}

ಸ್ಥಿರ ದಶಮಾಂಶ bilinear_interpolate(image im, ದಶಮಾಂಶ x, ದಶಮಾಂಶ y, ಪೂರ್ಣಾಂಕ c)
{
    ಪೂರ್ಣಾಂಕ ix = (ಪೂರ್ಣಾಂಕ) floorf(x);
    ಪೂರ್ಣಾಂಕ iy = (ಪೂರ್ಣಾಂಕ) floorf(y);

    ದಶಮಾಂಶ dx = x - ix;
    ದಶಮಾಂಶ dy = y - iy;

    ದಶಮಾಂಶ val = (1-dy) * (1-dx) * get_pixel_extend(im, ix, iy, c) + 
        dy     * (1-dx) * get_pixel_extend(im, ix, iy+1, c) + 
        (1-dy) *   dx   * get_pixel_extend(im, ix+1, iy, c) +
        dy     *   dx   * get_pixel_extend(im, ix+1, iy+1, c);
    ಹಿಂತಿರುಗಿ val;
}


ಶೂನ್ಯ ಸಂಯುಕ್ತ_image(image ಮೂಲ, image dest, ಪೂರ್ಣಾಂಕ dx, ಪೂರ್ಣಾಂಕ dy)
{
    ಪೂರ್ಣಾಂಕ x,y,k;
    ಗಾಗಿ(k = 0; k < ಮೂಲ.c; ++k){
        ಗಾಗಿ(y = 0; y < ಮೂಲ.h; ++y){
            ಗಾಗಿ(x = 0; x < ಮೂಲ.w; ++x){
                ದಶಮಾಂಶ val = get_pixel(ಮೂಲ, x, y, k);
                ದಶಮಾಂಶ val2 = get_pixel_extend(dest, dx+x, dy+y, k);
                ಗಣ_pixel(dest, dx+x, dy+y, k, val * val2);
            }
        }
    }
}

image border_image(image a, ಪೂರ್ಣಾಂಕ border)
{
    image b = make_image(a.w + 2*border, a.h + 2*border, a.c);
    ಪೂರ್ಣಾಂಕ x,y,k;
    ಗಾಗಿ(k = 0; k < b.c; ++k){
        ಗಾಗಿ(y = 0; y < b.h; ++y){
            ಗಾಗಿ(x = 0; x < b.w; ++x){
                ದಶಮಾಂಶ val = get_pixel_extend(a, x - border, y - border, k);
                ಆದರೆ(x - border < 0 || x - border >= a.w || y - border < 0 || y - border >= a.h) val = 1;
                ಗಣ_pixel(b, x, y, k, val);
            }
        }
    }
    ಹಿಂತಿರುಗಿ b;
}

image tile_images(image a, image b, ಪೂರ್ಣಾಂಕ dx)
{
    ಆದರೆ(a.w == 0) ಹಿಂತಿರುಗಿ copy_image(b);
    image c = make_image(a.w + b.w + dx, (a.h > b.h) ? a.h : b.h, (a.c > b.c) ? a.c : b.c);
    fill_cpu(c.w*c.h*c.c, 1, c.ದತ್ತಾಂಶ, 1);
    embed_image(a, c, 0, 0); 
    ಸಂಯುಕ್ತ_image(b, c, a.w + dx, 0);
    ಹಿಂತಿರುಗಿ c;
}

image get_label(image **characters, ಅಕ್ಷರ *string, ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2)
{
    ಗಾತ್ರ_2 = ಗಾತ್ರ_2/10;
    ಆದರೆ(ಗಾತ್ರ_2 > 7) ಗಾತ್ರ_2 = 7;
    image label = make_ಖಾಲಿಗಳು_image(0,0,0);
    ಆಗ(*string){
        image l = characters[ಗಾತ್ರ_2][(ಪೂರ್ಣಾಂಕ)*string];
        image n = tile_images(label, l, -ಗಾತ್ರ_2 - 1 + (ಗಾತ್ರ_2+1)/2);
        free_image(label);
        label = n;
        ++string;
    }
    image b = border_image(label, label.h*.25);
    free_image(label);
    ಹಿಂತಿರುಗಿ b;
}

ಶೂನ್ಯ draw_label(image a, ಪೂರ್ಣಾಂಕ r, ಪೂರ್ಣಾಂಕ c, image label, ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ *rgb)
{
    ಪೂರ್ಣಾಂಕ w = label.w;
    ಪೂರ್ಣಾಂಕ h = label.h;
    ಆದರೆ (r - h >= 0) r = r - h;

    ಪೂರ್ಣಾಂಕ i, j, k;
    ಗಾಗಿ(j = 0; j < h && j + r < a.h; ++j){
        ಗಾಗಿ(i = 0; i < w && i + c < a.w; ++i){
            ಗಾಗಿ(k = 0; k < label.c; ++k){
                ದಶಮಾಂಶ val = get_pixel(label, i, j, k);
                ಗಣ_pixel(a, i+c, j+r, k, rgb[k] * val);
            }
        }
    }
}

ಶೂನ್ಯ draw_box(image a, ಪೂರ್ಣಾಂಕ x1, ಪೂರ್ಣಾಂಕ y1, ಪೂರ್ಣಾಂಕ x2, ಪೂರ್ಣಾಂಕ y2, ದಶಮಾಂಶ r, ದಶಮಾಂಶ g, ದಶಮಾಂಶ b)
{
    //normalize_image(a);
    ಪೂರ್ಣಾಂಕ i;
    ಆದರೆ(x1 < 0) x1 = 0;
    ಆದರೆ(x1 >= a.w) x1 = a.w-1;
    ಆದರೆ(x2 < 0) x2 = 0;
    ಆದರೆ(x2 >= a.w) x2 = a.w-1;

    ಆದರೆ(y1 < 0) y1 = 0;
    ಆದರೆ(y1 >= a.h) y1 = a.h-1;
    ಆದರೆ(y2 < 0) y2 = 0;
    ಆದರೆ(y2 >= a.h) y2 = a.h-1;

    ಗಾಗಿ(i = x1; i <= x2; ++i){
        a.ದತ್ತಾಂಶ[i + y1*a.w + 0*a.w*a.h] = r;
        a.ದತ್ತಾಂಶ[i + y2*a.w + 0*a.w*a.h] = r;

        a.ದತ್ತಾಂಶ[i + y1*a.w + 1*a.w*a.h] = g;
        a.ದತ್ತಾಂಶ[i + y2*a.w + 1*a.w*a.h] = g;

        a.ದತ್ತಾಂಶ[i + y1*a.w + 2*a.w*a.h] = b;
        a.ದತ್ತಾಂಶ[i + y2*a.w + 2*a.w*a.h] = b;
    }
    ಗಾಗಿ(i = y1; i <= y2; ++i){
        a.ದತ್ತಾಂಶ[x1 + i*a.w + 0*a.w*a.h] = r;
        a.ದತ್ತಾಂಶ[x2 + i*a.w + 0*a.w*a.h] = r;

        a.ದತ್ತಾಂಶ[x1 + i*a.w + 1*a.w*a.h] = g;
        a.ದತ್ತಾಂಶ[x2 + i*a.w + 1*a.w*a.h] = g;

        a.ದತ್ತಾಂಶ[x1 + i*a.w + 2*a.w*a.h] = b;
        a.ದತ್ತಾಂಶ[x2 + i*a.w + 2*a.w*a.h] = b;
    }
}

ಶೂನ್ಯ draw_box_ಅಗಲ(image a, ಪೂರ್ಣಾಂಕ x1, ಪೂರ್ಣಾಂಕ y1, ಪೂರ್ಣಾಂಕ x2, ಪೂರ್ಣಾಂಕ y2, ಪೂರ್ಣಾಂಕ w, ದಶಮಾಂಶ r, ದಶಮಾಂಶ g, ದಶಮಾಂಶ b)
{
    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ(i = 0; i < w; ++i){
        draw_box(a, x1+i, y1+i, x2-i, y2-i, r, g, b);
    }
}

ಶೂನ್ಯ draw_bbox(image a, box bbox, ಪೂರ್ಣಾಂಕ w, ದಶಮಾಂಶ r, ದಶಮಾಂಶ g, ದಶಮಾಂಶ b)
{
    ಪೂರ್ಣಾಂಕ ಎಡ  = (bbox.x-bbox.w/2)*a.w;
    ಪೂರ್ಣಾಂಕ ಬಲ = (bbox.x+bbox.w/2)*a.w;
    ಪೂರ್ಣಾಂಕ ತುದಿ   = (bbox.y-bbox.h/2)*a.h;
    ಪೂರ್ಣಾಂಕ bot   = (bbox.y+bbox.h/2)*a.h;

    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ(i = 0; i < w; ++i){
        draw_box(a, ಎಡ+i, ತುದಿ+i, ಬಲ-i, bot-i, r, g, b);
    }
}

image **ತುಂಬು_alphabet()
{
    ಪೂರ್ಣಾಂಕ i, j;
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ nsize = 8;
    image **alphabets = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(nsize, ಗಾತ್ರ(image));
    ಗಾಗಿ(j = 0; j < nsize; ++j){
        alphabets[j] = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(128, ಗಾತ್ರ(image));
        ಗಾಗಿ(i = 32; i < 127; ++i){
            ಅಕ್ಷರ buff[256];
            sprintf(buff, "data/labels/%d_%d.png", i, j);
            alphabets[j][i] = ತುಂಬು_image_color(buff, 0, 0);
        }
    }
    ಹಿಂತಿರುಗಿ alphabets;
}

ಶೂನ್ಯ draw_detections(image im, detection *dets, ಪೂರ್ಣಾಂಕ ಅಂಕೆ, ದಶಮಾಂಶ thresh, ಅಕ್ಷರ **ಹೆಸರುಗಳು, image **alphabet, ಪೂರ್ಣಾಂಕ classes)
{
    ಪೂರ್ಣಾಂಕ i,j;

    ಗಾಗಿ(i = 0; i < ಅಂಕೆ; ++i){
        ಅಕ್ಷರ labelstr[4096] = {0};
        ಪೂರ್ಣಾಂಕ class = -1;
        ಗಾಗಿ(j = 0; j < classes; ++j){
            ಆದರೆ (dets[i].prob[j] > thresh){
                ಆದರೆ (class < 0) {
                    strcat(labelstr, ಹೆಸರುಗಳು[j]);
                    class = j;
                } ಇಲ್ಲದಿದ್ದರೆ {
                    strcat(labelstr, ", ");
                    strcat(labelstr, ಹೆಸರುಗಳು[j]);
                }
                ಬರೆ("%s: %.0f%%\n", ಹೆಸರುಗಳು[j], dets[i].prob[j]*100);
            }
        }
        ಆದರೆ(class >= 0){
            ಪೂರ್ಣಾಂಕ ಅಗಲ = im.h * .006;

            /*
               if(0){
               width = pow(prob, 1./2.)*10+1;
               alphabet = 0;
               }
             */

            //printf("%d %s: %.0f%%\n", i, names[class], prob*100);
            ಪೂರ್ಣಾಂಕ ಸ್ಥಾನಾಂತರ = class*123457 % classes;
            ದಶಮಾಂಶ red = get_color(2,ಸ್ಥಾನಾಂತರ,classes);
            ದಶಮಾಂಶ green = get_color(1,ಸ್ಥಾನಾಂತರ,classes);
            ದಶಮಾಂಶ blue = get_color(0,ಸ್ಥಾನಾಂತರ,classes);
            ದಶಮಾಂಶ rgb[3];

            //width = prob*20+2;

            rgb[0] = red;
            rgb[1] = green;
            rgb[2] = blue;
            box b = dets[i].bbox;
            //printf("%f %f %f %f\n", b.x, b.y, b.w, b.h);

            ಪೂರ್ಣಾಂಕ ಎಡ  = (b.x-b.w/2.)*im.w;
            ಪೂರ್ಣಾಂಕ ಬಲ = (b.x+b.w/2.)*im.w;
            ಪೂರ್ಣಾಂಕ ತುದಿ   = (b.y-b.h/2.)*im.h;
            ಪೂರ್ಣಾಂಕ bot   = (b.y+b.h/2.)*im.h;

            ಆದರೆ(ಎಡ < 0) ಎಡ = 0;
            ಆದರೆ(ಬಲ > im.w-1) ಬಲ = im.w-1;
            ಆದರೆ(ತುದಿ < 0) ತುದಿ = 0;
            ಆದರೆ(bot > im.h-1) bot = im.h-1;

            draw_box_ಅಗಲ(im, ಎಡ, ತುದಿ, ಬಲ, bot, ಅಗಲ, red, green, blue);
            ಆದರೆ (alphabet) {
                image label = get_label(alphabet, labelstr, (im.h*.03));
                draw_label(im, ತುದಿ + ಅಗಲ, ಎಡ, label, rgb);
                free_image(label);
            }
            ಆದರೆ (dets[i].ಮುಖವಾಡ){
                image ಮುಖವಾಡ = float_ಗಮ್ಯ_image(14, 14, 1, dets[i].ಮುಖವಾಡ);
                image resized_ಮುಖವಾಡ = resize_image(ಮುಖವಾಡ, b.w*im.w, b.h*im.h);
                image tmask = ಮಿತಿ_image(resized_ಮುಖವಾಡ, .5);
                embed_image(tmask, im, ಎಡ, ತುದಿ);
                free_image(ಮುಖವಾಡ);
                free_image(resized_ಮುಖವಾಡ);
                free_image(tmask);
            }
        }
    }
}

ಶೂನ್ಯ transpose_image(image im)
{
    ಪ್ರತಿಜ್ಞಾ(im.w == im.h);
    ಪೂರ್ಣಾಂಕ n, m;
    ಪೂರ್ಣಾಂಕ c;
    ಗಾಗಿ(c = 0; c < im.c; ++c){
        ಗಾಗಿ(n = 0; n < im.w-1; ++n){
            ಗಾಗಿ(m = n + 1; m < im.w; ++m){
                ದಶಮಾಂಶ ಅದಲುಬದಲು = im.ದತ್ತಾಂಶ[m + im.w*(n + im.h*c)];
                im.ದತ್ತಾಂಶ[m + im.w*(n + im.h*c)] = im.ದತ್ತಾಂಶ[n + im.w*(m + im.h*c)];
                im.ದತ್ತಾಂಶ[n + im.w*(m + im.h*c)] = ಅದಲುಬದಲು;
            }
        }
    }
}

ಶೂನ್ಯ rotate_image_cw(image im, ಪೂರ್ಣಾಂಕ times)
{
    ಪ್ರತಿಜ್ಞಾ(im.w == im.h);
    times = (times + 400) % 4;
    ಪೂರ್ಣಾಂಕ i, x, y, c;
    ಪೂರ್ಣಾಂಕ n = im.w;
    ಗಾಗಿ(i = 0; i < times; ++i){
        ಗಾಗಿ(c = 0; c < im.c; ++c){
            ಗಾಗಿ(x = 0; x < n/2; ++x){
                ಗಾಗಿ(y = 0; y < (n-1)/2 + 1; ++y){
                    ದಶಮಾಂಶ ತಾತ್ಕಾಲಿಕ = im.ದತ್ತಾಂಶ[y + im.w*(x + im.h*c)];
                    im.ದತ್ತಾಂಶ[y + im.w*(x + im.h*c)] = im.ದತ್ತಾಂಶ[n-1-x + im.w*(y + im.h*c)];
                    im.ದತ್ತಾಂಶ[n-1-x + im.w*(y + im.h*c)] = im.ದತ್ತಾಂಶ[n-1-y + im.w*(n-1-x + im.h*c)];
                    im.ದತ್ತಾಂಶ[n-1-y + im.w*(n-1-x + im.h*c)] = im.ದತ್ತಾಂಶ[x + im.w*(n-1-y + im.h*c)];
                    im.ದತ್ತಾಂಶ[x + im.w*(n-1-y + im.h*c)] = ತಾತ್ಕಾಲಿಕ;
                }
            }
        }
    }
}

ಶೂನ್ಯ flip_image(image a)
{
    ಪೂರ್ಣಾಂಕ i,j,k;
    ಗಾಗಿ(k = 0; k < a.c; ++k){
        ಗಾಗಿ(i = 0; i < a.h; ++i){
            ಗಾಗಿ(j = 0; j < a.w/2; ++j){
                ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ = j + a.w*(i + a.h*(k));
                ಪೂರ್ಣಾಂಕ flip = (a.w - j - 1) + a.w*(i + a.h*(k));
                ದಶಮಾಂಶ ಅದಲುಬದಲು = a.ದತ್ತಾಂಶ[flip];
                a.ದತ್ತಾಂಶ[flip] = a.ದತ್ತಾಂಶ[ಸೂಚ್ಯಂಕ];
                a.ದತ್ತಾಂಶ[ಸೂಚ್ಯಂಕ] = ಅದಲುಬದಲು;
            }
        }
    }
}

image image_ದೂರ(image a, image b)
{
    ಪೂರ್ಣಾಂಕ i,j;
    image dist = make_image(a.w, a.h, 1);
    ಗಾಗಿ(i = 0; i < a.c; ++i){
        ಗಾಗಿ(j = 0; j < a.h*a.w; ++j){
            dist.ದತ್ತಾಂಶ[j] += ಘಾತ(a.ದತ್ತಾಂಶ[i*a.h*a.w+j]-b.ದತ್ತಾಂಶ[i*a.h*a.w+j],2);
        }
    }
    ಗಾಗಿ(j = 0; j < a.h*a.w; ++j){
        dist.ದತ್ತಾಂಶ[j] = ವರ್ಗಮೂಲ(dist.ದತ್ತಾಂಶ[j]);
    }
    ಹಿಂತಿರುಗಿ dist;
}

ಶೂನ್ಯ ghost_image(image ಮೂಲ, image dest, ಪೂರ್ಣಾಂಕ dx, ಪೂರ್ಣಾಂಕ dy)
{
    ಪೂರ್ಣಾಂಕ x,y,k;
    ದಶಮಾಂಶ max_dist = ವರ್ಗಮೂಲ((-ಮೂಲ.w/2. + .5)*(-ಮೂಲ.w/2. + .5));
    ಗಾಗಿ(k = 0; k < ಮೂಲ.c; ++k){
        ಗಾಗಿ(y = 0; y < ಮೂಲ.h; ++y){
            ಗಾಗಿ(x = 0; x < ಮೂಲ.w; ++x){
                ದಶಮಾಂಶ dist = ವರ್ಗಮೂಲ((x - ಮೂಲ.w/2. + .5)*(x - ಮೂಲ.w/2. + .5) + (y - ಮೂಲ.h/2. + .5)*(y - ಮೂಲ.h/2. + .5));
                ದಶಮಾಂಶ alpha = (1 - dist/max_dist);
                ಆದರೆ(alpha < 0) alpha = 0;
                ದಶಮಾಂಶ v1 = get_pixel(ಮೂಲ, x,y,k);
                ದಶಮಾಂಶ v2 = get_pixel(dest, dx+x,dy+y,k);
                ದಶಮಾಂಶ val = alpha*v1 + (1-alpha)*v2;
                ಗಣ_pixel(dest, dx+x, dy+y, k, val);
            }
        }
    }
}

ಶೂನ್ಯ blocky_image(image im, ಪೂರ್ಣಾಂಕ s)
{
    ಪೂರ್ಣಾಂಕ i,j,k;
    ಗಾಗಿ(k = 0; k < im.c; ++k){
        ಗಾಗಿ(j = 0; j < im.h; ++j){
            ಗಾಗಿ(i = 0; i < im.w; ++i){
                im.ದತ್ತಾಂಶ[i + im.w*(j + im.h*k)] = im.ದತ್ತಾಂಶ[i/s*s + im.w*(j/s*s + im.h*k)];
            }
        }
    }
}

ಶೂನ್ಯ censor_image(image im, ಪೂರ್ಣಾಂಕ dx, ಪೂರ್ಣಾಂಕ dy, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h)
{
    ಪೂರ್ಣಾಂಕ i,j,k;
    ಪೂರ್ಣಾಂಕ s = 32;
    ಆದರೆ(dx < 0) dx = 0;
    ಆದರೆ(dy < 0) dy = 0;

    ಗಾಗಿ(k = 0; k < im.c; ++k){
        ಗಾಗಿ(j = dy; j < dy + h && j < im.h; ++j){
            ಗಾಗಿ(i = dx; i < dx + w && i < im.w; ++i){
                im.ದತ್ತಾಂಶ[i + im.w*(j + im.h*k)] = im.ದತ್ತಾಂಶ[i/s*s + im.w*(j/s*s + im.h*k)];
                //im.data[i + j*im.w + k*im.w*im.h] = 0;
            }
        }
    }
}

ಶೂನ್ಯ embed_image(image ಮೂಲ, image dest, ಪೂರ್ಣಾಂಕ dx, ಪೂರ್ಣಾಂಕ dy)
{
    ಪೂರ್ಣಾಂಕ x,y,k;
    ಗಾಗಿ(k = 0; k < ಮೂಲ.c; ++k){
        ಗಾಗಿ(y = 0; y < ಮೂಲ.h; ++y){
            ಗಾಗಿ(x = 0; x < ಮೂಲ.w; ++x){
                ದಶಮಾಂಶ val = get_pixel(ಮೂಲ, x,y,k);
                ಗಣ_pixel(dest, dx+x, dy+y, k, val);
            }
        }
    }
}

image collapse_image_layers(image ಮೂಲ, ಪೂರ್ಣಾಂಕ border)
{
    ಪೂರ್ಣಾಂಕ h = ಮೂಲ.h;
    h = (h+border)*ಮೂಲ.c - border;
    image dest = make_image(ಮೂಲ.w, h, 1);
    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ(i = 0; i < ಮೂಲ.c; ++i){
        image layer = get_image_layer(ಮೂಲ, i);
        ಪೂರ್ಣಾಂಕ h_ಸ್ಥಾನಾಂತರ = i*(ಮೂಲ.h+border);
        embed_image(layer, dest, 0, h_ಸ್ಥಾನಾಂತರ);
        free_image(layer);
    }
    ಹಿಂತಿರುಗಿ dest;
}

ಶೂನ್ಯ constrain_image(image im)
{
    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ(i = 0; i < im.w*im.h*im.c; ++i){
        ಆದರೆ(im.ದತ್ತಾಂಶ[i] < 0) im.ದತ್ತಾಂಶ[i] = 0;
        ಆದರೆ(im.ದತ್ತಾಂಶ[i] > 1) im.ದತ್ತಾಂಶ[i] = 1;
    }
}

ಶೂನ್ಯ normalize_image(image p)
{
    ಪೂರ್ಣಾಂಕ i;
    ದಶಮಾಂಶ min = 9999999;
    ದಶಮಾಂಶ max = -999999;

    ಗಾಗಿ(i = 0; i < p.h*p.w*p.c; ++i){
        ದಶಮಾಂಶ v = p.ದತ್ತಾಂಶ[i];
        ಆದರೆ(v < min) min = v;
        ಆದರೆ(v > max) max = v;
    }
    ಆದರೆ(max - min < .000000001){
        min = 0;
        max = 1;
    }
    ಗಾಗಿ(i = 0; i < p.c*p.w*p.h; ++i){
        p.ದತ್ತಾಂಶ[i] = (p.ದತ್ತಾಂಶ[i] - min)/(max-min);
    }
}

ಶೂನ್ಯ normalize_image2(image p)
{
    ದಶಮಾಂಶ *min = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(p.c, ಗಾತ್ರ(ದಶಮಾಂಶ));
    ದಶಮಾಂಶ *max = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(p.c, ಗಾತ್ರ(ದಶಮಾಂಶ));
    ಪೂರ್ಣಾಂಕ i,j;
    ಗಾಗಿ(i = 0; i < p.c; ++i) min[i] = max[i] = p.ದತ್ತಾಂಶ[i*p.h*p.w];

    ಗಾಗಿ(j = 0; j < p.c; ++j){
        ಗಾಗಿ(i = 0; i < p.h*p.w; ++i){
            ದಶಮಾಂಶ v = p.ದತ್ತಾಂಶ[i+j*p.h*p.w];
            ಆದರೆ(v < min[j]) min[j] = v;
            ಆದರೆ(v > max[j]) max[j] = v;
        }
    }
    ಗಾಗಿ(i = 0; i < p.c; ++i){
        ಆದರೆ(max[i] - min[i] < .000000001){
            min[i] = 0;
            max[i] = 1;
        }
    }
    ಗಾಗಿ(j = 0; j < p.c; ++j){
        ಗಾಗಿ(i = 0; i < p.w*p.h; ++i){
            p.ದತ್ತಾಂಶ[i+j*p.h*p.w] = (p.ದತ್ತಾಂಶ[i+j*p.h*p.w] - min[j])/(max[j]-min[j]);
        }
    }
    ಬಿಡುಗಡೆ(min);
    ಬಿಡುಗಡೆ(max);
}

ಶೂನ್ಯ copy_image_into(image src, image dest)
{
    ಸ್ಮರಣೆ_ನಕಲು(dest.ದತ್ತಾಂಶ, src.ದತ್ತಾಂಶ, src.h*src.w*src.c*ಗಾತ್ರ(ದಶಮಾಂಶ));
}

image copy_image(image p)
{
    image copy = p;
    copy.ದತ್ತಾಂಶ = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(p.h*p.w*p.c, ಗಾತ್ರ(ದಶಮಾಂಶ));
    ಸ್ಮರಣೆ_ನಕಲು(copy.ದತ್ತಾಂಶ, p.ದತ್ತಾಂಶ, p.h*p.w*p.c*ಗಾತ್ರ(ದಶಮಾಂಶ));
    ಹಿಂತಿರುಗಿ copy;
}

ಶೂನ್ಯ rgbgr_image(image im)
{
    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ(i = 0; i < im.w*im.h; ++i){
        ದಶಮಾಂಶ ಅದಲುಬದಲು = im.ದತ್ತಾಂಶ[i];
        im.ದತ್ತಾಂಶ[i] = im.ದತ್ತಾಂಶ[i+im.w*im.h*2];
        im.ದತ್ತಾಂಶ[i+im.w*im.h*2] = ಅದಲುಬದಲು;
    }
}

ಪೂರ್ಣಾಂಕ show_image(image p, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಪೂರ್ಣಾಂಕ ms)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ OPENCV
    ಪೂರ್ಣಾಂಕ c = show_image_cv(p, name, ms);
    ಹಿಂತಿರುಗಿ c;
#ಇಲ್ಲದಿದ್ದರೆ
    fprintf(stderr, "Not compiled with OpenCV, saving to %s.png instead\n", name);
    save_image(p, name);
    ಹಿಂತಿರುಗಿ -1;
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ save_image_options(image im, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, IMTYPE f, ಪೂರ್ಣಾಂಕ quality)
{
    ಅಕ್ಷರ buff[256];
    //sprintf(buff, "%s (%d)", name, windows);
    ಆದರೆ(f == PNG)       sprintf(buff, "%s.png", name);
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (f == BMP) sprintf(buff, "%s.bmp", name);
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (f == TGA) sprintf(buff, "%s.tga", name);
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (f == JPG) sprintf(buff, "%s.jpg", name);
    ಇಲ್ಲದಿದ್ದರೆ               sprintf(buff, "%s.png", name);
    ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *ದತ್ತಾಂಶ = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(im.w*im.h*im.c, ಗಾತ್ರ(ಅಕ್ಷರ));
    ಪೂರ್ಣಾಂಕ i,k;
    ಗಾಗಿ(k = 0; k < im.c; ++k){
        ಗಾಗಿ(i = 0; i < im.w*im.h; ++i){
            ದತ್ತಾಂಶ[i*im.c+k] = (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ) (255*im.ದತ್ತಾಂಶ[i + k*im.w*im.h]);
        }
    }
    ಪೂರ್ಣಾಂಕ success = 0;
    ಆದರೆ(f == PNG)       success = stbi_write_png(buff, im.w, im.h, im.c, ದತ್ತಾಂಶ, im.w*im.c);
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (f == BMP) success = stbi_write_bmp(buff, im.w, im.h, im.c, ದತ್ತಾಂಶ);
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (f == TGA) success = stbi_write_tga(buff, im.w, im.h, im.c, ದತ್ತಾಂಶ);
    ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (f == JPG) success = stbi_write_jpg(buff, im.w, im.h, im.c, ದತ್ತಾಂಶ, quality);
    ಬಿಡುಗಡೆ(ದತ್ತಾಂಶ);
    ಆದರೆ(!success) fprintf(stderr, "Failed to write image %s\n", buff);
}

ಶೂನ್ಯ save_image(image im, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name)
{
    save_image_options(im, name, JPG, 80);
}

ಶೂನ್ಯ show_image_layers(image p, ಅಕ್ಷರ *name)
{
    ಪೂರ್ಣಾಂಕ i;
    ಅಕ್ಷರ buff[256];
    ಗಾಗಿ(i = 0; i < p.c; ++i){
        sprintf(buff, "%s - Layer %d", name, i);
        image layer = get_image_layer(p, i);
        show_image(layer, buff, 1);
        free_image(layer);
    }
}

ಶೂನ್ಯ show_image_collapsed(image p, ಅಕ್ಷರ *name)
{
    image c = collapse_image_layers(p, 1);
    show_image(c, name, 1);
    free_image(c);
}

image make_ಖಾಲಿಗಳು_image(ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h, ಪೂರ್ಣಾಂಕ c)
{
    image ಹೊರಹೋಗು;
    ಹೊರಹೋಗು.ದತ್ತಾಂಶ = 0;
    ಹೊರಹೋಗು.h = h;
    ಹೊರಹೋಗು.w = w;
    ಹೊರಹೋಗು.c = c;
    ಹಿಂತಿರುಗಿ ಹೊರಹೋಗು;
}

image make_image(ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h, ಪೂರ್ಣಾಂಕ c)
{
    image ಹೊರಹೋಗು = make_ಖಾಲಿಗಳು_image(w,h,c);
    ಹೊರಹೋಗು.ದತ್ತಾಂಶ = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(h*w*c, ಗಾತ್ರ(ದಶಮಾಂಶ));
    ಹಿಂತಿರುಗಿ ಹೊರಹೋಗು;
}

image make_random_image(ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h, ಪೂರ್ಣಾಂಕ c)
{
    image ಹೊರಹೋಗು = make_ಖಾಲಿಗಳು_image(w,h,c);
    ಹೊರಹೋಗು.ದತ್ತಾಂಶ = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(h*w*c, ಗಾತ್ರ(ದಶಮಾಂಶ));
    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ(i = 0; i < w*h*c; ++i){
        ಹೊರಹೋಗು.ದತ್ತಾಂಶ[i] = (rand_ಸಾಮಾನ್ಯ() * .25) + .5;
    }
    ಹಿಂತಿರುಗಿ ಹೊರಹೋಗು;
}

image float_ಗಮ್ಯ_image(ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h, ಪೂರ್ಣಾಂಕ c, ದಶಮಾಂಶ *ದತ್ತಾಂಶ)
{
    image ಹೊರಹೋಗು = make_ಖಾಲಿಗಳು_image(w,h,c);
    ಹೊರಹೋಗು.ದತ್ತಾಂಶ = ದತ್ತಾಂಶ;
    ಹಿಂತಿರುಗಿ ಹೊರಹೋಗು;
}

ಶೂನ್ಯ ಇರಿಸು_image(image im, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h, ಪೂರ್ಣಾಂಕ dx, ಪೂರ್ಣಾಂಕ dy, image canvas)
{
    ಪೂರ್ಣಾಂಕ x, y, c;
    ಗಾಗಿ(c = 0; c < im.c; ++c){
        ಗಾಗಿ(y = 0; y < h; ++y){
            ಗಾಗಿ(x = 0; x < w; ++x){
                ದಶಮಾಂಶ rx = ((ದಶಮಾಂಶ)x / w) * im.w;
                ದಶಮಾಂಶ ry = ((ದಶಮಾಂಶ)y / h) * im.h;
                ದಶಮಾಂಶ val = bilinear_interpolate(im, rx, ry, c);
                ಗಣ_pixel(canvas, x + dx, y + dy, c, val);
            }
        }
    }
}

image center_crop_image(image im, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h)
{
    ಪೂರ್ಣಾಂಕ m = (im.w < im.h) ? im.w : im.h;   
    image c = crop_image(im, (im.w - m) / 2, (im.h - m)/2, m, m);
    image r = resize_image(c, w, h);
    free_image(c);
    ಹಿಂತಿರುಗಿ r;
}

image rotate_crop_image(image im, ದಶಮಾಂಶ rad, ದಶಮಾಂಶ s, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h, ದಶಮಾಂಶ dx, ದಶಮಾಂಶ dy, ದಶಮಾಂಶ aspect)
{
    ಪೂರ್ಣಾಂಕ x, y, c;
    ದಶಮಾಂಶ cx = im.w/2.;
    ದಶಮಾಂಶ cy = im.h/2.;
    image rot = make_image(w, h, im.c);
    ಗಾಗಿ(c = 0; c < im.c; ++c){
        ಗಾಗಿ(y = 0; y < h; ++y){
            ಗಾಗಿ(x = 0; x < w; ++x){
                ದಶಮಾಂಶ rx = ಕೋಜ್ಯಾ(rad)*((x - w/2.)/s*aspect + dx/s*aspect) - ಜ್ಯಾ(rad)*((y - h/2.)/s + dy/s) + cx;
                ದಶಮಾಂಶ ry = ಜ್ಯಾ(rad)*((x - w/2.)/s*aspect + dx/s*aspect) + ಕೋಜ್ಯಾ(rad)*((y - h/2.)/s + dy/s) + cy;
                ದಶಮಾಂಶ val = bilinear_interpolate(im, rx, ry, c);
                ಗಣ_pixel(rot, x, y, c, val);
            }
        }
    }
    ಹಿಂತಿರುಗಿ rot;
}

image rotate_image(image im, ದಶಮಾಂಶ rad)
{
    ಪೂರ್ಣಾಂಕ x, y, c;
    ದಶಮಾಂಶ cx = im.w/2.;
    ದಶಮಾಂಶ cy = im.h/2.;
    image rot = make_image(im.w, im.h, im.c);
    ಗಾಗಿ(c = 0; c < im.c; ++c){
        ಗಾಗಿ(y = 0; y < im.h; ++y){
            ಗಾಗಿ(x = 0; x < im.w; ++x){
                ದಶಮಾಂಶ rx = ಕೋಜ್ಯಾ(rad)*(x-cx) - ಜ್ಯಾ(rad)*(y-cy) + cx;
                ದಶಮಾಂಶ ry = ಜ್ಯಾ(rad)*(x-cx) + ಕೋಜ್ಯಾ(rad)*(y-cy) + cy;
                ದಶಮಾಂಶ val = bilinear_interpolate(im, rx, ry, c);
                ಗಣ_pixel(rot, x, y, c, val);
            }
        }
    }
    ಹಿಂತಿರುಗಿ rot;
}

ಶೂನ್ಯ fill_image(image m, ದಶಮಾಂಶ s)
{
    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ(i = 0; i < m.h*m.w*m.c; ++i) m.ದತ್ತಾಂಶ[i] = s;
}

ಶೂನ್ಯ translate_image(image m, ದಶಮಾಂಶ s)
{
    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ(i = 0; i < m.h*m.w*m.c; ++i) m.ದತ್ತಾಂಶ[i] += s;
}

ಶೂನ್ಯ scale_image(image m, ದಶಮಾಂಶ s)
{
    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ(i = 0; i < m.h*m.w*m.c; ++i) m.ದತ್ತಾಂಶ[i] *= s;
}

image crop_image(image im, ಪೂರ್ಣಾಂಕ dx, ಪೂರ್ಣಾಂಕ dy, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h)
{
    image cropped = make_image(w, h, im.c);
    ಪೂರ್ಣಾಂಕ i, j, k;
    ಗಾಗಿ(k = 0; k < im.c; ++k){
        ಗಾಗಿ(j = 0; j < h; ++j){
            ಗಾಗಿ(i = 0; i < w; ++i){
                ಪೂರ್ಣಾಂಕ r = j + dy;
                ಪೂರ್ಣಾಂಕ c = i + dx;
                ದಶಮಾಂಶ val = 0;
                r = constrain_int(r, 0, im.h-1);
                c = constrain_int(c, 0, im.w-1);
                val = get_pixel(im, c, r, k);
                ಗಣ_pixel(cropped, i, j, k, val);
            }
        }
    }
    ಹಿಂತಿರುಗಿ cropped;
}

ಪೂರ್ಣಾಂಕ ಉತ್ತಮ_3d_ಸರಿಸು_r(image a, image b, ಪೂರ್ಣಾಂಕ min, ಪೂರ್ಣಾಂಕ max)
{
    ಆದರೆ(min == max) ಹಿಂತಿರುಗಿ min;
    ಪೂರ್ಣಾಂಕ mid = floor((min + max) / 2.);
    image c1 = crop_image(b, 0, mid, b.w, b.h);
    image c2 = crop_image(b, 0, mid+1, b.w, b.h);
    ದಶಮಾಂಶ d1 = dist_array(c1.ದತ್ತಾಂಶ, a.ದತ್ತಾಂಶ, a.w*a.h*a.c, 10);
    ದಶಮಾಂಶ d2 = dist_array(c2.ದತ್ತಾಂಶ, a.ದತ್ತಾಂಶ, a.w*a.h*a.c, 10);
    free_image(c1);
    free_image(c2);
    ಆದರೆ(d1 < d2) ಹಿಂತಿರುಗಿ ಉತ್ತಮ_3d_ಸರಿಸು_r(a, b, min, mid);
    ಇಲ್ಲದಿದ್ದರೆ ಹಿಂತಿರುಗಿ ಉತ್ತಮ_3d_ಸರಿಸು_r(a, b, mid+1, max);
}

ಪೂರ್ಣಾಂಕ ಉತ್ತಮ_3d_ಸರಿಸು(image a, image b, ಪೂರ್ಣಾಂಕ min, ಪೂರ್ಣಾಂಕ max)
{
    ಪೂರ್ಣಾಂಕ i;
    ಪೂರ್ಣಾಂಕ ಉತ್ತಮ = 0;
    ದಶಮಾಂಶ ಉತ್ತಮ_ದೂರ = FLT_MAX;
    ಗಾಗಿ(i = min; i <= max; i += 2){
        image c = crop_image(b, 0, i, b.w, b.h);
        ದಶಮಾಂಶ d = dist_array(c.ದತ್ತಾಂಶ, a.ದತ್ತಾಂಶ, a.w*a.h*a.c, 100);
        ಆದರೆ(d < ಉತ್ತಮ_ದೂರ){
            ಉತ್ತಮ_ದೂರ = d;
            ಉತ್ತಮ = i;
        }
        ಬರೆ("%d %f\n", i, d);
        free_image(c);
    }
    ಹಿಂತಿರುಗಿ ಉತ್ತಮ;
}

ಶೂನ್ಯ ಸಂಯುಕ್ತ_3d(ಅಕ್ಷರ *f1, ಅಕ್ಷರ *f2, ಅಕ್ಷರ *ಹೊರಹೋಗು, ಪೂರ್ಣಾಂಕ delta)
{
    ಆದರೆ(!ಹೊರಹೋಗು) ಹೊರಹೋಗು = "out";
    image a = ತುಂಬು_image(f1, 0,0,0);
    image b = ತುಂಬು_image(f2, 0,0,0);
    ಪೂರ್ಣಾಂಕ ಸರಿಸು = ಉತ್ತಮ_3d_ಸರಿಸು_r(a, b, -a.h/100, a.h/100);

    image c1 = crop_image(b, 10, ಸರಿಸು, b.w, b.h);
    ದಶಮಾಂಶ d1 = dist_array(c1.ದತ್ತಾಂಶ, a.ದತ್ತಾಂಶ, a.w*a.h*a.c, 100);
    image c2 = crop_image(b, -10, ಸರಿಸು, b.w, b.h);
    ದಶಮಾಂಶ d2 = dist_array(c2.ದತ್ತಾಂಶ, a.ದತ್ತಾಂಶ, a.w*a.h*a.c, 100);

    ಆದರೆ(d2 < d1 && 0){
        image ಅದಲುಬದಲು = a;
        a = b;
        b = ಅದಲುಬದಲು;
        ಸರಿಸು = -ಸರಿಸು;
        ಬರೆ("swapped, %d\n", ಸರಿಸು);
    }
    ಇಲ್ಲದಿದ್ದರೆ{
        ಬರೆ("%d\n", ಸರಿಸು);
    }

    image c = crop_image(b, delta, ಸರಿಸು, a.w, a.h);
    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ(i = 0; i < c.w*c.h; ++i){
        c.ದತ್ತಾಂಶ[i] = a.ದತ್ತಾಂಶ[i];
    }
    save_image(c, ಹೊರಹೋಗು);
}

ಶೂನ್ಯ letterbox_image_into(image im, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h, image boxed)
{
    ಪೂರ್ಣಾಂಕ new_w = im.w;
    ಪೂರ್ಣಾಂಕ new_h = im.h;
    ಆದರೆ (((ದಶಮಾಂಶ)w/im.w) < ((ದಶಮಾಂಶ)h/im.h)) {
        new_w = w;
        new_h = (im.h * w)/im.w;
    } ಇಲ್ಲದಿದ್ದರೆ {
        new_h = h;
        new_w = (im.w * h)/im.h;
    }
    image resized = resize_image(im, new_w, new_h);
    embed_image(resized, boxed, (w-new_w)/2, (h-new_h)/2); 
    free_image(resized);
}

image letterbox_image(image im, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h)
{
    ಪೂರ್ಣಾಂಕ new_w = im.w;
    ಪೂರ್ಣಾಂಕ new_h = im.h;
    ಆದರೆ (((ದಶಮಾಂಶ)w/im.w) < ((ದಶಮಾಂಶ)h/im.h)) {
        new_w = w;
        new_h = (im.h * w)/im.w;
    } ಇಲ್ಲದಿದ್ದರೆ {
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
    ಹಿಂತಿರುಗಿ boxed;
}

image resize_max(image im, ಪೂರ್ಣಾಂಕ max)
{
    ಪೂರ್ಣಾಂಕ w = im.w;
    ಪೂರ್ಣಾಂಕ h = im.h;
    ಆದರೆ(w > h){
        h = (h * max) / w;
        w = max;
    } ಇಲ್ಲದಿದ್ದರೆ {
        w = (w * max) / h;
        h = max;
    }
    ಆದರೆ(w == im.w && h == im.h) ಹಿಂತಿರುಗಿ im;
    image resized = resize_image(im, w, h);
    ಹಿಂತಿರುಗಿ resized;
}

image resize_min(image im, ಪೂರ್ಣಾಂಕ min)
{
    ಪೂರ್ಣಾಂಕ w = im.w;
    ಪೂರ್ಣಾಂಕ h = im.h;
    ಆದರೆ(w < h){
        h = (h * min) / w;
        w = min;
    } ಇಲ್ಲದಿದ್ದರೆ {
        w = (w * min) / h;
        h = min;
    }
    ಆದರೆ(w == im.w && h == im.h) ಹಿಂತಿರುಗಿ im;
    image resized = resize_image(im, w, h);
    ಹಿಂತಿರುಗಿ resized;
}

image random_crop_image(image im, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h)
{
    ಪೂರ್ಣಾಂಕ dx = rand_int(0, im.w - w);
    ಪೂರ್ಣಾಂಕ dy = rand_int(0, im.h - h);
    image crop = crop_image(im, dx, dy, w, h);
    ಹಿಂತಿರುಗಿ crop;
}

augment_ವಾದಗಳು random_augment_ವಾದಗಳು(image im, ದಶಮಾಂಶ angle, ದಶಮಾಂಶ aspect, ಪೂರ್ಣಾಂಕ ಕೆಳಗಿನ, ಪೂರ್ಣಾಂಕ ಮೇಲಿನ, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h)
{
    augment_ವಾದಗಳು a = {0};
    aspect = rand_scale(aspect);
    ಪೂರ್ಣಾಂಕ r = rand_int(ಕೆಳಗಿನ, ಮೇಲಿನ);
    ಪೂರ್ಣಾಂಕ min = (im.h < im.w*aspect) ? im.h : im.w*aspect;
    ದಶಮಾಂಶ scale = (ದಶಮಾಂಶ)r / min;

    ದಶಮಾಂಶ rad = rand_ಏಕರೂಪ(-angle, angle) * TWO_PI / 360.;

    ದಶಮಾಂಶ dx = (im.w*scale/aspect - w) / 2.;
    ದಶಮಾಂಶ dy = (im.h*scale - w) / 2.;
    //if(dx < 0) dx = 0;
    //if(dy < 0) dy = 0;
    dx = rand_ಏಕರೂಪ(-dx, dx);
    dy = rand_ಏಕರೂಪ(-dy, dy);

    a.rad = rad;
    a.scale = scale;
    a.w = w;
    a.h = h;
    a.dx = dx;
    a.dy = dy;
    a.aspect = aspect;
    ಹಿಂತಿರುಗಿ a;
}

image random_augment_image(image im, ದಶಮಾಂಶ angle, ದಶಮಾಂಶ aspect, ಪೂರ್ಣಾಂಕ ಕೆಳಗಿನ, ಪೂರ್ಣಾಂಕ ಮೇಲಿನ, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h)
{
    augment_ವಾದಗಳು a = random_augment_ವಾದಗಳು(im, angle, aspect, ಕೆಳಗಿನ, ಮೇಲಿನ, w, h);
    image crop = rotate_crop_image(im, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);
    ಹಿಂತಿರುಗಿ crop;
}

ದಶಮಾಂಶ three_way_max(ದಶಮಾಂಶ a, ದಶಮಾಂಶ b, ದಶಮಾಂಶ c)
{
    ಹಿಂತಿರುಗಿ (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

ದಶಮಾಂಶ three_way_min(ದಶಮಾಂಶ a, ದಶಮಾಂಶ b, ದಶಮಾಂಶ c)
{
    ಹಿಂತಿರುಗಿ (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

ಶೂನ್ಯ yuv_ಗಮ್ಯ_rgb(image im)
{
    ಪ್ರತಿಜ್ಞಾ(im.c == 3);
    ಪೂರ್ಣಾಂಕ i, j;
    ದಶಮಾಂಶ r, g, b;
    ದಶಮಾಂಶ y, u, v;
    ಗಾಗಿ(j = 0; j < im.h; ++j){
        ಗಾಗಿ(i = 0; i < im.w; ++i){
            y = get_pixel(im, i , j, 0);
            u = get_pixel(im, i , j, 1);
            v = get_pixel(im, i , j, 2);

            r = y + 1.13983*v;
            g = y + -.39465*u + -.58060*v;
            b = y + 2.03211*u;

            ಗಣ_pixel(im, i, j, 0, r);
            ಗಣ_pixel(im, i, j, 1, g);
            ಗಣ_pixel(im, i, j, 2, b);
        }
    }
}

ಶೂನ್ಯ rgb_ಗಮ್ಯ_yuv(image im)
{
    ಪ್ರತಿಜ್ಞಾ(im.c == 3);
    ಪೂರ್ಣಾಂಕ i, j;
    ದಶಮಾಂಶ r, g, b;
    ದಶಮಾಂಶ y, u, v;
    ಗಾಗಿ(j = 0; j < im.h; ++j){
        ಗಾಗಿ(i = 0; i < im.w; ++i){
            r = get_pixel(im, i , j, 0);
            g = get_pixel(im, i , j, 1);
            b = get_pixel(im, i , j, 2);

            y = .299*r + .587*g + .114*b;
            u = -.14713*r + -.28886*g + .436*b;
            v = .615*r + -.51499*g + -.10001*b;

            ಗಣ_pixel(im, i, j, 0, y);
            ಗಣ_pixel(im, i, j, 1, u);
            ಗಣ_pixel(im, i, j, 2, v);
        }
    }
}

// http://www.cs.rit.edu/~ncs/color/t_convert.html
ಶೂನ್ಯ rgb_ಗಮ್ಯ_hsv(image im)
{
    ಪ್ರತಿಜ್ಞಾ(im.c == 3);
    ಪೂರ್ಣಾಂಕ i, j;
    ದಶಮಾಂಶ r, g, b;
    ದಶಮಾಂಶ h, s, v;
    ಗಾಗಿ(j = 0; j < im.h; ++j){
        ಗಾಗಿ(i = 0; i < im.w; ++i){
            r = get_pixel(im, i , j, 0);
            g = get_pixel(im, i , j, 1);
            b = get_pixel(im, i , j, 2);
            ದಶಮಾಂಶ max = three_way_max(r,g,b);
            ದಶಮಾಂಶ min = three_way_min(r,g,b);
            ದಶಮಾಂಶ delta = max - min;
            v = max;
            ಆದರೆ(max == 0){
                s = 0;
                h = 0;
            }ಇಲ್ಲದಿದ್ದರೆ{
                s = delta/max;
                ಆದರೆ(r == max){
                    h = (g - b) / delta;
                } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (g == max) {
                    h = 2 + (b - r) / delta;
                } ಇಲ್ಲದಿದ್ದರೆ {
                    h = 4 + (r - g) / delta;
                }
                ಆದರೆ (h < 0) h += 6;
                h = h/6.;
            }
            ಗಣ_pixel(im, i, j, 0, h);
            ಗಣ_pixel(im, i, j, 1, s);
            ಗಣ_pixel(im, i, j, 2, v);
        }
    }
}

ಶೂನ್ಯ hsv_ಗಮ್ಯ_rgb(image im)
{
    ಪ್ರತಿಜ್ಞಾ(im.c == 3);
    ಪೂರ್ಣಾಂಕ i, j;
    ದಶಮಾಂಶ r, g, b;
    ದಶಮಾಂಶ h, s, v;
    ದಶಮಾಂಶ f, p, q, t;
    ಗಾಗಿ(j = 0; j < im.h; ++j){
        ಗಾಗಿ(i = 0; i < im.w; ++i){
            h = 6 * get_pixel(im, i , j, 0);
            s = get_pixel(im, i , j, 1);
            v = get_pixel(im, i , j, 2);
            ಆದರೆ (s == 0) {
                r = g = b = v;
            } ಇಲ್ಲದಿದ್ದರೆ {
                ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ = floor(h);
                f = h - ಸೂಚ್ಯಂಕ;
                p = v*(1-s);
                q = v*(1-s*f);
                t = v*(1-s*(1-f));
                ಆದರೆ(ಸೂಚ್ಯಂಕ == 0){
                    r = v; g = t; b = p;
                } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(ಸೂಚ್ಯಂಕ == 1){
                    r = q; g = v; b = p;
                } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(ಸೂಚ್ಯಂಕ == 2){
                    r = p; g = v; b = t;
                } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(ಸೂಚ್ಯಂಕ == 3){
                    r = p; g = q; b = v;
                } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(ಸೂಚ್ಯಂಕ == 4){
                    r = t; g = p; b = v;
                } ಇಲ್ಲದಿದ್ದರೆ {
                    r = v; g = p; b = q;
                }
            }
            ಗಣ_pixel(im, i, j, 0, r);
            ಗಣ_pixel(im, i, j, 1, g);
            ಗಣ_pixel(im, i, j, 2, b);
        }
    }
}

ಶೂನ್ಯ grayscale_image_3c(image im)
{
    ಪ್ರತಿಜ್ಞಾ(im.c == 3);
    ಪೂರ್ಣಾಂಕ i, j, k;
    ದಶಮಾಂಶ scale[] = {0.299, 0.587, 0.114};
    ಗಾಗಿ(j = 0; j < im.h; ++j){
        ಗಾಗಿ(i = 0; i < im.w; ++i){
            ದಶಮಾಂಶ val = 0;
            ಗಾಗಿ(k = 0; k < 3; ++k){
                val += scale[k]*get_pixel(im, i, j, k);
            }
            im.ದತ್ತಾಂಶ[0*im.h*im.w + im.w*j + i] = val;
            im.ದತ್ತಾಂಶ[1*im.h*im.w + im.w*j + i] = val;
            im.ದತ್ತಾಂಶ[2*im.h*im.w + im.w*j + i] = val;
        }
    }
}

image grayscale_image(image im)
{
    ಪ್ರತಿಜ್ಞಾ(im.c == 3);
    ಪೂರ್ಣಾಂಕ i, j, k;
    image ಬೂದು = make_image(im.w, im.h, 1);
    ದಶಮಾಂಶ scale[] = {0.299, 0.587, 0.114};
    ಗಾಗಿ(k = 0; k < im.c; ++k){
        ಗಾಗಿ(j = 0; j < im.h; ++j){
            ಗಾಗಿ(i = 0; i < im.w; ++i){
                ಬೂದು.ದತ್ತಾಂಶ[i+im.w*j] += scale[k]*get_pixel(im, i, j, k);
            }
        }
    }
    ಹಿಂತಿರುಗಿ ಬೂದು;
}

image ಮಿತಿ_image(image im, ದಶಮಾಂಶ thresh)
{
    ಪೂರ್ಣಾಂಕ i;
    image t = make_image(im.w, im.h, im.c);
    ಗಾಗಿ(i = 0; i < im.w*im.h*im.c; ++i){
        t.ದತ್ತಾಂಶ[i] = im.ದತ್ತಾಂಶ[i]>thresh ? 1 : 0;
    }
    ಹಿಂತಿರುಗಿ t;
}

image blend_image(image fore, image ಹಿಂಭಾಗ, ದಶಮಾಂಶ alpha)
{
    ಪ್ರತಿಜ್ಞಾ(fore.w == ಹಿಂಭಾಗ.w && fore.h == ಹಿಂಭಾಗ.h && fore.c == ಹಿಂಭಾಗ.c);
    image blend = make_image(fore.w, fore.h, fore.c);
    ಪೂರ್ಣಾಂಕ i, j, k;
    ಗಾಗಿ(k = 0; k < fore.c; ++k){
        ಗಾಗಿ(j = 0; j < fore.h; ++j){
            ಗಾಗಿ(i = 0; i < fore.w; ++i){
                ದಶಮಾಂಶ val = alpha * get_pixel(fore, i, j, k) + 
                    (1 - alpha)* get_pixel(ಹಿಂಭಾಗ, i, j, k);
                ಗಣ_pixel(blend, i, j, k, val);
            }
        }
    }
    ಹಿಂತಿರುಗಿ blend;
}

ಶೂನ್ಯ scale_image_channel(image im, ಪೂರ್ಣಾಂಕ c, ದಶಮಾಂಶ v)
{
    ಪೂರ್ಣಾಂಕ i, j;
    ಗಾಗಿ(j = 0; j < im.h; ++j){
        ಗಾಗಿ(i = 0; i < im.w; ++i){
            ದಶಮಾಂಶ pix = get_pixel(im, i, j, c);
            pix = pix*v;
            ಗಣ_pixel(im, i, j, c, pix);
        }
    }
}

ಶೂನ್ಯ translate_image_channel(image im, ಪೂರ್ಣಾಂಕ c, ದಶಮಾಂಶ v)
{
    ಪೂರ್ಣಾಂಕ i, j;
    ಗಾಗಿ(j = 0; j < im.h; ++j){
        ಗಾಗಿ(i = 0; i < im.w; ++i){
            ದಶಮಾಂಶ pix = get_pixel(im, i, j, c);
            pix = pix+v;
            ಗಣ_pixel(im, i, j, c, pix);
        }
    }
}

image binarize_image(image im)
{
    image c = copy_image(im);
    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ(i = 0; i < im.w * im.h * im.c; ++i){
        ಆದರೆ(c.ದತ್ತಾಂಶ[i] > .5) c.ದತ್ತಾಂಶ[i] = 1;
        ಇಲ್ಲದಿದ್ದರೆ c.ದತ್ತಾಂಶ[i] = 0;
    }
    ಹಿಂತಿರುಗಿ c;
}

ಶೂನ್ಯ saturate_image(image im, ದಶಮಾಂಶ sat)
{
    rgb_ಗಮ್ಯ_hsv(im);
    scale_image_channel(im, 1, sat);
    hsv_ಗಮ್ಯ_rgb(im);
    constrain_image(im);
}

ಶೂನ್ಯ hue_image(image im, ದಶಮಾಂಶ hue)
{
    rgb_ಗಮ್ಯ_hsv(im);
    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ(i = 0; i < im.w*im.h; ++i){
        im.ದತ್ತಾಂಶ[i] = im.ದತ್ತಾಂಶ[i] + hue;
        ಆದರೆ (im.ದತ್ತಾಂಶ[i] > 1) im.ದತ್ತಾಂಶ[i] -= 1;
        ಆದರೆ (im.ದತ್ತಾಂಶ[i] < 0) im.ದತ್ತಾಂಶ[i] += 1;
    }
    hsv_ಗಮ್ಯ_rgb(im);
    constrain_image(im);
}

ಶೂನ್ಯ exposure_image(image im, ದಶಮಾಂಶ sat)
{
    rgb_ಗಮ್ಯ_hsv(im);
    scale_image_channel(im, 2, sat);
    hsv_ಗಮ್ಯ_rgb(im);
    constrain_image(im);
}

ಶೂನ್ಯ distort_image(image im, ದಶಮಾಂಶ hue, ದಶಮಾಂಶ sat, ದಶಮಾಂಶ val)
{
    rgb_ಗಮ್ಯ_hsv(im);
    scale_image_channel(im, 1, sat);
    scale_image_channel(im, 2, val);
    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ(i = 0; i < im.w*im.h; ++i){
        im.ದತ್ತಾಂಶ[i] = im.ದತ್ತಾಂಶ[i] + hue;
        ಆದರೆ (im.ದತ್ತಾಂಶ[i] > 1) im.ದತ್ತಾಂಶ[i] -= 1;
        ಆದರೆ (im.ದತ್ತಾಂಶ[i] < 0) im.ದತ್ತಾಂಶ[i] += 1;
    }
    hsv_ಗಮ್ಯ_rgb(im);
    constrain_image(im);
}

ಶೂನ್ಯ random_distort_image(image im, ದಶಮಾಂಶ hue, ದಶಮಾಂಶ saturation, ದಶಮಾಂಶ exposure)
{
    ದಶಮಾಂಶ dhue = rand_ಏಕರೂಪ(-hue, hue);
    ದಶಮಾಂಶ dsat = rand_scale(saturation);
    ದಶಮಾಂಶ dexp = rand_scale(exposure);
    distort_image(im, dhue, dsat, dexp);
}

ಶೂನ್ಯ saturate_exposure_image(image im, ದಶಮಾಂಶ sat, ದಶಮಾಂಶ exposure)
{
    rgb_ಗಮ್ಯ_hsv(im);
    scale_image_channel(im, 1, sat);
    scale_image_channel(im, 2, exposure);
    hsv_ಗಮ್ಯ_rgb(im);
    constrain_image(im);
}

image resize_image(image im, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h)
{
    image resized = make_image(w, h, im.c);   
    image part = make_image(w, im.h, im.c);
    ಪೂರ್ಣಾಂಕ r, c, k;
    ದಶಮಾಂಶ w_scale = (ದಶಮಾಂಶ)(im.w - 1) / (w - 1);
    ದಶಮಾಂಶ h_scale = (ದಶಮಾಂಶ)(im.h - 1) / (h - 1);
    ಗಾಗಿ(k = 0; k < im.c; ++k){
        ಗಾಗಿ(r = 0; r < im.h; ++r){
            ಗಾಗಿ(c = 0; c < w; ++c){
                ದಶಮಾಂಶ val = 0;
                ಆದರೆ(c == w-1 || im.w == 1){
                    val = get_pixel(im, im.w-1, r, k);
                } ಇಲ್ಲದಿದ್ದರೆ {
                    ದಶಮಾಂಶ sx = c*w_scale;
                    ಪೂರ್ಣಾಂಕ ix = (ಪೂರ್ಣಾಂಕ) sx;
                    ದಶಮಾಂಶ dx = sx - ix;
                    val = (1 - dx) * get_pixel(im, ix, r, k) + dx * get_pixel(im, ix+1, r, k);
                }
                ಗಣ_pixel(part, c, r, k, val);
            }
        }
    }
    ಗಾಗಿ(k = 0; k < im.c; ++k){
        ಗಾಗಿ(r = 0; r < h; ++r){
            ದಶಮಾಂಶ sy = r*h_scale;
            ಪೂರ್ಣಾಂಕ iy = (ಪೂರ್ಣಾಂಕ) sy;
            ದಶಮಾಂಶ dy = sy - iy;
            ಗಾಗಿ(c = 0; c < w; ++c){
                ದಶಮಾಂಶ val = (1-dy) * get_pixel(part, c, iy, k);
                ಗಣ_pixel(resized, c, r, k, val);
            }
            ಆದರೆ(r == h-1 || im.h == 1) ಮುಂದುವರಿ;
            ಗಾಗಿ(c = 0; c < w; ++c){
                ದಶಮಾಂಶ val = dy * get_pixel(part, c, iy+1, k);
                ಕೂಡಿಸು_pixel(resized, c, r, k, val);
            }
        }
    }

    free_image(part);
    ಹಿಂತಿರುಗಿ resized;
}


ಶೂನ್ಯ test_resize(ಅಕ್ಷರ *filename)
{
    image im = ತುಂಬು_image(filename, 0,0, 3);
    ದಶಮಾಂಶ mag = mag_array(im.ದತ್ತಾಂಶ, im.w*im.h*im.c);
    ಬರೆ("L2 Norm: %f\n", mag);
    image ಬೂದು = grayscale_image(im);

    image c1 = copy_image(im);
    image c2 = copy_image(im);
    image c3 = copy_image(im);
    image c4 = copy_image(im);
    distort_image(c1, .1, 1.5, 1.5);
    distort_image(c2, -.1, .66666, .66666);
    distort_image(c3, .1, 1.5, .66666);
    distort_image(c4, .1, .66666, 1.5);


    show_image(im,   "Original", 1);
    show_image(ಬೂದು, "Gray", 1);
    show_image(c1, "C1", 1);
    show_image(c2, "C2", 1);
    show_image(c3, "C3", 1);
    show_image(c4, "C4", 1);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ OPENCV
    ಆಗ(1){
        image aug = random_augment_image(im, 0, .75, 320, 448, 320, 320);
        show_image(aug, "aug", 1);
        free_image(aug);


        ದಶಮಾಂಶ exposure = 1.15;
        ದಶಮಾಂಶ saturation = 1.15;
        ದಶಮಾಂಶ hue = .05;

        image c = copy_image(im);

        ದಶಮಾಂಶ dexp = rand_scale(exposure);
        ದಶಮಾಂಶ dsat = rand_scale(saturation);
        ದಶಮಾಂಶ dhue = rand_ಏಕರೂಪ(-hue, hue);

        distort_image(c, dhue, dsat, dexp);
        show_image(c, "rand", 1);
        ಬರೆ("%f %f %f\n", dhue, dsat, dexp);
        free_image(c);
    }
#ಆದರೆ_ಅಂತ್ಯ
}


image ತುಂಬು_image_stb(ಅಕ್ಷರ *filename, ಪೂರ್ಣಾಂಕ channels)
{
    ಪೂರ್ಣಾಂಕ w, h, c;
    ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *ದತ್ತಾಂಶ = stbi_ತುಂಬು(filename, &w, &h, &c, channels);
    ಆದರೆ (!ದತ್ತಾಂಶ) {
        fprintf(stderr, "Cannot load image \"%s\"\nSTB Reason: %s\n", filename, stbi_failure_reason());
        ನಿರ್ಗಮಿಸು(0);
    }
    ಆದರೆ(channels) c = channels;
    ಪೂರ್ಣಾಂಕ i,j,k;
    image im = make_image(w, h, c);
    ಗಾಗಿ(k = 0; k < c; ++k){
        ಗಾಗಿ(j = 0; j < h; ++j){
            ಗಾಗಿ(i = 0; i < w; ++i){
                ಪೂರ್ಣಾಂಕ dst_ಸೂಚ್ಯಂಕ = i + w*j + w*h*k;
                ಪೂರ್ಣಾಂಕ src_ಸೂಚ್ಯಂಕ = k + c*i + c*w*j;
                im.ದತ್ತಾಂಶ[dst_ಸೂಚ್ಯಂಕ] = (ದಶಮಾಂಶ)ದತ್ತಾಂಶ[src_ಸೂಚ್ಯಂಕ]/255.;
            }
        }
    }
    ಬಿಡುಗಡೆ(ದತ್ತಾಂಶ);
    ಹಿಂತಿರುಗಿ im;
}

image ತುಂಬು_image(ಅಕ್ಷರ *filename, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h, ಪೂರ್ಣಾಂಕ c)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ OPENCV
    image ಹೊರಹೋಗು = ತುಂಬು_image_cv(filename, c);
#ಇಲ್ಲದಿದ್ದರೆ
    image ಹೊರಹೋಗು = ತುಂಬು_image_stb(filename, c);
#ಆದರೆ_ಅಂತ್ಯ

    ಆದರೆ((h && w) && (h != ಹೊರಹೋಗು.h || w != ಹೊರಹೋಗು.w)){
        image resized = resize_image(ಹೊರಹೋಗು, w, h);
        free_image(ಹೊರಹೋಗು);
        ಹೊರಹೋಗು = resized;
    }
    ಹಿಂತಿರುಗಿ ಹೊರಹೋಗು;
}

image ತುಂಬು_image_color(ಅಕ್ಷರ *filename, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h)
{
    ಹಿಂತಿರುಗಿ ತುಂಬು_image(filename, w, h, 3);
}

image get_image_layer(image m, ಪೂರ್ಣಾಂಕ l)
{
    image ಹೊರಹೋಗು = make_image(m.w, m.h, 1);
    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ(i = 0; i < m.h*m.w; ++i){
        ಹೊರಹೋಗು.ದತ್ತಾಂಶ[i] = m.ದತ್ತಾಂಶ[i+l*m.h*m.w];
    }
    ಹಿಂತಿರುಗಿ ಹೊರಹೋಗು;
}
ಶೂನ್ಯ print_image(image m)
{
    ಪೂರ್ಣಾಂಕ i, j, k;
    ಗಾಗಿ(i =0 ; i < m.c; ++i){
        ಗಾಗಿ(j =0 ; j < m.h; ++j){
            ಗಾಗಿ(k = 0; k < m.w; ++k){
                ಬರೆ("%.2lf, ", m.ದತ್ತಾಂಶ[i*m.h*m.w + j*m.w + k]);
                ಆದರೆ(k > 30) ನಿಲ್ಲಿಸು;
            }
            ಬರೆ("\n");
            ಆದರೆ(j > 30) ನಿಲ್ಲಿಸು;
        }
        ಬರೆ("\n");
    }
    ಬರೆ("\n");
}

image collapse_images_vert(image *ims, ಪೂರ್ಣಾಂಕ n)
{
    ಪೂರ್ಣಾಂಕ color = 1;
    ಪೂರ್ಣಾಂಕ border = 1;
    ಪೂರ್ಣಾಂಕ h,w,c;
    w = ims[0].w;
    h = (ims[0].h + border) * n - border;
    c = ims[0].c;
    ಆದರೆ(c != 3 || !color){
        w = (w+border)*c - border;
        c = 1;
    }

    image filters = make_image(w, h, c);
    ಪೂರ್ಣಾಂಕ i,j;
    ಗಾಗಿ(i = 0; i < n; ++i){
        ಪೂರ್ಣಾಂಕ h_ಸ್ಥಾನಾಂತರ = i*(ims[0].h+border);
        image copy = copy_image(ims[i]);
        //normalize_image(copy);
        ಆದರೆ(c == 3 && color){
            embed_image(copy, filters, 0, h_ಸ್ಥಾನಾಂತರ);
        }
        ಇಲ್ಲದಿದ್ದರೆ{
            ಗಾಗಿ(j = 0; j < copy.c; ++j){
                ಪೂರ್ಣಾಂಕ w_ಸ್ಥಾನಾಂತರ = j*(ims[0].w+border);
                image layer = get_image_layer(copy, j);
                embed_image(layer, filters, w_ಸ್ಥಾನಾಂತರ, h_ಸ್ಥಾನಾಂತರ);
                free_image(layer);
            }
        }
        free_image(copy);
    }
    ಹಿಂತಿರುಗಿ filters;
} 

image collapse_images_horz(image *ims, ಪೂರ್ಣಾಂಕ n)
{
    ಪೂರ್ಣಾಂಕ color = 1;
    ಪೂರ್ಣಾಂಕ border = 1;
    ಪೂರ್ಣಾಂಕ h,w,c;
    ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2 = ims[0].h;
    h = ಗಾತ್ರ_2;
    w = (ims[0].w + border) * n - border;
    c = ims[0].c;
    ಆದರೆ(c != 3 || !color){
        h = (h+border)*c - border;
        c = 1;
    }

    image filters = make_image(w, h, c);
    ಪೂರ್ಣಾಂಕ i,j;
    ಗಾಗಿ(i = 0; i < n; ++i){
        ಪೂರ್ಣಾಂಕ w_ಸ್ಥಾನಾಂತರ = i*(ಗಾತ್ರ_2+border);
        image copy = copy_image(ims[i]);
        //normalize_image(copy);
        ಆದರೆ(c == 3 && color){
            embed_image(copy, filters, w_ಸ್ಥಾನಾಂತರ, 0);
        }
        ಇಲ್ಲದಿದ್ದರೆ{
            ಗಾಗಿ(j = 0; j < copy.c; ++j){
                ಪೂರ್ಣಾಂಕ h_ಸ್ಥಾನಾಂತರ = j*(ಗಾತ್ರ_2+border);
                image layer = get_image_layer(copy, j);
                embed_image(layer, filters, w_ಸ್ಥಾನಾಂತರ, h_ಸ್ಥಾನಾಂತರ);
                free_image(layer);
            }
        }
        free_image(copy);
    }
    ಹಿಂತಿರುಗಿ filters;
} 

ಶೂನ್ಯ show_image_normalized(image im, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name)
{
    image c = copy_image(im);
    normalize_image(c);
    show_image(c, name, 1);
    free_image(c);
}

ಶೂನ್ಯ show_images(image *ims, ಪೂರ್ಣಾಂಕ n, ಅಕ್ಷರ *ಕಿಟಕಿ)
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
    save_image(m, ಕಿಟಕಿ);
    show_image(m, ಕಿಟಕಿ, 1);
    free_image(m);
}

ಶೂನ್ಯ free_image(image m)
{
    ಆದರೆ(m.ದತ್ತಾಂಶ){
        ಬಿಡುಗಡೆ(m.ದತ್ತಾಂಶ);
    }
}
