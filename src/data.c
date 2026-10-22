#ಅನ್ತರ್ಭಾವ "data.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "image.h"
#ಅನ್ತರ್ಭಾವ "cuda.h"

#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <string.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

list *get_paths(ವರ್ಣ *filename)
{
    ವರ್ಣ *path;
    FILE *file = fopen(filename, "r");
    ಯದಿ(!file) file_error(filename);
    list *ಸಾಲುಗಳು = make_list();
    ಯಾವತ್((path=fgetl(file))){
        list_ಸೇರಿಸು(ಸಾಲುಗಳು, path);
    }
    fclose(file);
    ಪ್ರತ್ಯಾಗಮ ಸಾಲುಗಳು;
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

ವರ್ಣ **get_random_paths(ವರ್ಣ **paths, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ m)
{
    ವರ್ಣ **random_paths = calloc(n, ಪರಿಮಾಣ(ವರ್ಣ*));
    ಪೂರ್ಣಾಙ್ಕ i;
    pthread_mutex_lock(&mutex);
    ಪ್ರತಿ(i = 0; i < n; ++i){
        ಪೂರ್ಣಾಙ್ಕ index = rand()%m;
        random_paths[i] = paths[index];
        //if(i == 0) printf("%s\n", paths[index]);
    }
    pthread_mutex_unlock(&mutex);
    ಪ್ರತ್ಯಾಗಮ random_paths;
}

ವರ್ಣ **find_replace_paths(ವರ್ಣ **paths, ಪೂರ್ಣಾಙ್ಕ n, ವರ್ಣ *find, ವರ್ಣ *replace)
{
    ವರ್ಣ **replace_paths = calloc(n, ಪರಿಮಾಣ(ವರ್ಣ*));
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < n; ++i){
        ವರ್ಣ replaced[4096];
        find_replace(paths[i], find, replace, replaced);
        replace_paths[i] = copy_string(replaced);
    }
    ಪ್ರತ್ಯಾಗಮ replace_paths;
}

matrix load_image_paths_gray(ವರ್ಣ **paths, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    matrix X;
    X.rows = n;
    X.vals = calloc(X.rows, ಪರಿಮಾಣ(ಭಿನ್ನ*));
    X.cols = 0;

    ಪ್ರತಿ(i = 0; i < n; ++i){
        image im = load_image(paths[i], w, h, 3);

        image gray = grayscale_image(im);
        free_image(im);
        im = gray;

        X.vals[i] = im.data;
        X.cols = im.h*im.w*im.c;
    }
    ಪ್ರತ್ಯಾಗಮ X;
}

matrix load_image_paths(ವರ್ಣ **paths, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    matrix X;
    X.rows = n;
    X.vals = calloc(X.rows, ಪರಿಮಾಣ(ಭಿನ್ನ*));
    X.cols = 0;

    ಪ್ರತಿ(i = 0; i < n; ++i){
        image im = load_image_color(paths[i], w, h);
        X.vals[i] = im.data;
        X.cols = im.h*im.w*im.c;
    }
    ಪ್ರತ್ಯಾಗಮ X;
}

matrix load_image_augment_paths(ವರ್ಣ **paths, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ min, ಪೂರ್ಣಾಙ್ಕ max, ಪೂರ್ಣಾಙ್ಕ size, ಭಿನ್ನ angle, ಭಿನ್ನ aspect, ಭಿನ್ನ hue, ಭಿನ್ನ saturation, ಭಿನ್ನ exposure, ಪೂರ್ಣಾಙ್ಕ center)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    matrix X;
    X.rows = n;
    X.vals = calloc(X.rows, ಪರಿಮಾಣ(ಭಿನ್ನ*));
    X.cols = 0;

    ಪ್ರತಿ(i = 0; i < n; ++i){
        image im = load_image_color(paths[i], 0, 0);
        image crop;
        ಯದಿ(center){
            crop = center_crop_image(im, size, size);
        } ಅನ್ಯಥಾ {
            crop = random_augment_image(im, angle, aspect, min, max, size, size);
        }
        ಪೂರ್ಣಾಙ್ಕ flip = rand()%2;
        ಯದಿ (flip) flip_image(crop);
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
    ಪ್ರತ್ಯಾಗಮ X;
}


box_label *read_boxes(ವರ್ಣ *filename, ಪೂರ್ಣಾಙ್ಕ *n)
{
    FILE *file = fopen(filename, "r");
    ಯದಿ(!file) file_error(filename);
    ಭಿನ್ನ x, y, h, w;
    ಪೂರ್ಣಾಙ್ಕ id;
    ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = 0;
    ಪೂರ್ಣಾಙ್ಕ size = 64;
    box_label *boxes = calloc(size, ಪರಿಮಾಣ(box_label));
    ಯಾವತ್(fscanf(file, "%d %f %f %f %f", &id, &x, &y, &w, &h) == 5){
        ಯದಿ(ಎಣಿಕೆ == size) {
            size = size * 2;
            boxes = realloc(boxes, size*ಪರಿಮಾಣ(box_label));
        }
        boxes[ಎಣಿಕೆ].id = id;
        boxes[ಎಣಿಕೆ].x = x;
        boxes[ಎಣಿಕೆ].y = y;
        boxes[ಎಣಿಕೆ].h = h;
        boxes[ಎಣಿಕೆ].w = w;
        boxes[ಎಣಿಕೆ].ಎಡ   = x - w/2;
        boxes[ಎಣಿಕೆ].ಬಲ  = x + w/2;
        boxes[ಎಣಿಕೆ].ತುದಿ    = y - h/2;
        boxes[ಎಣಿಕೆ].bottom = y + h/2;
        ++ಎಣಿಕೆ;
    }
    fclose(file);
    *n = ಎಣಿಕೆ;
    ಪ್ರತ್ಯಾಗಮ boxes;
}

ಶೂನ್ಯ randomize_boxes(box_label *b, ಪೂರ್ಣಾಙ್ಕ n)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < n; ++i){
        box_label swap = b[i];
        ಪೂರ್ಣಾಙ್ಕ index = rand()%n;
        b[i] = b[index];
        b[index] = swap;
    }
}

ಶೂನ್ಯ correct_boxes(box_label *boxes, ಪೂರ್ಣಾಙ್ಕ n, ಭಿನ್ನ dx, ಭಿನ್ನ dy, ಭಿನ್ನ sx, ಭಿನ್ನ sy, ಪೂರ್ಣಾಙ್ಕ flip)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < n; ++i){
        ಯದಿ(boxes[i].x == 0 && boxes[i].y == 0) {
            boxes[i].x = 999999;
            boxes[i].y = 999999;
            boxes[i].w = 999999;
            boxes[i].h = 999999;
            ಅನುವರ್ತ;
        }
        boxes[i].ಎಡ   = boxes[i].ಎಡ  * sx - dx;
        boxes[i].ಬಲ  = boxes[i].ಬಲ * sx - dx;
        boxes[i].ತುದಿ    = boxes[i].ತುದಿ   * sy - dy;
        boxes[i].bottom = boxes[i].bottom* sy - dy;

        ಯದಿ(flip){
            ಭಿನ್ನ swap = boxes[i].ಎಡ;
            boxes[i].ಎಡ = 1. - boxes[i].ಬಲ;
            boxes[i].ಬಲ = 1. - swap;
        }

        boxes[i].ಎಡ =  constrain(0, 1, boxes[i].ಎಡ);
        boxes[i].ಬಲ = constrain(0, 1, boxes[i].ಬಲ);
        boxes[i].ತುದಿ =   constrain(0, 1, boxes[i].ತುದಿ);
        boxes[i].bottom =   constrain(0, 1, boxes[i].bottom);

        boxes[i].x = (boxes[i].ಎಡ+boxes[i].ಬಲ)/2;
        boxes[i].y = (boxes[i].ತುದಿ+boxes[i].bottom)/2;
        boxes[i].w = (boxes[i].ಬಲ - boxes[i].ಎಡ);
        boxes[i].h = (boxes[i].bottom - boxes[i].ತುದಿ);

        boxes[i].w = constrain(0, 1, boxes[i].w);
        boxes[i].h = constrain(0, 1, boxes[i].h);
    }
}

ಶೂನ್ಯ fill_truth_swag(ವರ್ಣ *path, ಭಿನ್ನ *truth, ಪೂರ್ಣಾಙ್ಕ classes, ಪೂರ್ಣಾಙ್ಕ flip, ಭಿನ್ನ dx, ಭಿನ್ನ dy, ಭಿನ್ನ sx, ಭಿನ್ನ sy)
{
    ವರ್ಣ labelpath[4096];
    find_replace(path, "images", "labels", labelpath);
    find_replace(labelpath, "JPEGImages", "labels", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);

    ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = 0;
    box_label *boxes = read_boxes(labelpath, &ಎಣಿಕೆ);
    randomize_boxes(boxes, ಎಣಿಕೆ);
    correct_boxes(boxes, ಎಣಿಕೆ, dx, dy, sx, sy, flip);
    ಭಿನ್ನ x,y,w,h;
    ಪೂರ್ಣಾಙ್ಕ id;
    ಪೂರ್ಣಾಙ್ಕ i;

    ಪ್ರತಿ (i = 0; i < ಎಣಿಕೆ && i < 90; ++i) {
        x =  boxes[i].x;
        y =  boxes[i].y;
        w =  boxes[i].w;
        h =  boxes[i].h;
        id = boxes[i].id;

        ಯದಿ (w < .0 || h < .0) ಅನುವರ್ತ;

        ಪೂರ್ಣಾಙ್ಕ index = (4+classes) * i;

        truth[index++] = x;
        truth[index++] = y;
        truth[index++] = w;
        truth[index++] = h;

        ಯದಿ (id < classes) truth[index+id] = 1;
    }
    ಮುಕ್ತ(boxes);
}

