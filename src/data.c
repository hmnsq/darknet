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
    حرف *رستہ;
    FILE *file = fopen(filename, "r");
    إذا(!file) file_error(filename);
    list *سطریں = make_list();
    طالما((رستہ=fgetl(file))){
        list_داخل_کرو(سطریں, رستہ);
    }
    fclose(file);
    أرجع سطریں;
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
    حرف **random_paths = صفر_حافظہ_دیو(n, حجم(حرف*));
    صحيح i;
    pthread_mutex_lock(&mutex);
    لكل(i = 0; i < n; ++i){
        صحيح اشاریہ = rand()%m;
        random_paths[i] = paths[اشاریہ];
        //if(i == 0) printf("%s\n", paths[index]);
    }
    pthread_mutex_unlock(&mutex);
    أرجع random_paths;
}

حرف **لبھو_replace_paths(حرف **paths, صحيح n, حرف *لبھو, حرف *replace)
{
    حرف **replace_paths = صفر_حافظہ_دیو(n, حجم(حرف*));
    صحيح i;
    لكل(i = 0; i < n; ++i){
        حرف replaced[4096];
        لبھو_replace(paths[i], لبھو, replace, replaced);
        replace_paths[i] = copy_string(replaced);
    }
    أرجع replace_paths;
}

میٹرکس لدو_image_paths_سلیٹی(حرف **paths, صحيح n, صحيح w, صحيح h)
{
    صحيح i;
    میٹرکس X;
    X.صفاں = n;
    X.vals = صفر_حافظہ_دیو(X.صفاں, حجم(عائم*));
    X.خانے_گنتی = 0;

    لكل(i = 0; i < n; ++i){
        image im = لدو_image(paths[i], w, h, 3);

        image سلیٹی = grayscale_image(im);
        free_image(im);
        im = سلیٹی;

        X.vals[i] = im.انکڑے;
        X.خانے_گنتی = im.h*im.w*im.c;
    }
    أرجع X;
}

میٹرکس لدو_image_paths(حرف **paths, صحيح n, صحيح w, صحيح h)
{
    صحيح i;
    میٹرکس X;
    X.صفاں = n;
    X.vals = صفر_حافظہ_دیو(X.صفاں, حجم(عائم*));
    X.خانے_گنتی = 0;

    لكل(i = 0; i < n; ++i){
        image im = لدو_image_color(paths[i], w, h);
        X.vals[i] = im.انکڑے;
        X.خانے_گنتی = im.h*im.w*im.c;
    }
    أرجع X;
}

