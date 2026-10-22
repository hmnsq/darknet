#include "data.h"
#include "utils.h"
#include "image.h"
#include "cuda.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

list *get_paths(char *filename)
{
    char *path;
    FILE *file = fopen(filename, "r");
    만약(!file) file_error(filename);
    list *lines = make_list();
    동안((path=fgetl(file))){
        list_insert(lines, path);
    }
    fclose(file);
    반환 lines;
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

char **get_random_paths(char **paths, 정수 n, 정수 m)
{
    char **random_paths = calloc(n, sizeof(char*));
    정수 i;
    pthread_mutex_lock(&mutex);
    위해(i = 0; i < n; ++i){
        정수 index = rand()%m;
        random_paths[i] = paths[index];
        //if(i == 0) printf("%s\n", paths[index]);
    }
    pthread_mutex_unlock(&mutex);
    반환 random_paths;
}

char **find_replace_paths(char **paths, 정수 n, char *find, char *replace)
{
    char **replace_paths = calloc(n, sizeof(char*));
    정수 i;
    위해(i = 0; i < n; ++i){
        char replaced[4096];
        find_replace(paths[i], find, replace, replaced);
        replace_paths[i] = copy_string(replaced);
    }
    반환 replace_paths;
}

matrix load_image_paths_gray(char **paths, 정수 n, 정수 w, 정수 h)
{
    정수 i;
    matrix X;
    X.rows = n;
    X.vals = calloc(X.rows, sizeof(float*));
    X.cols = 0;

    위해(i = 0; i < n; ++i){
        image im = load_image(paths[i], w, h, 3);

        image gray = grayscale_image(im);
        free_image(im);
        im = gray;

        X.vals[i] = im.data;
        X.cols = im.h*im.w*im.c;
    }
    반환 X;
}

matrix load_image_paths(char **paths, 정수 n, 정수 w, 정수 h)
{
    정수 i;
    matrix X;
    X.rows = n;
    X.vals = calloc(X.rows, sizeof(float*));
    X.cols = 0;

    위해(i = 0; i < n; ++i){
        image im = load_image_color(paths[i], w, h);
        X.vals[i] = im.data;
        X.cols = im.h*im.w*im.c;
    }
    반환 X;
}

matrix load_image_augment_paths(char **paths, 정수 n, 정수 min, 정수 max, 정수 size, float angle, float aspect, float hue, float saturation, float exposure, 정수 center)
{
    정수 i;
    matrix X;
    X.rows = n;
    X.vals = calloc(X.rows, sizeof(float*));
    X.cols = 0;

    위해(i = 0; i < n; ++i){
        image im = load_image_color(paths[i], 0, 0);
        image crop;
        만약(center){
            crop = center_crop_image(im, size, size);
        } 아니면 {
            crop = random_augment_image(im, angle, aspect, min, max, size, size);
        }
        정수 flip = rand()%2;
        만약 (flip) flip_image(crop);
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
    반환 X;
}


box_label *read_boxes(char *filename, 정수 *n)
{
    FILE *file = fopen(filename, "r");
    만약(!file) file_error(filename);
    float x, y, h, w;
    정수 id;
    정수 count = 0;
    정수 size = 64;
    box_label *boxes = calloc(size, sizeof(box_label));
    동안(fscanf(file, "%d %f %f %f %f", &id, &x, &y, &w, &h) == 5){
        만약(count == size) {
            size = size * 2;
            boxes = realloc(boxes, size*sizeof(box_label));
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
    반환 boxes;
}

void randomize_boxes(box_label *b, 정수 n)
{
    정수 i;
    위해(i = 0; i < n; ++i){
        box_label swap = b[i];
        정수 index = rand()%n;
        b[i] = b[index];
        b[index] = swap;
    }
}

void correct_boxes(box_label *boxes, 정수 n, float dx, float dy, float sx, float sy, 정수 flip)
{
    정수 i;
    위해(i = 0; i < n; ++i){
        만약(boxes[i].x == 0 && boxes[i].y == 0) {
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

        만약(flip){
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

void fill_truth_swag(char *path, float *truth, 정수 classes, 정수 flip, float dx, float dy, float sx, float sy)
{
    char labelpath[4096];
    find_replace(path, "images", "labels", labelpath);
    find_replace(labelpath, "JPEGImages", "labels", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);

    정수 count = 0;
    box_label *boxes = read_boxes(labelpath, &count);
    randomize_boxes(boxes, count);
    correct_boxes(boxes, count, dx, dy, sx, sy, flip);
    float x,y,w,h;
    정수 id;
    정수 i;

    위해 (i = 0; i < count && i < 90; ++i) {
        x =  boxes[i].x;
        y =  boxes[i].y;
        w =  boxes[i].w;
        h =  boxes[i].h;
        id = boxes[i].id;

        만약 (w < .0 || h < .0) continue;

        정수 index = (4+classes) * i;

        truth[index++] = x;
        truth[index++] = y;
        truth[index++] = w;
        truth[index++] = h;

        만약 (id < classes) truth[index+id] = 1;
    }
    free(boxes);
}

void fill_truth_region(char *path, float *truth, 정수 classes, 정수 num_boxes, 정수 flip, float dx, float dy, float sx, float sy)
{
    char labelpath[4096];
    find_replace(path, "images", "labels", labelpath);
    find_replace(labelpath, "JPEGImages", "labels", labelpath);

    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".png", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    정수 count = 0;
    box_label *boxes = read_boxes(labelpath, &count);
    randomize_boxes(boxes, count);
    correct_boxes(boxes, count, dx, dy, sx, sy, flip);
    float x,y,w,h;
    정수 id;
    정수 i;

    위해 (i = 0; i < count; ++i) {
        x =  boxes[i].x;
        y =  boxes[i].y;
        w =  boxes[i].w;
        h =  boxes[i].h;
        id = boxes[i].id;

        만약 (w < .005 || h < .005) continue;

        정수 col = (정수)(x*num_boxes);
        정수 row = (정수)(y*num_boxes);

        x = x*num_boxes - col;
        y = y*num_boxes - row;

        정수 index = (col+row*num_boxes)*(5+classes);
        만약 (truth[index]) continue;
        truth[index++] = 1;

        만약 (id < classes) truth[index+id] = 1;
        index += classes;

        truth[index++] = x;
        truth[index++] = y;
        truth[index++] = w;
        truth[index++] = h;
    }
    free(boxes);
}

void load_rle(image im, 정수 *rle, 정수 n)
{
    정수 count = 0;
    정수 curr = 0;
    정수 i,j;
    위해(i = 0; i < n; ++i){
        위해(j = 0; j < rle[i]; ++j){
            im.data[count++] = curr;
        }
        curr = 1 - curr;
    }
    위해(; count < im.h*im.w*im.c; ++count){
        im.data[count] = curr;
    }
}

void or_image(image src, image dest, 정수 c)
{
    정수 i;
    위해(i = 0; i < src.w*src.h; ++i){
        만약(src.data[i]) dest.data[dest.w*dest.h*c + i] = 1;
    }
}

void exclusive_image(image src)
{
    정수 k, j, i;
    정수 s = src.w*src.h;
    위해(k = 0; k < src.c-1; ++k){
        위해(i = 0; i < s; ++i){
            만약 (src.data[k*s + i]){
                위해(j = k+1; j < src.c; ++j){
                    src.data[j*s + i] = 0;
                }
            }
        }
    }
}

box bound_image(image im)
{
    정수 x,y;
    정수 minx = im.w;
    정수 miny = im.h;
    정수 maxx = 0;
    정수 maxy = 0;
    위해(y = 0; y < im.h; ++y){
        위해(x = 0; x < im.w; ++x){
            만약(im.data[y*im.w + x]){
                minx = (x < minx) ? x : minx;
                miny = (y < miny) ? y : miny;
                maxx = (x > maxx) ? x : maxx;
                maxy = (y > maxy) ? y : maxy;
            }
        }
    }
    box b = {minx, miny, maxx-minx + 1, maxy-miny + 1};
    //printf("%f %f %f %f\n", b.x, b.y, b.w, b.h);
    반환 b;
}

void fill_truth_iseg(char *path, 정수 num_boxes, float *truth, 정수 classes, 정수 w, 정수 h, augment_args aug, 정수 flip, 정수 mw, 정수 mh)
{
    char labelpath[4096];
    find_replace(path, "images", "mask", labelpath);
    find_replace(labelpath, "JPEGImages", "mask", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    FILE *file = fopen(labelpath, "r");
    만약(!file) file_error(labelpath);
    char buff[32788];
    정수 id;
    정수 i = 0;
    정수 j;
    image part = make_image(w, h, 1);
    동안((fscanf(file, "%d %s", &id, buff) == 2) && i < num_boxes){
        정수 n = 0;
        정수 *rle = read_intlist(buff, &n, 0);
        load_rle(part, rle, n);
        image sized = rotate_crop_image(part, aug.rad, aug.scale, aug.w, aug.h, aug.dx, aug.dy, aug.aspect);
        만약(flip) flip_image(sized);

        image mask = resize_image(sized, mw, mh);
        truth[i*(mw*mh+1)] = id;
        위해(j = 0; j < mw*mh; ++j){
            truth[i*(mw*mh + 1) + 1 + j] = mask.data[j];
        }
        ++i;

        free_image(mask);
        free_image(sized);
        free(rle);
    }
    만약(i < num_boxes) truth[i*(mw*mh+1)] = -1;
    fclose(file);
    free_image(part);
}

void fill_truth_mask(char *path, 정수 num_boxes, float *truth, 정수 classes, 정수 w, 정수 h, augment_args aug, 정수 flip, 정수 mw, 정수 mh)
{
    char labelpath[4096];
    find_replace(path, "images", "mask", labelpath);
    find_replace(labelpath, "JPEGImages", "mask", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    FILE *file = fopen(labelpath, "r");
    만약(!file) file_error(labelpath);
    char buff[32788];
    정수 id;
    정수 i = 0;
    image part = make_image(w, h, 1);
    동안((fscanf(file, "%d %s", &id, buff) == 2) && i < num_boxes){
        정수 n = 0;
        정수 *rle = read_intlist(buff, &n, 0);
        load_rle(part, rle, n);
        image sized = rotate_crop_image(part, aug.rad, aug.scale, aug.w, aug.h, aug.dx, aug.dy, aug.aspect);
        만약(flip) flip_image(sized);
        box b = bound_image(sized);
        만약(b.w > 0){
            image crop = crop_image(sized, b.x, b.y, b.w, b.h);
            image mask = resize_image(crop, mw, mh);
            truth[i*(4 + mw*mh + 1) + 0] = (b.x + b.w/2.)/sized.w;
            truth[i*(4 + mw*mh + 1) + 1] = (b.y + b.h/2.)/sized.h;
            truth[i*(4 + mw*mh + 1) + 2] = b.w/sized.w;
            truth[i*(4 + mw*mh + 1) + 3] = b.h/sized.h;
            정수 j;
            위해(j = 0; j < mw*mh; ++j){
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


void fill_truth_detection(char *path, 정수 num_boxes, float *truth, 정수 classes, 정수 flip, float dx, float dy, float sx, float sy)
{
    char labelpath[4096];
    find_replace(path, "images", "labels", labelpath);
    find_replace(labelpath, "JPEGImages", "labels", labelpath);

    find_replace(labelpath, "raw", "labels", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".png", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    정수 count = 0;
    box_label *boxes = read_boxes(labelpath, &count);
    randomize_boxes(boxes, count);
    correct_boxes(boxes, count, dx, dy, sx, sy, flip);
    만약(count > num_boxes) count = num_boxes;
    float x,y,w,h;
    정수 id;
    정수 i;
    정수 sub = 0;

    위해 (i = 0; i < count; ++i) {
        x =  boxes[i].x;
        y =  boxes[i].y;
        w =  boxes[i].w;
        h =  boxes[i].h;
        id = boxes[i].id;

        만약 ((w < .001 || h < .001)) {
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

void print_letters(float *pred, 정수 n)
{
    정수 i;
    위해(i = 0; i < n; ++i){
        정수 index = max_index(pred+i*NUMCHARS, NUMCHARS);
        printf("%c", int_to_alphanum(index));
    }
    printf("\n");
}

void fill_truth_captcha(char *path, 정수 n, float *truth)
{
    char *begin = strrchr(path, '/');
    ++begin;
    정수 i;
    위해(i = 0; i < strlen(begin) && i < n && begin[i] != '.'; ++i){
        정수 index = alphanum_to_int(begin[i]);
        만약(index > 35) printf("Bad %c\n", begin[i]);
        truth[i*NUMCHARS+index] = 1;
    }
    위해(;i < n; ++i){
        truth[i*NUMCHARS + NUMCHARS-1] = 1;
    }
}

data load_data_captcha(char **paths, 정수 n, 정수 m, 정수 k, 정수 w, 정수 h)
{
    만약(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;
    d.X = load_image_paths(paths, n, w, h);
    d.y = make_matrix(n, k*NUMCHARS);
    정수 i;
    위해(i = 0; i < n; ++i){
        fill_truth_captcha(paths[i], k, d.y.vals[i]);
    }
    만약(m) free(paths);
    반환 d;
}

data load_data_captcha_encode(char **paths, 정수 n, 정수 m, 정수 w, 정수 h)
{
    만약(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;
    d.X = load_image_paths(paths, n, w, h);
    d.X.cols = 17100;
    d.y = d.X;
    만약(m) free(paths);
    반환 d;
}

void fill_truth(char *path, char **labels, 정수 k, float *truth)
{
    정수 i;
    memset(truth, 0, k*sizeof(float));
    정수 count = 0;
    위해(i = 0; i < k; ++i){
        만약(strstr(path, labels[i])){
            truth[i] = 1;
            ++count;
            //printf("%s %s %d\n", path, labels[i], i);
        }
    }
    만약(count != 1 && (k != 1 || count != 0)) printf("Too many or too few labels: %d, %s\n", count, path);
}

void fill_hierarchy(float *truth, 정수 k, tree *hierarchy)
{
    정수 j;
    위해(j = 0; j < k; ++j){
        만약(truth[j]){
            정수 parent = hierarchy->parent[j];
            동안(parent >= 0){
                truth[parent] = 1;
                parent = hierarchy->parent[parent];
            }
        }
    }
    정수 i;
    정수 count = 0;
    위해(j = 0; j < hierarchy->groups; ++j){
        //printf("%d\n", count);
        정수 mask = 1;
        위해(i = 0; i < hierarchy->group_size[j]; ++i){
            만약(truth[count + i]){
                mask = 0;
                break;
            }
        }
        만약 (mask) {
            위해(i = 0; i < hierarchy->group_size[j]; ++i){
                truth[count + i] = SECRET_NUM;
            }
        }
        count += hierarchy->group_size[j];
    }
}

matrix load_regression_labels_paths(char **paths, 정수 n, 정수 k)
{
    matrix y = make_matrix(n, k);
    정수 i,j;
    위해(i = 0; i < n; ++i){
        char labelpath[4096];
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
        위해(j = 0; j < k; ++j){
            fscanf(file, "%f", &(y.vals[i][j]));
        }
        fclose(file);
    }
    반환 y;
}

matrix load_labels_paths(char **paths, 정수 n, char **labels, 정수 k, tree *hierarchy)
{
    matrix y = make_matrix(n, k);
    정수 i;
    위해(i = 0; i < n && labels; ++i){
        fill_truth(paths[i], labels, k, y.vals[i]);
        만약(hierarchy){
            fill_hierarchy(y.vals[i], k, hierarchy);
        }
    }
    반환 y;
}

matrix load_tags_paths(char **paths, 정수 n, 정수 k)
{
    matrix y = make_matrix(n, k);
    정수 i;
    //int count = 0;
    위해(i = 0; i < n; ++i){
        char label[4096];
        find_replace(paths[i], "images", "labels", label);
        find_replace(label, ".jpg", ".txt", label);
        FILE *file = fopen(label, "r");
        만약 (!file) continue;
        //++count;
        정수 tag;
        동안(fscanf(file, "%d", &tag) == 1){
            만약(tag < k){
                y.vals[i][tag] = 1;
            }
        }
        fclose(file);
    }
    //printf("%d/%d\n", count, n);
    반환 y;
}

char **get_labels(char *filename)
{
    list *plist = get_paths(filename);
    char **labels = (char **)list_to_array(plist);
    free_list(plist);
    반환 labels;
}

void free_data(data d)
{
    만약(!d.shallow){
        free_matrix(d.X);
        free_matrix(d.y);
    }아니면{
        free(d.X.vals);
        free(d.y.vals);
    }
}

image get_segmentation_image(char *path, 정수 w, 정수 h, 정수 classes)
{
    char labelpath[4096];
    find_replace(path, "images", "mask", labelpath);
    find_replace(labelpath, "JPEGImages", "mask", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    image mask = make_image(w, h, classes);
    FILE *file = fopen(labelpath, "r");
    만약(!file) file_error(labelpath);
    char buff[32788];
    정수 id;
    image part = make_image(w, h, 1);
    동안(fscanf(file, "%d %s", &id, buff) == 2){
        정수 n = 0;
        정수 *rle = read_intlist(buff, &n, 0);
        load_rle(part, rle, n);
        or_image(part, mask, id);
        free(rle);
    }
    //exclusive_image(mask);
    fclose(file);
    free_image(part);
    반환 mask;
}

image get_segmentation_image2(char *path, 정수 w, 정수 h, 정수 classes)
{
    char labelpath[4096];
    find_replace(path, "images", "mask", labelpath);
    find_replace(labelpath, "JPEGImages", "mask", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    image mask = make_image(w, h, classes+1);
    정수 i;
    위해(i = 0; i < w*h; ++i){
        mask.data[w*h*classes + i] = 1;
    }
    FILE *file = fopen(labelpath, "r");
    만약(!file) file_error(labelpath);
    char buff[32788];
    정수 id;
    image part = make_image(w, h, 1);
    동안(fscanf(file, "%d %s", &id, buff) == 2){
        정수 n = 0;
        정수 *rle = read_intlist(buff, &n, 0);
        load_rle(part, rle, n);
        or_image(part, mask, id);
        위해(i = 0; i < w*h; ++i){
            만약(part.data[i]) mask.data[w*h*classes + i] = 0;
        }
        free(rle);
    }
    //exclusive_image(mask);
    fclose(file);
    free_image(part);
    반환 mask;
}

data load_data_seg(정수 n, char **paths, 정수 m, 정수 w, 정수 h, 정수 classes, 정수 min, 정수 max, float angle, float aspect, float hue, float saturation, float exposure, 정수 div)
{
    char **random_paths = get_random_paths(paths, n, m);
    정수 i;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, sizeof(float*));
    d.X.cols = h*w*3;


    d.y.rows = n;
    d.y.cols = h*w*classes/div/div;
    d.y.vals = calloc(d.X.rows, sizeof(float*));

    위해(i = 0; i < n; ++i){
        image orig = load_image_color(random_paths[i], 0, 0);
        augment_args a = random_augment_args(orig, angle, aspect, min, max, w, h);
        image sized = rotate_crop_image(orig, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);

        정수 flip = rand()%2;
        만약(flip) flip_image(sized);
        random_distort_image(sized, hue, saturation, exposure);
        d.X.vals[i] = sized.data;

        image mask = get_segmentation_image(random_paths[i], orig.w, orig.h, classes);
        //image mask = make_image(orig.w, orig.h, classes+1);
        image sized_m = rotate_crop_image(mask, a.rad, a.scale/div, a.w/div, a.h/div, a.dx/div, a.dy/div, a.aspect);

        만약(flip) flip_image(sized_m);
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
    반환 d;
}

data load_data_iseg(정수 n, char **paths, 정수 m, 정수 w, 정수 h, 정수 classes, 정수 boxes, 정수 div, 정수 min, 정수 max, float angle, float aspect, float hue, float saturation, float exposure)
{
    char **random_paths = get_random_paths(paths, n, m);
    정수 i;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, sizeof(float*));
    d.X.cols = h*w*3;

    d.y = make_matrix(n, (((w/div)*(h/div))+1)*boxes);

    위해(i = 0; i < n; ++i){
        image orig = load_image_color(random_paths[i], 0, 0);
        augment_args a = random_augment_args(orig, angle, aspect, min, max, w, h);
        image sized = rotate_crop_image(orig, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);

        정수 flip = rand()%2;
        만약(flip) flip_image(sized);
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
    반환 d;
}

data load_data_mask(정수 n, char **paths, 정수 m, 정수 w, 정수 h, 정수 classes, 정수 boxes, 정수 coords, 정수 min, 정수 max, float angle, float aspect, float hue, float saturation, float exposure)
{
    char **random_paths = get_random_paths(paths, n, m);
    정수 i;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, sizeof(float*));
    d.X.cols = h*w*3;

    d.y = make_matrix(n, (coords+1)*boxes);

    위해(i = 0; i < n; ++i){
        image orig = load_image_color(random_paths[i], 0, 0);
        augment_args a = random_augment_args(orig, angle, aspect, min, max, w, h);
        image sized = rotate_crop_image(orig, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);

        정수 flip = rand()%2;
        만약(flip) flip_image(sized);
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
    반환 d;
}

data load_data_region(정수 n, char **paths, 정수 m, 정수 w, 정수 h, 정수 size, 정수 classes, float jitter, float hue, float saturation, float exposure)
{
    char **random_paths = get_random_paths(paths, n, m);
    정수 i;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, sizeof(float*));
    d.X.cols = h*w*3;


    정수 k = size*size*(5+classes);
    d.y = make_matrix(n, k);
    위해(i = 0; i < n; ++i){
        image orig = load_image_color(random_paths[i], 0, 0);

        정수 oh = orig.h;
        정수 ow = orig.w;

        정수 dw = (ow*jitter);
        정수 dh = (oh*jitter);

        정수 pleft  = rand_uniform(-dw, dw);
        정수 pright = rand_uniform(-dw, dw);
        정수 ptop   = rand_uniform(-dh, dh);
        정수 pbot   = rand_uniform(-dh, dh);

        정수 swidth =  ow - pleft - pright;
        정수 sheight = oh - ptop - pbot;

        float sx = (float)swidth  / ow;
        float sy = (float)sheight / oh;

        정수 flip = rand()%2;
        image cropped = crop_image(orig, pleft, ptop, swidth, sheight);

        float dx = ((float)pleft/ow)/sx;
        float dy = ((float)ptop /oh)/sy;

        image sized = resize_image(cropped, w, h);
        만약(flip) flip_image(sized);
        random_distort_image(sized, hue, saturation, exposure);
        d.X.vals[i] = sized.data;

        fill_truth_region(random_paths[i], d.y.vals[i], classes, size, flip, dx, dy, 1./sx, 1./sy);

        free_image(orig);
        free_image(cropped);
    }
    free(random_paths);
    반환 d;
}

data load_data_compare(정수 n, char **paths, 정수 m, 정수 classes, 정수 w, 정수 h)
{
    만약(m) paths = get_random_paths(paths, 2*n, m);
    정수 i,j;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, sizeof(float*));
    d.X.cols = h*w*6;

    정수 k = 2*(classes);
    d.y = make_matrix(n, k);
    위해(i = 0; i < n; ++i){
        image im1 = load_image_color(paths[i*2],   w, h);
        image im2 = load_image_color(paths[i*2+1], w, h);

        d.X.vals[i] = calloc(d.X.cols, sizeof(float));
        memcpy(d.X.vals[i],         im1.data, h*w*3*sizeof(float));
        memcpy(d.X.vals[i] + h*w*3, im2.data, h*w*3*sizeof(float));

        정수 id;
        float iou;

        char imlabel1[4096];
        char imlabel2[4096];
        find_replace(paths[i*2],   "imgs", "labels", imlabel1);
        find_replace(imlabel1, "jpg", "txt", imlabel1);
        FILE *fp1 = fopen(imlabel1, "r");

        동안(fscanf(fp1, "%d %f", &id, &iou) == 2){
            만약 (d.y.vals[i][2*id] < iou) d.y.vals[i][2*id] = iou;
        }

        find_replace(paths[i*2+1], "imgs", "labels", imlabel2);
        find_replace(imlabel2, "jpg", "txt", imlabel2);
        FILE *fp2 = fopen(imlabel2, "r");

        동안(fscanf(fp2, "%d %f", &id, &iou) == 2){
            만약 (d.y.vals[i][2*id + 1] < iou) d.y.vals[i][2*id + 1] = iou;
        }

        위해 (j = 0; j < classes; ++j){
            만약 (d.y.vals[i][2*j] > .5 &&  d.y.vals[i][2*j+1] < .5){
                d.y.vals[i][2*j] = 1;
                d.y.vals[i][2*j+1] = 0;
            } 아니면 만약 (d.y.vals[i][2*j] < .5 &&  d.y.vals[i][2*j+1] > .5){
                d.y.vals[i][2*j] = 0;
                d.y.vals[i][2*j+1] = 1;
            } 아니면 {
                d.y.vals[i][2*j]   = SECRET_NUM;
                d.y.vals[i][2*j+1] = SECRET_NUM;
            }
        }
        fclose(fp1);
        fclose(fp2);

        free_image(im1);
        free_image(im2);
    }
    만약(m) free(paths);
    반환 d;
}

data load_data_swag(char **paths, 정수 n, 정수 classes, float jitter)
{
    정수 index = rand()%n;
    char *random_path = paths[index];

    image orig = load_image_color(random_path, 0, 0);
    정수 h = orig.h;
    정수 w = orig.w;

    data d = {0};
    d.shallow = 0;
    d.w = w;
    d.h = h;

    d.X.rows = 1;
    d.X.vals = calloc(d.X.rows, sizeof(float*));
    d.X.cols = h*w*3;

    정수 k = (4+classes)*90;
    d.y = make_matrix(1, k);

    정수 dw = w*jitter;
    정수 dh = h*jitter;

    정수 pleft  = rand_uniform(-dw, dw);
    정수 pright = rand_uniform(-dw, dw);
    정수 ptop   = rand_uniform(-dh, dh);
    정수 pbot   = rand_uniform(-dh, dh);

    정수 swidth =  w - pleft - pright;
    정수 sheight = h - ptop - pbot;

    float sx = (float)swidth  / w;
    float sy = (float)sheight / h;

    정수 flip = rand()%2;
    image cropped = crop_image(orig, pleft, ptop, swidth, sheight);

    float dx = ((float)pleft/w)/sx;
    float dy = ((float)ptop /h)/sy;

    image sized = resize_image(cropped, w, h);
    만약(flip) flip_image(sized);
    d.X.vals[0] = sized.data;

    fill_truth_swag(random_path, d.y.vals[0], classes, flip, dx, dy, 1./sx, 1./sy);

    free_image(orig);
    free_image(cropped);

    반환 d;
}

data load_data_detection(정수 n, char **paths, 정수 m, 정수 w, 정수 h, 정수 boxes, 정수 classes, float jitter, float hue, float saturation, float exposure)
{
    char **random_paths = get_random_paths(paths, n, m);
    정수 i;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, sizeof(float*));
    d.X.cols = h*w*3;

    d.y = make_matrix(n, 5*boxes);
    위해(i = 0; i < n; ++i){
        image orig = load_image_color(random_paths[i], 0, 0);
        image sized = make_image(w, h, orig.c);
        fill_image(sized, .5);

        float dw = jitter * orig.w;
        float dh = jitter * orig.h;

        float new_ar = (orig.w + rand_uniform(-dw, dw)) / (orig.h + rand_uniform(-dh, dh));
        //float scale = rand_uniform(.25, 2);
        float scale = 1;

        float nw, nh;

        만약(new_ar < 1){
            nh = scale * h;
            nw = nh * new_ar;
        } 아니면 {
            nw = scale * w;
            nh = nw / new_ar;
        }

        float dx = rand_uniform(0, w - nw);
        float dy = rand_uniform(0, h - nh);

        place_image(orig, nw, nh, dx, dy, sized);

        random_distort_image(sized, hue, saturation, exposure);

        정수 flip = rand()%2;
        만약(flip) flip_image(sized);
        d.X.vals[i] = sized.data;


        fill_truth_detection(random_paths[i], boxes, d.y.vals[i], classes, flip, -dx/w, -dy/h, nw/w, nh/h);

        free_image(orig);
    }
    free(random_paths);
    반환 d;
}

void *load_thread(void *ptr)
{
    //printf("Loading data: %d\n", rand());
    load_args a = *(struct load_args*)ptr;
    만약(a.exposure == 0) a.exposure = 1;
    만약(a.saturation == 0) a.saturation = 1;
    만약(a.aspect == 0) a.aspect = 1;

    만약 (a.type == OLD_CLASSIFICATION_DATA){
        *a.d = load_data_old(a.paths, a.n, a.m, a.labels, a.classes, a.w, a.h);
    } 아니면 만약 (a.type == REGRESSION_DATA){
        *a.d = load_data_regression(a.paths, a.n, a.m, a.classes, a.min, a.max, a.size, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    } 아니면 만약 (a.type == CLASSIFICATION_DATA){
        *a.d = load_data_augment(a.paths, a.n, a.m, a.labels, a.classes, a.hierarchy, a.min, a.max, a.size, a.angle, a.aspect, a.hue, a.saturation, a.exposure, a.center);
    } 아니면 만약 (a.type == SUPER_DATA){
        *a.d = load_data_super(a.paths, a.n, a.m, a.w, a.h, a.scale);
    } 아니면 만약 (a.type == WRITING_DATA){
        *a.d = load_data_writing(a.paths, a.n, a.m, a.w, a.h, a.out_w, a.out_h);
    } 아니면 만약 (a.type == ISEG_DATA){
        *a.d = load_data_iseg(a.n, a.paths, a.m, a.w, a.h, a.classes, a.num_boxes, a.scale, a.min, a.max, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    } 아니면 만약 (a.type == INSTANCE_DATA){
        *a.d = load_data_mask(a.n, a.paths, a.m, a.w, a.h, a.classes, a.num_boxes, a.coords, a.min, a.max, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    } 아니면 만약 (a.type == SEGMENTATION_DATA){
        *a.d = load_data_seg(a.n, a.paths, a.m, a.w, a.h, a.classes, a.min, a.max, a.angle, a.aspect, a.hue, a.saturation, a.exposure, a.scale);
    } 아니면 만약 (a.type == REGION_DATA){
        *a.d = load_data_region(a.n, a.paths, a.m, a.w, a.h, a.num_boxes, a.classes, a.jitter, a.hue, a.saturation, a.exposure);
    } 아니면 만약 (a.type == DETECTION_DATA){
        *a.d = load_data_detection(a.n, a.paths, a.m, a.w, a.h, a.num_boxes, a.classes, a.jitter, a.hue, a.saturation, a.exposure);
    } 아니면 만약 (a.type == SWAG_DATA){
        *a.d = load_data_swag(a.paths, a.n, a.classes, a.jitter);
    } 아니면 만약 (a.type == COMPARE_DATA){
        *a.d = load_data_compare(a.n, a.paths, a.m, a.classes, a.w, a.h);
    } 아니면 만약 (a.type == IMAGE_DATA){
        *(a.im) = load_image_color(a.path, 0, 0);
        *(a.resized) = resize_image(*(a.im), a.w, a.h);
    } 아니면 만약 (a.type == LETTERBOX_DATA){
        *(a.im) = load_image_color(a.path, 0, 0);
        *(a.resized) = letterbox_image(*(a.im), a.w, a.h);
    } 아니면 만약 (a.type == TAG_DATA){
        *a.d = load_data_tag(a.paths, a.n, a.m, a.classes, a.min, a.max, a.size, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    }
    free(ptr);
    반환 0;
}

pthread_t load_data_in_thread(load_args args)
{
    pthread_t thread;
    struct load_args *ptr = calloc(1, sizeof(struct load_args));
    *ptr = args;
    만약(pthread_create(&thread, 0, load_thread, ptr)) error("Thread creation failed");
    반환 thread;
}

void *load_threads(void *ptr)
{
    정수 i;
    load_args args = *(load_args *)ptr;
    만약 (args.threads == 0) args.threads = 1;
    data *out = args.d;
    정수 total = args.n;
    free(ptr);
    data *buffers = calloc(args.threads, sizeof(data));
    pthread_t *threads = calloc(args.threads, sizeof(pthread_t));
    위해(i = 0; i < args.threads; ++i){
        args.d = buffers + i;
        args.n = (i+1) * total/args.threads - i * total/args.threads;
        threads[i] = load_data_in_thread(args);
    }
    위해(i = 0; i < args.threads; ++i){
        pthread_join(threads[i], 0);
    }
    *out = concat_datas(buffers, args.threads);
    out->shallow = 0;
    위해(i = 0; i < args.threads; ++i){
        buffers[i].shallow = 1;
        free_data(buffers[i]);
    }
    free(buffers);
    free(threads);
    반환 0;
}

void load_data_blocking(load_args args)
{
    struct load_args *ptr = calloc(1, sizeof(struct load_args));
    *ptr = args;
    load_thread(ptr);
}

pthread_t load_data(load_args args)
{
    pthread_t thread;
    struct load_args *ptr = calloc(1, sizeof(struct load_args));
    *ptr = args;
    만약(pthread_create(&thread, 0, load_threads, ptr)) error("Thread creation failed");
    반환 thread;
}

data load_data_writing(char **paths, 정수 n, 정수 m, 정수 w, 정수 h, 정수 out_w, 정수 out_h)
{
    만약(m) paths = get_random_paths(paths, n, m);
    char **replace_paths = find_replace_paths(paths, n, ".png", "-label.png");
    data d = {0};
    d.shallow = 0;
    d.X = load_image_paths(paths, n, w, h);
    d.y = load_image_paths_gray(replace_paths, n, out_w, out_h);
    만약(m) free(paths);
    정수 i;
    위해(i = 0; i < n; ++i) free(replace_paths[i]);
    free(replace_paths);
    반환 d;
}

data load_data_old(char **paths, 정수 n, 정수 m, char **labels, 정수 k, 정수 w, 정수 h)
{
    만약(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;
    d.X = load_image_paths(paths, n, w, h);
    d.y = load_labels_paths(paths, n, labels, k, 0);
    만약(m) free(paths);
    반환 d;
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

data load_data_super(char **paths, 정수 n, 정수 m, 정수 w, 정수 h, 정수 scale)
{
    만약(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;

    정수 i;
    d.X.rows = n;
    d.X.vals = calloc(n, sizeof(float*));
    d.X.cols = w*h*3;

    d.y.rows = n;
    d.y.vals = calloc(n, sizeof(float*));
    d.y.cols = w*scale * h*scale * 3;

    위해(i = 0; i < n; ++i){
        image im = load_image_color(paths[i], 0, 0);
        image crop = random_crop_image(im, w*scale, h*scale);
        정수 flip = rand()%2;
        만약 (flip) flip_image(crop);
        image resize = resize_image(crop, w, h);
        d.X.vals[i] = resize.data;
        d.y.vals[i] = crop.data;
        free_image(im);
    }

    만약(m) free(paths);
    반환 d;
}

data load_data_regression(char **paths, 정수 n, 정수 m, 정수 k, 정수 min, 정수 max, 정수 size, float angle, float aspect, float hue, float saturation, float exposure)
{
    만약(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;
    d.X = load_image_augment_paths(paths, n, min, max, size, angle, aspect, hue, saturation, exposure, 0);
    d.y = load_regression_labels_paths(paths, n, k);
    만약(m) free(paths);
    반환 d;
}

data select_data(data *orig, 정수 *inds)
{
    data d = {0};
    d.shallow = 1;
    d.w = orig[0].w;
    d.h = orig[0].h;

    d.X.rows = orig[0].X.rows;
    d.y.rows = orig[0].X.rows;

    d.X.cols = orig[0].X.cols;
    d.y.cols = orig[0].y.cols;

    d.X.vals = calloc(orig[0].X.rows, sizeof(float *));
    d.y.vals = calloc(orig[0].y.rows, sizeof(float *));
    정수 i;
    위해(i = 0; i < d.X.rows; ++i){
        d.X.vals[i] = orig[inds[i]].X.vals[i];
        d.y.vals[i] = orig[inds[i]].y.vals[i];
    }
    반환 d;
}

data *tile_data(data orig, 정수 divs, 정수 size)
{
    data *ds = calloc(divs*divs, sizeof(data));
    정수 i, j;
#pragma omp parallel 위해
    위해(i = 0; i < divs*divs; ++i){
        data d;
        d.shallow = 0;
        d.w = orig.w/divs * size;
        d.h = orig.h/divs * size;
        d.X.rows = orig.X.rows;
        d.X.cols = d.w*d.h*3;
        d.X.vals = calloc(d.X.rows, sizeof(float*));

        d.y = copy_matrix(orig.y);
#pragma omp parallel 위해
        위해(j = 0; j < orig.X.rows; ++j){
            정수 x = (i%divs) * orig.w / divs - (d.w - orig.w/divs)/2;
            정수 y = (i/divs) * orig.h / divs - (d.h - orig.h/divs)/2;
            image im = float_to_image(orig.w, orig.h, 3, orig.X.vals[j]);
            d.X.vals[j] = crop_image(im, x, y, d.w, d.h).data;
        }
        ds[i] = d;
    }
    반환 ds;
}

data resize_data(data orig, 정수 w, 정수 h)
{
    data d = {0};
    d.shallow = 0;
    d.w = w;
    d.h = h;
    정수 i;
    d.X.rows = orig.X.rows;
    d.X.cols = w*h*3;
    d.X.vals = calloc(d.X.rows, sizeof(float*));

    d.y = copy_matrix(orig.y);
#pragma omp parallel 위해
    위해(i = 0; i < orig.X.rows; ++i){
        image im = float_to_image(orig.w, orig.h, 3, orig.X.vals[i]);
        d.X.vals[i] = resize_image(im, w, h).data;
    }
    반환 d;
}

data load_data_augment(char **paths, 정수 n, 정수 m, char **labels, 정수 k, tree *hierarchy, 정수 min, 정수 max, 정수 size, float angle, float aspect, float hue, float saturation, float exposure, 정수 center)
{
    만약(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;
    d.w=size;
    d.h=size;
    d.X = load_image_augment_paths(paths, n, min, max, size, angle, aspect, hue, saturation, exposure, center);
    d.y = load_labels_paths(paths, n, labels, k, hierarchy);
    만약(m) free(paths);
    반환 d;
}

data load_data_tag(char **paths, 정수 n, 정수 m, 정수 k, 정수 min, 정수 max, 정수 size, float angle, float aspect, float hue, float saturation, float exposure)
{
    만약(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.w = size;
    d.h = size;
    d.shallow = 0;
    d.X = load_image_augment_paths(paths, n, min, max, size, angle, aspect, hue, saturation, exposure, 0);
    d.y = load_tags_paths(paths, n, k);
    만약(m) free(paths);
    반환 d;
}

matrix concat_matrix(matrix m1, matrix m2)
{
    정수 i, count = 0;
    matrix m;
    m.cols = m1.cols;
    m.rows = m1.rows+m2.rows;
    m.vals = calloc(m1.rows + m2.rows, sizeof(float*));
    위해(i = 0; i < m1.rows; ++i){
        m.vals[count++] = m1.vals[i];
    }
    위해(i = 0; i < m2.rows; ++i){
        m.vals[count++] = m2.vals[i];
    }
    반환 m;
}

data concat_data(data d1, data d2)
{
    data d = {0};
    d.shallow = 1;
    d.X = concat_matrix(d1.X, d2.X);
    d.y = concat_matrix(d1.y, d2.y);
    d.w = d1.w;
    d.h = d1.h;
    반환 d;
}

data concat_datas(data *d, 정수 n)
{
    정수 i;
    data out = {0};
    위해(i = 0; i < n; ++i){
        data new = concat_data(d[i], out);
        free_data(out);
        out = new;
    }
    반환 out;
}

data load_categorical_data_csv(char *filename, 정수 target, 정수 k)
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
    반환 d;
}

data load_cifar10_data(char *filename)
{
    data d = {0};
    d.shallow = 0;
    long i,j;
    matrix X = make_matrix(10000, 3072);
    matrix y = make_matrix(10000, 10);
    d.X = X;
    d.y = y;

    FILE *fp = fopen(filename, "rb");
    만약(!fp) file_error(filename);
    위해(i = 0; i < 10000; ++i){
        unsigned char bytes[3073];
        fread(bytes, 1, 3073, fp);
        정수 class = bytes[0];
        y.vals[i][class] = 1;
        위해(j = 0; j < X.cols; ++j){
            X.vals[i][j] = (double)bytes[j+1];
        }
    }
    scale_data_rows(d, 1./255);
    //normalize_data_rows(d);
    fclose(fp);
    반환 d;
}

void get_random_batch(data d, 정수 n, float *X, float *y)
{
    정수 j;
    위해(j = 0; j < n; ++j){
        정수 index = rand()%d.X.rows;
        memcpy(X+j*d.X.cols, d.X.vals[index], d.X.cols*sizeof(float));
        memcpy(y+j*d.y.cols, d.y.vals[index], d.y.cols*sizeof(float));
    }
}

void get_next_batch(data d, 정수 n, 정수 offset, float *X, float *y)
{
    정수 j;
    위해(j = 0; j < n; ++j){
        정수 index = offset + j;
        memcpy(X+j*d.X.cols, d.X.vals[index], d.X.cols*sizeof(float));
        만약(y) memcpy(y+j*d.y.cols, d.y.vals[index], d.y.cols*sizeof(float));
    }
}

void smooth_data(data d)
{
    정수 i, j;
    float scale = 1. / d.y.cols;
    float eps = .1;
    위해(i = 0; i < d.y.rows; ++i){
        위해(j = 0; j < d.y.cols; ++j){
            d.y.vals[i][j] = eps * scale + (1-eps) * d.y.vals[i][j];
        }
    }
}

data load_all_cifar10()
{
    data d = {0};
    d.shallow = 0;
    정수 i,j,b;
    matrix X = make_matrix(50000, 3072);
    matrix y = make_matrix(50000, 10);
    d.X = X;
    d.y = y;


    위해(b = 0; b < 5; ++b){
        char buff[256];
        sprintf(buff, "data/cifar/cifar-10-batches-bin/data_batch_%d.bin", b+1);
        FILE *fp = fopen(buff, "rb");
        만약(!fp) file_error(buff);
        위해(i = 0; i < 10000; ++i){
            unsigned char bytes[3073];
            fread(bytes, 1, 3073, fp);
            정수 class = bytes[0];
            y.vals[i+b*10000][class] = 1;
            위해(j = 0; j < X.cols; ++j){
                X.vals[i+b*10000][j] = (double)bytes[j+1];
            }
        }
        fclose(fp);
    }
    //normalize_data_rows(d);
    scale_data_rows(d, 1./255);
    smooth_data(d);
    반환 d;
}

data load_go(char *filename)
{
    FILE *fp = fopen(filename, "rb");
    matrix X = make_matrix(3363059, 361);
    matrix y = make_matrix(3363059, 361);
    정수 row, col;

    만약(!fp) file_error(filename);
    char *label;
    정수 count = 0;
    동안((label = fgetl(fp))){
        정수 i;
        만약(count == X.rows){
            X = resize_matrix(X, count*2);
            y = resize_matrix(y, count*2);
        }
        sscanf(label, "%d %d", &row, &col);
        char *board = fgetl(fp);

        정수 index = row*19 + col;
        y.vals[count][index] = 1;

        위해(i = 0; i < 19*19; ++i){
            float val = 0;
            만약(board[i] == '1') val = 1;
            아니면 만약(board[i] == '2') val = -1;
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

    반환 d;
}


void randomize_data(data d)
{
    정수 i;
    위해(i = d.X.rows-1; i > 0; --i){
        정수 index = rand()%i;
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
    정수 i;
    위해(i = 0; i < d.X.rows; ++i){
        scale_array(d.X.vals[i], d.X.cols, s);
    }
}

void translate_data_rows(data d, float s)
{
    정수 i;
    위해(i = 0; i < d.X.rows; ++i){
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
    반환 c;
}

void normalize_data_rows(data d)
{
    정수 i;
    위해(i = 0; i < d.X.rows; ++i){
        normalize_array(d.X.vals[i], d.X.cols);
    }
}

data get_data_part(data d, 정수 part, 정수 total)
{
    data p = {0};
    p.shallow = 1;
    p.X.rows = d.X.rows * (part + 1) / total - d.X.rows * part / total;
    p.y.rows = d.y.rows * (part + 1) / total - d.y.rows * part / total;
    p.X.cols = d.X.cols;
    p.y.cols = d.y.cols;
    p.X.vals = d.X.vals + d.X.rows * part / total;
    p.y.vals = d.y.vals + d.y.rows * part / total;
    반환 p;
}

data get_random_data(data d, 정수 num)
{
    data r = {0};
    r.shallow = 1;

    r.X.rows = num;
    r.y.rows = num;

    r.X.cols = d.X.cols;
    r.y.cols = d.y.cols;

    r.X.vals = calloc(num, sizeof(float *));
    r.y.vals = calloc(num, sizeof(float *));

    정수 i;
    위해(i = 0; i < num; ++i){
        정수 index = rand()%d.X.rows;
        r.X.vals[i] = d.X.vals[index];
        r.y.vals[i] = d.y.vals[index];
    }
    반환 r;
}

data *split_data(data d, 정수 part, 정수 total)
{
    data *split = calloc(2, sizeof(data));
    정수 i;
    정수 start = part*d.X.rows/total;
    정수 end = (part+1)*d.X.rows/total;
    data train;
    data test;
    train.shallow = test.shallow = 1;

    test.X.rows = test.y.rows = end-start;
    train.X.rows = train.y.rows = d.X.rows - (end-start);
    train.X.cols = test.X.cols = d.X.cols;
    train.y.cols = test.y.cols = d.y.cols;

    train.X.vals = calloc(train.X.rows, sizeof(float*));
    test.X.vals = calloc(test.X.rows, sizeof(float*));
    train.y.vals = calloc(train.y.rows, sizeof(float*));
    test.y.vals = calloc(test.y.rows, sizeof(float*));

    위해(i = 0; i < start; ++i){
        train.X.vals[i] = d.X.vals[i];
        train.y.vals[i] = d.y.vals[i];
    }
    위해(i = start; i < end; ++i){
        test.X.vals[i-start] = d.X.vals[i];
        test.y.vals[i-start] = d.y.vals[i];
    }
    위해(i = end; i < d.X.rows; ++i){
        train.X.vals[i-(end-start)] = d.X.vals[i];
        train.y.vals[i-(end-start)] = d.y.vals[i];
    }
    split[0] = train;
    split[1] = test;
    반환 split;
}