ಶೂನ್ಯ fill_truth_region(ವರ್ಣ *path, ಭಿನ್ನ *truth, ಪೂರ್ಣಾಙ್ಕ classes, ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_boxes, ಪೂರ್ಣಾಙ್ಕ flip, ಭಿನ್ನ dx, ಭಿನ್ನ dy, ಭಿನ್ನ sx, ಭಿನ್ನ sy)
{
    ವರ್ಣ labelpath[4096];
    find_replace(path, "images", "labels", labelpath);
    find_replace(labelpath, "JPEGImages", "labels", labelpath);

    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".png", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = 0;
    box_label *boxes = read_boxes(labelpath, &ಎಣಿಕೆ);
    randomize_boxes(boxes, ಎಣಿಕೆ);
    correct_boxes(boxes, ಎಣಿಕೆ, dx, dy, sx, sy, flip);
    ಭಿನ್ನ x,y,w,h;
    ಪೂರ್ಣಾಙ್ಕ id;
    ಪೂರ್ಣಾಙ್ಕ i;

    ಪ್ರತಿ (i = 0; i < ಎಣಿಕೆ; ++i) {
        x =  boxes[i].x;
        y =  boxes[i].y;
        w =  boxes[i].w;
        h =  boxes[i].h;
        id = boxes[i].id;

        ಯದಿ (w < .005 || h < .005) ಅನುವರ್ತ;

        ಪೂರ್ಣಾಙ್ಕ col = (ಪೂರ್ಣಾಙ್ಕ)(x*ಅಂಕೆ_boxes);
        ಪೂರ್ಣಾಙ್ಕ row = (ಪೂರ್ಣಾಙ್ಕ)(y*ಅಂಕೆ_boxes);

        x = x*ಅಂಕೆ_boxes - col;
        y = y*ಅಂಕೆ_boxes - row;

        ಪೂರ್ಣಾಙ್ಕ index = (col+row*ಅಂಕೆ_boxes)*(5+classes);
        ಯದಿ (truth[index]) ಅನುವರ್ತ;
        truth[index++] = 1;

        ಯದಿ (id < classes) truth[index+id] = 1;
        index += classes;

        truth[index++] = x;
        truth[index++] = y;
        truth[index++] = w;
        truth[index++] = h;
    }
    ಮುಕ್ತ(boxes);
}

ಶೂನ್ಯ load_rle(image im, ಪೂರ್ಣಾಙ್ಕ *rle, ಪೂರ್ಣಾಙ್ಕ n)
{
    ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = 0;
    ಪೂರ್ಣಾಙ್ಕ curr = 0;
    ಪೂರ್ಣಾಙ್ಕ i,j;
    ಪ್ರತಿ(i = 0; i < n; ++i){
        ಪ್ರತಿ(j = 0; j < rle[i]; ++j){
            im.data[ಎಣಿಕೆ++] = curr;
        }
        curr = 1 - curr;
    }
    ಪ್ರತಿ(; ಎಣಿಕೆ < im.h*im.w*im.c; ++ಎಣಿಕೆ){
        im.data[ಎಣಿಕೆ] = curr;
    }
}

ಶೂನ್ಯ or_image(image src, image dest, ಪೂರ್ಣಾಙ್ಕ c)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < src.w*src.h; ++i){
        ಯದಿ(src.data[i]) dest.data[dest.w*dest.h*c + i] = 1;
    }
}

ಶೂನ್ಯ exclusive_image(image src)
{
    ಪೂರ್ಣಾಙ್ಕ k, j, i;
    ಪೂರ್ಣಾಙ್ಕ s = src.w*src.h;
    ಪ್ರತಿ(k = 0; k < src.c-1; ++k){
        ಪ್ರತಿ(i = 0; i < s; ++i){
            ಯದಿ (src.data[k*s + i]){
                ಪ್ರತಿ(j = k+1; j < src.c; ++j){
                    src.data[j*s + i] = 0;
                }
            }
        }
    }
}

box bound_image(image im)
{
    ಪೂರ್ಣಾಙ್ಕ x,y;
    ಪೂರ್ಣಾಙ್ಕ minx = im.w;
    ಪೂರ್ಣಾಙ್ಕ miny = im.h;
    ಪೂರ್ಣಾಙ್ಕ maxx = 0;
    ಪೂರ್ಣಾಙ್ಕ maxy = 0;
    ಪ್ರತಿ(y = 0; y < im.h; ++y){
        ಪ್ರತಿ(x = 0; x < im.w; ++x){
            ಯದಿ(im.data[y*im.w + x]){
                minx = (x < minx) ? x : minx;
                miny = (y < miny) ? y : miny;
                maxx = (x > maxx) ? x : maxx;
                maxy = (y > maxy) ? y : maxy;
            }
        }
    }
    box b = {minx, miny, maxx-minx + 1, maxy-miny + 1};
    //printf("%f %f %f %f\n", b.x, b.y, b.w, b.h);
    ಪ್ರತ್ಯಾಗಮ b;
}

ಶೂನ್ಯ fill_truth_iseg(ವರ್ಣ *path, ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_boxes, ಭಿನ್ನ *truth, ಪೂರ್ಣಾಙ್ಕ classes, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h, augment_args aug, ಪೂರ್ಣಾಙ್ಕ flip, ಪೂರ್ಣಾಙ್ಕ mw, ಪೂರ್ಣಾಙ್ಕ mh)
{
    ವರ್ಣ labelpath[4096];
    find_replace(path, "images", "mask", labelpath);
    find_replace(labelpath, "JPEGImages", "mask", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    FILE *file = fopen(labelpath, "r");
    ಯದಿ(!file) file_error(labelpath);
    ವರ್ಣ buff[32788];
    ಪೂರ್ಣಾಙ್ಕ id;
    ಪೂರ್ಣಾಙ್ಕ i = 0;
    ಪೂರ್ಣಾಙ್ಕ j;
    image part = make_image(w, h, 1);
    ಯಾವತ್((fscanf(file, "%d %s", &id, buff) == 2) && i < ಅಂಕೆ_boxes){
        ಪೂರ್ಣಾಙ್ಕ n = 0;
        ಪೂರ್ಣಾಙ್ಕ *rle = read_intlist(buff, &n, 0);
        load_rle(part, rle, n);
        image sized = rotate_crop_image(part, aug.rad, aug.scale, aug.w, aug.h, aug.dx, aug.dy, aug.aspect);
        ಯದಿ(flip) flip_image(sized);

        image mask = resize_image(sized, mw, mh);
        truth[i*(mw*mh+1)] = id;
        ಪ್ರತಿ(j = 0; j < mw*mh; ++j){
            truth[i*(mw*mh + 1) + 1 + j] = mask.data[j];
        }
        ++i;

        free_image(mask);
        free_image(sized);
        ಮುಕ್ತ(rle);
    }
    ಯದಿ(i < ಅಂಕೆ_boxes) truth[i*(mw*mh+1)] = -1;
    fclose(file);
    free_image(part);
}

ಶೂನ್ಯ fill_truth_mask(ವರ್ಣ *path, ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_boxes, ಭಿನ್ನ *truth, ಪೂರ್ಣಾಙ್ಕ classes, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h, augment_args aug, ಪೂರ್ಣಾಙ್ಕ flip, ಪೂರ್ಣಾಙ್ಕ mw, ಪೂರ್ಣಾಙ್ಕ mh)
{
    ವರ್ಣ labelpath[4096];
    find_replace(path, "images", "mask", labelpath);
    find_replace(labelpath, "JPEGImages", "mask", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    FILE *file = fopen(labelpath, "r");
    ಯದಿ(!file) file_error(labelpath);
    ವರ್ಣ buff[32788];
    ಪೂರ್ಣಾಙ್ಕ id;
    ಪೂರ್ಣಾಙ್ಕ i = 0;
    image part = make_image(w, h, 1);
    ಯಾವತ್((fscanf(file, "%d %s", &id, buff) == 2) && i < ಅಂಕೆ_boxes){
        ಪೂರ್ಣಾಙ್ಕ n = 0;
        ಪೂರ್ಣಾಙ್ಕ *rle = read_intlist(buff, &n, 0);
        load_rle(part, rle, n);
        image sized = rotate_crop_image(part, aug.rad, aug.scale, aug.w, aug.h, aug.dx, aug.dy, aug.aspect);
        ಯದಿ(flip) flip_image(sized);
        box b = bound_image(sized);
        ಯದಿ(b.w > 0){
            image crop = crop_image(sized, b.x, b.y, b.w, b.h);
            image mask = resize_image(crop, mw, mh);
            truth[i*(4 + mw*mh + 1) + 0] = (b.x + b.w/2.)/sized.w;
            truth[i*(4 + mw*mh + 1) + 1] = (b.y + b.h/2.)/sized.h;
            truth[i*(4 + mw*mh + 1) + 2] = b.w/sized.w;
            truth[i*(4 + mw*mh + 1) + 3] = b.h/sized.h;
            ಪೂರ್ಣಾಙ್ಕ j;
            ಪ್ರತಿ(j = 0; j < mw*mh; ++j){
                truth[i*(4 + mw*mh + 1) + 4 + j] = mask.data[j];
            }
            truth[i*(4 + mw*mh + 1) + 4 + mw*mh] = id;
            free_image(crop);
            free_image(mask);
            ++i;
        }
        free_image(sized);
        ಮುಕ್ತ(rle);
    }
    fclose(file);
    free_image(part);
}


ಶೂನ್ಯ fill_truth_detection(ವರ್ಣ *path, ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_boxes, ಭಿನ್ನ *truth, ಪೂರ್ಣಾಙ್ಕ classes, ಪೂರ್ಣಾಙ್ಕ flip, ಭಿನ್ನ dx, ಭಿನ್ನ dy, ಭಿನ್ನ sx, ಭಿನ್ನ sy)
{
    ವರ್ಣ labelpath[4096];
    find_replace(path, "images", "labels", labelpath);
    find_replace(labelpath, "JPEGImages", "labels", labelpath);

    find_replace(labelpath, "raw", "labels", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".png", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = 0;
    box_label *boxes = read_boxes(labelpath, &ಎಣಿಕೆ);
    randomize_boxes(boxes, ಎಣಿಕೆ);
    correct_boxes(boxes, ಎಣಿಕೆ, dx, dy, sx, sy, flip);
    ಯದಿ(ಎಣಿಕೆ > ಅಂಕೆ_boxes) ಎಣಿಕೆ = ಅಂಕೆ_boxes;
    ಭಿನ್ನ x,y,w,h;
    ಪೂರ್ಣಾಙ್ಕ id;
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪೂರ್ಣಾಙ್ಕ sub = 0;

    ಪ್ರತಿ (i = 0; i < ಎಣಿಕೆ; ++i) {
        x =  boxes[i].x;
        y =  boxes[i].y;
        w =  boxes[i].w;
        h =  boxes[i].h;
        id = boxes[i].id;

        ಯದಿ ((w < .001 || h < .001)) {
            ++sub;
            ಅನುವರ್ತ;
        }

        truth[(i-sub)*5+0] = x;
        truth[(i-sub)*5+1] = y;
        truth[(i-sub)*5+2] = w;
        truth[(i-sub)*5+3] = h;
        truth[(i-sub)*5+4] = id;
    }
    ಮುಕ್ತ(boxes);
}

#define NUMCHARS 37

ಶೂನ್ಯ print_letters(ಭಿನ್ನ *pred, ಪೂರ್ಣಾಙ್ಕ n)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < n; ++i){
        ಪೂರ್ಣಾಙ್ಕ index = max_index(pred+i*NUMCHARS, NUMCHARS);
        printf("%c", int_to_alphanum(index));
    }
    printf("\n");
}

ಶೂನ್ಯ fill_truth_captcha(ವರ್ಣ *path, ಪೂರ್ಣಾಙ್ಕ n, ಭಿನ್ನ *truth)
{
    ವರ್ಣ *begin = strrchr(path, '/');
    ++begin;
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < strlen(begin) && i < n && begin[i] != '.'; ++i){
        ಪೂರ್ಣಾಙ್ಕ index = alphanum_to_int(begin[i]);
        ಯದಿ(index > 35) printf("Bad %c\n", begin[i]);
        truth[i*NUMCHARS+index] = 1;
    }
    ಪ್ರತಿ(;i < n; ++i){
        truth[i*NUMCHARS + NUMCHARS-1] = 1;
    }
}

