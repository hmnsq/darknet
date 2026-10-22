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
    حرف *path;
    FILE *file = fopen(filename, "r");
    إذا(!file) file_error(filename);
    list *ليوك = make_list();
    طالما((path=fgetl(file))){
        list_سك(ليوك, path);
    }
    fclose(file);
    أرجع ليوك;
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
    حرف **random_paths = calloc(n, حجم(حرف*));
    صحيح i;
    pthread_mutex_lock(&mutex);
    لكل(i = 0; i < n; ++i){
        صحيح index = rand()%m;
        random_paths[i] = paths[index];
        //if(i == 0) printf("%s\n", paths[index]);
    }
    pthread_mutex_unlock(&mutex);
    أرجع random_paths;
}

حرف **find_replace_paths(حرف **paths, صحيح n, حرف *find, حرف *replace)
{
    حرف **replace_paths = calloc(n, حجم(حرف*));
    صحيح i;
    لكل(i = 0; i < n; ++i){
        حرف replaced[4096];
        find_replace(paths[i], find, replace, replaced);
        replace_paths[i] = copy_string(replaced);
    }
    أرجع replace_paths;
}

matrix load_image_paths_gray(حرف **paths, صحيح n, صحيح w, صحيح h)
{
    صحيح i;
    matrix X;
    X.rows = n;
    X.vals = calloc(X.rows, حجم(عائم*));
    X.cols = 0;

    لكل(i = 0; i < n; ++i){
        image im = load_image(paths[i], w, h, 3);

        image gray = grayscale_image(im);
        free_image(im);
        im = gray;

        X.vals[i] = im.data;
        X.cols = im.h*im.w*im.c;
    }
    أرجع X;
}

matrix load_image_paths(حرف **paths, صحيح n, صحيح w, صحيح h)
{
    صحيح i;
    matrix X;
    X.rows = n;
    X.vals = calloc(X.rows, حجم(عائم*));
    X.cols = 0;

    لكل(i = 0; i < n; ++i){
        image im = load_image_color(paths[i], w, h);
        X.vals[i] = im.data;
        X.cols = im.h*im.w*im.c;
    }
    أرجع X;
}

