#ଅନ୍ତର୍ଭାଵ "data.h"
#ଅନ୍ତର୍ଭାଵ "utils.h"
#ଅନ୍ତର୍ଭାଵ "image.h"
#ଅନ୍ତର୍ଭାଵ "cuda.h"

#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#ଅନ୍ତର୍ଭାଵ <string.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

list *get_paths(ଅକ୍ଷର *filename)
{
    ଅକ୍ଷର *path;
    FILE *file = fopen(filename, "r");
    ଯଦି(!file) file_error(filename);
    list *ଧାଡ଼ିଗୁଡ଼ିକ = make_list();
    ଯେତେବେଳେ((path=fgetl(file))){
        list_ଭର୍ତ୍ତି(ଧାଡ଼ିଗୁଡ଼ିକ, path);
    }
    fclose(file);
    ଫେରନ୍ତୁ ଧାଡ଼ିଗୁଡ଼ିକ;
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

ଅକ୍ଷର **get_random_paths(ଅକ୍ଷର **paths, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m)
{
    ଅକ୍ଷର **random_paths = calloc(n, ଆକାର(ଅକ୍ଷର*));
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    pthread_mutex_lock(&mutex);
    ପାଇଁ(i = 0; i < n; ++i){
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = rand()%m;
        random_paths[i] = paths[index];
        //if(i == 0) printf("%s\n", paths[index]);
    }
    pthread_mutex_unlock(&mutex);
    ଫେରନ୍ତୁ random_paths;
}

ଅକ୍ଷର **find_replace_paths(ଅକ୍ଷର **paths, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ଅକ୍ଷର *find, ଅକ୍ଷର *replace)
{
    ଅକ୍ଷର **replace_paths = calloc(n, ଆକାର(ଅକ୍ଷର*));
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < n; ++i){
        ଅକ୍ଷର replaced[4096];
        find_replace(paths[i], find, replace, replaced);
        replace_paths[i] = copy_string(replaced);
    }
    ଫେରନ୍ତୁ replace_paths;
}

matrix load_image_paths_gray(ଅକ୍ଷର **paths, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    matrix X;
    X.rows = n;
    X.vals = calloc(X.rows, ଆକାର(ଦଶମିକ*));
    X.cols = 0;

    ପାଇଁ(i = 0; i < n; ++i){
        image im = load_image(paths[i], w, h, 3);

        image gray = grayscale_image(im);
        free_image(im);
        im = gray;

        X.vals[i] = im.data;
        X.cols = im.h*im.w*im.c;
    }
    ଫେରନ୍ତୁ X;
}

matrix load_image_paths(ଅକ୍ଷର **paths, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    matrix X;
    X.rows = n;
    X.vals = calloc(X.rows, ଆକାର(ଦଶମିକ*));
    X.cols = 0;

    ପାଇଁ(i = 0; i < n; ++i){
        image im = load_image_color(paths[i], w, h);
        X.vals[i] = im.data;
        X.cols = im.h*im.w*im.c;
    }
    ଫେରନ୍ତୁ X;
}

matrix load_image_augment_paths(ଅକ୍ଷର **paths, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା min, ପୂର୍ଣ୍ଣସଂଖ୍ୟା max, ପୂର୍ଣ୍ଣସଂଖ୍ୟା size, ଦଶମିକ angle, ଦଶମିକ aspect, ଦଶମିକ hue, ଦଶମିକ saturation, ଦଶମିକ exposure, ପୂର୍ଣ୍ଣସଂଖ୍ୟା center)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    matrix X;
    X.rows = n;
    X.vals = calloc(X.rows, ଆକାର(ଦଶମିକ*));
    X.cols = 0;

    ପାଇଁ(i = 0; i < n; ++i){
        image im = load_image_color(paths[i], 0, 0);
        image crop;
        ଯଦି(center){
            crop = center_crop_image(im, size, size);
        } ଅନ୍ୟଥା {
            crop = random_augment_image(im, angle, aspect, min, max, size, size);
        }
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା flip = rand()%2;
        ଯଦି (flip) flip_image(crop);
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
    ଫେରନ୍ତୁ X;
}


box_label *read_boxes(ଅକ୍ଷର *filename, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *n)
{
    FILE *file = fopen(filename, "r");
    ଯଦି(!file) file_error(filename);
    ଦଶମିକ x, y, h, w;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା id;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା size = 64;
    box_label *boxes = calloc(size, ଆକାର(box_label));
    ଯେତେବେଳେ(fscanf(file, "%d %f %f %f %f", &id, &x, &y, &w, &h) == 5){
        ଯଦି(ଗଣନା_2 == size) {
            size = size * 2;
            boxes = realloc(boxes, size*ଆକାର(box_label));
        }
        boxes[ଗଣନା_2].id = id;
        boxes[ଗଣନା_2].x = x;
        boxes[ଗଣନା_2].y = y;
        boxes[ଗଣନା_2].h = h;
        boxes[ଗଣନା_2].w = w;
        boxes[ଗଣନା_2].ବାମ   = x - w/2;
        boxes[ଗଣନା_2].ଡାହାଣ  = x + w/2;
        boxes[ଗଣନା_2].ଶୀର୍ଷ    = y - h/2;
        boxes[ଗଣନା_2].bottom = y + h/2;
        ++ଗଣନା_2;
    }
    fclose(file);
    *n = ଗଣନା_2;
    ଫେରନ୍ତୁ boxes;
}

ଶୂନ୍ୟ randomize_boxes(box_label *b, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < n; ++i){
        box_label swap = b[i];
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = rand()%n;
        b[i] = b[index];
        b[index] = swap;
    }
}

ଶୂନ୍ୟ correct_boxes(box_label *boxes, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ଦଶମିକ dx, ଦଶମିକ dy, ଦଶମିକ sx, ଦଶମିକ sy, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flip)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < n; ++i){
        ଯଦି(boxes[i].x == 0 && boxes[i].y == 0) {
            boxes[i].x = 999999;
            boxes[i].y = 999999;
            boxes[i].w = 999999;
            boxes[i].h = 999999;
            ଜାରି;
        }
        boxes[i].ବାମ   = boxes[i].ବାମ  * sx - dx;
        boxes[i].ଡାହାଣ  = boxes[i].ଡାହାଣ * sx - dx;
        boxes[i].ଶୀର୍ଷ    = boxes[i].ଶୀର୍ଷ   * sy - dy;
        boxes[i].bottom = boxes[i].bottom* sy - dy;

        ଯଦି(flip){
            ଦଶମିକ swap = boxes[i].ବାମ;
            boxes[i].ବାମ = 1. - boxes[i].ଡାହାଣ;
            boxes[i].ଡାହାଣ = 1. - swap;
        }

        boxes[i].ବାମ =  constrain(0, 1, boxes[i].ବାମ);
        boxes[i].ଡାହାଣ = constrain(0, 1, boxes[i].ଡାହାଣ);
        boxes[i].ଶୀର୍ଷ =   constrain(0, 1, boxes[i].ଶୀର୍ଷ);
        boxes[i].bottom =   constrain(0, 1, boxes[i].bottom);

        boxes[i].x = (boxes[i].ବାମ+boxes[i].ଡାହାଣ)/2;
        boxes[i].y = (boxes[i].ଶୀର୍ଷ+boxes[i].bottom)/2;
        boxes[i].w = (boxes[i].ଡାହାଣ - boxes[i].ବାମ);
        boxes[i].h = (boxes[i].bottom - boxes[i].ଶୀର୍ଷ);

        boxes[i].w = constrain(0, 1, boxes[i].w);
        boxes[i].h = constrain(0, 1, boxes[i].h);
    }
}

