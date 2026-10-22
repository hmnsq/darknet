#அந்தர்பாவ "data.h"
#அந்தர்பாவ "utils.h"
#அந்தர்பாவ "image.h"
#அந்தர்பாவ "cuda.h"

#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <stdlib.h>
#அந்தர்பாவ <string.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

list *get_paths(எழுத்து *filename)
{
    எழுத்து *path;
    FILE *file = fopen(filename, "r");
    எனில்(!file) file_error(filename);
    list *வரிகள் = make_list();
    போது((path=fgetl(file))){
        list_செருகு(வரிகள், path);
    }
    fclose(file);
    திரும்பு வரிகள்;
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

எழுத்து **get_random_paths(எழுத்து **paths, முழு n, முழு m)
{
    எழுத்து **random_paths = calloc(n, அளவு(எழுத்து*));
    முழு i;
    pthread_mutex_lock(&mutex);
    க்கு(i = 0; i < n; ++i){
        முழு index = rand()%m;
        random_paths[i] = paths[index];
        //if(i == 0) printf("%s\n", paths[index]);
    }
    pthread_mutex_unlock(&mutex);
    திரும்பு random_paths;
}

எழுத்து **find_replace_paths(எழுத்து **paths, முழு n, எழுத்து *find, எழுத்து *replace)
{
    எழுத்து **replace_paths = calloc(n, அளவு(எழுத்து*));
    முழு i;
    க்கு(i = 0; i < n; ++i){
        எழுத்து replaced[4096];
        find_replace(paths[i], find, replace, replaced);
        replace_paths[i] = copy_string(replaced);
    }
    திரும்பு replace_paths;
}

matrix load_image_paths_gray(எழுத்து **paths, முழு n, முழு w, முழு h)
{
    முழு i;
    matrix X;
    X.rows = n;
    X.vals = calloc(X.rows, அளவு(தசம*));
    X.cols = 0;

    க்கு(i = 0; i < n; ++i){
        image im = load_image(paths[i], w, h, 3);

        image gray = grayscale_image(im);
        free_image(im);
        im = gray;

        X.vals[i] = im.data;
        X.cols = im.h*im.w*im.c;
    }
    திரும்பு X;
}

matrix load_image_paths(எழுத்து **paths, முழு n, முழு w, முழு h)
{
    முழு i;
    matrix X;
    X.rows = n;
    X.vals = calloc(X.rows, அளவு(தசம*));
    X.cols = 0;

    க்கு(i = 0; i < n; ++i){
        image im = load_image_color(paths[i], w, h);
        X.vals[i] = im.data;
        X.cols = im.h*im.w*im.c;
    }
    திரும்பு X;
}

matrix load_image_augment_paths(எழுத்து **paths, முழு n, முழு min, முழு max, முழு size, தசம angle, தசம aspect, தசம hue, தசம saturation, தசம exposure, முழு center)
{
    முழு i;
    matrix X;
    X.rows = n;
    X.vals = calloc(X.rows, அளவு(தசம*));
    X.cols = 0;

    க்கு(i = 0; i < n; ++i){
        image im = load_image_color(paths[i], 0, 0);
        image crop;
        எனில்(center){
            crop = center_crop_image(im, size, size);
        } இல்லையெனில் {
            crop = random_augment_image(im, angle, aspect, min, max, size, size);
        }
        முழு flip = rand()%2;
        எனில் (flip) flip_image(crop);
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
    திரும்பு X;
}


box_label *read_boxes(எழுத்து *filename, முழு *n)
{
    FILE *file = fopen(filename, "r");
    எனில்(!file) file_error(filename);
    தசம x, y, h, w;
    முழு id;
    முழு மொத்தம் = 0;
    முழு size = 64;
    box_label *boxes = calloc(size, அளவு(box_label));
    போது(fscanf(file, "%d %f %f %f %f", &id, &x, &y, &w, &h) == 5){
        எனில்(மொத்தம் == size) {
            size = size * 2;
            boxes = realloc(boxes, size*அளவு(box_label));
        }
        boxes[மொத்தம்].id = id;
        boxes[மொத்தம்].x = x;
        boxes[மொத்தம்].y = y;
        boxes[மொத்தம்].h = h;
        boxes[மொத்தம்].w = w;
        boxes[மொத்தம்].இடது   = x - w/2;
        boxes[மொத்தம்].வலது  = x + w/2;
        boxes[மொத்தம்].உச்சி    = y - h/2;
        boxes[மொத்தம்].bottom = y + h/2;
        ++மொத்தம்;
    }
    fclose(file);
    *n = மொத்தம்;
    திரும்பு boxes;
}

வெறுமை randomize_boxes(box_label *b, முழு n)
{
    முழு i;
    க்கு(i = 0; i < n; ++i){
        box_label swap = b[i];
        முழு index = rand()%n;
        b[i] = b[index];
        b[index] = swap;
    }
}

வெறுமை correct_boxes(box_label *boxes, முழு n, தசம dx, தசம dy, தசம sx, தசம sy, முழு flip)
{
    முழு i;
    க்கு(i = 0; i < n; ++i){
        எனில்(boxes[i].x == 0 && boxes[i].y == 0) {
            boxes[i].x = 999999;
            boxes[i].y = 999999;
            boxes[i].w = 999999;
            boxes[i].h = 999999;
            தொடர்;
        }
        boxes[i].இடது   = boxes[i].இடது  * sx - dx;
        boxes[i].வலது  = boxes[i].வலது * sx - dx;
        boxes[i].உச்சி    = boxes[i].உச்சி   * sy - dy;
        boxes[i].bottom = boxes[i].bottom* sy - dy;

        எனில்(flip){
            தசம swap = boxes[i].இடது;
            boxes[i].இடது = 1. - boxes[i].வலது;
            boxes[i].வலது = 1. - swap;
        }

        boxes[i].இடது =  constrain(0, 1, boxes[i].இடது);
        boxes[i].வலது = constrain(0, 1, boxes[i].வலது);
        boxes[i].உச்சி =   constrain(0, 1, boxes[i].உச்சி);
        boxes[i].bottom =   constrain(0, 1, boxes[i].bottom);

        boxes[i].x = (boxes[i].இடது+boxes[i].வலது)/2;
        boxes[i].y = (boxes[i].உச்சி+boxes[i].bottom)/2;
        boxes[i].w = (boxes[i].வலது - boxes[i].இடது);
        boxes[i].h = (boxes[i].bottom - boxes[i].உச்சி);

        boxes[i].w = constrain(0, 1, boxes[i].w);
        boxes[i].h = constrain(0, 1, boxes[i].h);
    }
}

வெறுமை fill_truth_swag(எழுத்து *path, தசம *truth, முழு classes, முழு flip, தசம dx, தசம dy, தசம sx, தசம sy)
{
    எழுத்து labelpath[4096];
    find_replace(path, "images", "labels", labelpath);
    find_replace(labelpath, "JPEGImages", "labels", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);

    முழு மொத்தம் = 0;
    box_label *boxes = read_boxes(labelpath, &மொத்தம்);
    randomize_boxes(boxes, மொத்தம்);
    correct_boxes(boxes, மொத்தம், dx, dy, sx, sy, flip);
    தசம x,y,w,h;
    முழு id;
    முழு i;

    க்கு (i = 0; i < மொத்தம் && i < 90; ++i) {
        x =  boxes[i].x;
        y =  boxes[i].y;
        w =  boxes[i].w;
        h =  boxes[i].h;
        id = boxes[i].id;

        எனில் (w < .0 || h < .0) தொடர்;

        முழு index = (4+classes) * i;

        truth[index++] = x;
        truth[index++] = y;
        truth[index++] = w;
        truth[index++] = h;

        எனில் (id < classes) truth[index+id] = 1;
    }
    விடுவி(boxes);
}

வெறுமை fill_truth_region(எழுத்து *path, தசம *truth, முழு classes, முழு இலக்கம்_boxes, முழு flip, தசம dx, தசம dy, தசம sx, தசம sy)
{
    எழுத்து labelpath[4096];
    find_replace(path, "images", "labels", labelpath);
    find_replace(labelpath, "JPEGImages", "labels", labelpath);

    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".png", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    முழு மொத்தம் = 0;
    box_label *boxes = read_boxes(labelpath, &மொத்தம்);
    randomize_boxes(boxes, மொத்தம்);
    correct_boxes(boxes, மொத்தம், dx, dy, sx, sy, flip);
    தசம x,y,w,h;
    முழு id;
    முழு i;

    க்கு (i = 0; i < மொத்தம்; ++i) {
        x =  boxes[i].x;
        y =  boxes[i].y;
        w =  boxes[i].w;
        h =  boxes[i].h;
        id = boxes[i].id;

        எனில் (w < .005 || h < .005) தொடர்;

        முழு col = (முழு)(x*இலக்கம்_boxes);
        முழு row = (முழு)(y*இலக்கம்_boxes);

        x = x*இலக்கம்_boxes - col;
        y = y*இலக்கம்_boxes - row;

        முழு index = (col+row*இலக்கம்_boxes)*(5+classes);
        எனில் (truth[index]) தொடர்;
        truth[index++] = 1;

        எனில் (id < classes) truth[index+id] = 1;
        index += classes;

        truth[index++] = x;
        truth[index++] = y;
        truth[index++] = w;
        truth[index++] = h;
    }
    விடுவி(boxes);
}

வெறுமை load_rle(image im, முழு *rle, முழு n)
{
    முழு மொத்தம் = 0;
    முழு curr = 0;
    முழு i,j;
    க்கு(i = 0; i < n; ++i){
        க்கு(j = 0; j < rle[i]; ++j){
            im.data[மொத்தம்++] = curr;
        }
        curr = 1 - curr;
    }
    க்கு(; மொத்தம் < im.h*im.w*im.c; ++மொத்தம்){
        im.data[மொத்தம்] = curr;
    }
}

வெறுமை or_image(image src, image dest, முழு c)
{
    முழு i;
    க்கு(i = 0; i < src.w*src.h; ++i){
        எனில்(src.data[i]) dest.data[dest.w*dest.h*c + i] = 1;
    }
}

வெறுமை exclusive_image(image src)
{
    முழு k, j, i;
    முழு s = src.w*src.h;
    க்கு(k = 0; k < src.c-1; ++k){
        க்கு(i = 0; i < s; ++i){
            எனில் (src.data[k*s + i]){
                க்கு(j = k+1; j < src.c; ++j){
                    src.data[j*s + i] = 0;
                }
            }
        }
    }
}

box bound_image(image im)
{
    முழு x,y;
    முழு minx = im.w;
    முழு miny = im.h;
    முழு maxx = 0;
    முழு maxy = 0;
    க்கு(y = 0; y < im.h; ++y){
        க்கு(x = 0; x < im.w; ++x){
            எனில்(im.data[y*im.w + x]){
                minx = (x < minx) ? x : minx;
                miny = (y < miny) ? y : miny;
                maxx = (x > maxx) ? x : maxx;
                maxy = (y > maxy) ? y : maxy;
            }
        }
    }
    box b = {minx, miny, maxx-minx + 1, maxy-miny + 1};
    //printf("%f %f %f %f\n", b.x, b.y, b.w, b.h);
    திரும்பு b;
}

வெறுமை fill_truth_iseg(எழுத்து *path, முழு இலக்கம்_boxes, தசம *truth, முழு classes, முழு w, முழு h, augment_args aug, முழு flip, முழு mw, முழு mh)
{
    எழுத்து labelpath[4096];
    find_replace(path, "images", "mask", labelpath);
    find_replace(labelpath, "JPEGImages", "mask", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    FILE *file = fopen(labelpath, "r");
    எனில்(!file) file_error(labelpath);
    எழுத்து buff[32788];
    முழு id;
    முழு i = 0;
    முழு j;
    image part = make_image(w, h, 1);
    போது((fscanf(file, "%d %s", &id, buff) == 2) && i < இலக்கம்_boxes){
        முழு n = 0;
        முழு *rle = read_intlist(buff, &n, 0);
        load_rle(part, rle, n);
        image sized = rotate_crop_image(part, aug.rad, aug.scale, aug.w, aug.h, aug.dx, aug.dy, aug.aspect);
        எனில்(flip) flip_image(sized);

        image mask = resize_image(sized, mw, mh);
        truth[i*(mw*mh+1)] = id;
        க்கு(j = 0; j < mw*mh; ++j){
            truth[i*(mw*mh + 1) + 1 + j] = mask.data[j];
        }
        ++i;

        free_image(mask);
        free_image(sized);
        விடுவி(rle);
    }
    எனில்(i < இலக்கம்_boxes) truth[i*(mw*mh+1)] = -1;
    fclose(file);
    free_image(part);
}

வெறுமை fill_truth_mask(எழுத்து *path, முழு இலக்கம்_boxes, தசம *truth, முழு classes, முழு w, முழு h, augment_args aug, முழு flip, முழு mw, முழு mh)
{
    எழுத்து labelpath[4096];
    find_replace(path, "images", "mask", labelpath);
    find_replace(labelpath, "JPEGImages", "mask", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    FILE *file = fopen(labelpath, "r");
    எனில்(!file) file_error(labelpath);
    எழுத்து buff[32788];
    முழு id;
    முழு i = 0;
    image part = make_image(w, h, 1);
    போது((fscanf(file, "%d %s", &id, buff) == 2) && i < இலக்கம்_boxes){
        முழு n = 0;
        முழு *rle = read_intlist(buff, &n, 0);
        load_rle(part, rle, n);
        image sized = rotate_crop_image(part, aug.rad, aug.scale, aug.w, aug.h, aug.dx, aug.dy, aug.aspect);
        எனில்(flip) flip_image(sized);
        box b = bound_image(sized);
        எனில்(b.w > 0){
            image crop = crop_image(sized, b.x, b.y, b.w, b.h);
            image mask = resize_image(crop, mw, mh);
            truth[i*(4 + mw*mh + 1) + 0] = (b.x + b.w/2.)/sized.w;
            truth[i*(4 + mw*mh + 1) + 1] = (b.y + b.h/2.)/sized.h;
            truth[i*(4 + mw*mh + 1) + 2] = b.w/sized.w;
            truth[i*(4 + mw*mh + 1) + 3] = b.h/sized.h;
            முழு j;
            க்கு(j = 0; j < mw*mh; ++j){
                truth[i*(4 + mw*mh + 1) + 4 + j] = mask.data[j];
            }
            truth[i*(4 + mw*mh + 1) + 4 + mw*mh] = id;
            free_image(crop);
            free_image(mask);
            ++i;
        }
        free_image(sized);
        விடுவி(rle);
    }
    fclose(file);
    free_image(part);
}


வெறுமை fill_truth_detection(எழுத்து *path, முழு இலக்கம்_boxes, தசம *truth, முழு classes, முழு flip, தசம dx, தசம dy, தசம sx, தசம sy)
{
    எழுத்து labelpath[4096];
    find_replace(path, "images", "labels", labelpath);
    find_replace(labelpath, "JPEGImages", "labels", labelpath);

    find_replace(labelpath, "raw", "labels", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".png", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    முழு மொத்தம் = 0;
    box_label *boxes = read_boxes(labelpath, &மொத்தம்);
    randomize_boxes(boxes, மொத்தம்);
    correct_boxes(boxes, மொத்தம், dx, dy, sx, sy, flip);
    எனில்(மொத்தம் > இலக்கம்_boxes) மொத்தம் = இலக்கம்_boxes;
    தசம x,y,w,h;
    முழு id;
    முழு i;
    முழு sub = 0;

    க்கு (i = 0; i < மொத்தம்; ++i) {
        x =  boxes[i].x;
        y =  boxes[i].y;
        w =  boxes[i].w;
        h =  boxes[i].h;
        id = boxes[i].id;

        எனில் ((w < .001 || h < .001)) {
            ++sub;
            தொடர்;
        }

        truth[(i-sub)*5+0] = x;
        truth[(i-sub)*5+1] = y;
        truth[(i-sub)*5+2] = w;
        truth[(i-sub)*5+3] = h;
        truth[(i-sub)*5+4] = id;
    }
    விடுவி(boxes);
}

#define NUMCHARS 37

வெறுமை print_letters(தசம *pred, முழு n)
{
    முழு i;
    க்கு(i = 0; i < n; ++i){
        முழு index = max_index(pred+i*NUMCHARS, NUMCHARS);
        printf("%c", int_to_alphanum(index));
    }
    printf("\n");
}

வெறுமை fill_truth_captcha(எழுத்து *path, முழு n, தசம *truth)
{
    எழுத்து *begin = strrchr(path, '/');
    ++begin;
    முழு i;
    க்கு(i = 0; i < strlen(begin) && i < n && begin[i] != '.'; ++i){
        முழு index = alphanum_to_int(begin[i]);
        எனில்(index > 35) printf("Bad %c\n", begin[i]);
        truth[i*NUMCHARS+index] = 1;
    }
    க்கு(;i < n; ++i){
        truth[i*NUMCHARS + NUMCHARS-1] = 1;
    }
}

data load_data_captcha(எழுத்து **paths, முழு n, முழு m, முழு k, முழு w, முழு h)
{
    எனில்(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;
    d.X = load_image_paths(paths, n, w, h);
    d.y = make_matrix(n, k*NUMCHARS);
    முழு i;
    க்கு(i = 0; i < n; ++i){
        fill_truth_captcha(paths[i], k, d.y.vals[i]);
    }
    எனில்(m) விடுவி(paths);
    திரும்பு d;
}

data load_data_captcha_encode(எழுத்து **paths, முழு n, முழு m, முழு w, முழு h)
{
    எனில்(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;
    d.X = load_image_paths(paths, n, w, h);
    d.X.cols = 17100;
    d.y = d.X;
    எனில்(m) விடுவி(paths);
    திரும்பு d;
}

வெறுமை fill_truth(எழுத்து *path, எழுத்து **labels, முழு k, தசம *truth)
{
    முழு i;
    memset(truth, 0, k*அளவு(தசம));
    முழு மொத்தம் = 0;
    க்கு(i = 0; i < k; ++i){
        எனில்(strstr(path, labels[i])){
            truth[i] = 1;
            ++மொத்தம்;
            //printf("%s %s %d\n", path, labels[i], i);
        }
    }
    எனில்(மொத்தம் != 1 && (k != 1 || மொத்தம் != 0)) printf("Too many or too few labels: %d, %s\n", மொத்தம், path);
}

வெறுமை fill_hierarchy(தசம *truth, முழு k, மரம் *hierarchy)
{
    முழு j;
    க்கு(j = 0; j < k; ++j){
        எனில்(truth[j]){
            முழு parent = hierarchy->parent[j];
            போது(parent >= 0){
                truth[parent] = 1;
                parent = hierarchy->parent[parent];
            }
        }
    }
    முழு i;
    முழு மொத்தம் = 0;
    க்கு(j = 0; j < hierarchy->groups; ++j){
        //printf("%d\n", count);
        முழு mask = 1;
        க்கு(i = 0; i < hierarchy->group_size[j]; ++i){
            எனில்(truth[மொத்தம் + i]){
                mask = 0;
                நிறுத்து;
            }
        }
        எனில் (mask) {
            க்கு(i = 0; i < hierarchy->group_size[j]; ++i){
                truth[மொத்தம் + i] = SECRET_இலக்கம்;
            }
        }
        மொத்தம் += hierarchy->group_size[j];
    }
}

matrix load_regression_labels_paths(எழுத்து **paths, முழு n, முழு k)
{
    matrix y = make_matrix(n, k);
    முழு i,j;
    க்கு(i = 0; i < n; ++i){
        எழுத்து labelpath[4096];
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
        க்கு(j = 0; j < k; ++j){
            fscanf(file, "%f", &(y.vals[i][j]));
        }
        fclose(file);
    }
    திரும்பு y;
}

matrix load_labels_paths(எழுத்து **paths, முழு n, எழுத்து **labels, முழு k, மரம் *hierarchy)
{
    matrix y = make_matrix(n, k);
    முழு i;
    க்கு(i = 0; i < n && labels; ++i){
        fill_truth(paths[i], labels, k, y.vals[i]);
        எனில்(hierarchy){
            fill_hierarchy(y.vals[i], k, hierarchy);
        }
    }
    திரும்பு y;
}

matrix load_tags_paths(எழுத்து **paths, முழு n, முழு k)
{
    matrix y = make_matrix(n, k);
    முழு i;
    //int count = 0;
    க்கு(i = 0; i < n; ++i){
        எழுத்து label[4096];
        find_replace(paths[i], "images", "labels", label);
        find_replace(label, ".jpg", ".txt", label);
        FILE *file = fopen(label, "r");
        எனில் (!file) தொடர்;
        //++count;
        முழு tag;
        போது(fscanf(file, "%d", &tag) == 1){
            எனில்(tag < k){
                y.vals[i][tag] = 1;
            }
        }
        fclose(file);
    }
    //printf("%d/%d\n", count, n);
    திரும்பு y;
}

எழுத்து **get_labels(எழுத்து *filename)
{
    list *plist = get_paths(filename);
    எழுத்து **labels = (எழுத்து **)list_to_array(plist);
    free_list(plist);
    திரும்பு labels;
}

வெறுமை free_data(data d)
{
    எனில்(!d.shallow){
        free_matrix(d.X);
        free_matrix(d.y);
    }இல்லையெனில்{
        விடுவி(d.X.vals);
        விடுவி(d.y.vals);
    }
}

image get_segmentation_image(எழுத்து *path, முழு w, முழு h, முழு classes)
{
    எழுத்து labelpath[4096];
    find_replace(path, "images", "mask", labelpath);
    find_replace(labelpath, "JPEGImages", "mask", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    image mask = make_image(w, h, classes);
    FILE *file = fopen(labelpath, "r");
    எனில்(!file) file_error(labelpath);
    எழுத்து buff[32788];
    முழு id;
    image part = make_image(w, h, 1);
    போது(fscanf(file, "%d %s", &id, buff) == 2){
        முழு n = 0;
        முழு *rle = read_intlist(buff, &n, 0);
        load_rle(part, rle, n);
        or_image(part, mask, id);
        விடுவி(rle);
    }
    //exclusive_image(mask);
    fclose(file);
    free_image(part);
    திரும்பு mask;
}

image get_segmentation_image2(எழுத்து *path, முழு w, முழு h, முழு classes)
{
    எழுத்து labelpath[4096];
    find_replace(path, "images", "mask", labelpath);
    find_replace(labelpath, "JPEGImages", "mask", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    image mask = make_image(w, h, classes+1);
    முழு i;
    க்கு(i = 0; i < w*h; ++i){
        mask.data[w*h*classes + i] = 1;
    }
    FILE *file = fopen(labelpath, "r");
    எனில்(!file) file_error(labelpath);
    எழுத்து buff[32788];
    முழு id;
    image part = make_image(w, h, 1);
    போது(fscanf(file, "%d %s", &id, buff) == 2){
        முழு n = 0;
        முழு *rle = read_intlist(buff, &n, 0);
        load_rle(part, rle, n);
        or_image(part, mask, id);
        க்கு(i = 0; i < w*h; ++i){
            எனில்(part.data[i]) mask.data[w*h*classes + i] = 0;
        }
        விடுவி(rle);
    }
    //exclusive_image(mask);
    fclose(file);
    free_image(part);
    திரும்பு mask;
}

data load_data_seg(முழு n, எழுத்து **paths, முழு m, முழு w, முழு h, முழு classes, முழு min, முழு max, தசம angle, தசம aspect, தசம hue, தசம saturation, தசம exposure, முழு div)
{
    எழுத்து **random_paths = get_random_paths(paths, n, m);
    முழு i;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, அளவு(தசம*));
    d.X.cols = h*w*3;


    d.y.rows = n;
    d.y.cols = h*w*classes/div/div;
    d.y.vals = calloc(d.X.rows, அளவு(தசம*));

    க்கு(i = 0; i < n; ++i){
        image orig = load_image_color(random_paths[i], 0, 0);
        augment_args a = random_augment_args(orig, angle, aspect, min, max, w, h);
        image sized = rotate_crop_image(orig, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);

        முழு flip = rand()%2;
        எனில்(flip) flip_image(sized);
        random_distort_image(sized, hue, saturation, exposure);
        d.X.vals[i] = sized.data;

        image mask = get_segmentation_image(random_paths[i], orig.w, orig.h, classes);
        //image mask = make_image(orig.w, orig.h, classes+1);
        image sized_m = rotate_crop_image(mask, a.rad, a.scale/div, a.w/div, a.h/div, a.dx/div, a.dy/div, a.aspect);

        எனில்(flip) flip_image(sized_m);
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
    விடுவி(random_paths);
    திரும்பு d;
}

data load_data_iseg(முழு n, எழுத்து **paths, முழு m, முழு w, முழு h, முழு classes, முழு boxes, முழு div, முழு min, முழு max, தசம angle, தசம aspect, தசம hue, தசம saturation, தசம exposure)
{
    எழுத்து **random_paths = get_random_paths(paths, n, m);
    முழு i;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, அளவு(தசம*));
    d.X.cols = h*w*3;

    d.y = make_matrix(n, (((w/div)*(h/div))+1)*boxes);

    க்கு(i = 0; i < n; ++i){
        image orig = load_image_color(random_paths[i], 0, 0);
        augment_args a = random_augment_args(orig, angle, aspect, min, max, w, h);
        image sized = rotate_crop_image(orig, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);

        முழு flip = rand()%2;
        எனில்(flip) flip_image(sized);
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
    விடுவி(random_paths);
    திரும்பு d;
}

data load_data_mask(முழு n, எழுத்து **paths, முழு m, முழு w, முழு h, முழு classes, முழு boxes, முழு coords, முழு min, முழு max, தசம angle, தசம aspect, தசம hue, தசம saturation, தசம exposure)
{
    எழுத்து **random_paths = get_random_paths(paths, n, m);
    முழு i;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, அளவு(தசம*));
    d.X.cols = h*w*3;

    d.y = make_matrix(n, (coords+1)*boxes);

    க்கு(i = 0; i < n; ++i){
        image orig = load_image_color(random_paths[i], 0, 0);
        augment_args a = random_augment_args(orig, angle, aspect, min, max, w, h);
        image sized = rotate_crop_image(orig, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);

        முழு flip = rand()%2;
        எனில்(flip) flip_image(sized);
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
    விடுவி(random_paths);
    திரும்பு d;
}

data load_data_region(முழு n, எழுத்து **paths, முழு m, முழு w, முழு h, முழு size, முழு classes, தசம jitter, தசம hue, தசம saturation, தசம exposure)
{
    எழுத்து **random_paths = get_random_paths(paths, n, m);
    முழு i;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, அளவு(தசம*));
    d.X.cols = h*w*3;


    முழு k = size*size*(5+classes);
    d.y = make_matrix(n, k);
    க்கு(i = 0; i < n; ++i){
        image orig = load_image_color(random_paths[i], 0, 0);

        முழு oh = orig.h;
        முழு ow = orig.w;

        முழு dw = (ow*jitter);
        முழு dh = (oh*jitter);

        முழு pleft  = rand_uniform(-dw, dw);
        முழு pright = rand_uniform(-dw, dw);
        முழு ptop   = rand_uniform(-dh, dh);
        முழு pbot   = rand_uniform(-dh, dh);

        முழு swidth =  ow - pleft - pright;
        முழு sheight = oh - ptop - pbot;

        தசம sx = (தசம)swidth  / ow;
        தசம sy = (தசம)sheight / oh;

        முழு flip = rand()%2;
        image cropped = crop_image(orig, pleft, ptop, swidth, sheight);

        தசம dx = ((தசம)pleft/ow)/sx;
        தசம dy = ((தசம)ptop /oh)/sy;

        image sized = resize_image(cropped, w, h);
        எனில்(flip) flip_image(sized);
        random_distort_image(sized, hue, saturation, exposure);
        d.X.vals[i] = sized.data;

        fill_truth_region(random_paths[i], d.y.vals[i], classes, size, flip, dx, dy, 1./sx, 1./sy);

        free_image(orig);
        free_image(cropped);
    }
    விடுவி(random_paths);
    திரும்பு d;
}

data load_data_compare(முழு n, எழுத்து **paths, முழு m, முழு classes, முழு w, முழு h)
{
    எனில்(m) paths = get_random_paths(paths, 2*n, m);
    முழு i,j;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, அளவு(தசம*));
    d.X.cols = h*w*6;

    முழு k = 2*(classes);
    d.y = make_matrix(n, k);
    க்கு(i = 0; i < n; ++i){
        image im1 = load_image_color(paths[i*2],   w, h);
        image im2 = load_image_color(paths[i*2+1], w, h);

        d.X.vals[i] = calloc(d.X.cols, அளவு(தசம));
        memcpy(d.X.vals[i],         im1.data, h*w*3*அளவு(தசம));
        memcpy(d.X.vals[i] + h*w*3, im2.data, h*w*3*அளவு(தசம));

        முழு id;
        தசம iou;

        எழுத்து imlabel1[4096];
        எழுத்து imlabel2[4096];
        find_replace(paths[i*2],   "imgs", "labels", imlabel1);
        find_replace(imlabel1, "jpg", "txt", imlabel1);
        FILE *fp1 = fopen(imlabel1, "r");

        போது(fscanf(fp1, "%d %f", &id, &iou) == 2){
            எனில் (d.y.vals[i][2*id] < iou) d.y.vals[i][2*id] = iou;
        }

        find_replace(paths[i*2+1], "imgs", "labels", imlabel2);
        find_replace(imlabel2, "jpg", "txt", imlabel2);
        FILE *fp2 = fopen(imlabel2, "r");

        போது(fscanf(fp2, "%d %f", &id, &iou) == 2){
            எனில் (d.y.vals[i][2*id + 1] < iou) d.y.vals[i][2*id + 1] = iou;
        }

        க்கு (j = 0; j < classes; ++j){
            எனில் (d.y.vals[i][2*j] > .5 &&  d.y.vals[i][2*j+1] < .5){
                d.y.vals[i][2*j] = 1;
                d.y.vals[i][2*j+1] = 0;
            } இல்லையெனில் எனில் (d.y.vals[i][2*j] < .5 &&  d.y.vals[i][2*j+1] > .5){
                d.y.vals[i][2*j] = 0;
                d.y.vals[i][2*j+1] = 1;
            } இல்லையெனில் {
                d.y.vals[i][2*j]   = SECRET_இலக்கம்;
                d.y.vals[i][2*j+1] = SECRET_இலக்கம்;
            }
        }
        fclose(fp1);
        fclose(fp2);

        free_image(im1);
        free_image(im2);
    }
    எனில்(m) விடுவி(paths);
    திரும்பு d;
}

data load_data_swag(எழுத்து **paths, முழு n, முழு classes, தசம jitter)
{
    முழு index = rand()%n;
    எழுத்து *random_path = paths[index];

    image orig = load_image_color(random_path, 0, 0);
    முழு h = orig.h;
    முழு w = orig.w;

    data d = {0};
    d.shallow = 0;
    d.w = w;
    d.h = h;

    d.X.rows = 1;
    d.X.vals = calloc(d.X.rows, அளவு(தசம*));
    d.X.cols = h*w*3;

    முழு k = (4+classes)*90;
    d.y = make_matrix(1, k);

    முழு dw = w*jitter;
    முழு dh = h*jitter;

    முழு pleft  = rand_uniform(-dw, dw);
    முழு pright = rand_uniform(-dw, dw);
    முழு ptop   = rand_uniform(-dh, dh);
    முழு pbot   = rand_uniform(-dh, dh);

    முழு swidth =  w - pleft - pright;
    முழு sheight = h - ptop - pbot;

    தசம sx = (தசம)swidth  / w;
    தசம sy = (தசம)sheight / h;

    முழு flip = rand()%2;
    image cropped = crop_image(orig, pleft, ptop, swidth, sheight);

    தசம dx = ((தசம)pleft/w)/sx;
    தசம dy = ((தசம)ptop /h)/sy;

    image sized = resize_image(cropped, w, h);
    எனில்(flip) flip_image(sized);
    d.X.vals[0] = sized.data;

    fill_truth_swag(random_path, d.y.vals[0], classes, flip, dx, dy, 1./sx, 1./sy);

    free_image(orig);
    free_image(cropped);

    திரும்பு d;
}

data load_data_detection(முழு n, எழுத்து **paths, முழு m, முழு w, முழு h, முழு boxes, முழு classes, தசம jitter, தசம hue, தசம saturation, தசம exposure)
{
    எழுத்து **random_paths = get_random_paths(paths, n, m);
    முழு i;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, அளவு(தசம*));
    d.X.cols = h*w*3;

    d.y = make_matrix(n, 5*boxes);
    க்கு(i = 0; i < n; ++i){
        image orig = load_image_color(random_paths[i], 0, 0);
        image sized = make_image(w, h, orig.c);
        fill_image(sized, .5);

        தசம dw = jitter * orig.w;
        தசம dh = jitter * orig.h;

        தசம new_ar = (orig.w + rand_uniform(-dw, dw)) / (orig.h + rand_uniform(-dh, dh));
        //float scale = rand_uniform(.25, 2);
        தசம scale = 1;

        தசம nw, nh;

        எனில்(new_ar < 1){
            nh = scale * h;
            nw = nh * new_ar;
        } இல்லையெனில் {
            nw = scale * w;
            nh = nw / new_ar;
        }

        தசம dx = rand_uniform(0, w - nw);
        தசம dy = rand_uniform(0, h - nh);

        place_image(orig, nw, nh, dx, dy, sized);

        random_distort_image(sized, hue, saturation, exposure);

        முழு flip = rand()%2;
        எனில்(flip) flip_image(sized);
        d.X.vals[i] = sized.data;


        fill_truth_detection(random_paths[i], boxes, d.y.vals[i], classes, flip, -dx/w, -dy/h, nw/w, nh/h);

        free_image(orig);
    }
    விடுவி(random_paths);
    திரும்பு d;
}

வெறுமை *load_thread(வெறுமை *ptr)
{
    //printf("Loading data: %d\n", rand());
    load_args a = *(கட்டமைப்பு load_args*)ptr;
    எனில்(a.exposure == 0) a.exposure = 1;
    எனில்(a.saturation == 0) a.saturation = 1;
    எனில்(a.aspect == 0) a.aspect = 1;

    எனில் (a.type == OLD_CLASSIFICATION_DATA){
        *a.d = load_data_old(a.paths, a.n, a.m, a.labels, a.classes, a.w, a.h);
    } இல்லையெனில் எனில் (a.type == REGRESSION_DATA){
        *a.d = load_data_regression(a.paths, a.n, a.m, a.classes, a.min, a.max, a.size, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    } இல்லையெனில் எனில் (a.type == CLASSIFICATION_DATA){
        *a.d = load_data_augment(a.paths, a.n, a.m, a.labels, a.classes, a.hierarchy, a.min, a.max, a.size, a.angle, a.aspect, a.hue, a.saturation, a.exposure, a.center);
    } இல்லையெனில் எனில் (a.type == SUPER_DATA){
        *a.d = load_data_super(a.paths, a.n, a.m, a.w, a.h, a.scale);
    } இல்லையெனில் எனில் (a.type == WRITING_DATA){
        *a.d = load_data_writing(a.paths, a.n, a.m, a.w, a.h, a.out_w, a.out_h);
    } இல்லையெனில் எனில் (a.type == ISEG_DATA){
        *a.d = load_data_iseg(a.n, a.paths, a.m, a.w, a.h, a.classes, a.இலக்கம்_boxes, a.scale, a.min, a.max, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    } இல்லையெனில் எனில் (a.type == INSTANCE_DATA){
        *a.d = load_data_mask(a.n, a.paths, a.m, a.w, a.h, a.classes, a.இலக்கம்_boxes, a.coords, a.min, a.max, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    } இல்லையெனில் எனில் (a.type == SEGMENTATION_DATA){
        *a.d = load_data_seg(a.n, a.paths, a.m, a.w, a.h, a.classes, a.min, a.max, a.angle, a.aspect, a.hue, a.saturation, a.exposure, a.scale);
    } இல்லையெனில் எனில் (a.type == REGION_DATA){
        *a.d = load_data_region(a.n, a.paths, a.m, a.w, a.h, a.இலக்கம்_boxes, a.classes, a.jitter, a.hue, a.saturation, a.exposure);
    } இல்லையெனில் எனில் (a.type == DETECTION_DATA){
        *a.d = load_data_detection(a.n, a.paths, a.m, a.w, a.h, a.இலக்கம்_boxes, a.classes, a.jitter, a.hue, a.saturation, a.exposure);
    } இல்லையெனில் எனில் (a.type == SWAG_DATA){
        *a.d = load_data_swag(a.paths, a.n, a.classes, a.jitter);
    } இல்லையெனில் எனில் (a.type == COMPARE_DATA){
        *a.d = load_data_compare(a.n, a.paths, a.m, a.classes, a.w, a.h);
    } இல்லையெனில் எனில் (a.type == IMAGE_DATA){
        *(a.im) = load_image_color(a.path, 0, 0);
        *(a.resized) = resize_image(*(a.im), a.w, a.h);
    } இல்லையெனில் எனில் (a.type == LETTERBOX_DATA){
        *(a.im) = load_image_color(a.path, 0, 0);
        *(a.resized) = letterbox_image(*(a.im), a.w, a.h);
    } இல்லையெனில் எனில் (a.type == TAG_DATA){
        *a.d = load_data_tag(a.paths, a.n, a.m, a.classes, a.min, a.max, a.size, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    }
    விடுவி(ptr);
    திரும்பு 0;
}

pthread_t load_data_in_thread(load_args args)
{
    pthread_t thread;
    கட்டமைப்பு load_args *ptr = calloc(1, அளவு(கட்டமைப்பு load_args));
    *ptr = args;
    எனில்(pthread_create(&thread, 0, load_thread, ptr)) error("Thread creation failed");
    திரும்பு thread;
}

வெறுமை *load_threads(வெறுமை *ptr)
{
    முழு i;
    load_args args = *(load_args *)ptr;
    எனில் (args.threads == 0) args.threads = 1;
    data *out = args.d;
    முழு total = args.n;
    விடுவி(ptr);
    data *buffers = calloc(args.threads, அளவு(data));
    pthread_t *threads = calloc(args.threads, அளவு(pthread_t));
    க்கு(i = 0; i < args.threads; ++i){
        args.d = buffers + i;
        args.n = (i+1) * total/args.threads - i * total/args.threads;
        threads[i] = load_data_in_thread(args);
    }
    க்கு(i = 0; i < args.threads; ++i){
        pthread_join(threads[i], 0);
    }
    *out = concat_datas(buffers, args.threads);
    out->shallow = 0;
    க்கு(i = 0; i < args.threads; ++i){
        buffers[i].shallow = 1;
        free_data(buffers[i]);
    }
    விடுவி(buffers);
    விடுவி(threads);
    திரும்பு 0;
}

வெறுமை load_data_blocking(load_args args)
{
    கட்டமைப்பு load_args *ptr = calloc(1, அளவு(கட்டமைப்பு load_args));
    *ptr = args;
    load_thread(ptr);
}

pthread_t load_data(load_args args)
{
    pthread_t thread;
    கட்டமைப்பு load_args *ptr = calloc(1, அளவு(கட்டமைப்பு load_args));
    *ptr = args;
    எனில்(pthread_create(&thread, 0, load_threads, ptr)) error("Thread creation failed");
    திரும்பு thread;
}

data load_data_writing(எழுத்து **paths, முழு n, முழு m, முழு w, முழு h, முழு out_w, முழு out_h)
{
    எனில்(m) paths = get_random_paths(paths, n, m);
    எழுத்து **replace_paths = find_replace_paths(paths, n, ".png", "-label.png");
    data d = {0};
    d.shallow = 0;
    d.X = load_image_paths(paths, n, w, h);
    d.y = load_image_paths_gray(replace_paths, n, out_w, out_h);
    எனில்(m) விடுவி(paths);
    முழு i;
    க்கு(i = 0; i < n; ++i) விடுவி(replace_paths[i]);
    விடுவி(replace_paths);
    திரும்பு d;
}

data load_data_old(எழுத்து **paths, முழு n, முழு m, எழுத்து **labels, முழு k, முழு w, முழு h)
{
    எனில்(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;
    d.X = load_image_paths(paths, n, w, h);
    d.y = load_labels_paths(paths, n, labels, k, 0);
    எனில்(m) விடுவி(paths);
    திரும்பு d;
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

data load_data_super(எழுத்து **paths, முழு n, முழு m, முழு w, முழு h, முழு scale)
{
    எனில்(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;

    முழு i;
    d.X.rows = n;
    d.X.vals = calloc(n, அளவு(தசம*));
    d.X.cols = w*h*3;

    d.y.rows = n;
    d.y.vals = calloc(n, அளவு(தசம*));
    d.y.cols = w*scale * h*scale * 3;

    க்கு(i = 0; i < n; ++i){
        image im = load_image_color(paths[i], 0, 0);
        image crop = random_crop_image(im, w*scale, h*scale);
        முழு flip = rand()%2;
        எனில் (flip) flip_image(crop);
        image resize = resize_image(crop, w, h);
        d.X.vals[i] = resize.data;
        d.y.vals[i] = crop.data;
        free_image(im);
    }

    எனில்(m) விடுவி(paths);
    திரும்பு d;
}

data load_data_regression(எழுத்து **paths, முழு n, முழு m, முழு k, முழு min, முழு max, முழு size, தசம angle, தசம aspect, தசம hue, தசம saturation, தசம exposure)
{
    எனில்(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;
    d.X = load_image_augment_paths(paths, n, min, max, size, angle, aspect, hue, saturation, exposure, 0);
    d.y = load_regression_labels_paths(paths, n, k);
    எனில்(m) விடுவி(paths);
    திரும்பு d;
}

data select_data(data *orig, முழு *inds)
{
    data d = {0};
    d.shallow = 1;
    d.w = orig[0].w;
    d.h = orig[0].h;

    d.X.rows = orig[0].X.rows;
    d.y.rows = orig[0].X.rows;

    d.X.cols = orig[0].X.cols;
    d.y.cols = orig[0].y.cols;

    d.X.vals = calloc(orig[0].X.rows, அளவு(தசம *));
    d.y.vals = calloc(orig[0].y.rows, அளவு(தசம *));
    முழு i;
    க்கு(i = 0; i < d.X.rows; ++i){
        d.X.vals[i] = orig[inds[i]].X.vals[i];
        d.y.vals[i] = orig[inds[i]].y.vals[i];
    }
    திரும்பு d;
}

data *tile_data(data orig, முழு divs, முழு size)
{
    data *ds = calloc(divs*divs, அளவு(data));
    முழு i, j;
#pragma omp parallel க்கு
    க்கு(i = 0; i < divs*divs; ++i){
        data d;
        d.shallow = 0;
        d.w = orig.w/divs * size;
        d.h = orig.h/divs * size;
        d.X.rows = orig.X.rows;
        d.X.cols = d.w*d.h*3;
        d.X.vals = calloc(d.X.rows, அளவு(தசம*));

        d.y = copy_matrix(orig.y);
#pragma omp parallel க்கு
        க்கு(j = 0; j < orig.X.rows; ++j){
            முழு x = (i%divs) * orig.w / divs - (d.w - orig.w/divs)/2;
            முழு y = (i/divs) * orig.h / divs - (d.h - orig.h/divs)/2;
            image im = float_to_image(orig.w, orig.h, 3, orig.X.vals[j]);
            d.X.vals[j] = crop_image(im, x, y, d.w, d.h).data;
        }
        ds[i] = d;
    }
    திரும்பு ds;
}

data resize_data(data orig, முழு w, முழு h)
{
    data d = {0};
    d.shallow = 0;
    d.w = w;
    d.h = h;
    முழு i;
    d.X.rows = orig.X.rows;
    d.X.cols = w*h*3;
    d.X.vals = calloc(d.X.rows, அளவு(தசம*));

    d.y = copy_matrix(orig.y);
#pragma omp parallel க்கு
    க்கு(i = 0; i < orig.X.rows; ++i){
        image im = float_to_image(orig.w, orig.h, 3, orig.X.vals[i]);
        d.X.vals[i] = resize_image(im, w, h).data;
    }
    திரும்பு d;
}

data load_data_augment(எழுத்து **paths, முழு n, முழு m, எழுத்து **labels, முழு k, மரம் *hierarchy, முழு min, முழு max, முழு size, தசம angle, தசம aspect, தசம hue, தசம saturation, தசம exposure, முழு center)
{
    எனில்(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;
    d.w=size;
    d.h=size;
    d.X = load_image_augment_paths(paths, n, min, max, size, angle, aspect, hue, saturation, exposure, center);
    d.y = load_labels_paths(paths, n, labels, k, hierarchy);
    எனில்(m) விடுவி(paths);
    திரும்பு d;
}

data load_data_tag(எழுத்து **paths, முழு n, முழு m, முழு k, முழு min, முழு max, முழு size, தசம angle, தசம aspect, தசம hue, தசம saturation, தசம exposure)
{
    எனில்(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.w = size;
    d.h = size;
    d.shallow = 0;
    d.X = load_image_augment_paths(paths, n, min, max, size, angle, aspect, hue, saturation, exposure, 0);
    d.y = load_tags_paths(paths, n, k);
    எனில்(m) விடுவி(paths);
    திரும்பு d;
}

matrix concat_matrix(matrix m1, matrix m2)
{
    முழு i, மொத்தம் = 0;
    matrix m;
    m.cols = m1.cols;
    m.rows = m1.rows+m2.rows;
    m.vals = calloc(m1.rows + m2.rows, அளவு(தசம*));
    க்கு(i = 0; i < m1.rows; ++i){
        m.vals[மொத்தம்++] = m1.vals[i];
    }
    க்கு(i = 0; i < m2.rows; ++i){
        m.vals[மொத்தம்++] = m2.vals[i];
    }
    திரும்பு m;
}

data concat_data(data d1, data d2)
{
    data d = {0};
    d.shallow = 1;
    d.X = concat_matrix(d1.X, d2.X);
    d.y = concat_matrix(d1.y, d2.y);
    d.w = d1.w;
    d.h = d1.h;
    திரும்பு d;
}

data concat_datas(data *d, முழு n)
{
    முழு i;
    data out = {0};
    க்கு(i = 0; i < n; ++i){
        data new = concat_data(d[i], out);
        free_data(out);
        out = new;
    }
    திரும்பு out;
}

data load_categorical_data_csv(எழுத்து *filename, முழு target, முழு k)
{
    data d = {0};
    d.shallow = 0;
    matrix X = csv_to_matrix(filename);
    தசம *truth_1d = எடு_column(&X, target);
    தசம **truth = one_hot_encode(truth_1d, X.rows, k);
    matrix y;
    y.rows = X.rows;
    y.cols = k;
    y.vals = truth;
    d.X = X;
    d.y = y;
    விடுவி(truth_1d);
    திரும்பு d;
}

data load_cifar10_data(எழுத்து *filename)
{
    data d = {0};
    d.shallow = 0;
    long i,j;
    matrix X = make_matrix(10000, 3072);
    matrix y = make_matrix(10000, 10);
    d.X = X;
    d.y = y;

    FILE *fp = fopen(filename, "rb");
    எனில்(!fp) file_error(filename);
    க்கு(i = 0; i < 10000; ++i){
        unsigned எழுத்து bytes[3073];
        fread(bytes, 1, 3073, fp);
        முழு class = bytes[0];
        y.vals[i][class] = 1;
        க்கு(j = 0; j < X.cols; ++j){
            X.vals[i][j] = (இரட்டை)bytes[j+1];
        }
    }
    scale_data_rows(d, 1./255);
    //normalize_data_rows(d);
    fclose(fp);
    திரும்பு d;
}

வெறுமை get_random_batch(data d, முழு n, தசம *X, தசம *y)
{
    முழு j;
    க்கு(j = 0; j < n; ++j){
        முழு index = rand()%d.X.rows;
        memcpy(X+j*d.X.cols, d.X.vals[index], d.X.cols*அளவு(தசம));
        memcpy(y+j*d.y.cols, d.y.vals[index], d.y.cols*அளவு(தசம));
    }
}

வெறுமை get_next_batch(data d, முழு n, முழு offset, தசம *X, தசம *y)
{
    முழு j;
    க்கு(j = 0; j < n; ++j){
        முழு index = offset + j;
        memcpy(X+j*d.X.cols, d.X.vals[index], d.X.cols*அளவு(தசம));
        எனில்(y) memcpy(y+j*d.y.cols, d.y.vals[index], d.y.cols*அளவு(தசம));
    }
}

வெறுமை smooth_data(data d)
{
    முழு i, j;
    தசம scale = 1. / d.y.cols;
    தசம eps = .1;
    க்கு(i = 0; i < d.y.rows; ++i){
        க்கு(j = 0; j < d.y.cols; ++j){
            d.y.vals[i][j] = eps * scale + (1-eps) * d.y.vals[i][j];
        }
    }
}

data load_all_cifar10()
{
    data d = {0};
    d.shallow = 0;
    முழு i,j,b;
    matrix X = make_matrix(50000, 3072);
    matrix y = make_matrix(50000, 10);
    d.X = X;
    d.y = y;


    க்கு(b = 0; b < 5; ++b){
        எழுத்து buff[256];
        sprintf(buff, "data/cifar/cifar-10-batches-bin/data_batch_%d.bin", b+1);
        FILE *fp = fopen(buff, "rb");
        எனில்(!fp) file_error(buff);
        க்கு(i = 0; i < 10000; ++i){
            unsigned எழுத்து bytes[3073];
            fread(bytes, 1, 3073, fp);
            முழு class = bytes[0];
            y.vals[i+b*10000][class] = 1;
            க்கு(j = 0; j < X.cols; ++j){
                X.vals[i+b*10000][j] = (இரட்டை)bytes[j+1];
            }
        }
        fclose(fp);
    }
    //normalize_data_rows(d);
    scale_data_rows(d, 1./255);
    smooth_data(d);
    திரும்பு d;
}

data load_go(எழுத்து *filename)
{
    FILE *fp = fopen(filename, "rb");
    matrix X = make_matrix(3363059, 361);
    matrix y = make_matrix(3363059, 361);
    முழு row, col;

    எனில்(!fp) file_error(filename);
    எழுத்து *label;
    முழு மொத்தம் = 0;
    போது((label = fgetl(fp))){
        முழு i;
        எனில்(மொத்தம் == X.rows){
            X = resize_matrix(X, மொத்தம்*2);
            y = resize_matrix(y, மொத்தம்*2);
        }
        sscanf(label, "%d %d", &row, &col);
        எழுத்து *board = fgetl(fp);

        முழு index = row*19 + col;
        y.vals[மொத்தம்][index] = 1;

        க்கு(i = 0; i < 19*19; ++i){
            தசம val = 0;
            எனில்(board[i] == '1') val = 1;
            இல்லையெனில் எனில்(board[i] == '2') val = -1;
            X.vals[மொத்தம்][i] = val;
        }
        ++மொத்தம்;
        விடுவி(label);
        விடுவி(board);
    }
    X = resize_matrix(X, மொத்தம்);
    y = resize_matrix(y, மொத்தம்);

    data d = {0};
    d.shallow = 0;
    d.X = X;
    d.y = y;


    fclose(fp);

    திரும்பு d;
}


வெறுமை randomize_data(data d)
{
    முழு i;
    க்கு(i = d.X.rows-1; i > 0; --i){
        முழு index = rand()%i;
        தசம *swap = d.X.vals[index];
        d.X.vals[index] = d.X.vals[i];
        d.X.vals[i] = swap;

        swap = d.y.vals[index];
        d.y.vals[index] = d.y.vals[i];
        d.y.vals[i] = swap;
    }
}

வெறுமை scale_data_rows(data d, தசம s)
{
    முழு i;
    க்கு(i = 0; i < d.X.rows; ++i){
        scale_array(d.X.vals[i], d.X.cols, s);
    }
}

வெறுமை translate_data_rows(data d, தசம s)
{
    முழு i;
    க்கு(i = 0; i < d.X.rows; ++i){
        translate_array(d.X.vals[i], d.X.cols, s);
    }
}

data copy_data(data d)
{
    data c = {0};
    c.w = d.w;
    c.h = d.h;
    c.shallow = 0;
    c.இலக்கம்_boxes = d.இலக்கம்_boxes;
    c.boxes = d.boxes;
    c.X = copy_matrix(d.X);
    c.y = copy_matrix(d.y);
    திரும்பு c;
}

வெறுமை normalize_data_rows(data d)
{
    முழு i;
    க்கு(i = 0; i < d.X.rows; ++i){
        normalize_array(d.X.vals[i], d.X.cols);
    }
}

data get_data_part(data d, முழு part, முழு total)
{
    data p = {0};
    p.shallow = 1;
    p.X.rows = d.X.rows * (part + 1) / total - d.X.rows * part / total;
    p.y.rows = d.y.rows * (part + 1) / total - d.y.rows * part / total;
    p.X.cols = d.X.cols;
    p.y.cols = d.y.cols;
    p.X.vals = d.X.vals + d.X.rows * part / total;
    p.y.vals = d.y.vals + d.y.rows * part / total;
    திரும்பு p;
}

data get_random_data(data d, முழு இலக்கம்)
{
    data r = {0};
    r.shallow = 1;

    r.X.rows = இலக்கம்;
    r.y.rows = இலக்கம்;

    r.X.cols = d.X.cols;
    r.y.cols = d.y.cols;

    r.X.vals = calloc(இலக்கம், அளவு(தசம *));
    r.y.vals = calloc(இலக்கம், அளவு(தசம *));

    முழு i;
    க்கு(i = 0; i < இலக்கம்; ++i){
        முழு index = rand()%d.X.rows;
        r.X.vals[i] = d.X.vals[index];
        r.y.vals[i] = d.y.vals[index];
    }
    திரும்பு r;
}

data *split_data(data d, முழு part, முழு total)
{
    data *split = calloc(2, அளவு(data));
    முழு i;
    முழு start = part*d.X.rows/total;
    முழு end = (part+1)*d.X.rows/total;
    data train;
    data test;
    train.shallow = test.shallow = 1;

    test.X.rows = test.y.rows = end-start;
    train.X.rows = train.y.rows = d.X.rows - (end-start);
    train.X.cols = test.X.cols = d.X.cols;
    train.y.cols = test.y.cols = d.y.cols;

    train.X.vals = calloc(train.X.rows, அளவு(தசம*));
    test.X.vals = calloc(test.X.rows, அளவு(தசம*));
    train.y.vals = calloc(train.y.rows, அளவு(தசம*));
    test.y.vals = calloc(test.y.rows, அளவு(தசம*));

    க்கு(i = 0; i < start; ++i){
        train.X.vals[i] = d.X.vals[i];
        train.y.vals[i] = d.y.vals[i];
    }
    க்கு(i = start; i < end; ++i){
        test.X.vals[i-start] = d.X.vals[i];
        test.y.vals[i-start] = d.y.vals[i];
    }
    க்கு(i = end; i < d.X.rows; ++i){
        train.X.vals[i-(end-start)] = d.X.vals[i];
        train.y.vals[i-(end-start)] = d.y.vals[i];
    }
    split[0] = train;
    split[1] = test;
    திரும்பு split;
}

