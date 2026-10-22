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
    حرف *رستو;
    FILE *file = fopen(filename, "r");
    إذا(!file) file_error(filename);
    list *سٽون = make_list();
    طالما((رستو=fgetl(file))){
        list_وجهو(سٽون, رستو);
    }
    fclose(file);
    أرجع سٽون;
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
    حرف **random_paths = صفر_يادگيري_ڏيو(n, حجم(حرف*));
    صحيح i;
    pthread_mutex_lock(&mutex);
    لكل(i = 0; i < n; ++i){
        صحيح اشاريو = rand()%m;
        random_paths[i] = paths[اشاريو];
        //if(i == 0) printf("%s\n", paths[index]);
    }
    pthread_mutex_unlock(&mutex);
    أرجع random_paths;
}

حرف **ڳوليو_replace_paths(حرف **paths, صحيح n, حرف *ڳوليو, حرف *replace)
{
    حرف **replace_paths = صفر_يادگيري_ڏيو(n, حجم(حرف*));
    صحيح i;
    لكل(i = 0; i < n; ++i){
        حرف replaced[4096];
        ڳوليو_replace(paths[i], ڳوليو, replace, replaced);
        replace_paths[i] = copy_string(replaced);
    }
    أرجع replace_paths;
}

ميٽرڪس ڀريو_image_paths_ناسي(حرف **paths, صحيح n, صحيح w, صحيح h)
{
    صحيح i;
    ميٽرڪس X;
    X.صفون = n;
    X.vals = صفر_يادگيري_ڏيو(X.صفون, حجم(عائم*));
    X.کانن_ڳڻپ = 0;

    لكل(i = 0; i < n; ++i){
        image im = ڀريو_image(paths[i], w, h, 3);

        image ناسي = grayscale_image(im);
        free_image(im);
        im = ناسي;

        X.vals[i] = im.معلومات;
        X.کانن_ڳڻپ = im.h*im.w*im.c;
    }
    أرجع X;
}

ميٽرڪس ڀريو_image_paths(حرف **paths, صحيح n, صحيح w, صحيح h)
{
    صحيح i;
    ميٽرڪس X;
    X.صفون = n;
    X.vals = صفر_يادگيري_ڏيو(X.صفون, حجم(عائم*));
    X.کانن_ڳڻپ = 0;

    لكل(i = 0; i < n; ++i){
        image im = ڀريو_image_color(paths[i], w, h);
        X.vals[i] = im.معلومات;
        X.کانن_ڳڻپ = im.h*im.w*im.c;
    }
    أرجع X;
}