ଶୂନ୍ୟ fill_truth_swag(ଅକ୍ଷର *path, ଦଶମିକ *truth, ପୂର୍ଣ୍ଣସଂଖ୍ୟା classes, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flip, ଦଶମିକ dx, ଦଶମିକ dy, ଦଶମିକ sx, ଦଶମିକ sy)
{
    ଅକ୍ଷର labelpath[4096];
    find_replace(path, "images", "labels", labelpath);
    find_replace(labelpath, "JPEGImages", "labels", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = 0;
    box_label *boxes = read_boxes(labelpath, &ଗଣନା_2);
    randomize_boxes(boxes, ଗଣନା_2);
    correct_boxes(boxes, ଗଣନା_2, dx, dy, sx, sy, flip);
    ଦଶମିକ x,y,w,h;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା id;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

    ପାଇଁ (i = 0; i < ଗଣନା_2 && i < 90; ++i) {
        x =  boxes[i].x;
        y =  boxes[i].y;
        w =  boxes[i].w;
        h =  boxes[i].h;
        id = boxes[i].id;

        ଯଦି (w < .0 || h < .0) ଜାରି;

        ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = (4+classes) * i;

        truth[index++] = x;
        truth[index++] = y;
        truth[index++] = w;
        truth[index++] = h;

        ଯଦି (id < classes) truth[index+id] = 1;
    }
    ମୁକ୍ତ(boxes);
}

ଶୂନ୍ୟ fill_truth_region(ଅକ୍ଷର *path, ଦଶମିକ *truth, ପୂର୍ଣ୍ଣସଂଖ୍ୟା classes, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_boxes, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flip, ଦଶମିକ dx, ଦଶମିକ dy, ଦଶମିକ sx, ଦଶମିକ sy)
{
    ଅକ୍ଷର labelpath[4096];
    find_replace(path, "images", "labels", labelpath);
    find_replace(labelpath, "JPEGImages", "labels", labelpath);

    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".png", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = 0;
    box_label *boxes = read_boxes(labelpath, &ଗଣନା_2);
    randomize_boxes(boxes, ଗଣନା_2);
    correct_boxes(boxes, ଗଣନା_2, dx, dy, sx, sy, flip);
    ଦଶମିକ x,y,w,h;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା id;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

    ପାଇଁ (i = 0; i < ଗଣନା_2; ++i) {
        x =  boxes[i].x;
        y =  boxes[i].y;
        w =  boxes[i].w;
        h =  boxes[i].h;
        id = boxes[i].id;

        ଯଦି (w < .005 || h < .005) ଜାରି;

        ପୂର୍ଣ୍ଣସଂଖ୍ୟା col = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(x*ଅଙ୍କ_boxes);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା row = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)(y*ଅଙ୍କ_boxes);

        x = x*ଅଙ୍କ_boxes - col;
        y = y*ଅଙ୍କ_boxes - row;

        ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = (col+row*ଅଙ୍କ_boxes)*(5+classes);
        ଯଦି (truth[index]) ଜାରି;
        truth[index++] = 1;

        ଯଦି (id < classes) truth[index+id] = 1;
        index += classes;

        truth[index++] = x;
        truth[index++] = y;
        truth[index++] = w;
        truth[index++] = h;
    }
    ମୁକ୍ତ(boxes);
}

ଶୂନ୍ୟ load_rle(image im, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *rle, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା curr = 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i,j;
    ପାଇଁ(i = 0; i < n; ++i){
        ପାଇଁ(j = 0; j < rle[i]; ++j){
            im.data[ଗଣନା_2++] = curr;
        }
        curr = 1 - curr;
    }
    ପାଇଁ(; ଗଣନା_2 < im.h*im.w*im.c; ++ଗଣନା_2){
        im.data[ଗଣନା_2] = curr;
    }
}

ଶୂନ୍ୟ or_image(image src, image dest, ପୂର୍ଣ୍ଣସଂଖ୍ୟା c)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < src.w*src.h; ++i){
        ଯଦି(src.data[i]) dest.data[dest.w*dest.h*c + i] = 1;
    }
}

ଶୂନ୍ୟ exclusive_image(image src)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା k, j, i;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା s = src.w*src.h;
    ପାଇଁ(k = 0; k < src.c-1; ++k){
        ପାଇଁ(i = 0; i < s; ++i){
            ଯଦି (src.data[k*s + i]){
                ପାଇଁ(j = k+1; j < src.c; ++j){
                    src.data[j*s + i] = 0;
                }
            }
        }
    }
}

box bound_image(image im)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା x,y;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା minx = im.w;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା miny = im.h;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା maxx = 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା maxy = 0;
    ପାଇଁ(y = 0; y < im.h; ++y){
        ପାଇଁ(x = 0; x < im.w; ++x){
            ଯଦି(im.data[y*im.w + x]){
                minx = (x < minx) ? x : minx;
                miny = (y < miny) ? y : miny;
                maxx = (x > maxx) ? x : maxx;
                maxy = (y > maxy) ? y : maxy;
            }
        }
    }
    box b = {minx, miny, maxx-minx + 1, maxy-miny + 1};
    //printf("%f %f %f %f\n", b.x, b.y, b.w, b.h);
    ଫେରନ୍ତୁ b;
}