matrix load_image_augment_paths(حرف **paths, صحيح n, صحيح min, صحيح max, صحيح size, عائم angle, عائم aspect, عائم hue, عائم saturation, عائم exposure, صحيح center)
{
    صحيح i;
    matrix X;
    X.rows = n;
    X.vals = calloc(X.rows, حجم(عائم*));
    X.cols = 0;

    لكل(i = 0; i < n; ++i){
        image im = load_image_color(paths[i], 0, 0);
        image crop;
        إذا(center){
            crop = center_crop_image(im, size, size);
        } وإلا {
            crop = random_augment_image(im, angle, aspect, min, max, size, size);
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
        X.vals[i] = crop.data;
        X.cols = crop.h*crop.w*crop.c;
    }
    أرجع X;
}


box_label *read_boxes(حرف *filename, صحيح *n)
{
    FILE *file = fopen(filename, "r");
    إذا(!file) file_error(filename);
    عائم x, y, h, w;
    صحيح id;
    صحيح كدي = 0;
    صحيح size = 64;
    box_label *boxes = calloc(size, حجم(box_label));
    طالما(fscanf(file, "%d %f %f %f %f", &id, &x, &y, &w, &h) == 5){
        إذا(كدي == size) {
            size = size * 2;
            boxes = realloc(boxes, size*حجم(box_label));
        }
        boxes[كدي].id = id;
        boxes[كدي].x = x;
        boxes[كدي].y = y;
        boxes[كدي].h = h;
        boxes[كدي].w = w;
        boxes[كدي].هغو   = x - w/2;
        boxes[كدي].دام  = x + w/2;
        boxes[كدي].كولي    = y - h/2;
        boxes[كدي].bottom = y + h/2;
        ++كدي;
    }
    fclose(file);
    *n = كدي;
    أرجع boxes;
}

فراغ randomize_boxes(box_label *b, صحيح n)
{
    صحيح i;
    لكل(i = 0; i < n; ++i){
        box_label swap = b[i];
        صحيح index = rand()%n;
        b[i] = b[index];
        b[index] = swap;
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
        boxes[i].هغو   = boxes[i].هغو  * sx - dx;
        boxes[i].دام  = boxes[i].دام * sx - dx;
        boxes[i].كولي    = boxes[i].كولي   * sy - dy;
        boxes[i].bottom = boxes[i].bottom* sy - dy;

        إذا(flip){
            عائم swap = boxes[i].هغو;
            boxes[i].هغو = 1. - boxes[i].دام;
            boxes[i].دام = 1. - swap;
        }

        boxes[i].هغو =  constrain(0, 1, boxes[i].هغو);
        boxes[i].دام = constrain(0, 1, boxes[i].دام);
        boxes[i].كولي =   constrain(0, 1, boxes[i].كولي);
        boxes[i].bottom =   constrain(0, 1, boxes[i].bottom);

        boxes[i].x = (boxes[i].هغو+boxes[i].دام)/2;
        boxes[i].y = (boxes[i].كولي+boxes[i].bottom)/2;
        boxes[i].w = (boxes[i].دام - boxes[i].هغو);
        boxes[i].h = (boxes[i].bottom - boxes[i].كولي);

        boxes[i].w = constrain(0, 1, boxes[i].w);
        boxes[i].h = constrain(0, 1, boxes[i].h);
    }
}

فراغ fill_truth_swag(حرف *path, عائم *truth, صحيح classes, صحيح flip, عائم dx, عائم dy, عائم sx, عائم sy)
{
    حرف labelpath[4096];
    find_replace(path, "images", "labels", labelpath);
    find_replace(labelpath, "JPEGImages", "labels", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);

    صحيح كدي = 0;
    box_label *boxes = read_boxes(labelpath, &كدي);
    randomize_boxes(boxes, كدي);
    correct_boxes(boxes, كدي, dx, dy, sx, sy, flip);
    عائم x,y,w,h;
    صحيح id;
    صحيح i;

    لكل (i = 0; i < كدي && i < 90; ++i) {
        x =  boxes[i].x;
        y =  boxes[i].y;
        w =  boxes[i].w;
        h =  boxes[i].h;
        id = boxes[i].id;

        إذا (w < .0 || h < .0) استمر;

        صحيح index = (4+classes) * i;

        truth[index++] = x;
        truth[index++] = y;
        truth[index++] = w;
        truth[index++] = h;

        إذا (id < classes) truth[index+id] = 1;
    }
    حرر(boxes);
}

فراغ fill_truth_region(حرف *path, عائم *truth, صحيح classes, صحيح لمب_boxes, صحيح flip, عائم dx, عائم dy, عائم sx, عائم sy)
{
    حرف labelpath[4096];
    find_replace(path, "images", "labels", labelpath);
    find_replace(labelpath, "JPEGImages", "labels", labelpath);

    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".png", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    صحيح كدي = 0;
    box_label *boxes = read_boxes(labelpath, &كدي);
    randomize_boxes(boxes, كدي);
    correct_boxes(boxes, كدي, dx, dy, sx, sy, flip);
    عائم x,y,w,h;
    صحيح id;
    صحيح i;

    لكل (i = 0; i < كدي; ++i) {
        x =  boxes[i].x;
        y =  boxes[i].y;
        w =  boxes[i].w;
        h =  boxes[i].h;
        id = boxes[i].id;

        إذا (w < .005 || h < .005) استمر;

        صحيح col = (صحيح)(x*لمب_boxes);
        صحيح row = (صحيح)(y*لمب_boxes);

        x = x*لمب_boxes - col;
        y = y*لمب_boxes - row;

        صحيح index = (col+row*لمب_boxes)*(5+classes);
        إذا (truth[index]) استمر;
        truth[index++] = 1;

        إذا (id < classes) truth[index+id] = 1;
        index += classes;

        truth[index++] = x;
        truth[index++] = y;
        truth[index++] = w;
        truth[index++] = h;
    }
    حرر(boxes);
}

فراغ load_rle(image im, صحيح *rle, صحيح n)
{
    صحيح كدي = 0;
    صحيح curr = 0;
    صحيح i,j;
    لكل(i = 0; i < n; ++i){
        لكل(j = 0; j < rle[i]; ++j){
            im.data[كدي++] = curr;
        }
        curr = 1 - curr;
    }
    لكل(; كدي < im.h*im.w*im.c; ++كدي){
        im.data[كدي] = curr;
    }
}

فراغ or_image(image src, image dest, صحيح c)
{
    صحيح i;
    لكل(i = 0; i < src.w*src.h; ++i){
        إذا(src.data[i]) dest.data[dest.w*dest.h*c + i] = 1;
    }
}

فراغ exclusive_image(image src)
{
    صحيح k, j, i;
    صحيح s = src.w*src.h;
    لكل(k = 0; k < src.c-1; ++k){
        لكل(i = 0; i < s; ++i){
            إذا (src.data[k*s + i]){
                لكل(j = k+1; j < src.c; ++j){
                    src.data[j*s + i] = 0;
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
            إذا(im.data[y*im.w + x]){
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

فراغ fill_truth_iseg(حرف *path, صحيح لمب_boxes, عائم *truth, صحيح classes, صحيح w, صحيح h, augment_حجوجي aug, صحيح flip, صحيح mw, صحيح mh)
{
    حرف labelpath[4096];
    find_replace(path, "images", "mask", labelpath);
    find_replace(labelpath, "JPEGImages", "mask", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    FILE *file = fopen(labelpath, "r");
    إذا(!file) file_error(labelpath);
    حرف buff[32788];
    صحيح id;
    صحيح i = 0;
    صحيح j;
    image part = make_image(w, h, 1);
    طالما((fscanf(file, "%d %s", &id, buff) == 2) && i < لمب_boxes){
        صحيح n = 0;
        صحيح *rle = read_intlist(buff, &n, 0);
        load_rle(part, rle, n);
        image sized = rotate_crop_image(part, aug.rad, aug.scale, aug.w, aug.h, aug.dx, aug.dy, aug.aspect);
        إذا(flip) flip_image(sized);

        image mask = resize_image(sized, mw, mh);
        truth[i*(mw*mh+1)] = id;
        لكل(j = 0; j < mw*mh; ++j){
            truth[i*(mw*mh + 1) + 1 + j] = mask.data[j];
        }
        ++i;

        free_image(mask);
        free_image(sized);
        حرر(rle);
    }
    إذا(i < لمب_boxes) truth[i*(mw*mh+1)] = -1;
    fclose(file);
    free_image(part);
}

فراغ fill_truth_mask(حرف *path, صحيح لمب_boxes, عائم *truth, صحيح classes, صحيح w, صحيح h, augment_حجوجي aug, صحيح flip, صحيح mw, صحيح mh)
{
    حرف labelpath[4096];
    find_replace(path, "images", "mask", labelpath);
    find_replace(labelpath, "JPEGImages", "mask", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    FILE *file = fopen(labelpath, "r");
    إذا(!file) file_error(labelpath);
    حرف buff[32788];
    صحيح id;
    صحيح i = 0;
    image part = make_image(w, h, 1);
    طالما((fscanf(file, "%d %s", &id, buff) == 2) && i < لمب_boxes){
        صحيح n = 0;
        صحيح *rle = read_intlist(buff, &n, 0);
        load_rle(part, rle, n);
        image sized = rotate_crop_image(part, aug.rad, aug.scale, aug.w, aug.h, aug.dx, aug.dy, aug.aspect);
        إذا(flip) flip_image(sized);
        box b = bound_image(sized);
        إذا(b.w > 0){
            image crop = crop_image(sized, b.x, b.y, b.w, b.h);
            image mask = resize_image(crop, mw, mh);
            truth[i*(4 + mw*mh + 1) + 0] = (b.x + b.w/2.)/sized.w;
            truth[i*(4 + mw*mh + 1) + 1] = (b.y + b.h/2.)/sized.h;
            truth[i*(4 + mw*mh + 1) + 2] = b.w/sized.w;
            truth[i*(4 + mw*mh + 1) + 3] = b.h/sized.h;
            صحيح j;
            لكل(j = 0; j < mw*mh; ++j){
                truth[i*(4 + mw*mh + 1) + 4 + j] = mask.data[j];
            }
            truth[i*(4 + mw*mh + 1) + 4 + mw*mh] = id;
            free_image(crop);
            free_image(mask);
            ++i;
        }
        free_image(sized);
        حرر(rle);
    }
    fclose(file);
    free_image(part);
}


فراغ fill_truth_detection(حرف *path, صحيح لمب_boxes, عائم *truth, صحيح classes, صحيح flip, عائم dx, عائم dy, عائم sx, عائم sy)
{
    حرف labelpath[4096];
    find_replace(path, "images", "labels", labelpath);
    find_replace(labelpath, "JPEGImages", "labels", labelpath);

    find_replace(labelpath, "raw", "labels", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".png", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    صحيح كدي = 0;
    box_label *boxes = read_boxes(labelpath, &كدي);
    randomize_boxes(boxes, كدي);
    correct_boxes(boxes, كدي, dx, dy, sx, sy, flip);
    إذا(كدي > لمب_boxes) كدي = لمب_boxes;
    عائم x,y,w,h;
    صحيح id;
    صحيح i;
    صحيح sub = 0;

    لكل (i = 0; i < كدي; ++i) {
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

#define NUMCHARS 37

فراغ بغ_letters(عائم *pred, صحيح n)
{
    صحيح i;
    لكل(i = 0; i < n; ++i){
        صحيح index = max_index(pred+i*NUMCHARS, NUMCHARS);
        printf("%c", int_to_alphanum(index));
    }
    printf("\n");
}

فراغ fill_truth_captcha(حرف *path, صحيح n, عائم *truth)
{
    حرف *begin = strrchr(path, '/');
    ++begin;
    صحيح i;
    لكل(i = 0; i < strlen(begin) && i < n && begin[i] != '.'; ++i){
        صحيح index = alphanum_to_int(begin[i]);
        إذا(index > 35) printf("Bad %c\n", begin[i]);
        truth[i*NUMCHARS+index] = 1;
    }
    لكل(;i < n; ++i){
        truth[i*NUMCHARS + NUMCHARS-1] = 1;
    }
}

data load_data_captcha(حرف **paths, صحيح n, صحيح m, صحيح k, صحيح w, صحيح h)
{
    إذا(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;
    d.X = load_image_paths(paths, n, w, h);
    d.y = make_matrix(n, k*NUMCHARS);
    صحيح i;
    لكل(i = 0; i < n; ++i){
        fill_truth_captcha(paths[i], k, d.y.vals[i]);
    }
    إذا(m) حرر(paths);
    أرجع d;
}

data load_data_captcha_encode(حرف **paths, صحيح n, صحيح m, صحيح w, صحيح h)
{
    إذا(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;
    d.X = load_image_paths(paths, n, w, h);
    d.X.cols = 17100;
    d.y = d.X;
    إذا(m) حرر(paths);
    أرجع d;
}

فراغ fill_truth(حرف *path, حرف **labels, صحيح k, عائم *truth)
{
    صحيح i;
    memset(truth, 0, k*حجم(عائم));
    صحيح كدي = 0;
    لكل(i = 0; i < k; ++i){
        إذا(strstr(path, labels[i])){
            truth[i] = 1;
            ++كدي;
            //printf("%s %s %d\n", path, labels[i], i);
        }
    }
    إذا(كدي != 1 && (k != 1 || كدي != 0)) printf("Too many or too few labels: %d, %s\n", كدي, path);
}

فراغ fill_hierarchy(عائم *truth, صحيح k, بشيا *hierarchy)
{
    صحيح j;
    لكل(j = 0; j < k; ++j){
        إذا(truth[j]){
            صحيح parent = hierarchy->parent[j];
            طالما(parent >= 0){
                truth[parent] = 1;
                parent = hierarchy->parent[parent];
            }
        }
    }
    صحيح i;
    صحيح كدي = 0;
    لكل(j = 0; j < hierarchy->groups; ++j){
        //printf("%d\n", count);
        صحيح mask = 1;
        لكل(i = 0; i < hierarchy->group_size[j]; ++i){
            إذا(truth[كدي + i]){
                mask = 0;
                اكسر;
            }
        }
        إذا (mask) {
            لكل(i = 0; i < hierarchy->group_size[j]; ++i){
                truth[كدي + i] = SECRET_لمب;
            }
        }
        كدي += hierarchy->group_size[j];
    }
}

matrix load_regression_labels_paths(حرف **paths, صحيح n, صحيح k)
{
    matrix y = make_matrix(n, k);
    صحيح i,j;
    لكل(i = 0; i < n; ++i){
        حرف labelpath[4096];
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
        لكل(j = 0; j < k; ++j){
            fscanf(file, "%f", &(y.vals[i][j]));
        }
        fclose(file);
    }
    أرجع y;
}

matrix load_labels_paths(حرف **paths, صحيح n, حرف **labels, صحيح k, بشيا *hierarchy)
{
    matrix y = make_matrix(n, k);
    صحيح i;
    لكل(i = 0; i < n && labels; ++i){
        fill_truth(paths[i], labels, k, y.vals[i]);
        إذا(hierarchy){
            fill_hierarchy(y.vals[i], k, hierarchy);
        }
    }
    أرجع y;
}

matrix load_tags_paths(حرف **paths, صحيح n, صحيح k)
{
    matrix y = make_matrix(n, k);
    صحيح i;
    //int count = 0;
    لكل(i = 0; i < n; ++i){
        حرف label[4096];
        find_replace(paths[i], "images", "labels", label);
        find_replace(label, ".jpg", ".txt", label);
        FILE *file = fopen(label, "r");
        إذا (!file) استمر;
        //++count;
        صحيح tag;
        طالما(fscanf(file, "%d", &tag) == 1){
            إذا(tag < k){
                y.vals[i][tag] = 1;
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
    حرف **labels = (حرف **)list_to_array(plist);
    free_list(plist);
    أرجع labels;
}

فراغ free_data(data d)
{
    إذا(!d.shallow){
        free_matrix(d.X);
        free_matrix(d.y);
    }وإلا{
        حرر(d.X.vals);
        حرر(d.y.vals);
    }
}

image get_segmentation_image(حرف *path, صحيح w, صحيح h, صحيح classes)
{
    حرف labelpath[4096];
    find_replace(path, "images", "mask", labelpath);
    find_replace(labelpath, "JPEGImages", "mask", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    image mask = make_image(w, h, classes);
    FILE *file = fopen(labelpath, "r");
    إذا(!file) file_error(labelpath);
    حرف buff[32788];
    صحيح id;
    image part = make_image(w, h, 1);
    طالما(fscanf(file, "%d %s", &id, buff) == 2){
        صحيح n = 0;
        صحيح *rle = read_intlist(buff, &n, 0);
        load_rle(part, rle, n);
        or_image(part, mask, id);
        حرر(rle);
    }
    //exclusive_image(mask);
    fclose(file);
    free_image(part);
    أرجع mask;
}

image get_segmentation_image2(حرف *path, صحيح w, صحيح h, صحيح classes)
{
    حرف labelpath[4096];
    find_replace(path, "images", "mask", labelpath);
    find_replace(labelpath, "JPEGImages", "mask", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    image mask = make_image(w, h, classes+1);
    صحيح i;
    لكل(i = 0; i < w*h; ++i){
        mask.data[w*h*classes + i] = 1;
    }
    FILE *file = fopen(labelpath, "r");
    إذا(!file) file_error(labelpath);
    حرف buff[32788];
    صحيح id;
    image part = make_image(w, h, 1);
    طالما(fscanf(file, "%d %s", &id, buff) == 2){
        صحيح n = 0;
        صحيح *rle = read_intlist(buff, &n, 0);
        load_rle(part, rle, n);
        or_image(part, mask, id);
        لكل(i = 0; i < w*h; ++i){
            إذا(part.data[i]) mask.data[w*h*classes + i] = 0;
        }
        حرر(rle);
    }
    //exclusive_image(mask);
    fclose(file);
    free_image(part);
    أرجع mask;
}

data load_data_seg(صحيح n, حرف **paths, صحيح m, صحيح w, صحيح h, صحيح classes, صحيح min, صحيح max, عائم angle, عائم aspect, عائم hue, عائم saturation, عائم exposure, صحيح div)
{
    حرف **random_paths = get_random_paths(paths, n, m);
    صحيح i;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, حجم(عائم*));
    d.X.cols = h*w*3;


    d.y.rows = n;
    d.y.cols = h*w*classes/div/div;
    d.y.vals = calloc(d.X.rows, حجم(عائم*));

    لكل(i = 0; i < n; ++i){
        image orig = load_image_color(random_paths[i], 0, 0);
        augment_حجوجي a = random_augment_حجوجي(orig, angle, aspect, min, max, w, h);
        image sized = rotate_crop_image(orig, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);

        صحيح flip = rand()%2;
        إذا(flip) flip_image(sized);
        random_distort_image(sized, hue, saturation, exposure);
        d.X.vals[i] = sized.data;

        image mask = get_segmentation_image(random_paths[i], orig.w, orig.h, classes);
        //image mask = make_image(orig.w, orig.h, classes+1);
        image sized_m = rotate_crop_image(mask, a.rad, a.scale/div, a.w/div, a.h/div, a.dx/div, a.dy/div, a.aspect);

        إذا(flip) flip_image(sized_m);
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
    حرر(random_paths);
    أرجع d;
}

data load_data_iseg(صحيح n, حرف **paths, صحيح m, صحيح w, صحيح h, صحيح classes, صحيح boxes, صحيح div, صحيح min, صحيح max, عائم angle, عائم aspect, عائم hue, عائم saturation, عائم exposure)
{
    حرف **random_paths = get_random_paths(paths, n, m);
    صحيح i;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, حجم(عائم*));
    d.X.cols = h*w*3;

    d.y = make_matrix(n, (((w/div)*(h/div))+1)*boxes);

    لكل(i = 0; i < n; ++i){
        image orig = load_image_color(random_paths[i], 0, 0);
        augment_حجوجي a = random_augment_حجوجي(orig, angle, aspect, min, max, w, h);
        image sized = rotate_crop_image(orig, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);

        صحيح flip = rand()%2;
        إذا(flip) flip_image(sized);
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
    حرر(random_paths);
    أرجع d;
}

data load_data_mask(صحيح n, حرف **paths, صحيح m, صحيح w, صحيح h, صحيح classes, صحيح boxes, صحيح coords, صحيح min, صحيح max, عائم angle, عائم aspect, عائم hue, عائم saturation, عائم exposure)
{
    حرف **random_paths = get_random_paths(paths, n, m);
    صحيح i;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, حجم(عائم*));
    d.X.cols = h*w*3;

    d.y = make_matrix(n, (coords+1)*boxes);

    لكل(i = 0; i < n; ++i){
        image orig = load_image_color(random_paths[i], 0, 0);
        augment_حجوجي a = random_augment_حجوجي(orig, angle, aspect, min, max, w, h);
        image sized = rotate_crop_image(orig, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);

        صحيح flip = rand()%2;
        إذا(flip) flip_image(sized);
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
    حرر(random_paths);
    أرجع d;
}

data load_data_region(صحيح n, حرف **paths, صحيح m, صحيح w, صحيح h, صحيح size, صحيح classes, عائم jitter, عائم hue, عائم saturation, عائم exposure)
{
    حرف **random_paths = get_random_paths(paths, n, m);
    صحيح i;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, حجم(عائم*));
    d.X.cols = h*w*3;


    صحيح k = size*size*(5+classes);
    d.y = make_matrix(n, k);
    لكل(i = 0; i < n; ++i){
        image orig = load_image_color(random_paths[i], 0, 0);

        صحيح oh = orig.h;
        صحيح ow = orig.w;

        صحيح dw = (ow*jitter);
        صحيح dh = (oh*jitter);

        صحيح pleft  = rand_uniform(-dw, dw);
        صحيح pright = rand_uniform(-dw, dw);
        صحيح ptop   = rand_uniform(-dh, dh);
        صحيح pbot   = rand_uniform(-dh, dh);

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
        d.X.vals[i] = sized.data;

        fill_truth_region(random_paths[i], d.y.vals[i], classes, size, flip, dx, dy, 1./sx, 1./sy);

        free_image(orig);
        free_image(cropped);
    }
    حرر(random_paths);
    أرجع d;
}

data load_data_compare(صحيح n, حرف **paths, صحيح m, صحيح classes, صحيح w, صحيح h)
{
    إذا(m) paths = get_random_paths(paths, 2*n, m);
    صحيح i,j;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, حجم(عائم*));
    d.X.cols = h*w*6;

    صحيح k = 2*(classes);
    d.y = make_matrix(n, k);
    لكل(i = 0; i < n; ++i){
        image im1 = load_image_color(paths[i*2],   w, h);
        image im2 = load_image_color(paths[i*2+1], w, h);

        d.X.vals[i] = calloc(d.X.cols, حجم(عائم));
        memcpy(d.X.vals[i],         im1.data, h*w*3*حجم(عائم));
        memcpy(d.X.vals[i] + h*w*3, im2.data, h*w*3*حجم(عائم));

        صحيح id;
        عائم iou;

        حرف imlabel1[4096];
        حرف imlabel2[4096];
        find_replace(paths[i*2],   "imgs", "labels", imlabel1);
        find_replace(imlabel1, "jpg", "txt", imlabel1);
        FILE *fp1 = fopen(imlabel1, "r");

        طالما(fscanf(fp1, "%d %f", &id, &iou) == 2){
            إذا (d.y.vals[i][2*id] < iou) d.y.vals[i][2*id] = iou;
        }

        find_replace(paths[i*2+1], "imgs", "labels", imlabel2);
        find_replace(imlabel2, "jpg", "txt", imlabel2);
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
                d.y.vals[i][2*j]   = SECRET_لمب;
                d.y.vals[i][2*j+1] = SECRET_لمب;
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

data load_data_swag(حرف **paths, صحيح n, صحيح classes, عائم jitter)
{
    صحيح index = rand()%n;
    حرف *random_path = paths[index];

    image orig = load_image_color(random_path, 0, 0);
    صحيح h = orig.h;
    صحيح w = orig.w;

    data d = {0};
    d.shallow = 0;
    d.w = w;
    d.h = h;

    d.X.rows = 1;
    d.X.vals = calloc(d.X.rows, حجم(عائم*));
    d.X.cols = h*w*3;

    صحيح k = (4+classes)*90;
    d.y = make_matrix(1, k);

    صحيح dw = w*jitter;
    صحيح dh = h*jitter;

    صحيح pleft  = rand_uniform(-dw, dw);
    صحيح pright = rand_uniform(-dw, dw);
    صحيح ptop   = rand_uniform(-dh, dh);
    صحيح pbot   = rand_uniform(-dh, dh);

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
    d.X.vals[0] = sized.data;

    fill_truth_swag(random_path, d.y.vals[0], classes, flip, dx, dy, 1./sx, 1./sy);

    free_image(orig);
    free_image(cropped);

    أرجع d;
}

data load_data_detection(صحيح n, حرف **paths, صحيح m, صحيح w, صحيح h, صحيح boxes, صحيح classes, عائم jitter, عائم hue, عائم saturation, عائم exposure)
{
    حرف **random_paths = get_random_paths(paths, n, m);
    صحيح i;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, حجم(عائم*));
    d.X.cols = h*w*3;

    d.y = make_matrix(n, 5*boxes);
    لكل(i = 0; i < n; ++i){
        image orig = load_image_color(random_paths[i], 0, 0);
        image sized = make_image(w, h, orig.c);
        fill_image(sized, .5);

        عائم dw = jitter * orig.w;
        عائم dh = jitter * orig.h;

        عائم new_ar = (orig.w + rand_uniform(-dw, dw)) / (orig.h + rand_uniform(-dh, dh));
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

        عائم dx = rand_uniform(0, w - nw);
        عائم dy = rand_uniform(0, h - nh);

        place_image(orig, nw, nh, dx, dy, sized);

        random_distort_image(sized, hue, saturation, exposure);

        صحيح flip = rand()%2;
        إذا(flip) flip_image(sized);
        d.X.vals[i] = sized.data;


        fill_truth_detection(random_paths[i], boxes, d.y.vals[i], classes, flip, -dx/w, -dy/h, nw/w, nh/h);

        free_image(orig);
    }
    حرر(random_paths);
    أرجع d;
}

فراغ *load_thread(فراغ *ptr)
{
    //printf("Loading data: %d\n", rand());
    load_حجوجي a = *(بنية load_حجوجي*)ptr;
    إذا(a.exposure == 0) a.exposure = 1;
    إذا(a.saturation == 0) a.saturation = 1;
    إذا(a.aspect == 0) a.aspect = 1;

    إذا (a.type == OLD_CLASSIFICATION_DATA){
        *a.d = load_data_old(a.paths, a.n, a.m, a.labels, a.classes, a.w, a.h);
    } وإلا إذا (a.type == REGRESSION_DATA){
        *a.d = load_data_regression(a.paths, a.n, a.m, a.classes, a.min, a.max, a.size, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    } وإلا إذا (a.type == CLASSIFICATION_DATA){
        *a.d = load_data_augment(a.paths, a.n, a.m, a.labels, a.classes, a.hierarchy, a.min, a.max, a.size, a.angle, a.aspect, a.hue, a.saturation, a.exposure, a.center);
    } وإلا إذا (a.type == SUPER_DATA){
        *a.d = load_data_super(a.paths, a.n, a.m, a.w, a.h, a.scale);
    } وإلا إذا (a.type == WRITING_DATA){
        *a.d = load_data_writing(a.paths, a.n, a.m, a.w, a.h, a.out_w, a.out_h);
    } وإلا إذا (a.type == ISEG_DATA){
        *a.d = load_data_iseg(a.n, a.paths, a.m, a.w, a.h, a.classes, a.لمب_boxes, a.scale, a.min, a.max, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    } وإلا إذا (a.type == INSTANCE_DATA){
        *a.d = load_data_mask(a.n, a.paths, a.m, a.w, a.h, a.classes, a.لمب_boxes, a.coords, a.min, a.max, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    } وإلا إذا (a.type == SEGMENTATION_DATA){
        *a.d = load_data_seg(a.n, a.paths, a.m, a.w, a.h, a.classes, a.min, a.max, a.angle, a.aspect, a.hue, a.saturation, a.exposure, a.scale);
    } وإلا إذا (a.type == REGION_DATA){
        *a.d = load_data_region(a.n, a.paths, a.m, a.w, a.h, a.لمب_boxes, a.classes, a.jitter, a.hue, a.saturation, a.exposure);
    } وإلا إذا (a.type == DETECTION_DATA){
        *a.d = load_data_detection(a.n, a.paths, a.m, a.w, a.h, a.لمب_boxes, a.classes, a.jitter, a.hue, a.saturation, a.exposure);
    } وإلا إذا (a.type == SWAG_DATA){
        *a.d = load_data_swag(a.paths, a.n, a.classes, a.jitter);
    } وإلا إذا (a.type == COMPARE_DATA){
        *a.d = load_data_compare(a.n, a.paths, a.m, a.classes, a.w, a.h);
    } وإلا إذا (a.type == IMAGE_DATA){
        *(a.im) = load_image_color(a.path, 0, 0);
        *(a.resized) = resize_image(*(a.im), a.w, a.h);
    } وإلا إذا (a.type == LETTERBOX_DATA){
        *(a.im) = load_image_color(a.path, 0, 0);
        *(a.resized) = letterbox_image(*(a.im), a.w, a.h);
    } وإلا إذا (a.type == TAG_DATA){
        *a.d = load_data_tag(a.paths, a.n, a.m, a.classes, a.min, a.max, a.size, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    }
    حرر(ptr);
    أرجع 0;
}

pthread_t load_data_in_thread(load_حجوجي حجوجي)
{
    pthread_t thread;
    بنية load_حجوجي *ptr = calloc(1, حجم(بنية load_حجوجي));
    *ptr = حجوجي;
    إذا(pthread_create(&thread, 0, load_thread, ptr)) error("Thread creation failed");
    أرجع thread;
}

فراغ *load_threads(فراغ *ptr)
{
    صحيح i;
    load_حجوجي حجوجي = *(load_حجوجي *)ptr;
    إذا (حجوجي.threads == 0) حجوجي.threads = 1;
    data *out = حجوجي.d;
    صحيح total = حجوجي.n;
    حرر(ptr);
    data *buffers = calloc(حجوجي.threads, حجم(data));
    pthread_t *threads = calloc(حجوجي.threads, حجم(pthread_t));
    لكل(i = 0; i < حجوجي.threads; ++i){
        حجوجي.d = buffers + i;
        حجوجي.n = (i+1) * total/حجوجي.threads - i * total/حجوجي.threads;
        threads[i] = load_data_in_thread(حجوجي);
    }
    لكل(i = 0; i < حجوجي.threads; ++i){
        pthread_join(threads[i], 0);
    }
    *out = concat_datas(buffers, حجوجي.threads);
    out->shallow = 0;
    لكل(i = 0; i < حجوجي.threads; ++i){
        buffers[i].shallow = 1;
        free_data(buffers[i]);
    }
    حرر(buffers);
    حرر(threads);
    أرجع 0;
}

فراغ load_data_blocking(load_حجوجي حجوجي)
{
    بنية load_حجوجي *ptr = calloc(1, حجم(بنية load_حجوجي));
    *ptr = حجوجي;
    load_thread(ptr);
}

pthread_t load_data(load_حجوجي حجوجي)
{
    pthread_t thread;
    بنية load_حجوجي *ptr = calloc(1, حجم(بنية load_حجوجي));
    *ptr = حجوجي;
    إذا(pthread_create(&thread, 0, load_threads, ptr)) error("Thread creation failed");
    أرجع thread;
}

data load_data_writing(حرف **paths, صحيح n, صحيح m, صحيح w, صحيح h, صحيح out_w, صحيح out_h)
{
    إذا(m) paths = get_random_paths(paths, n, m);
    حرف **replace_paths = find_replace_paths(paths, n, ".png", "-label.png");
    data d = {0};
    d.shallow = 0;
    d.X = load_image_paths(paths, n, w, h);
    d.y = load_image_paths_gray(replace_paths, n, out_w, out_h);
    إذا(m) حرر(paths);
    صحيح i;
    لكل(i = 0; i < n; ++i) حرر(replace_paths[i]);
    حرر(replace_paths);
    أرجع d;
}

data load_data_old(حرف **paths, صحيح n, صحيح m, حرف **labels, صحيح k, صحيح w, صحيح h)
{
    إذا(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;
    d.X = load_image_paths(paths, n, w, h);
    d.y = load_labels_paths(paths, n, labels, k, 0);
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

data load_data_super(حرف **paths, صحيح n, صحيح m, صحيح w, صحيح h, صحيح scale)
{
    إذا(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;

    صحيح i;
    d.X.rows = n;
    d.X.vals = calloc(n, حجم(عائم*));
    d.X.cols = w*h*3;

    d.y.rows = n;
    d.y.vals = calloc(n, حجم(عائم*));
    d.y.cols = w*scale * h*scale * 3;

    لكل(i = 0; i < n; ++i){
        image im = load_image_color(paths[i], 0, 0);
        image crop = random_crop_image(im, w*scale, h*scale);
        صحيح flip = rand()%2;
        إذا (flip) flip_image(crop);
        image resize = resize_image(crop, w, h);
        d.X.vals[i] = resize.data;
        d.y.vals[i] = crop.data;
        free_image(im);
    }

    إذا(m) حرر(paths);
    أرجع d;
}

data load_data_regression(حرف **paths, صحيح n, صحيح m, صحيح k, صحيح min, صحيح max, صحيح size, عائم angle, عائم aspect, عائم hue, عائم saturation, عائم exposure)
{
    إذا(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;
    d.X = load_image_augment_paths(paths, n, min, max, size, angle, aspect, hue, saturation, exposure, 0);
    d.y = load_regression_labels_paths(paths, n, k);
    إذا(m) حرر(paths);
    أرجع d;
}

data select_data(data *orig, صحيح *inds)
{
    data d = {0};
    d.shallow = 1;
    d.w = orig[0].w;
    d.h = orig[0].h;

    d.X.rows = orig[0].X.rows;
    d.y.rows = orig[0].X.rows;

    d.X.cols = orig[0].X.cols;
    d.y.cols = orig[0].y.cols;

    d.X.vals = calloc(orig[0].X.rows, حجم(عائم *));
    d.y.vals = calloc(orig[0].y.rows, حجم(عائم *));
    صحيح i;
    لكل(i = 0; i < d.X.rows; ++i){
        d.X.vals[i] = orig[inds[i]].X.vals[i];
        d.y.vals[i] = orig[inds[i]].y.vals[i];
    }
    أرجع d;
}

data *tile_data(data orig, صحيح divs, صحيح size)
{
    data *ds = calloc(divs*divs, حجم(data));
    صحيح i, j;
#pragma omp parallel لكل
    لكل(i = 0; i < divs*divs; ++i){
        data d;
        d.shallow = 0;
        d.w = orig.w/divs * size;
        d.h = orig.h/divs * size;
        d.X.rows = orig.X.rows;
        d.X.cols = d.w*d.h*3;
        d.X.vals = calloc(d.X.rows, حجم(عائم*));

        d.y = copy_matrix(orig.y);
#pragma omp parallel لكل
        لكل(j = 0; j < orig.X.rows; ++j){
            صحيح x = (i%divs) * orig.w / divs - (d.w - orig.w/divs)/2;
            صحيح y = (i/divs) * orig.h / divs - (d.h - orig.h/divs)/2;
            image im = float_to_image(orig.w, orig.h, 3, orig.X.vals[j]);
            d.X.vals[j] = crop_image(im, x, y, d.w, d.h).data;
        }
        ds[i] = d;
    }
    أرجع ds;
}

data resize_data(data orig, صحيح w, صحيح h)
{
    data d = {0};
    d.shallow = 0;
    d.w = w;
    d.h = h;
    صحيح i;
    d.X.rows = orig.X.rows;
    d.X.cols = w*h*3;
    d.X.vals = calloc(d.X.rows, حجم(عائم*));

    d.y = copy_matrix(orig.y);
#pragma omp parallel لكل
    لكل(i = 0; i < orig.X.rows; ++i){
        image im = float_to_image(orig.w, orig.h, 3, orig.X.vals[i]);
        d.X.vals[i] = resize_image(im, w, h).data;
    }
    أرجع d;
}

data load_data_augment(حرف **paths, صحيح n, صحيح m, حرف **labels, صحيح k, بشيا *hierarchy, صحيح min, صحيح max, صحيح size, عائم angle, عائم aspect, عائم hue, عائم saturation, عائم exposure, صحيح center)
{
    إذا(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;
    d.w=size;
    d.h=size;
    d.X = load_image_augment_paths(paths, n, min, max, size, angle, aspect, hue, saturation, exposure, center);
    d.y = load_labels_paths(paths, n, labels, k, hierarchy);
    إذا(m) حرر(paths);
    أرجع d;
}

data load_data_tag(حرف **paths, صحيح n, صحيح m, صحيح k, صحيح min, صحيح max, صحيح size, عائم angle, عائم aspect, عائم hue, عائم saturation, عائم exposure)
{
    إذا(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.w = size;
    d.h = size;
    d.shallow = 0;
    d.X = load_image_augment_paths(paths, n, min, max, size, angle, aspect, hue, saturation, exposure, 0);
    d.y = load_tags_paths(paths, n, k);
    إذا(m) حرر(paths);
    أرجع d;
}

matrix concat_matrix(matrix m1, matrix m2)
{
    صحيح i, كدي = 0;
    matrix m;
    m.cols = m1.cols;
    m.rows = m1.rows+m2.rows;
    m.vals = calloc(m1.rows + m2.rows, حجم(عائم*));
    لكل(i = 0; i < m1.rows; ++i){
        m.vals[كدي++] = m1.vals[i];
    }
    لكل(i = 0; i < m2.rows; ++i){
        m.vals[كدي++] = m2.vals[i];
    }
    أرجع m;
}

data concat_data(data d1, data d2)
{
    data d = {0};
    d.shallow = 1;
    d.X = concat_matrix(d1.X, d2.X);
    d.y = concat_matrix(d1.y, d2.y);
    d.w = d1.w;
    d.h = d1.h;
    أرجع d;
}

data concat_datas(data *d, صحيح n)
{
    صحيح i;
    data out = {0};
    لكل(i = 0; i < n; ++i){
        data new = concat_data(d[i], out);
        free_data(out);
        out = new;
    }
    أرجع out;
}

data load_categorical_data_csv(حرف *filename, صحيح target, صحيح k)
{
    data d = {0};
    d.shallow = 0;
    matrix X = csv_to_matrix(filename);
    عائم *truth_1d = چرو_column(&X, target);
    عائم **truth = one_hot_encode(truth_1d, X.rows, k);
    matrix y;
    y.rows = X.rows;
    y.cols = k;
    y.vals = truth;
    d.X = X;
    d.y = y;
    حرر(truth_1d);
    أرجع d;
}

data load_cifar10_data(حرف *filename)
{
    data d = {0};
    d.shallow = 0;
    long i,j;
    matrix X = make_matrix(10000, 3072);
    matrix y = make_matrix(10000, 10);
    d.X = X;
    d.y = y;

    FILE *fp = fopen(filename, "rb");
    إذا(!fp) file_error(filename);
    لكل(i = 0; i < 10000; ++i){
        unsigned حرف bytes[3073];
        fread(bytes, 1, 3073, fp);
        صحيح class = bytes[0];
        y.vals[i][class] = 1;
        لكل(j = 0; j < X.cols; ++j){
            X.vals[i][j] = (مزدوج)bytes[j+1];
        }
    }
    scale_data_rows(d, 1./255);
    //normalize_data_rows(d);
    fclose(fp);
    أرجع d;
}

فراغ get_random_batch(data d, صحيح n, عائم *X, عائم *y)
{
    صحيح j;
    لكل(j = 0; j < n; ++j){
        صحيح index = rand()%d.X.rows;
        memcpy(X+j*d.X.cols, d.X.vals[index], d.X.cols*حجم(عائم));
        memcpy(y+j*d.y.cols, d.y.vals[index], d.y.cols*حجم(عائم));
    }
}

فراغ get_next_batch(data d, صحيح n, صحيح offset, عائم *X, عائم *y)
{
    صحيح j;
    لكل(j = 0; j < n; ++j){
        صحيح index = offset + j;
        memcpy(X+j*d.X.cols, d.X.vals[index], d.X.cols*حجم(عائم));
        إذا(y) memcpy(y+j*d.y.cols, d.y.vals[index], d.y.cols*حجم(عائم));
    }
}

فراغ smooth_data(data d)
{
    صحيح i, j;
    عائم scale = 1. / d.y.cols;
    عائم eps = .1;
    لكل(i = 0; i < d.y.rows; ++i){
        لكل(j = 0; j < d.y.cols; ++j){
            d.y.vals[i][j] = eps * scale + (1-eps) * d.y.vals[i][j];
        }
    }
}

data load_all_cifar10()
{
    data d = {0};
    d.shallow = 0;
    صحيح i,j,b;
    matrix X = make_matrix(50000, 3072);
    matrix y = make_matrix(50000, 10);
    d.X = X;
    d.y = y;


    لكل(b = 0; b < 5; ++b){
        حرف buff[256];
        sprintf(buff, "data/cifar/cifar-10-batches-bin/data_batch_%d.bin", b+1);
        FILE *fp = fopen(buff, "rb");
        إذا(!fp) file_error(buff);
        لكل(i = 0; i < 10000; ++i){
            unsigned حرف bytes[3073];
            fread(bytes, 1, 3073, fp);
            صحيح class = bytes[0];
            y.vals[i+b*10000][class] = 1;
            لكل(j = 0; j < X.cols; ++j){
                X.vals[i+b*10000][j] = (مزدوج)bytes[j+1];
            }
        }
        fclose(fp);
    }
    //normalize_data_rows(d);
    scale_data_rows(d, 1./255);
    smooth_data(d);
    أرجع d;
}

data load_go(حرف *filename)
{
    FILE *fp = fopen(filename, "rb");
    matrix X = make_matrix(3363059, 361);
    matrix y = make_matrix(3363059, 361);
    صحيح row, col;

    إذا(!fp) file_error(filename);
    حرف *label;
    صحيح كدي = 0;
    طالما((label = fgetl(fp))){
        صحيح i;
        إذا(كدي == X.rows){
            X = resize_matrix(X, كدي*2);
            y = resize_matrix(y, كدي*2);
        }
        sscanf(label, "%d %d", &row, &col);
        حرف *board = fgetl(fp);

        صحيح index = row*19 + col;
        y.vals[كدي][index] = 1;

        لكل(i = 0; i < 19*19; ++i){
            عائم val = 0;
            إذا(board[i] == '1') val = 1;
            وإلا إذا(board[i] == '2') val = -1;
            X.vals[كدي][i] = val;
        }
        ++كدي;
        حرر(label);
        حرر(board);
    }
    X = resize_matrix(X, كدي);
    y = resize_matrix(y, كدي);

    data d = {0};
    d.shallow = 0;
    d.X = X;
    d.y = y;


    fclose(fp);

    أرجع d;
}


فراغ randomize_data(data d)
{
    صحيح i;
    لكل(i = d.X.rows-1; i > 0; --i){
        صحيح index = rand()%i;
        عائم *swap = d.X.vals[index];
        d.X.vals[index] = d.X.vals[i];
        d.X.vals[i] = swap;

        swap = d.y.vals[index];
        d.y.vals[index] = d.y.vals[i];
        d.y.vals[i] = swap;
    }
}

فراغ scale_data_rows(data d, عائم s)
{
    صحيح i;
    لكل(i = 0; i < d.X.rows; ++i){
        scale_array(d.X.vals[i], d.X.cols, s);
    }
}

فراغ translate_data_rows(data d, عائم s)
{
    صحيح i;
    لكل(i = 0; i < d.X.rows; ++i){
        translate_array(d.X.vals[i], d.X.cols, s);
    }
}

data copy_data(data d)
{
    data c = {0};
    c.w = d.w;
    c.h = d.h;
    c.shallow = 0;
    c.لمب_boxes = d.لمب_boxes;
    c.boxes = d.boxes;
    c.X = copy_matrix(d.X);
    c.y = copy_matrix(d.y);
    أرجع c;
}

فراغ normalize_data_rows(data d)
{
    صحيح i;
    لكل(i = 0; i < d.X.rows; ++i){
        normalize_array(d.X.vals[i], d.X.cols);
    }
}

data get_data_part(data d, صحيح part, صحيح total)
{
    data p = {0};
    p.shallow = 1;
    p.X.rows = d.X.rows * (part + 1) / total - d.X.rows * part / total;
    p.y.rows = d.y.rows * (part + 1) / total - d.y.rows * part / total;
    p.X.cols = d.X.cols;
    p.y.cols = d.y.cols;
    p.X.vals = d.X.vals + d.X.rows * part / total;
    p.y.vals = d.y.vals + d.y.rows * part / total;
    أرجع p;
}

data get_random_data(data d, صحيح لمب)
{
    data r = {0};
    r.shallow = 1;

    r.X.rows = لمب;
    r.y.rows = لمب;

    r.X.cols = d.X.cols;
    r.y.cols = d.y.cols;

    r.X.vals = calloc(لمب, حجم(عائم *));
    r.y.vals = calloc(لمب, حجم(عائم *));

    صحيح i;
    لكل(i = 0; i < لمب; ++i){
        صحيح index = rand()%d.X.rows;
        r.X.vals[i] = d.X.vals[index];
        r.y.vals[i] = d.y.vals[index];
    }
    أرجع r;
}

data *split_data(data d, صحيح part, صحيح total)
{
    data *split = calloc(2, حجم(data));
    صحيح i;
    صحيح start = part*d.X.rows/total;
    صحيح end = (part+1)*d.X.rows/total;
    data train;
    data test;
    train.shallow = test.shallow = 1;

    test.X.rows = test.y.rows = end-start;
    train.X.rows = train.y.rows = d.X.rows - (end-start);
    train.X.cols = test.X.cols = d.X.cols;
    train.y.cols = test.y.cols = d.y.cols;

    train.X.vals = calloc(train.X.rows, حجم(عائم*));
    test.X.vals = calloc(test.X.rows, حجم(عائم*));
    train.y.vals = calloc(train.y.rows, حجم(عائم*));
    test.y.vals = calloc(test.y.rows, حجم(عائم*));

    لكل(i = 0; i < start; ++i){
        train.X.vals[i] = d.X.vals[i];
        train.y.vals[i] = d.y.vals[i];
    }
    لكل(i = start; i < end; ++i){
        test.X.vals[i-start] = d.X.vals[i];
        test.y.vals[i-start] = d.y.vals[i];
    }
    لكل(i = end; i < d.X.rows; ++i){
        train.X.vals[i-(end-start)] = d.X.vals[i];
        train.y.vals[i-(end-start)] = d.y.vals[i];
    }
    split[0] = train;
    split[1] = test;
    أرجع split;
}

