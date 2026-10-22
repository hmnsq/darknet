#అన్తర్భావ "image.h"
#అన్తర్భావ "utils.h"
#అన్తర్భావ "blas.h"
#అన్తర్భావ "cuda.h"
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <math.h>

#నిర్వచించు STB_IMAGE_IMPLEMENTATION
#అన్తర్భావ "stb_image.h"
#నిర్వచించు STB_IMAGE_WRITE_IMPLEMENTATION
#అన్తర్భావ "stb_image_write.h"

పూర్ణాంకం windows = 0;

దశాంశం colors[6][3] = { {1,0,1}, {0,0,1},{0,1,1},{0,1,0},{1,1,0},{1,0,0} };

దశాంశం get_color(పూర్ణాంకం c, పూర్ణాంకం x, పూర్ణాంకం max)
{
    దశాంశం నిష్పత్తి = ((దశాంశం)x/max)*5;
    పూర్ణాంకం i = floor(నిష్పత్తి);
    పూర్ణాంకం j = ceil(నిష్పత్తి);
    నిష్పత్తి -= i;
    దశాంశం r = (1-నిష్పత్తి) * colors[i][c] + నిష్పత్తి*colors[j][c];
    //printf("%f\n", r);
    తిరిగి r;
}

image ముసుగు_గమ్యం_rgb(image ముసుగు)
{
    పూర్ణాంకం n = ముసుగు.c;
    image im = make_image(ముసుగు.w, ముసుగు.h, 3);
    పూర్ణాంకం i, j;
    కొరకు(j = 0; j < n; ++j){
        పూర్ణాంకం స్థానభ్రంశం = j*123457 % n;
        దశాంశం red = get_color(2,స్థానభ్రంశం,n);
        దశాంశం green = get_color(1,స్థానభ్రంశం,n);
        దశాంశం blue = get_color(0,స్థానభ్రంశం,n);
        కొరకు(i = 0; i < im.w*im.h; ++i){
            im.దత్తాంశం[i + 0*im.w*im.h] += ముసుగు.దత్తాంశం[j*im.h*im.w + i]*red;
            im.దత్తాంశం[i + 1*im.w*im.h] += ముసుగు.దత్తాంశం[j*im.h*im.w + i]*green;
            im.దత్తాంశం[i + 2*im.w*im.h] += ముసుగు.దత్తాంశం[j*im.h*im.w + i]*blue;
        }
    }
    తిరిగి im;
}

స్థిర దశాంశం get_pixel(image m, పూర్ణాంకం x, పూర్ణాంకం y, పూర్ణాంకం c)
{
    ప్రతిజ్ఞా(x < m.w && y < m.h && c < m.c);
    తిరిగి m.దత్తాంశం[c*m.h*m.w + y*m.w + x];
}
స్థిర దశాంశం get_pixel_extend(image m, పూర్ణాంకం x, పూర్ణాంకం y, పూర్ణాంకం c)
{
    ఒకవేళ(x < 0 || x >= m.w || y < 0 || y >= m.h) తిరిగి 0;
    /*
    if(x < 0) x = 0;
    if(x >= m.w) x = m.w-1;
    if(y < 0) y = 0;
    if(y >= m.h) y = m.h-1;
    */
    ఒకవేళ(c < 0 || c >= m.c) తిరిగి 0;
    తిరిగి get_pixel(m, x, y, c);
}
స్థిర శూన్య సమితి_pixel(image m, పూర్ణాంకం x, పూర్ణాంకం y, పూర్ణాంకం c, దశాంశం val)
{
    ఒకవేళ (x < 0 || y < 0 || c < 0 || x >= m.w || y >= m.h || c >= m.c) తిరిగి;
    ప్రతిజ్ఞా(x < m.w && y < m.h && c < m.c);
    m.దత్తాంశం[c*m.h*m.w + y*m.w + x] = val;
}
స్థిర శూన్య కూడు_pixel(image m, పూర్ణాంకం x, పూర్ణాంకం y, పూర్ణాంకం c, దశాంశం val)
{
    ప్రతిజ్ఞా(x < m.w && y < m.h && c < m.c);
    m.దత్తాంశం[c*m.h*m.w + y*m.w + x] += val;
}

స్థిర దశాంశం bilinear_interpolate(image im, దశాంశం x, దశాంశం y, పూర్ణాంకం c)
{
    పూర్ణాంకం ix = (పూర్ణాంకం) floorf(x);
    పూర్ణాంకం iy = (పూర్ణాంకం) floorf(y);

    దశాంశం dx = x - ix;
    దశాంశం dy = y - iy;

    దశాంశం val = (1-dy) * (1-dx) * get_pixel_extend(im, ix, iy, c) + 
        dy     * (1-dx) * get_pixel_extend(im, ix, iy+1, c) + 
        (1-dy) *   dx   * get_pixel_extend(im, ix+1, iy, c) +
        dy     *   dx   * get_pixel_extend(im, ix+1, iy+1, c);
    తిరిగి val;
}


శూన్య సంయుక్త_image(image ఆధారం, image dest, పూర్ణాంకం dx, పూర్ణాంకం dy)
{
    పూర్ణాంకం x,y,k;
    కొరకు(k = 0; k < ఆధారం.c; ++k){
        కొరకు(y = 0; y < ఆధారం.h; ++y){
            కొరకు(x = 0; x < ఆధారం.w; ++x){
                దశాంశం val = get_pixel(ఆధారం, x, y, k);
                దశాంశం val2 = get_pixel_extend(dest, dx+x, dy+y, k);
                సమితి_pixel(dest, dx+x, dy+y, k, val * val2);
            }
        }
    }
}

image border_image(image a, పూర్ణాంకం border)
{
    image b = make_image(a.w + 2*border, a.h + 2*border, a.c);
    పూర్ణాంకం x,y,k;
    కొరకు(k = 0; k < b.c; ++k){
        కొరకు(y = 0; y < b.h; ++y){
            కొరకు(x = 0; x < b.w; ++x){
                దశాంశం val = get_pixel_extend(a, x - border, y - border, k);
                ఒకవేళ(x - border < 0 || x - border >= a.w || y - border < 0 || y - border >= a.h) val = 1;
                సమితి_pixel(b, x, y, k, val);
            }
        }
    }
    తిరిగి b;
}

image tile_images(image a, image b, పూర్ణాంకం dx)
{
    ఒకవేళ(a.w == 0) తిరిగి copy_image(b);
    image c = make_image(a.w + b.w + dx, (a.h > b.h) ? a.h : b.h, (a.c > b.c) ? a.c : b.c);
    fill_cpu(c.w*c.h*c.c, 1, c.దత్తాంశం, 1);
    embed_image(a, c, 0, 0); 
    సంయుక్త_image(b, c, a.w + dx, 0);
    తిరిగి c;
}

image get_label(image **characters, అక్షరం *string, పూర్ణాంకం కొలత)
{
    కొలత = కొలత/10;
    ఒకవేళ(కొలత > 7) కొలత = 7;
    image label = make_ఖాళీలు_image(0,0,0);
    అయితే(*string){
        image l = characters[కొలత][(పూర్ణాంకం)*string];
        image n = tile_images(label, l, -కొలత - 1 + (కొలత+1)/2);
        free_image(label);
        label = n;
        ++string;
    }
    image b = border_image(label, label.h*.25);
    free_image(label);
    తిరిగి b;
}

శూన్య draw_label(image a, పూర్ణాంకం r, పూర్ణాంకం c, image label, స్థిరరాశి దశాంశం *rgb)
{
    పూర్ణాంకం w = label.w;
    పూర్ణాంకం h = label.h;
    ఒకవేళ (r - h >= 0) r = r - h;

    పూర్ణాంకం i, j, k;
    కొరకు(j = 0; j < h && j + r < a.h; ++j){
        కొరకు(i = 0; i < w && i + c < a.w; ++i){
            కొరకు(k = 0; k < label.c; ++k){
                దశాంశం val = get_pixel(label, i, j, k);
                సమితి_pixel(a, i+c, j+r, k, rgb[k] * val);
            }
        }
    }
}