ଶୂନ୍ୟ fill_truth_iseg(ଅକ୍ଷର *path, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_boxes, ଦଶମିକ *truth, ପୂର୍ଣ୍ଣସଂଖ୍ୟା classes, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, augment_args aug, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flip, ପୂର୍ଣ୍ଣସଂଖ୍ୟା mw, ପୂର୍ଣ୍ଣସଂଖ୍ୟା mh)
{
    ଅକ୍ଷର labelpath[4096];
    find_replace(path, "images", "mask", labelpath);
    find_replace(labelpath, "JPEGImages", "mask", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    FILE *file = fopen(labelpath, "r");
    ଯଦି(!file) file_error(labelpath);
    ଅକ୍ଷର buff[32788];
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା id;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା j;
    image part = make_image(w, h, 1);
    ଯେତେବେଳେ((fscanf(file, "%d %s", &id, buff) == 2) && i < ଅଙ୍କ_boxes){
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = 0;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା *rle = read_intlist(buff, &n, 0);
        load_rle(part, rle, n);
        image sized = rotate_crop_image(part, aug.rad, aug.scale, aug.w, aug.h, aug.dx, aug.dy, aug.aspect);
        ଯଦି(flip) flip_image(sized);

        image mask = resize_image(sized, mw, mh);
        truth[i*(mw*mh+1)] = id;
        ପାଇଁ(j = 0; j < mw*mh; ++j){
            truth[i*(mw*mh + 1) + 1 + j] = mask.data[j];
        }
        ++i;

        free_image(mask);
        free_image(sized);
        ମୁକ୍ତ(rle);
    }
    ଯଦି(i < ଅଙ୍କ_boxes) truth[i*(mw*mh+1)] = -1;
    fclose(file);
    free_image(part);
}

ଶୂନ୍ୟ fill_truth_mask(ଅକ୍ଷର *path, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_boxes, ଦଶମିକ *truth, ପୂର୍ଣ୍ଣସଂଖ୍ୟା classes, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, augment_args aug, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flip, ପୂର୍ଣ୍ଣସଂଖ୍ୟା mw, ପୂର୍ଣ୍ଣସଂଖ୍ୟା mh)
{
    ଅକ୍ଷର labelpath[4096];
    find_replace(path, "images", "mask", labelpath);
    find_replace(labelpath, "JPEGImages", "mask", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    FILE *file = fopen(labelpath, "r");
    ଯଦି(!file) file_error(labelpath);
    ଅକ୍ଷର buff[32788];
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା id;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0;
    image part = make_image(w, h, 1);
    ଯେତେବେଳେ((fscanf(file, "%d %s", &id, buff) == 2) && i < ଅଙ୍କ_boxes){
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = 0;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା *rle = read_intlist(buff, &n, 0);
        load_rle(part, rle, n);
        image sized = rotate_crop_image(part, aug.rad, aug.scale, aug.w, aug.h, aug.dx, aug.dy, aug.aspect);
        ଯଦି(flip) flip_image(sized);
        box b = bound_image(sized);
        ଯଦି(b.w > 0){
            image crop = crop_image(sized, b.x, b.y, b.w, b.h);
            image mask = resize_image(crop, mw, mh);
            truth[i*(4 + mw*mh + 1) + 0] = (b.x + b.w/2.)/sized.w;
            truth[i*(4 + mw*mh + 1) + 1] = (b.y + b.h/2.)/sized.h;
            truth[i*(4 + mw*mh + 1) + 2] = b.w/sized.w;
            truth[i*(4 + mw*mh + 1) + 3] = b.h/sized.h;
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା j;
            ପାଇଁ(j = 0; j < mw*mh; ++j){
                truth[i*(4 + mw*mh + 1) + 4 + j] = mask.data[j];
            }
            truth[i*(4 + mw*mh + 1) + 4 + mw*mh] = id;
            free_image(crop);
            free_image(mask);
            ++i;
        }
        free_image(sized);
        ମୁକ୍ତ(rle);
    }
    fclose(file);
    free_image(part);
}


ଶୂନ୍ୟ fill_truth_detection(ଅକ୍ଷର *path, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_boxes, ଦଶମିକ *truth, ପୂର୍ଣ୍ଣସଂଖ୍ୟା classes, ପୂର୍ଣ୍ଣସଂଖ୍ୟା flip, ଦଶମିକ dx, ଦଶମିକ dy, ଦଶମିକ sx, ଦଶମିକ sy)
{
    ଅକ୍ଷର labelpath[4096];
    find_replace(path, "images", "labels", labelpath);
    find_replace(labelpath, "JPEGImages", "labels", labelpath);

    find_replace(labelpath, "raw", "labels", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".png", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = 0;
    box_label *boxes = read_boxes(labelpath, &ଗଣନା_2);
    randomize_boxes(boxes, ଗଣନା_2);
    correct_boxes(boxes, ଗଣନା_2, dx, dy, sx, sy, flip);
    ଯଦି(ଗଣନା_2 > ଅଙ୍କ_boxes) ଗଣନା_2 = ଅଙ୍କ_boxes;
    ଦଶମିକ x,y,w,h;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା id;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା sub = 0;

    ପାଇଁ (i = 0; i < ଗଣନା_2; ++i) {
        x =  boxes[i].x;
        y =  boxes[i].y;
        w =  boxes[i].w;
        h =  boxes[i].h;
        id = boxes[i].id;

        ଯଦି ((w < .001 || h < .001)) {
            ++sub;
            ଜାରି;
        }

        truth[(i-sub)*5+0] = x;
        truth[(i-sub)*5+1] = y;
        truth[(i-sub)*5+2] = w;
        truth[(i-sub)*5+3] = h;
        truth[(i-sub)*5+4] = id;
    }
    ମୁକ୍ତ(boxes);
}

#define NUMCHARS 37

ଶୂନ୍ୟ print_letters(ଦଶମିକ *pred, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < n; ++i){
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = max_index(pred+i*NUMCHARS, NUMCHARS);
        printf("%c", int_to_alphanum(index));
    }
    printf("\n");
}

ଶୂନ୍ୟ fill_truth_captcha(ଅକ୍ଷର *path, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ଦଶମିକ *truth)
{
    ଅକ୍ଷର *begin = strrchr(path, '/');
    ++begin;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < strlen(begin) && i < n && begin[i] != '.'; ++i){
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = alphanum_to_int(begin[i]);
        ଯଦି(index > 35) printf("Bad %c\n", begin[i]);
        truth[i*NUMCHARS+index] = 1;
    }
    ପାଇଁ(;i < n; ++i){
        truth[i*NUMCHARS + NUMCHARS-1] = 1;
    }
}

data load_data_captcha(ଅକ୍ଷର **paths, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା k, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h)
{
    ଯଦି(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;
    d.X = load_image_paths(paths, n, w, h);
    d.y = make_matrix(n, k*NUMCHARS);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < n; ++i){
        fill_truth_captcha(paths[i], k, d.y.vals[i]);
    }
    ଯଦି(m) ମୁକ୍ତ(paths);
    ଫେରନ୍ତୁ d;
}

data load_data_captcha_encode(ଅକ୍ଷର **paths, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h)
{
    ଯଦି(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;
    d.X = load_image_paths(paths, n, w, h);
    d.X.cols = 17100;
    d.y = d.X;
    ଯଦି(m) ମୁକ୍ତ(paths);
    ଫେରନ୍ତୁ d;
}

ଶୂନ୍ୟ fill_truth(ଅକ୍ଷର *path, ଅକ୍ଷର **labels, ପୂର୍ଣ୍ଣସଂଖ୍ୟା k, ଦଶମିକ *truth)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    memset(truth, 0, k*ଆକାର(ଦଶମିକ));
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = 0;
    ପାଇଁ(i = 0; i < k; ++i){
        ଯଦି(strstr(path, labels[i])){
            truth[i] = 1;
            ++ଗଣନା_2;
            //printf("%s %s %d\n", path, labels[i], i);
        }
    }
    ଯଦି(ଗଣନା_2 != 1 && (k != 1 || ଗଣନା_2 != 0)) printf("Too many or too few labels: %d, %s\n", ଗଣନା_2, path);
}

ଶୂନ୍ୟ fill_hierarchy(ଦଶମିକ *truth, ପୂର୍ଣ୍ଣସଂଖ୍ୟା k, ବୃକ୍ଷ *hierarchy)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା j;
    ପାଇଁ(j = 0; j < k; ++j){
        ଯଦି(truth[j]){
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା parent = hierarchy->parent[j];
            ଯେତେବେଳେ(parent >= 0){
                truth[parent] = 1;
                parent = hierarchy->parent[parent];
            }
        }
    }
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = 0;
    ପାଇଁ(j = 0; j < hierarchy->groups; ++j){
        //printf("%d\n", count);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା mask = 1;
        ପାଇଁ(i = 0; i < hierarchy->group_size[j]; ++i){
            ଯଦି(truth[ଗଣନା_2 + i]){
                mask = 0;
                ଭାଙ୍ଗ;
            }
        }
        ଯଦି (mask) {
            ପାଇଁ(i = 0; i < hierarchy->group_size[j]; ++i){
                truth[ଗଣନା_2 + i] = SECRET_ଅଙ୍କ;
            }
        }
        ଗଣନା_2 += hierarchy->group_size[j];
    }
}

matrix load_regression_labels_paths(ଅକ୍ଷର **paths, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା k)
{
    matrix y = make_matrix(n, k);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i,j;
    ପାଇଁ(i = 0; i < n; ++i){
        ଅକ୍ଷର labelpath[4096];
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
        ପାଇଁ(j = 0; j < k; ++j){
            fscanf(file, "%f", &(y.vals[i][j]));
        }
        fclose(file);
    }
    ଫେରନ୍ତୁ y;
}

matrix load_labels_paths(ଅକ୍ଷର **paths, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ଅକ୍ଷର **labels, ପୂର୍ଣ୍ଣସଂଖ୍ୟା k, ବୃକ୍ଷ *hierarchy)
{
    matrix y = make_matrix(n, k);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < n && labels; ++i){
        fill_truth(paths[i], labels, k, y.vals[i]);
        ଯଦି(hierarchy){
            fill_hierarchy(y.vals[i], k, hierarchy);
        }
    }
    ଫେରନ୍ତୁ y;
}

matrix load_tags_paths(ଅକ୍ଷର **paths, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା k)
{
    matrix y = make_matrix(n, k);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    //int count = 0;
    ପାଇଁ(i = 0; i < n; ++i){
        ଅକ୍ଷର label[4096];
        find_replace(paths[i], "images", "labels", label);
        find_replace(label, ".jpg", ".txt", label);
        FILE *file = fopen(label, "r");
        ଯଦି (!file) ଜାରି;
        //++count;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା tag;
        ଯେତେବେଳେ(fscanf(file, "%d", &tag) == 1){
            ଯଦି(tag < k){
                y.vals[i][tag] = 1;
            }
        }
        fclose(file);
    }
    //printf("%d/%d\n", count, n);
    ଫେରନ୍ତୁ y;
}

ଅକ୍ଷର **get_labels(ଅକ୍ଷର *filename)
{
    list *plist = get_paths(filename);
    ଅକ୍ଷର **labels = (ଅକ୍ଷର **)list_to_array(plist);
    free_list(plist);
    ଫେରନ୍ତୁ labels;
}

ଶୂନ୍ୟ free_data(data d)
{
    ଯଦି(!d.shallow){
        free_matrix(d.X);
        free_matrix(d.y);
    }ଅନ୍ୟଥା{
        ମୁକ୍ତ(d.X.vals);
        ମୁକ୍ତ(d.y.vals);
    }
}