data load_data_captcha(ವರ್ಣ **paths, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ m, ಪೂರ್ಣಾಙ್ಕ k, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h)
{
    ಯದಿ(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;
    d.X = load_image_paths(paths, n, w, h);
    d.y = make_matrix(n, k*NUMCHARS);
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < n; ++i){
        fill_truth_captcha(paths[i], k, d.y.vals[i]);
    }
    ಯದಿ(m) ಮುಕ್ತ(paths);
    ಪ್ರತ್ಯಾಗಮ d;
}

data load_data_captcha_encode(ವರ್ಣ **paths, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ m, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h)
{
    ಯದಿ(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;
    d.X = load_image_paths(paths, n, w, h);
    d.X.cols = 17100;
    d.y = d.X;
    ಯದಿ(m) ಮುಕ್ತ(paths);
    ಪ್ರತ್ಯಾಗಮ d;
}

ಶೂನ್ಯ fill_truth(ವರ್ಣ *path, ವರ್ಣ **labels, ಪೂರ್ಣಾಙ್ಕ k, ಭಿನ್ನ *truth)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    memset(truth, 0, k*ಪರಿಮಾಣ(ಭಿನ್ನ));
    ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = 0;
    ಪ್ರತಿ(i = 0; i < k; ++i){
        ಯದಿ(strstr(path, labels[i])){
            truth[i] = 1;
            ++ಎಣಿಕೆ;
            //printf("%s %s %d\n", path, labels[i], i);
        }
    }
    ಯದಿ(ಎಣಿಕೆ != 1 && (k != 1 || ಎಣಿಕೆ != 0)) printf("Too many or too few labels: %d, %s\n", ಎಣಿಕೆ, path);
}

ಶೂನ್ಯ fill_hierarchy(ಭಿನ್ನ *truth, ಪೂರ್ಣಾಙ್ಕ k, ವೃಕ್ಷ *hierarchy)
{
    ಪೂರ್ಣಾಙ್ಕ j;
    ಪ್ರತಿ(j = 0; j < k; ++j){
        ಯದಿ(truth[j]){
            ಪೂರ್ಣಾಙ್ಕ parent = hierarchy->parent[j];
            ಯಾವತ್(parent >= 0){
                truth[parent] = 1;
                parent = hierarchy->parent[parent];
            }
        }
    }
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = 0;
    ಪ್ರತಿ(j = 0; j < hierarchy->groups; ++j){
        //printf("%d\n", count);
        ಪೂರ್ಣಾಙ್ಕ mask = 1;
        ಪ್ರತಿ(i = 0; i < hierarchy->group_size[j]; ++i){
            ಯದಿ(truth[ಎಣಿಕೆ + i]){
                mask = 0;
                ಭಙ್ಗ;
            }
        }
        ಯದಿ (mask) {
            ಪ್ರತಿ(i = 0; i < hierarchy->group_size[j]; ++i){
                truth[ಎಣಿಕೆ + i] = SECRET_ಅಂಕೆ;
            }
        }
        ಎಣಿಕೆ += hierarchy->group_size[j];
    }
}

matrix load_regression_labels_paths(ವರ್ಣ **paths, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ k)
{
    matrix y = make_matrix(n, k);
    ಪೂರ್ಣಾಙ್ಕ i,j;
    ಪ್ರತಿ(i = 0; i < n; ++i){
        ವರ್ಣ labelpath[4096];
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
        ಪ್ರತಿ(j = 0; j < k; ++j){
            fscanf(file, "%f", &(y.vals[i][j]));
        }
        fclose(file);
    }
    ಪ್ರತ್ಯಾಗಮ y;
}

matrix load_labels_paths(ವರ್ಣ **paths, ಪೂರ್ಣಾಙ್ಕ n, ವರ್ಣ **labels, ಪೂರ್ಣಾಙ್ಕ k, ವೃಕ್ಷ *hierarchy)
{
    matrix y = make_matrix(n, k);
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < n && labels; ++i){
        fill_truth(paths[i], labels, k, y.vals[i]);
        ಯದಿ(hierarchy){
            fill_hierarchy(y.vals[i], k, hierarchy);
        }
    }
    ಪ್ರತ್ಯಾಗಮ y;
}

matrix load_tags_paths(ವರ್ಣ **paths, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ k)
{
    matrix y = make_matrix(n, k);
    ಪೂರ್ಣಾಙ್ಕ i;
    //int count = 0;
    ಪ್ರತಿ(i = 0; i < n; ++i){
        ವರ್ಣ label[4096];
        find_replace(paths[i], "images", "labels", label);
        find_replace(label, ".jpg", ".txt", label);
        FILE *file = fopen(label, "r");
        ಯದಿ (!file) ಅನುವರ್ತ;
        //++count;
        ಪೂರ್ಣಾಙ್ಕ tag;
        ಯಾವತ್(fscanf(file, "%d", &tag) == 1){
            ಯದಿ(tag < k){
                y.vals[i][tag] = 1;
            }
        }
        fclose(file);
    }
    //printf("%d/%d\n", count, n);
    ಪ್ರತ್ಯಾಗಮ y;
}

ವರ್ಣ **get_labels(ವರ್ಣ *filename)
{
    list *plist = get_paths(filename);
    ವರ್ಣ **labels = (ವರ್ಣ **)list_to_array(plist);
    free_list(plist);
    ಪ್ರತ್ಯಾಗಮ labels;
}

ಶೂನ್ಯ free_data(data d)
{
    ಯದಿ(!d.shallow){
        free_matrix(d.X);
        free_matrix(d.y);
    }ಅನ್ಯಥಾ{
        ಮುಕ್ತ(d.X.vals);
        ಮುಕ್ತ(d.y.vals);
    }
}

