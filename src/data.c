#অন্তর্ভাৱ "data.h"
#অন্তর্ভাৱ "utils.h"
#অন্তর্ভাৱ "image.h"
#অন্তর্ভাৱ "cuda.h"

#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <stdlib.h>
#অন্তর্ভাৱ <string.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

list *get_paths(ৱর্ণ *filename)
{
    ৱর্ণ *path;
    FILE *file = fopen(filename, "r");
    যদি(!file) file_error(filename);
    list *সাৰিগুলি = make_list();
    যেতিয়ালৈ((path=fgetl(file))){
        list_ঢোকাও(সাৰিগুলি, path);
    }
    fclose(file);
    ঘূৰাই সাৰিগুলি;
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

ৱর্ণ **get_random_paths(ৱর্ণ **paths, পূর্ণাঙ্ক n, পূর্ণাঙ্ক m)
{
    ৱর্ণ **random_paths = calloc(n, পরিমাণ(ৱর্ণ*));
    পূর্ণাঙ্ক i;
    pthread_mutex_lock(&mutex);
    বাবে(i = 0; i < n; ++i){
        পূর্ণাঙ্ক index = rand()%m;
        random_paths[i] = paths[index];
        //if(i == 0) printf("%s\n", paths[index]);
    }
    pthread_mutex_unlock(&mutex);
    ঘূৰাই random_paths;
}

ৱর্ণ **find_replace_paths(ৱর্ণ **paths, পূর্ণাঙ্ক n, ৱর্ণ *find, ৱর্ণ *replace)
{
    ৱর্ণ **replace_paths = calloc(n, পরিমাণ(ৱর্ণ*));
    পূর্ণাঙ্ক i;
    বাবে(i = 0; i < n; ++i){
        ৱর্ণ replaced[4096];
        find_replace(paths[i], find, replace, replaced);
        replace_paths[i] = copy_string(replaced);
    }
    ঘূৰাই replace_paths;
}

matrix load_image_paths_gray(ৱর্ণ **paths, পূর্ণাঙ্ক n, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h)
{
    পূর্ণাঙ্ক i;
    matrix X;
    X.rows = n;
    X.vals = calloc(X.rows, পরিমাণ(ভিন্ন*));
    X.cols = 0;

    বাবে(i = 0; i < n; ++i){
        image im = load_image(paths[i], w, h, 3);

        image gray = grayscale_image(im);
        free_image(im);
        im = gray;

        X.vals[i] = im.data;
        X.cols = im.h*im.w*im.c;
    }
    ঘূৰাই X;
}

matrix load_image_paths(ৱর্ণ **paths, পূর্ণাঙ্ক n, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h)
{
    পূর্ণাঙ্ক i;
    matrix X;
    X.rows = n;
    X.vals = calloc(X.rows, পরিমাণ(ভিন্ন*));
    X.cols = 0;

    বাবে(i = 0; i < n; ++i){
        image im = load_image_color(paths[i], w, h);
        X.vals[i] = im.data;
        X.cols = im.h*im.w*im.c;
    }
    ঘূৰাই X;
}

matrix load_image_augment_paths(ৱর্ণ **paths, পূর্ণাঙ্ক n, পূর্ণাঙ্ক min, পূর্ণাঙ্ক max, পূর্ণাঙ্ক size, ভিন্ন angle, ভিন্ন aspect, ভিন্ন hue, ভিন্ন saturation, ভিন্ন exposure, পূর্ণাঙ্ক center)
{
    পূর্ণাঙ্ক i;
    matrix X;
    X.rows = n;
    X.vals = calloc(X.rows, পরিমাণ(ভিন্ন*));
    X.cols = 0;

    বাবে(i = 0; i < n; ++i){
        image im = load_image_color(paths[i], 0, 0);
        image crop;
        যদি(center){
            crop = center_crop_image(im, size, size);
        } অন্যথা {
            crop = random_augment_image(im, angle, aspect, min, max, size, size);
        }
        পূর্ণাঙ্ক flip = rand()%2;
        যদি (flip) flip_image(crop);
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
    ঘূৰাই X;
}


box_label *read_boxes(ৱর্ণ *filename, পূর্ণাঙ্ক *n)
{
    FILE *file = fopen(filename, "r");
    যদি(!file) file_error(filename);
    ভিন্ন x, y, h, w;
    পূর্ণাঙ্ক id;
    পূর্ণাঙ্ক গুনতি = 0;
    পূর্ণাঙ্ক size = 64;
    box_label *boxes = calloc(size, পরিমাণ(box_label));
    যেতিয়ালৈ(fscanf(file, "%d %f %f %f %f", &id, &x, &y, &w, &h) == 5){
        যদি(গুনতি == size) {
            size = size * 2;
            boxes = realloc(boxes, size*পরিমাণ(box_label));
        }
        boxes[গুনতি].id = id;
        boxes[গুনতি].x = x;
        boxes[গুনতি].y = y;
        boxes[গুনতি].h = h;
        boxes[গুনতি].w = w;
        boxes[গুনতি].বাম   = x - w/2;
        boxes[গুনতি].ডান  = x + w/2;
        boxes[গুনতি].শীৰ্ষ    = y - h/2;
        boxes[গুনতি].bottom = y + h/2;
        ++গুনতি;
    }
    fclose(file);
    *n = গুনতি;
    ঘূৰাই boxes;
}

শূন্য randomize_boxes(box_label *b, পূর্ণাঙ্ক n)
{
    পূর্ণাঙ্ক i;
    বাবে(i = 0; i < n; ++i){
        box_label swap = b[i];
        পূর্ণাঙ্ক index = rand()%n;
        b[i] = b[index];
        b[index] = swap;
    }
}

শূন্য correct_boxes(box_label *boxes, পূর্ণাঙ্ক n, ভিন্ন dx, ভিন্ন dy, ভিন্ন sx, ভিন্ন sy, পূর্ণাঙ্ক flip)
{
    পূর্ণাঙ্ক i;
    বাবে(i = 0; i < n; ++i){
        যদি(boxes[i].x == 0 && boxes[i].y == 0) {
            boxes[i].x = 999999;
            boxes[i].y = 999999;
            boxes[i].w = 999999;
            boxes[i].h = 999999;
            অনুৱর্ত;
        }
        boxes[i].বাম   = boxes[i].বাম  * sx - dx;
        boxes[i].ডান  = boxes[i].ডান * sx - dx;
        boxes[i].শীৰ্ষ    = boxes[i].শীৰ্ষ   * sy - dy;
        boxes[i].bottom = boxes[i].bottom* sy - dy;

        যদি(flip){
            ভিন্ন swap = boxes[i].বাম;
            boxes[i].বাম = 1. - boxes[i].ডান;
            boxes[i].ডান = 1. - swap;
        }

        boxes[i].বাম =  constrain(0, 1, boxes[i].বাম);
        boxes[i].ডান = constrain(0, 1, boxes[i].ডান);
        boxes[i].শীৰ্ষ =   constrain(0, 1, boxes[i].শীৰ্ষ);
        boxes[i].bottom =   constrain(0, 1, boxes[i].bottom);

        boxes[i].x = (boxes[i].বাম+boxes[i].ডান)/2;
        boxes[i].y = (boxes[i].শীৰ্ষ+boxes[i].bottom)/2;
        boxes[i].w = (boxes[i].ডান - boxes[i].বাম);
        boxes[i].h = (boxes[i].bottom - boxes[i].শীৰ্ষ);

        boxes[i].w = constrain(0, 1, boxes[i].w);
        boxes[i].h = constrain(0, 1, boxes[i].h);
    }
}

শূন্য fill_truth_swag(ৱর্ণ *path, ভিন্ন *truth, পূর্ণাঙ্ক classes, পূর্ণাঙ্ক flip, ভিন্ন dx, ভিন্ন dy, ভিন্ন sx, ভিন্ন sy)
{
    ৱর্ণ labelpath[4096];
    find_replace(path, "images", "labels", labelpath);
    find_replace(labelpath, "JPEGImages", "labels", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);

    পূর্ণাঙ্ক গুনতি = 0;
    box_label *boxes = read_boxes(labelpath, &গুনতি);
    randomize_boxes(boxes, গুনতি);
    correct_boxes(boxes, গুনতি, dx, dy, sx, sy, flip);
    ভিন্ন x,y,w,h;
    পূর্ণাঙ্ক id;
    পূর্ণাঙ্ক i;

    বাবে (i = 0; i < গুনতি && i < 90; ++i) {
        x =  boxes[i].x;
        y =  boxes[i].y;
        w =  boxes[i].w;
        h =  boxes[i].h;
        id = boxes[i].id;

        যদি (w < .0 || h < .0) অনুৱর্ত;

        পূর্ণাঙ্ক index = (4+classes) * i;

        truth[index++] = x;
        truth[index++] = y;
        truth[index++] = w;
        truth[index++] = h;

        যদি (id < classes) truth[index+id] = 1;
    }
    মুক্ত(boxes);
}

শূন্য fill_truth_region(ৱর্ণ *path, ভিন্ন *truth, পূর্ণাঙ্ক classes, পূর্ণাঙ্ক অঙ্ক_boxes, পূর্ণাঙ্ক flip, ভিন্ন dx, ভিন্ন dy, ভিন্ন sx, ভিন্ন sy)
{
    ৱর্ণ labelpath[4096];
    find_replace(path, "images", "labels", labelpath);
    find_replace(labelpath, "JPEGImages", "labels", labelpath);

    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".png", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    পূর্ণাঙ্ক গুনতি = 0;
    box_label *boxes = read_boxes(labelpath, &গুনতি);
    randomize_boxes(boxes, গুনতি);
    correct_boxes(boxes, গুনতি, dx, dy, sx, sy, flip);
    ভিন্ন x,y,w,h;
    পূর্ণাঙ্ক id;
    পূর্ণাঙ্ক i;

    বাবে (i = 0; i < গুনতি; ++i) {
        x =  boxes[i].x;
        y =  boxes[i].y;
        w =  boxes[i].w;
        h =  boxes[i].h;
        id = boxes[i].id;

        যদি (w < .005 || h < .005) অনুৱর্ত;

        পূর্ণাঙ্ক col = (পূর্ণাঙ্ক)(x*অঙ্ক_boxes);
        পূর্ণাঙ্ক row = (পূর্ণাঙ্ক)(y*অঙ্ক_boxes);

        x = x*অঙ্ক_boxes - col;
        y = y*অঙ্ক_boxes - row;

        পূর্ণাঙ্ক index = (col+row*অঙ্ক_boxes)*(5+classes);
        যদি (truth[index]) অনুৱর্ত;
        truth[index++] = 1;

        যদি (id < classes) truth[index+id] = 1;
        index += classes;

        truth[index++] = x;
        truth[index++] = y;
        truth[index++] = w;
        truth[index++] = h;
    }
    মুক্ত(boxes);
}

শূন্য load_rle(image im, পূর্ণাঙ্ক *rle, পূর্ণাঙ্ক n)
{
    পূর্ণাঙ্ক গুনতি = 0;
    পূর্ণাঙ্ক curr = 0;
    পূর্ণাঙ্ক i,j;
    বাবে(i = 0; i < n; ++i){
        বাবে(j = 0; j < rle[i]; ++j){
            im.data[গুনতি++] = curr;
        }
        curr = 1 - curr;
    }
    বাবে(; গুনতি < im.h*im.w*im.c; ++গুনতি){
        im.data[গুনতি] = curr;
    }
}

শূন্য or_image(image src, image dest, পূর্ণাঙ্ক c)
{
    পূর্ণাঙ্ক i;
    বাবে(i = 0; i < src.w*src.h; ++i){
        যদি(src.data[i]) dest.data[dest.w*dest.h*c + i] = 1;
    }
}

শূন্য exclusive_image(image src)
{
    পূর্ণাঙ্ক k, j, i;
    পূর্ণাঙ্ক s = src.w*src.h;
    বাবে(k = 0; k < src.c-1; ++k){
        বাবে(i = 0; i < s; ++i){
            যদি (src.data[k*s + i]){
                বাবে(j = k+1; j < src.c; ++j){
                    src.data[j*s + i] = 0;
                }
            }
        }
    }
}

box bound_image(image im)
{
    পূর্ণাঙ্ক x,y;
    পূর্ণাঙ্ক minx = im.w;
    পূর্ণাঙ্ক miny = im.h;
    পূর্ণাঙ্ক maxx = 0;
    পূর্ণাঙ্ক maxy = 0;
    বাবে(y = 0; y < im.h; ++y){
        বাবে(x = 0; x < im.w; ++x){
            যদি(im.data[y*im.w + x]){
                minx = (x < minx) ? x : minx;
                miny = (y < miny) ? y : miny;
                maxx = (x > maxx) ? x : maxx;
                maxy = (y > maxy) ? y : maxy;
            }
        }
    }
    box b = {minx, miny, maxx-minx + 1, maxy-miny + 1};
    //printf("%f %f %f %f\n", b.x, b.y, b.w, b.h);
    ঘূৰাই b;
}

শূন্য fill_truth_iseg(ৱর্ণ *path, পূর্ণাঙ্ক অঙ্ক_boxes, ভিন্ন *truth, পূর্ণাঙ্ক classes, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h, augment_args aug, পূর্ণাঙ্ক flip, পূর্ণাঙ্ক mw, পূর্ণাঙ্ক mh)
{
    ৱর্ণ labelpath[4096];
    find_replace(path, "images", "mask", labelpath);
    find_replace(labelpath, "JPEGImages", "mask", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    FILE *file = fopen(labelpath, "r");
    যদি(!file) file_error(labelpath);
    ৱর্ণ buff[32788];
    পূর্ণাঙ্ক id;
    পূর্ণাঙ্ক i = 0;
    পূর্ণাঙ্ক j;
    image part = make_image(w, h, 1);
    যেতিয়ালৈ((fscanf(file, "%d %s", &id, buff) == 2) && i < অঙ্ক_boxes){
        পূর্ণাঙ্ক n = 0;
        পূর্ণাঙ্ক *rle = read_intlist(buff, &n, 0);
        load_rle(part, rle, n);
        image sized = rotate_crop_image(part, aug.rad, aug.scale, aug.w, aug.h, aug.dx, aug.dy, aug.aspect);
        যদি(flip) flip_image(sized);

        image mask = resize_image(sized, mw, mh);
        truth[i*(mw*mh+1)] = id;
        বাবে(j = 0; j < mw*mh; ++j){
            truth[i*(mw*mh + 1) + 1 + j] = mask.data[j];
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

শূন্য fill_truth_mask(ৱর্ণ *path, পূর্ণাঙ্ক অঙ্ক_boxes, ভিন্ন *truth, পূর্ণাঙ্ক classes, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h, augment_args aug, পূর্ণাঙ্ক flip, পূর্ণাঙ্ক mw, পূর্ণাঙ্ক mh)
{
    ৱর্ণ labelpath[4096];
    find_replace(path, "images", "mask", labelpath);
    find_replace(labelpath, "JPEGImages", "mask", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    FILE *file = fopen(labelpath, "r");
    যদি(!file) file_error(labelpath);
    ৱর্ণ buff[32788];
    পূর্ণাঙ্ক id;
    পূর্ণাঙ্ক i = 0;
    image part = make_image(w, h, 1);
    যেতিয়ালৈ((fscanf(file, "%d %s", &id, buff) == 2) && i < অঙ্ক_boxes){
        পূর্ণাঙ্ক n = 0;
        পূর্ণাঙ্ক *rle = read_intlist(buff, &n, 0);
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
            পূর্ণাঙ্ক j;
            বাবে(j = 0; j < mw*mh; ++j){
                truth[i*(4 + mw*mh + 1) + 4 + j] = mask.data[j];
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


শূন্য fill_truth_detection(ৱর্ণ *path, পূর্ণাঙ্ক অঙ্ক_boxes, ভিন্ন *truth, পূর্ণাঙ্ক classes, পূর্ণাঙ্ক flip, ভিন্ন dx, ভিন্ন dy, ভিন্ন sx, ভিন্ন sy)
{
    ৱর্ণ labelpath[4096];
    find_replace(path, "images", "labels", labelpath);
    find_replace(labelpath, "JPEGImages", "labels", labelpath);

    find_replace(labelpath, "raw", "labels", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".png", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    পূর্ণাঙ্ক গুনতি = 0;
    box_label *boxes = read_boxes(labelpath, &গুনতি);
    randomize_boxes(boxes, গুনতি);
    correct_boxes(boxes, গুনতি, dx, dy, sx, sy, flip);
    যদি(গুনতি > অঙ্ক_boxes) গুনতি = অঙ্ক_boxes;
    ভিন্ন x,y,w,h;
    পূর্ণাঙ্ক id;
    পূর্ণাঙ্ক i;
    পূর্ণাঙ্ক sub = 0;

    বাবে (i = 0; i < গুনতি; ++i) {
        x =  boxes[i].x;
        y =  boxes[i].y;
        w =  boxes[i].w;
        h =  boxes[i].h;
        id = boxes[i].id;

        যদি ((w < .001 || h < .001)) {
            ++sub;
            অনুৱর্ত;
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

শূন্য print_letters(ভিন্ন *pred, পূর্ণাঙ্ক n)
{
    পূর্ণাঙ্ক i;
    বাবে(i = 0; i < n; ++i){
        পূর্ণাঙ্ক index = max_index(pred+i*NUMCHARS, NUMCHARS);
        printf("%c", int_to_alphanum(index));
    }
    printf("\n");
}

শূন্য fill_truth_captcha(ৱর্ণ *path, পূর্ণাঙ্ক n, ভিন্ন *truth)
{
    ৱর্ণ *begin = strrchr(path, '/');
    ++begin;
    পূর্ণাঙ্ক i;
    বাবে(i = 0; i < strlen(begin) && i < n && begin[i] != '.'; ++i){
        পূর্ণাঙ্ক index = alphanum_to_int(begin[i]);
        যদি(index > 35) printf("Bad %c\n", begin[i]);
        truth[i*NUMCHARS+index] = 1;
    }
    বাবে(;i < n; ++i){
        truth[i*NUMCHARS + NUMCHARS-1] = 1;
    }
}

data load_data_captcha(ৱর্ণ **paths, পূর্ণাঙ্ক n, পূর্ণাঙ্ক m, পূর্ণাঙ্ক k, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h)
{
    যদি(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;
    d.X = load_image_paths(paths, n, w, h);
    d.y = make_matrix(n, k*NUMCHARS);
    পূর্ণাঙ্ক i;
    বাবে(i = 0; i < n; ++i){
        fill_truth_captcha(paths[i], k, d.y.vals[i]);
    }
    যদি(m) মুক্ত(paths);
    ঘূৰাই d;
}

data load_data_captcha_encode(ৱর্ণ **paths, পূর্ণাঙ্ক n, পূর্ণাঙ্ক m, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h)
{
    যদি(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;
    d.X = load_image_paths(paths, n, w, h);
    d.X.cols = 17100;
    d.y = d.X;
    যদি(m) মুক্ত(paths);
    ঘূৰাই d;
}

শূন্য fill_truth(ৱর্ণ *path, ৱর্ণ **labels, পূর্ণাঙ্ক k, ভিন্ন *truth)
{
    পূর্ণাঙ্ক i;
    memset(truth, 0, k*পরিমাণ(ভিন্ন));
    পূর্ণাঙ্ক গুনতি = 0;
    বাবে(i = 0; i < k; ++i){
        যদি(strstr(path, labels[i])){
            truth[i] = 1;
            ++গুনতি;
            //printf("%s %s %d\n", path, labels[i], i);
        }
    }
    যদি(গুনতি != 1 && (k != 1 || গুনতি != 0)) printf("Too many or too few labels: %d, %s\n", গুনতি, path);
}

শূন্য fill_hierarchy(ভিন্ন *truth, পূর্ণাঙ্ক k, বৃক্ষ *hierarchy)
{
    পূর্ণাঙ্ক j;
    বাবে(j = 0; j < k; ++j){
        যদি(truth[j]){
            পূর্ণাঙ্ক parent = hierarchy->parent[j];
            যেতিয়ালৈ(parent >= 0){
                truth[parent] = 1;
                parent = hierarchy->parent[parent];
            }
        }
    }
    পূর্ণাঙ্ক i;
    পূর্ণাঙ্ক গুনতি = 0;
    বাবে(j = 0; j < hierarchy->groups; ++j){
        //printf("%d\n", count);
        পূর্ণাঙ্ক mask = 1;
        বাবে(i = 0; i < hierarchy->group_size[j]; ++i){
            যদি(truth[গুনতি + i]){
                mask = 0;
                ভঙ্গ;
            }
        }
        যদি (mask) {
            বাবে(i = 0; i < hierarchy->group_size[j]; ++i){
                truth[গুনতি + i] = SECRET_অঙ্ক;
            }
        }
        গুনতি += hierarchy->group_size[j];
    }
}

matrix load_regression_labels_paths(ৱর্ণ **paths, পূর্ণাঙ্ক n, পূর্ণাঙ্ক k)
{
    matrix y = make_matrix(n, k);
    পূর্ণাঙ্ক i,j;
    বাবে(i = 0; i < n; ++i){
        ৱর্ণ labelpath[4096];
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
        বাবে(j = 0; j < k; ++j){
            fscanf(file, "%f", &(y.vals[i][j]));
        }
        fclose(file);
    }
    ঘূৰাই y;
}

matrix load_labels_paths(ৱর্ণ **paths, পূর্ণাঙ্ক n, ৱর্ণ **labels, পূর্ণাঙ্ক k, বৃক্ষ *hierarchy)
{
    matrix y = make_matrix(n, k);
    পূর্ণাঙ্ক i;
    বাবে(i = 0; i < n && labels; ++i){
        fill_truth(paths[i], labels, k, y.vals[i]);
        যদি(hierarchy){
            fill_hierarchy(y.vals[i], k, hierarchy);
        }
    }
    ঘূৰাই y;
}

matrix load_tags_paths(ৱর্ণ **paths, পূর্ণাঙ্ক n, পূর্ণাঙ্ক k)
{
    matrix y = make_matrix(n, k);
    পূর্ণাঙ্ক i;
    //int count = 0;
    বাবে(i = 0; i < n; ++i){
        ৱর্ণ label[4096];
        find_replace(paths[i], "images", "labels", label);
        find_replace(label, ".jpg", ".txt", label);
        FILE *file = fopen(label, "r");
        যদি (!file) অনুৱর্ত;
        //++count;
        পূর্ণাঙ্ক tag;
        যেতিয়ালৈ(fscanf(file, "%d", &tag) == 1){
            যদি(tag < k){
                y.vals[i][tag] = 1;
            }
        }
        fclose(file);
    }
    //printf("%d/%d\n", count, n);
    ঘূৰাই y;
}

ৱর্ণ **get_labels(ৱর্ণ *filename)
{
    list *plist = get_paths(filename);
    ৱর্ণ **labels = (ৱর্ণ **)list_to_array(plist);
    free_list(plist);
    ঘূৰাই labels;
}

শূন্য free_data(data d)
{
    যদি(!d.shallow){
        free_matrix(d.X);
        free_matrix(d.y);
    }অন্যথা{
        মুক্ত(d.X.vals);
        মুক্ত(d.y.vals);
    }
}

image get_segmentation_image(ৱর্ণ *path, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h, পূর্ণাঙ্ক classes)
{
    ৱর্ণ labelpath[4096];
    find_replace(path, "images", "mask", labelpath);
    find_replace(labelpath, "JPEGImages", "mask", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    image mask = make_image(w, h, classes);
    FILE *file = fopen(labelpath, "r");
    যদি(!file) file_error(labelpath);
    ৱর্ণ buff[32788];
    পূর্ণাঙ্ক id;
    image part = make_image(w, h, 1);
    যেতিয়ালৈ(fscanf(file, "%d %s", &id, buff) == 2){
        পূর্ণাঙ্ক n = 0;
        পূর্ণাঙ্ক *rle = read_intlist(buff, &n, 0);
        load_rle(part, rle, n);
        or_image(part, mask, id);
        মুক্ত(rle);
    }
    //exclusive_image(mask);
    fclose(file);
    free_image(part);
    ঘূৰাই mask;
}

image get_segmentation_image2(ৱর্ণ *path, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h, পূর্ণাঙ্ক classes)
{
    ৱর্ণ labelpath[4096];
    find_replace(path, "images", "mask", labelpath);
    find_replace(labelpath, "JPEGImages", "mask", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    image mask = make_image(w, h, classes+1);
    পূর্ণাঙ্ক i;
    বাবে(i = 0; i < w*h; ++i){
        mask.data[w*h*classes + i] = 1;
    }
    FILE *file = fopen(labelpath, "r");
    যদি(!file) file_error(labelpath);
    ৱর্ণ buff[32788];
    পূর্ণাঙ্ক id;
    image part = make_image(w, h, 1);
    যেতিয়ালৈ(fscanf(file, "%d %s", &id, buff) == 2){
        পূর্ণাঙ্ক n = 0;
        পূর্ণাঙ্ক *rle = read_intlist(buff, &n, 0);
        load_rle(part, rle, n);
        or_image(part, mask, id);
        বাবে(i = 0; i < w*h; ++i){
            যদি(part.data[i]) mask.data[w*h*classes + i] = 0;
        }
        মুক্ত(rle);
    }
    //exclusive_image(mask);
    fclose(file);
    free_image(part);
    ঘূৰাই mask;
}

data load_data_seg(পূর্ণাঙ্ক n, ৱর্ণ **paths, পূর্ণাঙ্ক m, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h, পূর্ণাঙ্ক classes, পূর্ণাঙ্ক min, পূর্ণাঙ্ক max, ভিন্ন angle, ভিন্ন aspect, ভিন্ন hue, ভিন্ন saturation, ভিন্ন exposure, পূর্ণাঙ্ক div)
{
    ৱর্ণ **random_paths = get_random_paths(paths, n, m);
    পূর্ণাঙ্ক i;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, পরিমাণ(ভিন্ন*));
    d.X.cols = h*w*3;


    d.y.rows = n;
    d.y.cols = h*w*classes/div/div;
    d.y.vals = calloc(d.X.rows, পরিমাণ(ভিন্ন*));

    বাবে(i = 0; i < n; ++i){
        image orig = load_image_color(random_paths[i], 0, 0);
        augment_args a = random_augment_args(orig, angle, aspect, min, max, w, h);
        image sized = rotate_crop_image(orig, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);

        পূর্ণাঙ্ক flip = rand()%2;
        যদি(flip) flip_image(sized);
        random_distort_image(sized, hue, saturation, exposure);
        d.X.vals[i] = sized.data;

        image mask = get_segmentation_image(random_paths[i], orig.w, orig.h, classes);
        //image mask = make_image(orig.w, orig.h, classes+1);
        image sized_m = rotate_crop_image(mask, a.rad, a.scale/div, a.w/div, a.h/div, a.dx/div, a.dy/div, a.aspect);

        যদি(flip) flip_image(sized_m);
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
    মুক্ত(random_paths);
    ঘূৰাই d;
}

data load_data_iseg(পূর্ণাঙ্ক n, ৱর্ণ **paths, পূর্ণাঙ্ক m, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h, পূর্ণাঙ্ক classes, পূর্ণাঙ্ক boxes, পূর্ণাঙ্ক div, পূর্ণাঙ্ক min, পূর্ণাঙ্ক max, ভিন্ন angle, ভিন্ন aspect, ভিন্ন hue, ভিন্ন saturation, ভিন্ন exposure)
{
    ৱর্ণ **random_paths = get_random_paths(paths, n, m);
    পূর্ণাঙ্ক i;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, পরিমাণ(ভিন্ন*));
    d.X.cols = h*w*3;

    d.y = make_matrix(n, (((w/div)*(h/div))+1)*boxes);

    বাবে(i = 0; i < n; ++i){
        image orig = load_image_color(random_paths[i], 0, 0);
        augment_args a = random_augment_args(orig, angle, aspect, min, max, w, h);
        image sized = rotate_crop_image(orig, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);

        পূর্ণাঙ্ক flip = rand()%2;
        যদি(flip) flip_image(sized);
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
    মুক্ত(random_paths);
    ঘূৰাই d;
}

data load_data_mask(পূর্ণাঙ্ক n, ৱর্ণ **paths, পূর্ণাঙ্ক m, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h, পূর্ণাঙ্ক classes, পূর্ণাঙ্ক boxes, পূর্ণাঙ্ক coords, পূর্ণাঙ্ক min, পূর্ণাঙ্ক max, ভিন্ন angle, ভিন্ন aspect, ভিন্ন hue, ভিন্ন saturation, ভিন্ন exposure)
{
    ৱর্ণ **random_paths = get_random_paths(paths, n, m);
    পূর্ণাঙ্ক i;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, পরিমাণ(ভিন্ন*));
    d.X.cols = h*w*3;

    d.y = make_matrix(n, (coords+1)*boxes);

    বাবে(i = 0; i < n; ++i){
        image orig = load_image_color(random_paths[i], 0, 0);
        augment_args a = random_augment_args(orig, angle, aspect, min, max, w, h);
        image sized = rotate_crop_image(orig, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);

        পূর্ণাঙ্ক flip = rand()%2;
        যদি(flip) flip_image(sized);
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
    মুক্ত(random_paths);
    ঘূৰাই d;
}

data load_data_region(পূর্ণাঙ্ক n, ৱর্ণ **paths, পূর্ণাঙ্ক m, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h, পূর্ণাঙ্ক size, পূর্ণাঙ্ক classes, ভিন্ন jitter, ভিন্ন hue, ভিন্ন saturation, ভিন্ন exposure)
{
    ৱর্ণ **random_paths = get_random_paths(paths, n, m);
    পূর্ণাঙ্ক i;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, পরিমাণ(ভিন্ন*));
    d.X.cols = h*w*3;


    পূর্ণাঙ্ক k = size*size*(5+classes);
    d.y = make_matrix(n, k);
    বাবে(i = 0; i < n; ++i){
        image orig = load_image_color(random_paths[i], 0, 0);

        পূর্ণাঙ্ক oh = orig.h;
        পূর্ণাঙ্ক ow = orig.w;

        পূর্ণাঙ্ক dw = (ow*jitter);
        পূর্ণাঙ্ক dh = (oh*jitter);

        পূর্ণাঙ্ক pleft  = rand_uniform(-dw, dw);
        পূর্ণাঙ্ক pright = rand_uniform(-dw, dw);
        পূর্ণাঙ্ক ptop   = rand_uniform(-dh, dh);
        পূর্ণাঙ্ক pbot   = rand_uniform(-dh, dh);

        পূর্ণাঙ্ক swidth =  ow - pleft - pright;
        পূর্ণাঙ্ক sheight = oh - ptop - pbot;

        ভিন্ন sx = (ভিন্ন)swidth  / ow;
        ভিন্ন sy = (ভিন্ন)sheight / oh;

        পূর্ণাঙ্ক flip = rand()%2;
        image cropped = crop_image(orig, pleft, ptop, swidth, sheight);

        ভিন্ন dx = ((ভিন্ন)pleft/ow)/sx;
        ভিন্ন dy = ((ভিন্ন)ptop /oh)/sy;

        image sized = resize_image(cropped, w, h);
        যদি(flip) flip_image(sized);
        random_distort_image(sized, hue, saturation, exposure);
        d.X.vals[i] = sized.data;

        fill_truth_region(random_paths[i], d.y.vals[i], classes, size, flip, dx, dy, 1./sx, 1./sy);

        free_image(orig);
        free_image(cropped);
    }
    মুক্ত(random_paths);
    ঘূৰাই d;
}

data load_data_compare(পূর্ণাঙ্ক n, ৱর্ণ **paths, পূর্ণাঙ্ক m, পূর্ণাঙ্ক classes, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h)
{
    যদি(m) paths = get_random_paths(paths, 2*n, m);
    পূর্ণাঙ্ক i,j;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, পরিমাণ(ভিন্ন*));
    d.X.cols = h*w*6;

    পূর্ণাঙ্ক k = 2*(classes);
    d.y = make_matrix(n, k);
    বাবে(i = 0; i < n; ++i){
        image im1 = load_image_color(paths[i*2],   w, h);
        image im2 = load_image_color(paths[i*2+1], w, h);

        d.X.vals[i] = calloc(d.X.cols, পরিমাণ(ভিন্ন));
        memcpy(d.X.vals[i],         im1.data, h*w*3*পরিমাণ(ভিন্ন));
        memcpy(d.X.vals[i] + h*w*3, im2.data, h*w*3*পরিমাণ(ভিন্ন));

        পূর্ণাঙ্ক id;
        ভিন্ন iou;

        ৱর্ণ imlabel1[4096];
        ৱর্ণ imlabel2[4096];
        find_replace(paths[i*2],   "imgs", "labels", imlabel1);
        find_replace(imlabel1, "jpg", "txt", imlabel1);
        FILE *fp1 = fopen(imlabel1, "r");

        যেতিয়ালৈ(fscanf(fp1, "%d %f", &id, &iou) == 2){
            যদি (d.y.vals[i][2*id] < iou) d.y.vals[i][2*id] = iou;
        }

        find_replace(paths[i*2+1], "imgs", "labels", imlabel2);
        find_replace(imlabel2, "jpg", "txt", imlabel2);
        FILE *fp2 = fopen(imlabel2, "r");

        যেতিয়ালৈ(fscanf(fp2, "%d %f", &id, &iou) == 2){
            যদি (d.y.vals[i][2*id + 1] < iou) d.y.vals[i][2*id + 1] = iou;
        }

        বাবে (j = 0; j < classes; ++j){
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
    ঘূৰাই d;
}

data load_data_swag(ৱর্ণ **paths, পূর্ণাঙ্ক n, পূর্ণাঙ্ক classes, ভিন্ন jitter)
{
    পূর্ণাঙ্ক index = rand()%n;
    ৱর্ণ *random_path = paths[index];

    image orig = load_image_color(random_path, 0, 0);
    পূর্ণাঙ্ক h = orig.h;
    পূর্ণাঙ্ক w = orig.w;

    data d = {0};
    d.shallow = 0;
    d.w = w;
    d.h = h;

    d.X.rows = 1;
    d.X.vals = calloc(d.X.rows, পরিমাণ(ভিন্ন*));
    d.X.cols = h*w*3;

    পূর্ণাঙ্ক k = (4+classes)*90;
    d.y = make_matrix(1, k);

    পূর্ণাঙ্ক dw = w*jitter;
    পূর্ণাঙ্ক dh = h*jitter;

    পূর্ণাঙ্ক pleft  = rand_uniform(-dw, dw);
    পূর্ণাঙ্ক pright = rand_uniform(-dw, dw);
    পূর্ণাঙ্ক ptop   = rand_uniform(-dh, dh);
    পূর্ণাঙ্ক pbot   = rand_uniform(-dh, dh);

    পূর্ণাঙ্ক swidth =  w - pleft - pright;
    পূর্ণাঙ্ক sheight = h - ptop - pbot;

    ভিন্ন sx = (ভিন্ন)swidth  / w;
    ভিন্ন sy = (ভিন্ন)sheight / h;

    পূর্ণাঙ্ক flip = rand()%2;
    image cropped = crop_image(orig, pleft, ptop, swidth, sheight);

    ভিন্ন dx = ((ভিন্ন)pleft/w)/sx;
    ভিন্ন dy = ((ভিন্ন)ptop /h)/sy;

    image sized = resize_image(cropped, w, h);
    যদি(flip) flip_image(sized);
    d.X.vals[0] = sized.data;

    fill_truth_swag(random_path, d.y.vals[0], classes, flip, dx, dy, 1./sx, 1./sy);

    free_image(orig);
    free_image(cropped);

    ঘূৰাই d;
}

data load_data_detection(পূর্ণাঙ্ক n, ৱর্ণ **paths, পূর্ণাঙ্ক m, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h, পূর্ণাঙ্ক boxes, পূর্ণাঙ্ক classes, ভিন্ন jitter, ভিন্ন hue, ভিন্ন saturation, ভিন্ন exposure)
{
    ৱর্ণ **random_paths = get_random_paths(paths, n, m);
    পূর্ণাঙ্ক i;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, পরিমাণ(ভিন্ন*));
    d.X.cols = h*w*3;

    d.y = make_matrix(n, 5*boxes);
    বাবে(i = 0; i < n; ++i){
        image orig = load_image_color(random_paths[i], 0, 0);
        image sized = make_image(w, h, orig.c);
        fill_image(sized, .5);

        ভিন্ন dw = jitter * orig.w;
        ভিন্ন dh = jitter * orig.h;

        ভিন্ন new_ar = (orig.w + rand_uniform(-dw, dw)) / (orig.h + rand_uniform(-dh, dh));
        //float scale = rand_uniform(.25, 2);
        ভিন্ন scale = 1;

        ভিন্ন nw, nh;

        যদি(new_ar < 1){
            nh = scale * h;
            nw = nh * new_ar;
        } অন্যথা {
            nw = scale * w;
            nh = nw / new_ar;
        }

        ভিন্ন dx = rand_uniform(0, w - nw);
        ভিন্ন dy = rand_uniform(0, h - nh);

        place_image(orig, nw, nh, dx, dy, sized);

        random_distort_image(sized, hue, saturation, exposure);

        পূর্ণাঙ্ক flip = rand()%2;
        যদি(flip) flip_image(sized);
        d.X.vals[i] = sized.data;


        fill_truth_detection(random_paths[i], boxes, d.y.vals[i], classes, flip, -dx/w, -dy/h, nw/w, nh/h);

        free_image(orig);
    }
    মুক্ত(random_paths);
    ঘূৰাই d;
}

শূন্য *load_thread(শূন্য *ptr)
{
    //printf("Loading data: %d\n", rand());
    load_args a = *(সংরচনা load_args*)ptr;
    যদি(a.exposure == 0) a.exposure = 1;
    যদি(a.saturation == 0) a.saturation = 1;
    যদি(a.aspect == 0) a.aspect = 1;

    যদি (a.type == OLD_CLASSIFICATION_DATA){
        *a.d = load_data_old(a.paths, a.n, a.m, a.labels, a.classes, a.w, a.h);
    } অন্যথা যদি (a.type == REGRESSION_DATA){
        *a.d = load_data_regression(a.paths, a.n, a.m, a.classes, a.min, a.max, a.size, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    } অন্যথা যদি (a.type == CLASSIFICATION_DATA){
        *a.d = load_data_augment(a.paths, a.n, a.m, a.labels, a.classes, a.hierarchy, a.min, a.max, a.size, a.angle, a.aspect, a.hue, a.saturation, a.exposure, a.center);
    } অন্যথা যদি (a.type == SUPER_DATA){
        *a.d = load_data_super(a.paths, a.n, a.m, a.w, a.h, a.scale);
    } অন্যথা যদি (a.type == WRITING_DATA){
        *a.d = load_data_writing(a.paths, a.n, a.m, a.w, a.h, a.out_w, a.out_h);
    } অন্যথা যদি (a.type == ISEG_DATA){
        *a.d = load_data_iseg(a.n, a.paths, a.m, a.w, a.h, a.classes, a.অঙ্ক_boxes, a.scale, a.min, a.max, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    } অন্যথা যদি (a.type == INSTANCE_DATA){
        *a.d = load_data_mask(a.n, a.paths, a.m, a.w, a.h, a.classes, a.অঙ্ক_boxes, a.coords, a.min, a.max, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    } অন্যথা যদি (a.type == SEGMENTATION_DATA){
        *a.d = load_data_seg(a.n, a.paths, a.m, a.w, a.h, a.classes, a.min, a.max, a.angle, a.aspect, a.hue, a.saturation, a.exposure, a.scale);
    } অন্যথা যদি (a.type == REGION_DATA){
        *a.d = load_data_region(a.n, a.paths, a.m, a.w, a.h, a.অঙ্ক_boxes, a.classes, a.jitter, a.hue, a.saturation, a.exposure);
    } অন্যথা যদি (a.type == DETECTION_DATA){
        *a.d = load_data_detection(a.n, a.paths, a.m, a.w, a.h, a.অঙ্ক_boxes, a.classes, a.jitter, a.hue, a.saturation, a.exposure);
    } অন্যথা যদি (a.type == SWAG_DATA){
        *a.d = load_data_swag(a.paths, a.n, a.classes, a.jitter);
    } অন্যথা যদি (a.type == COMPARE_DATA){
        *a.d = load_data_compare(a.n, a.paths, a.m, a.classes, a.w, a.h);
    } অন্যথা যদি (a.type == IMAGE_DATA){
        *(a.im) = load_image_color(a.path, 0, 0);
        *(a.resized) = resize_image(*(a.im), a.w, a.h);
    } অন্যথা যদি (a.type == LETTERBOX_DATA){
        *(a.im) = load_image_color(a.path, 0, 0);
        *(a.resized) = letterbox_image(*(a.im), a.w, a.h);
    } অন্যথা যদি (a.type == TAG_DATA){
        *a.d = load_data_tag(a.paths, a.n, a.m, a.classes, a.min, a.max, a.size, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    }
    মুক্ত(ptr);
    ঘূৰাই 0;
}

pthread_t load_data_in_thread(load_args args)
{
    pthread_t thread;
    সংরচনা load_args *ptr = calloc(1, পরিমাণ(সংরচনা load_args));
    *ptr = args;
    যদি(pthread_create(&thread, 0, load_thread, ptr)) error("Thread creation failed");
    ঘূৰাই thread;
}

শূন্য *load_threads(শূন্য *ptr)
{
    পূর্ণাঙ্ক i;
    load_args args = *(load_args *)ptr;
    যদি (args.threads == 0) args.threads = 1;
    data *out = args.d;
    পূর্ণাঙ্ক total = args.n;
    মুক্ত(ptr);
    data *buffers = calloc(args.threads, পরিমাণ(data));
    pthread_t *threads = calloc(args.threads, পরিমাণ(pthread_t));
    বাবে(i = 0; i < args.threads; ++i){
        args.d = buffers + i;
        args.n = (i+1) * total/args.threads - i * total/args.threads;
        threads[i] = load_data_in_thread(args);
    }
    বাবে(i = 0; i < args.threads; ++i){
        pthread_join(threads[i], 0);
    }
    *out = concat_datas(buffers, args.threads);
    out->shallow = 0;
    বাবে(i = 0; i < args.threads; ++i){
        buffers[i].shallow = 1;
        free_data(buffers[i]);
    }
    মুক্ত(buffers);
    মুক্ত(threads);
    ঘূৰাই 0;
}

শূন্য load_data_blocking(load_args args)
{
    সংরচনা load_args *ptr = calloc(1, পরিমাণ(সংরচনা load_args));
    *ptr = args;
    load_thread(ptr);
}

pthread_t load_data(load_args args)
{
    pthread_t thread;
    সংরচনা load_args *ptr = calloc(1, পরিমাণ(সংরচনা load_args));
    *ptr = args;
    যদি(pthread_create(&thread, 0, load_threads, ptr)) error("Thread creation failed");
    ঘূৰাই thread;
}

data load_data_writing(ৱর্ণ **paths, পূর্ণাঙ্ক n, পূর্ণাঙ্ক m, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h, পূর্ণাঙ্ক out_w, পূর্ণাঙ্ক out_h)
{
    যদি(m) paths = get_random_paths(paths, n, m);
    ৱর্ণ **replace_paths = find_replace_paths(paths, n, ".png", "-label.png");
    data d = {0};
    d.shallow = 0;
    d.X = load_image_paths(paths, n, w, h);
    d.y = load_image_paths_gray(replace_paths, n, out_w, out_h);
    যদি(m) মুক্ত(paths);
    পূর্ণাঙ্ক i;
    বাবে(i = 0; i < n; ++i) মুক্ত(replace_paths[i]);
    মুক্ত(replace_paths);
    ঘূৰাই d;
}

data load_data_old(ৱর্ণ **paths, পূর্ণাঙ্ক n, পূর্ণাঙ্ক m, ৱর্ণ **labels, পূর্ণাঙ্ক k, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h)
{
    যদি(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;
    d.X = load_image_paths(paths, n, w, h);
    d.y = load_labels_paths(paths, n, labels, k, 0);
    যদি(m) মুক্ত(paths);
    ঘূৰাই d;
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

data load_data_super(ৱর্ণ **paths, পূর্ণাঙ্ক n, পূর্ণাঙ্ক m, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h, পূর্ণাঙ্ক scale)
{
    যদি(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;

    পূর্ণাঙ্ক i;
    d.X.rows = n;
    d.X.vals = calloc(n, পরিমাণ(ভিন্ন*));
    d.X.cols = w*h*3;

    d.y.rows = n;
    d.y.vals = calloc(n, পরিমাণ(ভিন্ন*));
    d.y.cols = w*scale * h*scale * 3;

    বাবে(i = 0; i < n; ++i){
        image im = load_image_color(paths[i], 0, 0);
        image crop = random_crop_image(im, w*scale, h*scale);
        পূর্ণাঙ্ক flip = rand()%2;
        যদি (flip) flip_image(crop);
        image resize = resize_image(crop, w, h);
        d.X.vals[i] = resize.data;
        d.y.vals[i] = crop.data;
        free_image(im);
    }

    যদি(m) মুক্ত(paths);
    ঘূৰাই d;
}

data load_data_regression(ৱর্ণ **paths, পূর্ণাঙ্ক n, পূর্ণাঙ্ক m, পূর্ণাঙ্ক k, পূর্ণাঙ্ক min, পূর্ণাঙ্ক max, পূর্ণাঙ্ক size, ভিন্ন angle, ভিন্ন aspect, ভিন্ন hue, ভিন্ন saturation, ভিন্ন exposure)
{
    যদি(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;
    d.X = load_image_augment_paths(paths, n, min, max, size, angle, aspect, hue, saturation, exposure, 0);
    d.y = load_regression_labels_paths(paths, n, k);
    যদি(m) মুক্ত(paths);
    ঘূৰাই d;
}

data select_data(data *orig, পূর্ণাঙ্ক *inds)
{
    data d = {0};
    d.shallow = 1;
    d.w = orig[0].w;
    d.h = orig[0].h;

    d.X.rows = orig[0].X.rows;
    d.y.rows = orig[0].X.rows;

    d.X.cols = orig[0].X.cols;
    d.y.cols = orig[0].y.cols;

    d.X.vals = calloc(orig[0].X.rows, পরিমাণ(ভিন্ন *));
    d.y.vals = calloc(orig[0].y.rows, পরিমাণ(ভিন্ন *));
    পূর্ণাঙ্ক i;
    বাবে(i = 0; i < d.X.rows; ++i){
        d.X.vals[i] = orig[inds[i]].X.vals[i];
        d.y.vals[i] = orig[inds[i]].y.vals[i];
    }
    ঘূৰাই d;
}

data *tile_data(data orig, পূর্ণাঙ্ক divs, পূর্ণাঙ্ক size)
{
    data *ds = calloc(divs*divs, পরিমাণ(data));
    পূর্ণাঙ্ক i, j;
#pragma omp parallel বাবে
    বাবে(i = 0; i < divs*divs; ++i){
        data d;
        d.shallow = 0;
        d.w = orig.w/divs * size;
        d.h = orig.h/divs * size;
        d.X.rows = orig.X.rows;
        d.X.cols = d.w*d.h*3;
        d.X.vals = calloc(d.X.rows, পরিমাণ(ভিন্ন*));

        d.y = copy_matrix(orig.y);
#pragma omp parallel বাবে
        বাবে(j = 0; j < orig.X.rows; ++j){
            পূর্ণাঙ্ক x = (i%divs) * orig.w / divs - (d.w - orig.w/divs)/2;
            পূর্ণাঙ্ক y = (i/divs) * orig.h / divs - (d.h - orig.h/divs)/2;
            image im = float_to_image(orig.w, orig.h, 3, orig.X.vals[j]);
            d.X.vals[j] = crop_image(im, x, y, d.w, d.h).data;
        }
        ds[i] = d;
    }
    ঘূৰাই ds;
}

data resize_data(data orig, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h)
{
    data d = {0};
    d.shallow = 0;
    d.w = w;
    d.h = h;
    পূর্ণাঙ্ক i;
    d.X.rows = orig.X.rows;
    d.X.cols = w*h*3;
    d.X.vals = calloc(d.X.rows, পরিমাণ(ভিন্ন*));

    d.y = copy_matrix(orig.y);
#pragma omp parallel বাবে
    বাবে(i = 0; i < orig.X.rows; ++i){
        image im = float_to_image(orig.w, orig.h, 3, orig.X.vals[i]);
        d.X.vals[i] = resize_image(im, w, h).data;
    }
    ঘূৰাই d;
}

data load_data_augment(ৱর্ণ **paths, পূর্ণাঙ্ক n, পূর্ণাঙ্ক m, ৱর্ণ **labels, পূর্ণাঙ্ক k, বৃক্ষ *hierarchy, পূর্ণাঙ্ক min, পূর্ণাঙ্ক max, পূর্ণাঙ্ক size, ভিন্ন angle, ভিন্ন aspect, ভিন্ন hue, ভিন্ন saturation, ভিন্ন exposure, পূর্ণাঙ্ক center)
{
    যদি(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;
    d.w=size;
    d.h=size;
    d.X = load_image_augment_paths(paths, n, min, max, size, angle, aspect, hue, saturation, exposure, center);
    d.y = load_labels_paths(paths, n, labels, k, hierarchy);
    যদি(m) মুক্ত(paths);
    ঘূৰাই d;
}

data load_data_tag(ৱর্ণ **paths, পূর্ণাঙ্ক n, পূর্ণাঙ্ক m, পূর্ণাঙ্ক k, পূর্ণাঙ্ক min, পূর্ণাঙ্ক max, পূর্ণাঙ্ক size, ভিন্ন angle, ভিন্ন aspect, ভিন্ন hue, ভিন্ন saturation, ভিন্ন exposure)
{
    যদি(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.w = size;
    d.h = size;
    d.shallow = 0;
    d.X = load_image_augment_paths(paths, n, min, max, size, angle, aspect, hue, saturation, exposure, 0);
    d.y = load_tags_paths(paths, n, k);
    যদি(m) মুক্ত(paths);
    ঘূৰাই d;
}

matrix concat_matrix(matrix m1, matrix m2)
{
    পূর্ণাঙ্ক i, গুনতি = 0;
    matrix m;
    m.cols = m1.cols;
    m.rows = m1.rows+m2.rows;
    m.vals = calloc(m1.rows + m2.rows, পরিমাণ(ভিন্ন*));
    বাবে(i = 0; i < m1.rows; ++i){
        m.vals[গুনতি++] = m1.vals[i];
    }
    বাবে(i = 0; i < m2.rows; ++i){
        m.vals[গুনতি++] = m2.vals[i];
    }
    ঘূৰাই m;
}

data concat_data(data d1, data d2)
{
    data d = {0};
    d.shallow = 1;
    d.X = concat_matrix(d1.X, d2.X);
    d.y = concat_matrix(d1.y, d2.y);
    d.w = d1.w;
    d.h = d1.h;
    ঘূৰাই d;
}

data concat_datas(data *d, পূর্ণাঙ্ক n)
{
    পূর্ণাঙ্ক i;
    data out = {0};
    বাবে(i = 0; i < n; ++i){
        data new = concat_data(d[i], out);
        free_data(out);
        out = new;
    }
    ঘূৰাই out;
}

data load_categorical_data_csv(ৱর্ণ *filename, পূর্ণাঙ্ক target, পূর্ণাঙ্ক k)
{
    data d = {0};
    d.shallow = 0;
    matrix X = csv_to_matrix(filename);
    ভিন্ন *truth_1d = বেৰ_কৰো_column(&X, target);
    ভিন্ন **truth = one_hot_encode(truth_1d, X.rows, k);
    matrix y;
    y.rows = X.rows;
    y.cols = k;
    y.vals = truth;
    d.X = X;
    d.y = y;
    মুক্ত(truth_1d);
    ঘূৰাই d;
}

data load_cifar10_data(ৱর্ণ *filename)
{
    data d = {0};
    d.shallow = 0;
    long i,j;
    matrix X = make_matrix(10000, 3072);
    matrix y = make_matrix(10000, 10);
    d.X = X;
    d.y = y;

    FILE *fp = fopen(filename, "rb");
    যদি(!fp) file_error(filename);
    বাবে(i = 0; i < 10000; ++i){
        unsigned ৱর্ণ bytes[3073];
        fread(bytes, 1, 3073, fp);
        পূর্ণাঙ্ক class = bytes[0];
        y.vals[i][class] = 1;
        বাবে(j = 0; j < X.cols; ++j){
            X.vals[i][j] = (দ্ৱিগুণ)bytes[j+1];
        }
    }
    scale_data_rows(d, 1./255);
    //normalize_data_rows(d);
    fclose(fp);
    ঘূৰাই d;
}

শূন্য get_random_batch(data d, পূর্ণাঙ্ক n, ভিন্ন *X, ভিন্ন *y)
{
    পূর্ণাঙ্ক j;
    বাবে(j = 0; j < n; ++j){
        পূর্ণাঙ্ক index = rand()%d.X.rows;
        memcpy(X+j*d.X.cols, d.X.vals[index], d.X.cols*পরিমাণ(ভিন্ন));
        memcpy(y+j*d.y.cols, d.y.vals[index], d.y.cols*পরিমাণ(ভিন্ন));
    }
}

শূন্য get_next_batch(data d, পূর্ণাঙ্ক n, পূর্ণাঙ্ক offset, ভিন্ন *X, ভিন্ন *y)
{
    পূর্ণাঙ্ক j;
    বাবে(j = 0; j < n; ++j){
        পূর্ণাঙ্ক index = offset + j;
        memcpy(X+j*d.X.cols, d.X.vals[index], d.X.cols*পরিমাণ(ভিন্ন));
        যদি(y) memcpy(y+j*d.y.cols, d.y.vals[index], d.y.cols*পরিমাণ(ভিন্ন));
    }
}

শূন্য smooth_data(data d)
{
    পূর্ণাঙ্ক i, j;
    ভিন্ন scale = 1. / d.y.cols;
    ভিন্ন eps = .1;
    বাবে(i = 0; i < d.y.rows; ++i){
        বাবে(j = 0; j < d.y.cols; ++j){
            d.y.vals[i][j] = eps * scale + (1-eps) * d.y.vals[i][j];
        }
    }
}

data load_all_cifar10()
{
    data d = {0};
    d.shallow = 0;
    পূর্ণাঙ্ক i,j,b;
    matrix X = make_matrix(50000, 3072);
    matrix y = make_matrix(50000, 10);
    d.X = X;
    d.y = y;


    বাবে(b = 0; b < 5; ++b){
        ৱর্ণ buff[256];
        sprintf(buff, "data/cifar/cifar-10-batches-bin/data_batch_%d.bin", b+1);
        FILE *fp = fopen(buff, "rb");
        যদি(!fp) file_error(buff);
        বাবে(i = 0; i < 10000; ++i){
            unsigned ৱর্ণ bytes[3073];
            fread(bytes, 1, 3073, fp);
            পূর্ণাঙ্ক class = bytes[0];
            y.vals[i+b*10000][class] = 1;
            বাবে(j = 0; j < X.cols; ++j){
                X.vals[i+b*10000][j] = (দ্ৱিগুণ)bytes[j+1];
            }
        }
        fclose(fp);
    }
    //normalize_data_rows(d);
    scale_data_rows(d, 1./255);
    smooth_data(d);
    ঘূৰাই d;
}

data load_go(ৱর্ণ *filename)
{
    FILE *fp = fopen(filename, "rb");
    matrix X = make_matrix(3363059, 361);
    matrix y = make_matrix(3363059, 361);
    পূর্ণাঙ্ক row, col;

    যদি(!fp) file_error(filename);
    ৱর্ণ *label;
    পূর্ণাঙ্ক গুনতি = 0;
    যেতিয়ালৈ((label = fgetl(fp))){
        পূর্ণাঙ্ক i;
        যদি(গুনতি == X.rows){
            X = resize_matrix(X, গুনতি*2);
            y = resize_matrix(y, গুনতি*2);
        }
        sscanf(label, "%d %d", &row, &col);
        ৱর্ণ *board = fgetl(fp);

        পূর্ণাঙ্ক index = row*19 + col;
        y.vals[গুনতি][index] = 1;

        বাবে(i = 0; i < 19*19; ++i){
            ভিন্ন val = 0;
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

    data d = {0};
    d.shallow = 0;
    d.X = X;
    d.y = y;


    fclose(fp);

    ঘূৰাই d;
}


শূন্য randomize_data(data d)
{
    পূর্ণাঙ্ক i;
    বাবে(i = d.X.rows-1; i > 0; --i){
        পূর্ণাঙ্ক index = rand()%i;
        ভিন্ন *swap = d.X.vals[index];
        d.X.vals[index] = d.X.vals[i];
        d.X.vals[i] = swap;

        swap = d.y.vals[index];
        d.y.vals[index] = d.y.vals[i];
        d.y.vals[i] = swap;
    }
}

শূন্য scale_data_rows(data d, ভিন্ন s)
{
    পূর্ণাঙ্ক i;
    বাবে(i = 0; i < d.X.rows; ++i){
        scale_array(d.X.vals[i], d.X.cols, s);
    }
}

শূন্য translate_data_rows(data d, ভিন্ন s)
{
    পূর্ণাঙ্ক i;
    বাবে(i = 0; i < d.X.rows; ++i){
        translate_array(d.X.vals[i], d.X.cols, s);
    }
}

data copy_data(data d)
{
    data c = {0};
    c.w = d.w;
    c.h = d.h;
    c.shallow = 0;
    c.অঙ্ক_boxes = d.অঙ্ক_boxes;
    c.boxes = d.boxes;
    c.X = copy_matrix(d.X);
    c.y = copy_matrix(d.y);
    ঘূৰাই c;
}

শূন্য normalize_data_rows(data d)
{
    পূর্ণাঙ্ক i;
    বাবে(i = 0; i < d.X.rows; ++i){
        normalize_array(d.X.vals[i], d.X.cols);
    }
}

data get_data_part(data d, পূর্ণাঙ্ক part, পূর্ণাঙ্ক total)
{
    data p = {0};
    p.shallow = 1;
    p.X.rows = d.X.rows * (part + 1) / total - d.X.rows * part / total;
    p.y.rows = d.y.rows * (part + 1) / total - d.y.rows * part / total;
    p.X.cols = d.X.cols;
    p.y.cols = d.y.cols;
    p.X.vals = d.X.vals + d.X.rows * part / total;
    p.y.vals = d.y.vals + d.y.rows * part / total;
    ঘূৰাই p;
}

data get_random_data(data d, পূর্ণাঙ্ক অঙ্ক)
{
    data r = {0};
    r.shallow = 1;

    r.X.rows = অঙ্ক;
    r.y.rows = অঙ্ক;

    r.X.cols = d.X.cols;
    r.y.cols = d.y.cols;

    r.X.vals = calloc(অঙ্ক, পরিমাণ(ভিন্ন *));
    r.y.vals = calloc(অঙ্ক, পরিমাণ(ভিন্ন *));

    পূর্ণাঙ্ক i;
    বাবে(i = 0; i < অঙ্ক; ++i){
        পূর্ণাঙ্ক index = rand()%d.X.rows;
        r.X.vals[i] = d.X.vals[index];
        r.y.vals[i] = d.y.vals[index];
    }
    ঘূৰাই r;
}

data *split_data(data d, পূর্ণাঙ্ক part, পূর্ণাঙ্ক total)
{
    data *split = calloc(2, পরিমাণ(data));
    পূর্ণাঙ্ক i;
    পূর্ণাঙ্ক start = part*d.X.rows/total;
    পূর্ণাঙ্ক end = (part+1)*d.X.rows/total;
    data train;
    data test;
    train.shallow = test.shallow = 1;

    test.X.rows = test.y.rows = end-start;
    train.X.rows = train.y.rows = d.X.rows - (end-start);
    train.X.cols = test.X.cols = d.X.cols;
    train.y.cols = test.y.cols = d.y.cols;

    train.X.vals = calloc(train.X.rows, পরিমাণ(ভিন্ন*));
    test.X.vals = calloc(test.X.rows, পরিমাণ(ভিন্ন*));
    train.y.vals = calloc(train.y.rows, পরিমাণ(ভিন্ন*));
    test.y.vals = calloc(test.y.rows, পরিমাণ(ভিন্ন*));

    বাবে(i = 0; i < start; ++i){
        train.X.vals[i] = d.X.vals[i];
        train.y.vals[i] = d.y.vals[i];
    }
    বাবে(i = start; i < end; ++i){
        test.X.vals[i-start] = d.X.vals[i];
        test.y.vals[i-start] = d.y.vals[i];
    }
    বাবে(i = end; i < d.X.rows; ++i){
        train.X.vals[i-(end-start)] = d.X.vals[i];
        train.y.vals[i-(end-start)] = d.y.vals[i];
    }
    split[0] = train;
    split[1] = test;
    ঘূৰাই split;
}