image get_segmentation_image(ଅକ୍ଷର *path, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ପୂର୍ଣ୍ଣସଂଖ୍ୟା classes)
{
    ଅକ୍ଷର labelpath[4096];
    find_replace(path, "images", "mask", labelpath);
    find_replace(labelpath, "JPEGImages", "mask", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    image mask = make_image(w, h, classes);
    FILE *file = fopen(labelpath, "r");
    ଯଦି(!file) file_error(labelpath);
    ଅକ୍ଷର buff[32788];
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା id;
    image part = make_image(w, h, 1);
    ଯେତେବେଳେ(fscanf(file, "%d %s", &id, buff) == 2){
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = 0;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା *rle = read_intlist(buff, &n, 0);
        load_rle(part, rle, n);
        or_image(part, mask, id);
        ମୁକ୍ତ(rle);
    }
    //exclusive_image(mask);
    fclose(file);
    free_image(part);
    ଫେରନ୍ତୁ mask;
}

image get_segmentation_image2(ଅକ୍ଷର *path, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ପୂର୍ଣ୍ଣସଂଖ୍ୟା classes)
{
    ଅକ୍ଷର labelpath[4096];
    find_replace(path, "images", "mask", labelpath);
    find_replace(labelpath, "JPEGImages", "mask", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    image mask = make_image(w, h, classes+1);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < w*h; ++i){
        mask.data[w*h*classes + i] = 1;
    }
    FILE *file = fopen(labelpath, "r");
    ଯଦି(!file) file_error(labelpath);
    ଅକ୍ଷର buff[32788];
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା id;
    image part = make_image(w, h, 1);
    ଯେତେବେଳେ(fscanf(file, "%d %s", &id, buff) == 2){
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା n = 0;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା *rle = read_intlist(buff, &n, 0);
        load_rle(part, rle, n);
        or_image(part, mask, id);
        ପାଇଁ(i = 0; i < w*h; ++i){
            ଯଦି(part.data[i]) mask.data[w*h*classes + i] = 0;
        }
        ମୁକ୍ତ(rle);
    }
    //exclusive_image(mask);
    fclose(file);
    free_image(part);
    ଫେରନ୍ତୁ mask;
}

data load_data_seg(ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ଅକ୍ଷର **paths, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ପୂର୍ଣ୍ଣସଂଖ୍ୟା classes, ପୂର୍ଣ୍ଣସଂଖ୍ୟା min, ପୂର୍ଣ୍ଣସଂଖ୍ୟା max, ଦଶମିକ angle, ଦଶମିକ aspect, ଦଶମିକ hue, ଦଶମିକ saturation, ଦଶମିକ exposure, ପୂର୍ଣ୍ଣସଂଖ୍ୟା div)
{
    ଅକ୍ଷର **random_paths = get_random_paths(paths, n, m);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, ଆକାର(ଦଶମିକ*));
    d.X.cols = h*w*3;


    d.y.rows = n;
    d.y.cols = h*w*classes/div/div;
    d.y.vals = calloc(d.X.rows, ଆକାର(ଦଶମିକ*));

    ପାଇଁ(i = 0; i < n; ++i){
        image orig = load_image_color(random_paths[i], 0, 0);
        augment_args a = random_augment_args(orig, angle, aspect, min, max, w, h);
        image sized = rotate_crop_image(orig, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);

        ପୂର୍ଣ୍ଣସଂଖ୍ୟା flip = rand()%2;
        ଯଦି(flip) flip_image(sized);
        random_distort_image(sized, hue, saturation, exposure);
        d.X.vals[i] = sized.data;

        image mask = get_segmentation_image(random_paths[i], orig.w, orig.h, classes);
        //image mask = make_image(orig.w, orig.h, classes+1);
        image sized_m = rotate_crop_image(mask, a.rad, a.scale/div, a.w/div, a.h/div, a.dx/div, a.dy/div, a.aspect);

        ଯଦି(flip) flip_image(sized_m);
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
    ମୁକ୍ତ(random_paths);
    ଫେରନ୍ତୁ d;
}

data load_data_iseg(ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ଅକ୍ଷର **paths, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ପୂର୍ଣ୍ଣସଂଖ୍ୟା classes, ପୂର୍ଣ୍ଣସଂଖ୍ୟା boxes, ପୂର୍ଣ୍ଣସଂଖ୍ୟା div, ପୂର୍ଣ୍ଣସଂଖ୍ୟା min, ପୂର୍ଣ୍ଣସଂଖ୍ୟା max, ଦଶମିକ angle, ଦଶମିକ aspect, ଦଶମିକ hue, ଦଶମିକ saturation, ଦଶମିକ exposure)
{
    ଅକ୍ଷର **random_paths = get_random_paths(paths, n, m);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, ଆକାର(ଦଶମିକ*));
    d.X.cols = h*w*3;

    d.y = make_matrix(n, (((w/div)*(h/div))+1)*boxes);

    ପାଇଁ(i = 0; i < n; ++i){
        image orig = load_image_color(random_paths[i], 0, 0);
        augment_args a = random_augment_args(orig, angle, aspect, min, max, w, h);
        image sized = rotate_crop_image(orig, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);

        ପୂର୍ଣ୍ଣସଂଖ୍ୟା flip = rand()%2;
        ଯଦି(flip) flip_image(sized);
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
    ମୁକ୍ତ(random_paths);
    ଫେରନ୍ତୁ d;
}

data load_data_mask(ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ଅକ୍ଷର **paths, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ପୂର୍ଣ୍ଣସଂଖ୍ୟା classes, ପୂର୍ଣ୍ଣସଂଖ୍ୟା boxes, ପୂର୍ଣ୍ଣସଂଖ୍ୟା coords, ପୂର୍ଣ୍ଣସଂଖ୍ୟା min, ପୂର୍ଣ୍ଣସଂଖ୍ୟା max, ଦଶମିକ angle, ଦଶମିକ aspect, ଦଶମିକ hue, ଦଶମିକ saturation, ଦଶମିକ exposure)
{
    ଅକ୍ଷର **random_paths = get_random_paths(paths, n, m);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, ଆକାର(ଦଶମିକ*));
    d.X.cols = h*w*3;

    d.y = make_matrix(n, (coords+1)*boxes);

    ପାଇଁ(i = 0; i < n; ++i){
        image orig = load_image_color(random_paths[i], 0, 0);
        augment_args a = random_augment_args(orig, angle, aspect, min, max, w, h);
        image sized = rotate_crop_image(orig, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);

        ପୂର୍ଣ୍ଣସଂଖ୍ୟା flip = rand()%2;
        ଯଦି(flip) flip_image(sized);
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
    ମୁକ୍ତ(random_paths);
    ଫେରନ୍ତୁ d;
}

data load_data_region(ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ଅକ୍ଷର **paths, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ପୂର୍ଣ୍ଣସଂଖ୍ୟା size, ପୂର୍ଣ୍ଣସଂଖ୍ୟା classes, ଦଶମିକ jitter, ଦଶମିକ hue, ଦଶମିକ saturation, ଦଶମିକ exposure)
{
    ଅକ୍ଷର **random_paths = get_random_paths(paths, n, m);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, ଆକାର(ଦଶମିକ*));
    d.X.cols = h*w*3;


    ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = size*size*(5+classes);
    d.y = make_matrix(n, k);
    ପାଇଁ(i = 0; i < n; ++i){
        image orig = load_image_color(random_paths[i], 0, 0);

        ପୂର୍ଣ୍ଣସଂଖ୍ୟା oh = orig.h;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା ow = orig.w;

        ପୂର୍ଣ୍ଣସଂଖ୍ୟା dw = (ow*jitter);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା dh = (oh*jitter);

        ପୂର୍ଣ୍ଣସଂଖ୍ୟା pleft  = rand_uniform(-dw, dw);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା pright = rand_uniform(-dw, dw);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା ptop   = rand_uniform(-dh, dh);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା pbot   = rand_uniform(-dh, dh);

        ପୂର୍ଣ୍ଣସଂଖ୍ୟା swidth =  ow - pleft - pright;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା sheight = oh - ptop - pbot;

        ଦଶମିକ sx = (ଦଶମିକ)swidth  / ow;
        ଦଶମିକ sy = (ଦଶମିକ)sheight / oh;

        ପୂର୍ଣ୍ଣସଂଖ୍ୟା flip = rand()%2;
        image cropped = crop_image(orig, pleft, ptop, swidth, sheight);

        ଦଶମିକ dx = ((ଦଶମିକ)pleft/ow)/sx;
        ଦଶମିକ dy = ((ଦଶମିକ)ptop /oh)/sy;

        image sized = resize_image(cropped, w, h);
        ଯଦି(flip) flip_image(sized);
        random_distort_image(sized, hue, saturation, exposure);
        d.X.vals[i] = sized.data;

        fill_truth_region(random_paths[i], d.y.vals[i], classes, size, flip, dx, dy, 1./sx, 1./sy);

        free_image(orig);
        free_image(cropped);
    }
    ମୁକ୍ତ(random_paths);
    ଫେରନ୍ତୁ d;
}

