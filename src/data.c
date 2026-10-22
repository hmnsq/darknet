#ضمن "data.h"
#ضمن "utils.h"
#ضمن "image.h"
#ضمن "cuda.h"

#ضمن <stdio.h>
#ضمن <stdlib.h>
#ضمن <string.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

list *get_paths(حرف *filename)
{
    حرف *مسیر;
    FILE *file = fopen(filename, "r");
    إذا(!file) file_error(filename);
    list *سطرها = make_list();
    طالما((مسیر=fgetl(file))){
        list_درج(سطرها, مسیر);
    }
    fclose(file);
    أرجع سطرها;
}

/*
char **get_random_paths_indexes(char **paths, int n, int m, int *indexes)
{
    char **random_paths = calloc(n, sizeof(char*));
    int i;
    pthread_mutex_lock(&mutex);
    for(i = 0; i < n; ++i){
        int index = rand()%m;
        indexes[i] = index;
        random_paths[i] = paths[index];
        if(i == 0) printf("%s\n", paths[index]);
    }
    pthread_mutex_unlock(&mutex);
    return random_paths;
}
*/

حرف **get_random_paths(حرف **paths, صحيح n, صحيح m)
{
    حرف **random_paths = حافظه_صفر_بده(n, حجم(حرف*));
    صحيح i;
    pthread_mutex_lock(&mutex);
    لكل(i = 0; i < n; ++i){
        صحيح شاخص = rand()%m;
        random_paths[i] = paths[شاخص];
        //if(i == 0) printf("%s\n", paths[index]);
    }
    pthread_mutex_unlock(&mutex);
    أرجع random_paths;
}

حرف **پیدا_کن_replace_paths(حرف **paths, صحيح n, حرف *پیدا_کن, حرف *replace)
{
    حرف **replace_paths = حافظه_صفر_بده(n, حجم(حرف*));
    صحيح i;
    لكل(i = 0; i < n; ++i){
        حرف replaced[4096];
        پیدا_کن_replace(paths[i], پیدا_کن, replace, replaced);
        replace_paths[i] = copy_string(replaced);
    }
    أرجع replace_paths;
}

ماتریس بارگیری_image_paths_خاکستری(حرف **paths, صحيح n, صحيح w, صحيح h)
{
    صحيح i;
    ماتریس X;
    X.ردیفها = n;
    X.vals = حافظه_صفر_بده(X.ردیفها, حجم(عائم*));
    X.تعداد_ستون = 0;

    لكل(i = 0; i < n; ++i){
        image im = بارگیری_image(paths[i], w, h, 3);

        image خاکستری = grayscale_image(im);
        free_image(im);
        im = خاکستری;

        X.vals[i] = im.داده;
        X.تعداد_ستون = im.h*im.w*im.c;
    }
    أرجع X;
}

ماتریس بارگیری_image_paths(حرف **paths, صحيح n, صحيح w, صحيح h)
{
    صحيح i;
    ماتریس X;
    X.ردیفها = n;
    X.vals = حافظه_صفر_بده(X.ردیفها, حجم(عائم*));
    X.تعداد_ستون = 0;

    لكل(i = 0; i < n; ++i){
        image im = بارگیری_image_color(paths[i], w, h);
        X.vals[i] = im.داده;
        X.تعداد_ستون = im.h*im.w*im.c;
    }
    أرجع X;
}

ماتریس بارگیری_image_augment_paths(حرف **paths, صحيح n, صحيح min, صحيح max, صحيح اندازه, عائم angle, عائم aspect, عائم hue, عائم saturation, عائم exposure, صحيح center)
{
    صحيح i;
    ماتریس X;
    X.ردیفها = n;
    X.vals = حافظه_صفر_بده(X.ردیفها, حجم(عائم*));
    X.تعداد_ستون = 0;

    لكل(i = 0; i < n; ++i){
        image im = بارگیری_image_color(paths[i], 0, 0);
        image crop;
        إذا(center){
            crop = center_crop_image(im, اندازه, اندازه);
        } وإلا {
            crop = random_augment_image(im, angle, aspect, min, max, اندازه, اندازه);
        }
        صحيح flip = rand()%2;
        إذا (flip) flip_image(crop);
        random_distort_image(crop, hue, saturation, exposure);

        /*
        show_image(im, "orig");
        show_image(crop, "crop");
        cvWaitKey(0);
        */
        //grayscale_image_3c(crop);
        free_image(im);
        X.vals[i] = crop.داده;
        X.تعداد_ستون = crop.h*crop.w*crop.c;
    }
    أرجع X;
}


box_label *read_boxes(حرف *filename, صحيح *n)
{
    FILE *file = fopen(filename, "r");
    إذا(!file) file_error(filename);
    عائم x, y, h, w;
    صحيح id;
    صحيح شمار = 0;
    صحيح اندازه = 64;
    box_label *boxes = حافظه_صفر_بده(اندازه, حجم(box_label));
    طالما(fscanf(file, "%d %f %f %f %f", &id, &x, &y, &w, &h) == 5){
        إذا(شمار == اندازه) {
            اندازه = اندازه * 2;
            boxes = realloc(boxes, اندازه*حجم(box_label));
        }
        boxes[شمار].id = id;
        boxes[شمار].x = x;
        boxes[شمار].y = y;
        boxes[شمار].h = h;
        boxes[شمار].w = w;
        boxes[شمار].چپ   = x - w/2;
        boxes[شمار].راست  = x + w/2;
        boxes[شمار].بالاترین    = y - h/2;
        boxes[شمار].bottom = y + h/2;
        ++شمار;
    }
    fclose(file);
    *n = شمار;
    أرجع boxes;
}

فراغ randomize_boxes(box_label *b, صحيح n)
{
    صحيح i;
    لكل(i = 0; i < n; ++i){
        box_label جابجا = b[i];
        صحيح شاخص = rand()%n;
        b[i] = b[شاخص];
        b[شاخص] = جابجا;
    }
}

فراغ correct_boxes(box_label *boxes, صحيح n, عائم dx, عائم dy, عائم sx, عائم sy, صحيح flip)
{
    صحيح i;
    لكل(i = 0; i < n; ++i){
        إذا(boxes[i].x == 0 && boxes[i].y == 0) {
            boxes[i].x = 999999;
            boxes[i].y = 999999;
            boxes[i].w = 999999;
            boxes[i].h = 999999;
            استمر;
        }
        boxes[i].چپ   = boxes[i].چپ  * sx - dx;
        boxes[i].راست  = boxes[i].راست * sx - dx;
        boxes[i].بالاترین    = boxes[i].بالاترین   * sy - dy;
        boxes[i].bottom = boxes[i].bottom* sy - dy;

        إذا(flip){
            عائم جابجا = boxes[i].چپ;
            boxes[i].چپ = 1. - boxes[i].راست;
            boxes[i].راست = 1. - جابجا;
        }

        boxes[i].چپ =  constrain(0, 1, boxes[i].چپ);
        boxes[i].راست = constrain(0, 1, boxes[i].راست);
        boxes[i].بالاترین =   constrain(0, 1, boxes[i].بالاترین);
        boxes[i].bottom =   constrain(0, 1, boxes[i].bottom);

        boxes[i].x = (boxes[i].چپ+boxes[i].راست)/2;
        boxes[i].y = (boxes[i].بالاترین+boxes[i].bottom)/2;
        boxes[i].w = (boxes[i].راست - boxes[i].چپ);
        boxes[i].h = (boxes[i].bottom - boxes[i].بالاترین);

        boxes[i].w = constrain(0, 1, boxes[i].w);
        boxes[i].h = constrain(0, 1, boxes[i].h);
    }
}

فراغ fill_truth_swag(حرف *مسیر, عائم *truth, صحيح classes, صحيح flip, عائم dx, عائم dy, عائم sx, عائم sy)
{
    حرف labelpath[4096];
    پیدا_کن_replace(مسیر, "images", "labels", labelpath);
    پیدا_کن_replace(labelpath, "JPEGImages", "labels", labelpath);
    پیدا_کن_replace(labelpath, ".jpg", ".txt", labelpath);
    پیدا_کن_replace(labelpath, ".JPG", ".txt", labelpath);
    پیدا_کن_replace(labelpath, ".JPEG", ".txt", labelpath);

    صحيح شمار = 0;
    box_label *boxes = read_boxes(labelpath, &شمار);
    randomize_boxes(boxes, شمار);
    correct_boxes(boxes, شمار, dx, dy, sx, sy, flip);
    عائم x,y,w,h;
    صحيح id;
    صحيح i;

    لكل (i = 0; i < شمار && i < 90; ++i) {
        x =  boxes[i].x;
        y =  boxes[i].y;
        w =  boxes[i].w;
        h =  boxes[i].h;
        id = boxes[i].id;

        إذا (w < .0 || h < .0) استمر;

        صحيح شاخص = (4+classes) * i;

        truth[شاخص++] = x;
        truth[شاخص++] = y;
        truth[شاخص++] = w;
        truth[شاخص++] = h;

        إذا (id < classes) truth[شاخص+id] = 1;
    }
    حرر(boxes);
}

