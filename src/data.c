#شامل "data.h"
#شامل "utils.h"
#شامل "image.h"
#شامل "cuda.h"

#شامل <stdio.h>
#شامل <stdlib.h>
#شامل <string.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

list *get_paths(حرف *filename)
{
    حرف *راستہ;
    FILE *file = fopen(filename, "r");
    اگر(!file) file_error(filename);
    list *سطریں = make_list();
    جب((راستہ=fgetl(file))){
        list_داخل_کرو(سطریں, راستہ);
    }
    fclose(file);
    واپس سطریں;
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

حرف **get_random_paths(حرف **paths, صحیح n, صحیح m)
{
    حرف **random_paths = صفر_حافظہ_دو(n, حجم(حرف*));
    صحیح i;
    pthread_mutex_lock(&mutex);
    برائے(i = 0; i < n; ++i){
        صحیح اشاریہ = rand()%m;
        random_paths[i] = paths[اشاریہ];
        //if(i == 0) printf("%s\n", paths[index]);
    }
    pthread_mutex_unlock(&mutex);
    واپس random_paths;
}

حرف **ڈھونڈو_replace_paths(حرف **paths, صحیح n, حرف *ڈھونڈو, حرف *replace)
{
    حرف **replace_paths = صفر_حافظہ_دو(n, حجم(حرف*));
    صحیح i;
    برائے(i = 0; i < n; ++i){
        حرف replaced[4096];
        ڈھونڈو_replace(paths[i], ڈھونڈو, replace, replaced);
        replace_paths[i] = copy_string(replaced);
    }
    واپس replace_paths;
}

میٹرکس بھرو_image_paths_سرمئی(حرف **paths, صحیح n, صحیح w, صحیح h)
{
    صحیح i;
    میٹرکس X;
    X.صفیں = n;
    X.vals = صفر_حافظہ_دو(X.صفیں, حجم(اعشاری*));
    X.ستون_تعداد = 0;

    برائے(i = 0; i < n; ++i){
        image im = بھرو_image(paths[i], w, h, 3);

        image سرمئی = grayscale_image(im);
        free_image(im);
        im = سرمئی;

        X.vals[i] = im.ڈیٹا;
        X.ستون_تعداد = im.h*im.w*im.c;
    }
    واپس X;
}

میٹرکس بھرو_image_paths(حرف **paths, صحیح n, صحیح w, صحیح h)
{
    صحیح i;
    میٹرکس X;
    X.صفیں = n;
    X.vals = صفر_حافظہ_دو(X.صفیں, حجم(اعشاری*));
    X.ستون_تعداد = 0;

    برائے(i = 0; i < n; ++i){
        image im = بھرو_image_color(paths[i], w, h);
        X.vals[i] = im.ڈیٹا;
        X.ستون_تعداد = im.h*im.w*im.c;
    }
    واپس X;
}

میٹرکس بھرو_image_augment_paths(حرف **paths, صحیح n, صحیح min, صحیح max, صحیح سائز, اعشاری angle, اعشاری aspect, اعشاری hue, اعشاری saturation, اعشاری exposure, صحیح center)
{
    صحیح i;
    میٹرکس X;
    X.صفیں = n;
    X.vals = صفر_حافظہ_دو(X.صفیں, حجم(اعشاری*));
    X.ستون_تعداد = 0;

    برائے(i = 0; i < n; ++i){
        image im = بھرو_image_color(paths[i], 0, 0);
        image crop;
        اگر(center){
            crop = center_crop_image(im, سائز, سائز);
        } ورنہ {
            crop = random_augment_image(im, angle, aspect, min, max, سائز, سائز);
        }
        صحیح flip = rand()%2;
        اگر (flip) flip_image(crop);
        random_distort_image(crop, hue, saturation, exposure);

        /*
        show_image(im, "orig");
        show_image(crop, "crop");
        cvWaitKey(0);
        */
        //grayscale_image_3c(crop);
        free_image(im);
        X.vals[i] = crop.ڈیٹا;
        X.ستون_تعداد = crop.h*crop.w*crop.c;
    }
    واپس X;
}


box_label *read_boxes(حرف *filename, صحیح *n)
{
    FILE *file = fopen(filename, "r");
    اگر(!file) file_error(filename);
    اعشاری x, y, h, w;
    صحیح id;
    صحیح گنتی = 0;
    صحیح سائز = 64;
    box_label *boxes = صفر_حافظہ_دو(سائز, حجم(box_label));
    جب(fscanf(file, "%d %f %f %f %f", &id, &x, &y, &w, &h) == 5){
        اگر(گنتی == سائز) {
            سائز = سائز * 2;
            boxes = realloc(boxes, سائز*حجم(box_label));
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
    واپس boxes;
}

خالی randomize_boxes(box_label *b, صحیح n)
{
    صحیح i;
    برائے(i = 0; i < n; ++i){
        box_label تبادلہ = b[i];
        صحیح اشاریہ = rand()%n;
        b[i] = b[اشاریہ];
        b[اشاریہ] = تبادلہ;
    }
}

خالی correct_boxes(box_label *boxes, صحیح n, اعشاری dx, اعشاری dy, اعشاری sx, اعشاری sy, صحیح flip)
{
    صحیح i;
    برائے(i = 0; i < n; ++i){
        اگر(boxes[i].x == 0 && boxes[i].y == 0) {
            boxes[i].x = 999999;
            boxes[i].y = 999999;
            boxes[i].w = 999999;
            boxes[i].h = 999999;
            جاری;
        }
        boxes[i].بایاں   = boxes[i].بایاں  * sx - dx;
        boxes[i].دایاں  = boxes[i].دایاں * sx - dx;
        boxes[i].چوٹی    = boxes[i].چوٹی   * sy - dy;
        boxes[i].bottom = boxes[i].bottom* sy - dy;

        اگر(flip){
            اعشاری تبادلہ = boxes[i].بایاں;
            boxes[i].بایاں = 1. - boxes[i].دایاں;
            boxes[i].دایاں = 1. - تبادلہ;
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

خالی fill_truth_swag(حرف *راستہ, اعشاری *truth, صحیح classes, صحیح flip, اعشاری dx, اعشاری dy, اعشاری sx, اعشاری sy)
{
    حرف labelpath[4096];
    ڈھونڈو_replace(راستہ, "images", "labels", labelpath);
    ڈھونڈو_replace(labelpath, "JPEGImages", "labels", labelpath);
    ڈھونڈو_replace(labelpath, ".jpg", ".txt", labelpath);
    ڈھونڈو_replace(labelpath, ".JPG", ".txt", labelpath);
    ڈھونڈو_replace(labelpath, ".JPEG", ".txt", labelpath);

    صحیح گنتی = 0;
    box_label *boxes = read_boxes(labelpath, &گنتی);
    randomize_boxes(boxes, گنتی);
    correct_boxes(boxes, گنتی, dx, dy, sx, sy, flip);
    اعشاری x,y,w,h;
    صحیح id;
    صحیح i;

    برائے (i = 0; i < گنتی && i < 90; ++i) {
        x =  boxes[i].x;
        y =  boxes[i].y;
        w =  boxes[i].w;
        h =  boxes[i].h;
        id = boxes[i].id;

        اگر (w < .0 || h < .0) جاری;

        صحیح اشاریہ = (4+classes) * i;

        truth[اشاریہ++] = x;
        truth[اشاریہ++] = y;
        truth[اشاریہ++] = w;
        truth[اشاریہ++] = h;

        اگر (id < classes) truth[اشاریہ+id] = 1;
    }
    آزاد(boxes);
}

خالی fill_truth_region(حرف *راستہ, اعشاری *truth, صحیح classes, صحیح عدد_boxes, صحیح flip, اعشاری dx, اعشاری dy, اعشاری sx, اعشاری sy)
{
    حرف labelpath[4096];
    ڈھونڈو_replace(راستہ, "images", "labels", labelpath);
    ڈھونڈو_replace(labelpath, "JPEGImages", "labels", labelpath);

    ڈھونڈو_replace(labelpath, ".jpg", ".txt", labelpath);
    ڈھونڈو_replace(labelpath, ".png", ".txt", labelpath);
    ڈھونڈو_replace(labelpath, ".JPG", ".txt", labelpath);
    ڈھونڈو_replace(labelpath, ".JPEG", ".txt", labelpath);
    صحیح گنتی = 0;
    box_label *boxes = read_boxes(labelpath, &گنتی);
    randomize_boxes(boxes, گنتی);
    correct_boxes(boxes, گنتی, dx, dy, sx, sy, flip);
    اعشاری x,y,w,h;
    صحیح id;
    صحیح i;

    برائے (i = 0; i < گنتی; ++i) {
        x =  boxes[i].x;
        y =  boxes[i].y;
        w =  boxes[i].w;
        h =  boxes[i].h;
        id = boxes[i].id;

        اگر (w < .005 || h < .005) جاری;

        صحیح ستون = (صحیح)(x*عدد_boxes);
        صحیح صف = (صحیح)(y*عدد_boxes);

        x = x*عدد_boxes - ستون;
        y = y*عدد_boxes - صف;

        صحیح اشاریہ = (ستون+صف*عدد_boxes)*(5+classes);
        اگر (truth[اشاریہ]) جاری;
        truth[اشاریہ++] = 1;

        اگر (id < classes) truth[اشاریہ+id] = 1;
        اشاریہ += classes;

        truth[اشاریہ++] = x;
        truth[اشاریہ++] = y;
        truth[اشاریہ++] = w;
        truth[اشاریہ++] = h;
    }
    آزاد(boxes);
}

خالی بھرو_rle(image im, صحیح *rle, صحیح n)
{
    صحیح گنتی = 0;
    صحیح curr = 0;
    صحیح i,j;
    برائے(i = 0; i < n; ++i){
        برائے(j = 0; j < rle[i]; ++j){
            im.ڈیٹا[گنتی++] = curr;
        }
        curr = 1 - curr;
    }
    برائے(; گنتی < im.h*im.w*im.c; ++گنتی){
        im.ڈیٹا[گنتی] = curr;
    }
}

خالی or_image(image src, image dest, صحیح c)
{
    صحیح i;
    برائے(i = 0; i < src.w*src.h; ++i){
        اگر(src.ڈیٹا[i]) dest.ڈیٹا[dest.w*dest.h*c + i] = 1;
    }
}

خالی exclusive_image(image src)
{
    صحیح k, j, i;
    صحیح s = src.w*src.h;
    برائے(k = 0; k < src.c-1; ++k){
        برائے(i = 0; i < s; ++i){
            اگر (src.ڈیٹا[k*s + i]){
                برائے(j = k+1; j < src.c; ++j){
                    src.ڈیٹا[j*s + i] = 0;
                }
            }
        }
    }
}

box bound_image(image im)
{
    صحیح x,y;
    صحیح minx = im.w;
    صحیح miny = im.h;
    صحیح maxx = 0;
    صحیح maxy = 0;
    برائے(y = 0; y < im.h; ++y){
        برائے(x = 0; x < im.w; ++x){
            اگر(im.ڈیٹا[y*im.w + x]){
                minx = (x < minx) ? x : minx;
                miny = (y < miny) ? y : miny;
                maxx = (x > maxx) ? x : maxx;
                maxy = (y > maxy) ? y : maxy;
            }
        }
    }
    box b = {minx, miny, maxx-minx + 1, maxy-miny + 1};
    //printf("%f %f %f %f\n", b.x, b.y, b.w, b.h);
    واپس b;
}

خالی fill_truth_iseg(حرف *راستہ, صحیح عدد_boxes, اعشاری *truth, صحیح classes, صحیح w, صحیح h, augment_دلائل aug, صحیح flip, صحیح mw, صحیح mh)
{
    حرف labelpath[4096];
    ڈھونڈو_replace(راستہ, "images", "mask", labelpath);
    ڈھونڈو_replace(labelpath, "JPEGImages", "mask", labelpath);
    ڈھونڈو_replace(labelpath, ".jpg", ".txt", labelpath);
    ڈھونڈو_replace(labelpath, ".JPG", ".txt", labelpath);
    ڈھونڈو_replace(labelpath, ".JPEG", ".txt", labelpath);
    FILE *file = fopen(labelpath, "r");
    اگر(!file) file_error(labelpath);
    حرف buff[32788];
    صحیح id;
    صحیح i = 0;
    صحیح j;
    image part = make_image(w, h, 1);
    جب((fscanf(file, "%d %s", &id, buff) == 2) && i < عدد_boxes){
        صحیح n = 0;
        صحیح *rle = read_intlist(buff, &n, 0);
        بھرو_rle(part, rle, n);
        image sized = rotate_crop_image(part, aug.rad, aug.scale, aug.w, aug.h, aug.dx, aug.dy, aug.aspect);
        اگر(flip) flip_image(sized);

        image نقاب = resize_image(sized, mw, mh);
        truth[i*(mw*mh+1)] = id;
        برائے(j = 0; j < mw*mh; ++j){
            truth[i*(mw*mh + 1) + 1 + j] = نقاب.ڈیٹا[j];
        }
        ++i;

        free_image(نقاب);
        free_image(sized);
        آزاد(rle);
    }
    اگر(i < عدد_boxes) truth[i*(mw*mh+1)] = -1;
    fclose(file);
    free_image(part);
}

خالی fill_truth_نقاب(حرف *راستہ, صحیح عدد_boxes, اعشاری *truth, صحیح classes, صحیح w, صحیح h, augment_دلائل aug, صحیح flip, صحیح mw, صحیح mh)
{
    حرف labelpath[4096];
    ڈھونڈو_replace(راستہ, "images", "mask", labelpath);
    ڈھونڈو_replace(labelpath, "JPEGImages", "mask", labelpath);
    ڈھونڈو_replace(labelpath, ".jpg", ".txt", labelpath);
    ڈھونڈو_replace(labelpath, ".JPG", ".txt", labelpath);
    ڈھونڈو_replace(labelpath, ".JPEG", ".txt", labelpath);
    FILE *file = fopen(labelpath, "r");
    اگر(!file) file_error(labelpath);
    حرف buff[32788];
    صحیح id;
    صحیح i = 0;
    image part = make_image(w, h, 1);
    جب((fscanf(file, "%d %s", &id, buff) == 2) && i < عدد_boxes){
        صحیح n = 0;
        صحیح *rle = read_intlist(buff, &n, 0);
        بھرو_rle(part, rle, n);
        image sized = rotate_crop_image(part, aug.rad, aug.scale, aug.w, aug.h, aug.dx, aug.dy, aug.aspect);
        اگر(flip) flip_image(sized);
        box b = bound_image(sized);
        اگر(b.w > 0){
            image crop = crop_image(sized, b.x, b.y, b.w, b.h);
            image نقاب = resize_image(crop, mw, mh);
            truth[i*(4 + mw*mh + 1) + 0] = (b.x + b.w/2.)/sized.w;
            truth[i*(4 + mw*mh + 1) + 1] = (b.y + b.h/2.)/sized.h;
            truth[i*(4 + mw*mh + 1) + 2] = b.w/sized.w;
            truth[i*(4 + mw*mh + 1) + 3] = b.h/sized.h;
            صحیح j;
            برائے(j = 0; j < mw*mh; ++j){
                truth[i*(4 + mw*mh + 1) + 4 + j] = نقاب.ڈیٹا[j];
            }
            truth[i*(4 + mw*mh + 1) + 4 + mw*mh] = id;
            free_image(crop);
            free_image(نقاب);
            ++i;
        }
        free_image(sized);
        آزاد(rle);
    }
    fclose(file);
    free_image(part);
}


خالی fill_truth_detection(حرف *راستہ, صحیح عدد_boxes, اعشاری *truth, صحیح classes, صحیح flip, اعشاری dx, اعشاری dy, اعشاری sx, اعشاری sy)
{
    حرف labelpath[4096];
    ڈھونڈو_replace(راستہ, "images", "labels", labelpath);
    ڈھونڈو_replace(labelpath, "JPEGImages", "labels", labelpath);

    ڈھونڈو_replace(labelpath, "raw", "labels", labelpath);
    ڈھونڈو_replace(labelpath, ".jpg", ".txt", labelpath);
    ڈھونڈو_replace(labelpath, ".png", ".txt", labelpath);
    ڈھونڈو_replace(labelpath, ".JPG", ".txt", labelpath);
    ڈھونڈو_replace(labelpath, ".JPEG", ".txt", labelpath);
    صحیح گنتی = 0;
    box_label *boxes = read_boxes(labelpath, &گنتی);
    randomize_boxes(boxes, گنتی);
    correct_boxes(boxes, گنتی, dx, dy, sx, sy, flip);
    اگر(گنتی > عدد_boxes) گنتی = عدد_boxes;
    اعشاری x,y,w,h;
    صحیح id;
    صحیح i;
    صحیح sub = 0;

    برائے (i = 0; i < گنتی; ++i) {
        x =  boxes[i].x;
        y =  boxes[i].y;
        w =  boxes[i].w;
        h =  boxes[i].h;
        id = boxes[i].id;

        اگر ((w < .001 || h < .001)) {
            ++sub;
            جاری;
        }

        truth[(i-sub)*5+0] = x;
        truth[(i-sub)*5+1] = y;
        truth[(i-sub)*5+2] = w;
        truth[(i-sub)*5+3] = h;
        truth[(i-sub)*5+4] = id;
    }
    آزاد(boxes);
}

#تعریف_کرو NUMCHARS 37

خالی print_letters(اعشاری *pred, صحیح n)
{
    صحیح i;
    برائے(i = 0; i < n; ++i){
        صحیح اشاریہ = max_اشاریہ(pred+i*NUMCHARS, NUMCHARS);
        لکھو("%c", int_مقصد_alphanum(اشاریہ));
    }
    لکھو("\n");
}

خالی fill_truth_captcha(حرف *راستہ, صحیح n, اعشاری *truth)
{
    حرف *begin = strrchr(راستہ, '/');
    ++begin;
    صحیح i;
    برائے(i = 0; i < لڑی_لمبائی(begin) && i < n && begin[i] != '.'; ++i){
        صحیح اشاریہ = alphanum_مقصد_int(begin[i]);
        اگر(اشاریہ > 35) لکھو("Bad %c\n", begin[i]);
        truth[i*NUMCHARS+اشاریہ] = 1;
    }
    برائے(;i < n; ++i){
        truth[i*NUMCHARS + NUMCHARS-1] = 1;
    }
}

ڈیٹا بھرو_ڈیٹا_captcha(حرف **paths, صحیح n, صحیح m, صحیح k, صحیح w, صحیح h)
{
    اگر(m) paths = get_random_paths(paths, n, m);
    ڈیٹا d = {0};
    d.shallow = 0;
    d.X = بھرو_image_paths(paths, n, w, h);
    d.y = make_میٹرکس(n, k*NUMCHARS);
    صحیح i;
    برائے(i = 0; i < n; ++i){
        fill_truth_captcha(paths[i], k, d.y.vals[i]);
    }
    اگر(m) آزاد(paths);
    واپس d;
}

ڈیٹا بھرو_ڈیٹا_captcha_encode(حرف **paths, صحیح n, صحیح m, صحیح w, صحیح h)
{
    اگر(m) paths = get_random_paths(paths, n, m);
    ڈیٹا d = {0};
    d.shallow = 0;
    d.X = بھرو_image_paths(paths, n, w, h);
    d.X.ستون_تعداد = 17100;
    d.y = d.X;
    اگر(m) آزاد(paths);
    واپس d;
}

خالی fill_truth(حرف *راستہ, حرف **labels, صحیح k, اعشاری *truth)
{
    صحیح i;
    memset(truth, 0, k*حجم(اعشاری));
    صحیح گنتی = 0;
    برائے(i = 0; i < k; ++i){
        اگر(لڑی_تلاش(راستہ, labels[i])){
            truth[i] = 1;
            ++گنتی;
            //printf("%s %s %d\n", path, labels[i], i);
        }
    }
    اگر(گنتی != 1 && (k != 1 || گنتی != 0)) لکھو("Too many or too few labels: %d, %s\n", گنتی, راستہ);
}

خالی fill_hierarchy(اعشاری *truth, صحیح k, درخت *hierarchy)
{
    صحیح j;
    برائے(j = 0; j < k; ++j){
        اگر(truth[j]){
            صحیح والد = hierarchy->والد[j];
            جب(والد >= 0){
                truth[والد] = 1;
                والد = hierarchy->والد[والد];
            }
        }
    }
    صحیح i;
    صحیح گنتی = 0;
    برائے(j = 0; j < hierarchy->groups; ++j){
        //printf("%d\n", count);
        صحیح نقاب = 1;
        برائے(i = 0; i < hierarchy->گروہ_سائز[j]; ++i){
            اگر(truth[گنتی + i]){
                نقاب = 0;
                توڑو;
            }
        }
        اگر (نقاب) {
            برائے(i = 0; i < hierarchy->گروہ_سائز[j]; ++i){
                truth[گنتی + i] = SECRET_عدد;
            }
        }
        گنتی += hierarchy->گروہ_سائز[j];
    }
}

میٹرکس بھرو_regression_labels_paths(حرف **paths, صحیح n, صحیح k)
{
    میٹرکس y = make_میٹرکس(n, k);
    صحیح i,j;
    برائے(i = 0; i < n; ++i){
        حرف labelpath[4096];
        ڈھونڈو_replace(paths[i], "images", "labels", labelpath);
        ڈھونڈو_replace(labelpath, "JPEGImages", "labels", labelpath);
        ڈھونڈو_replace(labelpath, ".BMP", ".txt", labelpath);
        ڈھونڈو_replace(labelpath, ".JPEG", ".txt", labelpath);
        ڈھونڈو_replace(labelpath, ".JPG", ".txt", labelpath);
        ڈھونڈو_replace(labelpath, ".JPeG", ".txt", labelpath);
        ڈھونڈو_replace(labelpath, ".Jpeg", ".txt", labelpath);
        ڈھونڈو_replace(labelpath, ".PNG", ".txt", labelpath);
        ڈھونڈو_replace(labelpath, ".TIF", ".txt", labelpath);
        ڈھونڈو_replace(labelpath, ".bmp", ".txt", labelpath);
        ڈھونڈو_replace(labelpath, ".jpeg", ".txt", labelpath);
        ڈھونڈو_replace(labelpath, ".jpg", ".txt", labelpath);
        ڈھونڈو_replace(labelpath, ".png", ".txt", labelpath);
        ڈھونڈو_replace(labelpath, ".tif", ".txt", labelpath);

        FILE *file = fopen(labelpath, "r");
        برائے(j = 0; j < k; ++j){
            fscanf(file, "%f", &(y.vals[i][j]));
        }
        fclose(file);
    }
    واپس y;
}

میٹرکس بھرو_labels_paths(حرف **paths, صحیح n, حرف **labels, صحیح k, درخت *hierarchy)
{
    میٹرکس y = make_میٹرکس(n, k);
    صحیح i;
    برائے(i = 0; i < n && labels; ++i){
        fill_truth(paths[i], labels, k, y.vals[i]);
        اگر(hierarchy){
            fill_hierarchy(y.vals[i], k, hierarchy);
        }
    }
    واپس y;
}

میٹرکس بھرو_لیبل_فہرست_paths(حرف **paths, صحیح n, صحیح k)
{
    میٹرکس y = make_میٹرکس(n, k);
    صحیح i;
    //int count = 0;
    برائے(i = 0; i < n; ++i){
        حرف label[4096];
        ڈھونڈو_replace(paths[i], "images", "labels", label);
        ڈھونڈو_replace(label, ".jpg", ".txt", label);
        FILE *file = fopen(label, "r");
        اگر (!file) جاری;
        //++count;
        صحیح لیبل;
        جب(fscanf(file, "%d", &لیبل) == 1){
            اگر(لیبل < k){
                y.vals[i][لیبل] = 1;
            }
        }
        fclose(file);
    }
    //printf("%d/%d\n", count, n);
    واپس y;
}

حرف **get_labels(حرف *filename)
{
    list *plist = get_paths(filename);
    حرف **labels = (حرف **)list_مقصد_array(plist);
    free_list(plist);
    واپس labels;
}

خالی free_ڈیٹا(ڈیٹا d)
{
    اگر(!d.shallow){
        free_میٹرکس(d.X);
        free_میٹرکس(d.y);
    }ورنہ{
        آزاد(d.X.vals);
        آزاد(d.y.vals);
    }
}

image get_segmentation_image(حرف *راستہ, صحیح w, صحیح h, صحیح classes)
{
    حرف labelpath[4096];
    ڈھونڈو_replace(راستہ, "images", "mask", labelpath);
    ڈھونڈو_replace(labelpath, "JPEGImages", "mask", labelpath);
    ڈھونڈو_replace(labelpath, ".jpg", ".txt", labelpath);
    ڈھونڈو_replace(labelpath, ".JPG", ".txt", labelpath);
    ڈھونڈو_replace(labelpath, ".JPEG", ".txt", labelpath);
    image نقاب = make_image(w, h, classes);
    FILE *file = fopen(labelpath, "r");
    اگر(!file) file_error(labelpath);
    حرف buff[32788];
    صحیح id;
    image part = make_image(w, h, 1);
    جب(fscanf(file, "%d %s", &id, buff) == 2){
        صحیح n = 0;
        صحیح *rle = read_intlist(buff, &n, 0);
        بھرو_rle(part, rle, n);
        or_image(part, نقاب, id);
        آزاد(rle);
    }
    //exclusive_image(mask);
    fclose(file);
    free_image(part);
    واپس نقاب;
}

image get_segmentation_image2(حرف *راستہ, صحیح w, صحیح h, صحیح classes)
{
    حرف labelpath[4096];
    ڈھونڈو_replace(راستہ, "images", "mask", labelpath);
    ڈھونڈو_replace(labelpath, "JPEGImages", "mask", labelpath);
    ڈھونڈو_replace(labelpath, ".jpg", ".txt", labelpath);
    ڈھونڈو_replace(labelpath, ".JPG", ".txt", labelpath);
    ڈھونڈو_replace(labelpath, ".JPEG", ".txt", labelpath);
    image نقاب = make_image(w, h, classes+1);
    صحیح i;
    برائے(i = 0; i < w*h; ++i){
        نقاب.ڈیٹا[w*h*classes + i] = 1;
    }
    FILE *file = fopen(labelpath, "r");
    اگر(!file) file_error(labelpath);
    حرف buff[32788];
    صحیح id;
    image part = make_image(w, h, 1);
    جب(fscanf(file, "%d %s", &id, buff) == 2){
        صحیح n = 0;
        صحیح *rle = read_intlist(buff, &n, 0);
        بھرو_rle(part, rle, n);
        or_image(part, نقاب, id);
        برائے(i = 0; i < w*h; ++i){
            اگر(part.ڈیٹا[i]) نقاب.ڈیٹا[w*h*classes + i] = 0;
        }
        آزاد(rle);
    }
    //exclusive_image(mask);
    fclose(file);
    free_image(part);
    واپس نقاب;
}

ڈیٹا بھرو_ڈیٹا_seg(صحیح n, حرف **paths, صحیح m, صحیح w, صحیح h, صحیح classes, صحیح min, صحیح max, اعشاری angle, اعشاری aspect, اعشاری hue, اعشاری saturation, اعشاری exposure, صحیح div)
{
    حرف **random_paths = get_random_paths(paths, n, m);
    صحیح i;
    ڈیٹا d = {0};
    d.shallow = 0;

    d.X.صفیں = n;
    d.X.vals = صفر_حافظہ_دو(d.X.صفیں, حجم(اعشاری*));
    d.X.ستون_تعداد = h*w*3;


    d.y.صفیں = n;
    d.y.ستون_تعداد = h*w*classes/div/div;
    d.y.vals = صفر_حافظہ_دو(d.X.صفیں, حجم(اعشاری*));

    برائے(i = 0; i < n; ++i){
        image orig = بھرو_image_color(random_paths[i], 0, 0);
        augment_دلائل a = random_augment_دلائل(orig, angle, aspect, min, max, w, h);
        image sized = rotate_crop_image(orig, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);

        صحیح flip = rand()%2;
        اگر(flip) flip_image(sized);
        random_distort_image(sized, hue, saturation, exposure);
        d.X.vals[i] = sized.ڈیٹا;

        image نقاب = get_segmentation_image(random_paths[i], orig.w, orig.h, classes);
        //image mask = make_image(orig.w, orig.h, classes+1);
        image sized_m = rotate_crop_image(نقاب, a.rad, a.scale/div, a.w/div, a.h/div, a.dx/div, a.dy/div, a.aspect);

        اگر(flip) flip_image(sized_m);
        d.y.vals[i] = sized_m.ڈیٹا;

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
    آزاد(random_paths);
    واپس d;
}

ڈیٹا بھرو_ڈیٹا_iseg(صحیح n, حرف **paths, صحیح m, صحیح w, صحیح h, صحیح classes, صحیح boxes, صحیح div, صحیح min, صحیح max, اعشاری angle, اعشاری aspect, اعشاری hue, اعشاری saturation, اعشاری exposure)
{
    حرف **random_paths = get_random_paths(paths, n, m);
    صحیح i;
    ڈیٹا d = {0};
    d.shallow = 0;

    d.X.صفیں = n;
    d.X.vals = صفر_حافظہ_دو(d.X.صفیں, حجم(اعشاری*));
    d.X.ستون_تعداد = h*w*3;

    d.y = make_میٹرکس(n, (((w/div)*(h/div))+1)*boxes);

    برائے(i = 0; i < n; ++i){
        image orig = بھرو_image_color(random_paths[i], 0, 0);
        augment_دلائل a = random_augment_دلائل(orig, angle, aspect, min, max, w, h);
        image sized = rotate_crop_image(orig, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);

        صحیح flip = rand()%2;
        اگر(flip) flip_image(sized);
        random_distort_image(sized, hue, saturation, exposure);
        d.X.vals[i] = sized.ڈیٹا;
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
    آزاد(random_paths);
    واپس d;
}

ڈیٹا بھرو_ڈیٹا_نقاب(صحیح n, حرف **paths, صحیح m, صحیح w, صحیح h, صحیح classes, صحیح boxes, صحیح coords, صحیح min, صحیح max, اعشاری angle, اعشاری aspect, اعشاری hue, اعشاری saturation, اعشاری exposure)
{
    حرف **random_paths = get_random_paths(paths, n, m);
    صحیح i;
    ڈیٹا d = {0};
    d.shallow = 0;

    d.X.صفیں = n;
    d.X.vals = صفر_حافظہ_دو(d.X.صفیں, حجم(اعشاری*));
    d.X.ستون_تعداد = h*w*3;

    d.y = make_میٹرکس(n, (coords+1)*boxes);

    برائے(i = 0; i < n; ++i){
        image orig = بھرو_image_color(random_paths[i], 0, 0);
        augment_دلائل a = random_augment_دلائل(orig, angle, aspect, min, max, w, h);
        image sized = rotate_crop_image(orig, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);

        صحیح flip = rand()%2;
        اگر(flip) flip_image(sized);
        random_distort_image(sized, hue, saturation, exposure);
        d.X.vals[i] = sized.ڈیٹا;
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
    آزاد(random_paths);
    واپس d;
}

ڈیٹا بھرو_ڈیٹا_region(صحیح n, حرف **paths, صحیح m, صحیح w, صحیح h, صحیح سائز, صحیح classes, اعشاری jitter, اعشاری hue, اعشاری saturation, اعشاری exposure)
{
    حرف **random_paths = get_random_paths(paths, n, m);
    صحیح i;
    ڈیٹا d = {0};
    d.shallow = 0;

    d.X.صفیں = n;
    d.X.vals = صفر_حافظہ_دو(d.X.صفیں, حجم(اعشاری*));
    d.X.ستون_تعداد = h*w*3;


    صحیح k = سائز*سائز*(5+classes);
    d.y = make_میٹرکس(n, k);
    برائے(i = 0; i < n; ++i){
        image orig = بھرو_image_color(random_paths[i], 0, 0);

        صحیح oh = orig.h;
        صحیح ow = orig.w;

        صحیح dw = (ow*jitter);
        صحیح dh = (oh*jitter);

        صحیح pleft  = rand_یکساں(-dw, dw);
        صحیح pright = rand_یکساں(-dw, dw);
        صحیح ptop   = rand_یکساں(-dh, dh);
        صحیح pbot   = rand_یکساں(-dh, dh);

        صحیح swidth =  ow - pleft - pright;
        صحیح sheight = oh - ptop - pbot;

        اعشاری sx = (اعشاری)swidth  / ow;
        اعشاری sy = (اعشاری)sheight / oh;

        صحیح flip = rand()%2;
        image cropped = crop_image(orig, pleft, ptop, swidth, sheight);

        اعشاری dx = ((اعشاری)pleft/ow)/sx;
        اعشاری dy = ((اعشاری)ptop /oh)/sy;

        image sized = resize_image(cropped, w, h);
        اگر(flip) flip_image(sized);
        random_distort_image(sized, hue, saturation, exposure);
        d.X.vals[i] = sized.ڈیٹا;

        fill_truth_region(random_paths[i], d.y.vals[i], classes, سائز, flip, dx, dy, 1./sx, 1./sy);

        free_image(orig);
        free_image(cropped);
    }
    آزاد(random_paths);
    واپس d;
}

ڈیٹا بھرو_ڈیٹا_compare(صحیح n, حرف **paths, صحیح m, صحیح classes, صحیح w, صحیح h)
{
    اگر(m) paths = get_random_paths(paths, 2*n, m);
    صحیح i,j;
    ڈیٹا d = {0};
    d.shallow = 0;

    d.X.صفیں = n;
    d.X.vals = صفر_حافظہ_دو(d.X.صفیں, حجم(اعشاری*));
    d.X.ستون_تعداد = h*w*6;

    صحیح k = 2*(classes);
    d.y = make_میٹرکس(n, k);
    برائے(i = 0; i < n; ++i){
        image im1 = بھرو_image_color(paths[i*2],   w, h);
        image im2 = بھرو_image_color(paths[i*2+1], w, h);

        d.X.vals[i] = صفر_حافظہ_دو(d.X.ستون_تعداد, حجم(اعشاری));
        حافظہ_نقل(d.X.vals[i],         im1.ڈیٹا, h*w*3*حجم(اعشاری));
        حافظہ_نقل(d.X.vals[i] + h*w*3, im2.ڈیٹا, h*w*3*حجم(اعشاری));

        صحیح id;
        اعشاری iou;

        حرف imlabel1[4096];
        حرف imlabel2[4096];
        ڈھونڈو_replace(paths[i*2],   "imgs", "labels", imlabel1);
        ڈھونڈو_replace(imlabel1, "jpg", "txt", imlabel1);
        FILE *fp1 = fopen(imlabel1, "r");

        جب(fscanf(fp1, "%d %f", &id, &iou) == 2){
            اگر (d.y.vals[i][2*id] < iou) d.y.vals[i][2*id] = iou;
        }

        ڈھونڈو_replace(paths[i*2+1], "imgs", "labels", imlabel2);
        ڈھونڈو_replace(imlabel2, "jpg", "txt", imlabel2);
        FILE *fp2 = fopen(imlabel2, "r");

        جب(fscanf(fp2, "%d %f", &id, &iou) == 2){
            اگر (d.y.vals[i][2*id + 1] < iou) d.y.vals[i][2*id + 1] = iou;
        }

        برائے (j = 0; j < classes; ++j){
            اگر (d.y.vals[i][2*j] > .5 &&  d.y.vals[i][2*j+1] < .5){
                d.y.vals[i][2*j] = 1;
                d.y.vals[i][2*j+1] = 0;
            } ورنہ اگر (d.y.vals[i][2*j] < .5 &&  d.y.vals[i][2*j+1] > .5){
                d.y.vals[i][2*j] = 0;
                d.y.vals[i][2*j+1] = 1;
            } ورنہ {
                d.y.vals[i][2*j]   = SECRET_عدد;
                d.y.vals[i][2*j+1] = SECRET_عدد;
            }
        }
        fclose(fp1);
        fclose(fp2);

        free_image(im1);
        free_image(im2);
    }
    اگر(m) آزاد(paths);
    واپس d;
}

ڈیٹا بھرو_ڈیٹا_swag(حرف **paths, صحیح n, صحیح classes, اعشاری jitter)
{
    صحیح اشاریہ = rand()%n;
    حرف *random_راستہ = paths[اشاریہ];

    image orig = بھرو_image_color(random_راستہ, 0, 0);
    صحیح h = orig.h;
    صحیح w = orig.w;

    ڈیٹا d = {0};
    d.shallow = 0;
    d.w = w;
    d.h = h;

    d.X.صفیں = 1;
    d.X.vals = صفر_حافظہ_دو(d.X.صفیں, حجم(اعشاری*));
    d.X.ستون_تعداد = h*w*3;

    صحیح k = (4+classes)*90;
    d.y = make_میٹرکس(1, k);

    صحیح dw = w*jitter;
    صحیح dh = h*jitter;

    صحیح pleft  = rand_یکساں(-dw, dw);
    صحیح pright = rand_یکساں(-dw, dw);
    صحیح ptop   = rand_یکساں(-dh, dh);
    صحیح pbot   = rand_یکساں(-dh, dh);

    صحیح swidth =  w - pleft - pright;
    صحیح sheight = h - ptop - pbot;

    اعشاری sx = (اعشاری)swidth  / w;
    اعشاری sy = (اعشاری)sheight / h;

    صحیح flip = rand()%2;
    image cropped = crop_image(orig, pleft, ptop, swidth, sheight);

    اعشاری dx = ((اعشاری)pleft/w)/sx;
    اعشاری dy = ((اعشاری)ptop /h)/sy;

    image sized = resize_image(cropped, w, h);
    اگر(flip) flip_image(sized);
    d.X.vals[0] = sized.ڈیٹا;

    fill_truth_swag(random_راستہ, d.y.vals[0], classes, flip, dx, dy, 1./sx, 1./sy);

    free_image(orig);
    free_image(cropped);

    واپس d;
}

ڈیٹا بھرو_ڈیٹا_detection(صحیح n, حرف **paths, صحیح m, صحیح w, صحیح h, صحیح boxes, صحیح classes, اعشاری jitter, اعشاری hue, اعشاری saturation, اعشاری exposure)
{
    حرف **random_paths = get_random_paths(paths, n, m);
    صحیح i;
    ڈیٹا d = {0};
    d.shallow = 0;

    d.X.صفیں = n;
    d.X.vals = صفر_حافظہ_دو(d.X.صفیں, حجم(اعشاری*));
    d.X.ستون_تعداد = h*w*3;

    d.y = make_میٹرکس(n, 5*boxes);
    برائے(i = 0; i < n; ++i){
        image orig = بھرو_image_color(random_paths[i], 0, 0);
        image sized = make_image(w, h, orig.c);
        fill_image(sized, .5);

        اعشاری dw = jitter * orig.w;
        اعشاری dh = jitter * orig.h;

        اعشاری new_ar = (orig.w + rand_یکساں(-dw, dw)) / (orig.h + rand_یکساں(-dh, dh));
        //float scale = rand_uniform(.25, 2);
        اعشاری scale = 1;

        اعشاری nw, nh;

        اگر(new_ar < 1){
            nh = scale * h;
            nw = nh * new_ar;
        } ورنہ {
            nw = scale * w;
            nh = nw / new_ar;
        }

        اعشاری dx = rand_یکساں(0, w - nw);
        اعشاری dy = rand_یکساں(0, h - nh);

        رکھو_image(orig, nw, nh, dx, dy, sized);

        random_distort_image(sized, hue, saturation, exposure);

        صحیح flip = rand()%2;
        اگر(flip) flip_image(sized);
        d.X.vals[i] = sized.ڈیٹا;


        fill_truth_detection(random_paths[i], boxes, d.y.vals[i], classes, flip, -dx/w, -dy/h, nw/w, nh/h);

        free_image(orig);
    }
    آزاد(random_paths);
    واپس d;
}

خالی *بھرو_thread(خالی *ptr)
{
    //printf("Loading data: %d\n", rand());
    بھرو_دلائل a = *(ساخت بھرو_دلائل*)ptr;
    اگر(a.exposure == 0) a.exposure = 1;
    اگر(a.saturation == 0) a.saturation = 1;
    اگر(a.aspect == 0) a.aspect = 1;

    اگر (a.type == OLD_CLASSIFICATION_ڈیٹا){
        *a.d = بھرو_ڈیٹا_old(a.paths, a.n, a.m, a.labels, a.classes, a.w, a.h);
    } ورنہ اگر (a.type == REGRESSION_ڈیٹا){
        *a.d = بھرو_ڈیٹا_regression(a.paths, a.n, a.m, a.classes, a.min, a.max, a.سائز, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    } ورنہ اگر (a.type == CLASSIFICATION_ڈیٹا){
        *a.d = بھرو_ڈیٹا_augment(a.paths, a.n, a.m, a.labels, a.classes, a.hierarchy, a.min, a.max, a.سائز, a.angle, a.aspect, a.hue, a.saturation, a.exposure, a.center);
    } ورنہ اگر (a.type == SUPER_ڈیٹا){
        *a.d = بھرو_ڈیٹا_super(a.paths, a.n, a.m, a.w, a.h, a.scale);
    } ورنہ اگر (a.type == WRITING_ڈیٹا){
        *a.d = بھرو_ڈیٹا_writing(a.paths, a.n, a.m, a.w, a.h, a.برآمد_w, a.برآمد_h);
    } ورنہ اگر (a.type == ISEG_ڈیٹا){
        *a.d = بھرو_ڈیٹا_iseg(a.n, a.paths, a.m, a.w, a.h, a.classes, a.عدد_boxes, a.scale, a.min, a.max, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    } ورنہ اگر (a.type == INSTANCE_ڈیٹا){
        *a.d = بھرو_ڈیٹا_نقاب(a.n, a.paths, a.m, a.w, a.h, a.classes, a.عدد_boxes, a.coords, a.min, a.max, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    } ورنہ اگر (a.type == SEGMENTATION_ڈیٹا){
        *a.d = بھرو_ڈیٹا_seg(a.n, a.paths, a.m, a.w, a.h, a.classes, a.min, a.max, a.angle, a.aspect, a.hue, a.saturation, a.exposure, a.scale);
    } ورنہ اگر (a.type == REGION_ڈیٹا){
        *a.d = بھرو_ڈیٹا_region(a.n, a.paths, a.m, a.w, a.h, a.عدد_boxes, a.classes, a.jitter, a.hue, a.saturation, a.exposure);
    } ورنہ اگر (a.type == DETECTION_ڈیٹا){
        *a.d = بھرو_ڈیٹا_detection(a.n, a.paths, a.m, a.w, a.h, a.عدد_boxes, a.classes, a.jitter, a.hue, a.saturation, a.exposure);
    } ورنہ اگر (a.type == SWAG_ڈیٹا){
        *a.d = بھرو_ڈیٹا_swag(a.paths, a.n, a.classes, a.jitter);
    } ورنہ اگر (a.type == COMPARE_ڈیٹا){
        *a.d = بھرو_ڈیٹا_compare(a.n, a.paths, a.m, a.classes, a.w, a.h);
    } ورنہ اگر (a.type == IMAGE_ڈیٹا){
        *(a.im) = بھرو_image_color(a.راستہ, 0, 0);
        *(a.resized) = resize_image(*(a.im), a.w, a.h);
    } ورنہ اگر (a.type == LETTERBOX_ڈیٹا){
        *(a.im) = بھرو_image_color(a.راستہ, 0, 0);
        *(a.resized) = letterbox_image(*(a.im), a.w, a.h);
    } ورنہ اگر (a.type == لیبل_ڈیٹا){
        *a.d = بھرو_ڈیٹا_لیبل(a.paths, a.n, a.m, a.classes, a.min, a.max, a.سائز, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    }
    آزاد(ptr);
    واپس 0;
}

pthread_t بھرو_ڈیٹا_اندر_thread(بھرو_دلائل دلائل)
{
    pthread_t thread;
    ساخت بھرو_دلائل *ptr = صفر_حافظہ_دو(1, حجم(ساخت بھرو_دلائل));
    *ptr = دلائل;
    اگر(pthread_create(&thread, 0, بھرو_thread, ptr)) error("Thread creation failed");
    واپس thread;
}

خالی *بھرو_دھاگے(خالی *ptr)
{
    صحیح i;
    بھرو_دلائل دلائل = *(بھرو_دلائل *)ptr;
    اگر (دلائل.دھاگے == 0) دلائل.دھاگے = 1;
    ڈیٹا *برآمد = دلائل.d;
    صحیح کل = دلائل.n;
    آزاد(ptr);
    ڈیٹا *buffers = صفر_حافظہ_دو(دلائل.دھاگے, حجم(ڈیٹا));
    pthread_t *دھاگے = صفر_حافظہ_دو(دلائل.دھاگے, حجم(pthread_t));
    برائے(i = 0; i < دلائل.دھاگے; ++i){
        دلائل.d = buffers + i;
        دلائل.n = (i+1) * کل/دلائل.دھاگے - i * کل/دلائل.دھاگے;
        دھاگے[i] = بھرو_ڈیٹا_اندر_thread(دلائل);
    }
    برائے(i = 0; i < دلائل.دھاگے; ++i){
        pthread_join(دھاگے[i], 0);
    }
    *برآمد = concat_datas(buffers, دلائل.دھاگے);
    برآمد->shallow = 0;
    برائے(i = 0; i < دلائل.دھاگے; ++i){
        buffers[i].shallow = 1;
        free_ڈیٹا(buffers[i]);
    }
    آزاد(buffers);
    آزاد(دھاگے);
    واپس 0;
}

خالی بھرو_ڈیٹا_blocking(بھرو_دلائل دلائل)
{
    ساخت بھرو_دلائل *ptr = صفر_حافظہ_دو(1, حجم(ساخت بھرو_دلائل));
    *ptr = دلائل;
    بھرو_thread(ptr);
}

pthread_t بھرو_ڈیٹا(بھرو_دلائل دلائل)
{
    pthread_t thread;
    ساخت بھرو_دلائل *ptr = صفر_حافظہ_دو(1, حجم(ساخت بھرو_دلائل));
    *ptr = دلائل;
    اگر(pthread_create(&thread, 0, بھرو_دھاگے, ptr)) error("Thread creation failed");
    واپس thread;
}

ڈیٹا بھرو_ڈیٹا_writing(حرف **paths, صحیح n, صحیح m, صحیح w, صحیح h, صحیح برآمد_w, صحیح برآمد_h)
{
    اگر(m) paths = get_random_paths(paths, n, m);
    حرف **replace_paths = ڈھونڈو_replace_paths(paths, n, ".png", "-label.png");
    ڈیٹا d = {0};
    d.shallow = 0;
    d.X = بھرو_image_paths(paths, n, w, h);
    d.y = بھرو_image_paths_سرمئی(replace_paths, n, برآمد_w, برآمد_h);
    اگر(m) آزاد(paths);
    صحیح i;
    برائے(i = 0; i < n; ++i) آزاد(replace_paths[i]);
    آزاد(replace_paths);
    واپس d;
}

ڈیٹا بھرو_ڈیٹا_old(حرف **paths, صحیح n, صحیح m, حرف **labels, صحیح k, صحیح w, صحیح h)
{
    اگر(m) paths = get_random_paths(paths, n, m);
    ڈیٹا d = {0};
    d.shallow = 0;
    d.X = بھرو_image_paths(paths, n, w, h);
    d.y = بھرو_labels_paths(paths, n, labels, k, 0);
    اگر(m) آزاد(paths);
    واپس d;
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

ڈیٹا بھرو_ڈیٹا_super(حرف **paths, صحیح n, صحیح m, صحیح w, صحیح h, صحیح scale)
{
    اگر(m) paths = get_random_paths(paths, n, m);
    ڈیٹا d = {0};
    d.shallow = 0;

    صحیح i;
    d.X.صفیں = n;
    d.X.vals = صفر_حافظہ_دو(n, حجم(اعشاری*));
    d.X.ستون_تعداد = w*h*3;

    d.y.صفیں = n;
    d.y.vals = صفر_حافظہ_دو(n, حجم(اعشاری*));
    d.y.ستون_تعداد = w*scale * h*scale * 3;

    برائے(i = 0; i < n; ++i){
        image im = بھرو_image_color(paths[i], 0, 0);
        image crop = random_crop_image(im, w*scale, h*scale);
        صحیح flip = rand()%2;
        اگر (flip) flip_image(crop);
        image resize = resize_image(crop, w, h);
        d.X.vals[i] = resize.ڈیٹا;
        d.y.vals[i] = crop.ڈیٹا;
        free_image(im);
    }

    اگر(m) آزاد(paths);
    واپس d;
}

ڈیٹا بھرو_ڈیٹا_regression(حرف **paths, صحیح n, صحیح m, صحیح k, صحیح min, صحیح max, صحیح سائز, اعشاری angle, اعشاری aspect, اعشاری hue, اعشاری saturation, اعشاری exposure)
{
    اگر(m) paths = get_random_paths(paths, n, m);
    ڈیٹا d = {0};
    d.shallow = 0;
    d.X = بھرو_image_augment_paths(paths, n, min, max, سائز, angle, aspect, hue, saturation, exposure, 0);
    d.y = بھرو_regression_labels_paths(paths, n, k);
    اگر(m) آزاد(paths);
    واپس d;
}

ڈیٹا select_ڈیٹا(ڈیٹا *orig, صحیح *inds)
{
    ڈیٹا d = {0};
    d.shallow = 1;
    d.w = orig[0].w;
    d.h = orig[0].h;

    d.X.صفیں = orig[0].X.صفیں;
    d.y.صفیں = orig[0].X.صفیں;

    d.X.ستون_تعداد = orig[0].X.ستون_تعداد;
    d.y.ستون_تعداد = orig[0].y.ستون_تعداد;

    d.X.vals = صفر_حافظہ_دو(orig[0].X.صفیں, حجم(اعشاری *));
    d.y.vals = صفر_حافظہ_دو(orig[0].y.صفیں, حجم(اعشاری *));
    صحیح i;
    برائے(i = 0; i < d.X.صفیں; ++i){
        d.X.vals[i] = orig[inds[i]].X.vals[i];
        d.y.vals[i] = orig[inds[i]].y.vals[i];
    }
    واپس d;
}

ڈیٹا *tile_ڈیٹا(ڈیٹا orig, صحیح divs, صحیح سائز)
{
    ڈیٹا *ds = صفر_حافظہ_دو(divs*divs, حجم(ڈیٹا));
    صحیح i, j;
#ہدایت omp parallel برائے
    برائے(i = 0; i < divs*divs; ++i){
        ڈیٹا d;
        d.shallow = 0;
        d.w = orig.w/divs * سائز;
        d.h = orig.h/divs * سائز;
        d.X.صفیں = orig.X.صفیں;
        d.X.ستون_تعداد = d.w*d.h*3;
        d.X.vals = صفر_حافظہ_دو(d.X.صفیں, حجم(اعشاری*));

        d.y = copy_میٹرکس(orig.y);
#ہدایت omp parallel برائے
        برائے(j = 0; j < orig.X.صفیں; ++j){
            صحیح x = (i%divs) * orig.w / divs - (d.w - orig.w/divs)/2;
            صحیح y = (i/divs) * orig.h / divs - (d.h - orig.h/divs)/2;
            image im = float_مقصد_image(orig.w, orig.h, 3, orig.X.vals[j]);
            d.X.vals[j] = crop_image(im, x, y, d.w, d.h).ڈیٹا;
        }
        ds[i] = d;
    }
    واپس ds;
}

ڈیٹا resize_ڈیٹا(ڈیٹا orig, صحیح w, صحیح h)
{
    ڈیٹا d = {0};
    d.shallow = 0;
    d.w = w;
    d.h = h;
    صحیح i;
    d.X.صفیں = orig.X.صفیں;
    d.X.ستون_تعداد = w*h*3;
    d.X.vals = صفر_حافظہ_دو(d.X.صفیں, حجم(اعشاری*));

    d.y = copy_میٹرکس(orig.y);
#ہدایت omp parallel برائے
    برائے(i = 0; i < orig.X.صفیں; ++i){
        image im = float_مقصد_image(orig.w, orig.h, 3, orig.X.vals[i]);
        d.X.vals[i] = resize_image(im, w, h).ڈیٹا;
    }
    واپس d;
}

ڈیٹا بھرو_ڈیٹا_augment(حرف **paths, صحیح n, صحیح m, حرف **labels, صحیح k, درخت *hierarchy, صحیح min, صحیح max, صحیح سائز, اعشاری angle, اعشاری aspect, اعشاری hue, اعشاری saturation, اعشاری exposure, صحیح center)
{
    اگر(m) paths = get_random_paths(paths, n, m);
    ڈیٹا d = {0};
    d.shallow = 0;
    d.w=سائز;
    d.h=سائز;
    d.X = بھرو_image_augment_paths(paths, n, min, max, سائز, angle, aspect, hue, saturation, exposure, center);
    d.y = بھرو_labels_paths(paths, n, labels, k, hierarchy);
    اگر(m) آزاد(paths);
    واپس d;
}

ڈیٹا بھرو_ڈیٹا_لیبل(حرف **paths, صحیح n, صحیح m, صحیح k, صحیح min, صحیح max, صحیح سائز, اعشاری angle, اعشاری aspect, اعشاری hue, اعشاری saturation, اعشاری exposure)
{
    اگر(m) paths = get_random_paths(paths, n, m);
    ڈیٹا d = {0};
    d.w = سائز;
    d.h = سائز;
    d.shallow = 0;
    d.X = بھرو_image_augment_paths(paths, n, min, max, سائز, angle, aspect, hue, saturation, exposure, 0);
    d.y = بھرو_لیبل_فہرست_paths(paths, n, k);
    اگر(m) آزاد(paths);
    واپس d;
}

میٹرکس concat_میٹرکس(میٹرکس m1, میٹرکس m2)
{
    صحیح i, گنتی = 0;
    میٹرکس m;
    m.ستون_تعداد = m1.ستون_تعداد;
    m.صفیں = m1.صفیں+m2.صفیں;
    m.vals = صفر_حافظہ_دو(m1.صفیں + m2.صفیں, حجم(اعشاری*));
    برائے(i = 0; i < m1.صفیں; ++i){
        m.vals[گنتی++] = m1.vals[i];
    }
    برائے(i = 0; i < m2.صفیں; ++i){
        m.vals[گنتی++] = m2.vals[i];
    }
    واپس m;
}

ڈیٹا concat_ڈیٹا(ڈیٹا d1, ڈیٹا d2)
{
    ڈیٹا d = {0};
    d.shallow = 1;
    d.X = concat_میٹرکس(d1.X, d2.X);
    d.y = concat_میٹرکس(d1.y, d2.y);
    d.w = d1.w;
    d.h = d1.h;
    واپس d;
}

ڈیٹا concat_datas(ڈیٹا *d, صحیح n)
{
    صحیح i;
    ڈیٹا برآمد = {0};
    برائے(i = 0; i < n; ++i){
        ڈیٹا new = concat_ڈیٹا(d[i], برآمد);
        free_ڈیٹا(برآمد);
        برآمد = new;
    }
    واپس برآمد;
}

ڈیٹا بھرو_categorical_ڈیٹا_csv(حرف *filename, صحیح ہدف, صحیح k)
{
    ڈیٹا d = {0};
    d.shallow = 0;
    میٹرکس X = csv_مقصد_میٹرکس(filename);
    اعشاری *truth_1d = نکالو_عمود(&X, ہدف);
    اعشاری **truth = one_hot_encode(truth_1d, X.صفیں, k);
    میٹرکس y;
    y.صفیں = X.صفیں;
    y.ستون_تعداد = k;
    y.vals = truth;
    d.X = X;
    d.y = y;
    آزاد(truth_1d);
    واپس d;
}

ڈیٹا بھرو_cifar10_ڈیٹا(حرف *filename)
{
    ڈیٹا d = {0};
    d.shallow = 0;
    طویل i,j;
    میٹرکس X = make_میٹرکس(10000, 3072);
    میٹرکس y = make_میٹرکس(10000, 10);
    d.X = X;
    d.y = y;

    FILE *fp = fopen(filename, "rb");
    اگر(!fp) file_error(filename);
    برائے(i = 0; i < 10000; ++i){
        غیرعلامتی حرف بائٹیں[3073];
        fread(بائٹیں, 1, 3073, fp);
        صحیح class = بائٹیں[0];
        y.vals[i][class] = 1;
        برائے(j = 0; j < X.ستون_تعداد; ++j){
            X.vals[i][j] = (دوہرا)بائٹیں[j+1];
        }
    }
    scale_ڈیٹا_صفیں(d, 1./255);
    //normalize_data_rows(d);
    fclose(fp);
    واپس d;
}

خالی get_random_batch(ڈیٹا d, صحیح n, اعشاری *X, اعشاری *y)
{
    صحیح j;
    برائے(j = 0; j < n; ++j){
        صحیح اشاریہ = rand()%d.X.صفیں;
        حافظہ_نقل(X+j*d.X.ستون_تعداد, d.X.vals[اشاریہ], d.X.ستون_تعداد*حجم(اعشاری));
        حافظہ_نقل(y+j*d.y.ستون_تعداد, d.y.vals[اشاریہ], d.y.ستون_تعداد*حجم(اعشاری));
    }
}

خالی get_اگلا_batch(ڈیٹا d, صحیح n, صحیح انحراف, اعشاری *X, اعشاری *y)
{
    صحیح j;
    برائے(j = 0; j < n; ++j){
        صحیح اشاریہ = انحراف + j;
        حافظہ_نقل(X+j*d.X.ستون_تعداد, d.X.vals[اشاریہ], d.X.ستون_تعداد*حجم(اعشاری));
        اگر(y) حافظہ_نقل(y+j*d.y.ستون_تعداد, d.y.vals[اشاریہ], d.y.ستون_تعداد*حجم(اعشاری));
    }
}

خالی smooth_ڈیٹا(ڈیٹا d)
{
    صحیح i, j;
    اعشاری scale = 1. / d.y.ستون_تعداد;
    اعشاری eps = .1;
    برائے(i = 0; i < d.y.صفیں; ++i){
        برائے(j = 0; j < d.y.ستون_تعداد; ++j){
            d.y.vals[i][j] = eps * scale + (1-eps) * d.y.vals[i][j];
        }
    }
}

ڈیٹا بھرو_all_cifar10()
{
    ڈیٹا d = {0};
    d.shallow = 0;
    صحیح i,j,b;
    میٹرکس X = make_میٹرکس(50000, 3072);
    میٹرکس y = make_میٹرکس(50000, 10);
    d.X = X;
    d.y = y;


    برائے(b = 0; b < 5; ++b){
        حرف buff[256];
        sprintf(buff, "data/cifar/cifar-10-batches-bin/data_batch_%d.bin", b+1);
        FILE *fp = fopen(buff, "rb");
        اگر(!fp) file_error(buff);
        برائے(i = 0; i < 10000; ++i){
            غیرعلامتی حرف بائٹیں[3073];
            fread(بائٹیں, 1, 3073, fp);
            صحیح class = بائٹیں[0];
            y.vals[i+b*10000][class] = 1;
            برائے(j = 0; j < X.ستون_تعداد; ++j){
                X.vals[i+b*10000][j] = (دوہرا)بائٹیں[j+1];
            }
        }
        fclose(fp);
    }
    //normalize_data_rows(d);
    scale_ڈیٹا_صفیں(d, 1./255);
    smooth_ڈیٹا(d);
    واپس d;
}

ڈیٹا بھرو_go(حرف *filename)
{
    FILE *fp = fopen(filename, "rb");
    میٹرکس X = make_میٹرکس(3363059, 361);
    میٹرکس y = make_میٹرکس(3363059, 361);
    صحیح صف, ستون;

    اگر(!fp) file_error(filename);
    حرف *label;
    صحیح گنتی = 0;
    جب((label = fgetl(fp))){
        صحیح i;
        اگر(گنتی == X.صفیں){
            X = resize_میٹرکس(X, گنتی*2);
            y = resize_میٹرکس(y, گنتی*2);
        }
        لڑی_سے_پڑھو(label, "%d %d", &صف, &ستون);
        حرف *board = fgetl(fp);

        صحیح اشاریہ = صف*19 + ستون;
        y.vals[گنتی][اشاریہ] = 1;

        برائے(i = 0; i < 19*19; ++i){
            اعشاری val = 0;
            اگر(board[i] == '1') val = 1;
            ورنہ اگر(board[i] == '2') val = -1;
            X.vals[گنتی][i] = val;
        }
        ++گنتی;
        آزاد(label);
        آزاد(board);
    }
    X = resize_میٹرکس(X, گنتی);
    y = resize_میٹرکس(y, گنتی);

    ڈیٹا d = {0};
    d.shallow = 0;
    d.X = X;
    d.y = y;


    fclose(fp);

    واپس d;
}


خالی randomize_ڈیٹا(ڈیٹا d)
{
    صحیح i;
    برائے(i = d.X.صفیں-1; i > 0; --i){
        صحیح اشاریہ = rand()%i;
        اعشاری *تبادلہ = d.X.vals[اشاریہ];
        d.X.vals[اشاریہ] = d.X.vals[i];
        d.X.vals[i] = تبادلہ;

        تبادلہ = d.y.vals[اشاریہ];
        d.y.vals[اشاریہ] = d.y.vals[i];
        d.y.vals[i] = تبادلہ;
    }
}

خالی scale_ڈیٹا_صفیں(ڈیٹا d, اعشاری s)
{
    صحیح i;
    برائے(i = 0; i < d.X.صفیں; ++i){
        scale_array(d.X.vals[i], d.X.ستون_تعداد, s);
    }
}

خالی translate_ڈیٹا_صفیں(ڈیٹا d, اعشاری s)
{
    صحیح i;
    برائے(i = 0; i < d.X.صفیں; ++i){
        translate_array(d.X.vals[i], d.X.ستون_تعداد, s);
    }
}

ڈیٹا copy_ڈیٹا(ڈیٹا d)
{
    ڈیٹا c = {0};
    c.w = d.w;
    c.h = d.h;
    c.shallow = 0;
    c.عدد_boxes = d.عدد_boxes;
    c.boxes = d.boxes;
    c.X = copy_میٹرکس(d.X);
    c.y = copy_میٹرکس(d.y);
    واپس c;
}

خالی normalize_ڈیٹا_صفیں(ڈیٹا d)
{
    صحیح i;
    برائے(i = 0; i < d.X.صفیں; ++i){
        normalize_array(d.X.vals[i], d.X.ستون_تعداد);
    }
}

ڈیٹا get_ڈیٹا_part(ڈیٹا d, صحیح part, صحیح کل)
{
    ڈیٹا p = {0};
    p.shallow = 1;
    p.X.صفیں = d.X.صفیں * (part + 1) / کل - d.X.صفیں * part / کل;
    p.y.صفیں = d.y.صفیں * (part + 1) / کل - d.y.صفیں * part / کل;
    p.X.ستون_تعداد = d.X.ستون_تعداد;
    p.y.ستون_تعداد = d.y.ستون_تعداد;
    p.X.vals = d.X.vals + d.X.صفیں * part / کل;
    p.y.vals = d.y.vals + d.y.صفیں * part / کل;
    واپس p;
}

ڈیٹا get_random_ڈیٹا(ڈیٹا d, صحیح عدد)
{
    ڈیٹا r = {0};
    r.shallow = 1;

    r.X.صفیں = عدد;
    r.y.صفیں = عدد;

    r.X.ستون_تعداد = d.X.ستون_تعداد;
    r.y.ستون_تعداد = d.y.ستون_تعداد;

    r.X.vals = صفر_حافظہ_دو(عدد, حجم(اعشاری *));
    r.y.vals = صفر_حافظہ_دو(عدد, حجم(اعشاری *));

    صحیح i;
    برائے(i = 0; i < عدد; ++i){
        صحیح اشاریہ = rand()%d.X.صفیں;
        r.X.vals[i] = d.X.vals[اشاریہ];
        r.y.vals[i] = d.y.vals[اشاریہ];
    }
    واپس r;
}

ڈیٹا *بانٹو_ڈیٹا(ڈیٹا d, صحیح part, صحیح کل)
{
    ڈیٹا *بانٹو = صفر_حافظہ_دو(2, حجم(ڈیٹا));
    صحیح i;
    صحیح آغاز = part*d.X.صفیں/کل;
    صحیح انتہا = (part+1)*d.X.صفیں/کل;
    ڈیٹا train;
    ڈیٹا test;
    train.shallow = test.shallow = 1;

    test.X.صفیں = test.y.صفیں = انتہا-آغاز;
    train.X.صفیں = train.y.صفیں = d.X.صفیں - (انتہا-آغاز);
    train.X.ستون_تعداد = test.X.ستون_تعداد = d.X.ستون_تعداد;
    train.y.ستون_تعداد = test.y.ستون_تعداد = d.y.ستون_تعداد;

    train.X.vals = صفر_حافظہ_دو(train.X.صفیں, حجم(اعشاری*));
    test.X.vals = صفر_حافظہ_دو(test.X.صفیں, حجم(اعشاری*));
    train.y.vals = صفر_حافظہ_دو(train.y.صفیں, حجم(اعشاری*));
    test.y.vals = صفر_حافظہ_دو(test.y.صفیں, حجم(اعشاری*));

    برائے(i = 0; i < آغاز; ++i){
        train.X.vals[i] = d.X.vals[i];
        train.y.vals[i] = d.y.vals[i];
    }
    برائے(i = آغاز; i < انتہا; ++i){
        test.X.vals[i-آغاز] = d.X.vals[i];
        test.y.vals[i-آغاز] = d.y.vals[i];
    }
    برائے(i = انتہا; i < d.X.صفیں; ++i){
        train.X.vals[i-(انتہا-آغاز)] = d.X.vals[i];
        train.y.vals[i-(انتہا-آغاز)] = d.y.vals[i];
    }
    بانٹو[0] = train;
    بانٹو[1] = test;
    واپس بانٹو;
}