data load_data_compare(ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ଅକ୍ଷର **paths, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା classes, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h)
{
    ଯଦି(m) paths = get_random_paths(paths, 2*n, m);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i,j;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, ଆକାର(ଦଶମିକ*));
    d.X.cols = h*w*6;

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = 2*(classes);
    d.y = make_matrix(n, k);
    ପାଇଁ(i = 0; i < n; ++i){
        image im1 = load_image_color(paths[i*2],   w, h);
        image im2 = load_image_color(paths[i*2+1], w, h);

        d.X.vals[i] = calloc(d.X.cols, ଆକାର(ଦଶମିକ));
        memcpy(d.X.vals[i],         im1.data, h*w*3*ଆକାର(ଦଶମିକ));
        memcpy(d.X.vals[i] + h*w*3, im2.data, h*w*3*ଆକାର(ଦଶମିକ));

        ପୂର୍ଣ୍ଣସଂଖ୍ୟା id;
        ଦଶମିକ iou;

        ଅକ୍ଷର imlabel1[4096];
        ଅକ୍ଷର imlabel2[4096];
        find_replace(paths[i*2],   "imgs", "labels", imlabel1);
        find_replace(imlabel1, "jpg", "txt", imlabel1);
        FILE *fp1 = fopen(imlabel1, "r");

        ଯେତେବେଳେ(fscanf(fp1, "%d %f", &id, &iou) == 2){
            ଯଦି (d.y.vals[i][2*id] < iou) d.y.vals[i][2*id] = iou;
        }

        find_replace(paths[i*2+1], "imgs", "labels", imlabel2);
        find_replace(imlabel2, "jpg", "txt", imlabel2);
        FILE *fp2 = fopen(imlabel2, "r");

        ଯେତେବେଳେ(fscanf(fp2, "%d %f", &id, &iou) == 2){
            ଯଦି (d.y.vals[i][2*id + 1] < iou) d.y.vals[i][2*id + 1] = iou;
        }

        ପାଇଁ (j = 0; j < classes; ++j){
            ଯଦି (d.y.vals[i][2*j] > .5 &&  d.y.vals[i][2*j+1] < .5){
                d.y.vals[i][2*j] = 1;
                d.y.vals[i][2*j+1] = 0;
            } ଅନ୍ୟଥା ଯଦି (d.y.vals[i][2*j] < .5 &&  d.y.vals[i][2*j+1] > .5){
                d.y.vals[i][2*j] = 0;
                d.y.vals[i][2*j+1] = 1;
            } ଅନ୍ୟଥା {
                d.y.vals[i][2*j]   = SECRET_ଅଙ୍କ;
                d.y.vals[i][2*j+1] = SECRET_ଅଙ୍କ;
            }
        }
        fclose(fp1);
        fclose(fp2);

        free_image(im1);
        free_image(im2);
    }
    ଯଦି(m) ମୁକ୍ତ(paths);
    ଫେରନ୍ତୁ d;
}

data load_data_swag(ଅକ୍ଷର **paths, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା classes, ଦଶମିକ jitter)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = rand()%n;
    ଅକ୍ଷର *random_path = paths[index];

    image orig = load_image_color(random_path, 0, 0);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା h = orig.h;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା w = orig.w;

    data d = {0};
    d.shallow = 0;
    d.w = w;
    d.h = h;

    d.X.rows = 1;
    d.X.vals = calloc(d.X.rows, ଆକାର(ଦଶମିକ*));
    d.X.cols = h*w*3;

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା k = (4+classes)*90;
    d.y = make_matrix(1, k);

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା dw = w*jitter;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା dh = h*jitter;

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା pleft  = rand_uniform(-dw, dw);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା pright = rand_uniform(-dw, dw);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ptop   = rand_uniform(-dh, dh);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା pbot   = rand_uniform(-dh, dh);

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା swidth =  w - pleft - pright;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା sheight = h - ptop - pbot;

    ଦଶମିକ sx = (ଦଶମିକ)swidth  / w;
    ଦଶମିକ sy = (ଦଶମିକ)sheight / h;

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା flip = rand()%2;
    image cropped = crop_image(orig, pleft, ptop, swidth, sheight);

    ଦଶମିକ dx = ((ଦଶମିକ)pleft/w)/sx;
    ଦଶମିକ dy = ((ଦଶମିକ)ptop /h)/sy;

    image sized = resize_image(cropped, w, h);
    ଯଦି(flip) flip_image(sized);
    d.X.vals[0] = sized.data;

    fill_truth_swag(random_path, d.y.vals[0], classes, flip, dx, dy, 1./sx, 1./sy);

    free_image(orig);
    free_image(cropped);

    ଫେରନ୍ତୁ d;
}

data load_data_detection(ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ଅକ୍ଷର **paths, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ପୂର୍ଣ୍ଣସଂଖ୍ୟା boxes, ପୂର୍ଣ୍ଣସଂଖ୍ୟା classes, ଦଶମିକ jitter, ଦଶମିକ hue, ଦଶମିକ saturation, ଦଶମିକ exposure)
{
    ଅକ୍ଷର **random_paths = get_random_paths(paths, n, m);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, ଆକାର(ଦଶମିକ*));
    d.X.cols = h*w*3;

    d.y = make_matrix(n, 5*boxes);
    ପାଇଁ(i = 0; i < n; ++i){
        image orig = load_image_color(random_paths[i], 0, 0);
        image sized = make_image(w, h, orig.c);
        fill_image(sized, .5);

        ଦଶମିକ dw = jitter * orig.w;
        ଦଶମିକ dh = jitter * orig.h;

        ଦଶମିକ new_ar = (orig.w + rand_uniform(-dw, dw)) / (orig.h + rand_uniform(-dh, dh));
        //float scale = rand_uniform(.25, 2);
        ଦଶମିକ scale = 1;

        ଦଶମିକ nw, nh;

        ଯଦି(new_ar < 1){
            nh = scale * h;
            nw = nh * new_ar;
        } ଅନ୍ୟଥା {
            nw = scale * w;
            nh = nw / new_ar;
        }

        ଦଶମିକ dx = rand_uniform(0, w - nw);
        ଦଶମିକ dy = rand_uniform(0, h - nh);

        place_image(orig, nw, nh, dx, dy, sized);

        random_distort_image(sized, hue, saturation, exposure);

        ପୂର୍ଣ୍ଣସଂଖ୍ୟା flip = rand()%2;
        ଯଦି(flip) flip_image(sized);
        d.X.vals[i] = sized.data;


        fill_truth_detection(random_paths[i], boxes, d.y.vals[i], classes, flip, -dx/w, -dy/h, nw/w, nh/h);

        free_image(orig);
    }
    ମୁକ୍ତ(random_paths);
    ଫେରନ୍ତୁ d;
}