image get_segmentation_image(ವರ್ಣ *path, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h, ಪೂರ್ಣಾಙ್ಕ classes)
{
    ವರ್ಣ labelpath[4096];
    find_replace(path, "images", "mask", labelpath);
    find_replace(labelpath, "JPEGImages", "mask", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    image mask = make_image(w, h, classes);
    FILE *file = fopen(labelpath, "r");
    ಯದಿ(!file) file_error(labelpath);
    ವರ್ಣ buff[32788];
    ಪೂರ್ಣಾಙ್ಕ id;
    image part = make_image(w, h, 1);
    ಯಾವತ್(fscanf(file, "%d %s", &id, buff) == 2){
        ಪೂರ್ಣಾಙ್ಕ n = 0;
        ಪೂರ್ಣಾಙ್ಕ *rle = read_intlist(buff, &n, 0);
        load_rle(part, rle, n);
        or_image(part, mask, id);
        ಮುಕ್ತ(rle);
    }
    //exclusive_image(mask);
    fclose(file);
    free_image(part);
    ಪ್ರತ್ಯಾಗಮ mask;
}

image get_segmentation_image2(ವರ್ಣ *path, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h, ಪೂರ್ಣಾಙ್ಕ classes)
{
    ವರ್ಣ labelpath[4096];
    find_replace(path, "images", "mask", labelpath);
    find_replace(labelpath, "JPEGImages", "mask", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    image mask = make_image(w, h, classes+1);
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < w*h; ++i){
        mask.data[w*h*classes + i] = 1;
    }
    FILE *file = fopen(labelpath, "r");
    ಯದಿ(!file) file_error(labelpath);
    ವರ್ಣ buff[32788];
    ಪೂರ್ಣಾಙ್ಕ id;
    image part = make_image(w, h, 1);
    ಯಾವತ್(fscanf(file, "%d %s", &id, buff) == 2){
        ಪೂರ್ಣಾಙ್ಕ n = 0;
        ಪೂರ್ಣಾಙ್ಕ *rle = read_intlist(buff, &n, 0);
        load_rle(part, rle, n);
        or_image(part, mask, id);
        ಪ್ರತಿ(i = 0; i < w*h; ++i){
            ಯದಿ(part.data[i]) mask.data[w*h*classes + i] = 0;
        }
        ಮುಕ್ತ(rle);
    }
    //exclusive_image(mask);
    fclose(file);
    free_image(part);
    ಪ್ರತ್ಯಾಗಮ mask;
}

data load_data_seg(ಪೂರ್ಣಾಙ್ಕ n, ವರ್ಣ **paths, ಪೂರ್ಣಾಙ್ಕ m, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h, ಪೂರ್ಣಾಙ್ಕ classes, ಪೂರ್ಣಾಙ್ಕ min, ಪೂರ್ಣಾಙ್ಕ max, ಭಿನ್ನ angle, ಭಿನ್ನ aspect, ಭಿನ್ನ hue, ಭಿನ್ನ saturation, ಭಿನ್ನ exposure, ಪೂರ್ಣಾಙ್ಕ div)
{
    ವರ್ಣ **random_paths = get_random_paths(paths, n, m);
    ಪೂರ್ಣಾಙ್ಕ i;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, ಪರಿಮಾಣ(ಭಿನ್ನ*));
    d.X.cols = h*w*3;


    d.y.rows = n;
    d.y.cols = h*w*classes/div/div;
    d.y.vals = calloc(d.X.rows, ಪರಿಮಾಣ(ಭಿನ್ನ*));

    ಪ್ರತಿ(i = 0; i < n; ++i){
        image orig = load_image_color(random_paths[i], 0, 0);
        augment_args a = random_augment_args(orig, angle, aspect, min, max, w, h);
        image sized = rotate_crop_image(orig, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);

        ಪೂರ್ಣಾಙ್ಕ flip = rand()%2;
        ಯದಿ(flip) flip_image(sized);
        random_distort_image(sized, hue, saturation, exposure);
        d.X.vals[i] = sized.data;

        image mask = get_segmentation_image(random_paths[i], orig.w, orig.h, classes);
        //image mask = make_image(orig.w, orig.h, classes+1);
        image sized_m = rotate_crop_image(mask, a.rad, a.scale/div, a.w/div, a.h/div, a.dx/div, a.dy/div, a.aspect);

        ಯದಿ(flip) flip_image(sized_m);
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
    ಮುಕ್ತ(random_paths);
    ಪ್ರತ್ಯಾಗಮ d;
}

data load_data_iseg(ಪೂರ್ಣಾಙ್ಕ n, ವರ್ಣ **paths, ಪೂರ್ಣಾಙ್ಕ m, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h, ಪೂರ್ಣಾಙ್ಕ classes, ಪೂರ್ಣಾಙ್ಕ boxes, ಪೂರ್ಣಾಙ್ಕ div, ಪೂರ್ಣಾಙ್ಕ min, ಪೂರ್ಣಾಙ್ಕ max, ಭಿನ್ನ angle, ಭಿನ್ನ aspect, ಭಿನ್ನ hue, ಭಿನ್ನ saturation, ಭಿನ್ನ exposure)
{
    ವರ್ಣ **random_paths = get_random_paths(paths, n, m);
    ಪೂರ್ಣಾಙ್ಕ i;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, ಪರಿಮಾಣ(ಭಿನ್ನ*));
    d.X.cols = h*w*3;

    d.y = make_matrix(n, (((w/div)*(h/div))+1)*boxes);

    ಪ್ರತಿ(i = 0; i < n; ++i){
        image orig = load_image_color(random_paths[i], 0, 0);
        augment_args a = random_augment_args(orig, angle, aspect, min, max, w, h);
        image sized = rotate_crop_image(orig, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);

        ಪೂರ್ಣಾಙ್ಕ flip = rand()%2;
        ಯದಿ(flip) flip_image(sized);
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
    ಮುಕ್ತ(random_paths);
    ಪ್ರತ್ಯಾಗಮ d;
}

data load_data_mask(ಪೂರ್ಣಾಙ್ಕ n, ವರ್ಣ **paths, ಪೂರ್ಣಾಙ್ಕ m, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h, ಪೂರ್ಣಾಙ್ಕ classes, ಪೂರ್ಣಾಙ್ಕ boxes, ಪೂರ್ಣಾಙ್ಕ coords, ಪೂರ್ಣಾಙ್ಕ min, ಪೂರ್ಣಾಙ್ಕ max, ಭಿನ್ನ angle, ಭಿನ್ನ aspect, ಭಿನ್ನ hue, ಭಿನ್ನ saturation, ಭಿನ್ನ exposure)
{
    ವರ್ಣ **random_paths = get_random_paths(paths, n, m);
    ಪೂರ್ಣಾಙ್ಕ i;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, ಪರಿಮಾಣ(ಭಿನ್ನ*));
    d.X.cols = h*w*3;

    d.y = make_matrix(n, (coords+1)*boxes);

    ಪ್ರತಿ(i = 0; i < n; ++i){
        image orig = load_image_color(random_paths[i], 0, 0);
        augment_args a = random_augment_args(orig, angle, aspect, min, max, w, h);
        image sized = rotate_crop_image(orig, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);

        ಪೂರ್ಣಾಙ್ಕ flip = rand()%2;
        ಯದಿ(flip) flip_image(sized);
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
    ಮುಕ್ತ(random_paths);
    ಪ್ರತ್ಯಾಗಮ d;
}

data load_data_region(ಪೂರ್ಣಾಙ್ಕ n, ವರ್ಣ **paths, ಪೂರ್ಣಾಙ್ಕ m, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h, ಪೂರ್ಣಾಙ್ಕ size, ಪೂರ್ಣಾಙ್ಕ classes, ಭಿನ್ನ jitter, ಭಿನ್ನ hue, ಭಿನ್ನ saturation, ಭಿನ್ನ exposure)
{
    ವರ್ಣ **random_paths = get_random_paths(paths, n, m);
    ಪೂರ್ಣಾಙ್ಕ i;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, ಪರಿಮಾಣ(ಭಿನ್ನ*));
    d.X.cols = h*w*3;


    ಪೂರ್ಣಾಙ್ಕ k = size*size*(5+classes);
    d.y = make_matrix(n, k);
    ಪ್ರತಿ(i = 0; i < n; ++i){
        image orig = load_image_color(random_paths[i], 0, 0);

        ಪೂರ್ಣಾಙ್ಕ oh = orig.h;
        ಪೂರ್ಣಾಙ್ಕ ow = orig.w;

        ಪೂರ್ಣಾಙ್ಕ dw = (ow*jitter);
        ಪೂರ್ಣಾಙ್ಕ dh = (oh*jitter);

        ಪೂರ್ಣಾಙ್ಕ pleft  = rand_uniform(-dw, dw);
        ಪೂರ್ಣಾಙ್ಕ pright = rand_uniform(-dw, dw);
        ಪೂರ್ಣಾಙ್ಕ ptop   = rand_uniform(-dh, dh);
        ಪೂರ್ಣಾಙ್ಕ pbot   = rand_uniform(-dh, dh);

        ಪೂರ್ಣಾಙ್ಕ swidth =  ow - pleft - pright;
        ಪೂರ್ಣಾಙ್ಕ sheight = oh - ptop - pbot;

        ಭಿನ್ನ sx = (ಭಿನ್ನ)swidth  / ow;
        ಭಿನ್ನ sy = (ಭಿನ್ನ)sheight / oh;

        ಪೂರ್ಣಾಙ್ಕ flip = rand()%2;
        image cropped = crop_image(orig, pleft, ptop, swidth, sheight);

        ಭಿನ್ನ dx = ((ಭಿನ್ನ)pleft/ow)/sx;
        ಭಿನ್ನ dy = ((ಭಿನ್ನ)ptop /oh)/sy;

        image sized = resize_image(cropped, w, h);
        ಯದಿ(flip) flip_image(sized);
        random_distort_image(sized, hue, saturation, exposure);
        d.X.vals[i] = sized.data;

        fill_truth_region(random_paths[i], d.y.vals[i], classes, size, flip, dx, dy, 1./sx, 1./sy);

        free_image(orig);
        free_image(cropped);
    }
    ಮುಕ್ತ(random_paths);
    ಪ್ರತ್ಯಾಗಮ d;
}

