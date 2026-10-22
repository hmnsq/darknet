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
    حرف *لار;
    FILE *file = fopen(filename, "r");
    إذا(!file) file_error(filename);
    list *کرښې = make_list();
    طالما((لار=fgetl(file))){
        list_ورننباسه(کرښې, لار);
    }
    fclose(file);
    أرجع کرښې;
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
    حرف **random_paths = حافظه_صفر_ورکړه(n, حجم(حرف*));
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

حرف **پیدا_کړه_replace_paths(حرف **paths, صحيح n, حرف *پیدا_کړه, حرف *replace)
{
    حرف **replace_paths = حافظه_صفر_ورکړه(n, حجم(حرف*));
    صحيح i;
    لكل(i = 0; i < n; ++i){
        حرف replaced[4096];
        پیدا_کړه_replace(paths[i], پیدا_کړه, replace, replaced);
        replace_paths[i] = copy_string(replaced);
    }
    أرجع replace_paths;
}

ماتریکس بارول_image_paths_خړ(حرف **paths, صحيح n, صحيح w, صحيح h)
{
    صحيح i;
    ماتریکس X;
    X.لیکې = n;
    X.vals = حافظه_صفر_ورکړه(X.لیکې, حجم(عائم*));
    X.ستون_شمېر = 0;

    لكل(i = 0; i < n; ++i){
        image im = بارول_image(paths[i], w, h, 3);

        image خړ = grayscale_image(im);
        free_image(im);
        im = خړ;

        X.vals[i] = im.معلومات;
        X.ستون_شمېر = im.h*im.w*im.c;
    }
    أرجع X;
}

ماتریکس بارول_image_paths(حرف **paths, صحيح n, صحيح w, صحيح h)
{
    صحيح i;
    ماتریکس X;
    X.لیکې = n;
    X.vals = حافظه_صفر_ورکړه(X.لیکې, حجم(عائم*));
    X.ستون_شمېر = 0;

    لكل(i = 0; i < n; ++i){
        image im = بارول_image_color(paths[i], w, h);
        X.vals[i] = im.معلومات;
        X.ستون_شمېر = im.h*im.w*im.c;
    }
    أرجع X;
}

ماتریکس بارول_image_augment_paths(حرف **paths, صحيح n, صحيح min, صحيح max, صحيح اندازه, عائم angle, عائم aspect, عائم hue, عائم saturation, عائم exposure, صحيح center)
{
    صحيح i;
    ماتریکس X;
    X.لیکې = n;
    X.vals = حافظه_صفر_ورکړه(X.لیکې, حجم(عائم*));
    X.ستون_شمېر = 0;

    لكل(i = 0; i < n; ++i){
        image im = بارول_image_color(paths[i], 0, 0);
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
        X.vals[i] = crop.معلومات;
        X.ستون_شمېر = crop.h*crop.w*crop.c;
    }
    أرجع X;
}


box_label *read_boxes(حرف *filename, صحيح *n)
{
    FILE *file = fopen(filename, "r");
    إذا(!file) file_error(filename);
    عائم x, y, h, w;
    صحيح id;
    صحيح شمېر = 0;
    صحيح اندازه = 64;
    box_label *boxes = حافظه_صفر_ورکړه(اندازه, حجم(box_label));
    طالما(fscanf(file, "%d %f %f %f %f", &id, &x, &y, &w, &h) == 5){
        إذا(شمېر == اندازه) {
            اندازه = اندازه * 2;
            boxes = realloc(boxes, اندازه*حجم(box_label));
        }
        boxes[شمېر].id = id;
        boxes[شمېر].x = x;
        boxes[شمېر].y = y;
        boxes[شمېر].h = h;
        boxes[شمېر].w = w;
        boxes[شمېر].کیڼ   = x - w/2;
        boxes[شمېر].ښی  = x + w/2;
        boxes[شمېر].سر    = y - h/2;
        boxes[شمېر].bottom = y + h/2;
        ++شمېر;
    }
    fclose(file);
    *n = شمېر;
    أرجع boxes;
}

فراغ randomize_boxes(box_label *b, صحيح n)
{
    صحيح i;
    لكل(i = 0; i < n; ++i){
        box_label بدلون = b[i];
        صحيح شاخص = rand()%n;
        b[i] = b[شاخص];
        b[شاخص] = بدلون;
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
        boxes[i].کیڼ   = boxes[i].کیڼ  * sx - dx;
        boxes[i].ښی  = boxes[i].ښی * sx - dx;
        boxes[i].سر    = boxes[i].سر   * sy - dy;
        boxes[i].bottom = boxes[i].bottom* sy - dy;

        إذا(flip){
            عائم بدلون = boxes[i].کیڼ;
            boxes[i].کیڼ = 1. - boxes[i].ښی;
            boxes[i].ښی = 1. - بدلون;
        }

        boxes[i].کیڼ =  constrain(0, 1, boxes[i].کیڼ);
        boxes[i].ښی = constrain(0, 1, boxes[i].ښی);
        boxes[i].سر =   constrain(0, 1, boxes[i].سر);
        boxes[i].bottom =   constrain(0, 1, boxes[i].bottom);

        boxes[i].x = (boxes[i].کیڼ+boxes[i].ښی)/2;
        boxes[i].y = (boxes[i].سر+boxes[i].bottom)/2;
        boxes[i].w = (boxes[i].ښی - boxes[i].کیڼ);
        boxes[i].h = (boxes[i].bottom - boxes[i].سر);

        boxes[i].w = constrain(0, 1, boxes[i].w);
        boxes[i].h = constrain(0, 1, boxes[i].h);
    }
}