ଶୂନ୍ୟ *load_thread(ଶୂନ୍ୟ *ptr)
{
    //printf("Loading data: %d\n", rand());
    load_args a = *(ସଂରଚନା load_args*)ptr;
    ଯଦି(a.exposure == 0) a.exposure = 1;
    ଯଦି(a.saturation == 0) a.saturation = 1;
    ଯଦି(a.aspect == 0) a.aspect = 1;

    ଯଦି (a.type == OLD_CLASSIFICATION_DATA){
        *a.d = load_data_old(a.paths, a.n, a.m, a.labels, a.classes, a.w, a.h);
    } ଅନ୍ୟଥା ଯଦି (a.type == REGRESSION_DATA){
        *a.d = load_data_regression(a.paths, a.n, a.m, a.classes, a.min, a.max, a.size, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    } ଅନ୍ୟଥା ଯଦି (a.type == CLASSIFICATION_DATA){
        *a.d = load_data_augment(a.paths, a.n, a.m, a.labels, a.classes, a.hierarchy, a.min, a.max, a.size, a.angle, a.aspect, a.hue, a.saturation, a.exposure, a.center);
    } ଅନ୍ୟଥା ଯଦି (a.type == SUPER_DATA){
        *a.d = load_data_super(a.paths, a.n, a.m, a.w, a.h, a.scale);
    } ଅନ୍ୟଥା ଯଦି (a.type == WRITING_DATA){
        *a.d = load_data_writing(a.paths, a.n, a.m, a.w, a.h, a.out_w, a.out_h);
    } ଅନ୍ୟଥା ଯଦି (a.type == ISEG_DATA){
        *a.d = load_data_iseg(a.n, a.paths, a.m, a.w, a.h, a.classes, a.ଅଙ୍କ_boxes, a.scale, a.min, a.max, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    } ଅନ୍ୟଥା ଯଦି (a.type == INSTANCE_DATA){
        *a.d = load_data_mask(a.n, a.paths, a.m, a.w, a.h, a.classes, a.ଅଙ୍କ_boxes, a.coords, a.min, a.max, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    } ଅନ୍ୟଥା ଯଦି (a.type == SEGMENTATION_DATA){
        *a.d = load_data_seg(a.n, a.paths, a.m, a.w, a.h, a.classes, a.min, a.max, a.angle, a.aspect, a.hue, a.saturation, a.exposure, a.scale);
    } ଅନ୍ୟଥା ଯଦି (a.type == REGION_DATA){
        *a.d = load_data_region(a.n, a.paths, a.m, a.w, a.h, a.ଅଙ୍କ_boxes, a.classes, a.jitter, a.hue, a.saturation, a.exposure);
    } ଅନ୍ୟଥା ଯଦି (a.type == DETECTION_DATA){
        *a.d = load_data_detection(a.n, a.paths, a.m, a.w, a.h, a.ଅଙ୍କ_boxes, a.classes, a.jitter, a.hue, a.saturation, a.exposure);
    } ଅନ୍ୟଥା ଯଦି (a.type == SWAG_DATA){
        *a.d = load_data_swag(a.paths, a.n, a.classes, a.jitter);
    } ଅନ୍ୟଥା ଯଦି (a.type == COMPARE_DATA){
        *a.d = load_data_compare(a.n, a.paths, a.m, a.classes, a.w, a.h);
    } ଅନ୍ୟଥା ଯଦି (a.type == IMAGE_DATA){
        *(a.im) = load_image_color(a.path, 0, 0);
        *(a.resized) = resize_image(*(a.im), a.w, a.h);
    } ଅନ୍ୟଥା ଯଦି (a.type == LETTERBOX_DATA){
        *(a.im) = load_image_color(a.path, 0, 0);
        *(a.resized) = letterbox_image(*(a.im), a.w, a.h);
    } ଅନ୍ୟଥା ଯଦି (a.type == TAG_DATA){
        *a.d = load_data_tag(a.paths, a.n, a.m, a.classes, a.min, a.max, a.size, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    }
    ମୁକ୍ତ(ptr);
    ଫେରନ୍ତୁ 0;
}

pthread_t load_data_in_thread(load_args args)
{
    pthread_t thread;
    ସଂରଚନା load_args *ptr = calloc(1, ଆକାର(ସଂରଚନା load_args));
    *ptr = args;
    ଯଦି(pthread_create(&thread, 0, load_thread, ptr)) error("Thread creation failed");
    ଫେରନ୍ତୁ thread;
}

ଶୂନ୍ୟ *load_threads(ଶୂନ୍ୟ *ptr)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    load_args args = *(load_args *)ptr;
    ଯଦି (args.threads == 0) args.threads = 1;
    data *out = args.d;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା total = args.n;
    ମୁକ୍ତ(ptr);
    data *buffers = calloc(args.threads, ଆକାର(data));
    pthread_t *threads = calloc(args.threads, ଆକାର(pthread_t));
    ପାଇଁ(i = 0; i < args.threads; ++i){
        args.d = buffers + i;
        args.n = (i+1) * total/args.threads - i * total/args.threads;
        threads[i] = load_data_in_thread(args);
    }
    ପାଇଁ(i = 0; i < args.threads; ++i){
        pthread_join(threads[i], 0);
    }
    *out = concat_datas(buffers, args.threads);
    out->shallow = 0;
    ପାଇଁ(i = 0; i < args.threads; ++i){
        buffers[i].shallow = 1;
        free_data(buffers[i]);
    }
    ମୁକ୍ତ(buffers);
    ମୁକ୍ତ(threads);
    ଫେରନ୍ତୁ 0;
}

ଶୂନ୍ୟ load_data_blocking(load_args args)
{
    ସଂରଚନା load_args *ptr = calloc(1, ଆକାର(ସଂରଚନା load_args));
    *ptr = args;
    load_thread(ptr);
}

pthread_t load_data(load_args args)
{
    pthread_t thread;
    ସଂରଚନା load_args *ptr = calloc(1, ଆକାର(ସଂରଚନା load_args));
    *ptr = args;
    ଯଦି(pthread_create(&thread, 0, load_threads, ptr)) error("Thread creation failed");
    ଫେରନ୍ତୁ thread;
}

data load_data_writing(ଅକ୍ଷର **paths, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ପୂର୍ଣ୍ଣସଂଖ୍ୟା out_w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା out_h)
{
    ଯଦି(m) paths = get_random_paths(paths, n, m);
    ଅକ୍ଷର **replace_paths = find_replace_paths(paths, n, ".png", "-label.png");
    data d = {0};
    d.shallow = 0;
    d.X = load_image_paths(paths, n, w, h);
    d.y = load_image_paths_gray(replace_paths, n, out_w, out_h);
    ଯଦି(m) ମୁକ୍ତ(paths);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < n; ++i) ମୁକ୍ତ(replace_paths[i]);
    ମୁକ୍ତ(replace_paths);
    ଫେରନ୍ତୁ d;
}

data load_data_old(ଅକ୍ଷର **paths, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ଅକ୍ଷର **labels, ପୂର୍ଣ୍ଣସଂଖ୍ୟା k, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h)
{
    ଯଦି(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;
    d.X = load_image_paths(paths, n, w, h);
    d.y = load_labels_paths(paths, n, labels, k, 0);
    ଯଦି(m) ମୁକ୍ତ(paths);
    ଫେରନ୍ତୁ d;
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

data load_data_super(ଅକ୍ଷର **paths, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h, ପୂର୍ଣ୍ଣସଂଖ୍ୟା scale)
{
    ଯଦି(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    d.X.rows = n;
    d.X.vals = calloc(n, ଆକାର(ଦଶମିକ*));
    d.X.cols = w*h*3;

    d.y.rows = n;
    d.y.vals = calloc(n, ଆକାର(ଦଶମିକ*));
    d.y.cols = w*scale * h*scale * 3;

    ପାଇଁ(i = 0; i < n; ++i){
        image im = load_image_color(paths[i], 0, 0);
        image crop = random_crop_image(im, w*scale, h*scale);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା flip = rand()%2;
        ଯଦି (flip) flip_image(crop);
        image resize = resize_image(crop, w, h);
        d.X.vals[i] = resize.data;
        d.y.vals[i] = crop.data;
        free_image(im);
    }

    ଯଦି(m) ମୁକ୍ତ(paths);
    ଫେରନ୍ତୁ d;
}

data load_data_regression(ଅକ୍ଷର **paths, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା k, ପୂର୍ଣ୍ଣସଂଖ୍ୟା min, ପୂର୍ଣ୍ଣସଂଖ୍ୟା max, ପୂର୍ଣ୍ଣସଂଖ୍ୟା size, ଦଶମିକ angle, ଦଶମିକ aspect, ଦଶମିକ hue, ଦଶମିକ saturation, ଦଶମିକ exposure)
{
    ଯଦି(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;
    d.X = load_image_augment_paths(paths, n, min, max, size, angle, aspect, hue, saturation, exposure, 0);
    d.y = load_regression_labels_paths(paths, n, k);
    ଯଦି(m) ମୁକ୍ତ(paths);
    ଫେରନ୍ତୁ d;
}

data select_data(data *orig, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *inds)
{
    data d = {0};
    d.shallow = 1;
    d.w = orig[0].w;
    d.h = orig[0].h;

    d.X.rows = orig[0].X.rows;
    d.y.rows = orig[0].X.rows;

    d.X.cols = orig[0].X.cols;
    d.y.cols = orig[0].y.cols;

    d.X.vals = calloc(orig[0].X.rows, ଆକାର(ଦଶମିକ *));
    d.y.vals = calloc(orig[0].y.rows, ଆକାର(ଦଶମିକ *));
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < d.X.rows; ++i){
        d.X.vals[i] = orig[inds[i]].X.vals[i];
        d.y.vals[i] = orig[inds[i]].y.vals[i];
    }
    ଫେରନ୍ତୁ d;
}

data *tile_data(data orig, ପୂର୍ଣ୍ଣସଂଖ୍ୟା divs, ପୂର୍ଣ୍ଣସଂଖ୍ୟା size)
{
    data *ds = calloc(divs*divs, ଆକାର(data));
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j;
#pragma omp parallel ପାଇଁ
    ପାଇଁ(i = 0; i < divs*divs; ++i){
        data d;
        d.shallow = 0;
        d.w = orig.w/divs * size;
        d.h = orig.h/divs * size;
        d.X.rows = orig.X.rows;
        d.X.cols = d.w*d.h*3;
        d.X.vals = calloc(d.X.rows, ଆକାର(ଦଶମିକ*));

        d.y = copy_matrix(orig.y);
#pragma omp parallel ପାଇଁ
        ପାଇଁ(j = 0; j < orig.X.rows; ++j){
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା x = (i%divs) * orig.w / divs - (d.w - orig.w/divs)/2;
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା y = (i/divs) * orig.h / divs - (d.h - orig.h/divs)/2;
            image im = float_to_image(orig.w, orig.h, 3, orig.X.vals[j]);
            d.X.vals[j] = crop_image(im, x, y, d.w, d.h).data;
        }
        ds[i] = d;
    }
    ଫେରନ୍ତୁ ds;
}

data resize_data(data orig, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h)
{
    data d = {0};
    d.shallow = 0;
    d.w = w;
    d.h = h;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    d.X.rows = orig.X.rows;
    d.X.cols = w*h*3;
    d.X.vals = calloc(d.X.rows, ଆକାର(ଦଶମିକ*));

    d.y = copy_matrix(orig.y);
#pragma omp parallel ପାଇଁ
    ପାଇଁ(i = 0; i < orig.X.rows; ++i){
        image im = float_to_image(orig.w, orig.h, 3, orig.X.vals[i]);
        d.X.vals[i] = resize_image(im, w, h).data;
    }
    ଫେରନ୍ତୁ d;
}

data load_data_augment(ଅକ୍ଷର **paths, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ଅକ୍ଷର **labels, ପୂର୍ଣ୍ଣସଂଖ୍ୟା k, ବୃକ୍ଷ *hierarchy, ପୂର୍ଣ୍ଣସଂଖ୍ୟା min, ପୂର୍ଣ୍ଣସଂଖ୍ୟା max, ପୂର୍ଣ୍ଣସଂଖ୍ୟା size, ଦଶମିକ angle, ଦଶମିକ aspect, ଦଶମିକ hue, ଦଶମିକ saturation, ଦଶମିକ exposure, ପୂର୍ଣ୍ଣସଂଖ୍ୟା center)
{
    ଯଦି(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;
    d.w=size;
    d.h=size;
    d.X = load_image_augment_paths(paths, n, min, max, size, angle, aspect, hue, saturation, exposure, center);
    d.y = load_labels_paths(paths, n, labels, k, hierarchy);
    ଯଦି(m) ମୁକ୍ତ(paths);
    ଫେରନ୍ତୁ d;
}

data load_data_tag(ଅକ୍ଷର **paths, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା m, ପୂର୍ଣ୍ଣସଂଖ୍ୟା k, ପୂର୍ଣ୍ଣସଂଖ୍ୟା min, ପୂର୍ଣ୍ଣସଂଖ୍ୟା max, ପୂର୍ଣ୍ଣସଂଖ୍ୟା size, ଦଶମିକ angle, ଦଶମିକ aspect, ଦଶମିକ hue, ଦଶମିକ saturation, ଦଶମିକ exposure)
{
    ଯଦି(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.w = size;
    d.h = size;
    d.shallow = 0;
    d.X = load_image_augment_paths(paths, n, min, max, size, angle, aspect, hue, saturation, exposure, 0);
    d.y = load_tags_paths(paths, n, k);
    ଯଦି(m) ମୁକ୍ତ(paths);
    ଫେରନ୍ତୁ d;
}

matrix concat_matrix(matrix m1, matrix m2)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, ଗଣନା_2 = 0;
    matrix m;
    m.cols = m1.cols;
    m.rows = m1.rows+m2.rows;
    m.vals = calloc(m1.rows + m2.rows, ଆକାର(ଦଶମିକ*));
    ପାଇଁ(i = 0; i < m1.rows; ++i){
        m.vals[ଗଣନା_2++] = m1.vals[i];
    }
    ପାଇଁ(i = 0; i < m2.rows; ++i){
        m.vals[ଗଣନା_2++] = m2.vals[i];
    }
    ଫେରନ୍ତୁ m;
}

data concat_data(data d1, data d2)
{
    data d = {0};
    d.shallow = 1;
    d.X = concat_matrix(d1.X, d2.X);
    d.y = concat_matrix(d1.y, d2.y);
    d.w = d1.w;
    d.h = d1.h;
    ଫେରନ୍ତୁ d;
}

data concat_datas(data *d, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    data out = {0};
    ପାଇଁ(i = 0; i < n; ++i){
        data new = concat_data(d[i], out);
        free_data(out);
        out = new;
    }
    ଫେରନ୍ତୁ out;
}

data load_categorical_data_csv(ଅକ୍ଷର *filename, ପୂର୍ଣ୍ଣସଂଖ୍ୟା target, ପୂର୍ଣ୍ଣସଂଖ୍ୟା k)
{
    data d = {0};
    d.shallow = 0;
    matrix X = csv_to_matrix(filename);
    ଦଶମିକ *truth_1d = କାଢ଼_column(&X, target);
    ଦଶମିକ **truth = one_hot_encode(truth_1d, X.rows, k);
    matrix y;
    y.rows = X.rows;
    y.cols = k;
    y.vals = truth;
    d.X = X;
    d.y = y;
    ମୁକ୍ତ(truth_1d);
    ଫେରନ୍ତୁ d;
}

data load_cifar10_data(ଅକ୍ଷର *filename)
{
    data d = {0};
    d.shallow = 0;
    long i,j;
    matrix X = make_matrix(10000, 3072);
    matrix y = make_matrix(10000, 10);
    d.X = X;
    d.y = y;

    FILE *fp = fopen(filename, "rb");
    ଯଦି(!fp) file_error(filename);
    ପାଇଁ(i = 0; i < 10000; ++i){
        unsigned ଅକ୍ଷର bytes[3073];
        fread(bytes, 1, 3073, fp);
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା class = bytes[0];
        y.vals[i][class] = 1;
        ପାଇଁ(j = 0; j < X.cols; ++j){
            X.vals[i][j] = (ଦ୍ୱିଗୁଣ)bytes[j+1];
        }
    }
    scale_data_rows(d, 1./255);
    //normalize_data_rows(d);
    fclose(fp);
    ଫେରନ୍ତୁ d;
}

ଶୂନ୍ୟ get_random_batch(data d, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ଦଶମିକ *X, ଦଶମିକ *y)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା j;
    ପାଇଁ(j = 0; j < n; ++j){
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = rand()%d.X.rows;
        memcpy(X+j*d.X.cols, d.X.vals[index], d.X.cols*ଆକାର(ଦଶମିକ));
        memcpy(y+j*d.y.cols, d.y.vals[index], d.y.cols*ଆକାର(ଦଶମିକ));
    }
}

ଶୂନ୍ୟ get_next_batch(data d, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା offset, ଦଶମିକ *X, ଦଶମିକ *y)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା j;
    ପାଇଁ(j = 0; j < n; ++j){
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = offset + j;
        memcpy(X+j*d.X.cols, d.X.vals[index], d.X.cols*ଆକାର(ଦଶମିକ));
        ଯଦି(y) memcpy(y+j*d.y.cols, d.y.vals[index], d.y.cols*ଆକାର(ଦଶମିକ));
    }
}

ଶୂନ୍ୟ smooth_data(data d)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, j;
    ଦଶମିକ scale = 1. / d.y.cols;
    ଦଶମିକ eps = .1;
    ପାଇଁ(i = 0; i < d.y.rows; ++i){
        ପାଇଁ(j = 0; j < d.y.cols; ++j){
            d.y.vals[i][j] = eps * scale + (1-eps) * d.y.vals[i][j];
        }
    }
}

data load_all_cifar10()
{
    data d = {0};
    d.shallow = 0;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i,j,b;
    matrix X = make_matrix(50000, 3072);
    matrix y = make_matrix(50000, 10);
    d.X = X;
    d.y = y;


    ପାଇଁ(b = 0; b < 5; ++b){
        ଅକ୍ଷର buff[256];
        sprintf(buff, "data/cifar/cifar-10-batches-bin/data_batch_%d.bin", b+1);
        FILE *fp = fopen(buff, "rb");
        ଯଦି(!fp) file_error(buff);
        ପାଇଁ(i = 0; i < 10000; ++i){
            unsigned ଅକ୍ଷର bytes[3073];
            fread(bytes, 1, 3073, fp);
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା class = bytes[0];
            y.vals[i+b*10000][class] = 1;
            ପାଇଁ(j = 0; j < X.cols; ++j){
                X.vals[i+b*10000][j] = (ଦ୍ୱିଗୁଣ)bytes[j+1];
            }
        }
        fclose(fp);
    }
    //normalize_data_rows(d);
    scale_data_rows(d, 1./255);
    smooth_data(d);
    ଫେରନ୍ତୁ d;
}

data load_go(ଅକ୍ଷର *filename)
{
    FILE *fp = fopen(filename, "rb");
    matrix X = make_matrix(3363059, 361);
    matrix y = make_matrix(3363059, 361);
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା row, col;

    ଯଦି(!fp) file_error(filename);
    ଅକ୍ଷର *label;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଗଣନା_2 = 0;
    ଯେତେବେଳେ((label = fgetl(fp))){
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
        ଯଦି(ଗଣନା_2 == X.rows){
            X = resize_matrix(X, ଗଣନା_2*2);
            y = resize_matrix(y, ଗଣନା_2*2);
        }
        sscanf(label, "%d %d", &row, &col);
        ଅକ୍ଷର *board = fgetl(fp);

        ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = row*19 + col;
        y.vals[ଗଣନା_2][index] = 1;

        ପାଇଁ(i = 0; i < 19*19; ++i){
            ଦଶମିକ val = 0;
            ଯଦି(board[i] == '1') val = 1;
            ଅନ୍ୟଥା ଯଦି(board[i] == '2') val = -1;
            X.vals[ଗଣନା_2][i] = val;
        }
        ++ଗଣନା_2;
        ମୁକ୍ତ(label);
        ମୁକ୍ତ(board);
    }
    X = resize_matrix(X, ଗଣନା_2);
    y = resize_matrix(y, ଗଣନା_2);

    data d = {0};
    d.shallow = 0;
    d.X = X;
    d.y = y;


    fclose(fp);

    ଫେରନ୍ତୁ d;
}


ଶୂନ୍ୟ randomize_data(data d)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = d.X.rows-1; i > 0; --i){
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = rand()%i;
        ଦଶମିକ *swap = d.X.vals[index];
        d.X.vals[index] = d.X.vals[i];
        d.X.vals[i] = swap;

        swap = d.y.vals[index];
        d.y.vals[index] = d.y.vals[i];
        d.y.vals[i] = swap;
    }
}

ଶୂନ୍ୟ scale_data_rows(data d, ଦଶମିକ s)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < d.X.rows; ++i){
        scale_array(d.X.vals[i], d.X.cols, s);
    }
}