శూన్య draw_box(image a, పూర్ణాంకం x1, పూర్ణాంకం y1, పూర్ణాంకం x2, పూర్ణాంకం y2, దశాంశం r, దశాంశం g, దశాంశం b)
{
    //normalize_image(a);
    పూర్ణాంకం i;
    ఒకవేళ(x1 < 0) x1 = 0;
    ఒకవేళ(x1 >= a.w) x1 = a.w-1;
    ఒకవేళ(x2 < 0) x2 = 0;
    ఒకవేళ(x2 >= a.w) x2 = a.w-1;

    ఒకవేళ(y1 < 0) y1 = 0;
    ఒకవేళ(y1 >= a.h) y1 = a.h-1;
    ఒకవేళ(y2 < 0) y2 = 0;
    ఒకవేళ(y2 >= a.h) y2 = a.h-1;

    కొరకు(i = x1; i <= x2; ++i){
        a.దత్తాంశం[i + y1*a.w + 0*a.w*a.h] = r;
        a.దత్తాంశం[i + y2*a.w + 0*a.w*a.h] = r;

        a.దత్తాంశం[i + y1*a.w + 1*a.w*a.h] = g;
        a.దత్తాంశం[i + y2*a.w + 1*a.w*a.h] = g;

        a.దత్తాంశం[i + y1*a.w + 2*a.w*a.h] = b;
        a.దత్తాంశం[i + y2*a.w + 2*a.w*a.h] = b;
    }
    కొరకు(i = y1; i <= y2; ++i){
        a.దత్తాంశం[x1 + i*a.w + 0*a.w*a.h] = r;
        a.దత్తాంశం[x2 + i*a.w + 0*a.w*a.h] = r;

        a.దత్తాంశం[x1 + i*a.w + 1*a.w*a.h] = g;
        a.దత్తాంశం[x2 + i*a.w + 1*a.w*a.h] = g;

        a.దత్తాంశం[x1 + i*a.w + 2*a.w*a.h] = b;
        a.దత్తాంశం[x2 + i*a.w + 2*a.w*a.h] = b;
    }
}

శూన్య draw_box_వెడల్పు(image a, పూర్ణాంకం x1, పూర్ణాంకం y1, పూర్ణాంకం x2, పూర్ణాంకం y2, పూర్ణాంకం w, దశాంశం r, దశాంశం g, దశాంశం b)
{
    పూర్ణాంకం i;
    కొరకు(i = 0; i < w; ++i){
        draw_box(a, x1+i, y1+i, x2-i, y2-i, r, g, b);
    }
}

శూన్య draw_bbox(image a, box bbox, పూర్ణాంకం w, దశాంశం r, దశాంశం g, దశాంశం b)
{
    పూర్ణాంకం ఎడమ  = (bbox.x-bbox.w/2)*a.w;
    పూర్ణాంకం కుడి = (bbox.x+bbox.w/2)*a.w;
    పూర్ణాంకం శిఖరం   = (bbox.y-bbox.h/2)*a.h;
    పూర్ణాంకం bot   = (bbox.y+bbox.h/2)*a.h;

    పూర్ణాంకం i;
    కొరకు(i = 0; i < w; ++i){
        draw_box(a, ఎడమ+i, శిఖరం+i, కుడి-i, bot-i, r, g, b);
    }
}

image **ఎక్కించు_alphabet()
{
    పూర్ణాంకం i, j;
    స్థిరరాశి పూర్ణాంకం nsize = 8;
    image **alphabets = సున్నా_జ్ఞాపకం_ఇవ్వు(nsize, పరిమాణం(image));
    కొరకు(j = 0; j < nsize; ++j){
        alphabets[j] = సున్నా_జ్ఞాపకం_ఇవ్వు(128, పరిమాణం(image));
        కొరకు(i = 32; i < 127; ++i){
            అక్షరం buff[256];
            sprintf(buff, "data/labels/%d_%d.png", i, j);
            alphabets[j][i] = ఎక్కించు_image_color(buff, 0, 0);
        }
    }
    తిరిగి alphabets;
}

శూన్య draw_detections(image im, detection *dets, పూర్ణాంకం అంకె, దశాంశం thresh, అక్షరం **పేర్లు, image **alphabet, పూర్ణాంకం classes)
{
    పూర్ణాంకం i,j;

    కొరకు(i = 0; i < అంకె; ++i){
        అక్షరం labelstr[4096] = {0};
        పూర్ణాంకం class = -1;
        కొరకు(j = 0; j < classes; ++j){
            ఒకవేళ (dets[i].prob[j] > thresh){
                ఒకవేళ (class < 0) {
                    strcat(labelstr, పేర్లు[j]);
                    class = j;
                } లేకపోతే {
                    strcat(labelstr, ", ");
                    strcat(labelstr, పేర్లు[j]);
                }
                రాయి("%s: %.0f%%\n", పేర్లు[j], dets[i].prob[j]*100);
            }
        }
        ఒకవేళ(class >= 0){
            పూర్ణాంకం వెడల్పు = im.h * .006;

            /*
               if(0){
               width = pow(prob, 1./2.)*10+1;
               alphabet = 0;
               }
             */

            //printf("%d %s: %.0f%%\n", i, names[class], prob*100);
            పూర్ణాంకం స్థానభ్రంశం = class*123457 % classes;
            దశాంశం red = get_color(2,స్థానభ్రంశం,classes);
            దశాంశం green = get_color(1,స్థానభ్రంశం,classes);
            దశాంశం blue = get_color(0,స్థానభ్రంశం,classes);
            దశాంశం rgb[3];

            //width = prob*20+2;

            rgb[0] = red;
            rgb[1] = green;
            rgb[2] = blue;
            box b = dets[i].bbox;
            //printf("%f %f %f %f\n", b.x, b.y, b.w, b.h);

            పూర్ణాంకం ఎడమ  = (b.x-b.w/2.)*im.w;
            పూర్ణాంకం కుడి = (b.x+b.w/2.)*im.w;
            పూర్ణాంకం శిఖరం   = (b.y-b.h/2.)*im.h;
            పూర్ణాంకం bot   = (b.y+b.h/2.)*im.h;

            ఒకవేళ(ఎడమ < 0) ఎడమ = 0;
            ఒకవేళ(కుడి > im.w-1) కుడి = im.w-1;
            ఒకవేళ(శిఖరం < 0) శిఖరం = 0;
            ఒకవేళ(bot > im.h-1) bot = im.h-1;

            draw_box_వెడల్పు(im, ఎడమ, శిఖరం, కుడి, bot, వెడల్పు, red, green, blue);
            ఒకవేళ (alphabet) {
                image label = get_label(alphabet, labelstr, (im.h*.03));
                draw_label(im, శిఖరం + వెడల్పు, ఎడమ, label, rgb);
                free_image(label);
            }
            ఒకవేళ (dets[i].ముసుగు){
                image ముసుగు = float_గమ్యం_image(14, 14, 1, dets[i].ముసుగు);
                image resized_ముసుగు = resize_image(ముసుగు, b.w*im.w, b.h*im.h);
                image tmask = పరిమితి_image(resized_ముసుగు, .5);
                embed_image(tmask, im, ఎడమ, శిఖరం);
                free_image(ముసుగు);
                free_image(resized_ముసుగు);
                free_image(tmask);
            }
        }
    }
}

శూన్య transpose_image(image im)
{
    ప్రతిజ్ఞా(im.w == im.h);
    పూర్ణాంకం n, m;
    పూర్ణాంకం c;
    కొరకు(c = 0; c < im.c; ++c){
        కొరకు(n = 0; n < im.w-1; ++n){
            కొరకు(m = n + 1; m < im.w; ++m){
                దశాంశం మార్పిడి = im.దత్తాంశం[m + im.w*(n + im.h*c)];
                im.దత్తాంశం[m + im.w*(n + im.h*c)] = im.దత్తాంశం[n + im.w*(m + im.h*c)];
                im.దత్తాంశం[n + im.w*(m + im.h*c)] = మార్పిడి;
            }
        }
    }
}