میٹرکس لدو_image_augment_paths(حرف **paths, صحيح n, صحيح min, صحيح max, صحيح ناپ, عائم angle, عائم aspect, عائم hue, عائم saturation, عائم exposure, صحيح center)
{
    صحيح i;
    میٹرکس X;
    X.صفاں = n;
    X.vals = صفر_حافظہ_دیو(X.صفاں, حجم(عائم*));
    X.خانے_گنتی = 0;

    لكل(i = 0; i < n; ++i){
        image im = لدو_image_color(paths[i], 0, 0);
        image crop;
        إذا(center){
            crop = center_crop_image(im, ناپ, ناپ);
        } وإلا {
            crop = random_augment_image(im, angle, aspect, min, max, ناپ, ناپ);
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
        X.vals[i] = crop.انکڑے;
        X.خانے_گنتی = crop.h*crop.w*crop.c;
    }
    أرجع X;
}


box_label *read_boxes(حرف *filename, صحيح *n)
{
    FILE *file = fopen(filename, "r");
    إذا(!file) file_error(filename);
    عائم x, y, h, w;
    صحيح id;
    صحيح گنتی = 0;
    صحيح ناپ = 64;
    box_label *boxes = صفر_حافظہ_دیو(ناپ, حجم(box_label));
    طالما(fscanf(file, "%d %f %f %f %f", &id, &x, &y, &w, &h) == 5){
        إذا(گنتی == ناپ) {
            ناپ = ناپ * 2;
            boxes = realloc(boxes, ناپ*حجم(box_label));
        }
        boxes[گنتی].id = id;
        boxes[گنتی].x = x;
        boxes[گنتی].y = y;
        boxes[گنتی].h = h;
        boxes[گنتی].w = w;
        boxes[گنتی].بایاں   = x - w/2;
        boxes[گنتی].دایاں  = x + w/2;
        boxes[گنتی].چوٹی    = y - h/2;
        boxes[گنتی].bottom = y + h/2;
        ++گنتی;
    }
    fclose(file);
    *n = گنتی;
    أرجع boxes;
}

فراغ randomize_boxes(box_label *b, صحيح n)
{
    صحيح i;
    لكل(i = 0; i < n; ++i){
        box_label وٹاندرا = b[i];
        صحيح اشاریہ = rand()%n;
        b[i] = b[اشاریہ];
        b[اشاریہ] = وٹاندرا;
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
        boxes[i].بایاں   = boxes[i].بایاں  * sx - dx;
        boxes[i].دایاں  = boxes[i].دایاں * sx - dx;
        boxes[i].چوٹی    = boxes[i].چوٹی   * sy - dy;
        boxes[i].bottom = boxes[i].bottom* sy - dy;

        إذا(flip){
            عائم وٹاندرا = boxes[i].بایاں;
            boxes[i].بایاں = 1. - boxes[i].دایاں;
            boxes[i].دایاں = 1. - وٹاندرا;
        }

        boxes[i].بایاں =  constrain(0, 1, boxes[i].بایاں);
        boxes[i].دایاں = constrain(0, 1, boxes[i].دایاں);
        boxes[i].چوٹی =   constrain(0, 1, boxes[i].چوٹی);
        boxes[i].bottom =   constrain(0, 1, boxes[i].bottom);

        boxes[i].x = (boxes[i].بایاں+boxes[i].دایاں)/2;
        boxes[i].y = (boxes[i].چوٹی+boxes[i].bottom)/2;
        boxes[i].w = (boxes[i].دایاں - boxes[i].بایاں);
        boxes[i].h = (boxes[i].bottom - boxes[i].چوٹی);

        boxes[i].w = constrain(0, 1, boxes[i].w);
        boxes[i].h = constrain(0, 1, boxes[i].h);
    }
}

فراغ fill_truth_swag(حرف *رستہ, عائم *truth, صحيح classes, صحيح flip, عائم dx, عائم dy, عائم sx, عائم sy)
{
    حرف labelpath[4096];
    لبھو_replace(رستہ, "images", "labels", labelpath);
    لبھو_replace(labelpath, "JPEGImages", "labels", labelpath);
    لبھو_replace(labelpath, ".jpg", ".txt", labelpath);
    لبھو_replace(labelpath, ".JPG", ".txt", labelpath);
    لبھو_replace(labelpath, ".JPEG", ".txt", labelpath);

    صحيح گنتی = 0;
    box_label *boxes = read_boxes(labelpath, &گنتی);
    randomize_boxes(boxes, گنتی);
    correct_boxes(boxes, گنتی, dx, dy, sx, sy, flip);
    عائم x,y,w,h;
    صحيح id;
    صحيح i;

    لكل (i = 0; i < گنتی && i < 90; ++i) {
        x =  boxes[i].x;
        y =  boxes[i].y;
        w =  boxes[i].w;
        h =  boxes[i].h;
        id = boxes[i].id;

        إذا (w < .0 || h < .0) استمر;

        صحيح اشاریہ = (4+classes) * i;

        truth[اشاریہ++] = x;
        truth[اشاریہ++] = y;
        truth[اشاریہ++] = w;
        truth[اشاریہ++] = h;

        إذا (id < classes) truth[اشاریہ+id] = 1;
    }
    حرر(boxes);
}

فراغ fill_truth_region(حرف *رستہ, عائم *truth, صحيح classes, صحيح عدد_boxes, صحيح flip, عائم dx, عائم dy, عائم sx, عائم sy)
{
    حرف labelpath[4096];
    لبھو_replace(رستہ, "images", "labels", labelpath);
    لبھو_replace(labelpath, "JPEGImages", "labels", labelpath);

    لبھو_replace(labelpath, ".jpg", ".txt", labelpath);
    لبھو_replace(labelpath, ".png", ".txt", labelpath);
    لبھو_replace(labelpath, ".JPG", ".txt", labelpath);
    لبھو_replace(labelpath, ".JPEG", ".txt", labelpath);
    صحيح گنتی = 0;
    box_label *boxes = read_boxes(labelpath, &گنتی);
    randomize_boxes(boxes, گنتی);
    correct_boxes(boxes, گنتی, dx, dy, sx, sy, flip);
    عائم x,y,w,h;
    صحيح id;
    صحيح i;

    لكل (i = 0; i < گنتی; ++i) {
        x =  boxes[i].x;
        y =  boxes[i].y;
        w =  boxes[i].w;
        h =  boxes[i].h;
        id = boxes[i].id;

        إذا (w < .005 || h < .005) استمر;

        صحيح خانہ = (صحيح)(x*عدد_boxes);
        صحيح صف = (صحيح)(y*عدد_boxes);

        x = x*عدد_boxes - خانہ;
        y = y*عدد_boxes - صف;

        صحيح اشاریہ = (خانہ+صف*عدد_boxes)*(5+classes);
        إذا (truth[اشاریہ]) استمر;
        truth[اشاریہ++] = 1;

        إذا (id < classes) truth[اشاریہ+id] = 1;
        اشاریہ += classes;

        truth[اشاریہ++] = x;
        truth[اشاریہ++] = y;
        truth[اشاریہ++] = w;
        truth[اشاریہ++] = h;
    }
    حرر(boxes);
}

فراغ لدو_rle(image im, صحيح *rle, صحيح n)
{
    صحيح گنتی = 0;
    صحيح curr = 0;
    صحيح i,j;
    لكل(i = 0; i < n; ++i){
        لكل(j = 0; j < rle[i]; ++j){
            im.انکڑے[گنتی++] = curr;
        }
        curr = 1 - curr;
    }
    لكل(; گنتی < im.h*im.w*im.c; ++گنتی){
        im.انکڑے[گنتی] = curr;
    }
}

فراغ or_image(image src, image dest, صحيح c)
{
    صحيح i;
    لكل(i = 0; i < src.w*src.h; ++i){
        إذا(src.انکڑے[i]) dest.انکڑے[dest.w*dest.h*c + i] = 1;
    }
}

فراغ exclusive_image(image src)
{
    صحيح k, j, i;
    صحيح s = src.w*src.h;
    لكل(k = 0; k < src.c-1; ++k){
        لكل(i = 0; i < s; ++i){
            إذا (src.انکڑے[k*s + i]){
                لكل(j = k+1; j < src.c; ++j){
                    src.انکڑے[j*s + i] = 0;
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
            إذا(im.انکڑے[y*im.w + x]){
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

فراغ fill_truth_iseg(حرف *رستہ, صحيح عدد_boxes, عائم *truth, صحيح classes, صحيح w, صحيح h, augment_دلیلاں aug, صحيح flip, صحيح mw, صحيح mh)
{
    حرف labelpath[4096];
    لبھو_replace(رستہ, "images", "mask", labelpath);
    لبھو_replace(labelpath, "JPEGImages", "mask", labelpath);
    لبھو_replace(labelpath, ".jpg", ".txt", labelpath);
    لبھو_replace(labelpath, ".JPG", ".txt", labelpath);
    لبھو_replace(labelpath, ".JPEG", ".txt", labelpath);
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
        لدو_rle(part, rle, n);
        image sized = rotate_crop_image(part, aug.rad, aug.scale, aug.w, aug.h, aug.dx, aug.dy, aug.aspect);
        إذا(flip) flip_image(sized);

        image نقاب = resize_image(sized, mw, mh);
        truth[i*(mw*mh+1)] = id;
        لكل(j = 0; j < mw*mh; ++j){
            truth[i*(mw*mh + 1) + 1 + j] = نقاب.انکڑے[j];
        }
        ++i;

        free_image(نقاب);
        free_image(sized);
        حرر(rle);
    }
    إذا(i < عدد_boxes) truth[i*(mw*mh+1)] = -1;
    fclose(file);
    free_image(part);
}

فراغ fill_truth_نقاب(حرف *رستہ, صحيح عدد_boxes, عائم *truth, صحيح classes, صحيح w, صحيح h, augment_دلیلاں aug, صحيح flip, صحيح mw, صحيح mh)
{
    حرف labelpath[4096];
    لبھو_replace(رستہ, "images", "mask", labelpath);
    لبھو_replace(labelpath, "JPEGImages", "mask", labelpath);
    لبھو_replace(labelpath, ".jpg", ".txt", labelpath);
    لبھو_replace(labelpath, ".JPG", ".txt", labelpath);
    لبھو_replace(labelpath, ".JPEG", ".txt", labelpath);
    FILE *file = fopen(labelpath, "r");
    إذا(!file) file_error(labelpath);
    حرف buff[32788];
    صحيح id;
    صحيح i = 0;
    image part = make_image(w, h, 1);
    طالما((fscanf(file, "%d %s", &id, buff) == 2) && i < عدد_boxes){
        صحيح n = 0;
        صحيح *rle = read_intlist(buff, &n, 0);
        لدو_rle(part, rle, n);
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
                truth[i*(4 + mw*mh + 1) + 4 + j] = نقاب.انکڑے[j];
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


فراغ fill_truth_detection(حرف *رستہ, صحيح عدد_boxes, عائم *truth, صحيح classes, صحيح flip, عائم dx, عائم dy, عائم sx, عائم sy)
{
    حرف labelpath[4096];
    لبھو_replace(رستہ, "images", "labels", labelpath);
    لبھو_replace(labelpath, "JPEGImages", "labels", labelpath);

    لبھو_replace(labelpath, "raw", "labels", labelpath);
    لبھو_replace(labelpath, ".jpg", ".txt", labelpath);
    لبھو_replace(labelpath, ".png", ".txt", labelpath);
    لبھو_replace(labelpath, ".JPG", ".txt", labelpath);
    لبھو_replace(labelpath, ".JPEG", ".txt", labelpath);
    صحيح گنتی = 0;
    box_label *boxes = read_boxes(labelpath, &گنتی);
    randomize_boxes(boxes, گنتی);
    correct_boxes(boxes, گنتی, dx, dy, sx, sy, flip);
    إذا(گنتی > عدد_boxes) گنتی = عدد_boxes;
    عائم x,y,w,h;
    صحيح id;
    صحيح i;
    صحيح sub = 0;

    لكل (i = 0; i < گنتی; ++i) {
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

#تعریف_کرو NUMCHARS 37

فراغ print_letters(عائم *pred, صحيح n)
{
    صحيح i;
    لكل(i = 0; i < n; ++i){
        صحيح اشاریہ = max_اشاریہ(pred+i*NUMCHARS, NUMCHARS);
        لکھو("%c", int_منزل_alphanum(اشاریہ));
    }
    لکھو("\n");
}

فراغ fill_truth_captcha(حرف *رستہ, صحيح n, عائم *truth)
{
    حرف *begin = strrchr(رستہ, '/');
    ++begin;
    صحيح i;
    لكل(i = 0; i < لڑی_لمبائی(begin) && i < n && begin[i] != '.'; ++i){
        صحيح اشاریہ = alphanum_منزل_int(begin[i]);
        إذا(اشاریہ > 35) لکھو("Bad %c\n", begin[i]);
        truth[i*NUMCHARS+اشاریہ] = 1;
    }
    لكل(;i < n; ++i){
        truth[i*NUMCHARS + NUMCHARS-1] = 1;
    }
}

انکڑے لدو_انکڑے_captcha(حرف **paths, صحيح n, صحيح m, صحيح k, صحيح w, صحيح h)
{
    إذا(m) paths = get_random_paths(paths, n, m);
    انکڑے d = {0};
    d.shallow = 0;
    d.X = لدو_image_paths(paths, n, w, h);
    d.y = make_میٹرکس(n, k*NUMCHARS);
    صحيح i;
    لكل(i = 0; i < n; ++i){
        fill_truth_captcha(paths[i], k, d.y.vals[i]);
    }
    إذا(m) حرر(paths);
    أرجع d;
}

انکڑے لدو_انکڑے_captcha_encode(حرف **paths, صحيح n, صحيح m, صحيح w, صحيح h)
{
    إذا(m) paths = get_random_paths(paths, n, m);
    انکڑے d = {0};
    d.shallow = 0;
    d.X = لدو_image_paths(paths, n, w, h);
    d.X.خانے_گنتی = 17100;
    d.y = d.X;
    إذا(m) حرر(paths);
    أرجع d;
}

فراغ fill_truth(حرف *رستہ, حرف **labels, صحيح k, عائم *truth)
{
    صحيح i;
    memset(truth, 0, k*حجم(عائم));
    صحيح گنتی = 0;
    لكل(i = 0; i < k; ++i){
        إذا(لڑی_کھوج(رستہ, labels[i])){
            truth[i] = 1;
            ++گنتی;
            //printf("%s %s %d\n", path, labels[i], i);
        }
    }
    إذا(گنتی != 1 && (k != 1 || گنتی != 0)) لکھو("Too many or too few labels: %d, %s\n", گنتی, رستہ);
}

فراغ fill_hierarchy(عائم *truth, صحيح k, درخت *hierarchy)
{
    صحيح j;
    لكل(j = 0; j < k; ++j){
        إذا(truth[j]){
            صحيح ماپا = hierarchy->ماپا[j];
            طالما(ماپا >= 0){
                truth[ماپا] = 1;
                ماپا = hierarchy->ماپا[ماپا];
            }
        }
    }
    صحيح i;
    صحيح گنتی = 0;
    لكل(j = 0; j < hierarchy->groups; ++j){
        //printf("%d\n", count);
        صحيح نقاب = 1;
        لكل(i = 0; i < hierarchy->گروہ_ناپ[j]; ++i){
            إذا(truth[گنتی + i]){
                نقاب = 0;
                اكسر;
            }
        }
        إذا (نقاب) {
            لكل(i = 0; i < hierarchy->گروہ_ناپ[j]; ++i){
                truth[گنتی + i] = SECRET_عدد;
            }
        }
        گنتی += hierarchy->گروہ_ناپ[j];
    }
}

میٹرکس لدو_regression_labels_paths(حرف **paths, صحيح n, صحيح k)
{
    میٹرکس y = make_میٹرکس(n, k);
    صحيح i,j;
    لكل(i = 0; i < n; ++i){
        حرف labelpath[4096];
        لبھو_replace(paths[i], "images", "labels", labelpath);
        لبھو_replace(labelpath, "JPEGImages", "labels", labelpath);
        لبھو_replace(labelpath, ".BMP", ".txt", labelpath);
        لبھو_replace(labelpath, ".JPEG", ".txt", labelpath);
        لبھو_replace(labelpath, ".JPG", ".txt", labelpath);
        لبھو_replace(labelpath, ".JPeG", ".txt", labelpath);
        لبھو_replace(labelpath, ".Jpeg", ".txt", labelpath);
        لبھو_replace(labelpath, ".PNG", ".txt", labelpath);
        لبھو_replace(labelpath, ".TIF", ".txt", labelpath);
        لبھو_replace(labelpath, ".bmp", ".txt", labelpath);
        لبھو_replace(labelpath, ".jpeg", ".txt", labelpath);
        لبھو_replace(labelpath, ".jpg", ".txt", labelpath);
        لبھو_replace(labelpath, ".png", ".txt", labelpath);
        لبھو_replace(labelpath, ".tif", ".txt", labelpath);

        FILE *file = fopen(labelpath, "r");
        لكل(j = 0; j < k; ++j){
            fscanf(file, "%f", &(y.vals[i][j]));
        }
        fclose(file);
    }
    أرجع y;
}

میٹرکس لدو_labels_paths(حرف **paths, صحيح n, حرف **labels, صحيح k, درخت *hierarchy)
{
    میٹرکس y = make_میٹرکس(n, k);
    صحيح i;
    لكل(i = 0; i < n && labels; ++i){
        fill_truth(paths[i], labels, k, y.vals[i]);
        إذا(hierarchy){
            fill_hierarchy(y.vals[i], k, hierarchy);
        }
    }
    أرجع y;
}

میٹرکس لدو_ٹیگ_سوچی_paths(حرف **paths, صحيح n, صحيح k)
{
    میٹرکس y = make_میٹرکس(n, k);
    صحيح i;
    //int count = 0;
    لكل(i = 0; i < n; ++i){
        حرف label[4096];
        لبھو_replace(paths[i], "images", "labels", label);
        لبھو_replace(label, ".jpg", ".txt", label);
        FILE *file = fopen(label, "r");
        إذا (!file) استمر;
        //++count;
        صحيح ٹیگ;
        طالما(fscanf(file, "%d", &ٹیگ) == 1){
            إذا(ٹیگ < k){
                y.vals[i][ٹیگ] = 1;
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

فراغ free_انکڑے(انکڑے d)
{
    إذا(!d.shallow){
        free_میٹرکس(d.X);
        free_میٹرکس(d.y);
    }وإلا{
        حرر(d.X.vals);
        حرر(d.y.vals);
    }
}

image get_segmentation_image(حرف *رستہ, صحيح w, صحيح h, صحيح classes)
{
    حرف labelpath[4096];
    لبھو_replace(رستہ, "images", "mask", labelpath);
    لبھو_replace(labelpath, "JPEGImages", "mask", labelpath);
    لبھو_replace(labelpath, ".jpg", ".txt", labelpath);
    لبھو_replace(labelpath, ".JPG", ".txt", labelpath);
    لبھو_replace(labelpath, ".JPEG", ".txt", labelpath);
    image نقاب = make_image(w, h, classes);
    FILE *file = fopen(labelpath, "r");
    إذا(!file) file_error(labelpath);
    حرف buff[32788];
    صحيح id;
    image part = make_image(w, h, 1);
    طالما(fscanf(file, "%d %s", &id, buff) == 2){
        صحيح n = 0;
        صحيح *rle = read_intlist(buff, &n, 0);
        لدو_rle(part, rle, n);
        or_image(part, نقاب, id);
        حرر(rle);
    }
    //exclusive_image(mask);
    fclose(file);
    free_image(part);
    أرجع نقاب;
}

image get_segmentation_image2(حرف *رستہ, صحيح w, صحيح h, صحيح classes)
{
    حرف labelpath[4096];
    لبھو_replace(رستہ, "images", "mask", labelpath);
    لبھو_replace(labelpath, "JPEGImages", "mask", labelpath);
    لبھو_replace(labelpath, ".jpg", ".txt", labelpath);
    لبھو_replace(labelpath, ".JPG", ".txt", labelpath);
    لبھو_replace(labelpath, ".JPEG", ".txt", labelpath);
    image نقاب = make_image(w, h, classes+1);
    صحيح i;
    لكل(i = 0; i < w*h; ++i){
        نقاب.انکڑے[w*h*classes + i] = 1;
    }
    FILE *file = fopen(labelpath, "r");
    إذا(!file) file_error(labelpath);
    حرف buff[32788];
    صحيح id;
    image part = make_image(w, h, 1);
    طالما(fscanf(file, "%d %s", &id, buff) == 2){
        صحيح n = 0;
        صحيح *rle = read_intlist(buff, &n, 0);
        لدو_rle(part, rle, n);
        or_image(part, نقاب, id);
        لكل(i = 0; i < w*h; ++i){
            إذا(part.انکڑے[i]) نقاب.انکڑے[w*h*classes + i] = 0;
        }
        حرر(rle);
    }
    //exclusive_image(mask);
    fclose(file);
    free_image(part);
    أرجع نقاب;
}

انکڑے لدو_انکڑے_seg(صحيح n, حرف **paths, صحيح m, صحيح w, صحيح h, صحيح classes, صحيح min, صحيح max, عائم angle, عائم aspect, عائم hue, عائم saturation, عائم exposure, صحيح div)
{
    حرف **random_paths = get_random_paths(paths, n, m);
    صحيح i;
    انکڑے d = {0};
    d.shallow = 0;

    d.X.صفاں = n;
    d.X.vals = صفر_حافظہ_دیو(d.X.صفاں, حجم(عائم*));
    d.X.خانے_گنتی = h*w*3;


    d.y.صفاں = n;
    d.y.خانے_گنتی = h*w*classes/div/div;
    d.y.vals = صفر_حافظہ_دیو(d.X.صفاں, حجم(عائم*));

    لكل(i = 0; i < n; ++i){
        image orig = لدو_image_color(random_paths[i], 0, 0);
        augment_دلیلاں a = random_augment_دلیلاں(orig, angle, aspect, min, max, w, h);
        image sized = rotate_crop_image(orig, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);

        صحيح flip = rand()%2;
        إذا(flip) flip_image(sized);
        random_distort_image(sized, hue, saturation, exposure);
        d.X.vals[i] = sized.انکڑے;

        image نقاب = get_segmentation_image(random_paths[i], orig.w, orig.h, classes);
        //image mask = make_image(orig.w, orig.h, classes+1);
        image sized_m = rotate_crop_image(نقاب, a.rad, a.scale/div, a.w/div, a.h/div, a.dx/div, a.dy/div, a.aspect);

        إذا(flip) flip_image(sized_m);
        d.y.vals[i] = sized_m.انکڑے;

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

انکڑے لدو_انکڑے_iseg(صحيح n, حرف **paths, صحيح m, صحيح w, صحيح h, صحيح classes, صحيح boxes, صحيح div, صحيح min, صحيح max, عائم angle, عائم aspect, عائم hue, عائم saturation, عائم exposure)
{
    حرف **random_paths = get_random_paths(paths, n, m);
    صحيح i;
    انکڑے d = {0};
    d.shallow = 0;

    d.X.صفاں = n;
    d.X.vals = صفر_حافظہ_دیو(d.X.صفاں, حجم(عائم*));
    d.X.خانے_گنتی = h*w*3;

    d.y = make_میٹرکس(n, (((w/div)*(h/div))+1)*boxes);

    لكل(i = 0; i < n; ++i){
        image orig = لدو_image_color(random_paths[i], 0, 0);
        augment_دلیلاں a = random_augment_دلیلاں(orig, angle, aspect, min, max, w, h);
        image sized = rotate_crop_image(orig, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);

        صحيح flip = rand()%2;
        إذا(flip) flip_image(sized);
        random_distort_image(sized, hue, saturation, exposure);
        d.X.vals[i] = sized.انکڑے;
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

انکڑے لدو_انکڑے_نقاب(صحيح n, حرف **paths, صحيح m, صحيح w, صحيح h, صحيح classes, صحيح boxes, صحيح coords, صحيح min, صحيح max, عائم angle, عائم aspect, عائم hue, عائم saturation, عائم exposure)
{
    حرف **random_paths = get_random_paths(paths, n, m);
    صحيح i;
    انکڑے d = {0};
    d.shallow = 0;

    d.X.صفاں = n;
    d.X.vals = صفر_حافظہ_دیو(d.X.صفاں, حجم(عائم*));
    d.X.خانے_گنتی = h*w*3;

    d.y = make_میٹرکس(n, (coords+1)*boxes);

    لكل(i = 0; i < n; ++i){
        image orig = لدو_image_color(random_paths[i], 0, 0);
        augment_دلیلاں a = random_augment_دلیلاں(orig, angle, aspect, min, max, w, h);
        image sized = rotate_crop_image(orig, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);

        صحيح flip = rand()%2;
        إذا(flip) flip_image(sized);
        random_distort_image(sized, hue, saturation, exposure);
        d.X.vals[i] = sized.انکڑے;
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

انکڑے لدو_انکڑے_region(صحيح n, حرف **paths, صحيح m, صحيح w, صحيح h, صحيح ناپ, صحيح classes, عائم jitter, عائم hue, عائم saturation, عائم exposure)
{
    حرف **random_paths = get_random_paths(paths, n, m);
    صحيح i;
    انکڑے d = {0};
    d.shallow = 0;

    d.X.صفاں = n;
    d.X.vals = صفر_حافظہ_دیو(d.X.صفاں, حجم(عائم*));
    d.X.خانے_گنتی = h*w*3;


    صحيح k = ناپ*ناپ*(5+classes);
    d.y = make_میٹرکس(n, k);
    لكل(i = 0; i < n; ++i){
        image orig = لدو_image_color(random_paths[i], 0, 0);

        صحيح oh = orig.h;
        صحيح ow = orig.w;

        صحيح dw = (ow*jitter);
        صحيح dh = (oh*jitter);

        صحيح pleft  = rand_اکسار(-dw, dw);
        صحيح pright = rand_اکسار(-dw, dw);
        صحيح ptop   = rand_اکسار(-dh, dh);
        صحيح pbot   = rand_اکسار(-dh, dh);

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
        d.X.vals[i] = sized.انکڑے;

        fill_truth_region(random_paths[i], d.y.vals[i], classes, ناپ, flip, dx, dy, 1./sx, 1./sy);

        free_image(orig);
        free_image(cropped);
    }
    حرر(random_paths);
    أرجع d;
}

انکڑے لدو_انکڑے_compare(صحيح n, حرف **paths, صحيح m, صحيح classes, صحيح w, صحيح h)
{
    إذا(m) paths = get_random_paths(paths, 2*n, m);
    صحيح i,j;
    انکڑے d = {0};
    d.shallow = 0;

    d.X.صفاں = n;
    d.X.vals = صفر_حافظہ_دیو(d.X.صفاں, حجم(عائم*));
    d.X.خانے_گنتی = h*w*6;

    صحيح k = 2*(classes);
    d.y = make_میٹرکس(n, k);
    لكل(i = 0; i < n; ++i){
        image im1 = لدو_image_color(paths[i*2],   w, h);
        image im2 = لدو_image_color(paths[i*2+1], w, h);

        d.X.vals[i] = صفر_حافظہ_دیو(d.X.خانے_گنتی, حجم(عائم));
        حافظہ_نقل(d.X.vals[i],         im1.انکڑے, h*w*3*حجم(عائم));
        حافظہ_نقل(d.X.vals[i] + h*w*3, im2.انکڑے, h*w*3*حجم(عائم));

        صحيح id;
        عائم iou;

        حرف imlabel1[4096];
        حرف imlabel2[4096];
        لبھو_replace(paths[i*2],   "imgs", "labels", imlabel1);
        لبھو_replace(imlabel1, "jpg", "txt", imlabel1);
        FILE *fp1 = fopen(imlabel1, "r");

        طالما(fscanf(fp1, "%d %f", &id, &iou) == 2){
            إذا (d.y.vals[i][2*id] < iou) d.y.vals[i][2*id] = iou;
        }

        لبھو_replace(paths[i*2+1], "imgs", "labels", imlabel2);
        لبھو_replace(imlabel2, "jpg", "txt", imlabel2);
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

انکڑے لدو_انکڑے_swag(حرف **paths, صحيح n, صحيح classes, عائم jitter)
{
    صحيح اشاریہ = rand()%n;
    حرف *random_رستہ = paths[اشاریہ];

    image orig = لدو_image_color(random_رستہ, 0, 0);
    صحيح h = orig.h;
    صحيح w = orig.w;

    انکڑے d = {0};
    d.shallow = 0;
    d.w = w;
    d.h = h;

    d.X.صفاں = 1;
    d.X.vals = صفر_حافظہ_دیو(d.X.صفاں, حجم(عائم*));
    d.X.خانے_گنتی = h*w*3;

    صحيح k = (4+classes)*90;
    d.y = make_میٹرکس(1, k);

    صحيح dw = w*jitter;
    صحيح dh = h*jitter;

    صحيح pleft  = rand_اکسار(-dw, dw);
    صحيح pright = rand_اکسار(-dw, dw);
    صحيح ptop   = rand_اکسار(-dh, dh);
    صحيح pbot   = rand_اکسار(-dh, dh);

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
    d.X.vals[0] = sized.انکڑے;

    fill_truth_swag(random_رستہ, d.y.vals[0], classes, flip, dx, dy, 1./sx, 1./sy);

    free_image(orig);
    free_image(cropped);

    أرجع d;
}

انکڑے لدو_انکڑے_detection(صحيح n, حرف **paths, صحيح m, صحيح w, صحيح h, صحيح boxes, صحيح classes, عائم jitter, عائم hue, عائم saturation, عائم exposure)
{
    حرف **random_paths = get_random_paths(paths, n, m);
    صحيح i;
    انکڑے d = {0};
    d.shallow = 0;

    d.X.صفاں = n;
    d.X.vals = صفر_حافظہ_دیو(d.X.صفاں, حجم(عائم*));
    d.X.خانے_گنتی = h*w*3;

    d.y = make_میٹرکس(n, 5*boxes);
    لكل(i = 0; i < n; ++i){
        image orig = لدو_image_color(random_paths[i], 0, 0);
        image sized = make_image(w, h, orig.c);
        fill_image(sized, .5);

        عائم dw = jitter * orig.w;
        عائم dh = jitter * orig.h;

        عائم new_ar = (orig.w + rand_اکسار(-dw, dw)) / (orig.h + rand_اکسار(-dh, dh));
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

        عائم dx = rand_اکسار(0, w - nw);
        عائم dy = rand_اکسار(0, h - nh);

        رکھو_image(orig, nw, nh, dx, dy, sized);

        random_distort_image(sized, hue, saturation, exposure);

        صحيح flip = rand()%2;
        إذا(flip) flip_image(sized);
        d.X.vals[i] = sized.انکڑے;


        fill_truth_detection(random_paths[i], boxes, d.y.vals[i], classes, flip, -dx/w, -dy/h, nw/w, nh/h);

        free_image(orig);
    }
    حرر(random_paths);
    أرجع d;
}

فراغ *لدو_thread(فراغ *ptr)
{
    //printf("Loading data: %d\n", rand());
    لدو_دلیلاں a = *(بنية لدو_دلیلاں*)ptr;
    إذا(a.exposure == 0) a.exposure = 1;
    إذا(a.saturation == 0) a.saturation = 1;
    إذا(a.aspect == 0) a.aspect = 1;

    إذا (a.type == OLD_CLASSIFICATION_انکڑے){
        *a.d = لدو_انکڑے_old(a.paths, a.n, a.m, a.labels, a.classes, a.w, a.h);
    } وإلا إذا (a.type == REGRESSION_انکڑے){
        *a.d = لدو_انکڑے_regression(a.paths, a.n, a.m, a.classes, a.min, a.max, a.ناپ, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    } وإلا إذا (a.type == CLASSIFICATION_انکڑے){
        *a.d = لدو_انکڑے_augment(a.paths, a.n, a.m, a.labels, a.classes, a.hierarchy, a.min, a.max, a.ناپ, a.angle, a.aspect, a.hue, a.saturation, a.exposure, a.center);
    } وإلا إذا (a.type == SUPER_انکڑے){
        *a.d = لدو_انکڑے_super(a.paths, a.n, a.m, a.w, a.h, a.scale);
    } وإلا إذا (a.type == WRITING_انکڑے){
        *a.d = لدو_انکڑے_writing(a.paths, a.n, a.m, a.w, a.h, a.باہرلا_w, a.باہرلا_h);
    } وإلا إذا (a.type == ISEG_انکڑے){
        *a.d = لدو_انکڑے_iseg(a.n, a.paths, a.m, a.w, a.h, a.classes, a.عدد_boxes, a.scale, a.min, a.max, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    } وإلا إذا (a.type == INSTANCE_انکڑے){
        *a.d = لدو_انکڑے_نقاب(a.n, a.paths, a.m, a.w, a.h, a.classes, a.عدد_boxes, a.coords, a.min, a.max, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    } وإلا إذا (a.type == SEGMENTATION_انکڑے){
        *a.d = لدو_انکڑے_seg(a.n, a.paths, a.m, a.w, a.h, a.classes, a.min, a.max, a.angle, a.aspect, a.hue, a.saturation, a.exposure, a.scale);
    } وإلا إذا (a.type == REGION_انکڑے){
        *a.d = لدو_انکڑے_region(a.n, a.paths, a.m, a.w, a.h, a.عدد_boxes, a.classes, a.jitter, a.hue, a.saturation, a.exposure);
    } وإلا إذا (a.type == DETECTION_انکڑے){
        *a.d = لدو_انکڑے_detection(a.n, a.paths, a.m, a.w, a.h, a.عدد_boxes, a.classes, a.jitter, a.hue, a.saturation, a.exposure);
    } وإلا إذا (a.type == SWAG_انکڑے){
        *a.d = لدو_انکڑے_swag(a.paths, a.n, a.classes, a.jitter);
    } وإلا إذا (a.type == COMPARE_انکڑے){
        *a.d = لدو_انکڑے_compare(a.n, a.paths, a.m, a.classes, a.w, a.h);
    } وإلا إذا (a.type == IMAGE_انکڑے){
        *(a.im) = لدو_image_color(a.رستہ, 0, 0);
        *(a.resized) = resize_image(*(a.im), a.w, a.h);
    } وإلا إذا (a.type == LETTERBOX_انکڑے){
        *(a.im) = لدو_image_color(a.رستہ, 0, 0);
        *(a.resized) = letterbox_image(*(a.im), a.w, a.h);
    } وإلا إذا (a.type == ٹیگ_انکڑے){
        *a.d = لدو_انکڑے_ٹیگ(a.paths, a.n, a.m, a.classes, a.min, a.max, a.ناپ, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    }
    حرر(ptr);
    أرجع 0;
}

pthread_t لدو_انکڑے_وچ_thread(لدو_دلیلاں دلیلاں)
{
    pthread_t thread;
    بنية لدو_دلیلاں *ptr = صفر_حافظہ_دیو(1, حجم(بنية لدو_دلیلاں));
    *ptr = دلیلاں;
    إذا(pthread_create(&thread, 0, لدو_thread, ptr)) error("Thread creation failed");
    أرجع thread;
}

فراغ *لدو_دھاگے(فراغ *ptr)
{
    صحيح i;
    لدو_دلیلاں دلیلاں = *(لدو_دلیلاں *)ptr;
    إذا (دلیلاں.دھاگے == 0) دلیلاں.دھاگے = 1;
    انکڑے *باہرلا = دلیلاں.d;
    صحيح کل = دلیلاں.n;
    حرر(ptr);
    انکڑے *buffers = صفر_حافظہ_دیو(دلیلاں.دھاگے, حجم(انکڑے));
    pthread_t *دھاگے = صفر_حافظہ_دیو(دلیلاں.دھاگے, حجم(pthread_t));
    لكل(i = 0; i < دلیلاں.دھاگے; ++i){
        دلیلاں.d = buffers + i;
        دلیلاں.n = (i+1) * کل/دلیلاں.دھاگے - i * کل/دلیلاں.دھاگے;
        دھاگے[i] = لدو_انکڑے_وچ_thread(دلیلاں);
    }
    لكل(i = 0; i < دلیلاں.دھاگے; ++i){
        pthread_join(دھاگے[i], 0);
    }
    *باہرلا = concat_datas(buffers, دلیلاں.دھاگے);
    باہرلا->shallow = 0;
    لكل(i = 0; i < دلیلاں.دھاگے; ++i){
        buffers[i].shallow = 1;
        free_انکڑے(buffers[i]);
    }
    حرر(buffers);
    حرر(دھاگے);
    أرجع 0;
}

فراغ لدو_انکڑے_blocking(لدو_دلیلاں دلیلاں)
{
    بنية لدو_دلیلاں *ptr = صفر_حافظہ_دیو(1, حجم(بنية لدو_دلیلاں));
    *ptr = دلیلاں;
    لدو_thread(ptr);
}

pthread_t لدو_انکڑے(لدو_دلیلاں دلیلاں)
{
    pthread_t thread;
    بنية لدو_دلیلاں *ptr = صفر_حافظہ_دیو(1, حجم(بنية لدو_دلیلاں));
    *ptr = دلیلاں;
    إذا(pthread_create(&thread, 0, لدو_دھاگے, ptr)) error("Thread creation failed");
    أرجع thread;
}

انکڑے لدو_انکڑے_writing(حرف **paths, صحيح n, صحيح m, صحيح w, صحيح h, صحيح باہرلا_w, صحيح باہرلا_h)
{
    إذا(m) paths = get_random_paths(paths, n, m);
    حرف **replace_paths = لبھو_replace_paths(paths, n, ".png", "-label.png");
    انکڑے d = {0};
    d.shallow = 0;
    d.X = لدو_image_paths(paths, n, w, h);
    d.y = لدو_image_paths_سلیٹی(replace_paths, n, باہرلا_w, باہرلا_h);
    إذا(m) حرر(paths);
    صحيح i;
    لكل(i = 0; i < n; ++i) حرر(replace_paths[i]);
    حرر(replace_paths);
    أرجع d;
}

انکڑے لدو_انکڑے_old(حرف **paths, صحيح n, صحيح m, حرف **labels, صحيح k, صحيح w, صحيح h)
{
    إذا(m) paths = get_random_paths(paths, n, m);
    انکڑے d = {0};
    d.shallow = 0;
    d.X = لدو_image_paths(paths, n, w, h);
    d.y = لدو_labels_paths(paths, n, labels, k, 0);
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

انکڑے لدو_انکڑے_super(حرف **paths, صحيح n, صحيح m, صحيح w, صحيح h, صحيح scale)
{
    إذا(m) paths = get_random_paths(paths, n, m);
    انکڑے d = {0};
    d.shallow = 0;

    صحيح i;
    d.X.صفاں = n;
    d.X.vals = صفر_حافظہ_دیو(n, حجم(عائم*));
    d.X.خانے_گنتی = w*h*3;

    d.y.صفاں = n;
    d.y.vals = صفر_حافظہ_دیو(n, حجم(عائم*));
    d.y.خانے_گنتی = w*scale * h*scale * 3;

    لكل(i = 0; i < n; ++i){
        image im = لدو_image_color(paths[i], 0, 0);
        image crop = random_crop_image(im, w*scale, h*scale);
        صحيح flip = rand()%2;
        إذا (flip) flip_image(crop);
        image resize = resize_image(crop, w, h);
        d.X.vals[i] = resize.انکڑے;
        d.y.vals[i] = crop.انکڑے;
        free_image(im);
    }

    إذا(m) حرر(paths);
    أرجع d;
}

انکڑے لدو_انکڑے_regression(حرف **paths, صحيح n, صحيح m, صحيح k, صحيح min, صحيح max, صحيح ناپ, عائم angle, عائم aspect, عائم hue, عائم saturation, عائم exposure)
{
    إذا(m) paths = get_random_paths(paths, n, m);
    انکڑے d = {0};
    d.shallow = 0;
    d.X = لدو_image_augment_paths(paths, n, min, max, ناپ, angle, aspect, hue, saturation, exposure, 0);
    d.y = لدو_regression_labels_paths(paths, n, k);
    إذا(m) حرر(paths);
    أرجع d;
}

انکڑے select_انکڑے(انکڑے *orig, صحيح *inds)
{
    انکڑے d = {0};
    d.shallow = 1;
    d.w = orig[0].w;
    d.h = orig[0].h;

    d.X.صفاں = orig[0].X.صفاں;
    d.y.صفاں = orig[0].X.صفاں;

    d.X.خانے_گنتی = orig[0].X.خانے_گنتی;
    d.y.خانے_گنتی = orig[0].y.خانے_گنتی;

    d.X.vals = صفر_حافظہ_دیو(orig[0].X.صفاں, حجم(عائم *));
    d.y.vals = صفر_حافظہ_دیو(orig[0].y.صفاں, حجم(عائم *));
    صحيح i;
    لكل(i = 0; i < d.X.صفاں; ++i){
        d.X.vals[i] = orig[inds[i]].X.vals[i];
        d.y.vals[i] = orig[inds[i]].y.vals[i];
    }
    أرجع d;
}

انکڑے *tile_انکڑے(انکڑے orig, صحيح divs, صحيح ناپ)
{
    انکڑے *ds = صفر_حافظہ_دیو(divs*divs, حجم(انکڑے));
    صحيح i, j;
#ہدایت omp parallel لكل
    لكل(i = 0; i < divs*divs; ++i){
        انکڑے d;
        d.shallow = 0;
        d.w = orig.w/divs * ناپ;
        d.h = orig.h/divs * ناپ;
        d.X.صفاں = orig.X.صفاں;
        d.X.خانے_گنتی = d.w*d.h*3;
        d.X.vals = صفر_حافظہ_دیو(d.X.صفاں, حجم(عائم*));

        d.y = copy_میٹرکس(orig.y);
#ہدایت omp parallel لكل
        لكل(j = 0; j < orig.X.صفاں; ++j){
            صحيح x = (i%divs) * orig.w / divs - (d.w - orig.w/divs)/2;
            صحيح y = (i/divs) * orig.h / divs - (d.h - orig.h/divs)/2;
            image im = float_منزل_image(orig.w, orig.h, 3, orig.X.vals[j]);
            d.X.vals[j] = crop_image(im, x, y, d.w, d.h).انکڑے;
        }
        ds[i] = d;
    }
    أرجع ds;
}

انکڑے resize_انکڑے(انکڑے orig, صحيح w, صحيح h)
{
    انکڑے d = {0};
    d.shallow = 0;
    d.w = w;
    d.h = h;
    صحيح i;
    d.X.صفاں = orig.X.صفاں;
    d.X.خانے_گنتی = w*h*3;
    d.X.vals = صفر_حافظہ_دیو(d.X.صفاں, حجم(عائم*));

    d.y = copy_میٹرکس(orig.y);
#ہدایت omp parallel لكل
    لكل(i = 0; i < orig.X.صفاں; ++i){
        image im = float_منزل_image(orig.w, orig.h, 3, orig.X.vals[i]);
        d.X.vals[i] = resize_image(im, w, h).انکڑے;
    }
    أرجع d;
}

انکڑے لدو_انکڑے_augment(حرف **paths, صحيح n, صحيح m, حرف **labels, صحيح k, درخت *hierarchy, صحيح min, صحيح max, صحيح ناپ, عائم angle, عائم aspect, عائم hue, عائم saturation, عائم exposure, صحيح center)
{
    إذا(m) paths = get_random_paths(paths, n, m);
    انکڑے d = {0};
    d.shallow = 0;
    d.w=ناپ;
    d.h=ناپ;
    d.X = لدو_image_augment_paths(paths, n, min, max, ناپ, angle, aspect, hue, saturation, exposure, center);
    d.y = لدو_labels_paths(paths, n, labels, k, hierarchy);
    إذا(m) حرر(paths);
    أرجع d;
}

انکڑے لدو_انکڑے_ٹیگ(حرف **paths, صحيح n, صحيح m, صحيح k, صحيح min, صحيح max, صحيح ناپ, عائم angle, عائم aspect, عائم hue, عائم saturation, عائم exposure)
{
    إذا(m) paths = get_random_paths(paths, n, m);
    انکڑے d = {0};
    d.w = ناپ;
    d.h = ناپ;
    d.shallow = 0;
    d.X = لدو_image_augment_paths(paths, n, min, max, ناپ, angle, aspect, hue, saturation, exposure, 0);
    d.y = لدو_ٹیگ_سوچی_paths(paths, n, k);
    إذا(m) حرر(paths);
    أرجع d;
}

میٹرکس concat_میٹرکس(میٹرکس m1, میٹرکس m2)
{
    صحيح i, گنتی = 0;
    میٹرکس m;
    m.خانے_گنتی = m1.خانے_گنتی;
    m.صفاں = m1.صفاں+m2.صفاں;
    m.vals = صفر_حافظہ_دیو(m1.صفاں + m2.صفاں, حجم(عائم*));
    لكل(i = 0; i < m1.صفاں; ++i){
        m.vals[گنتی++] = m1.vals[i];
    }
    لكل(i = 0; i < m2.صفاں; ++i){
        m.vals[گنتی++] = m2.vals[i];
    }
    أرجع m;
}

انکڑے concat_انکڑے(انکڑے d1, انکڑے d2)
{
    انکڑے d = {0};
    d.shallow = 1;
    d.X = concat_میٹرکس(d1.X, d2.X);
    d.y = concat_میٹرکس(d1.y, d2.y);
    d.w = d1.w;
    d.h = d1.h;
    أرجع d;
}

انکڑے concat_datas(انکڑے *d, صحيح n)
{
    صحيح i;
    انکڑے باہرلا = {0};
    لكل(i = 0; i < n; ++i){
        انکڑے new = concat_انکڑے(d[i], باہرلا);
        free_انکڑے(باہرلا);
        باہرلا = new;
    }
    أرجع باہرلا;
}

انکڑے لدو_categorical_انکڑے_csv(حرف *filename, صحيح نشانہ, صحيح k)
{
    انکڑے d = {0};
    d.shallow = 0;
    میٹرکس X = csv_منزل_میٹرکس(filename);
    عائم *truth_1d = نکالو_کالم(&X, نشانہ);
    عائم **truth = one_hot_encode(truth_1d, X.صفاں, k);
    میٹرکس y;
    y.صفاں = X.صفاں;
    y.خانے_گنتی = k;
    y.vals = truth;
    d.X = X;
    d.y = y;
    حرر(truth_1d);
    أرجع d;
}

انکڑے لدو_cifar10_انکڑے(حرف *filename)
{
    انکڑے d = {0};
    d.shallow = 0;
    لما i,j;
    میٹرکس X = make_میٹرکس(10000, 3072);
    میٹرکس y = make_میٹرکس(10000, 10);
    d.X = X;
    d.y = y;

    FILE *fp = fopen(filename, "rb");
    إذا(!fp) file_error(filename);
    لكل(i = 0; i < 10000; ++i){
        بنانشانی حرف بائٹاں[3073];
        fread(بائٹاں, 1, 3073, fp);
        صحيح class = بائٹاں[0];
        y.vals[i][class] = 1;
        لكل(j = 0; j < X.خانے_گنتی; ++j){
            X.vals[i][j] = (مزدوج)بائٹاں[j+1];
        }
    }
    scale_انکڑے_صفاں(d, 1./255);
    //normalize_data_rows(d);
    fclose(fp);
    أرجع d;
}

فراغ get_random_batch(انکڑے d, صحيح n, عائم *X, عائم *y)
{
    صحيح j;
    لكل(j = 0; j < n; ++j){
        صحيح اشاریہ = rand()%d.X.صفاں;
        حافظہ_نقل(X+j*d.X.خانے_گنتی, d.X.vals[اشاریہ], d.X.خانے_گنتی*حجم(عائم));
        حافظہ_نقل(y+j*d.y.خانے_گنتی, d.y.vals[اشاریہ], d.y.خانے_گنتی*حجم(عائم));
    }
}

فراغ get_اگلا_batch(انکڑے d, صحيح n, صحيح وتھ, عائم *X, عائم *y)
{
    صحيح j;
    لكل(j = 0; j < n; ++j){
        صحيح اشاریہ = وتھ + j;
        حافظہ_نقل(X+j*d.X.خانے_گنتی, d.X.vals[اشاریہ], d.X.خانے_گنتی*حجم(عائم));
        إذا(y) حافظہ_نقل(y+j*d.y.خانے_گنتی, d.y.vals[اشاریہ], d.y.خانے_گنتی*حجم(عائم));
    }
}

فراغ smooth_انکڑے(انکڑے d)
{
    صحيح i, j;
    عائم scale = 1. / d.y.خانے_گنتی;
    عائم eps = .1;
    لكل(i = 0; i < d.y.صفاں; ++i){
        لكل(j = 0; j < d.y.خانے_گنتی; ++j){
            d.y.vals[i][j] = eps * scale + (1-eps) * d.y.vals[i][j];
        }
    }
}

انکڑے لدو_all_cifar10()
{
    انکڑے d = {0};
    d.shallow = 0;
    صحيح i,j,b;
    میٹرکس X = make_میٹرکس(50000, 3072);
    میٹرکس y = make_میٹرکس(50000, 10);
    d.X = X;
    d.y = y;


    لكل(b = 0; b < 5; ++b){
        حرف buff[256];
        sprintf(buff, "data/cifar/cifar-10-batches-bin/data_batch_%d.bin", b+1);
        FILE *fp = fopen(buff, "rb");
        إذا(!fp) file_error(buff);
        لكل(i = 0; i < 10000; ++i){
            بنانشانی حرف بائٹاں[3073];
            fread(بائٹاں, 1, 3073, fp);
            صحيح class = بائٹاں[0];
            y.vals[i+b*10000][class] = 1;
            لكل(j = 0; j < X.خانے_گنتی; ++j){
                X.vals[i+b*10000][j] = (مزدوج)بائٹاں[j+1];
            }
        }
        fclose(fp);
    }
    //normalize_data_rows(d);
    scale_انکڑے_صفاں(d, 1./255);
    smooth_انکڑے(d);
    أرجع d;
}

انکڑے لدو_go(حرف *filename)
{
    FILE *fp = fopen(filename, "rb");
    میٹرکس X = make_میٹرکس(3363059, 361);
    میٹرکس y = make_میٹرکس(3363059, 361);
    صحيح صف, خانہ;

    إذا(!fp) file_error(filename);
    حرف *label;
    صحيح گنتی = 0;
    طالما((label = fgetl(fp))){
        صحيح i;
        إذا(گنتی == X.صفاں){
            X = resize_میٹرکس(X, گنتی*2);
            y = resize_میٹرکس(y, گنتی*2);
        }
        لڑی_توں_پڑھو(label, "%d %d", &صف, &خانہ);
        حرف *board = fgetl(fp);

        صحيح اشاریہ = صف*19 + خانہ;
        y.vals[گنتی][اشاریہ] = 1;

        لكل(i = 0; i < 19*19; ++i){
            عائم val = 0;
            إذا(board[i] == '1') val = 1;
            وإلا إذا(board[i] == '2') val = -1;
            X.vals[گنتی][i] = val;
        }
        ++گنتی;
        حرر(label);
        حرر(board);
    }
    X = resize_میٹرکس(X, گنتی);
    y = resize_میٹرکس(y, گنتی);

    انکڑے d = {0};
    d.shallow = 0;
    d.X = X;
    d.y = y;


    fclose(fp);

    أرجع d;
}


فراغ randomize_انکڑے(انکڑے d)
{
    صحيح i;
    لكل(i = d.X.صفاں-1; i > 0; --i){
        صحيح اشاریہ = rand()%i;
        عائم *وٹاندرا = d.X.vals[اشاریہ];
        d.X.vals[اشاریہ] = d.X.vals[i];
        d.X.vals[i] = وٹاندرا;

        وٹاندرا = d.y.vals[اشاریہ];
        d.y.vals[اشاریہ] = d.y.vals[i];
        d.y.vals[i] = وٹاندرا;
    }
}

فراغ scale_انکڑے_صفاں(انکڑے d, عائم s)
{
    صحيح i;
    لكل(i = 0; i < d.X.صفاں; ++i){
        scale_array(d.X.vals[i], d.X.خانے_گنتی, s);
    }
}

فراغ translate_انکڑے_صفاں(انکڑے d, عائم s)
{
    صحيح i;
    لكل(i = 0; i < d.X.صفاں; ++i){
        translate_array(d.X.vals[i], d.X.خانے_گنتی, s);
    }
}

انکڑے copy_انکڑے(انکڑے d)
{
    انکڑے c = {0};
    c.w = d.w;
    c.h = d.h;
    c.shallow = 0;
    c.عدد_boxes = d.عدد_boxes;
    c.boxes = d.boxes;
    c.X = copy_میٹرکس(d.X);
    c.y = copy_میٹرکس(d.y);
    أرجع c;
}

فراغ normalize_انکڑے_صفاں(انکڑے d)
{
    صحيح i;
    لكل(i = 0; i < d.X.صفاں; ++i){
        normalize_array(d.X.vals[i], d.X.خانے_گنتی);
    }
}

انکڑے get_انکڑے_part(انکڑے d, صحيح part, صحيح کل)
{
    انکڑے p = {0};
    p.shallow = 1;
    p.X.صفاں = d.X.صفاں * (part + 1) / کل - d.X.صفاں * part / کل;
    p.y.صفاں = d.y.صفاں * (part + 1) / کل - d.y.صفاں * part / کل;
    p.X.خانے_گنتی = d.X.خانے_گنتی;
    p.y.خانے_گنتی = d.y.خانے_گنتی;
    p.X.vals = d.X.vals + d.X.صفاں * part / کل;
    p.y.vals = d.y.vals + d.y.صفاں * part / کل;
    أرجع p;
}

انکڑے get_random_انکڑے(انکڑے d, صحيح عدد)
{
    انکڑے r = {0};
    r.shallow = 1;

    r.X.صفاں = عدد;
    r.y.صفاں = عدد;

    r.X.خانے_گنتی = d.X.خانے_گنتی;
    r.y.خانے_گنتی = d.y.خانے_گنتی;

    r.X.vals = صفر_حافظہ_دیو(عدد, حجم(عائم *));
    r.y.vals = صفر_حافظہ_دیو(عدد, حجم(عائم *));

    صحيح i;
    لكل(i = 0; i < عدد; ++i){
        صحيح اشاریہ = rand()%d.X.صفاں;
        r.X.vals[i] = d.X.vals[اشاریہ];
        r.y.vals[i] = d.y.vals[اشاریہ];
    }
    أرجع r;
}

انکڑے *ونڈو_انکڑے(انکڑے d, صحيح part, صحيح کل)
{
    انکڑے *ونڈو = صفر_حافظہ_دیو(2, حجم(انکڑے));
    صحيح i;
    صحيح شروع = part*d.X.صفاں/کل;
    صحيح سرا = (part+1)*d.X.صفاں/کل;
    انکڑے train;
    انکڑے test;
    train.shallow = test.shallow = 1;

    test.X.صفاں = test.y.صفاں = سرا-شروع;
    train.X.صفاں = train.y.صفاں = d.X.صفاں - (سرا-شروع);
    train.X.خانے_گنتی = test.X.خانے_گنتی = d.X.خانے_گنتی;
    train.y.خانے_گنتی = test.y.خانے_گنتی = d.y.خانے_گنتی;

    train.X.vals = صفر_حافظہ_دیو(train.X.صفاں, حجم(عائم*));
    test.X.vals = صفر_حافظہ_دیو(test.X.صفاں, حجم(عائم*));
    train.y.vals = صفر_حافظہ_دیو(train.y.صفاں, حجم(عائم*));
    test.y.vals = صفر_حافظہ_دیو(test.y.صفاں, حجم(عائم*));

    لكل(i = 0; i < شروع; ++i){
        train.X.vals[i] = d.X.vals[i];
        train.y.vals[i] = d.y.vals[i];
    }
    لكل(i = شروع; i < سرا; ++i){
        test.X.vals[i-شروع] = d.X.vals[i];
        test.y.vals[i-شروع] = d.y.vals[i];
    }
    لكل(i = سرا; i < d.X.صفاں; ++i){
        train.X.vals[i-(سرا-شروع)] = d.X.vals[i];
        train.y.vals[i-(سرا-شروع)] = d.y.vals[i];
    }
    ونڈو[0] = train;
    ونڈو[1] = test;
    أرجع ونڈو;
}