ଶୂନ୍ୟ translate_data_rows(data d, ଦଶମିକ s)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < d.X.rows; ++i){
        translate_array(d.X.vals[i], d.X.cols, s);
    }
}

data copy_data(data d)
{
    data c = {0};
    c.w = d.w;
    c.h = d.h;
    c.shallow = 0;
    c.ଅଙ୍କ_boxes = d.ଅଙ୍କ_boxes;
    c.boxes = d.boxes;
    c.X = copy_matrix(d.X);
    c.y = copy_matrix(d.y);
    ଫେରନ୍ତୁ c;
}

ଶୂନ୍ୟ normalize_data_rows(data d)
{
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < d.X.rows; ++i){
        normalize_array(d.X.vals[i], d.X.cols);
    }
}

data get_data_part(data d, ପୂର୍ଣ୍ଣସଂଖ୍ୟା part, ପୂର୍ଣ୍ଣସଂଖ୍ୟା total)
{
    data p = {0};
    p.shallow = 1;
    p.X.rows = d.X.rows * (part + 1) / total - d.X.rows * part / total;
    p.y.rows = d.y.rows * (part + 1) / total - d.y.rows * part / total;
    p.X.cols = d.X.cols;
    p.y.cols = d.y.cols;
    p.X.vals = d.X.vals + d.X.rows * part / total;
    p.y.vals = d.y.vals + d.y.rows * part / total;
    ଫେରନ୍ତୁ p;
}