data load_data_compare(ಪೂರ್ಣಾಙ್ಕ n, ವರ್ಣ **paths, ಪೂರ್ಣಾಙ್ಕ m, ಪೂರ್ಣಾಙ್ಕ classes, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h)
{
    ಯದಿ(m) paths = get_random_paths(paths, 2*n, m);
    ಪೂರ್ಣಾಙ್ಕ i,j;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, ಪರಿಮಾಣ(ಭಿನ್ನ*));
    d.X.cols = h*w*6;

    ಪೂರ್ಣಾಙ್ಕ k = 2*(classes);
    d.y = make_matrix(n, k);
    ಪ್ರತಿ(i = 0; i < n; ++i){
        image im1 = load_image_color(paths[i*2],   w, h);
        image im2 = load_image_color(paths[i*2+1], w, h);

        d.X.vals[i] = calloc(d.X.cols, ಪರಿಮಾಣ(ಭಿನ್ನ));
        memcpy(d.X.vals[i],         im1.data, h*w*3*ಪರಿಮಾಣ(ಭಿನ್ನ));
        memcpy(d.X.vals[i] + h*w*3, im2.data, h*w*3*ಪರಿಮಾಣ(ಭಿನ್ನ));

        ಪೂರ್ಣಾಙ್ಕ id;
        ಭಿನ್ನ iou;

        ವರ್ಣ imlabel1[4096];
        ವರ್ಣ imlabel2[4096];
        find_replace(paths[i*2],   "imgs", "labels", imlabel1);
        find_replace(imlabel1, "jpg", "txt", imlabel1);
        FILE *fp1 = fopen(imlabel1, "r");

        ಯಾವತ್(fscanf(fp1, "%d %f", &id, &iou) == 2){
            ಯದಿ (d.y.vals[i][2*id] < iou) d.y.vals[i][2*id] = iou;
        }

        find_replace(paths[i*2+1], "imgs", "labels", imlabel2);
        find_replace(imlabel2, "jpg", "txt", imlabel2);
        FILE *fp2 = fopen(imlabel2, "r");

        ಯಾವತ್(fscanf(fp2, "%d %f", &id, &iou) == 2){
            ಯದಿ (d.y.vals[i][2*id + 1] < iou) d.y.vals[i][2*id + 1] = iou;
        }

        ಪ್ರತಿ (j = 0; j < classes; ++j){
            ಯದಿ (d.y.vals[i][2*j] > .5 &&  d.y.vals[i][2*j+1] < .5){
                d.y.vals[i][2*j] = 1;
                d.y.vals[i][2*j+1] = 0;
            } ಅನ್ಯಥಾ ಯದಿ (d.y.vals[i][2*j] < .5 &&  d.y.vals[i][2*j+1] > .5){
                d.y.vals[i][2*j] = 0;
                d.y.vals[i][2*j+1] = 1;
            } ಅನ್ಯಥಾ {
                d.y.vals[i][2*j]   = SECRET_ಅಂಕೆ;
                d.y.vals[i][2*j+1] = SECRET_ಅಂಕೆ;
            }
        }
        fclose(fp1);
        fclose(fp2);

        free_image(im1);
        free_image(im2);
    }
    ಯದಿ(m) ಮುಕ್ತ(paths);
    ಪ್ರತ್ಯಾಗಮ d;
}

data load_data_swag(ವರ್ಣ **paths, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ classes, ಭಿನ್ನ jitter)
{
    ಪೂರ್ಣಾಙ್ಕ index = rand()%n;
    ವರ್ಣ *random_path = paths[index];

    image orig = load_image_color(random_path, 0, 0);
    ಪೂರ್ಣಾಙ್ಕ h = orig.h;
    ಪೂರ್ಣಾಙ್ಕ w = orig.w;

    data d = {0};
    d.shallow = 0;
    d.w = w;
    d.h = h;

    d.X.rows = 1;
    d.X.vals = calloc(d.X.rows, ಪರಿಮಾಣ(ಭಿನ್ನ*));
    d.X.cols = h*w*3;

    ಪೂರ್ಣಾಙ್ಕ k = (4+classes)*90;
    d.y = make_matrix(1, k);

    ಪೂರ್ಣಾಙ್ಕ dw = w*jitter;
    ಪೂರ್ಣಾಙ್ಕ dh = h*jitter;

    ಪೂರ್ಣಾಙ್ಕ pleft  = rand_uniform(-dw, dw);
    ಪೂರ್ಣಾಙ್ಕ pright = rand_uniform(-dw, dw);
    ಪೂರ್ಣಾಙ್ಕ ptop   = rand_uniform(-dh, dh);
    ಪೂರ್ಣಾಙ್ಕ pbot   = rand_uniform(-dh, dh);

    ಪೂರ್ಣಾಙ್ಕ swidth =  w - pleft - pright;
    ಪೂರ್ಣಾಙ್ಕ sheight = h - ptop - pbot;

    ಭಿನ್ನ sx = (ಭಿನ್ನ)swidth  / w;
    ಭಿನ್ನ sy = (ಭಿನ್ನ)sheight / h;

    ಪೂರ್ಣಾಙ್ಕ flip = rand()%2;
    image cropped = crop_image(orig, pleft, ptop, swidth, sheight);

    ಭಿನ್ನ dx = ((ಭಿನ್ನ)pleft/w)/sx;
    ಭಿನ್ನ dy = ((ಭಿನ್ನ)ptop /h)/sy;

    image sized = resize_image(cropped, w, h);
    ಯದಿ(flip) flip_image(sized);
    d.X.vals[0] = sized.data;

    fill_truth_swag(random_path, d.y.vals[0], classes, flip, dx, dy, 1./sx, 1./sy);

    free_image(orig);
    free_image(cropped);

    ಪ್ರತ್ಯಾಗಮ d;
}

data load_data_detection(ಪೂರ್ಣಾಙ್ಕ n, ವರ್ಣ **paths, ಪೂರ್ಣಾಙ್ಕ m, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h, ಪೂರ್ಣಾಙ್ಕ boxes, ಪೂರ್ಣಾಙ್ಕ classes, ಭಿನ್ನ jitter, ಭಿನ್ನ hue, ಭಿನ್ನ saturation, ಭಿನ್ನ exposure)
{
    ವರ್ಣ **random_paths = get_random_paths(paths, n, m);
    ಪೂರ್ಣಾಙ್ಕ i;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, ಪರಿಮಾಣ(ಭಿನ್ನ*));
    d.X.cols = h*w*3;

    d.y = make_matrix(n, 5*boxes);
    ಪ್ರತಿ(i = 0; i < n; ++i){
        image orig = load_image_color(random_paths[i], 0, 0);
        image sized = make_image(w, h, orig.c);
        fill_image(sized, .5);

        ಭಿನ್ನ dw = jitter * orig.w;
        ಭಿನ್ನ dh = jitter * orig.h;

        ಭಿನ್ನ new_ar = (orig.w + rand_uniform(-dw, dw)) / (orig.h + rand_uniform(-dh, dh));
        //float scale = rand_uniform(.25, 2);
        ಭಿನ್ನ scale = 1;

        ಭಿನ್ನ nw, nh;

        ಯದಿ(new_ar < 1){
            nh = scale * h;
            nw = nh * new_ar;
        } ಅನ್ಯಥಾ {
            nw = scale * w;
            nh = nw / new_ar;
        }

        ಭಿನ್ನ dx = rand_uniform(0, w - nw);
        ಭಿನ್ನ dy = rand_uniform(0, h - nh);

        place_image(orig, nw, nh, dx, dy, sized);

        random_distort_image(sized, hue, saturation, exposure);

        ಪೂರ್ಣಾಙ್ಕ flip = rand()%2;
        ಯದಿ(flip) flip_image(sized);
        d.X.vals[i] = sized.data;


        fill_truth_detection(random_paths[i], boxes, d.y.vals[i], classes, flip, -dx/w, -dy/h, nw/w, nh/h);

        free_image(orig);
    }
    ಮುಕ್ತ(random_paths);
    ಪ್ರತ್ಯಾಗಮ d;
}