فراغ fill_truth_region(حرف *مسیر, عائم *truth, صحيح classes, صحيح رقم_boxes, صحيح flip, عائم dx, عائم dy, عائم sx, عائم sy)
{
    حرف labelpath[4096];
    پیدا_کن_replace(مسیر, "images", "labels", labelpath);
    پیدا_کن_replace(labelpath, "JPEGImages", "labels", labelpath);

    پیدا_کن_replace(labelpath, ".jpg", ".txt", labelpath);
    پیدا_کن_replace(labelpath, ".png", ".txt", labelpath);
    پیدا_کن_replace(labelpath, ".JPG", ".txt", labelpath);
    پیدا_کن_replace(labelpath, ".JPEG", ".txt", labelpath);
    صحيح شمار = 0;
    box_label *boxes = read_boxes(labelpath, &شمار);
    randomize_boxes(boxes, شمار);
    correct_boxes(boxes, شمار, dx, dy, sx, sy, flip);
    عائم x,y,w,h;
    صحيح id;
    صحيح i;

    لكل (i = 0; i < شمار; ++i) {
        x =  boxes[i].x;
        y =  boxes[i].y;
        w =  boxes[i].w;
        h =  boxes[i].h;
        id = boxes[i].id;

        إذا (w < .005 || h < .005) استمر;

        صحيح ستون = (صحيح)(x*رقم_boxes);
        صحيح ردیف = (صحيح)(y*رقم_boxes);

        x = x*رقم_boxes - ستون;
        y = y*رقم_boxes - ردیف;

        صحيح شاخص = (ستون+ردیف*رقم_boxes)*(5+classes);
        إذا (truth[شاخص]) استمر;
        truth[شاخص++] = 1;

        إذا (id < classes) truth[شاخص+id] = 1;
        شاخص += classes;

        truth[شاخص++] = x;
        truth[شاخص++] = y;
        truth[شاخص++] = w;
        truth[شاخص++] = h;
    }
    حرر(boxes);
}

فراغ بارگیری_rle(image im, صحيح *rle, صحيح n)
{
    صحيح شمار = 0;
    صحيح curr = 0;
    صحيح i,j;
    لكل(i = 0; i < n; ++i){
        لكل(j = 0; j < rle[i]; ++j){
            im.داده[شمار++] = curr;
        }
        curr = 1 - curr;
    }
    لكل(; شمار < im.h*im.w*im.c; ++شمار){
        im.داده[شمار] = curr;
    }
}

فراغ or_image(image src, image dest, صحيح c)
{
    صحيح i;
    لكل(i = 0; i < src.w*src.h; ++i){
        إذا(src.داده[i]) dest.داده[dest.w*dest.h*c + i] = 1;
    }
}

فراغ exclusive_image(image src)
{
    صحيح k, j, i;
    صحيح s = src.w*src.h;
    لكل(k = 0; k < src.c-1; ++k){
        لكل(i = 0; i < s; ++i){
            إذا (src.داده[k*s + i]){
                لكل(j = k+1; j < src.c; ++j){
                    src.داده[j*s + i] = 0;
                }
            }
        }
    }
}

box bound_image(image im)
{
    صحيح x,y;
    صحيح minx = im.w;
    صحيح miny = im.h;
    صحيح maxx = 0;
    صحيح maxy = 0;
    لكل(y = 0; y < im.h; ++y){
        لكل(x = 0; x < im.w; ++x){
            إذا(im.داده[y*im.w + x]){
                minx = (x < minx) ? x : minx;
                miny = (y < miny) ? y : miny;
                maxx = (x > maxx) ? x : maxx;
                maxy = (y > maxy) ? y : maxy;
            }
        }
    }
    box b = {minx, miny, maxx-minx + 1, maxy-miny + 1};
    //printf("%f %f %f %f\n", b.x, b.y, b.w, b.h);
    أرجع b;
}

فراغ fill_truth_iseg(حرف *مسیر, صحيح رقم_boxes, عائم *truth, صحيح classes, صحيح w, صحيح h, augment_آرگومانها aug, صحيح flip, صحيح mw, صحيح mh)
{
    حرف labelpath[4096];
    پیدا_کن_replace(مسیر, "images", "mask", labelpath);
    پیدا_کن_replace(labelpath, "JPEGImages", "mask", labelpath);
    پیدا_کن_replace(labelpath, ".jpg", ".txt", labelpath);
    پیدا_کن_replace(labelpath, ".JPG", ".txt", labelpath);
    پیدا_کن_replace(labelpath, ".JPEG", ".txt", labelpath);
    FILE *file = fopen(labelpath, "r");
    إذا(!file) file_error(labelpath);
    حرف buff[32788];
    صحيح id;
    صحيح i = 0;
    صحيح j;
    image part = make_image(w, h, 1);
    طالما((fscanf(file, "%d %s", &id, buff) == 2) && i < رقم_boxes){
        صحيح n = 0;
        صحيح *rle = read_intlist(buff, &n, 0);
        بارگیری_rle(part, rle, n);
        image sized = rotate_crop_image(part, aug.rad, aug.scale, aug.w, aug.h, aug.dx, aug.dy, aug.aspect);
        إذا(flip) flip_image(sized);

        image ماسک = resize_image(sized, mw, mh);
        truth[i*(mw*mh+1)] = id;
        لكل(j = 0; j < mw*mh; ++j){
            truth[i*(mw*mh + 1) + 1 + j] = ماسک.داده[j];
        }
        ++i;

        free_image(ماسک);
        free_image(sized);
        حرر(rle);
    }
    إذا(i < رقم_boxes) truth[i*(mw*mh+1)] = -1;
    fclose(file);
    free_image(part);
}

فراغ fill_truth_ماسک(حرف *مسیر, صحيح رقم_boxes, عائم *truth, صحيح classes, صحيح w, صحيح h, augment_آرگومانها aug, صحيح flip, صحيح mw, صحيح mh)
{
    حرف labelpath[4096];
    پیدا_کن_replace(مسیر, "images", "mask", labelpath);
    پیدا_کن_replace(labelpath, "JPEGImages", "mask", labelpath);
    پیدا_کن_replace(labelpath, ".jpg", ".txt", labelpath);
    پیدا_کن_replace(labelpath, ".JPG", ".txt", labelpath);
    پیدا_کن_replace(labelpath, ".JPEG", ".txt", labelpath);
    FILE *file = fopen(labelpath, "r");
    إذا(!file) file_error(labelpath);
    حرف buff[32788];
    صحيح id;
    صحيح i = 0;
    image part = make_image(w, h, 1);
    طالما((fscanf(file, "%d %s", &id, buff) == 2) && i < رقم_boxes){
        صحيح n = 0;
        صحيح *rle = read_intlist(buff, &n, 0);
        بارگیری_rle(part, rle, n);
        image sized = rotate_crop_image(part, aug.rad, aug.scale, aug.w, aug.h, aug.dx, aug.dy, aug.aspect);
        إذا(flip) flip_image(sized);
        box b = bound_image(sized);
        إذا(b.w > 0){
            image crop = crop_image(sized, b.x, b.y, b.w, b.h);
            image ماسک = resize_image(crop, mw, mh);
            truth[i*(4 + mw*mh + 1) + 0] = (b.x + b.w/2.)/sized.w;
            truth[i*(4 + mw*mh + 1) + 1] = (b.y + b.h/2.)/sized.h;
            truth[i*(4 + mw*mh + 1) + 2] = b.w/sized.w;
            truth[i*(4 + mw*mh + 1) + 3] = b.h/sized.h;
            صحيح j;
            لكل(j = 0; j < mw*mh; ++j){
                truth[i*(4 + mw*mh + 1) + 4 + j] = ماسک.داده[j];
            }
            truth[i*(4 + mw*mh + 1) + 4 + mw*mh] = id;
            free_image(crop);
            free_image(ماسک);
            ++i;
        }
        free_image(sized);
        حرر(rle);
    }
    fclose(file);
    free_image(part);
}