ميٽرڪس ڀريو_image_augment_paths(حرف **paths, صحيح n, صحيح min, صحيح max, صحيح سائيز, عائم angle, عائم aspect, عائم hue, عائم saturation, عائم exposure, صحيح center)
{
    صحيح i;
    ميٽرڪس X;
    X.صفون = n;
    X.vals = صفر_يادگيري_ڏيو(X.صفون, حجم(عائم*));
    X.کانن_ڳڻپ = 0;

    لكل(i = 0; i < n; ++i){
        image im = ڀريو_image_color(paths[i], 0, 0);
        image crop;
        إذا(center){
            crop = center_crop_image(im, سائيز, سائيز);
        } وإلا {
            crop = random_augment_image(im, angle, aspect, min, max, سائيز, سائيز);
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
        X.کانن_ڳڻپ = crop.h*crop.w*crop.c;
    }
    أرجع X;
}


box_label *read_boxes(حرف *filename, صحيح *n)
{
    FILE *file = fopen(filename, "r");
    إذا(!file) file_error(filename);
    عائم x, y, h, w;
    صحيح id;
    صحيح ڳڻپ = 0;
    صحيح سائيز = 64;
    box_label *boxes = صفر_يادگيري_ڏيو(سائيز, حجم(box_label));
    طالما(fscanf(file, "%d %f %f %f %f", &id, &x, &y, &w, &h) == 5){
        إذا(ڳڻپ == سائيز) {
            سائيز = سائيز * 2;
            boxes = realloc(boxes, سائيز*حجم(box_label));
        }
        boxes[ڳڻپ].id = id;
        boxes[ڳڻپ].x = x;
        boxes[ڳڻپ].y = y;
        boxes[ڳڻپ].h = h;
        boxes[ڳڻپ].w = w;
        boxes[ڳڻپ].کاٻو   = x - w/2;
        boxes[ڳڻپ].ساڄو  = x + w/2;
        boxes[ڳڻپ].چوٽي    = y - h/2;
        boxes[ڳڻپ].bottom = y + h/2;
        ++ڳڻپ;
    }
    fclose(file);
    *n = ڳڻپ;
    أرجع boxes;
}

فراغ randomize_boxes(box_label *b, صحيح n)
{
    صحيح i;
    لكل(i = 0; i < n; ++i){
        box_label مٽا_سٽا = b[i];
        صحيح اشاريو = rand()%n;
        b[i] = b[اشاريو];
        b[اشاريو] = مٽا_سٽا;
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
        boxes[i].کاٻو   = boxes[i].کاٻو  * sx - dx;
        boxes[i].ساڄو  = boxes[i].ساڄو * sx - dx;
        boxes[i].چوٽي    = boxes[i].چوٽي   * sy - dy;
        boxes[i].bottom = boxes[i].bottom* sy - dy;

        إذا(flip){
            عائم مٽا_سٽا = boxes[i].کاٻو;
            boxes[i].کاٻو = 1. - boxes[i].ساڄو;
            boxes[i].ساڄو = 1. - مٽا_سٽا;
        }

        boxes[i].کاٻو =  constrain(0, 1, boxes[i].کاٻو);
        boxes[i].ساڄو = constrain(0, 1, boxes[i].ساڄو);
        boxes[i].چوٽي =   constrain(0, 1, boxes[i].چوٽي);
        boxes[i].bottom =   constrain(0, 1, boxes[i].bottom);

        boxes[i].x = (boxes[i].کاٻو+boxes[i].ساڄو)/2;
        boxes[i].y = (boxes[i].چوٽي+boxes[i].bottom)/2;
        boxes[i].w = (boxes[i].ساڄو - boxes[i].کاٻو);
        boxes[i].h = (boxes[i].bottom - boxes[i].چوٽي);

        boxes[i].w = constrain(0, 1, boxes[i].w);
        boxes[i].h = constrain(0, 1, boxes[i].h);
    }
}

فراغ fill_truth_swag(حرف *رستو, عائم *truth, صحيح classes, صحيح flip, عائم dx, عائم dy, عائم sx, عائم sy)
{
    حرف labelpath[4096];
    ڳوليو_replace(رستو, "images", "labels", labelpath);
    ڳوليو_replace(labelpath, "JPEGImages", "labels", labelpath);
    ڳوليو_replace(labelpath, ".jpg", ".txt", labelpath);
    ڳوليو_replace(labelpath, ".JPG", ".txt", labelpath);
    ڳوليو_replace(labelpath, ".JPEG", ".txt", labelpath);

    صحيح ڳڻپ = 0;
    box_label *boxes = read_boxes(labelpath, &ڳڻپ);
    randomize_boxes(boxes, ڳڻپ);
    correct_boxes(boxes, ڳڻپ, dx, dy, sx, sy, flip);
    عائم x,y,w,h;
    صحيح id;
    صحيح i;

    لكل (i = 0; i < ڳڻپ && i < 90; ++i) {
        x =  boxes[i].x;
        y =  boxes[i].y;
        w =  boxes[i].w;
        h =  boxes[i].h;
        id = boxes[i].id;

        إذا (w < .0 || h < .0) استمر;

        صحيح اشاريو = (4+classes) * i;

        truth[اشاريو++] = x;
        truth[اشاريو++] = y;
        truth[اشاريو++] = w;
        truth[اشاريو++] = h;

        إذا (id < classes) truth[اشاريو+id] = 1;
    }
    حرر(boxes);
}

فراغ fill_truth_region(حرف *رستو, عائم *truth, صحيح classes, صحيح انگ_boxes, صحيح flip, عائم dx, عائم dy, عائم sx, عائم sy)
{
    حرف labelpath[4096];
    ڳوليو_replace(رستو, "images", "labels", labelpath);
    ڳوليو_replace(labelpath, "JPEGImages", "labels", labelpath);

    ڳوليو_replace(labelpath, ".jpg", ".txt", labelpath);
    ڳوليو_replace(labelpath, ".png", ".txt", labelpath);
    ڳوليو_replace(labelpath, ".JPG", ".txt", labelpath);
    ڳوليو_replace(labelpath, ".JPEG", ".txt", labelpath);
    صحيح ڳڻپ = 0;
    box_label *boxes = read_boxes(labelpath, &ڳڻپ);
    randomize_boxes(boxes, ڳڻپ);
    correct_boxes(boxes, ڳڻپ, dx, dy, sx, sy, flip);
    عائم x,y,w,h;
    صحيح id;
    صحيح i;

    لكل (i = 0; i < ڳڻپ; ++i) {
        x =  boxes[i].x;
        y =  boxes[i].y;
        w =  boxes[i].w;
        h =  boxes[i].h;
        id = boxes[i].id;

        إذا (w < .005 || h < .005) استمر;

        صحيح کانو = (صحيح)(x*انگ_boxes);
        صحيح صف = (صحيح)(y*انگ_boxes);

        x = x*انگ_boxes - کانو;
        y = y*انگ_boxes - صف;

        صحيح اشاريو = (کانو+صف*انگ_boxes)*(5+classes);
        إذا (truth[اشاريو]) استمر;
        truth[اشاريو++] = 1;

        إذا (id < classes) truth[اشاريو+id] = 1;
        اشاريو += classes;

        truth[اشاريو++] = x;
        truth[اشاريو++] = y;
        truth[اشاريو++] = w;
        truth[اشاريو++] = h;
    }
    حرر(boxes);
}

فراغ ڀريو_rle(image im, صحيح *rle, صحيح n)
{
    صحيح ڳڻپ = 0;
    صحيح curr = 0;
    صحيح i,j;
    لكل(i = 0; i < n; ++i){
        لكل(j = 0; j < rle[i]; ++j){
            im.معلومات[ڳڻپ++] = curr;
        }
        curr = 1 - curr;
    }
    لكل(; ڳڻپ < im.h*im.w*im.c; ++ڳڻپ){
        im.معلومات[ڳڻپ] = curr;
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

فراغ fill_truth_iseg(حرف *رستو, صحيح انگ_boxes, عائم *truth, صحيح classes, صحيح w, صحيح h, augment_دليلون aug, صحيح flip, صحيح mw, صحيح mh)
{
    حرف labelpath[4096];
    ڳوليو_replace(رستو, "images", "mask", labelpath);
    ڳوليو_replace(labelpath, "JPEGImages", "mask", labelpath);
    ڳوليو_replace(labelpath, ".jpg", ".txt", labelpath);
    ڳوليو_replace(labelpath, ".JPG", ".txt", labelpath);
    ڳوليو_replace(labelpath, ".JPEG", ".txt", labelpath);
    FILE *file = fopen(labelpath, "r");
    إذا(!file) file_error(labelpath);
    حرف buff[32788];
    صحيح id;
    صحيح i = 0;
    صحيح j;
    image part = make_image(w, h, 1);
    طالما((fscanf(file, "%d %s", &id, buff) == 2) && i < انگ_boxes){
        صحيح n = 0;
        صحيح *rle = read_intlist(buff, &n, 0);
        ڀريو_rle(part, rle, n);
        image sized = rotate_crop_image(part, aug.rad, aug.scale, aug.w, aug.h, aug.dx, aug.dy, aug.aspect);
        إذا(flip) flip_image(sized);

        image نقاب = resize_image(sized, mw, mh);
        truth[i*(mw*mh+1)] = id;
        لكل(j = 0; j < mw*mh; ++j){
            truth[i*(mw*mh + 1) + 1 + j] = نقاب.معلومات[j];
        }
        ++i;

        free_image(نقاب);
        free_image(sized);
        حرر(rle);
    }
    إذا(i < انگ_boxes) truth[i*(mw*mh+1)] = -1;
    fclose(file);
    free_image(part);
}

فراغ fill_truth_نقاب(حرف *رستو, صحيح انگ_boxes, عائم *truth, صحيح classes, صحيح w, صحيح h, augment_دليلون aug, صحيح flip, صحيح mw, صحيح mh)
{
    حرف labelpath[4096];
    ڳوليو_replace(رستو, "images", "mask", labelpath);
    ڳوليو_replace(labelpath, "JPEGImages", "mask", labelpath);
    ڳوليو_replace(labelpath, ".jpg", ".txt", labelpath);
    ڳوليو_replace(labelpath, ".JPG", ".txt", labelpath);
    ڳوليو_replace(labelpath, ".JPEG", ".txt", labelpath);
    FILE *file = fopen(labelpath, "r");
    إذا(!file) file_error(labelpath);
    حرف buff[32788];
    صحيح id;
    صحيح i = 0;
    image part = make_image(w, h, 1);
    طالما((fscanf(file, "%d %s", &id, buff) == 2) && i < انگ_boxes){
        صحيح n = 0;
        صحيح *rle = read_intlist(buff, &n, 0);
        ڀريو_rle(part, rle, n);
        image sized = rotate_crop_image(part, aug.rad, aug.scale, aug.w, aug.h, aug.dx, aug.dy, aug.aspect);
        إذا(flip) flip_image(sized);
        box b = bound_image(sized);
        إذا(b.w > 0){
            image crop = crop_image(sized, b.x, b.y, b.w, b.h);
            image نقاب = resize_image(crop, mw, mh);
            truth[i*(4 + mw*mh + 1) + 0] = (b.x + b.w/2.)/sized.w;
            truth[i*(4 + mw*mh + 1) + 1] = (b.y + b.h/2.)/sized.h;
            truth[i*(4 + mw*mh + 1) + 2] = b.w/sized.w;
            truth[i*(4 + mw*mh + 1) + 3] = b.h/sized.h;
            صحيح j;
            لكل(j = 0; j < mw*mh; ++j){
                truth[i*(4 + mw*mh + 1) + 4 + j] = نقاب.معلومات[j];
            }
            truth[i*(4 + mw*mh + 1) + 4 + mw*mh] = id;
            free_image(crop);
            free_image(نقاب);
            ++i;
        }
        free_image(sized);
        حرر(rle);
    }
    fclose(file);
    free_image(part);
}


فراغ fill_truth_detection(حرف *رستو, صحيح انگ_boxes, عائم *truth, صحيح classes, صحيح flip, عائم dx, عائم dy, عائم sx, عائم sy)
{
    حرف labelpath[4096];
    ڳوليو_replace(رستو, "images", "labels", labelpath);
    ڳوليو_replace(labelpath, "JPEGImages", "labels", labelpath);

    ڳوليو_replace(labelpath, "raw", "labels", labelpath);
    ڳوليو_replace(labelpath, ".jpg", ".txt", labelpath);
    ڳوليو_replace(labelpath, ".png", ".txt", labelpath);
    ڳوليو_replace(labelpath, ".JPG", ".txt", labelpath);
    ڳوليو_replace(labelpath, ".JPEG", ".txt", labelpath);
    صحيح ڳڻپ = 0;
    box_label *boxes = read_boxes(labelpath, &ڳڻپ);
    randomize_boxes(boxes, ڳڻپ);
    correct_boxes(boxes, ڳڻپ, dx, dy, sx, sy, flip);
    إذا(ڳڻپ > انگ_boxes) ڳڻپ = انگ_boxes;
    عائم x,y,w,h;
    صحيح id;
    صحيح i;
    صحيح sub = 0;

    لكل (i = 0; i < ڳڻپ; ++i) {
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

#وصف_ڪريو NUMCHARS 37

فراغ print_letters(عائم *pred, صحيح n)
{
    صحيح i;
    لكل(i = 0; i < n; ++i){
        صحيح اشاريو = max_اشاريو(pred+i*NUMCHARS, NUMCHARS);
        لکو("%c", int_منزل_alphanum(اشاريو));
    }
    لکو("\n");
}

فراغ fill_truth_captcha(حرف *رستو, صحيح n, عائم *truth)
{
    حرف *begin = strrchr(رستو, '/');
    ++begin;
    صحيح i;
    لكل(i = 0; i < لڙي_ڊيگهه(begin) && i < n && begin[i] != '.'; ++i){
        صحيح اشاريو = alphanum_منزل_int(begin[i]);
        إذا(اشاريو > 35) لکو("Bad %c\n", begin[i]);
        truth[i*NUMCHARS+اشاريو] = 1;
    }
    لكل(;i < n; ++i){
        truth[i*NUMCHARS + NUMCHARS-1] = 1;
    }
}

معلومات ڀريو_معلومات_captcha(حرف **paths, صحيح n, صحيح m, صحيح k, صحيح w, صحيح h)
{
    إذا(m) paths = get_random_paths(paths, n, m);
    معلومات d = {0};
    d.shallow = 0;
    d.X = ڀريو_image_paths(paths, n, w, h);
    d.y = make_ميٽرڪس(n, k*NUMCHARS);
    صحيح i;
    لكل(i = 0; i < n; ++i){
        fill_truth_captcha(paths[i], k, d.y.vals[i]);
    }
    إذا(m) حرر(paths);
    أرجع d;
}

معلومات ڀريو_معلومات_captcha_encode(حرف **paths, صحيح n, صحيح m, صحيح w, صحيح h)
{
    إذا(m) paths = get_random_paths(paths, n, m);
    معلومات d = {0};
    d.shallow = 0;
    d.X = ڀريو_image_paths(paths, n, w, h);
    d.X.کانن_ڳڻپ = 17100;
    d.y = d.X;
    إذا(m) حرر(paths);
    أرجع d;
}

فراغ fill_truth(حرف *رستو, حرف **labels, صحيح k, عائم *truth)
{
    صحيح i;
    memset(truth, 0, k*حجم(عائم));
    صحيح ڳڻپ = 0;
    لكل(i = 0; i < k; ++i){
        إذا(لڙي_ڳولا(رستو, labels[i])){
            truth[i] = 1;
            ++ڳڻپ;
            //printf("%s %s %d\n", path, labels[i], i);
        }
    }
    إذا(ڳڻپ != 1 && (k != 1 || ڳڻپ != 0)) لکو("Too many or too few labels: %d, %s\n", ڳڻپ, رستو);
}

فراغ fill_hierarchy(عائم *truth, صحيح k, وڻ *hierarchy)
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
    صحيح ڳڻپ = 0;
    لكل(j = 0; j < hierarchy->groups; ++j){
        //printf("%d\n", count);
        صحيح نقاب = 1;
        لكل(i = 0; i < hierarchy->گروهه_سائيز[j]; ++i){
            إذا(truth[ڳڻپ + i]){
                نقاب = 0;
                اكسر;
            }
        }
        إذا (نقاب) {
            لكل(i = 0; i < hierarchy->گروهه_سائيز[j]; ++i){
                truth[ڳڻپ + i] = SECRET_انگ;
            }
        }
        ڳڻپ += hierarchy->گروهه_سائيز[j];
    }
}

ميٽرڪس ڀريو_regression_labels_paths(حرف **paths, صحيح n, صحيح k)
{
    ميٽرڪس y = make_ميٽرڪس(n, k);
    صحيح i,j;
    لكل(i = 0; i < n; ++i){
        حرف labelpath[4096];
        ڳوليو_replace(paths[i], "images", "labels", labelpath);
        ڳوليو_replace(labelpath, "JPEGImages", "labels", labelpath);
        ڳوليو_replace(labelpath, ".BMP", ".txt", labelpath);
        ڳوليو_replace(labelpath, ".JPEG", ".txt", labelpath);
        ڳوليو_replace(labelpath, ".JPG", ".txt", labelpath);
        ڳوليو_replace(labelpath, ".JPeG", ".txt", labelpath);
        ڳوليو_replace(labelpath, ".Jpeg", ".txt", labelpath);
        ڳوليو_replace(labelpath, ".PNG", ".txt", labelpath);
        ڳوليو_replace(labelpath, ".TIF", ".txt", labelpath);
        ڳوليو_replace(labelpath, ".bmp", ".txt", labelpath);
        ڳوليو_replace(labelpath, ".jpeg", ".txt", labelpath);
        ڳوليو_replace(labelpath, ".jpg", ".txt", labelpath);
        ڳوليو_replace(labelpath, ".png", ".txt", labelpath);
        ڳوليو_replace(labelpath, ".tif", ".txt", labelpath);

        FILE *file = fopen(labelpath, "r");
        لكل(j = 0; j < k; ++j){
            fscanf(file, "%f", &(y.vals[i][j]));
        }
        fclose(file);
    }
    أرجع y;
}

ميٽرڪس ڀريو_labels_paths(حرف **paths, صحيح n, حرف **labels, صحيح k, وڻ *hierarchy)
{
    ميٽرڪس y = make_ميٽرڪس(n, k);
    صحيح i;
    لكل(i = 0; i < n && labels; ++i){
        fill_truth(paths[i], labels, k, y.vals[i]);
        إذا(hierarchy){
            fill_hierarchy(y.vals[i], k, hierarchy);
        }
    }
    أرجع y;
}

ميٽرڪس ڀريو_ٽيگ_فهرست_paths(حرف **paths, صحيح n, صحيح k)
{
    ميٽرڪس y = make_ميٽرڪس(n, k);
    صحيح i;
    //int count = 0;
    لكل(i = 0; i < n; ++i){
        حرف label[4096];
        ڳوليو_replace(paths[i], "images", "labels", label);
        ڳوليو_replace(label, ".jpg", ".txt", label);
        FILE *file = fopen(label, "r");
        إذا (!file) استمر;
        //++count;
        صحيح ٽيگ;
        طالما(fscanf(file, "%d", &ٽيگ) == 1){
            إذا(ٽيگ < k){
                y.vals[i][ٽيگ] = 1;
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
        free_ميٽرڪس(d.X);
        free_ميٽرڪس(d.y);
    }وإلا{
        حرر(d.X.vals);
        حرر(d.y.vals);
    }
}

image get_segmentation_image(حرف *رستو, صحيح w, صحيح h, صحيح classes)
{
    حرف labelpath[4096];
    ڳوليو_replace(رستو, "images", "mask", labelpath);
    ڳوليو_replace(labelpath, "JPEGImages", "mask", labelpath);
    ڳوليو_replace(labelpath, ".jpg", ".txt", labelpath);
    ڳوليو_replace(labelpath, ".JPG", ".txt", labelpath);
    ڳوليو_replace(labelpath, ".JPEG", ".txt", labelpath);
    image نقاب = make_image(w, h, classes);
    FILE *file = fopen(labelpath, "r");
    إذا(!file) file_error(labelpath);
    حرف buff[32788];
    صحيح id;
    image part = make_image(w, h, 1);
    طالما(fscanf(file, "%d %s", &id, buff) == 2){
        صحيح n = 0;
        صحيح *rle = read_intlist(buff, &n, 0);
        ڀريو_rle(part, rle, n);
        or_image(part, نقاب, id);
        حرر(rle);
    }
    //exclusive_image(mask);
    fclose(file);
    free_image(part);
    أرجع نقاب;
}

image get_segmentation_image2(حرف *رستو, صحيح w, صحيح h, صحيح classes)
{
    حرف labelpath[4096];
    ڳوليو_replace(رستو, "images", "mask", labelpath);
    ڳوليو_replace(labelpath, "JPEGImages", "mask", labelpath);
    ڳوليو_replace(labelpath, ".jpg", ".txt", labelpath);
    ڳوليو_replace(labelpath, ".JPG", ".txt", labelpath);
    ڳوليو_replace(labelpath, ".JPEG", ".txt", labelpath);
    image نقاب = make_image(w, h, classes+1);
    صحيح i;
    لكل(i = 0; i < w*h; ++i){
        نقاب.معلومات[w*h*classes + i] = 1;
    }
    FILE *file = fopen(labelpath, "r");
    إذا(!file) file_error(labelpath);
    حرف buff[32788];
    صحيح id;
    image part = make_image(w, h, 1);
    طالما(fscanf(file, "%d %s", &id, buff) == 2){
        صحيح n = 0;
        صحيح *rle = read_intlist(buff, &n, 0);
        ڀريو_rle(part, rle, n);
        or_image(part, نقاب, id);
        لكل(i = 0; i < w*h; ++i){
            إذا(part.معلومات[i]) نقاب.معلومات[w*h*classes + i] = 0;
        }
        حرر(rle);
    }
    //exclusive_image(mask);
    fclose(file);
    free_image(part);
    أرجع نقاب;
}

معلومات ڀريو_معلومات_seg(صحيح n, حرف **paths, صحيح m, صحيح w, صحيح h, صحيح classes, صحيح min, صحيح max, عائم angle, عائم aspect, عائم hue, عائم saturation, عائم exposure, صحيح div)
{
    حرف **random_paths = get_random_paths(paths, n, m);
    صحيح i;
    معلومات d = {0};
    d.shallow = 0;

    d.X.صفون = n;
    d.X.vals = صفر_يادگيري_ڏيو(d.X.صفون, حجم(عائم*));
    d.X.کانن_ڳڻپ = h*w*3;


    d.y.صفون = n;
    d.y.کانن_ڳڻپ = h*w*classes/div/div;
    d.y.vals = صفر_يادگيري_ڏيو(d.X.صفون, حجم(عائم*));

    لكل(i = 0; i < n; ++i){
        image orig = ڀريو_image_color(random_paths[i], 0, 0);
        augment_دليلون a = random_augment_دليلون(orig, angle, aspect, min, max, w, h);
        image sized = rotate_crop_image(orig, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);

        صحيح flip = rand()%2;
        إذا(flip) flip_image(sized);
        random_distort_image(sized, hue, saturation, exposure);
        d.X.vals[i] = sized.معلومات;

        image نقاب = get_segmentation_image(random_paths[i], orig.w, orig.h, classes);
        //image mask = make_image(orig.w, orig.h, classes+1);
        image sized_m = rotate_crop_image(نقاب, a.rad, a.scale/div, a.w/div, a.h/div, a.dx/div, a.dy/div, a.aspect);

        إذا(flip) flip_image(sized_m);
        d.y.vals[i] = sized_m.معلومات;

        free_image(orig);
        free_image(نقاب);

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

معلومات ڀريو_معلومات_iseg(صحيح n, حرف **paths, صحيح m, صحيح w, صحيح h, صحيح classes, صحيح boxes, صحيح div, صحيح min, صحيح max, عائم angle, عائم aspect, عائم hue, عائم saturation, عائم exposure)
{
    حرف **random_paths = get_random_paths(paths, n, m);
    صحيح i;
    معلومات d = {0};
    d.shallow = 0;

    d.X.صفون = n;
    d.X.vals = صفر_يادگيري_ڏيو(d.X.صفون, حجم(عائم*));
    d.X.کانن_ڳڻپ = h*w*3;

    d.y = make_ميٽرڪس(n, (((w/div)*(h/div))+1)*boxes);

    لكل(i = 0; i < n; ++i){
        image orig = ڀريو_image_color(random_paths[i], 0, 0);
        augment_دليلون a = random_augment_دليلون(orig, angle, aspect, min, max, w, h);
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

معلومات ڀريو_معلومات_نقاب(صحيح n, حرف **paths, صحيح m, صحيح w, صحيح h, صحيح classes, صحيح boxes, صحيح coords, صحيح min, صحيح max, عائم angle, عائم aspect, عائم hue, عائم saturation, عائم exposure)
{
    حرف **random_paths = get_random_paths(paths, n, m);
    صحيح i;
    معلومات d = {0};
    d.shallow = 0;

    d.X.صفون = n;
    d.X.vals = صفر_يادگيري_ڏيو(d.X.صفون, حجم(عائم*));
    d.X.کانن_ڳڻپ = h*w*3;

    d.y = make_ميٽرڪس(n, (coords+1)*boxes);

    لكل(i = 0; i < n; ++i){
        image orig = ڀريو_image_color(random_paths[i], 0, 0);
        augment_دليلون a = random_augment_دليلون(orig, angle, aspect, min, max, w, h);
        image sized = rotate_crop_image(orig, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);

        صحيح flip = rand()%2;
        إذا(flip) flip_image(sized);
        random_distort_image(sized, hue, saturation, exposure);
        d.X.vals[i] = sized.معلومات;
        //show_image(sized, "image");

        fill_truth_نقاب(random_paths[i], boxes, d.y.vals[i], classes, orig.w, orig.h, a, flip, 14, 14);

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

معلومات ڀريو_معلومات_region(صحيح n, حرف **paths, صحيح m, صحيح w, صحيح h, صحيح سائيز, صحيح classes, عائم jitter, عائم hue, عائم saturation, عائم exposure)
{
    حرف **random_paths = get_random_paths(paths, n, m);
    صحيح i;
    معلومات d = {0};
    d.shallow = 0;

    d.X.صفون = n;
    d.X.vals = صفر_يادگيري_ڏيو(d.X.صفون, حجم(عائم*));
    d.X.کانن_ڳڻپ = h*w*3;


    صحيح k = سائيز*سائيز*(5+classes);
    d.y = make_ميٽرڪس(n, k);
    لكل(i = 0; i < n; ++i){
        image orig = ڀريو_image_color(random_paths[i], 0, 0);

        صحيح oh = orig.h;
        صحيح ow = orig.w;

        صحيح dw = (ow*jitter);
        صحيح dh = (oh*jitter);

        صحيح pleft  = rand_هڪجهڙو(-dw, dw);
        صحيح pright = rand_هڪجهڙو(-dw, dw);
        صحيح ptop   = rand_هڪجهڙو(-dh, dh);
        صحيح pbot   = rand_هڪجهڙو(-dh, dh);

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

        fill_truth_region(random_paths[i], d.y.vals[i], classes, سائيز, flip, dx, dy, 1./sx, 1./sy);

        free_image(orig);
        free_image(cropped);
    }
    حرر(random_paths);
    أرجع d;
}

معلومات ڀريو_معلومات_compare(صحيح n, حرف **paths, صحيح m, صحيح classes, صحيح w, صحيح h)
{
    إذا(m) paths = get_random_paths(paths, 2*n, m);
    صحيح i,j;
    معلومات d = {0};
    d.shallow = 0;

    d.X.صفون = n;
    d.X.vals = صفر_يادگيري_ڏيو(d.X.صفون, حجم(عائم*));
    d.X.کانن_ڳڻپ = h*w*6;

    صحيح k = 2*(classes);
    d.y = make_ميٽرڪس(n, k);
    لكل(i = 0; i < n; ++i){
        image im1 = ڀريو_image_color(paths[i*2],   w, h);
        image im2 = ڀريو_image_color(paths[i*2+1], w, h);

        d.X.vals[i] = صفر_يادگيري_ڏيو(d.X.کانن_ڳڻپ, حجم(عائم));
        يادگيري_نقل(d.X.vals[i],         im1.معلومات, h*w*3*حجم(عائم));
        يادگيري_نقل(d.X.vals[i] + h*w*3, im2.معلومات, h*w*3*حجم(عائم));

        صحيح id;
        عائم iou;

        حرف imlabel1[4096];
        حرف imlabel2[4096];
        ڳوليو_replace(paths[i*2],   "imgs", "labels", imlabel1);
        ڳوليو_replace(imlabel1, "jpg", "txt", imlabel1);
        FILE *fp1 = fopen(imlabel1, "r");

        طالما(fscanf(fp1, "%d %f", &id, &iou) == 2){
            إذا (d.y.vals[i][2*id] < iou) d.y.vals[i][2*id] = iou;
        }

        ڳوليو_replace(paths[i*2+1], "imgs", "labels", imlabel2);
        ڳوليو_replace(imlabel2, "jpg", "txt", imlabel2);
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
                d.y.vals[i][2*j]   = SECRET_انگ;
                d.y.vals[i][2*j+1] = SECRET_انگ;
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

معلومات ڀريو_معلومات_swag(حرف **paths, صحيح n, صحيح classes, عائم jitter)
{
    صحيح اشاريو = rand()%n;
    حرف *random_رستو = paths[اشاريو];

    image orig = ڀريو_image_color(random_رستو, 0, 0);
    صحيح h = orig.h;
    صحيح w = orig.w;

    معلومات d = {0};
    d.shallow = 0;
    d.w = w;
    d.h = h;

    d.X.صفون = 1;
    d.X.vals = صفر_يادگيري_ڏيو(d.X.صفون, حجم(عائم*));
    d.X.کانن_ڳڻپ = h*w*3;

    صحيح k = (4+classes)*90;
    d.y = make_ميٽرڪس(1, k);

    صحيح dw = w*jitter;
    صحيح dh = h*jitter;

    صحيح pleft  = rand_هڪجهڙو(-dw, dw);
    صحيح pright = rand_هڪجهڙو(-dw, dw);
    صحيح ptop   = rand_هڪجهڙو(-dh, dh);
    صحيح pbot   = rand_هڪجهڙو(-dh, dh);

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

    fill_truth_swag(random_رستو, d.y.vals[0], classes, flip, dx, dy, 1./sx, 1./sy);

    free_image(orig);
    free_image(cropped);

    أرجع d;
}

معلومات ڀريو_معلومات_detection(صحيح n, حرف **paths, صحيح m, صحيح w, صحيح h, صحيح boxes, صحيح classes, عائم jitter, عائم hue, عائم saturation, عائم exposure)
{
    حرف **random_paths = get_random_paths(paths, n, m);
    صحيح i;
    معلومات d = {0};
    d.shallow = 0;

    d.X.صفون = n;
    d.X.vals = صفر_يادگيري_ڏيو(d.X.صفون, حجم(عائم*));
    d.X.کانن_ڳڻپ = h*w*3;

    d.y = make_ميٽرڪس(n, 5*boxes);
    لكل(i = 0; i < n; ++i){
        image orig = ڀريو_image_color(random_paths[i], 0, 0);
        image sized = make_image(w, h, orig.c);
        fill_image(sized, .5);

        عائم dw = jitter * orig.w;
        عائم dh = jitter * orig.h;

        عائم new_ar = (orig.w + rand_هڪجهڙو(-dw, dw)) / (orig.h + rand_هڪجهڙو(-dh, dh));
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

        عائم dx = rand_هڪجهڙو(0, w - nw);
        عائم dy = rand_هڪجهڙو(0, h - nh);

        رکو_image(orig, nw, nh, dx, dy, sized);

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

فراغ *ڀريو_thread(فراغ *ptr)
{
    //printf("Loading data: %d\n", rand());
    ڀريو_دليلون a = *(بنية ڀريو_دليلون*)ptr;
    إذا(a.exposure == 0) a.exposure = 1;
    إذا(a.saturation == 0) a.saturation = 1;
    إذا(a.aspect == 0) a.aspect = 1;

    إذا (a.type == OLD_CLASSIFICATION_معلومات){
        *a.d = ڀريو_معلومات_old(a.paths, a.n, a.m, a.labels, a.classes, a.w, a.h);
    } وإلا إذا (a.type == REGRESSION_معلومات){
        *a.d = ڀريو_معلومات_regression(a.paths, a.n, a.m, a.classes, a.min, a.max, a.سائيز, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    } وإلا إذا (a.type == CLASSIFICATION_معلومات){
        *a.d = ڀريو_معلومات_augment(a.paths, a.n, a.m, a.labels, a.classes, a.hierarchy, a.min, a.max, a.سائيز, a.angle, a.aspect, a.hue, a.saturation, a.exposure, a.center);
    } وإلا إذا (a.type == SUPER_معلومات){
        *a.d = ڀريو_معلومات_super(a.paths, a.n, a.m, a.w, a.h, a.scale);
    } وإلا إذا (a.type == WRITING_معلومات){
        *a.d = ڀريو_معلومات_writing(a.paths, a.n, a.m, a.w, a.h, a.ٻاهريون_w, a.ٻاهريون_h);
    } وإلا إذا (a.type == ISEG_معلومات){
        *a.d = ڀريو_معلومات_iseg(a.n, a.paths, a.m, a.w, a.h, a.classes, a.انگ_boxes, a.scale, a.min, a.max, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    } وإلا إذا (a.type == INSTANCE_معلومات){
        *a.d = ڀريو_معلومات_نقاب(a.n, a.paths, a.m, a.w, a.h, a.classes, a.انگ_boxes, a.coords, a.min, a.max, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    } وإلا إذا (a.type == SEGMENTATION_معلومات){
        *a.d = ڀريو_معلومات_seg(a.n, a.paths, a.m, a.w, a.h, a.classes, a.min, a.max, a.angle, a.aspect, a.hue, a.saturation, a.exposure, a.scale);
    } وإلا إذا (a.type == REGION_معلومات){
        *a.d = ڀريو_معلومات_region(a.n, a.paths, a.m, a.w, a.h, a.انگ_boxes, a.classes, a.jitter, a.hue, a.saturation, a.exposure);
    } وإلا إذا (a.type == DETECTION_معلومات){
        *a.d = ڀريو_معلومات_detection(a.n, a.paths, a.m, a.w, a.h, a.انگ_boxes, a.classes, a.jitter, a.hue, a.saturation, a.exposure);
    } وإلا إذا (a.type == SWAG_معلومات){
        *a.d = ڀريو_معلومات_swag(a.paths, a.n, a.classes, a.jitter);
    } وإلا إذا (a.type == COMPARE_معلومات){
        *a.d = ڀريو_معلومات_compare(a.n, a.paths, a.m, a.classes, a.w, a.h);
    } وإلا إذا (a.type == IMAGE_معلومات){
        *(a.im) = ڀريو_image_color(a.رستو, 0, 0);
        *(a.resized) = resize_image(*(a.im), a.w, a.h);
    } وإلا إذا (a.type == LETTERBOX_معلومات){
        *(a.im) = ڀريو_image_color(a.رستو, 0, 0);
        *(a.resized) = letterbox_image(*(a.im), a.w, a.h);
    } وإلا إذا (a.type == ٽيگ_معلومات){
        *a.d = ڀريو_معلومات_ٽيگ(a.paths, a.n, a.m, a.classes, a.min, a.max, a.سائيز, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    }
    حرر(ptr);
    أرجع 0;
}

pthread_t ڀريو_معلومات_اندريون_thread(ڀريو_دليلون دليلون)
{
    pthread_t thread;
    بنية ڀريو_دليلون *ptr = صفر_يادگيري_ڏيو(1, حجم(بنية ڀريو_دليلون));
    *ptr = دليلون;
    إذا(pthread_create(&thread, 0, ڀريو_thread, ptr)) error("Thread creation failed");
    أرجع thread;
}

فراغ *ڀريو_ڌاڳا(فراغ *ptr)
{
    صحيح i;
    ڀريو_دليلون دليلون = *(ڀريو_دليلون *)ptr;
    إذا (دليلون.ڌاڳا == 0) دليلون.ڌاڳا = 1;
    معلومات *ٻاهريون = دليلون.d;
    صحيح ڪل = دليلون.n;
    حرر(ptr);
    معلومات *buffers = صفر_يادگيري_ڏيو(دليلون.ڌاڳا, حجم(معلومات));
    pthread_t *ڌاڳا = صفر_يادگيري_ڏيو(دليلون.ڌاڳا, حجم(pthread_t));
    لكل(i = 0; i < دليلون.ڌاڳا; ++i){
        دليلون.d = buffers + i;
        دليلون.n = (i+1) * ڪل/دليلون.ڌاڳا - i * ڪل/دليلون.ڌاڳا;
        ڌاڳا[i] = ڀريو_معلومات_اندريون_thread(دليلون);
    }
    لكل(i = 0; i < دليلون.ڌاڳا; ++i){
        pthread_join(ڌاڳا[i], 0);
    }
    *ٻاهريون = concat_datas(buffers, دليلون.ڌاڳا);
    ٻاهريون->shallow = 0;
    لكل(i = 0; i < دليلون.ڌاڳا; ++i){
        buffers[i].shallow = 1;
        free_معلومات(buffers[i]);
    }
    حرر(buffers);
    حرر(ڌاڳا);
    أرجع 0;
}

فراغ ڀريو_معلومات_blocking(ڀريو_دليلون دليلون)
{
    بنية ڀريو_دليلون *ptr = صفر_يادگيري_ڏيو(1, حجم(بنية ڀريو_دليلون));
    *ptr = دليلون;
    ڀريو_thread(ptr);
}

pthread_t ڀريو_معلومات(ڀريو_دليلون دليلون)
{
    pthread_t thread;
    بنية ڀريو_دليلون *ptr = صفر_يادگيري_ڏيو(1, حجم(بنية ڀريو_دليلون));
    *ptr = دليلون;
    إذا(pthread_create(&thread, 0, ڀريو_ڌاڳا, ptr)) error("Thread creation failed");
    أرجع thread;
}

معلومات ڀريو_معلومات_writing(حرف **paths, صحيح n, صحيح m, صحيح w, صحيح h, صحيح ٻاهريون_w, صحيح ٻاهريون_h)
{
    إذا(m) paths = get_random_paths(paths, n, m);
    حرف **replace_paths = ڳوليو_replace_paths(paths, n, ".png", "-label.png");
    معلومات d = {0};
    d.shallow = 0;
    d.X = ڀريو_image_paths(paths, n, w, h);
    d.y = ڀريو_image_paths_ناسي(replace_paths, n, ٻاهريون_w, ٻاهريون_h);
    إذا(m) حرر(paths);
    صحيح i;
    لكل(i = 0; i < n; ++i) حرر(replace_paths[i]);
    حرر(replace_paths);
    أرجع d;
}

معلومات ڀريو_معلومات_old(حرف **paths, صحيح n, صحيح m, حرف **labels, صحيح k, صحيح w, صحيح h)
{
    إذا(m) paths = get_random_paths(paths, n, m);
    معلومات d = {0};
    d.shallow = 0;
    d.X = ڀريو_image_paths(paths, n, w, h);
    d.y = ڀريو_labels_paths(paths, n, labels, k, 0);
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

معلومات ڀريو_معلومات_super(حرف **paths, صحيح n, صحيح m, صحيح w, صحيح h, صحيح scale)
{
    إذا(m) paths = get_random_paths(paths, n, m);
    معلومات d = {0};
    d.shallow = 0;

    صحيح i;
    d.X.صفون = n;
    d.X.vals = صفر_يادگيري_ڏيو(n, حجم(عائم*));
    d.X.کانن_ڳڻپ = w*h*3;

    d.y.صفون = n;
    d.y.vals = صفر_يادگيري_ڏيو(n, حجم(عائم*));
    d.y.کانن_ڳڻپ = w*scale * h*scale * 3;

    لكل(i = 0; i < n; ++i){
        image im = ڀريو_image_color(paths[i], 0, 0);
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

معلومات ڀريو_معلومات_regression(حرف **paths, صحيح n, صحيح m, صحيح k, صحيح min, صحيح max, صحيح سائيز, عائم angle, عائم aspect, عائم hue, عائم saturation, عائم exposure)
{
    إذا(m) paths = get_random_paths(paths, n, m);
    معلومات d = {0};
    d.shallow = 0;
    d.X = ڀريو_image_augment_paths(paths, n, min, max, سائيز, angle, aspect, hue, saturation, exposure, 0);
    d.y = ڀريو_regression_labels_paths(paths, n, k);
    إذا(m) حرر(paths);
    أرجع d;
}

معلومات select_معلومات(معلومات *orig, صحيح *inds)
{
    معلومات d = {0};
    d.shallow = 1;
    d.w = orig[0].w;
    d.h = orig[0].h;

    d.X.صفون = orig[0].X.صفون;
    d.y.صفون = orig[0].X.صفون;

    d.X.کانن_ڳڻپ = orig[0].X.کانن_ڳڻپ;
    d.y.کانن_ڳڻپ = orig[0].y.کانن_ڳڻپ;

    d.X.vals = صفر_يادگيري_ڏيو(orig[0].X.صفون, حجم(عائم *));
    d.y.vals = صفر_يادگيري_ڏيو(orig[0].y.صفون, حجم(عائم *));
    صحيح i;
    لكل(i = 0; i < d.X.صفون; ++i){
        d.X.vals[i] = orig[inds[i]].X.vals[i];
        d.y.vals[i] = orig[inds[i]].y.vals[i];
    }
    أرجع d;
}

معلومات *tile_معلومات(معلومات orig, صحيح divs, صحيح سائيز)
{
    معلومات *ds = صفر_يادگيري_ڏيو(divs*divs, حجم(معلومات));
    صحيح i, j;
#هدايت omp parallel لكل
    لكل(i = 0; i < divs*divs; ++i){
        معلومات d;
        d.shallow = 0;
        d.w = orig.w/divs * سائيز;
        d.h = orig.h/divs * سائيز;
        d.X.صفون = orig.X.صفون;
        d.X.کانن_ڳڻپ = d.w*d.h*3;
        d.X.vals = صفر_يادگيري_ڏيو(d.X.صفون, حجم(عائم*));

        d.y = copy_ميٽرڪس(orig.y);
#هدايت omp parallel لكل
        لكل(j = 0; j < orig.X.صفون; ++j){
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
    d.X.صفون = orig.X.صفون;
    d.X.کانن_ڳڻپ = w*h*3;
    d.X.vals = صفر_يادگيري_ڏيو(d.X.صفون, حجم(عائم*));

    d.y = copy_ميٽرڪس(orig.y);
#هدايت omp parallel لكل
    لكل(i = 0; i < orig.X.صفون; ++i){
        image im = float_منزل_image(orig.w, orig.h, 3, orig.X.vals[i]);
        d.X.vals[i] = resize_image(im, w, h).معلومات;
    }
    أرجع d;
}

معلومات ڀريو_معلومات_augment(حرف **paths, صحيح n, صحيح m, حرف **labels, صحيح k, وڻ *hierarchy, صحيح min, صحيح max, صحيح سائيز, عائم angle, عائم aspect, عائم hue, عائم saturation, عائم exposure, صحيح center)
{
    إذا(m) paths = get_random_paths(paths, n, m);
    معلومات d = {0};
    d.shallow = 0;
    d.w=سائيز;
    d.h=سائيز;
    d.X = ڀريو_image_augment_paths(paths, n, min, max, سائيز, angle, aspect, hue, saturation, exposure, center);
    d.y = ڀريو_labels_paths(paths, n, labels, k, hierarchy);
    إذا(m) حرر(paths);
    أرجع d;
}

معلومات ڀريو_معلومات_ٽيگ(حرف **paths, صحيح n, صحيح m, صحيح k, صحيح min, صحيح max, صحيح سائيز, عائم angle, عائم aspect, عائم hue, عائم saturation, عائم exposure)
{
    إذا(m) paths = get_random_paths(paths, n, m);
    معلومات d = {0};
    d.w = سائيز;
    d.h = سائيز;
    d.shallow = 0;
    d.X = ڀريو_image_augment_paths(paths, n, min, max, سائيز, angle, aspect, hue, saturation, exposure, 0);
    d.y = ڀريو_ٽيگ_فهرست_paths(paths, n, k);
    إذا(m) حرر(paths);
    أرجع d;
}

ميٽرڪس concat_ميٽرڪس(ميٽرڪس m1, ميٽرڪس m2)
{
    صحيح i, ڳڻپ = 0;
    ميٽرڪس m;
    m.کانن_ڳڻپ = m1.کانن_ڳڻپ;
    m.صفون = m1.صفون+m2.صفون;
    m.vals = صفر_يادگيري_ڏيو(m1.صفون + m2.صفون, حجم(عائم*));
    لكل(i = 0; i < m1.صفون; ++i){
        m.vals[ڳڻپ++] = m1.vals[i];
    }
    لكل(i = 0; i < m2.صفون; ++i){
        m.vals[ڳڻپ++] = m2.vals[i];
    }
    أرجع m;
}

معلومات concat_معلومات(معلومات d1, معلومات d2)
{
    معلومات d = {0};
    d.shallow = 1;
    d.X = concat_ميٽرڪس(d1.X, d2.X);
    d.y = concat_ميٽرڪس(d1.y, d2.y);
    d.w = d1.w;
    d.h = d1.h;
    أرجع d;
}

معلومات concat_datas(معلومات *d, صحيح n)
{
    صحيح i;
    معلومات ٻاهريون = {0};
    لكل(i = 0; i < n; ++i){
        معلومات new = concat_معلومات(d[i], ٻاهريون);
        free_معلومات(ٻاهريون);
        ٻاهريون = new;
    }
    أرجع ٻاهريون;
}

معلومات ڀريو_categorical_معلومات_csv(حرف *filename, صحيح هدف, صحيح k)
{
    معلومات d = {0};
    d.shallow = 0;
    ميٽرڪس X = csv_منزل_ميٽرڪس(filename);
    عائم *truth_1d = ڪڍو_ڪالم(&X, هدف);
    عائم **truth = one_hot_encode(truth_1d, X.صفون, k);
    ميٽرڪس y;
    y.صفون = X.صفون;
    y.کانن_ڳڻپ = k;
    y.vals = truth;
    d.X = X;
    d.y = y;
    حرر(truth_1d);
    أرجع d;
}

معلومات ڀريو_cifar10_معلومات(حرف *filename)
{
    معلومات d = {0};
    d.shallow = 0;
    ڊگهو i,j;
    ميٽرڪس X = make_ميٽرڪس(10000, 3072);
    ميٽرڪس y = make_ميٽرڪس(10000, 10);
    d.X = X;
    d.y = y;

    FILE *fp = fopen(filename, "rb");
    إذا(!fp) file_error(filename);
    لكل(i = 0; i < 10000; ++i){
        بنانشان حرف بائيٽون[3073];
        fread(بائيٽون, 1, 3073, fp);
        صحيح class = بائيٽون[0];
        y.vals[i][class] = 1;
        لكل(j = 0; j < X.کانن_ڳڻپ; ++j){
            X.vals[i][j] = (مزدوج)بائيٽون[j+1];
        }
    }
    scale_معلومات_صفون(d, 1./255);
    //normalize_data_rows(d);
    fclose(fp);
    أرجع d;
}

فراغ get_random_batch(معلومات d, صحيح n, عائم *X, عائم *y)
{
    صحيح j;
    لكل(j = 0; j < n; ++j){
        صحيح اشاريو = rand()%d.X.صفون;
        يادگيري_نقل(X+j*d.X.کانن_ڳڻپ, d.X.vals[اشاريو], d.X.کانن_ڳڻپ*حجم(عائم));
        يادگيري_نقل(y+j*d.y.کانن_ڳڻپ, d.y.vals[اشاريو], d.y.کانن_ڳڻپ*حجم(عائم));
    }
}

فراغ get_ايندڙ_batch(معلومات d, صحيح n, صحيح وٿي, عائم *X, عائم *y)
{
    صحيح j;
    لكل(j = 0; j < n; ++j){
        صحيح اشاريو = وٿي + j;
        يادگيري_نقل(X+j*d.X.کانن_ڳڻپ, d.X.vals[اشاريو], d.X.کانن_ڳڻپ*حجم(عائم));
        إذا(y) يادگيري_نقل(y+j*d.y.کانن_ڳڻپ, d.y.vals[اشاريو], d.y.کانن_ڳڻپ*حجم(عائم));
    }
}

فراغ smooth_معلومات(معلومات d)
{
    صحيح i, j;
    عائم scale = 1. / d.y.کانن_ڳڻپ;
    عائم eps = .1;
    لكل(i = 0; i < d.y.صفون; ++i){
        لكل(j = 0; j < d.y.کانن_ڳڻپ; ++j){
            d.y.vals[i][j] = eps * scale + (1-eps) * d.y.vals[i][j];
        }
    }
}

معلومات ڀريو_all_cifar10()
{
    معلومات d = {0};
    d.shallow = 0;
    صحيح i,j,b;
    ميٽرڪس X = make_ميٽرڪس(50000, 3072);
    ميٽرڪس y = make_ميٽرڪس(50000, 10);
    d.X = X;
    d.y = y;


    لكل(b = 0; b < 5; ++b){
        حرف buff[256];
        sprintf(buff, "data/cifar/cifar-10-batches-bin/data_batch_%d.bin", b+1);
        FILE *fp = fopen(buff, "rb");
        إذا(!fp) file_error(buff);
        لكل(i = 0; i < 10000; ++i){
            بنانشان حرف بائيٽون[3073];
            fread(بائيٽون, 1, 3073, fp);
            صحيح class = بائيٽون[0];
            y.vals[i+b*10000][class] = 1;
            لكل(j = 0; j < X.کانن_ڳڻپ; ++j){
                X.vals[i+b*10000][j] = (مزدوج)بائيٽون[j+1];
            }
        }
        fclose(fp);
    }
    //normalize_data_rows(d);
    scale_معلومات_صفون(d, 1./255);
    smooth_معلومات(d);
    أرجع d;
}

معلومات ڀريو_go(حرف *filename)
{
    FILE *fp = fopen(filename, "rb");
    ميٽرڪس X = make_ميٽرڪس(3363059, 361);
    ميٽرڪس y = make_ميٽرڪس(3363059, 361);
    صحيح صف, کانو;

    إذا(!fp) file_error(filename);
    حرف *label;
    صحيح ڳڻپ = 0;
    طالما((label = fgetl(fp))){
        صحيح i;
        إذا(ڳڻپ == X.صفون){
            X = resize_ميٽرڪس(X, ڳڻپ*2);
            y = resize_ميٽرڪس(y, ڳڻپ*2);
        }
        لڙي_مان_پڙهو(label, "%d %d", &صف, &کانو);
        حرف *board = fgetl(fp);

        صحيح اشاريو = صف*19 + کانو;
        y.vals[ڳڻپ][اشاريو] = 1;

        لكل(i = 0; i < 19*19; ++i){
            عائم val = 0;
            إذا(board[i] == '1') val = 1;
            وإلا إذا(board[i] == '2') val = -1;
            X.vals[ڳڻپ][i] = val;
        }
        ++ڳڻپ;
        حرر(label);
        حرر(board);
    }
    X = resize_ميٽرڪس(X, ڳڻپ);
    y = resize_ميٽرڪس(y, ڳڻپ);

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
    لكل(i = d.X.صفون-1; i > 0; --i){
        صحيح اشاريو = rand()%i;
        عائم *مٽا_سٽا = d.X.vals[اشاريو];
        d.X.vals[اشاريو] = d.X.vals[i];
        d.X.vals[i] = مٽا_سٽا;

        مٽا_سٽا = d.y.vals[اشاريو];
        d.y.vals[اشاريو] = d.y.vals[i];
        d.y.vals[i] = مٽا_سٽا;
    }
}

فراغ scale_معلومات_صفون(معلومات d, عائم s)
{
    صحيح i;
    لكل(i = 0; i < d.X.صفون; ++i){
        scale_array(d.X.vals[i], d.X.کانن_ڳڻپ, s);
    }
}

فراغ translate_معلومات_صفون(معلومات d, عائم s)
{
    صحيح i;
    لكل(i = 0; i < d.X.صفون; ++i){
        translate_array(d.X.vals[i], d.X.کانن_ڳڻپ, s);
    }
}

معلومات copy_معلومات(معلومات d)
{
    معلومات c = {0};
    c.w = d.w;
    c.h = d.h;
    c.shallow = 0;
    c.انگ_boxes = d.انگ_boxes;
    c.boxes = d.boxes;
    c.X = copy_ميٽرڪس(d.X);
    c.y = copy_ميٽرڪس(d.y);
    أرجع c;
}

فراغ normalize_معلومات_صفون(معلومات d)
{
    صحيح i;
    لكل(i = 0; i < d.X.صفون; ++i){
        normalize_array(d.X.vals[i], d.X.کانن_ڳڻپ);
    }
}

معلومات get_معلومات_part(معلومات d, صحيح part, صحيح ڪل)
{
    معلومات p = {0};
    p.shallow = 1;
    p.X.صفون = d.X.صفون * (part + 1) / ڪل - d.X.صفون * part / ڪل;
    p.y.صفون = d.y.صفون * (part + 1) / ڪل - d.y.صفون * part / ڪل;
    p.X.کانن_ڳڻپ = d.X.کانن_ڳڻپ;
    p.y.کانن_ڳڻپ = d.y.کانن_ڳڻپ;
    p.X.vals = d.X.vals + d.X.صفون * part / ڪل;
    p.y.vals = d.y.vals + d.y.صفون * part / ڪل;
    أرجع p;
}

معلومات get_random_معلومات(معلومات d, صحيح انگ)
{
    معلومات r = {0};
    r.shallow = 1;

    r.X.صفون = انگ;
    r.y.صفون = انگ;

    r.X.کانن_ڳڻپ = d.X.کانن_ڳڻپ;
    r.y.کانن_ڳڻپ = d.y.کانن_ڳڻپ;

    r.X.vals = صفر_يادگيري_ڏيو(انگ, حجم(عائم *));
    r.y.vals = صفر_يادگيري_ڏيو(انگ, حجم(عائم *));

    صحيح i;
    لكل(i = 0; i < انگ; ++i){
        صحيح اشاريو = rand()%d.X.صفون;
        r.X.vals[i] = d.X.vals[اشاريو];
        r.y.vals[i] = d.y.vals[اشاريو];
    }
    أرجع r;
}

معلومات *ورهايو_معلومات(معلومات d, صحيح part, صحيح ڪل)
{
    معلومات *ورهايو = صفر_يادگيري_ڏيو(2, حجم(معلومات));
    صحيح i;
    صحيح شروعات = part*d.X.صفون/ڪل;
    صحيح پڇاڙي = (part+1)*d.X.صفون/ڪل;
    معلومات train;
    معلومات test;
    train.shallow = test.shallow = 1;

    test.X.صفون = test.y.صفون = پڇاڙي-شروعات;
    train.X.صفون = train.y.صفون = d.X.صفون - (پڇاڙي-شروعات);
    train.X.کانن_ڳڻپ = test.X.کانن_ڳڻپ = d.X.کانن_ڳڻپ;
    train.y.کانن_ڳڻپ = test.y.کانن_ڳڻپ = d.y.کانن_ڳڻپ;

    train.X.vals = صفر_يادگيري_ڏيو(train.X.صفون, حجم(عائم*));
    test.X.vals = صفر_يادگيري_ڏيو(test.X.صفون, حجم(عائم*));
    train.y.vals = صفر_يادگيري_ڏيو(train.y.صفون, حجم(عائم*));
    test.y.vals = صفر_يادگيري_ڏيو(test.y.صفون, حجم(عائم*));

    لكل(i = 0; i < شروعات; ++i){
        train.X.vals[i] = d.X.vals[i];
        train.y.vals[i] = d.y.vals[i];
    }
    لكل(i = شروعات; i < پڇاڙي; ++i){
        test.X.vals[i-شروعات] = d.X.vals[i];
        test.y.vals[i-شروعات] = d.y.vals[i];
    }
    لكل(i = پڇاڙي; i < d.X.صفون; ++i){
        train.X.vals[i-(پڇاڙي-شروعات)] = d.X.vals[i];
        train.y.vals[i-(پڇاڙي-شروعات)] = d.y.vals[i];
    }
    ورهايو[0] = train;
    ورهايو[1] = test;
    أرجع ورهايو;
}