శూన్య rotate_image_cw(image im, పూర్ణాంకం times)
{
    ప్రతిజ్ఞా(im.w == im.h);
    times = (times + 400) % 4;
    పూర్ణాంకం i, x, y, c;
    పూర్ణాంకం n = im.w;
    కొరకు(i = 0; i < times; ++i){
        కొరకు(c = 0; c < im.c; ++c){
            కొరకు(x = 0; x < n/2; ++x){
                కొరకు(y = 0; y < (n-1)/2 + 1; ++y){
                    దశాంశం తాత్కాలిక = im.దత్తాంశం[y + im.w*(x + im.h*c)];
                    im.దత్తాంశం[y + im.w*(x + im.h*c)] = im.దత్తాంశం[n-1-x + im.w*(y + im.h*c)];
                    im.దత్తాంశం[n-1-x + im.w*(y + im.h*c)] = im.దత్తాంశం[n-1-y + im.w*(n-1-x + im.h*c)];
                    im.దత్తాంశం[n-1-y + im.w*(n-1-x + im.h*c)] = im.దత్తాంశం[x + im.w*(n-1-y + im.h*c)];
                    im.దత్తాంశం[x + im.w*(n-1-y + im.h*c)] = తాత్కాలిక;
                }
            }
        }
    }
}

శూన్య flip_image(image a)
{
    పూర్ణాంకం i,j,k;
    కొరకు(k = 0; k < a.c; ++k){
        కొరకు(i = 0; i < a.h; ++i){
            కొరకు(j = 0; j < a.w/2; ++j){
                పూర్ణాంకం సూచీ = j + a.w*(i + a.h*(k));
                పూర్ణాంకం flip = (a.w - j - 1) + a.w*(i + a.h*(k));
                దశాంశం మార్పిడి = a.దత్తాంశం[flip];
                a.దత్తాంశం[flip] = a.దత్తాంశం[సూచీ];
                a.దత్తాంశం[సూచీ] = మార్పిడి;
            }
        }
    }
}

image image_దూరం(image a, image b)
{
    పూర్ణాంకం i,j;
    image dist = make_image(a.w, a.h, 1);
    కొరకు(i = 0; i < a.c; ++i){
        కొరకు(j = 0; j < a.h*a.w; ++j){
            dist.దత్తాంశం[j] += ఘాతం(a.దత్తాంశం[i*a.h*a.w+j]-b.దత్తాంశం[i*a.h*a.w+j],2);
        }
    }
    కొరకు(j = 0; j < a.h*a.w; ++j){
        dist.దత్తాంశం[j] = వర్గమూలం(dist.దత్తాంశం[j]);
    }
    తిరిగి dist;
}

శూన్య ghost_image(image ఆధారం, image dest, పూర్ణాంకం dx, పూర్ణాంకం dy)
{
    పూర్ణాంకం x,y,k;
    దశాంశం max_dist = వర్గమూలం((-ఆధారం.w/2. + .5)*(-ఆధారం.w/2. + .5));
    కొరకు(k = 0; k < ఆధారం.c; ++k){
        కొరకు(y = 0; y < ఆధారం.h; ++y){
            కొరకు(x = 0; x < ఆధారం.w; ++x){
                దశాంశం dist = వర్గమూలం((x - ఆధారం.w/2. + .5)*(x - ఆధారం.w/2. + .5) + (y - ఆధారం.h/2. + .5)*(y - ఆధారం.h/2. + .5));
                దశాంశం alpha = (1 - dist/max_dist);
                ఒకవేళ(alpha < 0) alpha = 0;
                దశాంశం v1 = get_pixel(ఆధారం, x,y,k);
                దశాంశం v2 = get_pixel(dest, dx+x,dy+y,k);
                దశాంశం val = alpha*v1 + (1-alpha)*v2;
                సమితి_pixel(dest, dx+x, dy+y, k, val);
            }
        }
    }
}

శూన్య blocky_image(image im, పూర్ణాంకం s)
{
    పూర్ణాంకం i,j,k;
    కొరకు(k = 0; k < im.c; ++k){
        కొరకు(j = 0; j < im.h; ++j){
            కొరకు(i = 0; i < im.w; ++i){
                im.దత్తాంశం[i + im.w*(j + im.h*k)] = im.దత్తాంశం[i/s*s + im.w*(j/s*s + im.h*k)];
            }
        }
    }
}

శూన్య censor_image(image im, పూర్ణాంకం dx, పూర్ణాంకం dy, పూర్ణాంకం w, పూర్ణాంకం h)
{
    పూర్ణాంకం i,j,k;
    పూర్ణాంకం s = 32;
    ఒకవేళ(dx < 0) dx = 0;
    ఒకవేళ(dy < 0) dy = 0;

    కొరకు(k = 0; k < im.c; ++k){
        కొరకు(j = dy; j < dy + h && j < im.h; ++j){
            కొరకు(i = dx; i < dx + w && i < im.w; ++i){
                im.దత్తాంశం[i + im.w*(j + im.h*k)] = im.దత్తాంశం[i/s*s + im.w*(j/s*s + im.h*k)];
                //im.data[i + j*im.w + k*im.w*im.h] = 0;
            }
        }
    }
}

శూన్య embed_image(image ఆధారం, image dest, పూర్ణాంకం dx, పూర్ణాంకం dy)
{
    పూర్ణాంకం x,y,k;
    కొరకు(k = 0; k < ఆధారం.c; ++k){
        కొరకు(y = 0; y < ఆధారం.h; ++y){
            కొరకు(x = 0; x < ఆధారం.w; ++x){
                దశాంశం val = get_pixel(ఆధారం, x,y,k);
                సమితి_pixel(dest, dx+x, dy+y, k, val);
            }
        }
    }
}

image collapse_image_layers(image ఆధారం, పూర్ణాంకం border)
{
    పూర్ణాంకం h = ఆధారం.h;
    h = (h+border)*ఆధారం.c - border;
    image dest = make_image(ఆధారం.w, h, 1);
    పూర్ణాంకం i;
    కొరకు(i = 0; i < ఆధారం.c; ++i){
        image layer = get_image_layer(ఆధారం, i);
        పూర్ణాంకం h_స్థానభ్రంశం = i*(ఆధారం.h+border);
        embed_image(layer, dest, 0, h_స్థానభ్రంశం);
        free_image(layer);
    }
    తిరిగి dest;
}

శూన్య constrain_image(image im)
{
    పూర్ణాంకం i;
    కొరకు(i = 0; i < im.w*im.h*im.c; ++i){
        ఒకవేళ(im.దత్తాంశం[i] < 0) im.దత్తాంశం[i] = 0;
        ఒకవేళ(im.దత్తాంశం[i] > 1) im.దత్తాంశం[i] = 1;
    }
}

శూన్య normalize_image(image p)
{
    పూర్ణాంకం i;
    దశాంశం min = 9999999;
    దశాంశం max = -999999;

    కొరకు(i = 0; i < p.h*p.w*p.c; ++i){
        దశాంశం v = p.దత్తాంశం[i];
        ఒకవేళ(v < min) min = v;
        ఒకవేళ(v > max) max = v;
    }
    ఒకవేళ(max - min < .000000001){
        min = 0;
        max = 1;
    }
    కొరకు(i = 0; i < p.c*p.w*p.h; ++i){
        p.దత్తాంశం[i] = (p.దత్తాంశం[i] - min)/(max-min);
    }
}

