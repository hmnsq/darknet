#অন্তর্ভাব "data.h"
#অন্তর্ভাব "utils.h"
#অন্তর্ভাব "image.h"
#অন্তর্ভাব "cuda.h"

#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <stdlib.h>
#অন্তর্ভাব <string.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

list *get_paths(অক্ষর *filename)
{
    অক্ষর *path;
    FILE *file = fopen(filename, "r");
    যদি(!file) file_error(filename);
    list *সারিগুলি = make_list();
    যখন((path=fgetl(file))){
        list_ঢোকাও(সারিগুলি, path);
    }
    fclose(file);
    ফেরত সারিগুলি;
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

অক্ষর **get_random_paths(অক্ষর **paths, পূর্ণসংখ্যা n, পূর্ণসংখ্যা m)
{
    অক্ষর **random_paths = calloc(n, আকার(অক্ষর*));
    পূর্ণসংখ্যা i;
    pthread_mutex_lock(&mutex);
    জন্য(i = 0; i < n; ++i){
        পূর্ণসংখ্যা সূচক = rand()%m;
        random_paths[i] = paths[সূচক];
        //if(i == 0) printf("%s\n", paths[index]);
    }
    pthread_mutex_unlock(&mutex);
    ফেরত random_paths;
}

অক্ষর **find_replace_paths(অক্ষর **paths, পূর্ণসংখ্যা n, অক্ষর *find, অক্ষর *replace)
{
    অক্ষর **replace_paths = calloc(n, আকার(অক্ষর*));
    পূর্ণসংখ্যা i;
    জন্য(i = 0; i < n; ++i){
        অক্ষর replaced[4096];
        find_replace(paths[i], find, replace, replaced);
        replace_paths[i] = copy_string(replaced);
    }
    ফেরত replace_paths;
}

matrix load_image_paths_gray(অক্ষর **paths, পূর্ণসংখ্যা n, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h)
{
    পূর্ণসংখ্যা i;
    matrix X;
    X.rows = n;
    X.vals = calloc(X.rows, আকার(দশমিক*));
    X.cols = 0;

    জন্য(i = 0; i < n; ++i){
        image im = load_image(paths[i], w, h, 3);

        image gray = grayscale_image(im);
        free_image(im);
        im = gray;

        X.vals[i] = im.তথ্য;
        X.cols = im.h*im.w*im.c;
    }
    ফেরত X;
}

matrix load_image_paths(অক্ষর **paths, পূর্ণসংখ্যা n, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h)
{
    পূর্ণসংখ্যা i;
    matrix X;
    X.rows = n;
    X.vals = calloc(X.rows, আকার(দশমিক*));
    X.cols = 0;

    জন্য(i = 0; i < n; ++i){
        image im = load_image_color(paths[i], w, h);
        X.vals[i] = im.তথ্য;
        X.cols = im.h*im.w*im.c;
    }
    ফেরত X;
}

matrix load_image_augment_paths(অক্ষর **paths, পূর্ণসংখ্যা n, পূর্ণসংখ্যা min, পূর্ণসংখ্যা max, পূর্ণসংখ্যা আকার_2, দশমিক angle, দশমিক aspect, দশমিক hue, দশমিক saturation, দশমিক exposure, পূর্ণসংখ্যা center)
{
    পূর্ণসংখ্যা i;
    matrix X;
    X.rows = n;
    X.vals = calloc(X.rows, আকার(দশমিক*));
    X.cols = 0;

    জন্য(i = 0; i < n; ++i){
        image im = load_image_color(paths[i], 0, 0);
        image crop;
        যদি(center){
            crop = center_crop_image(im, আকার_2, আকার_2);
        } অন্যথা {
            crop = random_augment_image(im, angle, aspect, min, max, আকার_2, আকার_2);
        }
        পূর্ণসংখ্যা flip = rand()%2;
        যদি (flip) flip_image(crop);
        random_distort_image(crop, hue, saturation, exposure);

        /*
        show_image(im, "orig");
        show_image(crop, "crop");
        cvWaitKey(0);
        */
        //grayscale_image_3c(crop);
        free_image(im);
        X.vals[i] = crop.তথ্য;
        X.cols = crop.h*crop.w*crop.c;
    }
    ফেরত X;
}


box_label *read_boxes(অক্ষর *filename, পূর্ণসংখ্যা *n)
{
    FILE *file = fopen(filename, "r");
    যদি(!file) file_error(filename);
    দশমিক x, y, h, w;
    পূর্ণসংখ্যা id;
    পূর্ণসংখ্যা গুনতি = 0;
    পূর্ণসংখ্যা আকার_2 = 64;
    box_label *boxes = calloc(আকার_2, আকার(box_label));
    যখন(fscanf(file, "%d %f %f %f %f", &id, &x, &y, &w, &h) == 5){
        যদি(গুনতি == আকার_2) {
            আকার_2 = আকার_2 * 2;
            boxes = realloc(boxes, আকার_2*আকার(box_label));
        }
        boxes[গুনতি].id = id;
        boxes[গুনতি].x = x;
        boxes[গুনতি].y = y;
        boxes[গুনতি].h = h;
        boxes[গুনতি].w = w;
        boxes[গুনতি].বাম   = x - w/2;
        boxes[গুনতি].ডান  = x + w/2;
        boxes[গুনতি].শীর্ষ    = y - h/2;
        boxes[গুনতি].bottom = y + h/2;
        ++গুনতি;
    }
    fclose(file);
    *n = গুনতি;
    ফেরত boxes;
}

শূন্য randomize_boxes(box_label *b, পূর্ণসংখ্যা n)
{
    পূর্ণসংখ্যা i;
    জন্য(i = 0; i < n; ++i){
        box_label swap = b[i];
        পূর্ণসংখ্যা সূচক = rand()%n;
        b[i] = b[সূচক];
        b[সূচক] = swap;
    }
}

শূন্য correct_boxes(box_label *boxes, পূর্ণসংখ্যা n, দশমিক dx, দশমিক dy, দশমিক sx, দশমিক sy, পূর্ণসংখ্যা flip)
{
    পূর্ণসংখ্যা i;
    জন্য(i = 0; i < n; ++i){
        যদি(boxes[i].x == 0 && boxes[i].y == 0) {
            boxes[i].x = 999999;
            boxes[i].y = 999999;
            boxes[i].w = 999999;
            boxes[i].h = 999999;
            চালু;
        }
        boxes[i].বাম   = boxes[i].বাম  * sx - dx;
        boxes[i].ডান  = boxes[i].ডান * sx - dx;
        boxes[i].শীর্ষ    = boxes[i].শীর্ষ   * sy - dy;
        boxes[i].bottom = boxes[i].bottom* sy - dy;

        যদি(flip){
            দশমিক swap = boxes[i].বাম;
            boxes[i].বাম = 1. - boxes[i].ডান;
            boxes[i].ডান = 1. - swap;
        }

        boxes[i].বাম =  constrain(0, 1, boxes[i].বাম);
        boxes[i].ডান = constrain(0, 1, boxes[i].ডান);
        boxes[i].শীর্ষ =   constrain(0, 1, boxes[i].শীর্ষ);
        boxes[i].bottom =   constrain(0, 1, boxes[i].bottom);

        boxes[i].x = (boxes[i].বাম+boxes[i].ডান)/2;
        boxes[i].y = (boxes[i].শীর্ষ+boxes[i].bottom)/2;
        boxes[i].w = (boxes[i].ডান - boxes[i].বাম);
        boxes[i].h = (boxes[i].bottom - boxes[i].শীর্ষ);

        boxes[i].w = constrain(0, 1, boxes[i].w);
        boxes[i].h = constrain(0, 1, boxes[i].h);
    }
}

শূন্য fill_truth_swag(অক্ষর *path, দশমিক *truth, পূর্ণসংখ্যা classes, পূর্ণসংখ্যা flip, দশমিক dx, দশমিক dy, দশমিক sx, দশমিক sy)
{
    অক্ষর labelpath[4096];
    find_replace(path, "images", "labels", labelpath);
    find_replace(labelpath, "JPEGImages", "labels", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);

    পূর্ণসংখ্যা গুনতি = 0;
    box_label *boxes = read_boxes(labelpath, &গুনতি);
    randomize_boxes(boxes, গুনতি);
    correct_boxes(boxes, গুনতি, dx, dy, sx, sy, flip);
    দশমিক x,y,w,h;
    পূর্ণসংখ্যা id;
    পূর্ণসংখ্যা i;

    জন্য (i = 0; i < গুনতি && i < 90; ++i) {
        x =  boxes[i].x;
        y =  boxes[i].y;
        w =  boxes[i].w;
        h =  boxes[i].h;
        id = boxes[i].id;

        যদি (w < .0 || h < .0) চালু;

        পূর্ণসংখ্যা সূচক = (4+classes) * i;

        truth[সূচক++] = x;
        truth[সূচক++] = y;
        truth[সূচক++] = w;
        truth[সূচক++] = h;

        যদি (id < classes) truth[সূচক+id] = 1;
    }
    মুক্ত(boxes);
}

শূন্য fill_truth_region(অক্ষর *path, দশমিক *truth, পূর্ণসংখ্যা classes, পূর্ণসংখ্যা অঙ্ক_boxes, পূর্ণসংখ্যা flip, দশমিক dx, দশমিক dy, দশমিক sx, দশমিক sy)
{
    অক্ষর labelpath[4096];
    find_replace(path, "images", "labels", labelpath);
    find_replace(labelpath, "JPEGImages", "labels", labelpath);

    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".png", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    পূর্ণসংখ্যা গুনতি = 0;
    box_label *boxes = read_boxes(labelpath, &গুনতি);
    randomize_boxes(boxes, গুনতি);
    correct_boxes(boxes, গুনতি, dx, dy, sx, sy, flip);
    দশমিক x,y,w,h;
    পূর্ণসংখ্যা id;
    পূর্ণসংখ্যা i;

    জন্য (i = 0; i < গুনতি; ++i) {
        x =  boxes[i].x;
        y =  boxes[i].y;
        w =  boxes[i].w;
        h =  boxes[i].h;
        id = boxes[i].id;

        যদি (w < .005 || h < .005) চালু;

        পূর্ণসংখ্যা col = (পূর্ণসংখ্যা)(x*অঙ্ক_boxes);
        পূর্ণসংখ্যা row = (পূর্ণসংখ্যা)(y*অঙ্ক_boxes);

        x = x*অঙ্ক_boxes - col;
        y = y*অঙ্ক_boxes - row;

        পূর্ণসংখ্যা সূচক = (col+row*অঙ্ক_boxes)*(5+classes);
        যদি (truth[সূচক]) চালু;
        truth[সূচক++] = 1;

        যদি (id < classes) truth[সূচক+id] = 1;
        সূচক += classes;

        truth[সূচক++] = x;
        truth[সূচক++] = y;
        truth[সূচক++] = w;
        truth[সূচক++] = h;
    }
    মুক্ত(boxes);
}

শূন্য load_rle(image im, পূর্ণসংখ্যা *rle, পূর্ণসংখ্যা n)
{
    পূর্ণসংখ্যা গুনতি = 0;
    পূর্ণসংখ্যা curr = 0;
    পূর্ণসংখ্যা i,j;
    জন্য(i = 0; i < n; ++i){
        জন্য(j = 0; j < rle[i]; ++j){
            im.তথ্য[গুনতি++] = curr;
        }
        curr = 1 - curr;
    }
    জন্য(; গুনতি < im.h*im.w*im.c; ++গুনতি){
        im.তথ্য[গুনতি] = curr;
    }
}

শূন্য or_image(image src, image dest, পূর্ণসংখ্যা c)
{
    পূর্ণসংখ্যা i;
    জন্য(i = 0; i < src.w*src.h; ++i){
        যদি(src.তথ্য[i]) dest.তথ্য[dest.w*dest.h*c + i] = 1;
    }
}

শূন্য exclusive_image(image src)
{
    পূর্ণসংখ্যা k, j, i;
    পূর্ণসংখ্যা s = src.w*src.h;
    জন্য(k = 0; k < src.c-1; ++k){
        জন্য(i = 0; i < s; ++i){
            যদি (src.তথ্য[k*s + i]){
                জন্য(j = k+1; j < src.c; ++j){
                    src.তথ্য[j*s + i] = 0;
                }
            }
        }
    }
}

box bound_image(image im)
{
    পূর্ণসংখ্যা x,y;
    পূর্ণসংখ্যা minx = im.w;
    পূর্ণসংখ্যা miny = im.h;
    পূর্ণসংখ্যা maxx = 0;
    পূর্ণসংখ্যা maxy = 0;
    জন্য(y = 0; y < im.h; ++y){
        জন্য(x = 0; x < im.w; ++x){
            যদি(im.তথ্য[y*im.w + x]){
                minx = (x < minx) ? x : minx;
                miny = (y < miny) ? y : miny;
                maxx = (x > maxx) ? x : maxx;
                maxy = (y > maxy) ? y : maxy;
            }
        }
    }
    box b = {minx, miny, maxx-minx + 1, maxy-miny + 1};
    //printf("%f %f %f %f\n", b.x, b.y, b.w, b.h);
    ফেরত b;
}

শূন্য fill_truth_iseg(অক্ষর *path, পূর্ণসংখ্যা অঙ্ক_boxes, দশমিক *truth, পূর্ণসংখ্যা classes, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h, augment_args aug, পূর্ণসংখ্যা flip, পূর্ণসংখ্যা mw, পূর্ণসংখ্যা mh)
{
    অক্ষর labelpath[4096];
    find_replace(path, "images", "mask", labelpath);
    find_replace(labelpath, "JPEGImages", "mask", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    FILE *file = fopen(labelpath, "r");
    যদি(!file) file_error(labelpath);
    অক্ষর buff[32788];
    পূর্ণসংখ্যা id;
    পূর্ণসংখ্যা i = 0;
    পূর্ণসংখ্যা j;
    image part = make_image(w, h, 1);
    যখন((fscanf(file, "%d %s", &id, buff) == 2) && i < অঙ্ক_boxes){
        পূর্ণসংখ্যা n = 0;
        পূর্ণসংখ্যা *rle = read_intlist(buff, &n, 0);
        load_rle(part, rle, n);
        image sized = rotate_crop_image(part, aug.rad, aug.scale, aug.w, aug.h, aug.dx, aug.dy, aug.aspect);
        যদি(flip) flip_image(sized);

        image mask = resize_image(sized, mw, mh);
        truth[i*(mw*mh+1)] = id;
        জন্য(j = 0; j < mw*mh; ++j){
            truth[i*(mw*mh + 1) + 1 + j] = mask.তথ্য[j];
        }
        ++i;

        free_image(mask);
        free_image(sized);
        মুক্ত(rle);
    }
    যদি(i < অঙ্ক_boxes) truth[i*(mw*mh+1)] = -1;
    fclose(file);
    free_image(part);
}

শূন্য fill_truth_mask(অক্ষর *path, পূর্ণসংখ্যা অঙ্ক_boxes, দশমিক *truth, পূর্ণসংখ্যা classes, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h, augment_args aug, পূর্ণসংখ্যা flip, পূর্ণসংখ্যা mw, পূর্ণসংখ্যা mh)
{
    অক্ষর labelpath[4096];
    find_replace(path, "images", "mask", labelpath);
    find_replace(labelpath, "JPEGImages", "mask", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    FILE *file = fopen(labelpath, "r");
    যদি(!file) file_error(labelpath);
    অক্ষর buff[32788];
    পূর্ণসংখ্যা id;
    পূর্ণসংখ্যা i = 0;
    image part = make_image(w, h, 1);
    যখন((fscanf(file, "%d %s", &id, buff) == 2) && i < অঙ্ক_boxes){
        পূর্ণসংখ্যা n = 0;
        পূর্ণসংখ্যা *rle = read_intlist(buff, &n, 0);
        load_rle(part, rle, n);
        image sized = rotate_crop_image(part, aug.rad, aug.scale, aug.w, aug.h, aug.dx, aug.dy, aug.aspect);
        যদি(flip) flip_image(sized);
        box b = bound_image(sized);
        যদি(b.w > 0){
            image crop = crop_image(sized, b.x, b.y, b.w, b.h);
            image mask = resize_image(crop, mw, mh);
            truth[i*(4 + mw*mh + 1) + 0] = (b.x + b.w/2.)/sized.w;
            truth[i*(4 + mw*mh + 1) + 1] = (b.y + b.h/2.)/sized.h;
            truth[i*(4 + mw*mh + 1) + 2] = b.w/sized.w;
            truth[i*(4 + mw*mh + 1) + 3] = b.h/sized.h;
            পূর্ণসংখ্যা j;
            জন্য(j = 0; j < mw*mh; ++j){
                truth[i*(4 + mw*mh + 1) + 4 + j] = mask.তথ্য[j];
            }
            truth[i*(4 + mw*mh + 1) + 4 + mw*mh] = id;
            free_image(crop);
            free_image(mask);
            ++i;
        }
        free_image(sized);
        মুক্ত(rle);
    }
    fclose(file);
    free_image(part);
}


শূন্য fill_truth_detection(অক্ষর *path, পূর্ণসংখ্যা অঙ্ক_boxes, দশমিক *truth, পূর্ণসংখ্যা classes, পূর্ণসংখ্যা flip, দশমিক dx, দশমিক dy, দশমিক sx, দশমিক sy)
{
    অক্ষর labelpath[4096];
    find_replace(path, "images", "labels", labelpath);
    find_replace(labelpath, "JPEGImages", "labels", labelpath);

    find_replace(labelpath, "raw", "labels", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".png", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    পূর্ণসংখ্যা গুনতি = 0;
    box_label *boxes = read_boxes(labelpath, &গুনতি);
    randomize_boxes(boxes, গুনতি);
    correct_boxes(boxes, গুনতি, dx, dy, sx, sy, flip);
    যদি(গুনতি > অঙ্ক_boxes) গুনতি = অঙ্ক_boxes;
    দশমিক x,y,w,h;
    পূর্ণসংখ্যা id;
    পূর্ণসংখ্যা i;
    পূর্ণসংখ্যা sub = 0;

    জন্য (i = 0; i < গুনতি; ++i) {
        x =  boxes[i].x;
        y =  boxes[i].y;
        w =  boxes[i].w;
        h =  boxes[i].h;
        id = boxes[i].id;

        যদি ((w < .001 || h < .001)) {
            ++sub;
            চালু;
        }

        truth[(i-sub)*5+0] = x;
        truth[(i-sub)*5+1] = y;
        truth[(i-sub)*5+2] = w;
        truth[(i-sub)*5+3] = h;
        truth[(i-sub)*5+4] = id;
    }
    মুক্ত(boxes);
}

#define NUMCHARS 37

শূন্য print_letters(দশমিক *pred, পূর্ণসংখ্যা n)
{
    পূর্ণসংখ্যা i;
    জন্য(i = 0; i < n; ++i){
        পূর্ণসংখ্যা সূচক = max_সূচক(pred+i*NUMCHARS, NUMCHARS);
        printf("%c", int_to_alphanum(সূচক));
    }
    printf("\n");
}

শূন্য fill_truth_captcha(অক্ষর *path, পূর্ণসংখ্যা n, দশমিক *truth)
{
    অক্ষর *begin = strrchr(path, '/');
    ++begin;
    পূর্ণসংখ্যা i;
    জন্য(i = 0; i < strlen(begin) && i < n && begin[i] != '.'; ++i){
        পূর্ণসংখ্যা সূচক = alphanum_to_int(begin[i]);
        যদি(সূচক > 35) printf("Bad %c\n", begin[i]);
        truth[i*NUMCHARS+সূচক] = 1;
    }
    জন্য(;i < n; ++i){
        truth[i*NUMCHARS + NUMCHARS-1] = 1;
    }
}

তথ্য load_তথ্য_captcha(অক্ষর **paths, পূর্ণসংখ্যা n, পূর্ণসংখ্যা m, পূর্ণসংখ্যা k, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h)
{
    যদি(m) paths = get_random_paths(paths, n, m);
    তথ্য d = {0};
    d.shallow = 0;
    d.X = load_image_paths(paths, n, w, h);
    d.y = make_matrix(n, k*NUMCHARS);
    পূর্ণসংখ্যা i;
    জন্য(i = 0; i < n; ++i){
        fill_truth_captcha(paths[i], k, d.y.vals[i]);
    }
    যদি(m) মুক্ত(paths);
    ফেরত d;
}

তথ্য load_তথ্য_captcha_encode(অক্ষর **paths, পূর্ণসংখ্যা n, পূর্ণসংখ্যা m, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h)
{
    যদি(m) paths = get_random_paths(paths, n, m);
    তথ্য d = {0};
    d.shallow = 0;
    d.X = load_image_paths(paths, n, w, h);
    d.X.cols = 17100;
    d.y = d.X;
    যদি(m) মুক্ত(paths);
    ফেরত d;
}

শূন্য fill_truth(অক্ষর *path, অক্ষর **labels, পূর্ণসংখ্যা k, দশমিক *truth)
{
    পূর্ণসংখ্যা i;
    memset(truth, 0, k*আকার(দশমিক));
    পূর্ণসংখ্যা গুনতি = 0;
    জন্য(i = 0; i < k; ++i){
        যদি(strstr(path, labels[i])){
            truth[i] = 1;
            ++গুনতি;
            //printf("%s %s %d\n", path, labels[i], i);
        }
    }
    যদি(গুনতি != 1 && (k != 1 || গুনতি != 0)) printf("Too many or too few labels: %d, %s\n", গুনতি, path);
}

শূন্য fill_hierarchy(দশমিক *truth, পূর্ণসংখ্যা k, বৃক্ষ *hierarchy)
{
    পূর্ণসংখ্যা j;
    জন্য(j = 0; j < k; ++j){
        যদি(truth[j]){
            পূর্ণসংখ্যা parent = hierarchy->parent[j];
            যখন(parent >= 0){
                truth[parent] = 1;
                parent = hierarchy->parent[parent];
            }
        }
    }
    পূর্ণসংখ্যা i;
    পূর্ণসংখ্যা গুনতি = 0;
    জন্য(j = 0; j < hierarchy->groups; ++j){
        //printf("%d\n", count);
        পূর্ণসংখ্যা mask = 1;
        জন্য(i = 0; i < hierarchy->group_আকার[j]; ++i){
            যদি(truth[গুনতি + i]){
                mask = 0;
                বিরতি;
            }
        }
        যদি (mask) {
            জন্য(i = 0; i < hierarchy->group_আকার[j]; ++i){
                truth[গুনতি + i] = SECRET_অঙ্ক;
            }
        }
        গুনতি += hierarchy->group_আকার[j];
    }
}

matrix load_regression_labels_paths(অক্ষর **paths, পূর্ণসংখ্যা n, পূর্ণসংখ্যা k)
{
    matrix y = make_matrix(n, k);
    পূর্ণসংখ্যা i,j;
    জন্য(i = 0; i < n; ++i){
        অক্ষর labelpath[4096];
        find_replace(paths[i], "images", "labels", labelpath);
        find_replace(labelpath, "JPEGImages", "labels", labelpath);
        find_replace(labelpath, ".BMP", ".txt", labelpath);
        find_replace(labelpath, ".JPEG", ".txt", labelpath);
        find_replace(labelpath, ".JPG", ".txt", labelpath);
        find_replace(labelpath, ".JPeG", ".txt", labelpath);
        find_replace(labelpath, ".Jpeg", ".txt", labelpath);
        find_replace(labelpath, ".PNG", ".txt", labelpath);
        find_replace(labelpath, ".TIF", ".txt", labelpath);
        find_replace(labelpath, ".bmp", ".txt", labelpath);
        find_replace(labelpath, ".jpeg", ".txt", labelpath);
        find_replace(labelpath, ".jpg", ".txt", labelpath);
        find_replace(labelpath, ".png", ".txt", labelpath);
        find_replace(labelpath, ".tif", ".txt", labelpath);

        FILE *file = fopen(labelpath, "r");
        জন্য(j = 0; j < k; ++j){
            fscanf(file, "%f", &(y.vals[i][j]));
        }
        fclose(file);
    }
    ফেরত y;
}

matrix load_labels_paths(অক্ষর **paths, পূর্ণসংখ্যা n, অক্ষর **labels, পূর্ণসংখ্যা k, বৃক্ষ *hierarchy)
{
    matrix y = make_matrix(n, k);
    পূর্ণসংখ্যা i;
    জন্য(i = 0; i < n && labels; ++i){
        fill_truth(paths[i], labels, k, y.vals[i]);
        যদি(hierarchy){
            fill_hierarchy(y.vals[i], k, hierarchy);
        }
    }
    ফেরত y;
}

matrix load_tags_paths(অক্ষর **paths, পূর্ণসংখ্যা n, পূর্ণসংখ্যা k)
{
    matrix y = make_matrix(n, k);
    পূর্ণসংখ্যা i;
    //int count = 0;
    জন্য(i = 0; i < n; ++i){
        অক্ষর label[4096];
        find_replace(paths[i], "images", "labels", label);
        find_replace(label, ".jpg", ".txt", label);
        FILE *file = fopen(label, "r");
        যদি (!file) চালু;
        //++count;
        পূর্ণসংখ্যা tag;
        যখন(fscanf(file, "%d", &tag) == 1){
            যদি(tag < k){
                y.vals[i][tag] = 1;
            }
        }
        fclose(file);
    }
    //printf("%d/%d\n", count, n);
    ফেরত y;
}

অক্ষর **get_labels(অক্ষর *filename)
{
    list *plist = get_paths(filename);
    অক্ষর **labels = (অক্ষর **)list_to_array(plist);
    free_list(plist);
    ফেরত labels;
}

শূন্য free_তথ্য(তথ্য d)
{
    যদি(!d.shallow){
        free_matrix(d.X);
        free_matrix(d.y);
    }অন্যথা{
        মুক্ত(d.X.vals);
        মুক্ত(d.y.vals);
    }
}

image get_segmentation_image(অক্ষর *path, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h, পূর্ণসংখ্যা classes)
{
    অক্ষর labelpath[4096];
    find_replace(path, "images", "mask", labelpath);
    find_replace(labelpath, "JPEGImages", "mask", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    image mask = make_image(w, h, classes);
    FILE *file = fopen(labelpath, "r");
    যদি(!file) file_error(labelpath);
    অক্ষর buff[32788];
    পূর্ণসংখ্যা id;
    image part = make_image(w, h, 1);
    যখন(fscanf(file, "%d %s", &id, buff) == 2){
        পূর্ণসংখ্যা n = 0;
        পূর্ণসংখ্যা *rle = read_intlist(buff, &n, 0);
        load_rle(part, rle, n);
        or_image(part, mask, id);
        মুক্ত(rle);
    }
    //exclusive_image(mask);
    fclose(file);
    free_image(part);
    ফেরত mask;
}

image get_segmentation_image2(অক্ষর *path, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h, পূর্ণসংখ্যা classes)
{
    অক্ষর labelpath[4096];
    find_replace(path, "images", "mask", labelpath);
    find_replace(labelpath, "JPEGImages", "mask", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    image mask = make_image(w, h, classes+1);
    পূর্ণসংখ্যা i;
    জন্য(i = 0; i < w*h; ++i){
        mask.তথ্য[w*h*classes + i] = 1;
    }
    FILE *file = fopen(labelpath, "r");
    যদি(!file) file_error(labelpath);
    অক্ষর buff[32788];
    পূর্ণসংখ্যা id;
    image part = make_image(w, h, 1);
    যখন(fscanf(file, "%d %s", &id, buff) == 2){
        পূর্ণসংখ্যা n = 0;
        পূর্ণসংখ্যা *rle = read_intlist(buff, &n, 0);
        load_rle(part, rle, n);
        or_image(part, mask, id);
        জন্য(i = 0; i < w*h; ++i){
            যদি(part.তথ্য[i]) mask.তথ্য[w*h*classes + i] = 0;
        }
        মুক্ত(rle);
    }
    //exclusive_image(mask);
    fclose(file);
    free_image(part);
    ফেরত mask;
}

তথ্য load_তথ্য_seg(পূর্ণসংখ্যা n, অক্ষর **paths, পূর্ণসংখ্যা m, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h, পূর্ণসংখ্যা classes, পূর্ণসংখ্যা min, পূর্ণসংখ্যা max, দশমিক angle, দশমিক aspect, দশমিক hue, দশমিক saturation, দশমিক exposure, পূর্ণসংখ্যা div)
{
    অক্ষর **random_paths = get_random_paths(paths, n, m);
    পূর্ণসংখ্যা i;
    তথ্য d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, আকার(দশমিক*));
    d.X.cols = h*w*3;


    d.y.rows = n;
    d.y.cols = h*w*classes/div/div;
    d.y.vals = calloc(d.X.rows, আকার(দশমিক*));

    জন্য(i = 0; i < n; ++i){
        image orig = load_image_color(random_paths[i], 0, 0);
        augment_args a = random_augment_args(orig, angle, aspect, min, max, w, h);
        image sized = rotate_crop_image(orig, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);

        পূর্ণসংখ্যা flip = rand()%2;
        যদি(flip) flip_image(sized);
        random_distort_image(sized, hue, saturation, exposure);
        d.X.vals[i] = sized.তথ্য;

        image mask = get_segmentation_image(random_paths[i], orig.w, orig.h, classes);
        //image mask = make_image(orig.w, orig.h, classes+1);
        image sized_m = rotate_crop_image(mask, a.rad, a.scale/div, a.w/div, a.h/div, a.dx/div, a.dy/div, a.aspect);

        যদি(flip) flip_image(sized_m);
        d.y.vals[i] = sized_m.তথ্য;

        free_image(orig);
        free_image(mask);

        /*
           image rgb = mask_to_rgb(sized_m, classes);
           show_image(rgb, "part");
           show_image(sized, "orig");
           cvWaitKey(0);
           free_image(rgb);
         */
    }
    মুক্ত(random_paths);
    ফেরত d;
}

তথ্য load_তথ্য_iseg(পূর্ণসংখ্যা n, অক্ষর **paths, পূর্ণসংখ্যা m, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h, পূর্ণসংখ্যা classes, পূর্ণসংখ্যা boxes, পূর্ণসংখ্যা div, পূর্ণসংখ্যা min, পূর্ণসংখ্যা max, দশমিক angle, দশমিক aspect, দশমিক hue, দশমিক saturation, দশমিক exposure)
{
    অক্ষর **random_paths = get_random_paths(paths, n, m);
    পূর্ণসংখ্যা i;
    তথ্য d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, আকার(দশমিক*));
    d.X.cols = h*w*3;

    d.y = make_matrix(n, (((w/div)*(h/div))+1)*boxes);

    জন্য(i = 0; i < n; ++i){
        image orig = load_image_color(random_paths[i], 0, 0);
        augment_args a = random_augment_args(orig, angle, aspect, min, max, w, h);
        image sized = rotate_crop_image(orig, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);

        পূর্ণসংখ্যা flip = rand()%2;
        যদি(flip) flip_image(sized);
        random_distort_image(sized, hue, saturation, exposure);
        d.X.vals[i] = sized.তথ্য;
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
    মুক্ত(random_paths);
    ফেরত d;
}

তথ্য load_তথ্য_mask(পূর্ণসংখ্যা n, অক্ষর **paths, পূর্ণসংখ্যা m, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h, পূর্ণসংখ্যা classes, পূর্ণসংখ্যা boxes, পূর্ণসংখ্যা coords, পূর্ণসংখ্যা min, পূর্ণসংখ্যা max, দশমিক angle, দশমিক aspect, দশমিক hue, দশমিক saturation, দশমিক exposure)
{
    অক্ষর **random_paths = get_random_paths(paths, n, m);
    পূর্ণসংখ্যা i;
    তথ্য d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, আকার(দশমিক*));
    d.X.cols = h*w*3;

    d.y = make_matrix(n, (coords+1)*boxes);

    জন্য(i = 0; i < n; ++i){
        image orig = load_image_color(random_paths[i], 0, 0);
        augment_args a = random_augment_args(orig, angle, aspect, min, max, w, h);
        image sized = rotate_crop_image(orig, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);

        পূর্ণসংখ্যা flip = rand()%2;
        যদি(flip) flip_image(sized);
        random_distort_image(sized, hue, saturation, exposure);
        d.X.vals[i] = sized.তথ্য;
        //show_image(sized, "image");

        fill_truth_mask(random_paths[i], boxes, d.y.vals[i], classes, orig.w, orig.h, a, flip, 14, 14);

        free_image(orig);

        /*
           image rgb = mask_to_rgb(sized_m, classes);
           show_image(rgb, "part");
           show_image(sized, "orig");
           cvWaitKey(0);
           free_image(rgb);
         */
    }
    মুক্ত(random_paths);
    ফেরত d;
}

তথ্য load_তথ্য_region(পূর্ণসংখ্যা n, অক্ষর **paths, পূর্ণসংখ্যা m, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h, পূর্ণসংখ্যা আকার_2, পূর্ণসংখ্যা classes, দশমিক jitter, দশমিক hue, দশমিক saturation, দশমিক exposure)
{
    অক্ষর **random_paths = get_random_paths(paths, n, m);
    পূর্ণসংখ্যা i;
    তথ্য d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, আকার(দশমিক*));
    d.X.cols = h*w*3;


    পূর্ণসংখ্যা k = আকার_2*আকার_2*(5+classes);
    d.y = make_matrix(n, k);
    জন্য(i = 0; i < n; ++i){
        image orig = load_image_color(random_paths[i], 0, 0);

        পূর্ণসংখ্যা oh = orig.h;
        পূর্ণসংখ্যা ow = orig.w;

        পূর্ণসংখ্যা dw = (ow*jitter);
        পূর্ণসংখ্যা dh = (oh*jitter);

        পূর্ণসংখ্যা pleft  = rand_uniform(-dw, dw);
        পূর্ণসংখ্যা pright = rand_uniform(-dw, dw);
        পূর্ণসংখ্যা ptop   = rand_uniform(-dh, dh);
        পূর্ণসংখ্যা pbot   = rand_uniform(-dh, dh);

        পূর্ণসংখ্যা swidth =  ow - pleft - pright;
        পূর্ণসংখ্যা sheight = oh - ptop - pbot;

        দশমিক sx = (দশমিক)swidth  / ow;
        দশমিক sy = (দশমিক)sheight / oh;

        পূর্ণসংখ্যা flip = rand()%2;
        image cropped = crop_image(orig, pleft, ptop, swidth, sheight);

        দশমিক dx = ((দশমিক)pleft/ow)/sx;
        দশমিক dy = ((দশমিক)ptop /oh)/sy;

        image sized = resize_image(cropped, w, h);
        যদি(flip) flip_image(sized);
        random_distort_image(sized, hue, saturation, exposure);
        d.X.vals[i] = sized.তথ্য;

        fill_truth_region(random_paths[i], d.y.vals[i], classes, আকার_2, flip, dx, dy, 1./sx, 1./sy);

        free_image(orig);
        free_image(cropped);
    }
    মুক্ত(random_paths);
    ফেরত d;
}

তথ্য load_তথ্য_compare(পূর্ণসংখ্যা n, অক্ষর **paths, পূর্ণসংখ্যা m, পূর্ণসংখ্যা classes, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h)
{
    যদি(m) paths = get_random_paths(paths, 2*n, m);
    পূর্ণসংখ্যা i,j;
    তথ্য d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, আকার(দশমিক*));
    d.X.cols = h*w*6;

    পূর্ণসংখ্যা k = 2*(classes);
    d.y = make_matrix(n, k);
    জন্য(i = 0; i < n; ++i){
        image im1 = load_image_color(paths[i*2],   w, h);
        image im2 = load_image_color(paths[i*2+1], w, h);

        d.X.vals[i] = calloc(d.X.cols, আকার(দশমিক));
        memcpy(d.X.vals[i],         im1.তথ্য, h*w*3*আকার(দশমিক));
        memcpy(d.X.vals[i] + h*w*3, im2.তথ্য, h*w*3*আকার(দশমিক));

        পূর্ণসংখ্যা id;
        দশমিক iou;

        অক্ষর imlabel1[4096];
        অক্ষর imlabel2[4096];
        find_replace(paths[i*2],   "imgs", "labels", imlabel1);
        find_replace(imlabel1, "jpg", "txt", imlabel1);
        FILE *fp1 = fopen(imlabel1, "r");

        যখন(fscanf(fp1, "%d %f", &id, &iou) == 2){
            যদি (d.y.vals[i][2*id] < iou) d.y.vals[i][2*id] = iou;
        }

        find_replace(paths[i*2+1], "imgs", "labels", imlabel2);
        find_replace(imlabel2, "jpg", "txt", imlabel2);
        FILE *fp2 = fopen(imlabel2, "r");

        যখন(fscanf(fp2, "%d %f", &id, &iou) == 2){
            যদি (d.y.vals[i][2*id + 1] < iou) d.y.vals[i][2*id + 1] = iou;
        }

        জন্য (j = 0; j < classes; ++j){
            যদি (d.y.vals[i][2*j] > .5 &&  d.y.vals[i][2*j+1] < .5){
                d.y.vals[i][2*j] = 1;
                d.y.vals[i][2*j+1] = 0;
            } অন্যথা যদি (d.y.vals[i][2*j] < .5 &&  d.y.vals[i][2*j+1] > .5){
                d.y.vals[i][2*j] = 0;
                d.y.vals[i][2*j+1] = 1;
            } অন্যথা {
                d.y.vals[i][2*j]   = SECRET_অঙ্ক;
                d.y.vals[i][2*j+1] = SECRET_অঙ্ক;
            }
        }
        fclose(fp1);
        fclose(fp2);

        free_image(im1);
        free_image(im2);
    }
    যদি(m) মুক্ত(paths);
    ফেরত d;
}

তথ্য load_তথ্য_swag(অক্ষর **paths, পূর্ণসংখ্যা n, পূর্ণসংখ্যা classes, দশমিক jitter)
{
    পূর্ণসংখ্যা সূচক = rand()%n;
    অক্ষর *random_path = paths[সূচক];

    image orig = load_image_color(random_path, 0, 0);
    পূর্ণসংখ্যা h = orig.h;
    পূর্ণসংখ্যা w = orig.w;

    তথ্য d = {0};
    d.shallow = 0;
    d.w = w;
    d.h = h;

    d.X.rows = 1;
    d.X.vals = calloc(d.X.rows, আকার(দশমিক*));
    d.X.cols = h*w*3;

    পূর্ণসংখ্যা k = (4+classes)*90;
    d.y = make_matrix(1, k);

    পূর্ণসংখ্যা dw = w*jitter;
    পূর্ণসংখ্যা dh = h*jitter;

    পূর্ণসংখ্যা pleft  = rand_uniform(-dw, dw);
    পূর্ণসংখ্যা pright = rand_uniform(-dw, dw);
    পূর্ণসংখ্যা ptop   = rand_uniform(-dh, dh);
    পূর্ণসংখ্যা pbot   = rand_uniform(-dh, dh);

    পূর্ণসংখ্যা swidth =  w - pleft - pright;
    পূর্ণসংখ্যা sheight = h - ptop - pbot;

    দশমিক sx = (দশমিক)swidth  / w;
    দশমিক sy = (দশমিক)sheight / h;

    পূর্ণসংখ্যা flip = rand()%2;
    image cropped = crop_image(orig, pleft, ptop, swidth, sheight);

    দশমিক dx = ((দশমিক)pleft/w)/sx;
    দশমিক dy = ((দশমিক)ptop /h)/sy;

    image sized = resize_image(cropped, w, h);
    যদি(flip) flip_image(sized);
    d.X.vals[0] = sized.তথ্য;

    fill_truth_swag(random_path, d.y.vals[0], classes, flip, dx, dy, 1./sx, 1./sy);

    free_image(orig);
    free_image(cropped);

    ফেরত d;
}

তথ্য load_তথ্য_detection(পূর্ণসংখ্যা n, অক্ষর **paths, পূর্ণসংখ্যা m, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h, পূর্ণসংখ্যা boxes, পূর্ণসংখ্যা classes, দশমিক jitter, দশমিক hue, দশমিক saturation, দশমিক exposure)
{
    অক্ষর **random_paths = get_random_paths(paths, n, m);
    পূর্ণসংখ্যা i;
    তথ্য d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, আকার(দশমিক*));
    d.X.cols = h*w*3;

    d.y = make_matrix(n, 5*boxes);
    জন্য(i = 0; i < n; ++i){
        image orig = load_image_color(random_paths[i], 0, 0);
        image sized = make_image(w, h, orig.c);
        fill_image(sized, .5);

        দশমিক dw = jitter * orig.w;
        দশমিক dh = jitter * orig.h;

        দশমিক new_ar = (orig.w + rand_uniform(-dw, dw)) / (orig.h + rand_uniform(-dh, dh));
        //float scale = rand_uniform(.25, 2);
        দশমিক scale = 1;

        দশমিক nw, nh;

        যদি(new_ar < 1){
            nh = scale * h;
            nw = nh * new_ar;
        } অন্যথা {
            nw = scale * w;
            nh = nw / new_ar;
        }

        দশমিক dx = rand_uniform(0, w - nw);
        দশমিক dy = rand_uniform(0, h - nh);

        place_image(orig, nw, nh, dx, dy, sized);

        random_distort_image(sized, hue, saturation, exposure);

        পূর্ণসংখ্যা flip = rand()%2;
        যদি(flip) flip_image(sized);
        d.X.vals[i] = sized.তথ্য;


        fill_truth_detection(random_paths[i], boxes, d.y.vals[i], classes, flip, -dx/w, -dy/h, nw/w, nh/h);

        free_image(orig);
    }
    মুক্ত(random_paths);
    ফেরত d;
}

শূন্য *load_thread(শূন্য *ptr)
{
    //printf("Loading data: %d\n", rand());
    load_args a = *(কাঠামো load_args*)ptr;
    যদি(a.exposure == 0) a.exposure = 1;
    যদি(a.saturation == 0) a.saturation = 1;
    যদি(a.aspect == 0) a.aspect = 1;

    যদি (a.type == OLD_CLASSIFICATION_তথ্য){
        *a.d = load_তথ্য_old(a.paths, a.n, a.m, a.labels, a.classes, a.w, a.h);
    } অন্যথা যদি (a.type == REGRESSION_তথ্য){
        *a.d = load_তথ্য_regression(a.paths, a.n, a.m, a.classes, a.min, a.max, a.আকার_2, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    } অন্যথা যদি (a.type == CLASSIFICATION_তথ্য){
        *a.d = load_তথ্য_augment(a.paths, a.n, a.m, a.labels, a.classes, a.hierarchy, a.min, a.max, a.আকার_2, a.angle, a.aspect, a.hue, a.saturation, a.exposure, a.center);
    } অন্যথা যদি (a.type == SUPER_তথ্য){
        *a.d = load_তথ্য_super(a.paths, a.n, a.m, a.w, a.h, a.scale);
    } অন্যথা যদি (a.type == WRITING_তথ্য){
        *a.d = load_তথ্য_writing(a.paths, a.n, a.m, a.w, a.h, a.out_w, a.out_h);
    } অন্যথা যদি (a.type == ISEG_তথ্য){
        *a.d = load_তথ্য_iseg(a.n, a.paths, a.m, a.w, a.h, a.classes, a.অঙ্ক_boxes, a.scale, a.min, a.max, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    } অন্যথা যদি (a.type == INSTANCE_তথ্য){
        *a.d = load_তথ্য_mask(a.n, a.paths, a.m, a.w, a.h, a.classes, a.অঙ্ক_boxes, a.coords, a.min, a.max, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    } অন্যথা যদি (a.type == SEGMENTATION_তথ্য){
        *a.d = load_তথ্য_seg(a.n, a.paths, a.m, a.w, a.h, a.classes, a.min, a.max, a.angle, a.aspect, a.hue, a.saturation, a.exposure, a.scale);
    } অন্যথা যদি (a.type == REGION_তথ্য){
        *a.d = load_তথ্য_region(a.n, a.paths, a.m, a.w, a.h, a.অঙ্ক_boxes, a.classes, a.jitter, a.hue, a.saturation, a.exposure);
    } অন্যথা যদি (a.type == DETECTION_তথ্য){
        *a.d = load_তথ্য_detection(a.n, a.paths, a.m, a.w, a.h, a.অঙ্ক_boxes, a.classes, a.jitter, a.hue, a.saturation, a.exposure);
    } অন্যথা যদি (a.type == SWAG_তথ্য){
        *a.d = load_তথ্য_swag(a.paths, a.n, a.classes, a.jitter);
    } অন্যথা যদি (a.type == COMPARE_তথ্য){
        *a.d = load_তথ্য_compare(a.n, a.paths, a.m, a.classes, a.w, a.h);
    } অন্যথা যদি (a.type == IMAGE_তথ্য){
        *(a.im) = load_image_color(a.path, 0, 0);
        *(a.resized) = resize_image(*(a.im), a.w, a.h);
    } অন্যথা যদি (a.type == LETTERBOX_তথ্য){
        *(a.im) = load_image_color(a.path, 0, 0);
        *(a.resized) = letterbox_image(*(a.im), a.w, a.h);
    } অন্যথা যদি (a.type == TAG_তথ্য){
        *a.d = load_তথ্য_tag(a.paths, a.n, a.m, a.classes, a.min, a.max, a.আকার_2, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    }
    মুক্ত(ptr);
    ফেরত 0;
}

pthread_t load_তথ্য_in_thread(load_args args)
{
    pthread_t thread;
    কাঠামো load_args *ptr = calloc(1, আকার(কাঠামো load_args));
    *ptr = args;
    যদি(pthread_create(&thread, 0, load_thread, ptr)) error("Thread creation failed");
    ফেরত thread;
}

শূন্য *load_threads(শূন্য *ptr)
{
    পূর্ণসংখ্যা i;
    load_args args = *(load_args *)ptr;
    যদি (args.threads == 0) args.threads = 1;
    তথ্য *out = args.d;
    পূর্ণসংখ্যা যোগ_2 = args.n;
    মুক্ত(ptr);
    তথ্য *buffers = calloc(args.threads, আকার(তথ্য));
    pthread_t *threads = calloc(args.threads, আকার(pthread_t));
    জন্য(i = 0; i < args.threads; ++i){
        args.d = buffers + i;
        args.n = (i+1) * যোগ_2/args.threads - i * যোগ_2/args.threads;
        threads[i] = load_তথ্য_in_thread(args);
    }
    জন্য(i = 0; i < args.threads; ++i){
        pthread_join(threads[i], 0);
    }
    *out = concat_datas(buffers, args.threads);
    out->shallow = 0;
    জন্য(i = 0; i < args.threads; ++i){
        buffers[i].shallow = 1;
        free_তথ্য(buffers[i]);
    }
    মুক্ত(buffers);
    মুক্ত(threads);
    ফেরত 0;
}

শূন্য load_তথ্য_blocking(load_args args)
{
    কাঠামো load_args *ptr = calloc(1, আকার(কাঠামো load_args));
    *ptr = args;
    load_thread(ptr);
}

pthread_t load_তথ্য(load_args args)
{
    pthread_t thread;
    কাঠামো load_args *ptr = calloc(1, আকার(কাঠামো load_args));
    *ptr = args;
    যদি(pthread_create(&thread, 0, load_threads, ptr)) error("Thread creation failed");
    ফেরত thread;
}

তথ্য load_তথ্য_writing(অক্ষর **paths, পূর্ণসংখ্যা n, পূর্ণসংখ্যা m, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h, পূর্ণসংখ্যা out_w, পূর্ণসংখ্যা out_h)
{
    যদি(m) paths = get_random_paths(paths, n, m);
    অক্ষর **replace_paths = find_replace_paths(paths, n, ".png", "-label.png");
    তথ্য d = {0};
    d.shallow = 0;
    d.X = load_image_paths(paths, n, w, h);
    d.y = load_image_paths_gray(replace_paths, n, out_w, out_h);
    যদি(m) মুক্ত(paths);
    পূর্ণসংখ্যা i;
    জন্য(i = 0; i < n; ++i) মুক্ত(replace_paths[i]);
    মুক্ত(replace_paths);
    ফেরত d;
}

তথ্য load_তথ্য_old(অক্ষর **paths, পূর্ণসংখ্যা n, পূর্ণসংখ্যা m, অক্ষর **labels, পূর্ণসংখ্যা k, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h)
{
    যদি(m) paths = get_random_paths(paths, n, m);
    তথ্য d = {0};
    d.shallow = 0;
    d.X = load_image_paths(paths, n, w, h);
    d.y = load_labels_paths(paths, n, labels, k, 0);
    যদি(m) মুক্ত(paths);
    ফেরত d;
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

তথ্য load_তথ্য_super(অক্ষর **paths, পূর্ণসংখ্যা n, পূর্ণসংখ্যা m, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h, পূর্ণসংখ্যা scale)
{
    যদি(m) paths = get_random_paths(paths, n, m);
    তথ্য d = {0};
    d.shallow = 0;

    পূর্ণসংখ্যা i;
    d.X.rows = n;
    d.X.vals = calloc(n, আকার(দশমিক*));
    d.X.cols = w*h*3;

    d.y.rows = n;
    d.y.vals = calloc(n, আকার(দশমিক*));
    d.y.cols = w*scale * h*scale * 3;

    জন্য(i = 0; i < n; ++i){
        image im = load_image_color(paths[i], 0, 0);
        image crop = random_crop_image(im, w*scale, h*scale);
        পূর্ণসংখ্যা flip = rand()%2;
        যদি (flip) flip_image(crop);
        image resize = resize_image(crop, w, h);
        d.X.vals[i] = resize.তথ্য;
        d.y.vals[i] = crop.তথ্য;
        free_image(im);
    }

    যদি(m) মুক্ত(paths);
    ফেরত d;
}

তথ্য load_তথ্য_regression(অক্ষর **paths, পূর্ণসংখ্যা n, পূর্ণসংখ্যা m, পূর্ণসংখ্যা k, পূর্ণসংখ্যা min, পূর্ণসংখ্যা max, পূর্ণসংখ্যা আকার_2, দশমিক angle, দশমিক aspect, দশমিক hue, দশমিক saturation, দশমিক exposure)
{
    যদি(m) paths = get_random_paths(paths, n, m);
    তথ্য d = {0};
    d.shallow = 0;
    d.X = load_image_augment_paths(paths, n, min, max, আকার_2, angle, aspect, hue, saturation, exposure, 0);
    d.y = load_regression_labels_paths(paths, n, k);
    যদি(m) মুক্ত(paths);
    ফেরত d;
}

তথ্য select_তথ্য(তথ্য *orig, পূর্ণসংখ্যা *inds)
{
    তথ্য d = {0};
    d.shallow = 1;
    d.w = orig[0].w;
    d.h = orig[0].h;

    d.X.rows = orig[0].X.rows;
    d.y.rows = orig[0].X.rows;

    d.X.cols = orig[0].X.cols;
    d.y.cols = orig[0].y.cols;

    d.X.vals = calloc(orig[0].X.rows, আকার(দশমিক *));
    d.y.vals = calloc(orig[0].y.rows, আকার(দশমিক *));
    পূর্ণসংখ্যা i;
    জন্য(i = 0; i < d.X.rows; ++i){
        d.X.vals[i] = orig[inds[i]].X.vals[i];
        d.y.vals[i] = orig[inds[i]].y.vals[i];
    }
    ফেরত d;
}

তথ্য *tile_তথ্য(তথ্য orig, পূর্ণসংখ্যা divs, পূর্ণসংখ্যা আকার_2)
{
    তথ্য *ds = calloc(divs*divs, আকার(তথ্য));
    পূর্ণসংখ্যা i, j;
#pragma omp parallel জন্য
    জন্য(i = 0; i < divs*divs; ++i){
        তথ্য d;
        d.shallow = 0;
        d.w = orig.w/divs * আকার_2;
        d.h = orig.h/divs * আকার_2;
        d.X.rows = orig.X.rows;
        d.X.cols = d.w*d.h*3;
        d.X.vals = calloc(d.X.rows, আকার(দশমিক*));

        d.y = copy_matrix(orig.y);
#pragma omp parallel জন্য
        জন্য(j = 0; j < orig.X.rows; ++j){
            পূর্ণসংখ্যা x = (i%divs) * orig.w / divs - (d.w - orig.w/divs)/2;
            পূর্ণসংখ্যা y = (i/divs) * orig.h / divs - (d.h - orig.h/divs)/2;
            image im = float_to_image(orig.w, orig.h, 3, orig.X.vals[j]);
            d.X.vals[j] = crop_image(im, x, y, d.w, d.h).তথ্য;
        }
        ds[i] = d;
    }
    ফেরত ds;
}

তথ্য resize_তথ্য(তথ্য orig, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h)
{
    তথ্য d = {0};
    d.shallow = 0;
    d.w = w;
    d.h = h;
    পূর্ণসংখ্যা i;
    d.X.rows = orig.X.rows;
    d.X.cols = w*h*3;
    d.X.vals = calloc(d.X.rows, আকার(দশমিক*));

    d.y = copy_matrix(orig.y);
#pragma omp parallel জন্য
    জন্য(i = 0; i < orig.X.rows; ++i){
        image im = float_to_image(orig.w, orig.h, 3, orig.X.vals[i]);
        d.X.vals[i] = resize_image(im, w, h).তথ্য;
    }
    ফেরত d;
}

তথ্য load_তথ্য_augment(অক্ষর **paths, পূর্ণসংখ্যা n, পূর্ণসংখ্যা m, অক্ষর **labels, পূর্ণসংখ্যা k, বৃক্ষ *hierarchy, পূর্ণসংখ্যা min, পূর্ণসংখ্যা max, পূর্ণসংখ্যা আকার_2, দশমিক angle, দশমিক aspect, দশমিক hue, দশমিক saturation, দশমিক exposure, পূর্ণসংখ্যা center)
{
    যদি(m) paths = get_random_paths(paths, n, m);
    তথ্য d = {0};
    d.shallow = 0;
    d.w=আকার_2;
    d.h=আকার_2;
    d.X = load_image_augment_paths(paths, n, min, max, আকার_2, angle, aspect, hue, saturation, exposure, center);
    d.y = load_labels_paths(paths, n, labels, k, hierarchy);
    যদি(m) মুক্ত(paths);
    ফেরত d;
}

তথ্য load_তথ্য_tag(অক্ষর **paths, পূর্ণসংখ্যা n, পূর্ণসংখ্যা m, পূর্ণসংখ্যা k, পূর্ণসংখ্যা min, পূর্ণসংখ্যা max, পূর্ণসংখ্যা আকার_2, দশমিক angle, দশমিক aspect, দশমিক hue, দশমিক saturation, দশমিক exposure)
{
    যদি(m) paths = get_random_paths(paths, n, m);
    তথ্য d = {0};
    d.w = আকার_2;
    d.h = আকার_2;
    d.shallow = 0;
    d.X = load_image_augment_paths(paths, n, min, max, আকার_2, angle, aspect, hue, saturation, exposure, 0);
    d.y = load_tags_paths(paths, n, k);
    যদি(m) মুক্ত(paths);
    ফেরত d;
}

matrix concat_matrix(matrix m1, matrix m2)
{
    পূর্ণসংখ্যা i, গুনতি = 0;
    matrix m;
    m.cols = m1.cols;
    m.rows = m1.rows+m2.rows;
    m.vals = calloc(m1.rows + m2.rows, আকার(দশমিক*));
    জন্য(i = 0; i < m1.rows; ++i){
        m.vals[গুনতি++] = m1.vals[i];
    }
    জন্য(i = 0; i < m2.rows; ++i){
        m.vals[গুনতি++] = m2.vals[i];
    }
    ফেরত m;
}

তথ্য concat_তথ্য(তথ্য d1, তথ্য d2)
{
    তথ্য d = {0};
    d.shallow = 1;
    d.X = concat_matrix(d1.X, d2.X);
    d.y = concat_matrix(d1.y, d2.y);
    d.w = d1.w;
    d.h = d1.h;
    ফেরত d;
}

তথ্য concat_datas(তথ্য *d, পূর্ণসংখ্যা n)
{
    পূর্ণসংখ্যা i;
    তথ্য out = {0};
    জন্য(i = 0; i < n; ++i){
        তথ্য new = concat_তথ্য(d[i], out);
        free_তথ্য(out);
        out = new;
    }
    ফেরত out;
}

তথ্য load_categorical_তথ্য_csv(অক্ষর *filename, পূর্ণসংখ্যা target, পূর্ণসংখ্যা k)
{
    তথ্য d = {0};
    d.shallow = 0;
    matrix X = csv_to_matrix(filename);
    দশমিক *truth_1d = বের_করো_column(&X, target);
    দশমিক **truth = one_hot_encode(truth_1d, X.rows, k);
    matrix y;
    y.rows = X.rows;
    y.cols = k;
    y.vals = truth;
    d.X = X;
    d.y = y;
    মুক্ত(truth_1d);
    ফেরত d;
}

তথ্য load_cifar10_তথ্য(অক্ষর *filename)
{
    তথ্য d = {0};
    d.shallow = 0;
    long i,j;
    matrix X = make_matrix(10000, 3072);
    matrix y = make_matrix(10000, 10);
    d.X = X;
    d.y = y;

    FILE *fp = fopen(filename, "rb");
    যদি(!fp) file_error(filename);
    জন্য(i = 0; i < 10000; ++i){
        unsigned অক্ষর bytes[3073];
        fread(bytes, 1, 3073, fp);
        পূর্ণসংখ্যা class = bytes[0];
        y.vals[i][class] = 1;
        জন্য(j = 0; j < X.cols; ++j){
            X.vals[i][j] = (দ্বিগুণ)bytes[j+1];
        }
    }
    scale_তথ্য_rows(d, 1./255);
    //normalize_data_rows(d);
    fclose(fp);
    ফেরত d;
}

শূন্য get_random_batch(তথ্য d, পূর্ণসংখ্যা n, দশমিক *X, দশমিক *y)
{
    পূর্ণসংখ্যা j;
    জন্য(j = 0; j < n; ++j){
        পূর্ণসংখ্যা সূচক = rand()%d.X.rows;
        memcpy(X+j*d.X.cols, d.X.vals[সূচক], d.X.cols*আকার(দশমিক));
        memcpy(y+j*d.y.cols, d.y.vals[সূচক], d.y.cols*আকার(দশমিক));
    }
}

শূন্য get_পরবর্তী_batch(তথ্য d, পূর্ণসংখ্যা n, পূর্ণসংখ্যা offset, দশমিক *X, দশমিক *y)
{
    পূর্ণসংখ্যা j;
    জন্য(j = 0; j < n; ++j){
        পূর্ণসংখ্যা সূচক = offset + j;
        memcpy(X+j*d.X.cols, d.X.vals[সূচক], d.X.cols*আকার(দশমিক));
        যদি(y) memcpy(y+j*d.y.cols, d.y.vals[সূচক], d.y.cols*আকার(দশমিক));
    }
}

শূন্য smooth_তথ্য(তথ্য d)
{
    পূর্ণসংখ্যা i, j;
    দশমিক scale = 1. / d.y.cols;
    দশমিক eps = .1;
    জন্য(i = 0; i < d.y.rows; ++i){
        জন্য(j = 0; j < d.y.cols; ++j){
            d.y.vals[i][j] = eps * scale + (1-eps) * d.y.vals[i][j];
        }
    }
}

তথ্য load_all_cifar10()
{
    তথ্য d = {0};
    d.shallow = 0;
    পূর্ণসংখ্যা i,j,b;
    matrix X = make_matrix(50000, 3072);
    matrix y = make_matrix(50000, 10);
    d.X = X;
    d.y = y;


    জন্য(b = 0; b < 5; ++b){
        অক্ষর buff[256];
        sprintf(buff, "data/cifar/cifar-10-batches-bin/data_batch_%d.bin", b+1);
        FILE *fp = fopen(buff, "rb");
        যদি(!fp) file_error(buff);
        জন্য(i = 0; i < 10000; ++i){
            unsigned অক্ষর bytes[3073];
            fread(bytes, 1, 3073, fp);
            পূর্ণসংখ্যা class = bytes[0];
            y.vals[i+b*10000][class] = 1;
            জন্য(j = 0; j < X.cols; ++j){
                X.vals[i+b*10000][j] = (দ্বিগুণ)bytes[j+1];
            }
        }
        fclose(fp);
    }
    //normalize_data_rows(d);
    scale_তথ্য_rows(d, 1./255);
    smooth_তথ্য(d);
    ফেরত d;
}

তথ্য load_go(অক্ষর *filename)
{
    FILE *fp = fopen(filename, "rb");
    matrix X = make_matrix(3363059, 361);
    matrix y = make_matrix(3363059, 361);
    পূর্ণসংখ্যা row, col;

    যদি(!fp) file_error(filename);
    অক্ষর *label;
    পূর্ণসংখ্যা গুনতি = 0;
    যখন((label = fgetl(fp))){
        পূর্ণসংখ্যা i;
        যদি(গুনতি == X.rows){
            X = resize_matrix(X, গুনতি*2);
            y = resize_matrix(y, গুনতি*2);
        }
        sscanf(label, "%d %d", &row, &col);
        অক্ষর *board = fgetl(fp);

        পূর্ণসংখ্যা সূচক = row*19 + col;
        y.vals[গুনতি][সূচক] = 1;

        জন্য(i = 0; i < 19*19; ++i){
            দশমিক val = 0;
            যদি(board[i] == '1') val = 1;
            অন্যথা যদি(board[i] == '2') val = -1;
            X.vals[গুনতি][i] = val;
        }
        ++গুনতি;
        মুক্ত(label);
        মুক্ত(board);
    }
    X = resize_matrix(X, গুনতি);
    y = resize_matrix(y, গুনতি);

    তথ্য d = {0};
    d.shallow = 0;
    d.X = X;
    d.y = y;


    fclose(fp);

    ফেরত d;
}


শূন্য randomize_তথ্য(তথ্য d)
{
    পূর্ণসংখ্যা i;
    জন্য(i = d.X.rows-1; i > 0; --i){
        পূর্ণসংখ্যা সূচক = rand()%i;
        দশমিক *swap = d.X.vals[সূচক];
        d.X.vals[সূচক] = d.X.vals[i];
        d.X.vals[i] = swap;

        swap = d.y.vals[সূচক];
        d.y.vals[সূচক] = d.y.vals[i];
        d.y.vals[i] = swap;
    }
}

শূন্য scale_তথ্য_rows(তথ্য d, দশমিক s)
{
    পূর্ণসংখ্যা i;
    জন্য(i = 0; i < d.X.rows; ++i){
        scale_array(d.X.vals[i], d.X.cols, s);
    }
}

শূন্য translate_তথ্য_rows(তথ্য d, দশমিক s)
{
    পূর্ণসংখ্যা i;
    জন্য(i = 0; i < d.X.rows; ++i){
        translate_array(d.X.vals[i], d.X.cols, s);
    }
}

তথ্য copy_তথ্য(তথ্য d)
{
    তথ্য c = {0};
    c.w = d.w;
    c.h = d.h;
    c.shallow = 0;
    c.অঙ্ক_boxes = d.অঙ্ক_boxes;
    c.boxes = d.boxes;
    c.X = copy_matrix(d.X);
    c.y = copy_matrix(d.y);
    ফেরত c;
}

শূন্য normalize_তথ্য_rows(তথ্য d)
{
    পূর্ণসংখ্যা i;
    জন্য(i = 0; i < d.X.rows; ++i){
        normalize_array(d.X.vals[i], d.X.cols);
    }
}

তথ্য get_তথ্য_part(তথ্য d, পূর্ণসংখ্যা part, পূর্ণসংখ্যা যোগ_2)
{
    তথ্য p = {0};
    p.shallow = 1;
    p.X.rows = d.X.rows * (part + 1) / যোগ_2 - d.X.rows * part / যোগ_2;
    p.y.rows = d.y.rows * (part + 1) / যোগ_2 - d.y.rows * part / যোগ_2;
    p.X.cols = d.X.cols;
    p.y.cols = d.y.cols;
    p.X.vals = d.X.vals + d.X.rows * part / যোগ_2;
    p.y.vals = d.y.vals + d.y.rows * part / যোগ_2;
    ফেরত p;
}

তথ্য get_random_তথ্য(তথ্য d, পূর্ণসংখ্যা অঙ্ক)
{
    তথ্য r = {0};
    r.shallow = 1;

    r.X.rows = অঙ্ক;
    r.y.rows = অঙ্ক;

    r.X.cols = d.X.cols;
    r.y.cols = d.y.cols;

    r.X.vals = calloc(অঙ্ক, আকার(দশমিক *));
    r.y.vals = calloc(অঙ্ক, আকার(দশমিক *));

    পূর্ণসংখ্যা i;
    জন্য(i = 0; i < অঙ্ক; ++i){
        পূর্ণসংখ্যা সূচক = rand()%d.X.rows;
        r.X.vals[i] = d.X.vals[সূচক];
        r.y.vals[i] = d.y.vals[সূচক];
    }
    ফেরত r;
}

তথ্য *split_তথ্য(তথ্য d, পূর্ণসংখ্যা part, পূর্ণসংখ্যা যোগ_2)
{
    তথ্য *split = calloc(2, আকার(তথ্য));
    পূর্ণসংখ্যা i;
    পূর্ণসংখ্যা start = part*d.X.rows/যোগ_2;
    পূর্ণসংখ্যা end = (part+1)*d.X.rows/যোগ_2;
    তথ্য train;
    তথ্য test;
    train.shallow = test.shallow = 1;

    test.X.rows = test.y.rows = end-start;
    train.X.rows = train.y.rows = d.X.rows - (end-start);
    train.X.cols = test.X.cols = d.X.cols;
    train.y.cols = test.y.cols = d.y.cols;

    train.X.vals = calloc(train.X.rows, আকার(দশমিক*));
    test.X.vals = calloc(test.X.rows, আকার(দশমিক*));
    train.y.vals = calloc(train.y.rows, আকার(দশমিক*));
    test.y.vals = calloc(test.y.rows, আকার(দশমিক*));

    জন্য(i = 0; i < start; ++i){
        train.X.vals[i] = d.X.vals[i];
        train.y.vals[i] = d.y.vals[i];
    }
    জন্য(i = start; i < end; ++i){
        test.X.vals[i-start] = d.X.vals[i];
        test.y.vals[i-start] = d.y.vals[i];
    }
    জন্য(i = end; i < d.X.rows; ++i){
        train.X.vals[i-(end-start)] = d.X.vals[i];
        train.y.vals[i-(end-start)] = d.y.vals[i];
    }
    split[0] = train;
    split[1] = test;
    ফেরত split;
}