فراغ fill_truth_detection(حرف *مسیر, صحيح رقم_boxes, عائم *truth, صحيح classes, صحيح flip, عائم dx, عائم dy, عائم sx, عائم sy)
{
    حرف labelpath[4096];
    پیدا_کن_replace(مسیر, "images", "labels", labelpath);
    پیدا_کن_replace(labelpath, "JPEGImages", "labels", labelpath);

    پیدا_کن_replace(labelpath, "raw", "labels", labelpath);
    پیدا_کن_replace(labelpath, ".jpg", ".txt", labelpath);
    پیدا_کن_replace(labelpath, ".png", ".txt", labelpath);
    پیدا_کن_replace(labelpath, ".JPG", ".txt", labelpath);
    پیدا_کن_replace(labelpath, ".JPEG", ".txt", labelpath);
    صحيح شمار = 0;
    box_label *boxes = read_boxes(labelpath, &شمار);
    randomize_boxes(boxes, شمار);
    correct_boxes(boxes, شمار, dx, dy, sx, sy, flip);
    إذا(شمار > رقم_boxes) شمار = رقم_boxes;
    عائم x,y,w,h;
    صحيح id;
    صحيح i;
    صحيح sub = 0;

    لكل (i = 0; i < شمار; ++i) {
        x =  boxes[i].x;
        y =  boxes[i].y;
        w =  boxes[i].w;
        h =  boxes[i].h;
        id = boxes[i].id;

        إذا ((w < .001 || h < .001)) {
            ++sub;
            استمر;
        }

        truth[(i-sub)*5+0] = x;
        truth[(i-sub)*5+1] = y;
        truth[(i-sub)*5+2] = w;
        truth[(i-sub)*5+3] = h;
        truth[(i-sub)*5+4] = id;
    }
    حرر(boxes);
}

#تعریف_کن NUMCHARS 37

فراغ print_letters(عائم *pred, صحيح n)
{
    صحيح i;
    لكل(i = 0; i < n; ++i){
        صحيح شاخص = max_شاخص(pred+i*NUMCHARS, NUMCHARS);
        چاپ_کن("%c", int_مقصد_alphanum(شاخص));
    }
    چاپ_کن("\n");
}

فراغ fill_truth_captcha(حرف *مسیر, صحيح n, عائم *truth)
{
    حرف *begin = strrchr(مسیر, '/');
    ++begin;
    صحيح i;
    لكل(i = 0; i < طول_رشته(begin) && i < n && begin[i] != '.'; ++i){
        صحيح شاخص = alphanum_مقصد_int(begin[i]);
        إذا(شاخص > 35) چاپ_کن("Bad %c\n", begin[i]);
        truth[i*NUMCHARS+شاخص] = 1;
    }
    لكل(;i < n; ++i){
        truth[i*NUMCHARS + NUMCHARS-1] = 1;
    }
}

داده بارگیری_داده_captcha(حرف **paths, صحيح n, صحيح m, صحيح k, صحيح w, صحيح h)
{
    إذا(m) paths = get_random_paths(paths, n, m);
    داده d = {0};
    d.shallow = 0;
    d.X = بارگیری_image_paths(paths, n, w, h);
    d.y = make_ماتریس(n, k*NUMCHARS);
    صحيح i;
    لكل(i = 0; i < n; ++i){
        fill_truth_captcha(paths[i], k, d.y.vals[i]);
    }
    إذا(m) حرر(paths);
    أرجع d;
}

داده بارگیری_داده_captcha_encode(حرف **paths, صحيح n, صحيح m, صحيح w, صحيح h)
{
    إذا(m) paths = get_random_paths(paths, n, m);
    داده d = {0};
    d.shallow = 0;
    d.X = بارگیری_image_paths(paths, n, w, h);
    d.X.تعداد_ستون = 17100;
    d.y = d.X;
    إذا(m) حرر(paths);
    أرجع d;
}

فراغ fill_truth(حرف *مسیر, حرف **labels, صحيح k, عائم *truth)
{
    صحيح i;
    memset(truth, 0, k*حجم(عائم));
    صحيح شمار = 0;
    لكل(i = 0; i < k; ++i){
        إذا(جستجوی_رشته(مسیر, labels[i])){
            truth[i] = 1;
            ++شمار;
            //printf("%s %s %d\n", path, labels[i], i);
        }
    }
    إذا(شمار != 1 && (k != 1 || شمار != 0)) چاپ_کن("Too many or too few labels: %d, %s\n", شمار, مسیر);
}

فراغ fill_hierarchy(عائم *truth, صحيح k, درخت *hierarchy)
{
    صحيح j;
    لكل(j = 0; j < k; ++j){
        إذا(truth[j]){
            صحيح والد = hierarchy->والد[j];
            طالما(والد >= 0){
                truth[والد] = 1;
                والد = hierarchy->والد[والد];
            }
        }
    }
    صحيح i;
    صحيح شمار = 0;
    لكل(j = 0; j < hierarchy->groups; ++j){
        //printf("%d\n", count);
        صحيح ماسک = 1;
        لكل(i = 0; i < hierarchy->گروه_اندازه[j]; ++i){
            إذا(truth[شمار + i]){
                ماسک = 0;
                اكسر;
            }
        }
        إذا (ماسک) {
            لكل(i = 0; i < hierarchy->گروه_اندازه[j]; ++i){
                truth[شمار + i] = SECRET_رقم;
            }
        }
        شمار += hierarchy->گروه_اندازه[j];
    }
}

ماتریس بارگیری_regression_labels_paths(حرف **paths, صحيح n, صحيح k)
{
    ماتریس y = make_ماتریس(n, k);
    صحيح i,j;
    لكل(i = 0; i < n; ++i){
        حرف labelpath[4096];
        پیدا_کن_replace(paths[i], "images", "labels", labelpath);
        پیدا_کن_replace(labelpath, "JPEGImages", "labels", labelpath);
        پیدا_کن_replace(labelpath, ".BMP", ".txt", labelpath);
        پیدا_کن_replace(labelpath, ".JPEG", ".txt", labelpath);
        پیدا_کن_replace(labelpath, ".JPG", ".txt", labelpath);
        پیدا_کن_replace(labelpath, ".JPeG", ".txt", labelpath);
        پیدا_کن_replace(labelpath, ".Jpeg", ".txt", labelpath);
        پیدا_کن_replace(labelpath, ".PNG", ".txt", labelpath);
        پیدا_کن_replace(labelpath, ".TIF", ".txt", labelpath);
        پیدا_کن_replace(labelpath, ".bmp", ".txt", labelpath);
        پیدا_کن_replace(labelpath, ".jpeg", ".txt", labelpath);
        پیدا_کن_replace(labelpath, ".jpg", ".txt", labelpath);
        پیدا_کن_replace(labelpath, ".png", ".txt", labelpath);
        پیدا_کن_replace(labelpath, ".tif", ".txt", labelpath);

        FILE *file = fopen(labelpath, "r");
        لكل(j = 0; j < k; ++j){
            fscanf(file, "%f", &(y.vals[i][j]));
        }
        fclose(file);
    }
    أرجع y;
}

ماتریس بارگیری_labels_paths(حرف **paths, صحيح n, حرف **labels, صحيح k, درخت *hierarchy)
{
    ماتریس y = make_ماتریس(n, k);
    صحيح i;
    لكل(i = 0; i < n && labels; ++i){
        fill_truth(paths[i], labels, k, y.vals[i]);
        إذا(hierarchy){
            fill_hierarchy(y.vals[i], k, hierarchy);
        }
    }
    أرجع y;
}

ماتریس بارگیری_برچسبها_paths(حرف **paths, صحيح n, صحيح k)
{
    ماتریس y = make_ماتریس(n, k);
    صحيح i;
    //int count = 0;
    لكل(i = 0; i < n; ++i){
        حرف label[4096];
        پیدا_کن_replace(paths[i], "images", "labels", label);
        پیدا_کن_replace(label, ".jpg", ".txt", label);
        FILE *file = fopen(label, "r");
        إذا (!file) استمر;
        //++count;
        صحيح برچسب;
        طالما(fscanf(file, "%d", &برچسب) == 1){
            إذا(برچسب < k){
                y.vals[i][برچسب] = 1;
            }
        }
        fclose(file);
    }
    //printf("%d/%d\n", count, n);
    أرجع y;
}

