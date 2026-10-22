#include "data.h"
#include "utils.h"
#include "image.h"
#include "cuda.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

list *get_paths(var_na *filename)
{
    var_na *path;
    FILE *file = fopen(filename, "r");
    གལ་ཏེ(!file) file_error(filename);
    list *lines = make_list();
    སྐབས((path=fgetl(file))){
        list_insert(lines, path);
    }
    fclose(file);
    ཕྱིར lines;
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

var_na **get_random_paths(var_na **paths, ཧྲིལ n, ཧྲིལ m)
{
    var_na **random_paths = calloc(n, parimaa_na(var_na*));
    ཧྲིལ i;
    pthread_mutex_lock(&mutex);
    ཆེད་དུ(i = 0; i < n; ++i){
        ཧྲིལ index = rand()%m;
        random_paths[i] = paths[index];
        //if(i == 0) printf("%s\n", paths[index]);
    }
    pthread_mutex_unlock(&mutex);
    ཕྱིར random_paths;
}

var_na **find_replace_paths(var_na **paths, ཧྲིལ n, var_na *find, var_na *replace)
{
    var_na **replace_paths = calloc(n, parimaa_na(var_na*));
    ཧྲིལ i;
    ཆེད་དུ(i = 0; i < n; ++i){
        var_na replaced[4096];
        find_replace(paths[i], find, replace, replaced);
        replace_paths[i] = copy_string(replaced);
    }
    ཕྱིར replace_paths;
}

matrix load_image_paths_gray(var_na **paths, ཧྲིལ n, ཧྲིལ w, ཧྲིལ h)
{
    ཧྲིལ i;
    matrix X;
    X.rows = n;
    X.vals = calloc(X.rows, parimaa_na(float*));
    X.cols = 0;

    ཆེད་དུ(i = 0; i < n; ++i){
        image im = load_image(paths[i], w, h, 3);

        image gray = grayscale_image(im);
        free_image(im);
        im = gray;

        X.vals[i] = im.data;
        X.cols = im.h*im.w*im.c;
    }
    ཕྱིར X;
}

matrix load_image_paths(var_na **paths, ཧྲིལ n, ཧྲིལ w, ཧྲིལ h)
{
    ཧྲིལ i;
    matrix X;
    X.rows = n;
    X.vals = calloc(X.rows, parimaa_na(float*));
    X.cols = 0;

    ཆེད་དུ(i = 0; i < n; ++i){
        image im = load_image_color(paths[i], w, h);
        X.vals[i] = im.data;
        X.cols = im.h*im.w*im.c;
    }
    ཕྱིར X;
}

matrix load_image_augment_paths(var_na **paths, ཧྲིལ n, ཧྲིལ min, ཧྲིལ max, ཧྲིལ size, float angle, float aspect, float hue, float saturation, float exposure, ཧྲིལ center)
{
    ཧྲིལ i;
    matrix X;
    X.rows = n;
    X.vals = calloc(X.rows, parimaa_na(float*));
    X.cols = 0;

    ཆེད་དུ(i = 0; i < n; ++i){
        image im = load_image_color(paths[i], 0, 0);
        image crop;
        གལ་ཏེ(center){
            crop = center_crop_image(im, size, size);
        } དེ་མིན {
            crop = random_augment_image(im, angle, aspect, min, max, size, size);
        }
        ཧྲིལ flip = rand()%2;
        གལ་ཏེ (flip) flip_image(crop);
        random_distort_image(crop, hue, saturation, exposure);

        /*
        show_image(im, "orig");
        show_image(crop, "crop");
        cvWaitKey(0);
        */
        //grayscale_image_3c(crop);
        free_image(im);
        X.vals[i] = crop.data;
        X.cols = crop.h*crop.w*crop.c;
    }
    ཕྱིར X;
}


box_label *read_boxes(var_na *filename, ཧྲིལ *n)
{
    FILE *file = fopen(filename, "r");
    གལ་ཏེ(!file) file_error(filename);
    float x, y, h, w;
    ཧྲིལ id;
    ཧྲིལ count = 0;
    ཧྲིལ size = 64;
    box_label *boxes = calloc(size, parimaa_na(box_label));
    སྐབས(fscanf(file, "%d %f %f %f %f", &id, &x, &y, &w, &h) == 5){
        གལ་ཏེ(count == size) {
            size = size * 2;
            boxes = realloc(boxes, size*parimaa_na(box_label));
        }
        boxes[count].id = id;
        boxes[count].x = x;
        boxes[count].y = y;
        boxes[count].h = h;
        boxes[count].w = w;
        boxes[count].left   = x - w/2;
        boxes[count].right  = x + w/2;
        boxes[count].top    = y - h/2;
        boxes[count].bottom = y + h/2;
        ++count;
    }
    fclose(file);
    *n = count;
    ཕྱིར boxes;
}

void randomize_boxes(box_label *b, ཧྲིལ n)
{
    ཧྲིལ i;
    ཆེད་དུ(i = 0; i < n; ++i){
        box_label swap = b[i];
        ཧྲིལ index = rand()%n;
        b[i] = b[index];
        b[index] = swap;
    }
}

void correct_boxes(box_label *boxes, ཧྲིལ n, float dx, float dy, float sx, float sy, ཧྲིལ flip)
{
    ཧྲིལ i;
    ཆེད་དུ(i = 0; i < n; ++i){
        གལ་ཏེ(boxes[i].x == 0 && boxes[i].y == 0) {
            boxes[i].x = 999999;
            boxes[i].y = 999999;
            boxes[i].w = 999999;
            boxes[i].h = 999999;
            continue;
        }
        boxes[i].left   = boxes[i].left  * sx - dx;
        boxes[i].right  = boxes[i].right * sx - dx;
        boxes[i].top    = boxes[i].top   * sy - dy;
        boxes[i].bottom = boxes[i].bottom* sy - dy;

        གལ་ཏེ(flip){
            float swap = boxes[i].left;
            boxes[i].left = 1. - boxes[i].right;
            boxes[i].right = 1. - swap;
        }

        boxes[i].left =  constrain(0, 1, boxes[i].left);
        boxes[i].right = constrain(0, 1, boxes[i].right);
        boxes[i].top =   constrain(0, 1, boxes[i].top);
        boxes[i].bottom =   constrain(0, 1, boxes[i].bottom);

        boxes[i].x = (boxes[i].left+boxes[i].right)/2;
        boxes[i].y = (boxes[i].top+boxes[i].bottom)/2;
        boxes[i].w = (boxes[i].right - boxes[i].left);
        boxes[i].h = (boxes[i].bottom - boxes[i].top);

        boxes[i].w = constrain(0, 1, boxes[i].w);
        boxes[i].h = constrain(0, 1, boxes[i].h);
    }
}

void fill_truth_swag(var_na *path, float *truth, ཧྲིལ classes, ཧྲིལ flip, float dx, float dy, float sx, float sy)
{
    var_na labelpath[4096];
    find_replace(path, "images", "labels", labelpath);
    find_replace(labelpath, "JPEGImages", "labels", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);

    ཧྲིལ count = 0;
    box_label *boxes = read_boxes(labelpath, &count);
    randomize_boxes(boxes, count);
    correct_boxes(boxes, count, dx, dy, sx, sy, flip);
    float x,y,w,h;
    ཧྲིལ id;
    ཧྲིལ i;

    ཆེད་དུ (i = 0; i < count && i < 90; ++i) {
        x =  boxes[i].x;
        y =  boxes[i].y;
        w =  boxes[i].w;
        h =  boxes[i].h;
        id = boxes[i].id;

        གལ་ཏེ (w < .0 || h < .0) continue;

        ཧྲིལ index = (4+classes) * i;

        truth[index++] = x;
        truth[index++] = y;
        truth[index++] = w;
        truth[index++] = h;

        གལ་ཏེ (id < classes) truth[index+id] = 1;
    }
    free(boxes);
}

void fill_truth_region(var_na *path, float *truth, ཧྲིལ classes, ཧྲིལ num_boxes, ཧྲིལ flip, float dx, float dy, float sx, float sy)
{
    var_na labelpath[4096];
    find_replace(path, "images", "labels", labelpath);
    find_replace(labelpath, "JPEGImages", "labels", labelpath);

    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".png", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    ཧྲིལ count = 0;
    box_label *boxes = read_boxes(labelpath, &count);
    randomize_boxes(boxes, count);
    correct_boxes(boxes, count, dx, dy, sx, sy, flip);
    float x,y,w,h;
    ཧྲིལ id;
    ཧྲིལ i;

    ཆེད་དུ (i = 0; i < count; ++i) {
        x =  boxes[i].x;
        y =  boxes[i].y;
        w =  boxes[i].w;
        h =  boxes[i].h;
        id = boxes[i].id;

        གལ་ཏེ (w < .005 || h < .005) continue;

        ཧྲིལ col = (ཧྲིལ)(x*num_boxes);
        ཧྲིལ row = (ཧྲིལ)(y*num_boxes);

        x = x*num_boxes - col;
        y = y*num_boxes - row;

        ཧྲིལ index = (col+row*num_boxes)*(5+classes);
        གལ་ཏེ (truth[index]) continue;
        truth[index++] = 1;

        གལ་ཏེ (id < classes) truth[index+id] = 1;
        index += classes;

        truth[index++] = x;
        truth[index++] = y;
        truth[index++] = w;
        truth[index++] = h;
    }
    free(boxes);
}

void load_rle(image im, ཧྲིལ *rle, ཧྲིལ n)
{
    ཧྲིལ count = 0;
    ཧྲིལ curr = 0;
    ཧྲིལ i,j;
    ཆེད་དུ(i = 0; i < n; ++i){
        ཆེད་དུ(j = 0; j < rle[i]; ++j){
            im.data[count++] = curr;
        }
        curr = 1 - curr;
    }
    ཆེད་དུ(; count < im.h*im.w*im.c; ++count){
        im.data[count] = curr;
    }
}

void or_image(image src, image dest, ཧྲིལ c)
{
    ཧྲིལ i;
    ཆེད་དུ(i = 0; i < src.w*src.h; ++i){
        གལ་ཏེ(src.data[i]) dest.data[dest.w*dest.h*c + i] = 1;
    }
}

void exclusive_image(image src)
{
    ཧྲིལ k, j, i;
    ཧྲིལ s = src.w*src.h;
    ཆེད་དུ(k = 0; k < src.c-1; ++k){
        ཆེད་དུ(i = 0; i < s; ++i){
            གལ་ཏེ (src.data[k*s + i]){
                ཆེད་དུ(j = k+1; j < src.c; ++j){
                    src.data[j*s + i] = 0;
                }
            }
        }
    }
}

box bound_image(image im)
{
    ཧྲིལ x,y;
    ཧྲིལ minx = im.w;
    ཧྲིལ miny = im.h;
    ཧྲིལ maxx = 0;
    ཧྲིལ maxy = 0;
    ཆེད་དུ(y = 0; y < im.h; ++y){
        ཆེད་དུ(x = 0; x < im.w; ++x){
            གལ་ཏེ(im.data[y*im.w + x]){
                minx = (x < minx) ? x : minx;
                miny = (y < miny) ? y : miny;
                maxx = (x > maxx) ? x : maxx;
                maxy = (y > maxy) ? y : maxy;
            }
        }
    }
    box b = {minx, miny, maxx-minx + 1, maxy-miny + 1};
    //printf("%f %f %f %f\n", b.x, b.y, b.w, b.h);
    ཕྱིར b;
}

void fill_truth_iseg(var_na *path, ཧྲིལ num_boxes, float *truth, ཧྲིལ classes, ཧྲིལ w, ཧྲིལ h, augment_args aug, ཧྲིལ flip, ཧྲིལ mw, ཧྲིལ mh)
{
    var_na labelpath[4096];
    find_replace(path, "images", "mask", labelpath);
    find_replace(labelpath, "JPEGImages", "mask", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    FILE *file = fopen(labelpath, "r");
    གལ་ཏེ(!file) file_error(labelpath);
    var_na buff[32788];
    ཧྲིལ id;
    ཧྲིལ i = 0;
    ཧྲིལ j;
    image part = make_image(w, h, 1);
    སྐབས((fscanf(file, "%d %s", &id, buff) == 2) && i < num_boxes){
        ཧྲིལ n = 0;
        ཧྲིལ *rle = read_intlist(buff, &n, 0);
        load_rle(part, rle, n);
        image sized = rotate_crop_image(part, aug.rad, aug.scale, aug.w, aug.h, aug.dx, aug.dy, aug.aspect);
        གལ་ཏེ(flip) flip_image(sized);

        image mask = resize_image(sized, mw, mh);
        truth[i*(mw*mh+1)] = id;
        ཆེད་དུ(j = 0; j < mw*mh; ++j){
            truth[i*(mw*mh + 1) + 1 + j] = mask.data[j];
        }
        ++i;

        free_image(mask);
        free_image(sized);
        free(rle);
    }
    གལ་ཏེ(i < num_boxes) truth[i*(mw*mh+1)] = -1;
    fclose(file);
    free_image(part);
}

void fill_truth_mask(var_na *path, ཧྲིལ num_boxes, float *truth, ཧྲིལ classes, ཧྲིལ w, ཧྲིལ h, augment_args aug, ཧྲིལ flip, ཧྲིལ mw, ཧྲིལ mh)
{
    var_na labelpath[4096];
    find_replace(path, "images", "mask", labelpath);
    find_replace(labelpath, "JPEGImages", "mask", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    FILE *file = fopen(labelpath, "r");
    གལ་ཏེ(!file) file_error(labelpath);
    var_na buff[32788];
    ཧྲིལ id;
    ཧྲིལ i = 0;
    image part = make_image(w, h, 1);
    སྐབས((fscanf(file, "%d %s", &id, buff) == 2) && i < num_boxes){
        ཧྲིལ n = 0;
        ཧྲིལ *rle = read_intlist(buff, &n, 0);
        load_rle(part, rle, n);
        image sized = rotate_crop_image(part, aug.rad, aug.scale, aug.w, aug.h, aug.dx, aug.dy, aug.aspect);
        གལ་ཏེ(flip) flip_image(sized);
        box b = bound_image(sized);
        གལ་ཏེ(b.w > 0){
            image crop = crop_image(sized, b.x, b.y, b.w, b.h);
            image mask = resize_image(crop, mw, mh);
            truth[i*(4 + mw*mh + 1) + 0] = (b.x + b.w/2.)/sized.w;
            truth[i*(4 + mw*mh + 1) + 1] = (b.y + b.h/2.)/sized.h;
            truth[i*(4 + mw*mh + 1) + 2] = b.w/sized.w;
            truth[i*(4 + mw*mh + 1) + 3] = b.h/sized.h;
            ཧྲིལ j;
            ཆེད་དུ(j = 0; j < mw*mh; ++j){
                truth[i*(4 + mw*mh + 1) + 4 + j] = mask.data[j];
            }
            truth[i*(4 + mw*mh + 1) + 4 + mw*mh] = id;
            free_image(crop);
            free_image(mask);
            ++i;
        }
        free_image(sized);
        free(rle);
    }
    fclose(file);
    free_image(part);
}


void fill_truth_detection(var_na *path, ཧྲིལ num_boxes, float *truth, ཧྲིལ classes, ཧྲིལ flip, float dx, float dy, float sx, float sy)
{
    var_na labelpath[4096];
    find_replace(path, "images", "labels", labelpath);
    find_replace(labelpath, "JPEGImages", "labels", labelpath);

    find_replace(labelpath, "raw", "labels", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".png", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    ཧྲིལ count = 0;
    box_label *boxes = read_boxes(labelpath, &count);
    randomize_boxes(boxes, count);
    correct_boxes(boxes, count, dx, dy, sx, sy, flip);
    གལ་ཏེ(count > num_boxes) count = num_boxes;
    float x,y,w,h;
    ཧྲིལ id;
    ཧྲིལ i;
    ཧྲིལ sub = 0;

    ཆེད་དུ (i = 0; i < count; ++i) {
        x =  boxes[i].x;
        y =  boxes[i].y;
        w =  boxes[i].w;
        h =  boxes[i].h;
        id = boxes[i].id;

        གལ་ཏེ ((w < .001 || h < .001)) {
            ++sub;
            continue;
        }

        truth[(i-sub)*5+0] = x;
        truth[(i-sub)*5+1] = y;
        truth[(i-sub)*5+2] = w;
        truth[(i-sub)*5+3] = h;
        truth[(i-sub)*5+4] = id;
    }
    free(boxes);
}

#define NUMCHARS 37

void print_letters(float *pred, ཧྲིལ n)
{
    ཧྲིལ i;
    ཆེད་དུ(i = 0; i < n; ++i){
        ཧྲིལ index = max_index(pred+i*NUMCHARS, NUMCHARS);
        printf("%c", int_to_alphanum(index));
    }
    printf("\n");
}

void fill_truth_captcha(var_na *path, ཧྲིལ n, float *truth)
{
    var_na *begin = strrchr(path, '/');
    ++begin;
    ཧྲིལ i;
    ཆེད་དུ(i = 0; i < strlen(begin) && i < n && begin[i] != '.'; ++i){
        ཧྲིལ index = alphanum_to_int(begin[i]);
        གལ་ཏེ(index > 35) printf("Bad %c\n", begin[i]);
        truth[i*NUMCHARS+index] = 1;
    }
    ཆེད་དུ(;i < n; ++i){
        truth[i*NUMCHARS + NUMCHARS-1] = 1;
    }
}

data load_data_captcha(var_na **paths, ཧྲིལ n, ཧྲིལ m, ཧྲིལ k, ཧྲིལ w, ཧྲིལ h)
{
    གལ་ཏེ(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;
    d.X = load_image_paths(paths, n, w, h);
    d.y = make_matrix(n, k*NUMCHARS);
    ཧྲིལ i;
    ཆེད་དུ(i = 0; i < n; ++i){
        fill_truth_captcha(paths[i], k, d.y.vals[i]);
    }
    གལ་ཏེ(m) free(paths);
    ཕྱིར d;
}

data load_data_captcha_encode(var_na **paths, ཧྲིལ n, ཧྲིལ m, ཧྲིལ w, ཧྲིལ h)
{
    གལ་ཏེ(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;
    d.X = load_image_paths(paths, n, w, h);
    d.X.cols = 17100;
    d.y = d.X;
    གལ་ཏེ(m) free(paths);
    ཕྱིར d;
}

void fill_truth(var_na *path, var_na **labels, ཧྲིལ k, float *truth)
{
    ཧྲིལ i;
    memset(truth, 0, k*parimaa_na(float));
    ཧྲིལ count = 0;
    ཆེད་དུ(i = 0; i < k; ++i){
        གལ་ཏེ(strstr(path, labels[i])){
            truth[i] = 1;
            ++count;
            //printf("%s %s %d\n", path, labels[i], i);
        }
    }
    གལ་ཏེ(count != 1 && (k != 1 || count != 0)) printf("Too many or too few labels: %d, %s\n", count, path);
}

void fill_hierarchy(float *truth, ཧྲིལ k, tree *hierarchy)
{
    ཧྲིལ j;
    ཆེད་དུ(j = 0; j < k; ++j){
        གལ་ཏེ(truth[j]){
            ཧྲིལ parent = hierarchy->parent[j];
            སྐབས(parent >= 0){
                truth[parent] = 1;
                parent = hierarchy->parent[parent];
            }
        }
    }
    ཧྲིལ i;
    ཧྲིལ count = 0;
    ཆེད་དུ(j = 0; j < hierarchy->groups; ++j){
        //printf("%d\n", count);
        ཧྲིལ mask = 1;
        ཆེད་དུ(i = 0; i < hierarchy->group_size[j]; ++i){
            གལ་ཏེ(truth[count + i]){
                mask = 0;
                bha_nga;
            }
        }
        གལ་ཏེ (mask) {
            ཆེད་དུ(i = 0; i < hierarchy->group_size[j]; ++i){
                truth[count + i] = SECRET_NUM;
            }
        }
        count += hierarchy->group_size[j];
    }
}

matrix load_regression_labels_paths(var_na **paths, ཧྲིལ n, ཧྲིལ k)
{
    matrix y = make_matrix(n, k);
    ཧྲིལ i,j;
    ཆེད་དུ(i = 0; i < n; ++i){
        var_na labelpath[4096];
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
        ཆེད་དུ(j = 0; j < k; ++j){
            fscanf(file, "%f", &(y.vals[i][j]));
        }
        fclose(file);
    }
    ཕྱིར y;
}

matrix load_labels_paths(var_na **paths, ཧྲིལ n, var_na **labels, ཧྲིལ k, tree *hierarchy)
{
    matrix y = make_matrix(n, k);
    ཧྲིལ i;
    ཆེད་དུ(i = 0; i < n && labels; ++i){
        fill_truth(paths[i], labels, k, y.vals[i]);
        གལ་ཏེ(hierarchy){
            fill_hierarchy(y.vals[i], k, hierarchy);
        }
    }
    ཕྱིར y;
}

matrix load_tags_paths(var_na **paths, ཧྲིལ n, ཧྲིལ k)
{
    matrix y = make_matrix(n, k);
    ཧྲིལ i;
    //int count = 0;
    ཆེད་དུ(i = 0; i < n; ++i){
        var_na label[4096];
        find_replace(paths[i], "images", "labels", label);
        find_replace(label, ".jpg", ".txt", label);
        FILE *file = fopen(label, "r");
        གལ་ཏེ (!file) continue;
        //++count;
        ཧྲིལ tag;
        སྐབས(fscanf(file, "%d", &tag) == 1){
            གལ་ཏེ(tag < k){
                y.vals[i][tag] = 1;
            }
        }
        fclose(file);
    }
    //printf("%d/%d\n", count, n);
    ཕྱིར y;
}

var_na **get_labels(var_na *filename)
{
    list *plist = get_paths(filename);
    var_na **labels = (var_na **)list_to_array(plist);
    free_list(plist);
    ཕྱིར labels;
}

void free_data(data d)
{
    གལ་ཏེ(!d.shallow){
        free_matrix(d.X);
        free_matrix(d.y);
    }དེ་མིན{
        free(d.X.vals);
        free(d.y.vals);
    }
}

image get_segmentation_image(var_na *path, ཧྲིལ w, ཧྲིལ h, ཧྲིལ classes)
{
    var_na labelpath[4096];
    find_replace(path, "images", "mask", labelpath);
    find_replace(labelpath, "JPEGImages", "mask", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    image mask = make_image(w, h, classes);
    FILE *file = fopen(labelpath, "r");
    གལ་ཏེ(!file) file_error(labelpath);
    var_na buff[32788];
    ཧྲིལ id;
    image part = make_image(w, h, 1);
    སྐབས(fscanf(file, "%d %s", &id, buff) == 2){
        ཧྲིལ n = 0;
        ཧྲིལ *rle = read_intlist(buff, &n, 0);
        load_rle(part, rle, n);
        or_image(part, mask, id);
        free(rle);
    }
    //exclusive_image(mask);
    fclose(file);
    free_image(part);
    ཕྱིར mask;
}

image get_segmentation_image2(var_na *path, ཧྲིལ w, ཧྲིལ h, ཧྲིལ classes)
{
    var_na labelpath[4096];
    find_replace(path, "images", "mask", labelpath);
    find_replace(labelpath, "JPEGImages", "mask", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    image mask = make_image(w, h, classes+1);
    ཧྲིལ i;
    ཆེད་དུ(i = 0; i < w*h; ++i){
        mask.data[w*h*classes + i] = 1;
    }
    FILE *file = fopen(labelpath, "r");
    གལ་ཏེ(!file) file_error(labelpath);
    var_na buff[32788];
    ཧྲིལ id;
    image part = make_image(w, h, 1);
    སྐབས(fscanf(file, "%d %s", &id, buff) == 2){
        ཧྲིལ n = 0;
        ཧྲིལ *rle = read_intlist(buff, &n, 0);
        load_rle(part, rle, n);
        or_image(part, mask, id);
        ཆེད་དུ(i = 0; i < w*h; ++i){
            གལ་ཏེ(part.data[i]) mask.data[w*h*classes + i] = 0;
        }
        free(rle);
    }
    //exclusive_image(mask);
    fclose(file);
    free_image(part);
    ཕྱིར mask;
}

data load_data_seg(ཧྲིལ n, var_na **paths, ཧྲིལ m, ཧྲིལ w, ཧྲིལ h, ཧྲིལ classes, ཧྲིལ min, ཧྲིལ max, float angle, float aspect, float hue, float saturation, float exposure, ཧྲིལ div)
{
    var_na **random_paths = get_random_paths(paths, n, m);
    ཧྲིལ i;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, parimaa_na(float*));
    d.X.cols = h*w*3;


    d.y.rows = n;
    d.y.cols = h*w*classes/div/div;
    d.y.vals = calloc(d.X.rows, parimaa_na(float*));

    ཆེད་དུ(i = 0; i < n; ++i){
        image orig = load_image_color(random_paths[i], 0, 0);
        augment_args a = random_augment_args(orig, angle, aspect, min, max, w, h);
        image sized = rotate_crop_image(orig, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);

        ཧྲིལ flip = rand()%2;
        གལ་ཏེ(flip) flip_image(sized);
        random_distort_image(sized, hue, saturation, exposure);
        d.X.vals[i] = sized.data;

        image mask = get_segmentation_image(random_paths[i], orig.w, orig.h, classes);
        //image mask = make_image(orig.w, orig.h, classes+1);
        image sized_m = rotate_crop_image(mask, a.rad, a.scale/div, a.w/div, a.h/div, a.dx/div, a.dy/div, a.aspect);

        གལ་ཏེ(flip) flip_image(sized_m);
        d.y.vals[i] = sized_m.data;

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
    free(random_paths);
    ཕྱིར d;
}

data load_data_iseg(ཧྲིལ n, var_na **paths, ཧྲིལ m, ཧྲིལ w, ཧྲིལ h, ཧྲིལ classes, ཧྲིལ boxes, ཧྲིལ div, ཧྲིལ min, ཧྲིལ max, float angle, float aspect, float hue, float saturation, float exposure)
{
    var_na **random_paths = get_random_paths(paths, n, m);
    ཧྲིལ i;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, parimaa_na(float*));
    d.X.cols = h*w*3;

    d.y = make_matrix(n, (((w/div)*(h/div))+1)*boxes);

    ཆེད་དུ(i = 0; i < n; ++i){
        image orig = load_image_color(random_paths[i], 0, 0);
        augment_args a = random_augment_args(orig, angle, aspect, min, max, w, h);
        image sized = rotate_crop_image(orig, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);

        ཧྲིལ flip = rand()%2;
        གལ་ཏེ(flip) flip_image(sized);
        random_distort_image(sized, hue, saturation, exposure);
        d.X.vals[i] = sized.data;
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
    free(random_paths);
    ཕྱིར d;
}

data load_data_mask(ཧྲིལ n, var_na **paths, ཧྲིལ m, ཧྲིལ w, ཧྲིལ h, ཧྲིལ classes, ཧྲིལ boxes, ཧྲིལ coords, ཧྲིལ min, ཧྲིལ max, float angle, float aspect, float hue, float saturation, float exposure)
{
    var_na **random_paths = get_random_paths(paths, n, m);
    ཧྲིལ i;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, parimaa_na(float*));
    d.X.cols = h*w*3;

    d.y = make_matrix(n, (coords+1)*boxes);

    ཆེད་དུ(i = 0; i < n; ++i){
        image orig = load_image_color(random_paths[i], 0, 0);
        augment_args a = random_augment_args(orig, angle, aspect, min, max, w, h);
        image sized = rotate_crop_image(orig, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);

        ཧྲིལ flip = rand()%2;
        གལ་ཏེ(flip) flip_image(sized);
        random_distort_image(sized, hue, saturation, exposure);
        d.X.vals[i] = sized.data;
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
    free(random_paths);
    ཕྱིར d;
}

data load_data_region(ཧྲིལ n, var_na **paths, ཧྲིལ m, ཧྲིལ w, ཧྲིལ h, ཧྲིལ size, ཧྲིལ classes, float jitter, float hue, float saturation, float exposure)
{
    var_na **random_paths = get_random_paths(paths, n, m);
    ཧྲིལ i;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, parimaa_na(float*));
    d.X.cols = h*w*3;


    ཧྲིལ k = size*size*(5+classes);
    d.y = make_matrix(n, k);
    ཆེད་དུ(i = 0; i < n; ++i){
        image orig = load_image_color(random_paths[i], 0, 0);

        ཧྲིལ oh = orig.h;
        ཧྲིལ ow = orig.w;

        ཧྲིལ dw = (ow*jitter);
        ཧྲིལ dh = (oh*jitter);

        ཧྲིལ pleft  = rand_uniform(-dw, dw);
        ཧྲིལ pright = rand_uniform(-dw, dw);
        ཧྲིལ ptop   = rand_uniform(-dh, dh);
        ཧྲིལ pbot   = rand_uniform(-dh, dh);

        ཧྲིལ swidth =  ow - pleft - pright;
        ཧྲིལ sheight = oh - ptop - pbot;

        float sx = (float)swidth  / ow;
        float sy = (float)sheight / oh;

        ཧྲིལ flip = rand()%2;
        image cropped = crop_image(orig, pleft, ptop, swidth, sheight);

        float dx = ((float)pleft/ow)/sx;
        float dy = ((float)ptop /oh)/sy;

        image sized = resize_image(cropped, w, h);
        གལ་ཏེ(flip) flip_image(sized);
        random_distort_image(sized, hue, saturation, exposure);
        d.X.vals[i] = sized.data;

        fill_truth_region(random_paths[i], d.y.vals[i], classes, size, flip, dx, dy, 1./sx, 1./sy);

        free_image(orig);
        free_image(cropped);
    }
    free(random_paths);
    ཕྱིར d;
}

data load_data_compare(ཧྲིལ n, var_na **paths, ཧྲིལ m, ཧྲིལ classes, ཧྲིལ w, ཧྲིལ h)
{
    གལ་ཏེ(m) paths = get_random_paths(paths, 2*n, m);
    ཧྲིལ i,j;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, parimaa_na(float*));
    d.X.cols = h*w*6;

    ཧྲིལ k = 2*(classes);
    d.y = make_matrix(n, k);
    ཆེད་དུ(i = 0; i < n; ++i){
        image im1 = load_image_color(paths[i*2],   w, h);
        image im2 = load_image_color(paths[i*2+1], w, h);

        d.X.vals[i] = calloc(d.X.cols, parimaa_na(float));
        memcpy(d.X.vals[i],         im1.data, h*w*3*parimaa_na(float));
        memcpy(d.X.vals[i] + h*w*3, im2.data, h*w*3*parimaa_na(float));

        ཧྲིལ id;
        float iou;

        var_na imlabel1[4096];
        var_na imlabel2[4096];
        find_replace(paths[i*2],   "imgs", "labels", imlabel1);
        find_replace(imlabel1, "jpg", "txt", imlabel1);
        FILE *fp1 = fopen(imlabel1, "r");

        སྐབས(fscanf(fp1, "%d %f", &id, &iou) == 2){
            གལ་ཏེ (d.y.vals[i][2*id] < iou) d.y.vals[i][2*id] = iou;
        }

        find_replace(paths[i*2+1], "imgs", "labels", imlabel2);
        find_replace(imlabel2, "jpg", "txt", imlabel2);
        FILE *fp2 = fopen(imlabel2, "r");

        སྐབས(fscanf(fp2, "%d %f", &id, &iou) == 2){
            གལ་ཏེ (d.y.vals[i][2*id + 1] < iou) d.y.vals[i][2*id + 1] = iou;
        }

        ཆེད་དུ (j = 0; j < classes; ++j){
            གལ་ཏེ (d.y.vals[i][2*j] > .5 &&  d.y.vals[i][2*j+1] < .5){
                d.y.vals[i][2*j] = 1;
                d.y.vals[i][2*j+1] = 0;
            } དེ་མིན གལ་ཏེ (d.y.vals[i][2*j] < .5 &&  d.y.vals[i][2*j+1] > .5){
                d.y.vals[i][2*j] = 0;
                d.y.vals[i][2*j+1] = 1;
            } དེ་མིན {
                d.y.vals[i][2*j]   = SECRET_NUM;
                d.y.vals[i][2*j+1] = SECRET_NUM;
            }
        }
        fclose(fp1);
        fclose(fp2);

        free_image(im1);
        free_image(im2);
    }
    གལ་ཏེ(m) free(paths);
    ཕྱིར d;
}

data load_data_swag(var_na **paths, ཧྲིལ n, ཧྲིལ classes, float jitter)
{
    ཧྲིལ index = rand()%n;
    var_na *random_path = paths[index];

    image orig = load_image_color(random_path, 0, 0);
    ཧྲིལ h = orig.h;
    ཧྲིལ w = orig.w;

    data d = {0};
    d.shallow = 0;
    d.w = w;
    d.h = h;

    d.X.rows = 1;
    d.X.vals = calloc(d.X.rows, parimaa_na(float*));
    d.X.cols = h*w*3;

    ཧྲིལ k = (4+classes)*90;
    d.y = make_matrix(1, k);

    ཧྲིལ dw = w*jitter;
    ཧྲིལ dh = h*jitter;

    ཧྲིལ pleft  = rand_uniform(-dw, dw);
    ཧྲིལ pright = rand_uniform(-dw, dw);
    ཧྲིལ ptop   = rand_uniform(-dh, dh);
    ཧྲིལ pbot   = rand_uniform(-dh, dh);

    ཧྲིལ swidth =  w - pleft - pright;
    ཧྲིལ sheight = h - ptop - pbot;

    float sx = (float)swidth  / w;
    float sy = (float)sheight / h;

    ཧྲིལ flip = rand()%2;
    image cropped = crop_image(orig, pleft, ptop, swidth, sheight);

    float dx = ((float)pleft/w)/sx;
    float dy = ((float)ptop /h)/sy;

    image sized = resize_image(cropped, w, h);
    གལ་ཏེ(flip) flip_image(sized);
    d.X.vals[0] = sized.data;

    fill_truth_swag(random_path, d.y.vals[0], classes, flip, dx, dy, 1./sx, 1./sy);

    free_image(orig);
    free_image(cropped);

    ཕྱིར d;
}

data load_data_detection(ཧྲིལ n, var_na **paths, ཧྲིལ m, ཧྲིལ w, ཧྲིལ h, ཧྲིལ boxes, ཧྲིལ classes, float jitter, float hue, float saturation, float exposure)
{
    var_na **random_paths = get_random_paths(paths, n, m);
    ཧྲིལ i;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, parimaa_na(float*));
    d.X.cols = h*w*3;

    d.y = make_matrix(n, 5*boxes);
    ཆེད་དུ(i = 0; i < n; ++i){
        image orig = load_image_color(random_paths[i], 0, 0);
        image sized = make_image(w, h, orig.c);
        fill_image(sized, .5);

        float dw = jitter * orig.w;
        float dh = jitter * orig.h;

        float new_ar = (orig.w + rand_uniform(-dw, dw)) / (orig.h + rand_uniform(-dh, dh));
        //float scale = rand_uniform(.25, 2);
        float scale = 1;

        float nw, nh;

        གལ་ཏེ(new_ar < 1){
            nh = scale * h;
            nw = nh * new_ar;
        } དེ་མིན {
            nw = scale * w;
            nh = nw / new_ar;
        }

        float dx = rand_uniform(0, w - nw);
        float dy = rand_uniform(0, h - nh);

        place_image(orig, nw, nh, dx, dy, sized);

        random_distort_image(sized, hue, saturation, exposure);

        ཧྲིལ flip = rand()%2;
        གལ་ཏེ(flip) flip_image(sized);
        d.X.vals[i] = sized.data;


        fill_truth_detection(random_paths[i], boxes, d.y.vals[i], classes, flip, -dx/w, -dy/h, nw/w, nh/h);

        free_image(orig);
    }
    free(random_paths);
    ཕྱིར d;
}

void *load_thread(void *ptr)
{
    //printf("Loading data: %d\n", rand());
    load_args a = *(sa_nracanaa load_args*)ptr;
    གལ་ཏེ(a.exposure == 0) a.exposure = 1;
    གལ་ཏེ(a.saturation == 0) a.saturation = 1;
    གལ་ཏེ(a.aspect == 0) a.aspect = 1;

    གལ་ཏེ (a.type == OLD_CLASSIFICATION_DATA){
        *a.d = load_data_old(a.paths, a.n, a.m, a.labels, a.classes, a.w, a.h);
    } དེ་མིན གལ་ཏེ (a.type == REGRESSION_DATA){
        *a.d = load_data_regression(a.paths, a.n, a.m, a.classes, a.min, a.max, a.size, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    } དེ་མིན གལ་ཏེ (a.type == CLASSIFICATION_DATA){
        *a.d = load_data_augment(a.paths, a.n, a.m, a.labels, a.classes, a.hierarchy, a.min, a.max, a.size, a.angle, a.aspect, a.hue, a.saturation, a.exposure, a.center);
    } དེ་མིན གལ་ཏེ (a.type == SUPER_DATA){
        *a.d = load_data_super(a.paths, a.n, a.m, a.w, a.h, a.scale);
    } དེ་མིན གལ་ཏེ (a.type == WRITING_DATA){
        *a.d = load_data_writing(a.paths, a.n, a.m, a.w, a.h, a.out_w, a.out_h);
    } དེ་མིན གལ་ཏེ (a.type == ISEG_DATA){
        *a.d = load_data_iseg(a.n, a.paths, a.m, a.w, a.h, a.classes, a.num_boxes, a.scale, a.min, a.max, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    } དེ་མིན གལ་ཏེ (a.type == INSTANCE_DATA){
        *a.d = load_data_mask(a.n, a.paths, a.m, a.w, a.h, a.classes, a.num_boxes, a.coords, a.min, a.max, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    } དེ་མིན གལ་ཏེ (a.type == SEGMENTATION_DATA){
        *a.d = load_data_seg(a.n, a.paths, a.m, a.w, a.h, a.classes, a.min, a.max, a.angle, a.aspect, a.hue, a.saturation, a.exposure, a.scale);
    } དེ་མིན གལ་ཏེ (a.type == REGION_DATA){
        *a.d = load_data_region(a.n, a.paths, a.m, a.w, a.h, a.num_boxes, a.classes, a.jitter, a.hue, a.saturation, a.exposure);
    } དེ་མིན གལ་ཏེ (a.type == DETECTION_DATA){
        *a.d = load_data_detection(a.n, a.paths, a.m, a.w, a.h, a.num_boxes, a.classes, a.jitter, a.hue, a.saturation, a.exposure);
    } དེ་མིན གལ་ཏེ (a.type == SWAG_DATA){
        *a.d = load_data_swag(a.paths, a.n, a.classes, a.jitter);
    } དེ་མིན གལ་ཏེ (a.type == COMPARE_DATA){
        *a.d = load_data_compare(a.n, a.paths, a.m, a.classes, a.w, a.h);
    } དེ་མིན གལ་ཏེ (a.type == IMAGE_DATA){
        *(a.im) = load_image_color(a.path, 0, 0);
        *(a.resized) = resize_image(*(a.im), a.w, a.h);
    } དེ་མིན གལ་ཏེ (a.type == LETTERBOX_DATA){
        *(a.im) = load_image_color(a.path, 0, 0);
        *(a.resized) = letterbox_image(*(a.im), a.w, a.h);
    } དེ་མིན གལ་ཏེ (a.type == TAG_DATA){
        *a.d = load_data_tag(a.paths, a.n, a.m, a.classes, a.min, a.max, a.size, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    }
    free(ptr);
    ཕྱིར 0;
}

pthread_t load_data_in_thread(load_args args)
{
    pthread_t thread;
    sa_nracanaa load_args *ptr = calloc(1, parimaa_na(sa_nracanaa load_args));
    *ptr = args;
    གལ་ཏེ(pthread_create(&thread, 0, load_thread, ptr)) error("Thread creation failed");
    ཕྱིར thread;
}

void *load_threads(void *ptr)
{
    ཧྲིལ i;
    load_args args = *(load_args *)ptr;
    གལ་ཏེ (args.threads == 0) args.threads = 1;
    data *out = args.d;
    ཧྲིལ total = args.n;
    free(ptr);
    data *buffers = calloc(args.threads, parimaa_na(data));
    pthread_t *threads = calloc(args.threads, parimaa_na(pthread_t));
    ཆེད་དུ(i = 0; i < args.threads; ++i){
        args.d = buffers + i;
        args.n = (i+1) * total/args.threads - i * total/args.threads;
        threads[i] = load_data_in_thread(args);
    }
    ཆེད་དུ(i = 0; i < args.threads; ++i){
        pthread_join(threads[i], 0);
    }
    *out = concat_datas(buffers, args.threads);
    out->shallow = 0;
    ཆེད་དུ(i = 0; i < args.threads; ++i){
        buffers[i].shallow = 1;
        free_data(buffers[i]);
    }
    free(buffers);
    free(threads);
    ཕྱིར 0;
}

void load_data_blocking(load_args args)
{
    sa_nracanaa load_args *ptr = calloc(1, parimaa_na(sa_nracanaa load_args));
    *ptr = args;
    load_thread(ptr);
}

pthread_t load_data(load_args args)
{
    pthread_t thread;
    sa_nracanaa load_args *ptr = calloc(1, parimaa_na(sa_nracanaa load_args));
    *ptr = args;
    གལ་ཏེ(pthread_create(&thread, 0, load_threads, ptr)) error("Thread creation failed");
    ཕྱིར thread;
}

data load_data_writing(var_na **paths, ཧྲིལ n, ཧྲིལ m, ཧྲིལ w, ཧྲིལ h, ཧྲིལ out_w, ཧྲིལ out_h)
{
    གལ་ཏེ(m) paths = get_random_paths(paths, n, m);
    var_na **replace_paths = find_replace_paths(paths, n, ".png", "-label.png");
    data d = {0};
    d.shallow = 0;
    d.X = load_image_paths(paths, n, w, h);
    d.y = load_image_paths_gray(replace_paths, n, out_w, out_h);
    གལ་ཏེ(m) free(paths);
    ཧྲིལ i;
    ཆེད་དུ(i = 0; i < n; ++i) free(replace_paths[i]);
    free(replace_paths);
    ཕྱིར d;
}

data load_data_old(var_na **paths, ཧྲིལ n, ཧྲིལ m, var_na **labels, ཧྲིལ k, ཧྲིལ w, ཧྲིལ h)
{
    གལ་ཏེ(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;
    d.X = load_image_paths(paths, n, w, h);
    d.y = load_labels_paths(paths, n, labels, k, 0);
    གལ་ཏེ(m) free(paths);
    ཕྱིར d;
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

data load_data_super(var_na **paths, ཧྲིལ n, ཧྲིལ m, ཧྲིལ w, ཧྲིལ h, ཧྲིལ scale)
{
    གལ་ཏེ(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;

    ཧྲིལ i;
    d.X.rows = n;
    d.X.vals = calloc(n, parimaa_na(float*));
    d.X.cols = w*h*3;

    d.y.rows = n;
    d.y.vals = calloc(n, parimaa_na(float*));
    d.y.cols = w*scale * h*scale * 3;

    ཆེད་དུ(i = 0; i < n; ++i){
        image im = load_image_color(paths[i], 0, 0);
        image crop = random_crop_image(im, w*scale, h*scale);
        ཧྲིལ flip = rand()%2;
        གལ་ཏེ (flip) flip_image(crop);
        image resize = resize_image(crop, w, h);
        d.X.vals[i] = resize.data;
        d.y.vals[i] = crop.data;
        free_image(im);
    }

    གལ་ཏེ(m) free(paths);
    ཕྱིར d;
}

data load_data_regression(var_na **paths, ཧྲིལ n, ཧྲིལ m, ཧྲིལ k, ཧྲིལ min, ཧྲིལ max, ཧྲིལ size, float angle, float aspect, float hue, float saturation, float exposure)
{
    གལ་ཏེ(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;
    d.X = load_image_augment_paths(paths, n, min, max, size, angle, aspect, hue, saturation, exposure, 0);
    d.y = load_regression_labels_paths(paths, n, k);
    གལ་ཏེ(m) free(paths);
    ཕྱིར d;
}

data select_data(data *orig, ཧྲིལ *inds)
{
    data d = {0};
    d.shallow = 1;
    d.w = orig[0].w;
    d.h = orig[0].h;

    d.X.rows = orig[0].X.rows;
    d.y.rows = orig[0].X.rows;

    d.X.cols = orig[0].X.cols;
    d.y.cols = orig[0].y.cols;

    d.X.vals = calloc(orig[0].X.rows, parimaa_na(float *));
    d.y.vals = calloc(orig[0].y.rows, parimaa_na(float *));
    ཧྲིལ i;
    ཆེད་དུ(i = 0; i < d.X.rows; ++i){
        d.X.vals[i] = orig[inds[i]].X.vals[i];
        d.y.vals[i] = orig[inds[i]].y.vals[i];
    }
    ཕྱིར d;
}

data *tile_data(data orig, ཧྲིལ divs, ཧྲིལ size)
{
    data *ds = calloc(divs*divs, parimaa_na(data));
    ཧྲིལ i, j;
#pragma omp parallel ཆེད་དུ
    ཆེད་དུ(i = 0; i < divs*divs; ++i){
        data d;
        d.shallow = 0;
        d.w = orig.w/divs * size;
        d.h = orig.h/divs * size;
        d.X.rows = orig.X.rows;
        d.X.cols = d.w*d.h*3;
        d.X.vals = calloc(d.X.rows, parimaa_na(float*));

        d.y = copy_matrix(orig.y);
#pragma omp parallel ཆེད་དུ
        ཆེད་དུ(j = 0; j < orig.X.rows; ++j){
            ཧྲིལ x = (i%divs) * orig.w / divs - (d.w - orig.w/divs)/2;
            ཧྲིལ y = (i/divs) * orig.h / divs - (d.h - orig.h/divs)/2;
            image im = float_to_image(orig.w, orig.h, 3, orig.X.vals[j]);
            d.X.vals[j] = crop_image(im, x, y, d.w, d.h).data;
        }
        ds[i] = d;
    }
    ཕྱིར ds;
}

data resize_data(data orig, ཧྲིལ w, ཧྲིལ h)
{
    data d = {0};
    d.shallow = 0;
    d.w = w;
    d.h = h;
    ཧྲིལ i;
    d.X.rows = orig.X.rows;
    d.X.cols = w*h*3;
    d.X.vals = calloc(d.X.rows, parimaa_na(float*));

    d.y = copy_matrix(orig.y);
#pragma omp parallel ཆེད་དུ
    ཆེད་དུ(i = 0; i < orig.X.rows; ++i){
        image im = float_to_image(orig.w, orig.h, 3, orig.X.vals[i]);
        d.X.vals[i] = resize_image(im, w, h).data;
    }
    ཕྱིར d;
}

data load_data_augment(var_na **paths, ཧྲིལ n, ཧྲིལ m, var_na **labels, ཧྲིལ k, tree *hierarchy, ཧྲིལ min, ཧྲིལ max, ཧྲིལ size, float angle, float aspect, float hue, float saturation, float exposure, ཧྲིལ center)
{
    གལ་ཏེ(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;
    d.w=size;
    d.h=size;
    d.X = load_image_augment_paths(paths, n, min, max, size, angle, aspect, hue, saturation, exposure, center);
    d.y = load_labels_paths(paths, n, labels, k, hierarchy);
    གལ་ཏེ(m) free(paths);
    ཕྱིར d;
}

data load_data_tag(var_na **paths, ཧྲིལ n, ཧྲིལ m, ཧྲིལ k, ཧྲིལ min, ཧྲིལ max, ཧྲིལ size, float angle, float aspect, float hue, float saturation, float exposure)
{
    གལ་ཏེ(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.w = size;
    d.h = size;
    d.shallow = 0;
    d.X = load_image_augment_paths(paths, n, min, max, size, angle, aspect, hue, saturation, exposure, 0);
    d.y = load_tags_paths(paths, n, k);
    གལ་ཏེ(m) free(paths);
    ཕྱིར d;
}

matrix concat_matrix(matrix m1, matrix m2)
{
    ཧྲིལ i, count = 0;
    matrix m;
    m.cols = m1.cols;
    m.rows = m1.rows+m2.rows;
    m.vals = calloc(m1.rows + m2.rows, parimaa_na(float*));
    ཆེད་དུ(i = 0; i < m1.rows; ++i){
        m.vals[count++] = m1.vals[i];
    }
    ཆེད་དུ(i = 0; i < m2.rows; ++i){
        m.vals[count++] = m2.vals[i];
    }
    ཕྱིར m;
}

data concat_data(data d1, data d2)
{
    data d = {0};
    d.shallow = 1;
    d.X = concat_matrix(d1.X, d2.X);
    d.y = concat_matrix(d1.y, d2.y);
    d.w = d1.w;
    d.h = d1.h;
    ཕྱིར d;
}

data concat_datas(data *d, ཧྲིལ n)
{
    ཧྲིལ i;
    data out = {0};
    ཆེད་དུ(i = 0; i < n; ++i){
        data new = concat_data(d[i], out);
        free_data(out);
        out = new;
    }
    ཕྱིར out;
}

data load_categorical_data_csv(var_na *filename, ཧྲིལ target, ཧྲིལ k)
{
    data d = {0};
    d.shallow = 0;
    matrix X = csv_to_matrix(filename);
    float *truth_1d = pop_column(&X, target);
    float **truth = one_hot_encode(truth_1d, X.rows, k);
    matrix y;
    y.rows = X.rows;
    y.cols = k;
    y.vals = truth;
    d.X = X;
    d.y = y;
    free(truth_1d);
    ཕྱིར d;
}

data load_cifar10_data(var_na *filename)
{
    data d = {0};
    d.shallow = 0;
    long i,j;
    matrix X = make_matrix(10000, 3072);
    matrix y = make_matrix(10000, 10);
    d.X = X;
    d.y = y;

    FILE *fp = fopen(filename, "rb");
    གལ་ཏེ(!fp) file_error(filename);
    ཆེད་དུ(i = 0; i < 10000; ++i){
        unsigned var_na bytes[3073];
        fread(bytes, 1, 3073, fp);
        ཧྲིལ class = bytes[0];
        y.vals[i][class] = 1;
        ཆེད་དུ(j = 0; j < X.cols; ++j){
            X.vals[i][j] = (dviguu_na)bytes[j+1];
        }
    }
    scale_data_rows(d, 1./255);
    //normalize_data_rows(d);
    fclose(fp);
    ཕྱིར d;
}

void get_random_batch(data d, ཧྲིལ n, float *X, float *y)
{
    ཧྲིལ j;
    ཆེད་དུ(j = 0; j < n; ++j){
        ཧྲིལ index = rand()%d.X.rows;
        memcpy(X+j*d.X.cols, d.X.vals[index], d.X.cols*parimaa_na(float));
        memcpy(y+j*d.y.cols, d.y.vals[index], d.y.cols*parimaa_na(float));
    }
}

void get_next_batch(data d, ཧྲིལ n, ཧྲིལ offset, float *X, float *y)
{
    ཧྲིལ j;
    ཆེད་དུ(j = 0; j < n; ++j){
        ཧྲིལ index = offset + j;
        memcpy(X+j*d.X.cols, d.X.vals[index], d.X.cols*parimaa_na(float));
        གལ་ཏེ(y) memcpy(y+j*d.y.cols, d.y.vals[index], d.y.cols*parimaa_na(float));
    }
}

void smooth_data(data d)
{
    ཧྲིལ i, j;
    float scale = 1. / d.y.cols;
    float eps = .1;
    ཆེད་དུ(i = 0; i < d.y.rows; ++i){
        ཆེད་དུ(j = 0; j < d.y.cols; ++j){
            d.y.vals[i][j] = eps * scale + (1-eps) * d.y.vals[i][j];
        }
    }
}

data load_all_cifar10()
{
    data d = {0};
    d.shallow = 0;
    ཧྲིལ i,j,b;
    matrix X = make_matrix(50000, 3072);
    matrix y = make_matrix(50000, 10);
    d.X = X;
    d.y = y;


    ཆེད་དུ(b = 0; b < 5; ++b){
        var_na buff[256];
        sprintf(buff, "data/cifar/cifar-10-batches-bin/data_batch_%d.bin", b+1);
        FILE *fp = fopen(buff, "rb");
        གལ་ཏེ(!fp) file_error(buff);
        ཆེད་དུ(i = 0; i < 10000; ++i){
            unsigned var_na bytes[3073];
            fread(bytes, 1, 3073, fp);
            ཧྲིལ class = bytes[0];
            y.vals[i+b*10000][class] = 1;
            ཆེད་དུ(j = 0; j < X.cols; ++j){
                X.vals[i+b*10000][j] = (dviguu_na)bytes[j+1];
            }
        }
        fclose(fp);
    }
    //normalize_data_rows(d);
    scale_data_rows(d, 1./255);
    smooth_data(d);
    ཕྱིར d;
}

data load_go(var_na *filename)
{
    FILE *fp = fopen(filename, "rb");
    matrix X = make_matrix(3363059, 361);
    matrix y = make_matrix(3363059, 361);
    ཧྲིལ row, col;

    གལ་ཏེ(!fp) file_error(filename);
    var_na *label;
    ཧྲིལ count = 0;
    སྐབས((label = fgetl(fp))){
        ཧྲིལ i;
        གལ་ཏེ(count == X.rows){
            X = resize_matrix(X, count*2);
            y = resize_matrix(y, count*2);
        }
        sscanf(label, "%d %d", &row, &col);
        var_na *board = fgetl(fp);

        ཧྲིལ index = row*19 + col;
        y.vals[count][index] = 1;

        ཆེད་དུ(i = 0; i < 19*19; ++i){
            float val = 0;
            གལ་ཏེ(board[i] == '1') val = 1;
            དེ་མིན གལ་ཏེ(board[i] == '2') val = -1;
            X.vals[count][i] = val;
        }
        ++count;
        free(label);
        free(board);
    }
    X = resize_matrix(X, count);
    y = resize_matrix(y, count);

    data d = {0};
    d.shallow = 0;
    d.X = X;
    d.y = y;


    fclose(fp);

    ཕྱིར d;
}


void randomize_data(data d)
{
    ཧྲིལ i;
    ཆེད་དུ(i = d.X.rows-1; i > 0; --i){
        ཧྲིལ index = rand()%i;
        float *swap = d.X.vals[index];
        d.X.vals[index] = d.X.vals[i];
        d.X.vals[i] = swap;

        swap = d.y.vals[index];
        d.y.vals[index] = d.y.vals[i];
        d.y.vals[i] = swap;
    }
}

void scale_data_rows(data d, float s)
{
    ཧྲིལ i;
    ཆེད་དུ(i = 0; i < d.X.rows; ++i){
        scale_array(d.X.vals[i], d.X.cols, s);
    }
}

void translate_data_rows(data d, float s)
{
    ཧྲིལ i;
    ཆེད་དུ(i = 0; i < d.X.rows; ++i){
        translate_array(d.X.vals[i], d.X.cols, s);
    }
}

data copy_data(data d)
{
    data c = {0};
    c.w = d.w;
    c.h = d.h;
    c.shallow = 0;
    c.num_boxes = d.num_boxes;
    c.boxes = d.boxes;
    c.X = copy_matrix(d.X);
    c.y = copy_matrix(d.y);
    ཕྱིར c;
}

void normalize_data_rows(data d)
{
    ཧྲིལ i;
    ཆེད་དུ(i = 0; i < d.X.rows; ++i){
        normalize_array(d.X.vals[i], d.X.cols);
    }
}

data get_data_part(data d, ཧྲིལ part, ཧྲིལ total)
{
    data p = {0};
    p.shallow = 1;
    p.X.rows = d.X.rows * (part + 1) / total - d.X.rows * part / total;
    p.y.rows = d.y.rows * (part + 1) / total - d.y.rows * part / total;
    p.X.cols = d.X.cols;
    p.y.cols = d.y.cols;
    p.X.vals = d.X.vals + d.X.rows * part / total;
    p.y.vals = d.y.vals + d.y.rows * part / total;
    ཕྱིར p;
}

data get_random_data(data d, ཧྲིལ num)
{
    data r = {0};
    r.shallow = 1;

    r.X.rows = num;
    r.y.rows = num;

    r.X.cols = d.X.cols;
    r.y.cols = d.y.cols;

    r.X.vals = calloc(num, parimaa_na(float *));
    r.y.vals = calloc(num, parimaa_na(float *));

    ཧྲིལ i;
    ཆེད་དུ(i = 0; i < num; ++i){
        ཧྲིལ index = rand()%d.X.rows;
        r.X.vals[i] = d.X.vals[index];
        r.y.vals[i] = d.y.vals[index];
    }
    ཕྱིར r;
}

data *split_data(data d, ཧྲིལ part, ཧྲིལ total)
{
    data *split = calloc(2, parimaa_na(data));
    ཧྲིལ i;
    ཧྲིལ start = part*d.X.rows/total;
    ཧྲིལ end = (part+1)*d.X.rows/total;
    data train;
    data test;
    train.shallow = test.shallow = 1;

    test.X.rows = test.y.rows = end-start;
    train.X.rows = train.y.rows = d.X.rows - (end-start);
    train.X.cols = test.X.cols = d.X.cols;
    train.y.cols = test.y.cols = d.y.cols;

    train.X.vals = calloc(train.X.rows, parimaa_na(float*));
    test.X.vals = calloc(test.X.rows, parimaa_na(float*));
    train.y.vals = calloc(train.y.rows, parimaa_na(float*));
    test.y.vals = calloc(test.y.rows, parimaa_na(float*));

    ཆེད་དུ(i = 0; i < start; ++i){
        train.X.vals[i] = d.X.vals[i];
        train.y.vals[i] = d.y.vals[i];
    }
    ཆེད་དུ(i = start; i < end; ++i){
        test.X.vals[i-start] = d.X.vals[i];
        test.y.vals[i-start] = d.y.vals[i];
    }
    ཆེད་དུ(i = end; i < d.X.rows; ++i){
        train.X.vals[i-(end-start)] = d.X.vals[i];
        train.y.vals[i-(end-start)] = d.y.vals[i];
    }
    split[0] = train;
    split[1] = test;
    ཕྱིར split;
}