data get_random_data(data d, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ)
{
    data r = {0};
    r.shallow = 1;

    r.X.rows = ଅଙ୍କ;
    r.y.rows = ଅଙ୍କ;

    r.X.cols = d.X.cols;
    r.y.cols = d.y.cols;

    r.X.vals = calloc(ଅଙ୍କ, ଆକାର(ଦଶମିକ *));
    r.y.vals = calloc(ଅଙ୍କ, ଆକାର(ଦଶମିକ *));

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପାଇଁ(i = 0; i < ଅଙ୍କ; ++i){
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା index = rand()%d.X.rows;
        r.X.vals[i] = d.X.vals[index];
        r.y.vals[i] = d.y.vals[index];
    }
    ଫେରନ୍ତୁ r;
}

data *split_data(data d, ପୂର୍ଣ୍ଣସଂଖ୍ୟା part, ପୂର୍ଣ୍ଣସଂଖ୍ୟା total)
{
    data *split = calloc(2, ଆକାର(data));
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା start = part*d.X.rows/total;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା end = (part+1)*d.X.rows/total;
    data train;
    data test;
    train.shallow = test.shallow = 1;

    test.X.rows = test.y.rows = end-start;
    train.X.rows = train.y.rows = d.X.rows - (end-start);
    train.X.cols = test.X.cols = d.X.cols;
    train.y.cols = test.y.cols = d.y.cols;

    train.X.vals = calloc(train.X.rows, ଆକାର(ଦଶମିକ*));
    test.X.vals = calloc(test.X.rows, ଆକାର(ଦଶମିକ*));
    train.y.vals = calloc(train.y.rows, ଆକାର(ଦଶମିକ*));
    test.y.vals = calloc(test.y.rows, ଆକାର(ଦଶମିକ*));

    ପାଇଁ(i = 0; i < start; ++i){
        train.X.vals[i] = d.X.vals[i];
        train.y.vals[i] = d.y.vals[i];
    }
    ପାଇଁ(i = start; i < end; ++i){
        test.X.vals[i-start] = d.X.vals[i];
        test.y.vals[i-start] = d.y.vals[i];
    }
    ପାଇଁ(i = end; i < d.X.rows; ++i){
        train.X.vals[i-(end-start)] = d.X.vals[i];
        train.y.vals[i-(end-start)] = d.y.vals[i];
    }
    split[0] = train;
    split[1] = test;
    ଫେରନ୍ତୁ split;
}