حرف **get_labels(حرف *filename)
{
    list *plist = get_paths(filename);
    حرف **labels = (حرف **)list_مقصد_array(plist);
    free_list(plist);
    أرجع labels;
}

فراغ free_داده(داده d)
{
    إذا(!d.shallow){
        free_ماتریس(d.X);
        free_ماتریس(d.y);
    }وإلا{
        حرر(d.X.vals);
        حرر(d.y.vals);
    }
}

image get_segmentation_image(حرف *مسیر, صحيح w, صحيح h, صحيح classes)
{
    حرف labelpath[4096];
    پیدا_کن_replace(مسیر, "images", "mask", labelpath);
    پیدا_کن_replace(labelpath, "JPEGImages", "mask", labelpath);
    پیدا_کن_replace(labelpath, ".jpg", ".txt", labelpath);
    پیدا_کن_replace(labelpath, ".JPG", ".txt", labelpath);
    پیدا_کن_replace(labelpath, ".JPEG", ".txt", labelpath);
    image ماسک = make_image(w, h, classes);
    FILE *file = fopen(labelpath, "r");
    إذا(!file) file_error(labelpath);
    حرف buff[32788];
    صحيح id;
    image part = make_image(w, h, 1);
    طالما(fscanf(file, "%d %s", &id, buff) == 2){
        صحيح n = 0;
        صحيح *rle = read_intlist(buff, &n, 0);
        بارگیری_rle(part, rle, n);
        or_image(part, ماسک, id);
        حرر(rle);
    }
    //exclusive_image(mask);
    fclose(file);
    free_image(part);
    أرجع ماسک;
}

image get_segmentation_image2(حرف *مسیر, صحيح w, صحيح h, صحيح classes)
{
    حرف labelpath[4096];
    پیدا_کن_replace(مسیر, "images", "mask", labelpath);
    پیدا_کن_replace(labelpath, "JPEGImages", "mask", labelpath);
    پیدا_کن_replace(labelpath, ".jpg", ".txt", labelpath);
    پیدا_کن_replace(labelpath, ".JPG", ".txt", labelpath);
    پیدا_کن_replace(labelpath, ".JPEG", ".txt", labelpath);
    image ماسک = make_image(w, h, classes+1);
    صحيح i;
    لكل(i = 0; i < w*h; ++i){
        ماسک.داده[w*h*classes + i] = 1;
    }
    FILE *file = fopen(labelpath, "r");
    إذا(!file) file_error(labelpath);
    حرف buff[32788];
    صحيح id;
    image part = make_image(w, h, 1);
    طالما(fscanf(file, "%d %s", &id, buff) == 2){
        صحيح n = 0;
        صحيح *rle = read_intlist(buff, &n, 0);
        بارگیری_rle(part, rle, n);
        or_image(part, ماسک, id);
        لكل(i = 0; i < w*h; ++i){
            إذا(part.داده[i]) ماسک.داده[w*h*classes + i] = 0;
        }
        حرر(rle);
    }
    //exclusive_image(mask);
    fclose(file);
    free_image(part);
    أرجع ماسک;
}

داده بارگیری_داده_seg(صحيح n, حرف **paths, صحيح m, صحيح w, صحيح h, صحيح classes, صحيح min, صحيح max, عائم angle, عائم aspect, عائم hue, عائم saturation, عائم exposure, صحيح div)
{
    حرف **random_paths = get_random_paths(paths, n, m);
    صحيح i;
    داده d = {0};
    d.shallow = 0;

    d.X.ردیفها = n;
    d.X.vals = حافظه_صفر_بده(d.X.ردیفها, حجم(عائم*));
    d.X.تعداد_ستون = h*w*3;


    d.y.ردیفها = n;
    d.y.تعداد_ستون = h*w*classes/div/div;
    d.y.vals = حافظه_صفر_بده(d.X.ردیفها, حجم(عائم*));

    لكل(i = 0; i < n; ++i){
        image orig = بارگیری_image_color(random_paths[i], 0, 0);
        augment_آرگومانها a = random_augment_آرگومانها(orig, angle, aspect, min, max, w, h);
        image sized = rotate_crop_image(orig, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);

        صحيح flip = rand()%2;
        إذا(flip) flip_image(sized);
        random_distort_image(sized, hue, saturation, exposure);
        d.X.vals[i] = sized.داده;

        image ماسک = get_segmentation_image(random_paths[i], orig.w, orig.h, classes);
        //image mask = make_image(orig.w, orig.h, classes+1);
        image sized_m = rotate_crop_image(ماسک, a.rad, a.scale/div, a.w/div, a.h/div, a.dx/div, a.dy/div, a.aspect);

        إذا(flip) flip_image(sized_m);
        d.y.vals[i] = sized_m.داده;

        free_image(orig);
        free_image(ماسک);

        /*
           image rgb = mask_to_rgb(sized_m, classes);
           show_image(rgb, "part");
           show_image(sized, "orig");
           cvWaitKey(0);
           free_image(rgb);
         */
    }
    حرر(random_paths);
    أرجع d;
}

داده بارگیری_داده_iseg(صحيح n, حرف **paths, صحيح m, صحيح w, صحيح h, صحيح classes, صحيح boxes, صحيح div, صحيح min, صحيح max, عائم angle, عائم aspect, عائم hue, عائم saturation, عائم exposure)
{
    حرف **random_paths = get_random_paths(paths, n, m);
    صحيح i;
    داده d = {0};
    d.shallow = 0;

    d.X.ردیفها = n;
    d.X.vals = حافظه_صفر_بده(d.X.ردیفها, حجم(عائم*));
    d.X.تعداد_ستون = h*w*3;

    d.y = make_ماتریس(n, (((w/div)*(h/div))+1)*boxes);

    لكل(i = 0; i < n; ++i){
        image orig = بارگیری_image_color(random_paths[i], 0, 0);
        augment_آرگومانها a = random_augment_آرگومانها(orig, angle, aspect, min, max, w, h);
        image sized = rotate_crop_image(orig, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);

        صحيح flip = rand()%2;
        إذا(flip) flip_image(sized);
        random_distort_image(sized, hue, saturation, exposure);
        d.X.vals[i] = sized.داده;
        //show_image(sized, "image");

        fill_truth_iseg(random_paths[i], boxes, d.y.vals[i], classes, orig.w, orig.h, a, flip, w/div, h/div);

        free_image(orig);

        /*
           image rgb = mask_to_rgb(sized_m, classes);
           show_image(rgb, "part");
           show_image(sized, "orig");
           cvWaitKey(0);
           free_image(rgb);
         */
    }
    حرر(random_paths);
    أرجع d;
}

داده بارگیری_داده_ماسک(صحيح n, حرف **paths, صحيح m, صحيح w, صحيح h, صحيح classes, صحيح boxes, صحيح coords, صحيح min, صحيح max, عائم angle, عائم aspect, عائم hue, عائم saturation, عائم exposure)
{
    حرف **random_paths = get_random_paths(paths, n, m);
    صحيح i;
    داده d = {0};
    d.shallow = 0;

    d.X.ردیفها = n;
    d.X.vals = حافظه_صفر_بده(d.X.ردیفها, حجم(عائم*));
    d.X.تعداد_ستون = h*w*3;

    d.y = make_ماتریس(n, (coords+1)*boxes);

    لكل(i = 0; i < n; ++i){
        image orig = بارگیری_image_color(random_paths[i], 0, 0);
        augment_آرگومانها a = random_augment_آرگومانها(orig, angle, aspect, min, max, w, h);
        image sized = rotate_crop_image(orig, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);

        صحيح flip = rand()%2;
        إذا(flip) flip_image(sized);
        random_distort_image(sized, hue, saturation, exposure);
        d.X.vals[i] = sized.داده;
        //show_image(sized, "image");

        fill_truth_ماسک(random_paths[i], boxes, d.y.vals[i], classes, orig.w, orig.h, a, flip, 14, 14);

        free_image(orig);

        /*
           image rgb = mask_to_rgb(sized_m, classes);
           show_image(rgb, "part");
           show_image(sized, "orig");
           cvWaitKey(0);
           free_image(rgb);
         */
    }
    حرر(random_paths);
    أرجع d;
}