فراغ fill_truth_swag(حرف *لار, عائم *truth, صحيح classes, صحيح flip, عائم dx, عائم dy, عائم sx, عائم sy)
{
    حرف labelpath[4096];
    پیدا_کړه_replace(لار, "images", "labels", labelpath);
    پیدا_کړه_replace(labelpath, "JPEGImages", "labels", labelpath);
    پیدا_کړه_replace(labelpath, ".jpg", ".txt", labelpath);
    پیدا_کړه_replace(labelpath, ".JPG", ".txt", labelpath);
    پیدا_کړه_replace(labelpath, ".JPEG", ".txt", labelpath);

    صحيح شمېر = 0;
    box_label *boxes = read_boxes(labelpath, &شمېر);
    randomize_boxes(boxes, شمېر);
    correct_boxes(boxes, شمېر, dx, dy, sx, sy, flip);
    عائم x,y,w,h;
    صحيح id;
    صحيح i;

    لكل (i = 0; i < شمېر && i < 90; ++i) {
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

فراغ fill_truth_region(حرف *لار, عائم *truth, صحيح classes, صحيح عدد_boxes, صحيح flip, عائم dx, عائم dy, عائم sx, عائم sy)
{
    حرف labelpath[4096];
    پیدا_کړه_replace(لار, "images", "labels", labelpath);
    پیدا_کړه_replace(labelpath, "JPEGImages", "labels", labelpath);

    پیدا_کړه_replace(labelpath, ".jpg", ".txt", labelpath);
    پیدا_کړه_replace(labelpath, ".png", ".txt", labelpath);
    پیدا_کړه_replace(labelpath, ".JPG", ".txt", labelpath);
    پیدا_کړه_replace(labelpath, ".JPEG", ".txt", labelpath);
    صحيح شمېر = 0;
    box_label *boxes = read_boxes(labelpath, &شمېر);
    randomize_boxes(boxes, شمېر);
    correct_boxes(boxes, شمېر, dx, dy, sx, sy, flip);
    عائم x,y,w,h;
    صحيح id;
    صحيح i;

    لكل (i = 0; i < شمېر; ++i) {
        x =  boxes[i].x;
        y =  boxes[i].y;
        w =  boxes[i].w;
        h =  boxes[i].h;
        id = boxes[i].id;

        إذا (w < .005 || h < .005) استمر;

        صحيح ستون = (صحيح)(x*عدد_boxes);
        صحيح لیکه = (صحيح)(y*عدد_boxes);

        x = x*عدد_boxes - ستون;
        y = y*عدد_boxes - لیکه;

        صحيح شاخص = (ستون+لیکه*عدد_boxes)*(5+classes);
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

فراغ بارول_rle(image im, صحيح *rle, صحيح n)
{
    صحيح شمېر = 0;
    صحيح curr = 0;
    صحيح i,j;
    لكل(i = 0; i < n; ++i){
        لكل(j = 0; j < rle[i]; ++j){
            im.معلومات[شمېر++] = curr;
        }
        curr = 1 - curr;
    }
    لكل(; شمېر < im.h*im.w*im.c; ++شمېر){
        im.معلومات[شمېر] = curr;
    }
}

فراغ or_image(image src, image dest, صحيح c)
{
    صحيح i;
    لكل(i = 0; i < src.w*src.h; ++i){
        إذا(src.معلومات[i]) dest.معلومات[dest.w*dest.h*c + i] = 1;
    }
}

فراغ exclusive_image(image src)
{
    صحيح k, j, i;
    صحيح s = src.w*src.h;
    لكل(k = 0; k < src.c-1; ++k){
        لكل(i = 0; i < s; ++i){
            إذا (src.معلومات[k*s + i]){
                لكل(j = k+1; j < src.c; ++j){
                    src.معلومات[j*s + i] = 0;
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
            إذا(im.معلومات[y*im.w + x]){
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

فراغ fill_truth_iseg(حرف *لار, صحيح عدد_boxes, عائم *truth, صحيح classes, صحيح w, صحيح h, augment_دلیلونه aug, صحيح flip, صحيح mw, صحيح mh)
{
    حرف labelpath[4096];
    پیدا_کړه_replace(لار, "images", "mask", labelpath);
    پیدا_کړه_replace(labelpath, "JPEGImages", "mask", labelpath);
    پیدا_کړه_replace(labelpath, ".jpg", ".txt", labelpath);
    پیدا_کړه_replace(labelpath, ".JPG", ".txt", labelpath);
    پیدا_کړه_replace(labelpath, ".JPEG", ".txt", labelpath);
    FILE *file = fopen(labelpath, "r");
    إذا(!file) file_error(labelpath);
    حرف buff[32788];
    صحيح id;
    صحيح i = 0;
    صحيح j;
    image part = make_image(w, h, 1);
    طالما((fscanf(file, "%d %s", &id, buff) == 2) && i < عدد_boxes){
        صحيح n = 0;
        صحيح *rle = read_intlist(buff, &n, 0);
        بارول_rle(part, rle, n);
        image sized = rotate_crop_image(part, aug.rad, aug.scale, aug.w, aug.h, aug.dx, aug.dy, aug.aspect);
        إذا(flip) flip_image(sized);

        image ماسک = resize_image(sized, mw, mh);
        truth[i*(mw*mh+1)] = id;
        لكل(j = 0; j < mw*mh; ++j){
            truth[i*(mw*mh + 1) + 1 + j] = ماسک.معلومات[j];
        }
        ++i;

        free_image(ماسک);
        free_image(sized);
        حرر(rle);
    }
    إذا(i < عدد_boxes) truth[i*(mw*mh+1)] = -1;
    fclose(file);
    free_image(part);
}

فراغ fill_truth_ماسک(حرف *لار, صحيح عدد_boxes, عائم *truth, صحيح classes, صحيح w, صحيح h, augment_دلیلونه aug, صحيح flip, صحيح mw, صحيح mh)
{
    حرف labelpath[4096];
    پیدا_کړه_replace(لار, "images", "mask", labelpath);
    پیدا_کړه_replace(labelpath, "JPEGImages", "mask", labelpath);
    پیدا_کړه_replace(labelpath, ".jpg", ".txt", labelpath);
    پیدا_کړه_replace(labelpath, ".JPG", ".txt", labelpath);
    پیدا_کړه_replace(labelpath, ".JPEG", ".txt", labelpath);
    FILE *file = fopen(labelpath, "r");
    إذا(!file) file_error(labelpath);
    حرف buff[32788];
    صحيح id;
    صحيح i = 0;
    image part = make_image(w, h, 1);
    طالما((fscanf(file, "%d %s", &id, buff) == 2) && i < عدد_boxes){
        صحيح n = 0;
        صحيح *rle = read_intlist(buff, &n, 0);
        بارول_rle(part, rle, n);
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
                truth[i*(4 + mw*mh + 1) + 4 + j] = ماسک.معلومات[j];
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


فراغ fill_truth_detection(حرف *لار, صحيح عدد_boxes, عائم *truth, صحيح classes, صحيح flip, عائم dx, عائم dy, عائم sx, عائم sy)
{
    حرف labelpath[4096];
    پیدا_کړه_replace(لار, "images", "labels", labelpath);
    پیدا_کړه_replace(labelpath, "JPEGImages", "labels", labelpath);

    پیدا_کړه_replace(labelpath, "raw", "labels", labelpath);
    پیدا_کړه_replace(labelpath, ".jpg", ".txt", labelpath);
    پیدا_کړه_replace(labelpath, ".png", ".txt", labelpath);
    پیدا_کړه_replace(labelpath, ".JPG", ".txt", labelpath);
    پیدا_کړه_replace(labelpath, ".JPEG", ".txt", labelpath);
    صحيح شمېر = 0;
    box_label *boxes = read_boxes(labelpath, &شمېر);
    randomize_boxes(boxes, شمېر);
    correct_boxes(boxes, شمېر, dx, dy, sx, sy, flip);
    إذا(شمېر > عدد_boxes) شمېر = عدد_boxes;
    عائم x,y,w,h;
    صحيح id;
    صحيح i;
    صحيح sub = 0;

    لكل (i = 0; i < شمېر; ++i) {
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

#تعریف_کړه NUMCHARS 37

فراغ print_letters(عائم *pred, صحيح n)
{
    صحيح i;
    لكل(i = 0; i < n; ++i){
        صحيح شاخص = max_شاخص(pred+i*NUMCHARS, NUMCHARS);
        چاپ_کړه("%c", int_منزل_alphanum(شاخص));
    }
    چاپ_کړه("\n");
}

فراغ fill_truth_captcha(حرف *لار, صحيح n, عائم *truth)
{
    حرف *begin = strrchr(لار, '/');
    ++begin;
    صحيح i;
    لكل(i = 0; i < مزي_اوږدوالی(begin) && i < n && begin[i] != '.'; ++i){
        صحيح شاخص = alphanum_منزل_int(begin[i]);
        إذا(شاخص > 35) چاپ_کړه("Bad %c\n", begin[i]);
        truth[i*NUMCHARS+شاخص] = 1;
    }
    لكل(;i < n; ++i){
        truth[i*NUMCHARS + NUMCHARS-1] = 1;
    }
}

معلومات بارول_معلومات_captcha(حرف **paths, صحيح n, صحيح m, صحيح k, صحيح w, صحيح h)
{
    إذا(m) paths = get_random_paths(paths, n, m);
    معلومات d = {0};
    d.shallow = 0;
    d.X = بارول_image_paths(paths, n, w, h);
    d.y = make_ماتریکس(n, k*NUMCHARS);
    صحيح i;
    لكل(i = 0; i < n; ++i){
        fill_truth_captcha(paths[i], k, d.y.vals[i]);
    }
    إذا(m) حرر(paths);
    أرجع d;
}

معلومات بارول_معلومات_captcha_encode(حرف **paths, صحيح n, صحيح m, صحيح w, صحيح h)
{
    إذا(m) paths = get_random_paths(paths, n, m);
    معلومات d = {0};
    d.shallow = 0;
    d.X = بارول_image_paths(paths, n, w, h);
    d.X.ستون_شمېر = 17100;
    d.y = d.X;
    إذا(m) حرر(paths);
    أرجع d;
}

فراغ fill_truth(حرف *لار, حرف **labels, صحيح k, عائم *truth)
{
    صحيح i;
    memset(truth, 0, k*حجم(عائم));
    صحيح شمېر = 0;
    لكل(i = 0; i < k; ++i){
        إذا(مزي_پلټنه(لار, labels[i])){
            truth[i] = 1;
            ++شمېر;
            //printf("%s %s %d\n", path, labels[i], i);
        }
    }
    إذا(شمېر != 1 && (k != 1 || شمېر != 0)) چاپ_کړه("Too many or too few labels: %d, %s\n", شمېر, لار);
}

فراغ fill_hierarchy(عائم *truth, صحيح k, ونه *hierarchy)
{
    صحيح j;
    لكل(j = 0; j < k; ++j){
        إذا(truth[j]){
            صحيح پلار = hierarchy->پلار[j];
            طالما(پلار >= 0){
                truth[پلار] = 1;
                پلار = hierarchy->پلار[پلار];
            }
        }
    }
    صحيح i;
    صحيح شمېر = 0;
    لكل(j = 0; j < hierarchy->groups; ++j){
        //printf("%d\n", count);
        صحيح ماسک = 1;
        لكل(i = 0; i < hierarchy->ډله_اندازه[j]; ++i){
            إذا(truth[شمېر + i]){
                ماسک = 0;
                اكسر;
            }
        }
        إذا (ماسک) {
            لكل(i = 0; i < hierarchy->ډله_اندازه[j]; ++i){
                truth[شمېر + i] = SECRET_عدد;
            }
        }
        شمېر += hierarchy->ډله_اندازه[j];
    }
}

ماتریکس بارول_regression_labels_paths(حرف **paths, صحيح n, صحيح k)
{
    ماتریکس y = make_ماتریکس(n, k);
    صحيح i,j;
    لكل(i = 0; i < n; ++i){
        حرف labelpath[4096];
        پیدا_کړه_replace(paths[i], "images", "labels", labelpath);
        پیدا_کړه_replace(labelpath, "JPEGImages", "labels", labelpath);
        پیدا_کړه_replace(labelpath, ".BMP", ".txt", labelpath);
        پیدا_کړه_replace(labelpath, ".JPEG", ".txt", labelpath);
        پیدا_کړه_replace(labelpath, ".JPG", ".txt", labelpath);
        پیدا_کړه_replace(labelpath, ".JPeG", ".txt", labelpath);
        پیدا_کړه_replace(labelpath, ".Jpeg", ".txt", labelpath);
        پیدا_کړه_replace(labelpath, ".PNG", ".txt", labelpath);
        پیدا_کړه_replace(labelpath, ".TIF", ".txt", labelpath);
        پیدا_کړه_replace(labelpath, ".bmp", ".txt", labelpath);
        پیدا_کړه_replace(labelpath, ".jpeg", ".txt", labelpath);
        پیدا_کړه_replace(labelpath, ".jpg", ".txt", labelpath);
        پیدا_کړه_replace(labelpath, ".png", ".txt", labelpath);
        پیدا_کړه_replace(labelpath, ".tif", ".txt", labelpath);

        FILE *file = fopen(labelpath, "r");
        لكل(j = 0; j < k; ++j){
            fscanf(file, "%f", &(y.vals[i][j]));
        }
        fclose(file);
    }
    أرجع y;
}

ماتریکس بارول_labels_paths(حرف **paths, صحيح n, حرف **labels, صحيح k, ونه *hierarchy)
{
    ماتریکس y = make_ماتریکس(n, k);
    صحيح i;
    لكل(i = 0; i < n && labels; ++i){
        fill_truth(paths[i], labels, k, y.vals[i]);
        إذا(hierarchy){
            fill_hierarchy(y.vals[i], k, hierarchy);
        }
    }
    أرجع y;
}

ماتریکس بارول_نښانونه_paths(حرف **paths, صحيح n, صحيح k)
{
    ماتریکس y = make_ماتریکس(n, k);
    صحيح i;
    //int count = 0;
    لكل(i = 0; i < n; ++i){
        حرف label[4096];
        پیدا_کړه_replace(paths[i], "images", "labels", label);
        پیدا_کړه_replace(label, ".jpg", ".txt", label);
        FILE *file = fopen(label, "r");
        إذا (!file) استمر;
        //++count;
        صحيح نښان;
        طالما(fscanf(file, "%d", &نښان) == 1){
            إذا(نښان < k){
                y.vals[i][نښان] = 1;
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
    حرف **labels = (حرف **)list_منزل_array(plist);
    free_list(plist);
    أرجع labels;
}

فراغ free_معلومات(معلومات d)
{
    إذا(!d.shallow){
        free_ماتریکس(d.X);
        free_ماتریکس(d.y);
    }وإلا{
        حرر(d.X.vals);
        حرر(d.y.vals);
    }
}

image get_segmentation_image(حرف *لار, صحيح w, صحيح h, صحيح classes)
{
    حرف labelpath[4096];
    پیدا_کړه_replace(لار, "images", "mask", labelpath);
    پیدا_کړه_replace(labelpath, "JPEGImages", "mask", labelpath);
    پیدا_کړه_replace(labelpath, ".jpg", ".txt", labelpath);
    پیدا_کړه_replace(labelpath, ".JPG", ".txt", labelpath);
    پیدا_کړه_replace(labelpath, ".JPEG", ".txt", labelpath);
    image ماسک = make_image(w, h, classes);
    FILE *file = fopen(labelpath, "r");
    إذا(!file) file_error(labelpath);
    حرف buff[32788];
    صحيح id;
    image part = make_image(w, h, 1);
    طالما(fscanf(file, "%d %s", &id, buff) == 2){
        صحيح n = 0;
        صحيح *rle = read_intlist(buff, &n, 0);
        بارول_rle(part, rle, n);
        or_image(part, ماسک, id);
        حرر(rle);
    }
    //exclusive_image(mask);
    fclose(file);
    free_image(part);
    أرجع ماسک;
}

image get_segmentation_image2(حرف *لار, صحيح w, صحيح h, صحيح classes)
{
    حرف labelpath[4096];
    پیدا_کړه_replace(لار, "images", "mask", labelpath);
    پیدا_کړه_replace(labelpath, "JPEGImages", "mask", labelpath);
    پیدا_کړه_replace(labelpath, ".jpg", ".txt", labelpath);
    پیدا_کړه_replace(labelpath, ".JPG", ".txt", labelpath);
    پیدا_کړه_replace(labelpath, ".JPEG", ".txt", labelpath);
    image ماسک = make_image(w, h, classes+1);
    صحيح i;
    لكل(i = 0; i < w*h; ++i){
        ماسک.معلومات[w*h*classes + i] = 1;
    }
    FILE *file = fopen(labelpath, "r");
    إذا(!file) file_error(labelpath);
    حرف buff[32788];
    صحيح id;
    image part = make_image(w, h, 1);
    طالما(fscanf(file, "%d %s", &id, buff) == 2){
        صحيح n = 0;
        صحيح *rle = read_intlist(buff, &n, 0);
        بارول_rle(part, rle, n);
        or_image(part, ماسک, id);
        لكل(i = 0; i < w*h; ++i){
            إذا(part.معلومات[i]) ماسک.معلومات[w*h*classes + i] = 0;
        }
        حرر(rle);
    }
    //exclusive_image(mask);
    fclose(file);
    free_image(part);
    أرجع ماسک;
}

معلومات بارول_معلومات_seg(صحيح n, حرف **paths, صحيح m, صحيح w, صحيح h, صحيح classes, صحيح min, صحيح max, عائم angle, عائم aspect, عائم hue, عائم saturation, عائم exposure, صحيح div)
{
    حرف **random_paths = get_random_paths(paths, n, m);
    صحيح i;
    معلومات d = {0};
    d.shallow = 0;

    d.X.لیکې = n;
    d.X.vals = حافظه_صفر_ورکړه(d.X.لیکې, حجم(عائم*));
    d.X.ستون_شمېر = h*w*3;


    d.y.لیکې = n;
    d.y.ستون_شمېر = h*w*classes/div/div;
    d.y.vals = حافظه_صفر_ورکړه(d.X.لیکې, حجم(عائم*));

    لكل(i = 0; i < n; ++i){
        image orig = بارول_image_color(random_paths[i], 0, 0);
        augment_دلیلونه a = random_augment_دلیلونه(orig, angle, aspect, min, max, w, h);
        image sized = rotate_crop_image(orig, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);

        صحيح flip = rand()%2;
        إذا(flip) flip_image(sized);
        random_distort_image(sized, hue, saturation, exposure);
        d.X.vals[i] = sized.معلومات;

        image ماسک = get_segmentation_image(random_paths[i], orig.w, orig.h, classes);
        //image mask = make_image(orig.w, orig.h, classes+1);
        image sized_m = rotate_crop_image(ماسک, a.rad, a.scale/div, a.w/div, a.h/div, a.dx/div, a.dy/div, a.aspect);

        إذا(flip) flip_image(sized_m);
        d.y.vals[i] = sized_m.معلومات;

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

معلومات بارول_معلومات_iseg(صحيح n, حرف **paths, صحيح m, صحيح w, صحيح h, صحيح classes, صحيح boxes, صحيح div, صحيح min, صحيح max, عائم angle, عائم aspect, عائم hue, عائم saturation, عائم exposure)
{
    حرف **random_paths = get_random_paths(paths, n, m);
    صحيح i;
    معلومات d = {0};
    d.shallow = 0;

    d.X.لیکې = n;
    d.X.vals = حافظه_صفر_ورکړه(d.X.لیکې, حجم(عائم*));
    d.X.ستون_شمېر = h*w*3;

    d.y = make_ماتریکس(n, (((w/div)*(h/div))+1)*boxes);

    لكل(i = 0; i < n; ++i){
        image orig = بارول_image_color(random_paths[i], 0, 0);
        augment_دلیلونه a = random_augment_دلیلونه(orig, angle, aspect, min, max, w, h);
        image sized = rotate_crop_image(orig, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);

        صحيح flip = rand()%2;
        إذا(flip) flip_image(sized);
        random_distort_image(sized, hue, saturation, exposure);
        d.X.vals[i] = sized.معلومات;
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

معلومات بارول_معلومات_ماسک(صحيح n, حرف **paths, صحيح m, صحيح w, صحيح h, صحيح classes, صحيح boxes, صحيح coords, صحيح min, صحيح max, عائم angle, عائم aspect, عائم hue, عائم saturation, عائم exposure)
{
    حرف **random_paths = get_random_paths(paths, n, m);
    صحيح i;
    معلومات d = {0};
    d.shallow = 0;

    d.X.لیکې = n;
    d.X.vals = حافظه_صفر_ورکړه(d.X.لیکې, حجم(عائم*));
    d.X.ستون_شمېر = h*w*3;

    d.y = make_ماتریکس(n, (coords+1)*boxes);

    لكل(i = 0; i < n; ++i){
        image orig = بارول_image_color(random_paths[i], 0, 0);
        augment_دلیلونه a = random_augment_دلیلونه(orig, angle, aspect, min, max, w, h);
        image sized = rotate_crop_image(orig, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);

        صحيح flip = rand()%2;
        إذا(flip) flip_image(sized);
        random_distort_image(sized, hue, saturation, exposure);
        d.X.vals[i] = sized.معلومات;
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

معلومات بارول_معلومات_region(صحيح n, حرف **paths, صحيح m, صحيح w, صحيح h, صحيح اندازه, صحيح classes, عائم jitter, عائم hue, عائم saturation, عائم exposure)
{
    حرف **random_paths = get_random_paths(paths, n, m);
    صحيح i;
    معلومات d = {0};
    d.shallow = 0;

    d.X.لیکې = n;
    d.X.vals = حافظه_صفر_ورکړه(d.X.لیکې, حجم(عائم*));
    d.X.ستون_شمېر = h*w*3;


    صحيح k = اندازه*اندازه*(5+classes);
    d.y = make_ماتریکس(n, k);
    لكل(i = 0; i < n; ++i){
        image orig = بارول_image_color(random_paths[i], 0, 0);

        صحيح oh = orig.h;
        صحيح ow = orig.w;

        صحيح dw = (ow*jitter);
        صحيح dh = (oh*jitter);

        صحيح pleft  = rand_یوشان(-dw, dw);
        صحيح pright = rand_یوشان(-dw, dw);
        صحيح ptop   = rand_یوشان(-dh, dh);
        صحيح pbot   = rand_یوشان(-dh, dh);

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
        d.X.vals[i] = sized.معلومات;

        fill_truth_region(random_paths[i], d.y.vals[i], classes, اندازه, flip, dx, dy, 1./sx, 1./sy);

        free_image(orig);
        free_image(cropped);
    }
    حرر(random_paths);
    أرجع d;
}

معلومات بارول_معلومات_compare(صحيح n, حرف **paths, صحيح m, صحيح classes, صحيح w, صحيح h)
{
    إذا(m) paths = get_random_paths(paths, 2*n, m);
    صحيح i,j;
    معلومات d = {0};
    d.shallow = 0;

    d.X.لیکې = n;
    d.X.vals = حافظه_صفر_ورکړه(d.X.لیکې, حجم(عائم*));
    d.X.ستون_شمېر = h*w*6;

    صحيح k = 2*(classes);
    d.y = make_ماتریکس(n, k);
    لكل(i = 0; i < n; ++i){
        image im1 = بارول_image_color(paths[i*2],   w, h);
        image im2 = بارول_image_color(paths[i*2+1], w, h);

        d.X.vals[i] = حافظه_صفر_ورکړه(d.X.ستون_شمېر, حجم(عائم));
        حافظه_نقل(d.X.vals[i],         im1.معلومات, h*w*3*حجم(عائم));
        حافظه_نقل(d.X.vals[i] + h*w*3, im2.معلومات, h*w*3*حجم(عائم));

        صحيح id;
        عائم iou;

        حرف imlabel1[4096];
        حرف imlabel2[4096];
        پیدا_کړه_replace(paths[i*2],   "imgs", "labels", imlabel1);
        پیدا_کړه_replace(imlabel1, "jpg", "txt", imlabel1);
        FILE *fp1 = fopen(imlabel1, "r");

        طالما(fscanf(fp1, "%d %f", &id, &iou) == 2){
            إذا (d.y.vals[i][2*id] < iou) d.y.vals[i][2*id] = iou;
        }

        پیدا_کړه_replace(paths[i*2+1], "imgs", "labels", imlabel2);
        پیدا_کړه_replace(imlabel2, "jpg", "txt", imlabel2);
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
                d.y.vals[i][2*j]   = SECRET_عدد;
                d.y.vals[i][2*j+1] = SECRET_عدد;
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

معلومات بارول_معلومات_swag(حرف **paths, صحيح n, صحيح classes, عائم jitter)
{
    صحيح شاخص = rand()%n;
    حرف *random_لار = paths[شاخص];

    image orig = بارول_image_color(random_لار, 0, 0);
    صحيح h = orig.h;
    صحيح w = orig.w;

    معلومات d = {0};
    d.shallow = 0;
    d.w = w;
    d.h = h;

    d.X.لیکې = 1;
    d.X.vals = حافظه_صفر_ورکړه(d.X.لیکې, حجم(عائم*));
    d.X.ستون_شمېر = h*w*3;

    صحيح k = (4+classes)*90;
    d.y = make_ماتریکس(1, k);

    صحيح dw = w*jitter;
    صحيح dh = h*jitter;

    صحيح pleft  = rand_یوشان(-dw, dw);
    صحيح pright = rand_یوشان(-dw, dw);
    صحيح ptop   = rand_یوشان(-dh, dh);
    صحيح pbot   = rand_یوشان(-dh, dh);

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
    d.X.vals[0] = sized.معلومات;

    fill_truth_swag(random_لار, d.y.vals[0], classes, flip, dx, dy, 1./sx, 1./sy);

    free_image(orig);
    free_image(cropped);

    أرجع d;
}

معلومات بارول_معلومات_detection(صحيح n, حرف **paths, صحيح m, صحيح w, صحيح h, صحيح boxes, صحيح classes, عائم jitter, عائم hue, عائم saturation, عائم exposure)
{
    حرف **random_paths = get_random_paths(paths, n, m);
    صحيح i;
    معلومات d = {0};
    d.shallow = 0;

    d.X.لیکې = n;
    d.X.vals = حافظه_صفر_ورکړه(d.X.لیکې, حجم(عائم*));
    d.X.ستون_شمېر = h*w*3;

    d.y = make_ماتریکس(n, 5*boxes);
    لكل(i = 0; i < n; ++i){
        image orig = بارول_image_color(random_paths[i], 0, 0);
        image sized = make_image(w, h, orig.c);
        fill_image(sized, .5);

        عائم dw = jitter * orig.w;
        عائم dh = jitter * orig.h;

        عائم new_ar = (orig.w + rand_یوشان(-dw, dw)) / (orig.h + rand_یوشان(-dh, dh));
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

        عائم dx = rand_یوشان(0, w - nw);
        عائم dy = rand_یوشان(0, h - nh);

        ځای_پرځای_کړه_image(orig, nw, nh, dx, dy, sized);

        random_distort_image(sized, hue, saturation, exposure);

        صحيح flip = rand()%2;
        إذا(flip) flip_image(sized);
        d.X.vals[i] = sized.معلومات;


        fill_truth_detection(random_paths[i], boxes, d.y.vals[i], classes, flip, -dx/w, -dy/h, nw/w, nh/h);

        free_image(orig);
    }
    حرر(random_paths);
    أرجع d;
}

فراغ *بارول_thread(فراغ *ptr)
{
    //printf("Loading data: %d\n", rand());
    بارول_دلیلونه a = *(بنية بارول_دلیلونه*)ptr;
    إذا(a.exposure == 0) a.exposure = 1;
    إذا(a.saturation == 0) a.saturation = 1;
    إذا(a.aspect == 0) a.aspect = 1;

    إذا (a.type == OLD_CLASSIFICATION_معلومات){
        *a.d = بارول_معلومات_old(a.paths, a.n, a.m, a.labels, a.classes, a.w, a.h);
    } وإلا إذا (a.type == REGRESSION_معلومات){
        *a.d = بارول_معلومات_regression(a.paths, a.n, a.m, a.classes, a.min, a.max, a.اندازه, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    } وإلا إذا (a.type == CLASSIFICATION_معلومات){
        *a.d = بارول_معلومات_augment(a.paths, a.n, a.m, a.labels, a.classes, a.hierarchy, a.min, a.max, a.اندازه, a.angle, a.aspect, a.hue, a.saturation, a.exposure, a.center);
    } وإلا إذا (a.type == SUPER_معلومات){
        *a.d = بارول_معلومات_super(a.paths, a.n, a.m, a.w, a.h, a.scale);
    } وإلا إذا (a.type == WRITING_معلومات){
        *a.d = بارول_معلومات_writing(a.paths, a.n, a.m, a.w, a.h, a.بهر_w, a.بهر_h);
    } وإلا إذا (a.type == ISEG_معلومات){
        *a.d = بارول_معلومات_iseg(a.n, a.paths, a.m, a.w, a.h, a.classes, a.عدد_boxes, a.scale, a.min, a.max, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    } وإلا إذا (a.type == INSTANCE_معلومات){
        *a.d = بارول_معلومات_ماسک(a.n, a.paths, a.m, a.w, a.h, a.classes, a.عدد_boxes, a.coords, a.min, a.max, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    } وإلا إذا (a.type == SEGMENTATION_معلومات){
        *a.d = بارول_معلومات_seg(a.n, a.paths, a.m, a.w, a.h, a.classes, a.min, a.max, a.angle, a.aspect, a.hue, a.saturation, a.exposure, a.scale);
    } وإلا إذا (a.type == REGION_معلومات){
        *a.d = بارول_معلومات_region(a.n, a.paths, a.m, a.w, a.h, a.عدد_boxes, a.classes, a.jitter, a.hue, a.saturation, a.exposure);
    } وإلا إذا (a.type == DETECTION_معلومات){
        *a.d = بارول_معلومات_detection(a.n, a.paths, a.m, a.w, a.h, a.عدد_boxes, a.classes, a.jitter, a.hue, a.saturation, a.exposure);
    } وإلا إذا (a.type == SWAG_معلومات){
        *a.d = بارول_معلومات_swag(a.paths, a.n, a.classes, a.jitter);
    } وإلا إذا (a.type == COMPARE_معلومات){
        *a.d = بارول_معلومات_compare(a.n, a.paths, a.m, a.classes, a.w, a.h);
    } وإلا إذا (a.type == IMAGE_معلومات){
        *(a.im) = بارول_image_color(a.لار, 0, 0);
        *(a.resized) = resize_image(*(a.im), a.w, a.h);
    } وإلا إذا (a.type == LETTERBOX_معلومات){
        *(a.im) = بارول_image_color(a.لار, 0, 0);
        *(a.resized) = letterbox_image(*(a.im), a.w, a.h);
    } وإلا إذا (a.type == نښان_معلومات){
        *a.d = بارول_معلومات_نښان(a.paths, a.n, a.m, a.classes, a.min, a.max, a.اندازه, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    }
    حرر(ptr);
    أرجع 0;
}

pthread_t بارول_معلومات_دننه_thread(بارول_دلیلونه دلیلونه)
{
    pthread_t thread;
    بنية بارول_دلیلونه *ptr = حافظه_صفر_ورکړه(1, حجم(بنية بارول_دلیلونه));
    *ptr = دلیلونه;
    إذا(pthread_create(&thread, 0, بارول_thread, ptr)) error("Thread creation failed");
    أرجع thread;
}

فراغ *بارول_تارونه(فراغ *ptr)
{
    صحيح i;
    بارول_دلیلونه دلیلونه = *(بارول_دلیلونه *)ptr;
    إذا (دلیلونه.تارونه == 0) دلیلونه.تارونه = 1;
    معلومات *بهر = دلیلونه.d;
    صحيح ټول = دلیلونه.n;
    حرر(ptr);
    معلومات *buffers = حافظه_صفر_ورکړه(دلیلونه.تارونه, حجم(معلومات));
    pthread_t *تارونه = حافظه_صفر_ورکړه(دلیلونه.تارونه, حجم(pthread_t));
    لكل(i = 0; i < دلیلونه.تارونه; ++i){
        دلیلونه.d = buffers + i;
        دلیلونه.n = (i+1) * ټول/دلیلونه.تارونه - i * ټول/دلیلونه.تارونه;
        تارونه[i] = بارول_معلومات_دننه_thread(دلیلونه);
    }
    لكل(i = 0; i < دلیلونه.تارونه; ++i){
        pthread_join(تارونه[i], 0);
    }
    *بهر = concat_datas(buffers, دلیلونه.تارونه);
    بهر->shallow = 0;
    لكل(i = 0; i < دلیلونه.تارونه; ++i){
        buffers[i].shallow = 1;
        free_معلومات(buffers[i]);
    }
    حرر(buffers);
    حرر(تارونه);
    أرجع 0;
}

فراغ بارول_معلومات_blocking(بارول_دلیلونه دلیلونه)
{
    بنية بارول_دلیلونه *ptr = حافظه_صفر_ورکړه(1, حجم(بنية بارول_دلیلونه));
    *ptr = دلیلونه;
    بارول_thread(ptr);
}

pthread_t بارول_معلومات(بارول_دلیلونه دلیلونه)
{
    pthread_t thread;
    بنية بارول_دلیلونه *ptr = حافظه_صفر_ورکړه(1, حجم(بنية بارول_دلیلونه));
    *ptr = دلیلونه;
    إذا(pthread_create(&thread, 0, بارول_تارونه, ptr)) error("Thread creation failed");
    أرجع thread;
}

معلومات بارول_معلومات_writing(حرف **paths, صحيح n, صحيح m, صحيح w, صحيح h, صحيح بهر_w, صحيح بهر_h)
{
    إذا(m) paths = get_random_paths(paths, n, m);
    حرف **replace_paths = پیدا_کړه_replace_paths(paths, n, ".png", "-label.png");
    معلومات d = {0};
    d.shallow = 0;
    d.X = بارول_image_paths(paths, n, w, h);
    d.y = بارول_image_paths_خړ(replace_paths, n, بهر_w, بهر_h);
    إذا(m) حرر(paths);
    صحيح i;
    لكل(i = 0; i < n; ++i) حرر(replace_paths[i]);
    حرر(replace_paths);
    أرجع d;
}

معلومات بارول_معلومات_old(حرف **paths, صحيح n, صحيح m, حرف **labels, صحيح k, صحيح w, صحيح h)
{
    إذا(m) paths = get_random_paths(paths, n, m);
    معلومات d = {0};
    d.shallow = 0;
    d.X = بارول_image_paths(paths, n, w, h);
    d.y = بارول_labels_paths(paths, n, labels, k, 0);
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

معلومات بارول_معلومات_super(حرف **paths, صحيح n, صحيح m, صحيح w, صحيح h, صحيح scale)
{
    إذا(m) paths = get_random_paths(paths, n, m);
    معلومات d = {0};
    d.shallow = 0;

    صحيح i;
    d.X.لیکې = n;
    d.X.vals = حافظه_صفر_ورکړه(n, حجم(عائم*));
    d.X.ستون_شمېر = w*h*3;

    d.y.لیکې = n;
    d.y.vals = حافظه_صفر_ورکړه(n, حجم(عائم*));
    d.y.ستون_شمېر = w*scale * h*scale * 3;

    لكل(i = 0; i < n; ++i){
        image im = بارول_image_color(paths[i], 0, 0);
        image crop = random_crop_image(im, w*scale, h*scale);
        صحيح flip = rand()%2;
        إذا (flip) flip_image(crop);
        image resize = resize_image(crop, w, h);
        d.X.vals[i] = resize.معلومات;
        d.y.vals[i] = crop.معلومات;
        free_image(im);
    }

    إذا(m) حرر(paths);
    أرجع d;
}

معلومات بارول_معلومات_regression(حرف **paths, صحيح n, صحيح m, صحيح k, صحيح min, صحيح max, صحيح اندازه, عائم angle, عائم aspect, عائم hue, عائم saturation, عائم exposure)
{
    إذا(m) paths = get_random_paths(paths, n, m);
    معلومات d = {0};
    d.shallow = 0;
    d.X = بارول_image_augment_paths(paths, n, min, max, اندازه, angle, aspect, hue, saturation, exposure, 0);
    d.y = بارول_regression_labels_paths(paths, n, k);
    إذا(m) حرر(paths);
    أرجع d;
}

معلومات select_معلومات(معلومات *orig, صحيح *inds)
{
    معلومات d = {0};
    d.shallow = 1;
    d.w = orig[0].w;
    d.h = orig[0].h;

    d.X.لیکې = orig[0].X.لیکې;
    d.y.لیکې = orig[0].X.لیکې;

    d.X.ستون_شمېر = orig[0].X.ستون_شمېر;
    d.y.ستون_شمېر = orig[0].y.ستون_شمېر;

    d.X.vals = حافظه_صفر_ورکړه(orig[0].X.لیکې, حجم(عائم *));
    d.y.vals = حافظه_صفر_ورکړه(orig[0].y.لیکې, حجم(عائم *));
    صحيح i;
    لكل(i = 0; i < d.X.لیکې; ++i){
        d.X.vals[i] = orig[inds[i]].X.vals[i];
        d.y.vals[i] = orig[inds[i]].y.vals[i];
    }
    أرجع d;
}

معلومات *tile_معلومات(معلومات orig, صحيح divs, صحيح اندازه)
{
    معلومات *ds = حافظه_صفر_ورکړه(divs*divs, حجم(معلومات));
    صحيح i, j;
#لارښوونه omp parallel لكل
    لكل(i = 0; i < divs*divs; ++i){
        معلومات d;
        d.shallow = 0;
        d.w = orig.w/divs * اندازه;
        d.h = orig.h/divs * اندازه;
        d.X.لیکې = orig.X.لیکې;
        d.X.ستون_شمېر = d.w*d.h*3;
        d.X.vals = حافظه_صفر_ورکړه(d.X.لیکې, حجم(عائم*));

        d.y = copy_ماتریکس(orig.y);
#لارښوونه omp parallel لكل
        لكل(j = 0; j < orig.X.لیکې; ++j){
            صحيح x = (i%divs) * orig.w / divs - (d.w - orig.w/divs)/2;
            صحيح y = (i/divs) * orig.h / divs - (d.h - orig.h/divs)/2;
            image im = float_منزل_image(orig.w, orig.h, 3, orig.X.vals[j]);
            d.X.vals[j] = crop_image(im, x, y, d.w, d.h).معلومات;
        }
        ds[i] = d;
    }
    أرجع ds;
}

معلومات resize_معلومات(معلومات orig, صحيح w, صحيح h)
{
    معلومات d = {0};
    d.shallow = 0;
    d.w = w;
    d.h = h;
    صحيح i;
    d.X.لیکې = orig.X.لیکې;
    d.X.ستون_شمېر = w*h*3;
    d.X.vals = حافظه_صفر_ورکړه(d.X.لیکې, حجم(عائم*));

    d.y = copy_ماتریکس(orig.y);
#لارښوونه omp parallel لكل
    لكل(i = 0; i < orig.X.لیکې; ++i){
        image im = float_منزل_image(orig.w, orig.h, 3, orig.X.vals[i]);
        d.X.vals[i] = resize_image(im, w, h).معلومات;
    }
    أرجع d;
}

معلومات بارول_معلومات_augment(حرف **paths, صحيح n, صحيح m, حرف **labels, صحيح k, ونه *hierarchy, صحيح min, صحيح max, صحيح اندازه, عائم angle, عائم aspect, عائم hue, عائم saturation, عائم exposure, صحيح center)
{
    إذا(m) paths = get_random_paths(paths, n, m);
    معلومات d = {0};
    d.shallow = 0;
    d.w=اندازه;
    d.h=اندازه;
    d.X = بارول_image_augment_paths(paths, n, min, max, اندازه, angle, aspect, hue, saturation, exposure, center);
    d.y = بارول_labels_paths(paths, n, labels, k, hierarchy);
    إذا(m) حرر(paths);
    أرجع d;
}

معلومات بارول_معلومات_نښان(حرف **paths, صحيح n, صحيح m, صحيح k, صحيح min, صحيح max, صحيح اندازه, عائم angle, عائم aspect, عائم hue, عائم saturation, عائم exposure)
{
    إذا(m) paths = get_random_paths(paths, n, m);
    معلومات d = {0};
    d.w = اندازه;
    d.h = اندازه;
    d.shallow = 0;
    d.X = بارول_image_augment_paths(paths, n, min, max, اندازه, angle, aspect, hue, saturation, exposure, 0);
    d.y = بارول_نښانونه_paths(paths, n, k);
    إذا(m) حرر(paths);
    أرجع d;
}

ماتریکس concat_ماتریکس(ماتریکس m1, ماتریکس m2)
{
    صحيح i, شمېر = 0;
    ماتریکس m;
    m.ستون_شمېر = m1.ستون_شمېر;
    m.لیکې = m1.لیکې+m2.لیکې;
    m.vals = حافظه_صفر_ورکړه(m1.لیکې + m2.لیکې, حجم(عائم*));
    لكل(i = 0; i < m1.لیکې; ++i){
        m.vals[شمېر++] = m1.vals[i];
    }
    لكل(i = 0; i < m2.لیکې; ++i){
        m.vals[شمېر++] = m2.vals[i];
    }
    أرجع m;
}

معلومات concat_معلومات(معلومات d1, معلومات d2)
{
    معلومات d = {0};
    d.shallow = 1;
    d.X = concat_ماتریکس(d1.X, d2.X);
    d.y = concat_ماتریکس(d1.y, d2.y);
    d.w = d1.w;
    d.h = d1.h;
    أرجع d;
}

معلومات concat_datas(معلومات *d, صحيح n)
{
    صحيح i;
    معلومات بهر = {0};
    لكل(i = 0; i < n; ++i){
        معلومات new = concat_معلومات(d[i], بهر);
        free_معلومات(بهر);
        بهر = new;
    }
    أرجع بهر;
}

معلومات بارول_categorical_معلومات_csv(حرف *filename, صحيح موخه, صحيح k)
{
    معلومات d = {0};
    d.shallow = 0;
    ماتریکس X = csv_منزل_ماتریکس(filename);
    عائم *truth_1d = راوباسه_ستنه(&X, موخه);
    عائم **truth = one_hot_encode(truth_1d, X.لیکې, k);
    ماتریکس y;
    y.لیکې = X.لیکې;
    y.ستون_شمېر = k;
    y.vals = truth;
    d.X = X;
    d.y = y;
    حرر(truth_1d);
    أرجع d;
}

معلومات بارول_cifar10_معلومات(حرف *filename)
{
    معلومات d = {0};
    d.shallow = 0;
    اوږد i,j;
    ماتریکس X = make_ماتریکس(10000, 3072);
    ماتریکس y = make_ماتریکس(10000, 10);
    d.X = X;
    d.y = y;

    FILE *fp = fopen(filename, "rb");
    إذا(!fp) file_error(filename);
    لكل(i = 0; i < 10000; ++i){
        بینښه حرف بایټونه[3073];
        fread(بایټونه, 1, 3073, fp);
        صحيح class = بایټونه[0];
        y.vals[i][class] = 1;
        لكل(j = 0; j < X.ستون_شمېر; ++j){
            X.vals[i][j] = (مزدوج)بایټونه[j+1];
        }
    }
    scale_معلومات_لیکې(d, 1./255);
    //normalize_data_rows(d);
    fclose(fp);
    أرجع d;
}

فراغ get_random_batch(معلومات d, صحيح n, عائم *X, عائم *y)
{
    صحيح j;
    لكل(j = 0; j < n; ++j){
        صحيح شاخص = rand()%d.X.لیکې;
        حافظه_نقل(X+j*d.X.ستون_شمېر, d.X.vals[شاخص], d.X.ستون_شمېر*حجم(عائم));
        حافظه_نقل(y+j*d.y.ستون_شمېر, d.y.vals[شاخص], d.y.ستون_شمېر*حجم(عائم));
    }
}

فراغ get_بل_batch(معلومات d, صحيح n, صحيح دننه_واټن, عائم *X, عائم *y)
{
    صحيح j;
    لكل(j = 0; j < n; ++j){
        صحيح شاخص = دننه_واټن + j;
        حافظه_نقل(X+j*d.X.ستون_شمېر, d.X.vals[شاخص], d.X.ستون_شمېر*حجم(عائم));
        إذا(y) حافظه_نقل(y+j*d.y.ستون_شمېر, d.y.vals[شاخص], d.y.ستون_شمېر*حجم(عائم));
    }
}

فراغ smooth_معلومات(معلومات d)
{
    صحيح i, j;
    عائم scale = 1. / d.y.ستون_شمېر;
    عائم eps = .1;
    لكل(i = 0; i < d.y.لیکې; ++i){
        لكل(j = 0; j < d.y.ستون_شمېر; ++j){
            d.y.vals[i][j] = eps * scale + (1-eps) * d.y.vals[i][j];
        }
    }
}

معلومات بارول_all_cifar10()
{
    معلومات d = {0};
    d.shallow = 0;
    صحيح i,j,b;
    ماتریکس X = make_ماتریکس(50000, 3072);
    ماتریکس y = make_ماتریکس(50000, 10);
    d.X = X;
    d.y = y;


    لكل(b = 0; b < 5; ++b){
        حرف buff[256];
        sprintf(buff, "data/cifar/cifar-10-batches-bin/data_batch_%d.bin", b+1);
        FILE *fp = fopen(buff, "rb");
        إذا(!fp) file_error(buff);
        لكل(i = 0; i < 10000; ++i){
            بینښه حرف بایټونه[3073];
            fread(بایټونه, 1, 3073, fp);
            صحيح class = بایټونه[0];
            y.vals[i+b*10000][class] = 1;
            لكل(j = 0; j < X.ستون_شمېر; ++j){
                X.vals[i+b*10000][j] = (مزدوج)بایټونه[j+1];
            }
        }
        fclose(fp);
    }
    //normalize_data_rows(d);
    scale_معلومات_لیکې(d, 1./255);
    smooth_معلومات(d);
    أرجع d;
}

معلومات بارول_go(حرف *filename)
{
    FILE *fp = fopen(filename, "rb");
    ماتریکس X = make_ماتریکس(3363059, 361);
    ماتریکس y = make_ماتریکس(3363059, 361);
    صحيح لیکه, ستون;

    إذا(!fp) file_error(filename);
    حرف *label;
    صحيح شمېر = 0;
    طالما((label = fgetl(fp))){
        صحيح i;
        إذا(شمېر == X.لیکې){
            X = resize_ماتریکس(X, شمېر*2);
            y = resize_ماتریکس(y, شمېر*2);
        }
        له_مزي_ولوله(label, "%d %d", &لیکه, &ستون);
        حرف *board = fgetl(fp);

        صحيح شاخص = لیکه*19 + ستون;
        y.vals[شمېر][شاخص] = 1;

        لكل(i = 0; i < 19*19; ++i){
            عائم val = 0;
            إذا(board[i] == '1') val = 1;
            وإلا إذا(board[i] == '2') val = -1;
            X.vals[شمېر][i] = val;
        }
        ++شمېر;
        حرر(label);
        حرر(board);
    }
    X = resize_ماتریکس(X, شمېر);
    y = resize_ماتریکس(y, شمېر);

    معلومات d = {0};
    d.shallow = 0;
    d.X = X;
    d.y = y;


    fclose(fp);

    أرجع d;
}


فراغ randomize_معلومات(معلومات d)
{
    صحيح i;
    لكل(i = d.X.لیکې-1; i > 0; --i){
        صحيح شاخص = rand()%i;
        عائم *بدلون = d.X.vals[شاخص];
        d.X.vals[شاخص] = d.X.vals[i];
        d.X.vals[i] = بدلون;

        بدلون = d.y.vals[شاخص];
        d.y.vals[شاخص] = d.y.vals[i];
        d.y.vals[i] = بدلون;
    }
}

فراغ scale_معلومات_لیکې(معلومات d, عائم s)
{
    صحيح i;
    لكل(i = 0; i < d.X.لیکې; ++i){
        scale_array(d.X.vals[i], d.X.ستون_شمېر, s);
    }
}

فراغ translate_معلومات_لیکې(معلومات d, عائم s)
{
    صحيح i;
    لكل(i = 0; i < d.X.لیکې; ++i){
        translate_array(d.X.vals[i], d.X.ستون_شمېر, s);
    }
}

معلومات copy_معلومات(معلومات d)
{
    معلومات c = {0};
    c.w = d.w;
    c.h = d.h;
    c.shallow = 0;
    c.عدد_boxes = d.عدد_boxes;
    c.boxes = d.boxes;
    c.X = copy_ماتریکس(d.X);
    c.y = copy_ماتریکس(d.y);
    أرجع c;
}

فراغ normalize_معلومات_لیکې(معلومات d)
{
    صحيح i;
    لكل(i = 0; i < d.X.لیکې; ++i){
        normalize_array(d.X.vals[i], d.X.ستون_شمېر);
    }
}

معلومات get_معلومات_part(معلومات d, صحيح part, صحيح ټول)
{
    معلومات p = {0};
    p.shallow = 1;
    p.X.لیکې = d.X.لیکې * (part + 1) / ټول - d.X.لیکې * part / ټول;
    p.y.لیکې = d.y.لیکې * (part + 1) / ټول - d.y.لیکې * part / ټول;
    p.X.ستون_شمېر = d.X.ستون_شمېر;
    p.y.ستون_شمېر = d.y.ستون_شمېر;
    p.X.vals = d.X.vals + d.X.لیکې * part / ټول;
    p.y.vals = d.y.vals + d.y.لیکې * part / ټول;
    أرجع p;
}

معلومات get_random_معلومات(معلومات d, صحيح عدد)
{
    معلومات r = {0};
    r.shallow = 1;

    r.X.لیکې = عدد;
    r.y.لیکې = عدد;

    r.X.ستون_شمېر = d.X.ستون_شمېر;
    r.y.ستون_شمېر = d.y.ستون_شمېر;

    r.X.vals = حافظه_صفر_ورکړه(عدد, حجم(عائم *));
    r.y.vals = حافظه_صفر_ورکړه(عدد, حجم(عائم *));

    صحيح i;
    لكل(i = 0; i < عدد; ++i){
        صحيح شاخص = rand()%d.X.لیکې;
        r.X.vals[i] = d.X.vals[شاخص];
        r.y.vals[i] = d.y.vals[شاخص];
    }
    أرجع r;
}

معلومات *وېشل_معلومات(معلومات d, صحيح part, صحيح ټول)
{
    معلومات *وېشل = حافظه_صفر_ورکړه(2, حجم(معلومات));
    صحيح i;
    صحيح پیل = part*d.X.لیکې/ټول;
    صحيح پای_ټکی = (part+1)*d.X.لیکې/ټول;
    معلومات train;
    معلومات test;
    train.shallow = test.shallow = 1;

    test.X.لیکې = test.y.لیکې = پای_ټکی-پیل;
    train.X.لیکې = train.y.لیکې = d.X.لیکې - (پای_ټکی-پیل);
    train.X.ستون_شمېر = test.X.ستون_شمېر = d.X.ستون_شمېر;
    train.y.ستون_شمېر = test.y.ستون_شمېر = d.y.ستون_شمېر;

    train.X.vals = حافظه_صفر_ورکړه(train.X.لیکې, حجم(عائم*));
    test.X.vals = حافظه_صفر_ورکړه(test.X.لیکې, حجم(عائم*));
    train.y.vals = حافظه_صفر_ورکړه(train.y.لیکې, حجم(عائم*));
    test.y.vals = حافظه_صفر_ورکړه(test.y.لیکې, حجم(عائم*));

    لكل(i = 0; i < پیل; ++i){
        train.X.vals[i] = d.X.vals[i];
        train.y.vals[i] = d.y.vals[i];
    }
    لكل(i = پیل; i < پای_ټکی; ++i){
        test.X.vals[i-پیل] = d.X.vals[i];
        test.y.vals[i-پیل] = d.y.vals[i];
    }
    لكل(i = پای_ټکی; i < d.X.لیکې; ++i){
        train.X.vals[i-(پای_ټکی-پیل)] = d.X.vals[i];
        train.y.vals[i-(پای_ټکی-پیل)] = d.y.vals[i];
    }
    وېشل[0] = train;
    وېشل[1] = test;
    أرجع وېشل;
}