ಶೂನ್ಯ *load_thread(ಶೂನ್ಯ *ptr)
{
    //printf("Loading data: %d\n", rand());
    load_args a = *(ಸಂರಚನಾ load_args*)ptr;
    ಯದಿ(a.exposure == 0) a.exposure = 1;
    ಯದಿ(a.saturation == 0) a.saturation = 1;
    ಯದಿ(a.aspect == 0) a.aspect = 1;

    ಯದಿ (a.type == OLD_CLASSIFICATION_DATA){
        *a.d = load_data_old(a.paths, a.n, a.m, a.labels, a.classes, a.w, a.h);
    } ಅನ್ಯಥಾ ಯದಿ (a.type == REGRESSION_DATA){
        *a.d = load_data_regression(a.paths, a.n, a.m, a.classes, a.min, a.max, a.size, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    } ಅನ್ಯಥಾ ಯದಿ (a.type == CLASSIFICATION_DATA){
        *a.d = load_data_augment(a.paths, a.n, a.m, a.labels, a.classes, a.hierarchy, a.min, a.max, a.size, a.angle, a.aspect, a.hue, a.saturation, a.exposure, a.center);
    } ಅನ್ಯಥಾ ಯದಿ (a.type == SUPER_DATA){
        *a.d = load_data_super(a.paths, a.n, a.m, a.w, a.h, a.scale);
    } ಅನ್ಯಥಾ ಯದಿ (a.type == WRITING_DATA){
        *a.d = load_data_writing(a.paths, a.n, a.m, a.w, a.h, a.out_w, a.out_h);
    } ಅನ್ಯಥಾ ಯದಿ (a.type == ISEG_DATA){
        *a.d = load_data_iseg(a.n, a.paths, a.m, a.w, a.h, a.classes, a.ಅಂಕೆ_boxes, a.scale, a.min, a.max, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    } ಅನ್ಯಥಾ ಯದಿ (a.type == INSTANCE_DATA){
        *a.d = load_data_mask(a.n, a.paths, a.m, a.w, a.h, a.classes, a.ಅಂಕೆ_boxes, a.coords, a.min, a.max, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    } ಅನ್ಯಥಾ ಯದಿ (a.type == SEGMENTATION_DATA){
        *a.d = load_data_seg(a.n, a.paths, a.m, a.w, a.h, a.classes, a.min, a.max, a.angle, a.aspect, a.hue, a.saturation, a.exposure, a.scale);
    } ಅನ್ಯಥಾ ಯದಿ (a.type == REGION_DATA){
        *a.d = load_data_region(a.n, a.paths, a.m, a.w, a.h, a.ಅಂಕೆ_boxes, a.classes, a.jitter, a.hue, a.saturation, a.exposure);
    } ಅನ್ಯಥಾ ಯದಿ (a.type == DETECTION_DATA){
        *a.d = load_data_detection(a.n, a.paths, a.m, a.w, a.h, a.ಅಂಕೆ_boxes, a.classes, a.jitter, a.hue, a.saturation, a.exposure);
    } ಅನ್ಯಥಾ ಯದಿ (a.type == SWAG_DATA){
        *a.d = load_data_swag(a.paths, a.n, a.classes, a.jitter);
    } ಅನ್ಯಥಾ ಯದಿ (a.type == COMPARE_DATA){
        *a.d = load_data_compare(a.n, a.paths, a.m, a.classes, a.w, a.h);
    } ಅನ್ಯಥಾ ಯದಿ (a.type == IMAGE_DATA){
        *(a.im) = load_image_color(a.path, 0, 0);
        *(a.resized) = resize_image(*(a.im), a.w, a.h);
    } ಅನ್ಯಥಾ ಯದಿ (a.type == LETTERBOX_DATA){
        *(a.im) = load_image_color(a.path, 0, 0);
        *(a.resized) = letterbox_image(*(a.im), a.w, a.h);
    } ಅನ್ಯಥಾ ಯದಿ (a.type == TAG_DATA){
        *a.d = load_data_tag(a.paths, a.n, a.m, a.classes, a.min, a.max, a.size, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    }
    ಮುಕ್ತ(ptr);
    ಪ್ರತ್ಯಾಗಮ 0;
}

pthread_t load_data_in_thread(load_args args)
{
    pthread_t thread;
    ಸಂರಚನಾ load_args *ptr = calloc(1, ಪರಿಮಾಣ(ಸಂರಚನಾ load_args));
    *ptr = args;
    ಯದಿ(pthread_create(&thread, 0, load_thread, ptr)) error("Thread creation failed");
    ಪ್ರತ್ಯಾಗಮ thread;
}

ಶೂನ್ಯ *load_threads(ಶೂನ್ಯ *ptr)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    load_args args = *(load_args *)ptr;
    ಯದಿ (args.threads == 0) args.threads = 1;
    data *out = args.d;
    ಪೂರ್ಣಾಙ್ಕ total = args.n;
    ಮುಕ್ತ(ptr);
    data *buffers = calloc(args.threads, ಪರಿಮಾಣ(data));
    pthread_t *threads = calloc(args.threads, ಪರಿಮಾಣ(pthread_t));
    ಪ್ರತಿ(i = 0; i < args.threads; ++i){
        args.d = buffers + i;
        args.n = (i+1) * total/args.threads - i * total/args.threads;
        threads[i] = load_data_in_thread(args);
    }
    ಪ್ರತಿ(i = 0; i < args.threads; ++i){
        pthread_join(threads[i], 0);
    }
    *out = concat_datas(buffers, args.threads);
    out->shallow = 0;
    ಪ್ರತಿ(i = 0; i < args.threads; ++i){
        buffers[i].shallow = 1;
        free_data(buffers[i]);
    }
    ಮುಕ್ತ(buffers);
    ಮುಕ್ತ(threads);
    ಪ್ರತ್ಯಾಗಮ 0;
}

ಶೂನ್ಯ load_data_blocking(load_args args)
{
    ಸಂರಚನಾ load_args *ptr = calloc(1, ಪರಿಮಾಣ(ಸಂರಚನಾ load_args));
    *ptr = args;
    load_thread(ptr);
}

pthread_t load_data(load_args args)
{
    pthread_t thread;
    ಸಂರಚನಾ load_args *ptr = calloc(1, ಪರಿಮಾಣ(ಸಂರಚನಾ load_args));
    *ptr = args;
    ಯದಿ(pthread_create(&thread, 0, load_threads, ptr)) error("Thread creation failed");
    ಪ್ರತ್ಯಾಗಮ thread;
}

data load_data_writing(ವರ್ಣ **paths, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ m, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h, ಪೂರ್ಣಾಙ್ಕ out_w, ಪೂರ್ಣಾಙ್ಕ out_h)
{
    ಯದಿ(m) paths = get_random_paths(paths, n, m);
    ವರ್ಣ **replace_paths = find_replace_paths(paths, n, ".png", "-label.png");
    data d = {0};
    d.shallow = 0;
    d.X = load_image_paths(paths, n, w, h);
    d.y = load_image_paths_gray(replace_paths, n, out_w, out_h);
    ಯದಿ(m) ಮುಕ್ತ(paths);
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < n; ++i) ಮುಕ್ತ(replace_paths[i]);
    ಮುಕ್ತ(replace_paths);
    ಪ್ರತ್ಯಾಗಮ d;
}

data load_data_old(ವರ್ಣ **paths, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ m, ವರ್ಣ **labels, ಪೂರ್ಣಾಙ್ಕ k, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h)
{
    ಯದಿ(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;
    d.X = load_image_paths(paths, n, w, h);
    d.y = load_labels_paths(paths, n, labels, k, 0);
    ಯದಿ(m) ಮುಕ್ತ(paths);
    ಪ್ರತ್ಯಾಗಮ d;
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

data load_data_super(ವರ್ಣ **paths, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ m, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h, ಪೂರ್ಣಾಙ್ಕ scale)
{
    ಯದಿ(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;

    ಪೂರ್ಣಾಙ್ಕ i;
    d.X.rows = n;
    d.X.vals = calloc(n, ಪರಿಮಾಣ(ಭಿನ್ನ*));
    d.X.cols = w*h*3;

    d.y.rows = n;
    d.y.vals = calloc(n, ಪರಿಮಾಣ(ಭಿನ್ನ*));
    d.y.cols = w*scale * h*scale * 3;

    ಪ್ರತಿ(i = 0; i < n; ++i){
        image im = load_image_color(paths[i], 0, 0);
        image crop = random_crop_image(im, w*scale, h*scale);
        ಪೂರ್ಣಾಙ್ಕ flip = rand()%2;
        ಯದಿ (flip) flip_image(crop);
        image resize = resize_image(crop, w, h);
        d.X.vals[i] = resize.data;
        d.y.vals[i] = crop.data;
        free_image(im);
    }

    ಯದಿ(m) ಮುಕ್ತ(paths);
    ಪ್ರತ್ಯಾಗಮ d;
}

data load_data_regression(ವರ್ಣ **paths, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ m, ಪೂರ್ಣಾಙ್ಕ k, ಪೂರ್ಣಾಙ್ಕ min, ಪೂರ್ಣಾಙ್ಕ max, ಪೂರ್ಣಾಙ್ಕ size, ಭಿನ್ನ angle, ಭಿನ್ನ aspect, ಭಿನ್ನ hue, ಭಿನ್ನ saturation, ಭಿನ್ನ exposure)
{
    ಯದಿ(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;
    d.X = load_image_augment_paths(paths, n, min, max, size, angle, aspect, hue, saturation, exposure, 0);
    d.y = load_regression_labels_paths(paths, n, k);
    ಯದಿ(m) ಮುಕ್ತ(paths);
    ಪ್ರತ್ಯಾಗಮ d;
}

data select_data(data *orig, ಪೂರ್ಣಾಙ್ಕ *inds)
{
    data d = {0};
    d.shallow = 1;
    d.w = orig[0].w;
    d.h = orig[0].h;

    d.X.rows = orig[0].X.rows;
    d.y.rows = orig[0].X.rows;

    d.X.cols = orig[0].X.cols;
    d.y.cols = orig[0].y.cols;

    d.X.vals = calloc(orig[0].X.rows, ಪರಿಮಾಣ(ಭಿನ್ನ *));
    d.y.vals = calloc(orig[0].y.rows, ಪರಿಮಾಣ(ಭಿನ್ನ *));
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < d.X.rows; ++i){
        d.X.vals[i] = orig[inds[i]].X.vals[i];
        d.y.vals[i] = orig[inds[i]].y.vals[i];
    }
    ಪ್ರತ್ಯಾಗಮ d;
}

data *tile_data(data orig, ಪೂರ್ಣಾಙ್ಕ divs, ಪೂರ್ಣಾಙ್ಕ size)
{
    data *ds = calloc(divs*divs, ಪರಿಮಾಣ(data));
    ಪೂರ್ಣಾಙ್ಕ i, j;
#pragma omp parallel ಪ್ರತಿ
    ಪ್ರತಿ(i = 0; i < divs*divs; ++i){
        data d;
        d.shallow = 0;
        d.w = orig.w/divs * size;
        d.h = orig.h/divs * size;
        d.X.rows = orig.X.rows;
        d.X.cols = d.w*d.h*3;
        d.X.vals = calloc(d.X.rows, ಪರಿಮಾಣ(ಭಿನ್ನ*));

        d.y = copy_matrix(orig.y);
#pragma omp parallel ಪ್ರತಿ
        ಪ್ರತಿ(j = 0; j < orig.X.rows; ++j){
            ಪೂರ್ಣಾಙ್ಕ x = (i%divs) * orig.w / divs - (d.w - orig.w/divs)/2;
            ಪೂರ್ಣಾಙ್ಕ y = (i/divs) * orig.h / divs - (d.h - orig.h/divs)/2;
            image im = float_to_image(orig.w, orig.h, 3, orig.X.vals[j]);
            d.X.vals[j] = crop_image(im, x, y, d.w, d.h).data;
        }
        ds[i] = d;
    }
    ಪ್ರತ್ಯಾಗಮ ds;
}

data resize_data(data orig, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h)
{
    data d = {0};
    d.shallow = 0;
    d.w = w;
    d.h = h;
    ಪೂರ್ಣಾಙ್ಕ i;
    d.X.rows = orig.X.rows;
    d.X.cols = w*h*3;
    d.X.vals = calloc(d.X.rows, ಪರಿಮಾಣ(ಭಿನ್ನ*));

    d.y = copy_matrix(orig.y);
#pragma omp parallel ಪ್ರತಿ
    ಪ್ರತಿ(i = 0; i < orig.X.rows; ++i){
        image im = float_to_image(orig.w, orig.h, 3, orig.X.vals[i]);
        d.X.vals[i] = resize_image(im, w, h).data;
    }
    ಪ್ರತ್ಯಾಗಮ d;
}

data load_data_augment(ವರ್ಣ **paths, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ m, ವರ್ಣ **labels, ಪೂರ್ಣಾಙ್ಕ k, ವೃಕ್ಷ *hierarchy, ಪೂರ್ಣಾಙ್ಕ min, ಪೂರ್ಣಾಙ್ಕ max, ಪೂರ್ಣಾಙ್ಕ size, ಭಿನ್ನ angle, ಭಿನ್ನ aspect, ಭಿನ್ನ hue, ಭಿನ್ನ saturation, ಭಿನ್ನ exposure, ಪೂರ್ಣಾಙ್ಕ center)
{
    ಯದಿ(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;
    d.w=size;
    d.h=size;
    d.X = load_image_augment_paths(paths, n, min, max, size, angle, aspect, hue, saturation, exposure, center);
    d.y = load_labels_paths(paths, n, labels, k, hierarchy);
    ಯದಿ(m) ಮುಕ್ತ(paths);
    ಪ್ರತ್ಯಾಗಮ d;
}

data load_data_tag(ವರ್ಣ **paths, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ m, ಪೂರ್ಣಾಙ್ಕ k, ಪೂರ್ಣಾಙ್ಕ min, ಪೂರ್ಣಾಙ್ಕ max, ಪೂರ್ಣಾಙ್ಕ size, ಭಿನ್ನ angle, ಭಿನ್ನ aspect, ಭಿನ್ನ hue, ಭಿನ್ನ saturation, ಭಿನ್ನ exposure)
{
    ಯದಿ(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.w = size;
    d.h = size;
    d.shallow = 0;
    d.X = load_image_augment_paths(paths, n, min, max, size, angle, aspect, hue, saturation, exposure, 0);
    d.y = load_tags_paths(paths, n, k);
    ಯದಿ(m) ಮುಕ್ತ(paths);
    ಪ್ರತ್ಯಾಗಮ d;
}

matrix concat_matrix(matrix m1, matrix m2)
{
    ಪೂರ್ಣಾಙ್ಕ i, ಎಣಿಕೆ = 0;
    matrix m;
    m.cols = m1.cols;
    m.rows = m1.rows+m2.rows;
    m.vals = calloc(m1.rows + m2.rows, ಪರಿಮಾಣ(ಭಿನ್ನ*));
    ಪ್ರತಿ(i = 0; i < m1.rows; ++i){
        m.vals[ಎಣಿಕೆ++] = m1.vals[i];
    }
    ಪ್ರತಿ(i = 0; i < m2.rows; ++i){
        m.vals[ಎಣಿಕೆ++] = m2.vals[i];
    }
    ಪ್ರತ್ಯಾಗಮ m;
}

data concat_data(data d1, data d2)
{
    data d = {0};
    d.shallow = 1;
    d.X = concat_matrix(d1.X, d2.X);
    d.y = concat_matrix(d1.y, d2.y);
    d.w = d1.w;
    d.h = d1.h;
    ಪ್ರತ್ಯಾಗಮ d;
}

data concat_datas(data *d, ಪೂರ್ಣಾಙ್ಕ n)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    data out = {0};
    ಪ್ರತಿ(i = 0; i < n; ++i){
        data new = concat_data(d[i], out);
        free_data(out);
        out = new;
    }
    ಪ್ರತ್ಯಾಗಮ out;
}

data load_categorical_data_csv(ವರ್ಣ *filename, ಪೂರ್ಣಾಙ್ಕ target, ಪೂರ್ಣಾಙ್ಕ k)
{
    data d = {0};
    d.shallow = 0;
    matrix X = csv_to_matrix(filename);
    ಭಿನ್ನ *truth_1d = ತೆಗೆ_column(&X, target);
    ಭಿನ್ನ **truth = one_hot_encode(truth_1d, X.rows, k);
    matrix y;
    y.rows = X.rows;
    y.cols = k;
    y.vals = truth;
    d.X = X;
    d.y = y;
    ಮುಕ್ತ(truth_1d);
    ಪ್ರತ್ಯಾಗಮ d;
}

data load_cifar10_data(ವರ್ಣ *filename)
{
    data d = {0};
    d.shallow = 0;
    long i,j;
    matrix X = make_matrix(10000, 3072);
    matrix y = make_matrix(10000, 10);
    d.X = X;
    d.y = y;

    FILE *fp = fopen(filename, "rb");
    ಯದಿ(!fp) file_error(filename);
    ಪ್ರತಿ(i = 0; i < 10000; ++i){
        unsigned ವರ್ಣ bytes[3073];
        fread(bytes, 1, 3073, fp);
        ಪೂರ್ಣಾಙ್ಕ class = bytes[0];
        y.vals[i][class] = 1;
        ಪ್ರತಿ(j = 0; j < X.cols; ++j){
            X.vals[i][j] = (ದ್ವಿಗುಣ)bytes[j+1];
        }
    }
    scale_data_rows(d, 1./255);
    //normalize_data_rows(d);
    fclose(fp);
    ಪ್ರತ್ಯಾಗಮ d;
}

ಶೂನ್ಯ get_random_batch(data d, ಪೂರ್ಣಾಙ್ಕ n, ಭಿನ್ನ *X, ಭಿನ್ನ *y)
{
    ಪೂರ್ಣಾಙ್ಕ j;
    ಪ್ರತಿ(j = 0; j < n; ++j){
        ಪೂರ್ಣಾಙ್ಕ index = rand()%d.X.rows;
        memcpy(X+j*d.X.cols, d.X.vals[index], d.X.cols*ಪರಿಮಾಣ(ಭಿನ್ನ));
        memcpy(y+j*d.y.cols, d.y.vals[index], d.y.cols*ಪರಿಮಾಣ(ಭಿನ್ನ));
    }
}

ಶೂನ್ಯ get_next_batch(data d, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ offset, ಭಿನ್ನ *X, ಭಿನ್ನ *y)
{
    ಪೂರ್ಣಾಙ್ಕ j;
    ಪ್ರತಿ(j = 0; j < n; ++j){
        ಪೂರ್ಣಾಙ್ಕ index = offset + j;
        memcpy(X+j*d.X.cols, d.X.vals[index], d.X.cols*ಪರಿಮಾಣ(ಭಿನ್ನ));
        ಯದಿ(y) memcpy(y+j*d.y.cols, d.y.vals[index], d.y.cols*ಪರಿಮಾಣ(ಭಿನ್ನ));
    }
}

ಶೂನ್ಯ smooth_data(data d)
{
    ಪೂರ್ಣಾಙ್ಕ i, j;
    ಭಿನ್ನ scale = 1. / d.y.cols;
    ಭಿನ್ನ eps = .1;
    ಪ್ರತಿ(i = 0; i < d.y.rows; ++i){
        ಪ್ರತಿ(j = 0; j < d.y.cols; ++j){
            d.y.vals[i][j] = eps * scale + (1-eps) * d.y.vals[i][j];
        }
    }
}

data load_all_cifar10()
{
    data d = {0};
    d.shallow = 0;
    ಪೂರ್ಣಾಙ್ಕ i,j,b;
    matrix X = make_matrix(50000, 3072);
    matrix y = make_matrix(50000, 10);
    d.X = X;
    d.y = y;


    ಪ್ರತಿ(b = 0; b < 5; ++b){
        ವರ್ಣ buff[256];
        sprintf(buff, "data/cifar/cifar-10-batches-bin/data_batch_%d.bin", b+1);
        FILE *fp = fopen(buff, "rb");
        ಯದಿ(!fp) file_error(buff);
        ಪ್ರತಿ(i = 0; i < 10000; ++i){
            unsigned ವರ್ಣ bytes[3073];
            fread(bytes, 1, 3073, fp);
            ಪೂರ್ಣಾಙ್ಕ class = bytes[0];
            y.vals[i+b*10000][class] = 1;
            ಪ್ರತಿ(j = 0; j < X.cols; ++j){
                X.vals[i+b*10000][j] = (ದ್ವಿಗುಣ)bytes[j+1];
            }
        }
        fclose(fp);
    }
    //normalize_data_rows(d);
    scale_data_rows(d, 1./255);
    smooth_data(d);
    ಪ್ರತ್ಯಾಗಮ d;
}

data load_go(ವರ್ಣ *filename)
{
    FILE *fp = fopen(filename, "rb");
    matrix X = make_matrix(3363059, 361);
    matrix y = make_matrix(3363059, 361);
    ಪೂರ್ಣಾಙ್ಕ row, col;

    ಯದಿ(!fp) file_error(filename);
    ವರ್ಣ *label;
    ಪೂರ್ಣಾಙ್ಕ ಎಣಿಕೆ = 0;
    ಯಾವತ್((label = fgetl(fp))){
        ಪೂರ್ಣಾಙ್ಕ i;
        ಯದಿ(ಎಣಿಕೆ == X.rows){
            X = resize_matrix(X, ಎಣಿಕೆ*2);
            y = resize_matrix(y, ಎಣಿಕೆ*2);
        }
        sscanf(label, "%d %d", &row, &col);
        ವರ್ಣ *board = fgetl(fp);

        ಪೂರ್ಣಾಙ್ಕ index = row*19 + col;
        y.vals[ಎಣಿಕೆ][index] = 1;

        ಪ್ರತಿ(i = 0; i < 19*19; ++i){
            ಭಿನ್ನ val = 0;
            ಯದಿ(board[i] == '1') val = 1;
            ಅನ್ಯಥಾ ಯದಿ(board[i] == '2') val = -1;
            X.vals[ಎಣಿಕೆ][i] = val;
        }
        ++ಎಣಿಕೆ;
        ಮುಕ್ತ(label);
        ಮುಕ್ತ(board);
    }
    X = resize_matrix(X, ಎಣಿಕೆ);
    y = resize_matrix(y, ಎಣಿಕೆ);

    data d = {0};
    d.shallow = 0;
    d.X = X;
    d.y = y;


    fclose(fp);

    ಪ್ರತ್ಯಾಗಮ d;
}


ಶೂನ್ಯ randomize_data(data d)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = d.X.rows-1; i > 0; --i){
        ಪೂರ್ಣಾಙ್ಕ index = rand()%i;
        ಭಿನ್ನ *swap = d.X.vals[index];
        d.X.vals[index] = d.X.vals[i];
        d.X.vals[i] = swap;

        swap = d.y.vals[index];
        d.y.vals[index] = d.y.vals[i];
        d.y.vals[i] = swap;
    }
}

ಶೂನ್ಯ scale_data_rows(data d, ಭಿನ್ನ s)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < d.X.rows; ++i){
        scale_array(d.X.vals[i], d.X.cols, s);
    }
}

ಶೂನ್ಯ translate_data_rows(data d, ಭಿನ್ನ s)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < d.X.rows; ++i){
        translate_array(d.X.vals[i], d.X.cols, s);
    }
}