శూన్య normalize_image2(image p)
{
    దశాంశం *min = సున్నా_జ్ఞాపకం_ఇవ్వు(p.c, పరిమాణం(దశాంశం));
    దశాంశం *max = సున్నా_జ్ఞాపకం_ఇవ్వు(p.c, పరిమాణం(దశాంశం));
    పూర్ణాంకం i,j;
    కొరకు(i = 0; i < p.c; ++i) min[i] = max[i] = p.దత్తాంశం[i*p.h*p.w];

    కొరకు(j = 0; j < p.c; ++j){
        కొరకు(i = 0; i < p.h*p.w; ++i){
            దశాంశం v = p.దత్తాంశం[i+j*p.h*p.w];
            ఒకవేళ(v < min[j]) min[j] = v;
            ఒకవేళ(v > max[j]) max[j] = v;
        }
    }
    కొరకు(i = 0; i < p.c; ++i){
        ఒకవేళ(max[i] - min[i] < .000000001){
            min[i] = 0;
            max[i] = 1;
        }
    }
    కొరకు(j = 0; j < p.c; ++j){
        కొరకు(i = 0; i < p.w*p.h; ++i){
            p.దత్తాంశం[i+j*p.h*p.w] = (p.దత్తాంశం[i+j*p.h*p.w] - min[j])/(max[j]-min[j]);
        }
    }
    విడుదల(min);
    విడుదల(max);
}

శూన్య copy_image_into(image src, image dest)
{
    జ్ఞాపకం_నకలు(dest.దత్తాంశం, src.దత్తాంశం, src.h*src.w*src.c*పరిమాణం(దశాంశం));
}

image copy_image(image p)
{
    image copy = p;
    copy.దత్తాంశం = సున్నా_జ్ఞాపకం_ఇవ్వు(p.h*p.w*p.c, పరిమాణం(దశాంశం));
    జ్ఞాపకం_నకలు(copy.దత్తాంశం, p.దత్తాంశం, p.h*p.w*p.c*పరిమాణం(దశాంశం));
    తిరిగి copy;
}

శూన్య rgbgr_image(image im)
{
    పూర్ణాంకం i;
    కొరకు(i = 0; i < im.w*im.h; ++i){
        దశాంశం మార్పిడి = im.దత్తాంశం[i];
        im.దత్తాంశం[i] = im.దత్తాంశం[i+im.w*im.h*2];
        im.దత్తాంశం[i+im.w*im.h*2] = మార్పిడి;
    }
}

పూర్ణాంకం show_image(image p, స్థిరరాశి అక్షరం *name, పూర్ణాంకం ms)
{
#నిర్వచితమైతే OPENCV
    పూర్ణాంకం c = show_image_cv(p, name, ms);
    తిరిగి c;
#లేకపోతే
    fprintf(stderr, "Not compiled with OpenCV, saving to %s.png instead\n", name);
    save_image(p, name);
    తిరిగి -1;
#అయితే_ముగింపు
}

శూన్య save_image_options(image im, స్థిరరాశి అక్షరం *name, IMTYPE f, పూర్ణాంకం quality)
{
    అక్షరం buff[256];
    //sprintf(buff, "%s (%d)", name, windows);
    ఒకవేళ(f == PNG)       sprintf(buff, "%s.png", name);
    లేకపోతే ఒకవేళ (f == BMP) sprintf(buff, "%s.bmp", name);
    లేకపోతే ఒకవేళ (f == TGA) sprintf(buff, "%s.tga", name);
    లేకపోతే ఒకవేళ (f == JPG) sprintf(buff, "%s.jpg", name);
    లేకపోతే               sprintf(buff, "%s.png", name);
    గుర్తులేని అక్షరం *దత్తాంశం = సున్నా_జ్ఞాపకం_ఇవ్వు(im.w*im.h*im.c, పరిమాణం(అక్షరం));
    పూర్ణాంకం i,k;
    కొరకు(k = 0; k < im.c; ++k){
        కొరకు(i = 0; i < im.w*im.h; ++i){
            దత్తాంశం[i*im.c+k] = (గుర్తులేని అక్షరం) (255*im.దత్తాంశం[i + k*im.w*im.h]);
        }
    }
    పూర్ణాంకం success = 0;
    ఒకవేళ(f == PNG)       success = stbi_write_png(buff, im.w, im.h, im.c, దత్తాంశం, im.w*im.c);
    లేకపోతే ఒకవేళ (f == BMP) success = stbi_write_bmp(buff, im.w, im.h, im.c, దత్తాంశం);
    లేకపోతే ఒకవేళ (f == TGA) success = stbi_write_tga(buff, im.w, im.h, im.c, దత్తాంశం);
    లేకపోతే ఒకవేళ (f == JPG) success = stbi_write_jpg(buff, im.w, im.h, im.c, దత్తాంశం, quality);
    విడుదల(దత్తాంశం);
    ఒకవేళ(!success) fprintf(stderr, "Failed to write image %s\n", buff);
}

శూన్య save_image(image im, స్థిరరాశి అక్షరం *name)
{
    save_image_options(im, name, JPG, 80);
}

శూన్య show_image_layers(image p, అక్షరం *name)
{
    పూర్ణాంకం i;
    అక్షరం buff[256];
    కొరకు(i = 0; i < p.c; ++i){
        sprintf(buff, "%s - Layer %d", name, i);
        image layer = get_image_layer(p, i);
        show_image(layer, buff, 1);
        free_image(layer);
    }
}

శూన్య show_image_collapsed(image p, అక్షరం *name)
{
    image c = collapse_image_layers(p, 1);
    show_image(c, name, 1);
    free_image(c);
}

image make_ఖాళీలు_image(పూర్ణాంకం w, పూర్ణాంకం h, పూర్ణాంకం c)
{
    image బహిర్గతం;
    బహిర్గతం.దత్తాంశం = 0;
    బహిర్గతం.h = h;
    బహిర్గతం.w = w;
    బహిర్గతం.c = c;
    తిరిగి బహిర్గతం;
}

image make_image(పూర్ణాంకం w, పూర్ణాంకం h, పూర్ణాంకం c)
{
    image బహిర్గతం = make_ఖాళీలు_image(w,h,c);
    బహిర్గతం.దత్తాంశం = సున్నా_జ్ఞాపకం_ఇవ్వు(h*w*c, పరిమాణం(దశాంశం));
    తిరిగి బహిర్గతం;
}

image make_random_image(పూర్ణాంకం w, పూర్ణాంకం h, పూర్ణాంకం c)
{
    image బహిర్గతం = make_ఖాళీలు_image(w,h,c);
    బహిర్గతం.దత్తాంశం = సున్నా_జ్ఞాపకం_ఇవ్వు(h*w*c, పరిమాణం(దశాంశం));
    పూర్ణాంకం i;
    కొరకు(i = 0; i < w*h*c; ++i){
        బహిర్గతం.దత్తాంశం[i] = (rand_సాధారణ() * .25) + .5;
    }
    తిరిగి బహిర్గతం;
}

image float_గమ్యం_image(పూర్ణాంకం w, పూర్ణాంకం h, పూర్ణాంకం c, దశాంశం *దత్తాంశం)
{
    image బహిర్గతం = make_ఖాళీలు_image(w,h,c);
    బహిర్గతం.దత్తాంశం = దత్తాంశం;
    తిరిగి బహిర్గతం;
}

శూన్య ఉంచు_image(image im, పూర్ణాంకం w, పూర్ణాంకం h, పూర్ణాంకం dx, పూర్ణాంకం dy, image canvas)
{
    పూర్ణాంకం x, y, c;
    కొరకు(c = 0; c < im.c; ++c){
        కొరకు(y = 0; y < h; ++y){
            కొరకు(x = 0; x < w; ++x){
                దశాంశం rx = ((దశాంశం)x / w) * im.w;
                దశాంశం ry = ((దశాంశం)y / h) * im.h;
                దశాంశం val = bilinear_interpolate(im, rx, ry, c);
                సమితి_pixel(canvas, x + dx, y + dy, c, val);
            }
        }
    }
}

image center_crop_image(image im, పూర్ణాంకం w, పూర్ణాంకం h)
{
    పూర్ణాంకం m = (im.w < im.h) ? im.w : im.h;   
    image c = crop_image(im, (im.w - m) / 2, (im.h - m)/2, m, m);
    image r = resize_image(c, w, h);
    free_image(c);
    తిరిగి r;
}