داده بارگیری_داده_region(صحيح n, حرف **paths, صحيح m, صحيح w, صحيح h, صحيح اندازه, صحيح classes, عائم jitter, عائم hue, عائم saturation, عائم exposure)
{
    حرف **random_paths = get_random_paths(paths, n, m);
    صحيح i;
    داده d = {0};
    d.shallow = 0;

    d.X.ردیفها = n;
    d.X.vals = حافظه_صفر_بده(d.X.ردیفها, حجم(عائم*));
    d.X.تعداد_ستون = h*w*3;


    صحيح k = اندازه*اندازه*(5+classes);
    d.y = make_ماتریس(n, k);
    لكل(i = 0; i < n; ++i){
        image orig = بارگیری_image_color(random_paths[i], 0, 0);

        صحيح oh = orig.h;
        صحيح ow = orig.w;

        صحيح dw = (ow*jitter);
        صحيح dh = (oh*jitter);

        صحيح pleft  = rand_یکنواخت(-dw, dw);
        صحيح pright = rand_یکنواخت(-dw, dw);
        صحيح ptop   = rand_یکنواخت(-dh, dh);
        صحيح pbot   = rand_یکنواخت(-dh, dh);

        صحيح swidth =  ow - pleft - pright;
        صحيح sheight = oh - ptop - pbot;

        عائم sx = (عائم)swidth  / ow;
        عائم sy = (عائم)sheight / oh;

        صحيح flip = rand()%2;
        image cropped = crop_image(orig, pleft, ptop, swidth, sheight);

        عائم dx = ((عائم)pleft/ow)/sx;
        عائم dy = ((عائم)ptop /oh)/sy;

        image sized = resize_image(cropped, w, h);
        إذا(flip) flip_image(sized);
        random_distort_image(sized, hue, saturation, exposure);
        d.X.vals[i] = sized.داده;

        fill_truth_region(random_paths[i], d.y.vals[i], classes, اندازه, flip, dx, dy, 1./sx, 1./sy);

        free_image(orig);
        free_image(cropped);
    }
    حرر(random_paths);
    أرجع d;
}

داده بارگیری_داده_compare(صحيح n, حرف **paths, صحيح m, صحيح classes, صحيح w, صحيح h)
{
    إذا(m) paths = get_random_paths(paths, 2*n, m);
    صحيح i,j;
    داده d = {0};
    d.shallow = 0;

    d.X.ردیفها = n;
    d.X.vals = حافظه_صفر_بده(d.X.ردیفها, حجم(عائم*));
    d.X.تعداد_ستون = h*w*6;

    صحيح k = 2*(classes);
    d.y = make_ماتریس(n, k);
    لكل(i = 0; i < n; ++i){
        image im1 = بارگیری_image_color(paths[i*2],   w, h);
        image im2 = بارگیری_image_color(paths[i*2+1], w, h);

        d.X.vals[i] = حافظه_صفر_بده(d.X.تعداد_ستون, حجم(عائم));
        کپی_حافظه(d.X.vals[i],         im1.داده, h*w*3*حجم(عائم));
        کپی_حافظه(d.X.vals[i] + h*w*3, im2.داده, h*w*3*حجم(عائم));

        صحيح id;
        عائم iou;

        حرف imlabel1[4096];
        حرف imlabel2[4096];
        پیدا_کن_replace(paths[i*2],   "imgs", "labels", imlabel1);
        پیدا_کن_replace(imlabel1, "jpg", "txt", imlabel1);
        FILE *fp1 = fopen(imlabel1, "r");

        طالما(fscanf(fp1, "%d %f", &id, &iou) == 2){
            إذا (d.y.vals[i][2*id] < iou) d.y.vals[i][2*id] = iou;
        }

        پیدا_کن_replace(paths[i*2+1], "imgs", "labels", imlabel2);
        پیدا_کن_replace(imlabel2, "jpg", "txt", imlabel2);
        FILE *fp2 = fopen(imlabel2, "r");

        طالما(fscanf(fp2, "%d %f", &id, &iou) == 2){
            إذا (d.y.vals[i][2*id + 1] < iou) d.y.vals[i][2*id + 1] = iou;
        }

        لكل (j = 0; j < classes; ++j){
            إذا (d.y.vals[i][2*j] > .5 &&  d.y.vals[i][2*j+1] < .5){
                d.y.vals[i][2*j] = 1;
                d.y.vals[i][2*j+1] = 0;
            } وإلا إذا (d.y.vals[i][2*j] < .5 &&  d.y.vals[i][2*j+1] > .5){
                d.y.vals[i][2*j] = 0;
                d.y.vals[i][2*j+1] = 1;
            } وإلا {
                d.y.vals[i][2*j]   = SECRET_رقم;
                d.y.vals[i][2*j+1] = SECRET_رقم;
            }
        }
        fclose(fp1);
        fclose(fp2);

        free_image(im1);
        free_image(im2);
    }
    إذا(m) حرر(paths);
    أرجع d;
}

داده بارگیری_داده_swag(حرف **paths, صحيح n, صحيح classes, عائم jitter)
{
    صحيح شاخص = rand()%n;
    حرف *random_مسیر = paths[شاخص];

    image orig = بارگیری_image_color(random_مسیر, 0, 0);
    صحيح h = orig.h;
    صحيح w = orig.w;

    داده d = {0};
    d.shallow = 0;
    d.w = w;
    d.h = h;

    d.X.ردیفها = 1;
    d.X.vals = حافظه_صفر_بده(d.X.ردیفها, حجم(عائم*));
    d.X.تعداد_ستون = h*w*3;

    صحيح k = (4+classes)*90;
    d.y = make_ماتریس(1, k);

    صحيح dw = w*jitter;
    صحيح dh = h*jitter;

    صحيح pleft  = rand_یکنواخت(-dw, dw);
    صحيح pright = rand_یکنواخت(-dw, dw);
    صحيح ptop   = rand_یکنواخت(-dh, dh);
    صحيح pbot   = rand_یکنواخت(-dh, dh);

    صحيح swidth =  w - pleft - pright;
    صحيح sheight = h - ptop - pbot;

    عائم sx = (عائم)swidth  / w;
    عائم sy = (عائم)sheight / h;

    صحيح flip = rand()%2;
    image cropped = crop_image(orig, pleft, ptop, swidth, sheight);

    عائم dx = ((عائم)pleft/w)/sx;
    عائم dy = ((عائم)ptop /h)/sy;

    image sized = resize_image(cropped, w, h);
    إذا(flip) flip_image(sized);
    d.X.vals[0] = sized.داده;

    fill_truth_swag(random_مسیر, d.y.vals[0], classes, flip, dx, dy, 1./sx, 1./sy);

    free_image(orig);
    free_image(cropped);

    أرجع d;
}

داده بارگیری_داده_detection(صحيح n, حرف **paths, صحيح m, صحيح w, صحيح h, صحيح boxes, صحيح classes, عائم jitter, عائم hue, عائم saturation, عائم exposure)
{
    حرف **random_paths = get_random_paths(paths, n, m);
    صحيح i;
    داده d = {0};
    d.shallow = 0;

    d.X.ردیفها = n;
    d.X.vals = حافظه_صفر_بده(d.X.ردیفها, حجم(عائم*));
    d.X.تعداد_ستون = h*w*3;

    d.y = make_ماتریس(n, 5*boxes);
    لكل(i = 0; i < n; ++i){
        image orig = بارگیری_image_color(random_paths[i], 0, 0);
        image sized = make_image(w, h, orig.c);
        fill_image(sized, .5);

        عائم dw = jitter * orig.w;
        عائم dh = jitter * orig.h;

        عائم new_ar = (orig.w + rand_یکنواخت(-dw, dw)) / (orig.h + rand_یکنواخت(-dh, dh));
        //float scale = rand_uniform(.25, 2);
        عائم scale = 1;

        عائم nw, nh;

        إذا(new_ar < 1){
            nh = scale * h;
            nw = nh * new_ar;
        } وإلا {
            nw = scale * w;
            nh = nw / new_ar;
        }

        عائم dx = rand_یکنواخت(0, w - nw);
        عائم dy = rand_یکنواخت(0, h - nh);

        قراربده_image(orig, nw, nh, dx, dy, sized);

        random_distort_image(sized, hue, saturation, exposure);

        صحيح flip = rand()%2;
        إذا(flip) flip_image(sized);
        d.X.vals[i] = sized.داده;


        fill_truth_detection(random_paths[i], boxes, d.y.vals[i], classes, flip, -dx/w, -dy/h, nw/w, nh/h);

        free_image(orig);
    }
    حرر(random_paths);
    أرجع d;
}