data copy_data(data d)
{
    data c = {0};
    c.w = d.w;
    c.h = d.h;
    c.shallow = 0;
    c.ಅಂಕೆ_boxes = d.ಅಂಕೆ_boxes;
    c.boxes = d.boxes;
    c.X = copy_matrix(d.X);
    c.y = copy_matrix(d.y);
    ಪ್ರತ್ಯಾಗಮ c;
}

ಶೂನ್ಯ normalize_data_rows(data d)
{
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < d.X.rows; ++i){
        normalize_array(d.X.vals[i], d.X.cols);
    }
}

data get_data_part(data d, ಪೂರ್ಣಾಙ್ಕ part, ಪೂರ್ಣಾಙ್ಕ total)
{
    data p = {0};
    p.shallow = 1;
    p.X.rows = d.X.rows * (part + 1) / total - d.X.rows * part / total;
    p.y.rows = d.y.rows * (part + 1) / total - d.y.rows * part / total;
    p.X.cols = d.X.cols;
    p.y.cols = d.y.cols;
    p.X.vals = d.X.vals + d.X.rows * part / total;
    p.y.vals = d.y.vals + d.y.rows * part / total;
    ಪ್ರತ್ಯಾಗಮ p;
}

data get_random_data(data d, ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ)
{
    data r = {0};
    r.shallow = 1;

    r.X.rows = ಅಂಕೆ;
    r.y.rows = ಅಂಕೆ;

    r.X.cols = d.X.cols;
    r.y.cols = d.y.cols;

    r.X.vals = calloc(ಅಂಕೆ, ಪರಿಮಾಣ(ಭಿನ್ನ *));
    r.y.vals = calloc(ಅಂಕೆ, ಪರಿಮಾಣ(ಭಿನ್ನ *));

    ಪೂರ್ಣಾಙ್ಕ i;
    ಪ್ರತಿ(i = 0; i < ಅಂಕೆ; ++i){
        ಪೂರ್ಣಾಙ್ಕ index = rand()%d.X.rows;
        r.X.vals[i] = d.X.vals[index];
        r.y.vals[i] = d.y.vals[index];
    }
    ಪ್ರತ್ಯಾಗಮ r;
}