image rotate_crop_image(image im, దశాంశం rad, దశాంశం s, పూర్ణాంకం w, పూర్ణాంకం h, దశాంశం dx, దశాంశం dy, దశాంశం aspect)
{
    పూర్ణాంకం x, y, c;
    దశాంశం cx = im.w/2.;
    దశాంశం cy = im.h/2.;
    image rot = make_image(w, h, im.c);
    కొరకు(c = 0; c < im.c; ++c){
        కొరకు(y = 0; y < h; ++y){
            కొరకు(x = 0; x < w; ++x){
                దశాంశం rx = కోజ్యా(rad)*((x - w/2.)/s*aspect + dx/s*aspect) - జ్యా(rad)*((y - h/2.)/s + dy/s) + cx;
                దశాంశం ry = జ్యా(rad)*((x - w/2.)/s*aspect + dx/s*aspect) + కోజ్యా(rad)*((y - h/2.)/s + dy/s) + cy;
                దశాంశం val = bilinear_interpolate(im, rx, ry, c);
                సమితి_pixel(rot, x, y, c, val);
            }
        }
    }
    తిరిగి rot;
}

image rotate_image(image im, దశాంశం rad)
{
    పూర్ణాంకం x, y, c;
    దశాంశం cx = im.w/2.;
    దశాంశం cy = im.h/2.;
    image rot = make_image(im.w, im.h, im.c);
    కొరకు(c = 0; c < im.c; ++c){
        కొరకు(y = 0; y < im.h; ++y){
            కొరకు(x = 0; x < im.w; ++x){
                దశాంశం rx = కోజ్యా(rad)*(x-cx) - జ్యా(rad)*(y-cy) + cx;
                దశాంశం ry = జ్యా(rad)*(x-cx) + కోజ్యా(rad)*(y-cy) + cy;
                దశాంశం val = bilinear_interpolate(im, rx, ry, c);
                సమితి_pixel(rot, x, y, c, val);
            }
        }
    }
    తిరిగి rot;
}

శూన్య fill_image(image m, దశాంశం s)
{
    పూర్ణాంకం i;
    కొరకు(i = 0; i < m.h*m.w*m.c; ++i) m.దత్తాంశం[i] = s;
}

శూన్య translate_image(image m, దశాంశం s)
{
    పూర్ణాంకం i;
    కొరకు(i = 0; i < m.h*m.w*m.c; ++i) m.దత్తాంశం[i] += s;
}

శూన్య scale_image(image m, దశాంశం s)
{
    పూర్ణాంకం i;
    కొరకు(i = 0; i < m.h*m.w*m.c; ++i) m.దత్తాంశం[i] *= s;
}

image crop_image(image im, పూర్ణాంకం dx, పూర్ణాంకం dy, పూర్ణాంకం w, పూర్ణాంకం h)
{
    image cropped = make_image(w, h, im.c);
    పూర్ణాంకం i, j, k;
    కొరకు(k = 0; k < im.c; ++k){
        కొరకు(j = 0; j < h; ++j){
            కొరకు(i = 0; i < w; ++i){
                పూర్ణాంకం r = j + dy;
                పూర్ణాంకం c = i + dx;
                దశాంశం val = 0;
                r = constrain_int(r, 0, im.h-1);
                c = constrain_int(c, 0, im.w-1);
                val = get_pixel(im, c, r, k);
                సమితి_pixel(cropped, i, j, k, val);
            }
        }
    }
    తిరిగి cropped;
}

పూర్ణాంకం ఉత్తమ_3d_జరుపు_r(image a, image b, పూర్ణాంకం min, పూర్ణాంకం max)
{
    ఒకవేళ(min == max) తిరిగి min;
    పూర్ణాంకం mid = floor((min + max) / 2.);
    image c1 = crop_image(b, 0, mid, b.w, b.h);
    image c2 = crop_image(b, 0, mid+1, b.w, b.h);
    దశాంశం d1 = dist_array(c1.దత్తాంశం, a.దత్తాంశం, a.w*a.h*a.c, 10);
    దశాంశం d2 = dist_array(c2.దత్తాంశం, a.దత్తాంశం, a.w*a.h*a.c, 10);
    free_image(c1);
    free_image(c2);
    ఒకవేళ(d1 < d2) తిరిగి ఉత్తమ_3d_జరుపు_r(a, b, min, mid);
    లేకపోతే తిరిగి ఉత్తమ_3d_జరుపు_r(a, b, mid+1, max);
}

పూర్ణాంకం ఉత్తమ_3d_జరుపు(image a, image b, పూర్ణాంకం min, పూర్ణాంకం max)
{
    పూర్ణాంకం i;
    పూర్ణాంకం ఉత్తమ = 0;
    దశాంశం ఉత్తమ_దూరం = FLT_MAX;
    కొరకు(i = min; i <= max; i += 2){
        image c = crop_image(b, 0, i, b.w, b.h);
        దశాంశం d = dist_array(c.దత్తాంశం, a.దత్తాంశం, a.w*a.h*a.c, 100);
        ఒకవేళ(d < ఉత్తమ_దూరం){
            ఉత్తమ_దూరం = d;
            ఉత్తమ = i;
        }
        రాయి("%d %f\n", i, d);
        free_image(c);
    }
    తిరిగి ఉత్తమ;
}

శూన్య సంయుక్త_3d(అక్షరం *f1, అక్షరం *f2, అక్షరం *బహిర్గతం, పూర్ణాంకం delta)
{
    ఒకవేళ(!బహిర్గతం) బహిర్గతం = "out";
    image a = ఎక్కించు_image(f1, 0,0,0);
    image b = ఎక్కించు_image(f2, 0,0,0);
    పూర్ణాంకం జరుపు = ఉత్తమ_3d_జరుపు_r(a, b, -a.h/100, a.h/100);

    image c1 = crop_image(b, 10, జరుపు, b.w, b.h);
    దశాంశం d1 = dist_array(c1.దత్తాంశం, a.దత్తాంశం, a.w*a.h*a.c, 100);
    image c2 = crop_image(b, -10, జరుపు, b.w, b.h);
    దశాంశం d2 = dist_array(c2.దత్తాంశం, a.దత్తాంశం, a.w*a.h*a.c, 100);

    ఒకవేళ(d2 < d1 && 0){
        image మార్పిడి = a;
        a = b;
        b = మార్పిడి;
        జరుపు = -జరుపు;
        రాయి("swapped, %d\n", జరుపు);
    }
    లేకపోతే{
        రాయి("%d\n", జరుపు);
    }

    image c = crop_image(b, delta, జరుపు, a.w, a.h);
    పూర్ణాంకం i;
    కొరకు(i = 0; i < c.w*c.h; ++i){
        c.దత్తాంశం[i] = a.దత్తాంశం[i];
    }
    save_image(c, బహిర్గతం);
}

శూన్య letterbox_image_into(image im, పూర్ణాంకం w, పూర్ణాంకం h, image boxed)
{
    పూర్ణాంకం new_w = im.w;
    పూర్ణాంకం new_h = im.h;
    ఒకవేళ (((దశాంశం)w/im.w) < ((దశాంశం)h/im.h)) {
        new_w = w;
        new_h = (im.h * w)/im.w;
    } లేకపోతే {
        new_h = h;
        new_w = (im.w * h)/im.h;
    }
    image resized = resize_image(im, new_w, new_h);
    embed_image(resized, boxed, (w-new_w)/2, (h-new_h)/2); 
    free_image(resized);
}