فراغ *بارگیری_thread(فراغ *ptr)
{
    //printf("Loading data: %d\n", rand());
    بارگیری_آرگومانها a = *(بنية بارگیری_آرگومانها*)ptr;
    إذا(a.exposure == 0) a.exposure = 1;
    إذا(a.saturation == 0) a.saturation = 1;
    إذا(a.aspect == 0) a.aspect = 1;

    إذا (a.type == OLD_CLASSIFICATION_داده){
        *a.d = بارگیری_داده_old(a.paths, a.n, a.m, a.labels, a.classes, a.w, a.h);
    } وإلا إذا (a.type == REGRESSION_داده){
        *a.d = بارگیری_داده_regression(a.paths, a.n, a.m, a.classes, a.min, a.max, a.اندازه, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    } وإلا إذا (a.type == CLASSIFICATION_داده){
        *a.d = بارگیری_داده_augment(a.paths, a.n, a.m, a.labels, a.classes, a.hierarchy, a.min, a.max, a.اندازه, a.angle, a.aspect, a.hue, a.saturation, a.exposure, a.center);
    } وإلا إذا (a.type == SUPER_داده){
        *a.d = بارگیری_داده_super(a.paths, a.n, a.m, a.w, a.h, a.scale);
    } وإلا إذا (a.type == WRITING_داده){
        *a.d = بارگیری_داده_writing(a.paths, a.n, a.m, a.w, a.h, a.بیرون_w, a.بیرون_h);
    } وإلا إذا (a.type == ISEG_داده){
        *a.d = بارگیری_داده_iseg(a.n, a.paths, a.m, a.w, a.h, a.classes, a.رقم_boxes, a.scale, a.min, a.max, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    } وإلا إذا (a.type == INSTANCE_داده){
        *a.d = بارگیری_داده_ماسک(a.n, a.paths, a.m, a.w, a.h, a.classes, a.رقم_boxes, a.coords, a.min, a.max, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    } وإلا إذا (a.type == SEGMENTATION_داده){
        *a.d = بارگیری_داده_seg(a.n, a.paths, a.m, a.w, a.h, a.classes, a.min, a.max, a.angle, a.aspect, a.hue, a.saturation, a.exposure, a.scale);
    } وإلا إذا (a.type == REGION_داده){
        *a.d = بارگیری_داده_region(a.n, a.paths, a.m, a.w, a.h, a.رقم_boxes, a.classes, a.jitter, a.hue, a.saturation, a.exposure);
    } وإلا إذا (a.type == DETECTION_داده){
        *a.d = بارگیری_داده_detection(a.n, a.paths, a.m, a.w, a.h, a.رقم_boxes, a.classes, a.jitter, a.hue, a.saturation, a.exposure);
    } وإلا إذا (a.type == SWAG_داده){
        *a.d = بارگیری_داده_swag(a.paths, a.n, a.classes, a.jitter);
    } وإلا إذا (a.type == COMPARE_داده){
        *a.d = بارگیری_داده_compare(a.n, a.paths, a.m, a.classes, a.w, a.h);
    } وإلا إذا (a.type == IMAGE_داده){
        *(a.im) = بارگیری_image_color(a.مسیر, 0, 0);
        *(a.resized) = resize_image(*(a.im), a.w, a.h);
    } وإلا إذا (a.type == LETTERBOX_داده){
        *(a.im) = بارگیری_image_color(a.مسیر, 0, 0);
        *(a.resized) = letterbox_image(*(a.im), a.w, a.h);
    } وإلا إذا (a.type == برچسب_داده){
        *a.d = بارگیری_داده_برچسب(a.paths, a.n, a.m, a.classes, a.min, a.max, a.اندازه, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    }
    حرر(ptr);
    أرجع 0;
}

pthread_t بارگیری_داده_درون_thread(بارگیری_آرگومانها آرگومانها)
{
    pthread_t thread;
    بنية بارگیری_آرگومانها *ptr = حافظه_صفر_بده(1, حجم(بنية بارگیری_آرگومانها));
    *ptr = آرگومانها;
    إذا(pthread_create(&thread, 0, بارگیری_thread, ptr)) error("Thread creation failed");
    أرجع thread;
}

فراغ *بارگیری_نخها(فراغ *ptr)
{
    صحيح i;
    بارگیری_آرگومانها آرگومانها = *(بارگیری_آرگومانها *)ptr;
    إذا (آرگومانها.نخها == 0) آرگومانها.نخها = 1;
    داده *بیرون = آرگومانها.d;
    صحيح مجموع = آرگومانها.n;
    حرر(ptr);
    داده *buffers = حافظه_صفر_بده(آرگومانها.نخها, حجم(داده));
    pthread_t *نخها = حافظه_صفر_بده(آرگومانها.نخها, حجم(pthread_t));
    لكل(i = 0; i < آرگومانها.نخها; ++i){
        آرگومانها.d = buffers + i;
        آرگومانها.n = (i+1) * مجموع/آرگومانها.نخها - i * مجموع/آرگومانها.نخها;
        نخها[i] = بارگیری_داده_درون_thread(آرگومانها);
    }
    لكل(i = 0; i < آرگومانها.نخها; ++i){
        pthread_join(نخها[i], 0);
    }
    *بیرون = concat_datas(buffers, آرگومانها.نخها);
    بیرون->shallow = 0;
    لكل(i = 0; i < آرگومانها.نخها; ++i){
        buffers[i].shallow = 1;
        free_داده(buffers[i]);
    }
    حرر(buffers);
    حرر(نخها);
    أرجع 0;
}

فراغ بارگیری_داده_blocking(بارگیری_آرگومانها آرگومانها)
{
    بنية بارگیری_آرگومانها *ptr = حافظه_صفر_بده(1, حجم(بنية بارگیری_آرگومانها));
    *ptr = آرگومانها;
    بارگیری_thread(ptr);
}

pthread_t بارگیری_داده(بارگیری_آرگومانها آرگومانها)
{
    pthread_t thread;
    بنية بارگیری_آرگومانها *ptr = حافظه_صفر_بده(1, حجم(بنية بارگیری_آرگومانها));
    *ptr = آرگومانها;
    إذا(pthread_create(&thread, 0, بارگیری_نخها, ptr)) error("Thread creation failed");
    أرجع thread;
}

داده بارگیری_داده_writing(حرف **paths, صحيح n, صحيح m, صحيح w, صحيح h, صحيح بیرون_w, صحيح بیرون_h)
{
    إذا(m) paths = get_random_paths(paths, n, m);
    حرف **replace_paths = پیدا_کن_replace_paths(paths, n, ".png", "-label.png");
    داده d = {0};
    d.shallow = 0;
    d.X = بارگیری_image_paths(paths, n, w, h);
    d.y = بارگیری_image_paths_خاکستری(replace_paths, n, بیرون_w, بیرون_h);
    إذا(m) حرر(paths);
    صحيح i;
    لكل(i = 0; i < n; ++i) حرر(replace_paths[i]);
    حرر(replace_paths);
    أرجع d;
}

داده بارگیری_داده_old(حرف **paths, صحيح n, صحيح m, حرف **labels, صحيح k, صحيح w, صحيح h)
{
    إذا(m) paths = get_random_paths(paths, n, m);
    داده d = {0};
    d.shallow = 0;
    d.X = بارگیری_image_paths(paths, n, w, h);
    d.y = بارگیری_labels_paths(paths, n, labels, k, 0);
    إذا(m) حرر(paths);
    أرجع d;
}

/*
   data load_data_study(char **paths, int n, int m, char **labels, int k, int min, int max, int size, float angle, float aspect, float hue, float saturation, float exposure)
   {
   data d = {0};
   d.indexes = calloc(n, sizeof(int));
   if(m) paths = get_random_paths_indexes(paths, n, m, d.indexes);
   d.shallow = 0;
   d.X = load_image_augment_paths(paths, n, min, max, size, angle, aspect, hue, saturation, exposure);
   d.y = load_labels_paths(paths, n, labels, k);
   if(m) free(paths);
   return d;
   }
 */

داده بارگیری_داده_super(حرف **paths, صحيح n, صحيح m, صحيح w, صحيح h, صحيح scale)
{
    إذا(m) paths = get_random_paths(paths, n, m);
    داده d = {0};
    d.shallow = 0;

    صحيح i;
    d.X.ردیفها = n;
    d.X.vals = حافظه_صفر_بده(n, حجم(عائم*));
    d.X.تعداد_ستون = w*h*3;

    d.y.ردیفها = n;
    d.y.vals = حافظه_صفر_بده(n, حجم(عائم*));
    d.y.تعداد_ستون = w*scale * h*scale * 3;

    لكل(i = 0; i < n; ++i){
        image im = بارگیری_image_color(paths[i], 0, 0);
        image crop = random_crop_image(im, w*scale, h*scale);
        صحيح flip = rand()%2;
        إذا (flip) flip_image(crop);
        image resize = resize_image(crop, w, h);
        d.X.vals[i] = resize.داده;
        d.y.vals[i] = crop.داده;
        free_image(im);
    }

    إذا(m) حرر(paths);
    أرجع d;
}

داده بارگیری_داده_regression(حرف **paths, صحيح n, صحيح m, صحيح k, صحيح min, صحيح max, صحيح اندازه, عائم angle, عائم aspect, عائم hue, عائم saturation, عائم exposure)
{
    إذا(m) paths = get_random_paths(paths, n, m);
    داده d = {0};
    d.shallow = 0;
    d.X = بارگیری_image_augment_paths(paths, n, min, max, اندازه, angle, aspect, hue, saturation, exposure, 0);
    d.y = بارگیری_regression_labels_paths(paths, n, k);
    إذا(m) حرر(paths);
    أرجع d;
}

داده select_داده(داده *orig, صحيح *inds)
{
    داده d = {0};
    d.shallow = 1;
    d.w = orig[0].w;
    d.h = orig[0].h;

    d.X.ردیفها = orig[0].X.ردیفها;
    d.y.ردیفها = orig[0].X.ردیفها;

    d.X.تعداد_ستون = orig[0].X.تعداد_ستون;
    d.y.تعداد_ستون = orig[0].y.تعداد_ستون;

    d.X.vals = حافظه_صفر_بده(orig[0].X.ردیفها, حجم(عائم *));
    d.y.vals = حافظه_صفر_بده(orig[0].y.ردیفها, حجم(عائم *));
    صحيح i;
    لكل(i = 0; i < d.X.ردیفها; ++i){
        d.X.vals[i] = orig[inds[i]].X.vals[i];
        d.y.vals[i] = orig[inds[i]].y.vals[i];
    }
    أرجع d;
}

داده *tile_داده(داده orig, صحيح divs, صحيح اندازه)
{
    داده *ds = حافظه_صفر_بده(divs*divs, حجم(داده));
    صحيح i, j;
#رهنمود omp parallel لكل
    لكل(i = 0; i < divs*divs; ++i){
        داده d;
        d.shallow = 0;
        d.w = orig.w/divs * اندازه;
        d.h = orig.h/divs * اندازه;
        d.X.ردیفها = orig.X.ردیفها;
        d.X.تعداد_ستون = d.w*d.h*3;
        d.X.vals = حافظه_صفر_بده(d.X.ردیفها, حجم(عائم*));

        d.y = copy_ماتریس(orig.y);
#رهنمود omp parallel لكل
        لكل(j = 0; j < orig.X.ردیفها; ++j){
            صحيح x = (i%divs) * orig.w / divs - (d.w - orig.w/divs)/2;
            صحيح y = (i/divs) * orig.h / divs - (d.h - orig.h/divs)/2;
            image im = float_مقصد_image(orig.w, orig.h, 3, orig.X.vals[j]);
            d.X.vals[j] = crop_image(im, x, y, d.w, d.h).داده;
        }
        ds[i] = d;
    }
    أرجع ds;
}

داده resize_داده(داده orig, صحيح w, صحيح h)
{
    داده d = {0};
    d.shallow = 0;
    d.w = w;
    d.h = h;
    صحيح i;
    d.X.ردیفها = orig.X.ردیفها;
    d.X.تعداد_ستون = w*h*3;
    d.X.vals = حافظه_صفر_بده(d.X.ردیفها, حجم(عائم*));

    d.y = copy_ماتریس(orig.y);
#رهنمود omp parallel لكل
    لكل(i = 0; i < orig.X.ردیفها; ++i){
        image im = float_مقصد_image(orig.w, orig.h, 3, orig.X.vals[i]);
        d.X.vals[i] = resize_image(im, w, h).داده;
    }
    أرجع d;
}

داده بارگیری_داده_augment(حرف **paths, صحيح n, صحيح m, حرف **labels, صحيح k, درخت *hierarchy, صحيح min, صحيح max, صحيح اندازه, عائم angle, عائم aspect, عائم hue, عائم saturation, عائم exposure, صحيح center)
{
    إذا(m) paths = get_random_paths(paths, n, m);
    داده d = {0};
    d.shallow = 0;
    d.w=اندازه;
    d.h=اندازه;
    d.X = بارگیری_image_augment_paths(paths, n, min, max, اندازه, angle, aspect, hue, saturation, exposure, center);
    d.y = بارگیری_labels_paths(paths, n, labels, k, hierarchy);
    إذا(m) حرر(paths);
    أرجع d;
}

داده بارگیری_داده_برچسب(حرف **paths, صحيح n, صحيح m, صحيح k, صحيح min, صحيح max, صحيح اندازه, عائم angle, عائم aspect, عائم hue, عائم saturation, عائم exposure)
{
    إذا(m) paths = get_random_paths(paths, n, m);
    داده d = {0};
    d.w = اندازه;
    d.h = اندازه;
    d.shallow = 0;
    d.X = بارگیری_image_augment_paths(paths, n, min, max, اندازه, angle, aspect, hue, saturation, exposure, 0);
    d.y = بارگیری_برچسبها_paths(paths, n, k);
    إذا(m) حرر(paths);
    أرجع d;
}

ماتریس concat_ماتریس(ماتریس m1, ماتریس m2)
{
    صحيح i, شمار = 0;
    ماتریس m;
    m.تعداد_ستون = m1.تعداد_ستون;
    m.ردیفها = m1.ردیفها+m2.ردیفها;
    m.vals = حافظه_صفر_بده(m1.ردیفها + m2.ردیفها, حجم(عائم*));
    لكل(i = 0; i < m1.ردیفها; ++i){
        m.vals[شمار++] = m1.vals[i];
    }
    لكل(i = 0; i < m2.ردیفها; ++i){
        m.vals[شمار++] = m2.vals[i];
    }
    أرجع m;
}

داده concat_داده(داده d1, داده d2)
{
    داده d = {0};
    d.shallow = 1;
    d.X = concat_ماتریس(d1.X, d2.X);
    d.y = concat_ماتریس(d1.y, d2.y);
    d.w = d1.w;
    d.h = d1.h;
    أرجع d;
}

داده concat_datas(داده *d, صحيح n)
{
    صحيح i;
    داده بیرون = {0};
    لكل(i = 0; i < n; ++i){
        داده new = concat_داده(d[i], بیرون);
        free_داده(بیرون);
        بیرون = new;
    }
    أرجع بیرون;
}

داده بارگیری_categorical_داده_csv(حرف *filename, صحيح هدف, صحيح k)
{
    داده d = {0};
    d.shallow = 0;
    ماتریس X = csv_مقصد_ماتریس(filename);
    عائم *truth_1d = برداشت_ستونی(&X, هدف);
    عائم **truth = one_hot_encode(truth_1d, X.ردیفها, k);
    ماتریس y;
    y.ردیفها = X.ردیفها;
    y.تعداد_ستون = k;
    y.vals = truth;
    d.X = X;
    d.y = y;
    حرر(truth_1d);
    أرجع d;
}

داده بارگیری_cifar10_داده(حرف *filename)
{
    داده d = {0};
    d.shallow = 0;
    دراز i,j;
    ماتریس X = make_ماتریس(10000, 3072);
    ماتریس y = make_ماتریس(10000, 10);
    d.X = X;
    d.y = y;

    FILE *fp = fopen(filename, "rb");
    إذا(!fp) file_error(filename);
    لكل(i = 0; i < 10000; ++i){
        بیعلامت حرف بایتها[3073];
        fread(بایتها, 1, 3073, fp);
        صحيح class = بایتها[0];
        y.vals[i][class] = 1;
        لكل(j = 0; j < X.تعداد_ستون; ++j){
            X.vals[i][j] = (مزدوج)بایتها[j+1];
        }
    }
    scale_داده_ردیفها(d, 1./255);
    //normalize_data_rows(d);
    fclose(fp);
    أرجع d;
}

فراغ get_random_batch(داده d, صحيح n, عائم *X, عائم *y)
{
    صحيح j;
    لكل(j = 0; j < n; ++j){
        صحيح شاخص = rand()%d.X.ردیفها;
        کپی_حافظه(X+j*d.X.تعداد_ستون, d.X.vals[شاخص], d.X.تعداد_ستون*حجم(عائم));
        کپی_حافظه(y+j*d.y.تعداد_ستون, d.y.vals[شاخص], d.y.تعداد_ستون*حجم(عائم));
    }
}

فراغ get_بعدی_batch(داده d, صحيح n, صحيح جابه_جایی, عائم *X, عائم *y)
{
    صحيح j;
    لكل(j = 0; j < n; ++j){
        صحيح شاخص = جابه_جایی + j;
        کپی_حافظه(X+j*d.X.تعداد_ستون, d.X.vals[شاخص], d.X.تعداد_ستون*حجم(عائم));
        إذا(y) کپی_حافظه(y+j*d.y.تعداد_ستون, d.y.vals[شاخص], d.y.تعداد_ستون*حجم(عائم));
    }
}

فراغ smooth_داده(داده d)
{
    صحيح i, j;
    عائم scale = 1. / d.y.تعداد_ستون;
    عائم eps = .1;
    لكل(i = 0; i < d.y.ردیفها; ++i){
        لكل(j = 0; j < d.y.تعداد_ستون; ++j){
            d.y.vals[i][j] = eps * scale + (1-eps) * d.y.vals[i][j];
        }
    }
}

داده بارگیری_all_cifar10()
{
    داده d = {0};
    d.shallow = 0;
    صحيح i,j,b;
    ماتریس X = make_ماتریس(50000, 3072);
    ماتریس y = make_ماتریس(50000, 10);
    d.X = X;
    d.y = y;


    لكل(b = 0; b < 5; ++b){
        حرف buff[256];
        sprintf(buff, "data/cifar/cifar-10-batches-bin/data_batch_%d.bin", b+1);
        FILE *fp = fopen(buff, "rb");
        إذا(!fp) file_error(buff);
        لكل(i = 0; i < 10000; ++i){
            بیعلامت حرف بایتها[3073];
            fread(بایتها, 1, 3073, fp);
            صحيح class = بایتها[0];
            y.vals[i+b*10000][class] = 1;
            لكل(j = 0; j < X.تعداد_ستون; ++j){
                X.vals[i+b*10000][j] = (مزدوج)بایتها[j+1];
            }
        }
        fclose(fp);
    }
    //normalize_data_rows(d);
    scale_داده_ردیفها(d, 1./255);
    smooth_داده(d);
    أرجع d;
}

داده بارگیری_go(حرف *filename)
{
    FILE *fp = fopen(filename, "rb");
    ماتریس X = make_ماتریس(3363059, 361);
    ماتریس y = make_ماتریس(3363059, 361);
    صحيح ردیف, ستون;

    إذا(!fp) file_error(filename);
    حرف *label;
    صحيح شمار = 0;
    طالما((label = fgetl(fp))){
        صحيح i;
        إذا(شمار == X.ردیفها){
            X = resize_ماتریس(X, شمار*2);
            y = resize_ماتریس(y, شمار*2);
        }
        از_رشته_بخوان(label, "%d %d", &ردیف, &ستون);
        حرف *board = fgetl(fp);

        صحيح شاخص = ردیف*19 + ستون;
        y.vals[شمار][شاخص] = 1;

        لكل(i = 0; i < 19*19; ++i){
            عائم val = 0;
            إذا(board[i] == '1') val = 1;
            وإلا إذا(board[i] == '2') val = -1;
            X.vals[شمار][i] = val;
        }
        ++شمار;
        حرر(label);
        حرر(board);
    }
    X = resize_ماتریس(X, شمار);
    y = resize_ماتریس(y, شمار);

    داده d = {0};
    d.shallow = 0;
    d.X = X;
    d.y = y;


    fclose(fp);

    أرجع d;
}


فراغ randomize_داده(داده d)
{
    صحيح i;
    لكل(i = d.X.ردیفها-1; i > 0; --i){
        صحيح شاخص = rand()%i;
        عائم *جابجا = d.X.vals[شاخص];
        d.X.vals[شاخص] = d.X.vals[i];
        d.X.vals[i] = جابجا;

        جابجا = d.y.vals[شاخص];
        d.y.vals[شاخص] = d.y.vals[i];
        d.y.vals[i] = جابجا;
    }
}

فراغ scale_داده_ردیفها(داده d, عائم s)
{
    صحيح i;
    لكل(i = 0; i < d.X.ردیفها; ++i){
        scale_array(d.X.vals[i], d.X.تعداد_ستون, s);
    }
}

فراغ translate_داده_ردیفها(داده d, عائم s)
{
    صحيح i;
    لكل(i = 0; i < d.X.ردیفها; ++i){
        translate_array(d.X.vals[i], d.X.تعداد_ستون, s);
    }
}

داده copy_داده(داده d)
{
    داده c = {0};
    c.w = d.w;
    c.h = d.h;
    c.shallow = 0;
    c.رقم_boxes = d.رقم_boxes;
    c.boxes = d.boxes;
    c.X = copy_ماتریس(d.X);
    c.y = copy_ماتریس(d.y);
    أرجع c;
}

فراغ normalize_داده_ردیفها(داده d)
{
    صحيح i;
    لكل(i = 0; i < d.X.ردیفها; ++i){
        normalize_array(d.X.vals[i], d.X.تعداد_ستون);
    }
}

داده get_داده_part(داده d, صحيح part, صحيح مجموع)
{
    داده p = {0};
    p.shallow = 1;
    p.X.ردیفها = d.X.ردیفها * (part + 1) / مجموع - d.X.ردیفها * part / مجموع;
    p.y.ردیفها = d.y.ردیفها * (part + 1) / مجموع - d.y.ردیفها * part / مجموع;
    p.X.تعداد_ستون = d.X.تعداد_ستون;
    p.y.تعداد_ستون = d.y.تعداد_ستون;
    p.X.vals = d.X.vals + d.X.ردیفها * part / مجموع;
    p.y.vals = d.y.vals + d.y.ردیفها * part / مجموع;
    أرجع p;
}

داده get_random_داده(داده d, صحيح رقم)
{
    داده r = {0};
    r.shallow = 1;

    r.X.ردیفها = رقم;
    r.y.ردیفها = رقم;

    r.X.تعداد_ستون = d.X.تعداد_ستون;
    r.y.تعداد_ستون = d.y.تعداد_ستون;

    r.X.vals = حافظه_صفر_بده(رقم, حجم(عائم *));
    r.y.vals = حافظه_صفر_بده(رقم, حجم(عائم *));

    صحيح i;
    لكل(i = 0; i < رقم; ++i){
        صحيح شاخص = rand()%d.X.ردیفها;
        r.X.vals[i] = d.X.vals[شاخص];
        r.y.vals[i] = d.y.vals[شاخص];
    }
    أرجع r;
}

داده *تقسیم_داده(داده d, صحيح part, صحيح مجموع)
{
    داده *تقسیم = حافظه_صفر_بده(2, حجم(داده));
    صحيح i;
    صحيح شروع = part*d.X.ردیفها/مجموع;
    صحيح انتها = (part+1)*d.X.ردیفها/مجموع;
    داده train;
    داده test;
    train.shallow = test.shallow = 1;

    test.X.ردیفها = test.y.ردیفها = انتها-شروع;
    train.X.ردیفها = train.y.ردیفها = d.X.ردیفها - (انتها-شروع);
    train.X.تعداد_ستون = test.X.تعداد_ستون = d.X.تعداد_ستون;
    train.y.تعداد_ستون = test.y.تعداد_ستون = d.y.تعداد_ستون;

    train.X.vals = حافظه_صفر_بده(train.X.ردیفها, حجم(عائم*));
    test.X.vals = حافظه_صفر_بده(test.X.ردیفها, حجم(عائم*));
    train.y.vals = حافظه_صفر_بده(train.y.ردیفها, حجم(عائم*));
    test.y.vals = حافظه_صفر_بده(test.y.ردیفها, حجم(عائم*));

    لكل(i = 0; i < شروع; ++i){
        train.X.vals[i] = d.X.vals[i];
        train.y.vals[i] = d.y.vals[i];
    }
    لكل(i = شروع; i < انتها; ++i){
        test.X.vals[i-شروع] = d.X.vals[i];
        test.y.vals[i-شروع] = d.y.vals[i];
    }
    لكل(i = انتها; i < d.X.ردیفها; ++i){
        train.X.vals[i-(انتها-شروع)] = d.X.vals[i];
        train.y.vals[i-(انتها-شروع)] = d.y.vals[i];
    }
    تقسیم[0] = train;
    تقسیم[1] = test;
    أرجع تقسیم;
}