data *split_data(data d, ಪೂರ್ಣಾಙ್ಕ part, ಪೂರ್ಣಾಙ್ಕ total)
{
    data *split = calloc(2, ಪರಿಮಾಣ(data));
    ಪೂರ್ಣಾಙ್ಕ i;
    ಪೂರ್ಣಾಙ್ಕ start = part*d.X.rows/total;
    ಪೂರ್ಣಾಙ್ಕ end = (part+1)*d.X.rows/total;
    data train;
    data test;
    train.shallow = test.shallow = 1;

    test.X.rows = test.y.rows = end-start;
    train.X.rows = train.y.rows = d.X.rows - (end-start);
    train.X.cols = test.X.cols = d.X.cols;
    train.y.cols = test.y.cols = d.y.cols;

    train.X.vals = calloc(train.X.rows, ಪರಿಮಾಣ(ಭಿನ್ನ*));
    test.X.vals = calloc(test.X.rows, ಪರಿಮಾಣ(ಭಿನ್ನ*));
    train.y.vals = calloc(train.y.rows, ಪರಿಮಾಣ(ಭಿನ್ನ*));
    test.y.vals = calloc(test.y.rows, ಪರಿಮಾಣ(ಭಿನ್ನ*));

    ಪ್ರತಿ(i = 0; i < start; ++i){
        train.X.vals[i] = d.X.vals[i];
        train.y.vals[i] = d.y.vals[i];
    }
    ಪ್ರತಿ(i = start; i < end; ++i){
        test.X.vals[i-start] = d.X.vals[i];
        test.y.vals[i-start] = d.y.vals[i];
    }
    ಪ್ರತಿ(i = end; i < d.X.rows; ++i){
        train.X.vals[i-(end-start)] = d.X.vals[i];
        train.y.vals[i-(end-start)] = d.y.vals[i];
    }
    split[0] = train;
    split[1] = test;
    ಪ್ರತ್ಯಾಗಮ split;
}