image letterbox_image(image im, పూర్ణాంకం w, పూర్ణాంకం h)
{
    పూర్ణాంకం new_w = im.w;
    పూర్ణాంకం new_h = im.h;
    ఒకవేళ (((దశాంశం)w/im.w) < ((దశాంశం)h/im.h)) {
        new_w = w;
        new_h = (im.h * w)/im.w;
    } లేకపోతే {
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
    తిరిగి boxed;
}

image resize_max(image im, పూర్ణాంకం max)
{
    పూర్ణాంకం w = im.w;
    పూర్ణాంకం h = im.h;
    ఒకవేళ(w > h){
        h = (h * max) / w;
        w = max;
    } లేకపోతే {
        w = (w * max) / h;
        h = max;
    }
    ఒకవేళ(w == im.w && h == im.h) తిరిగి im;
    image resized = resize_image(im, w, h);
    తిరిగి resized;
}

image resize_min(image im, పూర్ణాంకం min)
{
    పూర్ణాంకం w = im.w;
    పూర్ణాంకం h = im.h;
    ఒకవేళ(w < h){
        h = (h * min) / w;
        w = min;
    } లేకపోతే {
        w = (w * min) / h;
        h = min;
    }
    ఒకవేళ(w == im.w && h == im.h) తిరిగి im;
    image resized = resize_image(im, w, h);
    తిరిగి resized;
}

image random_crop_image(image im, పూర్ణాంకం w, పూర్ణాంకం h)
{
    పూర్ణాంకం dx = rand_int(0, im.w - w);
    పూర్ణాంకం dy = rand_int(0, im.h - h);
    image crop = crop_image(im, dx, dy, w, h);
    తిరిగి crop;
}

augment_వాదనలు random_augment_వాదనలు(image im, దశాంశం angle, దశాంశం aspect, పూర్ణాంకం దిగువ, పూర్ణాంకం ఎగువ, పూర్ణాంకం w, పూర్ణాంకం h)
{
    augment_వాదనలు a = {0};
    aspect = rand_scale(aspect);
    పూర్ణాంకం r = rand_int(దిగువ, ఎగువ);
    పూర్ణాంకం min = (im.h < im.w*aspect) ? im.h : im.w*aspect;
    దశాంశం scale = (దశాంశం)r / min;

    దశాంశం rad = rand_ఏకరీతి(-angle, angle) * TWO_PI / 360.;

    దశాంశం dx = (im.w*scale/aspect - w) / 2.;
    దశాంశం dy = (im.h*scale - w) / 2.;
    //if(dx < 0) dx = 0;
    //if(dy < 0) dy = 0;
    dx = rand_ఏకరీతి(-dx, dx);
    dy = rand_ఏకరీతి(-dy, dy);

    a.rad = rad;
    a.scale = scale;
    a.w = w;
    a.h = h;
    a.dx = dx;
    a.dy = dy;
    a.aspect = aspect;
    తిరిగి a;
}

image random_augment_image(image im, దశాంశం angle, దశాంశం aspect, పూర్ణాంకం దిగువ, పూర్ణాంకం ఎగువ, పూర్ణాంకం w, పూర్ణాంకం h)
{
    augment_వాదనలు a = random_augment_వాదనలు(im, angle, aspect, దిగువ, ఎగువ, w, h);
    image crop = rotate_crop_image(im, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);
    తిరిగి crop;
}

దశాంశం three_way_max(దశాంశం a, దశాంశం b, దశాంశం c)
{
    తిరిగి (a > b) ? ( (a > c) ? a : c) : ( (b > c) ? b : c) ;
}

దశాంశం three_way_min(దశాంశం a, దశాంశం b, దశాంశం c)
{
    తిరిగి (a < b) ? ( (a < c) ? a : c) : ( (b < c) ? b : c) ;
}

శూన్య yuv_గమ్యం_rgb(image im)
{
    ప్రతిజ్ఞా(im.c == 3);
    పూర్ణాంకం i, j;
    దశాంశం r, g, b;
    దశాంశం y, u, v;
    కొరకు(j = 0; j < im.h; ++j){
        కొరకు(i = 0; i < im.w; ++i){
            y = get_pixel(im, i , j, 0);
            u = get_pixel(im, i , j, 1);
            v = get_pixel(im, i , j, 2);

            r = y + 1.13983*v;
            g = y + -.39465*u + -.58060*v;
            b = y + 2.03211*u;

            సమితి_pixel(im, i, j, 0, r);
            సమితి_pixel(im, i, j, 1, g);
            సమితి_pixel(im, i, j, 2, b);
        }
    }
}

శూన్య rgb_గమ్యం_yuv(image im)
{
    ప్రతిజ్ఞా(im.c == 3);
    పూర్ణాంకం i, j;
    దశాంశం r, g, b;
    దశాంశం y, u, v;
    కొరకు(j = 0; j < im.h; ++j){
        కొరకు(i = 0; i < im.w; ++i){
            r = get_pixel(im, i , j, 0);
            g = get_pixel(im, i , j, 1);
            b = get_pixel(im, i , j, 2);

            y = .299*r + .587*g + .114*b;
            u = -.14713*r + -.28886*g + .436*b;
            v = .615*r + -.51499*g + -.10001*b;

            సమితి_pixel(im, i, j, 0, y);
            సమితి_pixel(im, i, j, 1, u);
            సమితి_pixel(im, i, j, 2, v);
        }
    }
}

// http://www.cs.rit.edu/~ncs/color/t_convert.html
శూన్య rgb_గమ్యం_hsv(image im)
{
    ప్రతిజ్ఞా(im.c == 3);
    పూర్ణాంకం i, j;
    దశాంశం r, g, b;
    దశాంశం h, s, v;
    కొరకు(j = 0; j < im.h; ++j){
        కొరకు(i = 0; i < im.w; ++i){
            r = get_pixel(im, i , j, 0);
            g = get_pixel(im, i , j, 1);
            b = get_pixel(im, i , j, 2);
            దశాంశం max = three_way_max(r,g,b);
            దశాంశం min = three_way_min(r,g,b);
            దశాంశం delta = max - min;
            v = max;
            ఒకవేళ(max == 0){
                s = 0;
                h = 0;
            }లేకపోతే{
                s = delta/max;
                ఒకవేళ(r == max){
                    h = (g - b) / delta;
                } లేకపోతే ఒకవేళ (g == max) {
                    h = 2 + (b - r) / delta;
                } లేకపోతే {
                    h = 4 + (r - g) / delta;
                }
                ఒకవేళ (h < 0) h += 6;
                h = h/6.;
            }
            సమితి_pixel(im, i, j, 0, h);
            సమితి_pixel(im, i, j, 1, s);
            సమితి_pixel(im, i, j, 2, v);
        }
    }
}

శూన్య hsv_గమ్యం_rgb(image im)
{
    ప్రతిజ్ఞా(im.c == 3);
    పూర్ణాంకం i, j;
    దశాంశం r, g, b;
    దశాంశం h, s, v;
    దశాంశం f, p, q, t;
    కొరకు(j = 0; j < im.h; ++j){
        కొరకు(i = 0; i < im.w; ++i){
            h = 6 * get_pixel(im, i , j, 0);
            s = get_pixel(im, i , j, 1);
            v = get_pixel(im, i , j, 2);
            ఒకవేళ (s == 0) {
                r = g = b = v;
            } లేకపోతే {
                పూర్ణాంకం సూచీ = floor(h);
                f = h - సూచీ;
                p = v*(1-s);
                q = v*(1-s*f);
                t = v*(1-s*(1-f));
                ఒకవేళ(సూచీ == 0){
                    r = v; g = t; b = p;
                } లేకపోతే ఒకవేళ(సూచీ == 1){
                    r = q; g = v; b = p;
                } లేకపోతే ఒకవేళ(సూచీ == 2){
                    r = p; g = v; b = t;
                } లేకపోతే ఒకవేళ(సూచీ == 3){
                    r = p; g = q; b = v;
                } లేకపోతే ఒకవేళ(సూచీ == 4){
                    r = t; g = p; b = v;
                } లేకపోతే {
                    r = v; g = p; b = q;
                }
            }
            సమితి_pixel(im, i, j, 0, r);
            సమితి_pixel(im, i, j, 1, g);
            సమితి_pixel(im, i, j, 2, b);
        }
    }
}

శూన్య grayscale_image_3c(image im)
{
    ప్రతిజ్ఞా(im.c == 3);
    పూర్ణాంకం i, j, k;
    దశాంశం scale[] = {0.299, 0.587, 0.114};
    కొరకు(j = 0; j < im.h; ++j){
        కొరకు(i = 0; i < im.w; ++i){
            దశాంశం val = 0;
            కొరకు(k = 0; k < 3; ++k){
                val += scale[k]*get_pixel(im, i, j, k);
            }
            im.దత్తాంశం[0*im.h*im.w + im.w*j + i] = val;
            im.దత్తాంశం[1*im.h*im.w + im.w*j + i] = val;
            im.దత్తాంశం[2*im.h*im.w + im.w*j + i] = val;
        }
    }
}

image grayscale_image(image im)
{
    ప్రతిజ్ఞా(im.c == 3);
    పూర్ణాంకం i, j, k;
    image బూడిద = make_image(im.w, im.h, 1);
    దశాంశం scale[] = {0.299, 0.587, 0.114};
    కొరకు(k = 0; k < im.c; ++k){
        కొరకు(j = 0; j < im.h; ++j){
            కొరకు(i = 0; i < im.w; ++i){
                బూడిద.దత్తాంశం[i+im.w*j] += scale[k]*get_pixel(im, i, j, k);
            }
        }
    }
    తిరిగి బూడిద;
}

image పరిమితి_image(image im, దశాంశం thresh)
{
    పూర్ణాంకం i;
    image t = make_image(im.w, im.h, im.c);
    కొరకు(i = 0; i < im.w*im.h*im.c; ++i){
        t.దత్తాంశం[i] = im.దత్తాంశం[i]>thresh ? 1 : 0;
    }
    తిరిగి t;
}

image blend_image(image fore, image వెనుక, దశాంశం alpha)
{
    ప్రతిజ్ఞా(fore.w == వెనుక.w && fore.h == వెనుక.h && fore.c == వెనుక.c);
    image blend = make_image(fore.w, fore.h, fore.c);
    పూర్ణాంకం i, j, k;
    కొరకు(k = 0; k < fore.c; ++k){
        కొరకు(j = 0; j < fore.h; ++j){
            కొరకు(i = 0; i < fore.w; ++i){
                దశాంశం val = alpha * get_pixel(fore, i, j, k) + 
                    (1 - alpha)* get_pixel(వెనుక, i, j, k);
                సమితి_pixel(blend, i, j, k, val);
            }
        }
    }
    తిరిగి blend;
}

శూన్య scale_image_channel(image im, పూర్ణాంకం c, దశాంశం v)
{
    పూర్ణాంకం i, j;
    కొరకు(j = 0; j < im.h; ++j){
        కొరకు(i = 0; i < im.w; ++i){
            దశాంశం pix = get_pixel(im, i, j, c);
            pix = pix*v;
            సమితి_pixel(im, i, j, c, pix);
        }
    }
}

శూన్య translate_image_channel(image im, పూర్ణాంకం c, దశాంశం v)
{
    పూర్ణాంకం i, j;
    కొరకు(j = 0; j < im.h; ++j){
        కొరకు(i = 0; i < im.w; ++i){
            దశాంశం pix = get_pixel(im, i, j, c);
            pix = pix+v;
            సమితి_pixel(im, i, j, c, pix);
        }
    }
}

image binarize_image(image im)
{
    image c = copy_image(im);
    పూర్ణాంకం i;
    కొరకు(i = 0; i < im.w * im.h * im.c; ++i){
        ఒకవేళ(c.దత్తాంశం[i] > .5) c.దత్తాంశం[i] = 1;
        లేకపోతే c.దత్తాంశం[i] = 0;
    }
    తిరిగి c;
}

శూన్య saturate_image(image im, దశాంశం sat)
{
    rgb_గమ్యం_hsv(im);
    scale_image_channel(im, 1, sat);
    hsv_గమ్యం_rgb(im);
    constrain_image(im);
}

శూన్య hue_image(image im, దశాంశం hue)
{
    rgb_గమ్యం_hsv(im);
    పూర్ణాంకం i;
    కొరకు(i = 0; i < im.w*im.h; ++i){
        im.దత్తాంశం[i] = im.దత్తాంశం[i] + hue;
        ఒకవేళ (im.దత్తాంశం[i] > 1) im.దత్తాంశం[i] -= 1;
        ఒకవేళ (im.దత్తాంశం[i] < 0) im.దత్తాంశం[i] += 1;
    }
    hsv_గమ్యం_rgb(im);
    constrain_image(im);
}

శూన్య exposure_image(image im, దశాంశం sat)
{
    rgb_గమ్యం_hsv(im);
    scale_image_channel(im, 2, sat);
    hsv_గమ్యం_rgb(im);
    constrain_image(im);
}

శూన్య distort_image(image im, దశాంశం hue, దశాంశం sat, దశాంశం val)
{
    rgb_గమ్యం_hsv(im);
    scale_image_channel(im, 1, sat);
    scale_image_channel(im, 2, val);
    పూర్ణాంకం i;
    కొరకు(i = 0; i < im.w*im.h; ++i){
        im.దత్తాంశం[i] = im.దత్తాంశం[i] + hue;
        ఒకవేళ (im.దత్తాంశం[i] > 1) im.దత్తాంశం[i] -= 1;
        ఒకవేళ (im.దత్తాంశం[i] < 0) im.దత్తాంశం[i] += 1;
    }
    hsv_గమ్యం_rgb(im);
    constrain_image(im);
}

శూన్య random_distort_image(image im, దశాంశం hue, దశాంశం saturation, దశాంశం exposure)
{
    దశాంశం dhue = rand_ఏకరీతి(-hue, hue);
    దశాంశం dsat = rand_scale(saturation);
    దశాంశం dexp = rand_scale(exposure);
    distort_image(im, dhue, dsat, dexp);
}

శూన్య saturate_exposure_image(image im, దశాంశం sat, దశాంశం exposure)
{
    rgb_గమ్యం_hsv(im);
    scale_image_channel(im, 1, sat);
    scale_image_channel(im, 2, exposure);
    hsv_గమ్యం_rgb(im);
    constrain_image(im);
}

image resize_image(image im, పూర్ణాంకం w, పూర్ణాంకం h)
{
    image resized = make_image(w, h, im.c);   
    image part = make_image(w, im.h, im.c);
    పూర్ణాంకం r, c, k;
    దశాంశం w_scale = (దశాంశం)(im.w - 1) / (w - 1);
    దశాంశం h_scale = (దశాంశం)(im.h - 1) / (h - 1);
    కొరకు(k = 0; k < im.c; ++k){
        కొరకు(r = 0; r < im.h; ++r){
            కొరకు(c = 0; c < w; ++c){
                దశాంశం val = 0;
                ఒకవేళ(c == w-1 || im.w == 1){
                    val = get_pixel(im, im.w-1, r, k);
                } లేకపోతే {
                    దశాంశం sx = c*w_scale;
                    పూర్ణాంకం ix = (పూర్ణాంకం) sx;
                    దశాంశం dx = sx - ix;
                    val = (1 - dx) * get_pixel(im, ix, r, k) + dx * get_pixel(im, ix+1, r, k);
                }
                సమితి_pixel(part, c, r, k, val);
            }
        }
    }
    కొరకు(k = 0; k < im.c; ++k){
        కొరకు(r = 0; r < h; ++r){
            దశాంశం sy = r*h_scale;
            పూర్ణాంకం iy = (పూర్ణాంకం) sy;
            దశాంశం dy = sy - iy;
            కొరకు(c = 0; c < w; ++c){
                దశాంశం val = (1-dy) * get_pixel(part, c, iy, k);
                సమితి_pixel(resized, c, r, k, val);
            }
            ఒకవేళ(r == h-1 || im.h == 1) కొనసాగు;
            కొరకు(c = 0; c < w; ++c){
                దశాంశం val = dy * get_pixel(part, c, iy+1, k);
                కూడు_pixel(resized, c, r, k, val);
            }
        }
    }

    free_image(part);
    తిరిగి resized;
}


శూన్య test_resize(అక్షరం *filename)
{
    image im = ఎక్కించు_image(filename, 0,0, 3);
    దశాంశం mag = mag_array(im.దత్తాంశం, im.w*im.h*im.c);
    రాయి("L2 Norm: %f\n", mag);
    image బూడిద = grayscale_image(im);

    image c1 = copy_image(im);
    image c2 = copy_image(im);
    image c3 = copy_image(im);
    image c4 = copy_image(im);
    distort_image(c1, .1, 1.5, 1.5);
    distort_image(c2, -.1, .66666, .66666);
    distort_image(c3, .1, 1.5, .66666);
    distort_image(c4, .1, .66666, 1.5);


    show_image(im,   "Original", 1);
    show_image(బూడిద, "Gray", 1);
    show_image(c1, "C1", 1);
    show_image(c2, "C2", 1);
    show_image(c3, "C3", 1);
    show_image(c4, "C4", 1);
#నిర్వచితమైతే OPENCV
    అయితే(1){
        image aug = random_augment_image(im, 0, .75, 320, 448, 320, 320);
        show_image(aug, "aug", 1);
        free_image(aug);


        దశాంశం exposure = 1.15;
        దశాంశం saturation = 1.15;
        దశాంశం hue = .05;

        image c = copy_image(im);

        దశాంశం dexp = rand_scale(exposure);
        దశాంశం dsat = rand_scale(saturation);
        దశాంశం dhue = rand_ఏకరీతి(-hue, hue);

        distort_image(c, dhue, dsat, dexp);
        show_image(c, "rand", 1);
        రాయి("%f %f %f\n", dhue, dsat, dexp);
        free_image(c);
    }
#అయితే_ముగింపు
}


image ఎక్కించు_image_stb(అక్షరం *filename, పూర్ణాంకం channels)
{
    పూర్ణాంకం w, h, c;
    గుర్తులేని అక్షరం *దత్తాంశం = stbi_ఎక్కించు(filename, &w, &h, &c, channels);
    ఒకవేళ (!దత్తాంశం) {
        fprintf(stderr, "Cannot load image \"%s\"\nSTB Reason: %s\n", filename, stbi_failure_reason());
        నిష్క్రమించు(0);
    }
    ఒకవేళ(channels) c = channels;
    పూర్ణాంకం i,j,k;
    image im = make_image(w, h, c);
    కొరకు(k = 0; k < c; ++k){
        కొరకు(j = 0; j < h; ++j){
            కొరకు(i = 0; i < w; ++i){
                పూర్ణాంకం dst_సూచీ = i + w*j + w*h*k;
                పూర్ణాంకం src_సూచీ = k + c*i + c*w*j;
                im.దత్తాంశం[dst_సూచీ] = (దశాంశం)దత్తాంశం[src_సూచీ]/255.;
            }
        }
    }
    విడుదల(దత్తాంశం);
    తిరిగి im;
}

image ఎక్కించు_image(అక్షరం *filename, పూర్ణాంకం w, పూర్ణాంకం h, పూర్ణాంకం c)
{
#నిర్వచితమైతే OPENCV
    image బహిర్గతం = ఎక్కించు_image_cv(filename, c);
#లేకపోతే
    image బహిర్గతం = ఎక్కించు_image_stb(filename, c);
#అయితే_ముగింపు

    ఒకవేళ((h && w) && (h != బహిర్గతం.h || w != బహిర్గతం.w)){
        image resized = resize_image(బహిర్గతం, w, h);
        free_image(బహిర్గతం);
        బహిర్గతం = resized;
    }
    తిరిగి బహిర్గతం;
}

image ఎక్కించు_image_color(అక్షరం *filename, పూర్ణాంకం w, పూర్ణాంకం h)
{
    తిరిగి ఎక్కించు_image(filename, w, h, 3);
}

image get_image_layer(image m, పూర్ణాంకం l)
{
    image బహిర్గతం = make_image(m.w, m.h, 1);
    పూర్ణాంకం i;
    కొరకు(i = 0; i < m.h*m.w; ++i){
        బహిర్గతం.దత్తాంశం[i] = m.దత్తాంశం[i+l*m.h*m.w];
    }
    తిరిగి బహిర్గతం;
}
శూన్య print_image(image m)
{
    పూర్ణాంకం i, j, k;
    కొరకు(i =0 ; i < m.c; ++i){
        కొరకు(j =0 ; j < m.h; ++j){
            కొరకు(k = 0; k < m.w; ++k){
                రాయి("%.2lf, ", m.దత్తాంశం[i*m.h*m.w + j*m.w + k]);
                ఒకవేళ(k > 30) ఆపు;
            }
            రాయి("\n");
            ఒకవేళ(j > 30) ఆపు;
        }
        రాయి("\n");
    }
    రాయి("\n");
}

image collapse_images_vert(image *ims, పూర్ణాంకం n)
{
    పూర్ణాంకం color = 1;
    పూర్ణాంకం border = 1;
    పూర్ణాంకం h,w,c;
    w = ims[0].w;
    h = (ims[0].h + border) * n - border;
    c = ims[0].c;
    ఒకవేళ(c != 3 || !color){
        w = (w+border)*c - border;
        c = 1;
    }

    image filters = make_image(w, h, c);
    పూర్ణాంకం i,j;
    కొరకు(i = 0; i < n; ++i){
        పూర్ణాంకం h_స్థానభ్రంశం = i*(ims[0].h+border);
        image copy = copy_image(ims[i]);
        //normalize_image(copy);
        ఒకవేళ(c == 3 && color){
            embed_image(copy, filters, 0, h_స్థానభ్రంశం);
        }
        లేకపోతే{
            కొరకు(j = 0; j < copy.c; ++j){
                పూర్ణాంకం w_స్థానభ్రంశం = j*(ims[0].w+border);
                image layer = get_image_layer(copy, j);
                embed_image(layer, filters, w_స్థానభ్రంశం, h_స్థానభ్రంశం);
                free_image(layer);
            }
        }
        free_image(copy);
    }
    తిరిగి filters;
} 

image collapse_images_horz(image *ims, పూర్ణాంకం n)
{
    పూర్ణాంకం color = 1;
    పూర్ణాంకం border = 1;
    పూర్ణాంకం h,w,c;
    పూర్ణాంకం కొలత = ims[0].h;
    h = కొలత;
    w = (ims[0].w + border) * n - border;
    c = ims[0].c;
    ఒకవేళ(c != 3 || !color){
        h = (h+border)*c - border;
        c = 1;
    }

    image filters = make_image(w, h, c);
    పూర్ణాంకం i,j;
    కొరకు(i = 0; i < n; ++i){
        పూర్ణాంకం w_స్థానభ్రంశం = i*(కొలత+border);
        image copy = copy_image(ims[i]);
        //normalize_image(copy);
        ఒకవేళ(c == 3 && color){
            embed_image(copy, filters, w_స్థానభ్రంశం, 0);
        }
        లేకపోతే{
            కొరకు(j = 0; j < copy.c; ++j){
                పూర్ణాంకం h_స్థానభ్రంశం = j*(కొలత+border);
                image layer = get_image_layer(copy, j);
                embed_image(layer, filters, w_స్థానభ్రంశం, h_స్థానభ్రంశం);
                free_image(layer);
            }
        }
        free_image(copy);
    }
    తిరిగి filters;
} 

శూన్య show_image_normalized(image im, స్థిరరాశి అక్షరం *name)
{
    image c = copy_image(im);
    normalize_image(c);
    show_image(c, name, 1);
    free_image(c);
}

శూన్య show_images(image *ims, పూర్ణాంకం n, అక్షరం *కిటికీ)
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
    save_image(m, కిటికీ);
    show_image(m, కిటికీ, 1);
    free_image(m);
}

శూన్య free_image(image m)
{
    ఒకవేళ(m.దత్తాంశం){
        విడుదల(m.దత్తాంశం);
    }
}
