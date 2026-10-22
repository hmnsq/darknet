#شامل "data.h"
#شامل "utils.h"
#شامل "image.h"
#شامل "cuda.h"

#شامل <stdio.h>
#شامل <stdlib.h>
#شامل <string.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

list *get_paths(نویسه *filename)
{
    نویسه *path;
    FILE *file = fopen(filename, "r");
    اگر(!file) file_error(filename);
    list *سطرها = make_list();
    وقتی((path=fgetl(file))){
        list_درج(سطرها, path);
    }
    fclose(file);
    بازگشت سطرها;
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

نویسه **get_random_paths(نویسه **paths, صحیح n, صحیح m)
{
    نویسه **random_paths = calloc(n, اندازه(نویسه*));
    صحیح i;
    pthread_mutex_lock(&mutex);
    برای(i = 0; i < n; ++i){
        صحیح index = rand()%m;
        random_paths[i] = paths[index];
        //if(i == 0) printf("%s\n", paths[index]);
    }
    pthread_mutex_unlock(&mutex);
    بازگشت random_paths;
}

نویسه **find_replace_paths(نویسه **paths, صحیح n, نویسه *find, نویسه *replace)
{
    نویسه **replace_paths = calloc(n, اندازه(نویسه*));
    صحیح i;
    برای(i = 0; i < n; ++i){
        نویسه replaced[4096];
        find_replace(paths[i], find, replace, replaced);
        replace_paths[i] = copy_string(replaced);
    }
    بازگشت replace_paths;
}

matrix load_image_paths_gray(نویسه **paths, صحیح n, صحیح w, صحیح h)
{
    صحیح i;
    matrix X;
    X.rows = n;
    X.vals = calloc(X.rows, اندازه(اعشاری*));
    X.cols = 0;

    برای(i = 0; i < n; ++i){
        image im = load_image(paths[i], w, h, 3);

        image gray = grayscale_image(im);
        free_image(im);
        im = gray;

        X.vals[i] = im.data;
        X.cols = im.h*im.w*im.c;
    }
    بازگشت X;
}

matrix load_image_paths(نویسه **paths, صحیح n, صحیح w, صحیح h)
{
    صحیح i;
    matrix X;
    X.rows = n;
    X.vals = calloc(X.rows, اندازه(اعشاری*));
    X.cols = 0;

    برای(i = 0; i < n; ++i){
        image im = load_image_color(paths[i], w, h);
        X.vals[i] = im.data;
        X.cols = im.h*im.w*im.c;
    }
    بازگشت X;
}

matrix load_image_augment_paths(نویسه **paths, صحیح n, صحیح min, صحیح max, صحیح size, اعشاری angle, اعشاری aspect, اعشاری hue, اعشاری saturation, اعشاری exposure, صحیح center)
{
    صحیح i;
    matrix X;
    X.rows = n;
    X.vals = calloc(X.rows, اندازه(اعشاری*));
    X.cols = 0;

    برای(i = 0; i < n; ++i){
        image im = load_image_color(paths[i], 0, 0);
        image crop;
        اگر(center){
            crop = center_crop_image(im, size, size);
        } وگرنه {
            crop = random_augment_image(im, angle, aspect, min, max, size, size);
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
        X.vals[i] = crop.data;
        X.cols = crop.h*crop.w*crop.c;
    }
    بازگشت X;
}


box_label *read_boxes(نویسه *filename, صحیح *n)
{
    FILE *file = fopen(filename, "r");
    اگر(!file) file_error(filename);
    اعشاری x, y, h, w;
    صحیح id;
    صحیح شمار = 0;
    صحیح size = 64;
    box_label *boxes = calloc(size, اندازه(box_label));
    وقتی(fscanf(file, "%d %f %f %f %f", &id, &x, &y, &w, &h) == 5){
        اگر(شمار == size) {
            size = size * 2;
            boxes = realloc(boxes, size*اندازه(box_label));
        }
        boxes[شمار].id = id;
        boxes[شمار].x = x;
        boxes[شمار].y = y;
        boxes[شمار].h = h;
        boxes[شمار].w = w;
        boxes[شمار].چپ   = x - w/2;
        boxes[شمار].راست  = x + w/2;
        boxes[شمار].بالاترین    = y - h/2;
        boxes[شمار].bottom = y + h/2;
        ++شمار;
    }
    fclose(file);
    *n = شمار;
    بازگشت boxes;
}

تهی randomize_boxes(box_label *b, صحیح n)
{
    صحیح i;
    برای(i = 0; i < n; ++i){
        box_label swap = b[i];
        صحیح index = rand()%n;
        b[i] = b[index];
        b[index] = swap;
    }
}

تهی correct_boxes(box_label *boxes, صحیح n, اعشاری dx, اعشاری dy, اعشاری sx, اعشاری sy, صحیح flip)
{
    صحیح i;
    برای(i = 0; i < n; ++i){
        اگر(boxes[i].x == 0 && boxes[i].y == 0) {
            boxes[i].x = 999999;
            boxes[i].y = 999999;
            boxes[i].w = 999999;
            boxes[i].h = 999999;
            ادامه;
        }
        boxes[i].چپ   = boxes[i].چپ  * sx - dx;
        boxes[i].راست  = boxes[i].راست * sx - dx;
        boxes[i].بالاترین    = boxes[i].بالاترین   * sy - dy;
        boxes[i].bottom = boxes[i].bottom* sy - dy;

        اگر(flip){
            اعشاری swap = boxes[i].چپ;
            boxes[i].چپ = 1. - boxes[i].راست;
            boxes[i].راست = 1. - swap;
        }

        boxes[i].چپ =  constrain(0, 1, boxes[i].چپ);
        boxes[i].راست = constrain(0, 1, boxes[i].راست);
        boxes[i].بالاترین =   constrain(0, 1, boxes[i].بالاترین);
        boxes[i].bottom =   constrain(0, 1, boxes[i].bottom);

        boxes[i].x = (boxes[i].چپ+boxes[i].راست)/2;
        boxes[i].y = (boxes[i].بالاترین+boxes[i].bottom)/2;
        boxes[i].w = (boxes[i].راست - boxes[i].چپ);
        boxes[i].h = (boxes[i].bottom - boxes[i].بالاترین);

        boxes[i].w = constrain(0, 1, boxes[i].w);
        boxes[i].h = constrain(0, 1, boxes[i].h);
    }
}

تهی fill_truth_swag(نویسه *path, اعشاری *truth, صحیح classes, صحیح flip, اعشاری dx, اعشاری dy, اعشاری sx, اعشاری sy)
{
    نویسه labelpath[4096];
    find_replace(path, "images", "labels", labelpath);
    find_replace(labelpath, "JPEGImages", "labels", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);

    صحیح شمار = 0;
    box_label *boxes = read_boxes(labelpath, &شمار);
    randomize_boxes(boxes, شمار);
    correct_boxes(boxes, شمار, dx, dy, sx, sy, flip);
    اعشاری x,y,w,h;
    صحیح id;
    صحیح i;

    برای (i = 0; i < شمار && i < 90; ++i) {
        x =  boxes[i].x;
        y =  boxes[i].y;
        w =  boxes[i].w;
        h =  boxes[i].h;
        id = boxes[i].id;

        اگر (w < .0 || h < .0) ادامه;

        صحیح index = (4+classes) * i;

        truth[index++] = x;
        truth[index++] = y;
        truth[index++] = w;
        truth[index++] = h;

        اگر (id < classes) truth[index+id] = 1;
    }
    آزاد(boxes);
}

تهی fill_truth_region(نویسه *path, اعشاری *truth, صحیح classes, صحیح رقم_boxes, صحیح flip, اعشاری dx, اعشاری dy, اعشاری sx, اعشاری sy)
{
    نویسه labelpath[4096];
    find_replace(path, "images", "labels", labelpath);
    find_replace(labelpath, "JPEGImages", "labels", labelpath);

    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".png", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    صحیح شمار = 0;
    box_label *boxes = read_boxes(labelpath, &شمار);
    randomize_boxes(boxes, شمار);
    correct_boxes(boxes, شمار, dx, dy, sx, sy, flip);
    اعشاری x,y,w,h;
    صحیح id;
    صحیح i;

    برای (i = 0; i < شمار; ++i) {
        x =  boxes[i].x;
        y =  boxes[i].y;
        w =  boxes[i].w;
        h =  boxes[i].h;
        id = boxes[i].id;

        اگر (w < .005 || h < .005) ادامه;

        صحیح col = (صحیح)(x*رقم_boxes);
        صحیح row = (صحیح)(y*رقم_boxes);

        x = x*رقم_boxes - col;
        y = y*رقم_boxes - row;

        صحیح index = (col+row*رقم_boxes)*(5+classes);
        اگر (truth[index]) ادامه;
        truth[index++] = 1;

        اگر (id < classes) truth[index+id] = 1;
        index += classes;

        truth[index++] = x;
        truth[index++] = y;
        truth[index++] = w;
        truth[index++] = h;
    }
    آزاد(boxes);
}

تهی load_rle(image im, صحیح *rle, صحیح n)
{
    صحیح شمار = 0;
    صحیح curr = 0;
    صحیح i,j;
    برای(i = 0; i < n; ++i){
        برای(j = 0; j < rle[i]; ++j){
            im.data[شمار++] = curr;
        }
        curr = 1 - curr;
    }
    برای(; شمار < im.h*im.w*im.c; ++شمار){
        im.data[شمار] = curr;
    }
}

تهی or_image(image src, image dest, صحیح c)
{
    صحیح i;
    برای(i = 0; i < src.w*src.h; ++i){
        اگر(src.data[i]) dest.data[dest.w*dest.h*c + i] = 1;
    }
}

تهی exclusive_image(image src)
{
    صحیح k, j, i;
    صحیح s = src.w*src.h;
    برای(k = 0; k < src.c-1; ++k){
        برای(i = 0; i < s; ++i){
            اگر (src.data[k*s + i]){
                برای(j = k+1; j < src.c; ++j){
                    src.data[j*s + i] = 0;
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
    برای(y = 0; y < im.h; ++y){
        برای(x = 0; x < im.w; ++x){
            اگر(im.data[y*im.w + x]){
                minx = (x < minx) ? x : minx;
                miny = (y < miny) ? y : miny;
                maxx = (x > maxx) ? x : maxx;
                maxy = (y > maxy) ? y : maxy;
            }
        }
    }
    box b = {minx, miny, maxx-minx + 1, maxy-miny + 1};
    //printf("%f %f %f %f\n", b.x, b.y, b.w, b.h);
    بازگشت b;
}

تهی fill_truth_iseg(نویسه *path, صحیح رقم_boxes, اعشاری *truth, صحیح classes, صحیح w, صحیح h, augment_args aug, صحیح flip, صحیح mw, صحیح mh)
{
    نویسه labelpath[4096];
    find_replace(path, "images", "mask", labelpath);
    find_replace(labelpath, "JPEGImages", "mask", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    FILE *file = fopen(labelpath, "r");
    اگر(!file) file_error(labelpath);
    نویسه buff[32788];
    صحیح id;
    صحیح i = 0;
    صحیح j;
    image part = make_image(w, h, 1);
    وقتی((fscanf(file, "%d %s", &id, buff) == 2) && i < رقم_boxes){
        صحیح n = 0;
        صحیح *rle = read_intlist(buff, &n, 0);
        load_rle(part, rle, n);
        image sized = rotate_crop_image(part, aug.rad, aug.scale, aug.w, aug.h, aug.dx, aug.dy, aug.aspect);
        اگر(flip) flip_image(sized);

        image mask = resize_image(sized, mw, mh);
        truth[i*(mw*mh+1)] = id;
        برای(j = 0; j < mw*mh; ++j){
            truth[i*(mw*mh + 1) + 1 + j] = mask.data[j];
        }
        ++i;

        free_image(mask);
        free_image(sized);
        آزاد(rle);
    }
    اگر(i < رقم_boxes) truth[i*(mw*mh+1)] = -1;
    fclose(file);
    free_image(part);
}

تهی fill_truth_mask(نویسه *path, صحیح رقم_boxes, اعشاری *truth, صحیح classes, صحیح w, صحیح h, augment_args aug, صحیح flip, صحیح mw, صحیح mh)
{
    نویسه labelpath[4096];
    find_replace(path, "images", "mask", labelpath);
    find_replace(labelpath, "JPEGImages", "mask", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    FILE *file = fopen(labelpath, "r");
    اگر(!file) file_error(labelpath);
    نویسه buff[32788];
    صحیح id;
    صحیح i = 0;
    image part = make_image(w, h, 1);
    وقتی((fscanf(file, "%d %s", &id, buff) == 2) && i < رقم_boxes){
        صحیح n = 0;
        صحیح *rle = read_intlist(buff, &n, 0);
        load_rle(part, rle, n);
        image sized = rotate_crop_image(part, aug.rad, aug.scale, aug.w, aug.h, aug.dx, aug.dy, aug.aspect);
        اگر(flip) flip_image(sized);
        box b = bound_image(sized);
        اگر(b.w > 0){
            image crop = crop_image(sized, b.x, b.y, b.w, b.h);
            image mask = resize_image(crop, mw, mh);
            truth[i*(4 + mw*mh + 1) + 0] = (b.x + b.w/2.)/sized.w;
            truth[i*(4 + mw*mh + 1) + 1] = (b.y + b.h/2.)/sized.h;
            truth[i*(4 + mw*mh + 1) + 2] = b.w/sized.w;
            truth[i*(4 + mw*mh + 1) + 3] = b.h/sized.h;
            صحیح j;
            برای(j = 0; j < mw*mh; ++j){
                truth[i*(4 + mw*mh + 1) + 4 + j] = mask.data[j];
            }
            truth[i*(4 + mw*mh + 1) + 4 + mw*mh] = id;
            free_image(crop);
            free_image(mask);
            ++i;
        }
        free_image(sized);
        آزاد(rle);
    }
    fclose(file);
    free_image(part);
}


تهی fill_truth_detection(نویسه *path, صحیح رقم_boxes, اعشاری *truth, صحیح classes, صحیح flip, اعشاری dx, اعشاری dy, اعشاری sx, اعشاری sy)
{
    نویسه labelpath[4096];
    find_replace(path, "images", "labels", labelpath);
    find_replace(labelpath, "JPEGImages", "labels", labelpath);

    find_replace(labelpath, "raw", "labels", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".png", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    صحیح شمار = 0;
    box_label *boxes = read_boxes(labelpath, &شمار);
    randomize_boxes(boxes, شمار);
    correct_boxes(boxes, شمار, dx, dy, sx, sy, flip);
    اگر(شمار > رقم_boxes) شمار = رقم_boxes;
    اعشاری x,y,w,h;
    صحیح id;
    صحیح i;
    صحیح sub = 0;

    برای (i = 0; i < شمار; ++i) {
        x =  boxes[i].x;
        y =  boxes[i].y;
        w =  boxes[i].w;
        h =  boxes[i].h;
        id = boxes[i].id;

        اگر ((w < .001 || h < .001)) {
            ++sub;
            ادامه;
        }

        truth[(i-sub)*5+0] = x;
        truth[(i-sub)*5+1] = y;
        truth[(i-sub)*5+2] = w;
        truth[(i-sub)*5+3] = h;
        truth[(i-sub)*5+4] = id;
    }
    آزاد(boxes);
}

#define NUMCHARS 37

تهی print_letters(اعشاری *pred, صحیح n)
{
    صحیح i;
    برای(i = 0; i < n; ++i){
        صحیح index = max_index(pred+i*NUMCHARS, NUMCHARS);
        printf("%c", int_to_alphanum(index));
    }
    printf("\n");
}

تهی fill_truth_captcha(نویسه *path, صحیح n, اعشاری *truth)
{
    نویسه *begin = strrchr(path, '/');
    ++begin;
    صحیح i;
    برای(i = 0; i < strlen(begin) && i < n && begin[i] != '.'; ++i){
        صحیح index = alphanum_to_int(begin[i]);
        اگر(index > 35) printf("Bad %c\n", begin[i]);
        truth[i*NUMCHARS+index] = 1;
    }
    برای(;i < n; ++i){
        truth[i*NUMCHARS + NUMCHARS-1] = 1;
    }
}

data load_data_captcha(نویسه **paths, صحیح n, صحیح m, صحیح k, صحیح w, صحیح h)
{
    اگر(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;
    d.X = load_image_paths(paths, n, w, h);
    d.y = make_matrix(n, k*NUMCHARS);
    صحیح i;
    برای(i = 0; i < n; ++i){
        fill_truth_captcha(paths[i], k, d.y.vals[i]);
    }
    اگر(m) آزاد(paths);
    بازگشت d;
}

data load_data_captcha_encode(نویسه **paths, صحیح n, صحیح m, صحیح w, صحیح h)
{
    اگر(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;
    d.X = load_image_paths(paths, n, w, h);
    d.X.cols = 17100;
    d.y = d.X;
    اگر(m) آزاد(paths);
    بازگشت d;
}

تهی fill_truth(نویسه *path, نویسه **labels, صحیح k, اعشاری *truth)
{
    صحیح i;
    memset(truth, 0, k*اندازه(اعشاری));
    صحیح شمار = 0;
    برای(i = 0; i < k; ++i){
        اگر(strstr(path, labels[i])){
            truth[i] = 1;
            ++شمار;
            //printf("%s %s %d\n", path, labels[i], i);
        }
    }
    اگر(شمار != 1 && (k != 1 || شمار != 0)) printf("Too many or too few labels: %d, %s\n", شمار, path);
}

تهی fill_hierarchy(اعشاری *truth, صحیح k, درخت *hierarchy)
{
    صحیح j;
    برای(j = 0; j < k; ++j){
        اگر(truth[j]){
            صحیح parent = hierarchy->parent[j];
            وقتی(parent >= 0){
                truth[parent] = 1;
                parent = hierarchy->parent[parent];
            }
        }
    }
    صحیح i;
    صحیح شمار = 0;
    برای(j = 0; j < hierarchy->groups; ++j){
        //printf("%d\n", count);
        صحیح mask = 1;
        برای(i = 0; i < hierarchy->group_size[j]; ++i){
            اگر(truth[شمار + i]){
                mask = 0;
                بشکن;
            }
        }
        اگر (mask) {
            برای(i = 0; i < hierarchy->group_size[j]; ++i){
                truth[شمار + i] = SECRET_رقم;
            }
        }
        شمار += hierarchy->group_size[j];
    }
}

matrix load_regression_labels_paths(نویسه **paths, صحیح n, صحیح k)
{
    matrix y = make_matrix(n, k);
    صحیح i,j;
    برای(i = 0; i < n; ++i){
        نویسه labelpath[4096];
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
        برای(j = 0; j < k; ++j){
            fscanf(file, "%f", &(y.vals[i][j]));
        }
        fclose(file);
    }
    بازگشت y;
}

matrix load_labels_paths(نویسه **paths, صحیح n, نویسه **labels, صحیح k, درخت *hierarchy)
{
    matrix y = make_matrix(n, k);
    صحیح i;
    برای(i = 0; i < n && labels; ++i){
        fill_truth(paths[i], labels, k, y.vals[i]);
        اگر(hierarchy){
            fill_hierarchy(y.vals[i], k, hierarchy);
        }
    }
    بازگشت y;
}

matrix load_tags_paths(نویسه **paths, صحیح n, صحیح k)
{
    matrix y = make_matrix(n, k);
    صحیح i;
    //int count = 0;
    برای(i = 0; i < n; ++i){
        نویسه label[4096];
        find_replace(paths[i], "images", "labels", label);
        find_replace(label, ".jpg", ".txt", label);
        FILE *file = fopen(label, "r");
        اگر (!file) ادامه;
        //++count;
        صحیح tag;
        وقتی(fscanf(file, "%d", &tag) == 1){
            اگر(tag < k){
                y.vals[i][tag] = 1;
            }
        }
        fclose(file);
    }
    //printf("%d/%d\n", count, n);
    بازگشت y;
}

نویسه **get_labels(نویسه *filename)
{
    list *plist = get_paths(filename);
    نویسه **labels = (نویسه **)list_to_array(plist);
    free_list(plist);
    بازگشت labels;
}

تهی free_data(data d)
{
    اگر(!d.shallow){
        free_matrix(d.X);
        free_matrix(d.y);
    }وگرنه{
        آزاد(d.X.vals);
        آزاد(d.y.vals);
    }
}

image get_segmentation_image(نویسه *path, صحیح w, صحیح h, صحیح classes)
{
    نویسه labelpath[4096];
    find_replace(path, "images", "mask", labelpath);
    find_replace(labelpath, "JPEGImages", "mask", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    image mask = make_image(w, h, classes);
    FILE *file = fopen(labelpath, "r");
    اگر(!file) file_error(labelpath);
    نویسه buff[32788];
    صحیح id;
    image part = make_image(w, h, 1);
    وقتی(fscanf(file, "%d %s", &id, buff) == 2){
        صحیح n = 0;
        صحیح *rle = read_intlist(buff, &n, 0);
        load_rle(part, rle, n);
        or_image(part, mask, id);
        آزاد(rle);
    }
    //exclusive_image(mask);
    fclose(file);
    free_image(part);
    بازگشت mask;
}

image get_segmentation_image2(نویسه *path, صحیح w, صحیح h, صحیح classes)
{
    نویسه labelpath[4096];
    find_replace(path, "images", "mask", labelpath);
    find_replace(labelpath, "JPEGImages", "mask", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    image mask = make_image(w, h, classes+1);
    صحیح i;
    برای(i = 0; i < w*h; ++i){
        mask.data[w*h*classes + i] = 1;
    }
    FILE *file = fopen(labelpath, "r");
    اگر(!file) file_error(labelpath);
    نویسه buff[32788];
    صحیح id;
    image part = make_image(w, h, 1);
    وقتی(fscanf(file, "%d %s", &id, buff) == 2){
        صحیح n = 0;
        صحیح *rle = read_intlist(buff, &n, 0);
        load_rle(part, rle, n);
        or_image(part, mask, id);
        برای(i = 0; i < w*h; ++i){
            اگر(part.data[i]) mask.data[w*h*classes + i] = 0;
        }
        آزاد(rle);
    }
    //exclusive_image(mask);
    fclose(file);
    free_image(part);
    بازگشت mask;
}

data load_data_seg(صحیح n, نویسه **paths, صحیح m, صحیح w, صحیح h, صحیح classes, صحیح min, صحیح max, اعشاری angle, اعشاری aspect, اعشاری hue, اعشاری saturation, اعشاری exposure, صحیح div)
{
    نویسه **random_paths = get_random_paths(paths, n, m);
    صحیح i;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, اندازه(اعشاری*));
    d.X.cols = h*w*3;


    d.y.rows = n;
    d.y.cols = h*w*classes/div/div;
    d.y.vals = calloc(d.X.rows, اندازه(اعشاری*));

    برای(i = 0; i < n; ++i){
        image orig = load_image_color(random_paths[i], 0, 0);
        augment_args a = random_augment_args(orig, angle, aspect, min, max, w, h);
        image sized = rotate_crop_image(orig, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);

        صحیح flip = rand()%2;
        اگر(flip) flip_image(sized);
        random_distort_image(sized, hue, saturation, exposure);
        d.X.vals[i] = sized.data;

        image mask = get_segmentation_image(random_paths[i], orig.w, orig.h, classes);
        //image mask = make_image(orig.w, orig.h, classes+1);
        image sized_m = rotate_crop_image(mask, a.rad, a.scale/div, a.w/div, a.h/div, a.dx/div, a.dy/div, a.aspect);

        اگر(flip) flip_image(sized_m);
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
    آزاد(random_paths);
    بازگشت d;
}

data load_data_iseg(صحیح n, نویسه **paths, صحیح m, صحیح w, صحیح h, صحیح classes, صحیح boxes, صحیح div, صحیح min, صحیح max, اعشاری angle, اعشاری aspect, اعشاری hue, اعشاری saturation, اعشاری exposure)
{
    نویسه **random_paths = get_random_paths(paths, n, m);
    صحیح i;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, اندازه(اعشاری*));
    d.X.cols = h*w*3;

    d.y = make_matrix(n, (((w/div)*(h/div))+1)*boxes);

    برای(i = 0; i < n; ++i){
        image orig = load_image_color(random_paths[i], 0, 0);
        augment_args a = random_augment_args(orig, angle, aspect, min, max, w, h);
        image sized = rotate_crop_image(orig, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);

        صحیح flip = rand()%2;
        اگر(flip) flip_image(sized);
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
    آزاد(random_paths);
    بازگشت d;
}

data load_data_mask(صحیح n, نویسه **paths, صحیح m, صحیح w, صحیح h, صحیح classes, صحیح boxes, صحیح coords, صحیح min, صحیح max, اعشاری angle, اعشاری aspect, اعشاری hue, اعشاری saturation, اعشاری exposure)
{
    نویسه **random_paths = get_random_paths(paths, n, m);
    صحیح i;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, اندازه(اعشاری*));
    d.X.cols = h*w*3;

    d.y = make_matrix(n, (coords+1)*boxes);

    برای(i = 0; i < n; ++i){
        image orig = load_image_color(random_paths[i], 0, 0);
        augment_args a = random_augment_args(orig, angle, aspect, min, max, w, h);
        image sized = rotate_crop_image(orig, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);

        صحیح flip = rand()%2;
        اگر(flip) flip_image(sized);
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
    آزاد(random_paths);
    بازگشت d;
}

data load_data_region(صحیح n, نویسه **paths, صحیح m, صحیح w, صحیح h, صحیح size, صحیح classes, اعشاری jitter, اعشاری hue, اعشاری saturation, اعشاری exposure)
{
    نویسه **random_paths = get_random_paths(paths, n, m);
    صحیح i;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, اندازه(اعشاری*));
    d.X.cols = h*w*3;


    صحیح k = size*size*(5+classes);
    d.y = make_matrix(n, k);
    برای(i = 0; i < n; ++i){
        image orig = load_image_color(random_paths[i], 0, 0);

        صحیح oh = orig.h;
        صحیح ow = orig.w;

        صحیح dw = (ow*jitter);
        صحیح dh = (oh*jitter);

        صحیح pleft  = rand_uniform(-dw, dw);
        صحیح pright = rand_uniform(-dw, dw);
        صحیح ptop   = rand_uniform(-dh, dh);
        صحیح pbot   = rand_uniform(-dh, dh);

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
        d.X.vals[i] = sized.data;

        fill_truth_region(random_paths[i], d.y.vals[i], classes, size, flip, dx, dy, 1./sx, 1./sy);

        free_image(orig);
        free_image(cropped);
    }
    آزاد(random_paths);
    بازگشت d;
}

data load_data_compare(صحیح n, نویسه **paths, صحیح m, صحیح classes, صحیح w, صحیح h)
{
    اگر(m) paths = get_random_paths(paths, 2*n, m);
    صحیح i,j;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, اندازه(اعشاری*));
    d.X.cols = h*w*6;

    صحیح k = 2*(classes);
    d.y = make_matrix(n, k);
    برای(i = 0; i < n; ++i){
        image im1 = load_image_color(paths[i*2],   w, h);
        image im2 = load_image_color(paths[i*2+1], w, h);

        d.X.vals[i] = calloc(d.X.cols, اندازه(اعشاری));
        memcpy(d.X.vals[i],         im1.data, h*w*3*اندازه(اعشاری));
        memcpy(d.X.vals[i] + h*w*3, im2.data, h*w*3*اندازه(اعشاری));

        صحیح id;
        اعشاری iou;

        نویسه imlabel1[4096];
        نویسه imlabel2[4096];
        find_replace(paths[i*2],   "imgs", "labels", imlabel1);
        find_replace(imlabel1, "jpg", "txt", imlabel1);
        FILE *fp1 = fopen(imlabel1, "r");

        وقتی(fscanf(fp1, "%d %f", &id, &iou) == 2){
            اگر (d.y.vals[i][2*id] < iou) d.y.vals[i][2*id] = iou;
        }

        find_replace(paths[i*2+1], "imgs", "labels", imlabel2);
        find_replace(imlabel2, "jpg", "txt", imlabel2);
        FILE *fp2 = fopen(imlabel2, "r");

        وقتی(fscanf(fp2, "%d %f", &id, &iou) == 2){
            اگر (d.y.vals[i][2*id + 1] < iou) d.y.vals[i][2*id + 1] = iou;
        }

        برای (j = 0; j < classes; ++j){
            اگر (d.y.vals[i][2*j] > .5 &&  d.y.vals[i][2*j+1] < .5){
                d.y.vals[i][2*j] = 1;
                d.y.vals[i][2*j+1] = 0;
            } وگرنه اگر (d.y.vals[i][2*j] < .5 &&  d.y.vals[i][2*j+1] > .5){
                d.y.vals[i][2*j] = 0;
                d.y.vals[i][2*j+1] = 1;
            } وگرنه {
                d.y.vals[i][2*j]   = SECRET_رقم;
                d.y.vals[i][2*j+1] = SECRET_رقم;
            }
        }
        fclose(fp1);
        fclose(fp2);

        free_image(im1);
        free_image(im2);
    }
    اگر(m) آزاد(paths);
    بازگشت d;
}

data load_data_swag(نویسه **paths, صحیح n, صحیح classes, اعشاری jitter)
{
    صحیح index = rand()%n;
    نویسه *random_path = paths[index];

    image orig = load_image_color(random_path, 0, 0);
    صحیح h = orig.h;
    صحیح w = orig.w;

    data d = {0};
    d.shallow = 0;
    d.w = w;
    d.h = h;

    d.X.rows = 1;
    d.X.vals = calloc(d.X.rows, اندازه(اعشاری*));
    d.X.cols = h*w*3;

    صحیح k = (4+classes)*90;
    d.y = make_matrix(1, k);

    صحیح dw = w*jitter;
    صحیح dh = h*jitter;

    صحیح pleft  = rand_uniform(-dw, dw);
    صحیح pright = rand_uniform(-dw, dw);
    صحیح ptop   = rand_uniform(-dh, dh);
    صحیح pbot   = rand_uniform(-dh, dh);

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
    d.X.vals[0] = sized.data;

    fill_truth_swag(random_path, d.y.vals[0], classes, flip, dx, dy, 1./sx, 1./sy);

    free_image(orig);
    free_image(cropped);

    بازگشت d;
}

data load_data_detection(صحیح n, نویسه **paths, صحیح m, صحیح w, صحیح h, صحیح boxes, صحیح classes, اعشاری jitter, اعشاری hue, اعشاری saturation, اعشاری exposure)
{
    نویسه **random_paths = get_random_paths(paths, n, m);
    صحیح i;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, اندازه(اعشاری*));
    d.X.cols = h*w*3;

    d.y = make_matrix(n, 5*boxes);
    برای(i = 0; i < n; ++i){
        image orig = load_image_color(random_paths[i], 0, 0);
        image sized = make_image(w, h, orig.c);
        fill_image(sized, .5);

        اعشاری dw = jitter * orig.w;
        اعشاری dh = jitter * orig.h;

        اعشاری new_ar = (orig.w + rand_uniform(-dw, dw)) / (orig.h + rand_uniform(-dh, dh));
        //float scale = rand_uniform(.25, 2);
        اعشاری scale = 1;

        اعشاری nw, nh;

        اگر(new_ar < 1){
            nh = scale * h;
            nw = nh * new_ar;
        } وگرنه {
            nw = scale * w;
            nh = nw / new_ar;
        }

        اعشاری dx = rand_uniform(0, w - nw);
        اعشاری dy = rand_uniform(0, h - nh);

        place_image(orig, nw, nh, dx, dy, sized);

        random_distort_image(sized, hue, saturation, exposure);

        صحیح flip = rand()%2;
        اگر(flip) flip_image(sized);
        d.X.vals[i] = sized.data;


        fill_truth_detection(random_paths[i], boxes, d.y.vals[i], classes, flip, -dx/w, -dy/h, nw/w, nh/h);

        free_image(orig);
    }
    آزاد(random_paths);
    بازگشت d;
}

تهی *load_thread(تهی *ptr)
{
    //printf("Loading data: %d\n", rand());
    load_args a = *(ساختار load_args*)ptr;
    اگر(a.exposure == 0) a.exposure = 1;
    اگر(a.saturation == 0) a.saturation = 1;
    اگر(a.aspect == 0) a.aspect = 1;

    اگر (a.type == OLD_CLASSIFICATION_DATA){
        *a.d = load_data_old(a.paths, a.n, a.m, a.labels, a.classes, a.w, a.h);
    } وگرنه اگر (a.type == REGRESSION_DATA){
        *a.d = load_data_regression(a.paths, a.n, a.m, a.classes, a.min, a.max, a.size, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    } وگرنه اگر (a.type == CLASSIFICATION_DATA){
        *a.d = load_data_augment(a.paths, a.n, a.m, a.labels, a.classes, a.hierarchy, a.min, a.max, a.size, a.angle, a.aspect, a.hue, a.saturation, a.exposure, a.center);
    } وگرنه اگر (a.type == SUPER_DATA){
        *a.d = load_data_super(a.paths, a.n, a.m, a.w, a.h, a.scale);
    } وگرنه اگر (a.type == WRITING_DATA){
        *a.d = load_data_writing(a.paths, a.n, a.m, a.w, a.h, a.out_w, a.out_h);
    } وگرنه اگر (a.type == ISEG_DATA){
        *a.d = load_data_iseg(a.n, a.paths, a.m, a.w, a.h, a.classes, a.رقم_boxes, a.scale, a.min, a.max, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    } وگرنه اگر (a.type == INSTANCE_DATA){
        *a.d = load_data_mask(a.n, a.paths, a.m, a.w, a.h, a.classes, a.رقم_boxes, a.coords, a.min, a.max, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    } وگرنه اگر (a.type == SEGMENTATION_DATA){
        *a.d = load_data_seg(a.n, a.paths, a.m, a.w, a.h, a.classes, a.min, a.max, a.angle, a.aspect, a.hue, a.saturation, a.exposure, a.scale);
    } وگرنه اگر (a.type == REGION_DATA){
        *a.d = load_data_region(a.n, a.paths, a.m, a.w, a.h, a.رقم_boxes, a.classes, a.jitter, a.hue, a.saturation, a.exposure);
    } وگرنه اگر (a.type == DETECTION_DATA){
        *a.d = load_data_detection(a.n, a.paths, a.m, a.w, a.h, a.رقم_boxes, a.classes, a.jitter, a.hue, a.saturation, a.exposure);
    } وگرنه اگر (a.type == SWAG_DATA){
        *a.d = load_data_swag(a.paths, a.n, a.classes, a.jitter);
    } وگرنه اگر (a.type == COMPARE_DATA){
        *a.d = load_data_compare(a.n, a.paths, a.m, a.classes, a.w, a.h);
    } وگرنه اگر (a.type == IMAGE_DATA){
        *(a.im) = load_image_color(a.path, 0, 0);
        *(a.resized) = resize_image(*(a.im), a.w, a.h);
    } وگرنه اگر (a.type == LETTERBOX_DATA){
        *(a.im) = load_image_color(a.path, 0, 0);
        *(a.resized) = letterbox_image(*(a.im), a.w, a.h);
    } وگرنه اگر (a.type == TAG_DATA){
        *a.d = load_data_tag(a.paths, a.n, a.m, a.classes, a.min, a.max, a.size, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    }
    آزاد(ptr);
    بازگشت 0;
}

pthread_t load_data_in_thread(load_args args)
{
    pthread_t thread;
    ساختار load_args *ptr = calloc(1, اندازه(ساختار load_args));
    *ptr = args;
    اگر(pthread_create(&thread, 0, load_thread, ptr)) error("Thread creation failed");
    بازگشت thread;
}

تهی *load_threads(تهی *ptr)
{
    صحیح i;
    load_args args = *(load_args *)ptr;
    اگر (args.threads == 0) args.threads = 1;
    data *out = args.d;
    صحیح total = args.n;
    آزاد(ptr);
    data *buffers = calloc(args.threads, اندازه(data));
    pthread_t *threads = calloc(args.threads, اندازه(pthread_t));
    برای(i = 0; i < args.threads; ++i){
        args.d = buffers + i;
        args.n = (i+1) * total/args.threads - i * total/args.threads;
        threads[i] = load_data_in_thread(args);
    }
    برای(i = 0; i < args.threads; ++i){
        pthread_join(threads[i], 0);
    }
    *out = concat_datas(buffers, args.threads);
    out->shallow = 0;
    برای(i = 0; i < args.threads; ++i){
        buffers[i].shallow = 1;
        free_data(buffers[i]);
    }
    آزاد(buffers);
    آزاد(threads);
    بازگشت 0;
}

تهی load_data_blocking(load_args args)
{
    ساختار load_args *ptr = calloc(1, اندازه(ساختار load_args));
    *ptr = args;
    load_thread(ptr);
}

pthread_t load_data(load_args args)
{
    pthread_t thread;
    ساختار load_args *ptr = calloc(1, اندازه(ساختار load_args));
    *ptr = args;
    اگر(pthread_create(&thread, 0, load_threads, ptr)) error("Thread creation failed");
    بازگشت thread;
}

data load_data_writing(نویسه **paths, صحیح n, صحیح m, صحیح w, صحیح h, صحیح out_w, صحیح out_h)
{
    اگر(m) paths = get_random_paths(paths, n, m);
    نویسه **replace_paths = find_replace_paths(paths, n, ".png", "-label.png");
    data d = {0};
    d.shallow = 0;
    d.X = load_image_paths(paths, n, w, h);
    d.y = load_image_paths_gray(replace_paths, n, out_w, out_h);
    اگر(m) آزاد(paths);
    صحیح i;
    برای(i = 0; i < n; ++i) آزاد(replace_paths[i]);
    آزاد(replace_paths);
    بازگشت d;
}

data load_data_old(نویسه **paths, صحیح n, صحیح m, نویسه **labels, صحیح k, صحیح w, صحیح h)
{
    اگر(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;
    d.X = load_image_paths(paths, n, w, h);
    d.y = load_labels_paths(paths, n, labels, k, 0);
    اگر(m) آزاد(paths);
    بازگشت d;
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

data load_data_super(نویسه **paths, صحیح n, صحیح m, صحیح w, صحیح h, صحیح scale)
{
    اگر(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;

    صحیح i;
    d.X.rows = n;
    d.X.vals = calloc(n, اندازه(اعشاری*));
    d.X.cols = w*h*3;

    d.y.rows = n;
    d.y.vals = calloc(n, اندازه(اعشاری*));
    d.y.cols = w*scale * h*scale * 3;

    برای(i = 0; i < n; ++i){
        image im = load_image_color(paths[i], 0, 0);
        image crop = random_crop_image(im, w*scale, h*scale);
        صحیح flip = rand()%2;
        اگر (flip) flip_image(crop);
        image resize = resize_image(crop, w, h);
        d.X.vals[i] = resize.data;
        d.y.vals[i] = crop.data;
        free_image(im);
    }

    اگر(m) آزاد(paths);
    بازگشت d;
}

data load_data_regression(نویسه **paths, صحیح n, صحیح m, صحیح k, صحیح min, صحیح max, صحیح size, اعشاری angle, اعشاری aspect, اعشاری hue, اعشاری saturation, اعشاری exposure)
{
    اگر(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;
    d.X = load_image_augment_paths(paths, n, min, max, size, angle, aspect, hue, saturation, exposure, 0);
    d.y = load_regression_labels_paths(paths, n, k);
    اگر(m) آزاد(paths);
    بازگشت d;
}

data select_data(data *orig, صحیح *inds)
{
    data d = {0};
    d.shallow = 1;
    d.w = orig[0].w;
    d.h = orig[0].h;

    d.X.rows = orig[0].X.rows;
    d.y.rows = orig[0].X.rows;

    d.X.cols = orig[0].X.cols;
    d.y.cols = orig[0].y.cols;

    d.X.vals = calloc(orig[0].X.rows, اندازه(اعشاری *));
    d.y.vals = calloc(orig[0].y.rows, اندازه(اعشاری *));
    صحیح i;
    برای(i = 0; i < d.X.rows; ++i){
        d.X.vals[i] = orig[inds[i]].X.vals[i];
        d.y.vals[i] = orig[inds[i]].y.vals[i];
    }
    بازگشت d;
}

data *tile_data(data orig, صحیح divs, صحیح size)
{
    data *ds = calloc(divs*divs, اندازه(data));
    صحیح i, j;
#pragma omp parallel برای
    برای(i = 0; i < divs*divs; ++i){
        data d;
        d.shallow = 0;
        d.w = orig.w/divs * size;
        d.h = orig.h/divs * size;
        d.X.rows = orig.X.rows;
        d.X.cols = d.w*d.h*3;
        d.X.vals = calloc(d.X.rows, اندازه(اعشاری*));

        d.y = copy_matrix(orig.y);
#pragma omp parallel برای
        برای(j = 0; j < orig.X.rows; ++j){
            صحیح x = (i%divs) * orig.w / divs - (d.w - orig.w/divs)/2;
            صحیح y = (i/divs) * orig.h / divs - (d.h - orig.h/divs)/2;
            image im = float_to_image(orig.w, orig.h, 3, orig.X.vals[j]);
            d.X.vals[j] = crop_image(im, x, y, d.w, d.h).data;
        }
        ds[i] = d;
    }
    بازگشت ds;
}

data resize_data(data orig, صحیح w, صحیح h)
{
    data d = {0};
    d.shallow = 0;
    d.w = w;
    d.h = h;
    صحیح i;
    d.X.rows = orig.X.rows;
    d.X.cols = w*h*3;
    d.X.vals = calloc(d.X.rows, اندازه(اعشاری*));

    d.y = copy_matrix(orig.y);
#pragma omp parallel برای
    برای(i = 0; i < orig.X.rows; ++i){
        image im = float_to_image(orig.w, orig.h, 3, orig.X.vals[i]);
        d.X.vals[i] = resize_image(im, w, h).data;
    }
    بازگشت d;
}

data load_data_augment(نویسه **paths, صحیح n, صحیح m, نویسه **labels, صحیح k, درخت *hierarchy, صحیح min, صحیح max, صحیح size, اعشاری angle, اعشاری aspect, اعشاری hue, اعشاری saturation, اعشاری exposure, صحیح center)
{
    اگر(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;
    d.w=size;
    d.h=size;
    d.X = load_image_augment_paths(paths, n, min, max, size, angle, aspect, hue, saturation, exposure, center);
    d.y = load_labels_paths(paths, n, labels, k, hierarchy);
    اگر(m) آزاد(paths);
    بازگشت d;
}

data load_data_tag(نویسه **paths, صحیح n, صحیح m, صحیح k, صحیح min, صحیح max, صحیح size, اعشاری angle, اعشاری aspect, اعشاری hue, اعشاری saturation, اعشاری exposure)
{
    اگر(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.w = size;
    d.h = size;
    d.shallow = 0;
    d.X = load_image_augment_paths(paths, n, min, max, size, angle, aspect, hue, saturation, exposure, 0);
    d.y = load_tags_paths(paths, n, k);
    اگر(m) آزاد(paths);
    بازگشت d;
}

matrix concat_matrix(matrix m1, matrix m2)
{
    صحیح i, شمار = 0;
    matrix m;
    m.cols = m1.cols;
    m.rows = m1.rows+m2.rows;
    m.vals = calloc(m1.rows + m2.rows, اندازه(اعشاری*));
    برای(i = 0; i < m1.rows; ++i){
        m.vals[شمار++] = m1.vals[i];
    }
    برای(i = 0; i < m2.rows; ++i){
        m.vals[شمار++] = m2.vals[i];
    }
    بازگشت m;
}

data concat_data(data d1, data d2)
{
    data d = {0};
    d.shallow = 1;
    d.X = concat_matrix(d1.X, d2.X);
    d.y = concat_matrix(d1.y, d2.y);
    d.w = d1.w;
    d.h = d1.h;
    بازگشت d;
}

data concat_datas(data *d, صحیح n)
{
    صحیح i;
    data out = {0};
    برای(i = 0; i < n; ++i){
        data new = concat_data(d[i], out);
        free_data(out);
        out = new;
    }
    بازگشت out;
}

data load_categorical_data_csv(نویسه *filename, صحیح target, صحیح k)
{
    data d = {0};
    d.shallow = 0;
    matrix X = csv_to_matrix(filename);
    اعشاری *truth_1d = برداشت_column(&X, target);
    اعشاری **truth = one_hot_encode(truth_1d, X.rows, k);
    matrix y;
    y.rows = X.rows;
    y.cols = k;
    y.vals = truth;
    d.X = X;
    d.y = y;
    آزاد(truth_1d);
    بازگشت d;
}

data load_cifar10_data(نویسه *filename)
{
    data d = {0};
    d.shallow = 0;
    long i,j;
    matrix X = make_matrix(10000, 3072);
    matrix y = make_matrix(10000, 10);
    d.X = X;
    d.y = y;

    FILE *fp = fopen(filename, "rb");
    اگر(!fp) file_error(filename);
    برای(i = 0; i < 10000; ++i){
        unsigned نویسه bytes[3073];
        fread(bytes, 1, 3073, fp);
        صحیح class = bytes[0];
        y.vals[i][class] = 1;
        برای(j = 0; j < X.cols; ++j){
            X.vals[i][j] = (دوگانه)bytes[j+1];
        }
    }
    scale_data_rows(d, 1./255);
    //normalize_data_rows(d);
    fclose(fp);
    بازگشت d;
}

تهی get_random_batch(data d, صحیح n, اعشاری *X, اعشاری *y)
{
    صحیح j;
    برای(j = 0; j < n; ++j){
        صحیح index = rand()%d.X.rows;
        memcpy(X+j*d.X.cols, d.X.vals[index], d.X.cols*اندازه(اعشاری));
        memcpy(y+j*d.y.cols, d.y.vals[index], d.y.cols*اندازه(اعشاری));
    }
}

تهی get_next_batch(data d, صحیح n, صحیح offset, اعشاری *X, اعشاری *y)
{
    صحیح j;
    برای(j = 0; j < n; ++j){
        صحیح index = offset + j;
        memcpy(X+j*d.X.cols, d.X.vals[index], d.X.cols*اندازه(اعشاری));
        اگر(y) memcpy(y+j*d.y.cols, d.y.vals[index], d.y.cols*اندازه(اعشاری));
    }
}

تهی smooth_data(data d)
{
    صحیح i, j;
    اعشاری scale = 1. / d.y.cols;
    اعشاری eps = .1;
    برای(i = 0; i < d.y.rows; ++i){
        برای(j = 0; j < d.y.cols; ++j){
            d.y.vals[i][j] = eps * scale + (1-eps) * d.y.vals[i][j];
        }
    }
}

data load_all_cifar10()
{
    data d = {0};
    d.shallow = 0;
    صحیح i,j,b;
    matrix X = make_matrix(50000, 3072);
    matrix y = make_matrix(50000, 10);
    d.X = X;
    d.y = y;


    برای(b = 0; b < 5; ++b){
        نویسه buff[256];
        sprintf(buff, "data/cifar/cifar-10-batches-bin/data_batch_%d.bin", b+1);
        FILE *fp = fopen(buff, "rb");
        اگر(!fp) file_error(buff);
        برای(i = 0; i < 10000; ++i){
            unsigned نویسه bytes[3073];
            fread(bytes, 1, 3073, fp);
            صحیح class = bytes[0];
            y.vals[i+b*10000][class] = 1;
            برای(j = 0; j < X.cols; ++j){
                X.vals[i+b*10000][j] = (دوگانه)bytes[j+1];
            }
        }
        fclose(fp);
    }
    //normalize_data_rows(d);
    scale_data_rows(d, 1./255);
    smooth_data(d);
    بازگشت d;
}

data load_go(نویسه *filename)
{
    FILE *fp = fopen(filename, "rb");
    matrix X = make_matrix(3363059, 361);
    matrix y = make_matrix(3363059, 361);
    صحیح row, col;

    اگر(!fp) file_error(filename);
    نویسه *label;
    صحیح شمار = 0;
    وقتی((label = fgetl(fp))){
        صحیح i;
        اگر(شمار == X.rows){
            X = resize_matrix(X, شمار*2);
            y = resize_matrix(y, شمار*2);
        }
        sscanf(label, "%d %d", &row, &col);
        نویسه *board = fgetl(fp);

        صحیح index = row*19 + col;
        y.vals[شمار][index] = 1;

        برای(i = 0; i < 19*19; ++i){
            اعشاری val = 0;
            اگر(board[i] == '1') val = 1;
            وگرنه اگر(board[i] == '2') val = -1;
            X.vals[شمار][i] = val;
        }
        ++شمار;
        آزاد(label);
        آزاد(board);
    }
    X = resize_matrix(X, شمار);
    y = resize_matrix(y, شمار);

    data d = {0};
    d.shallow = 0;
    d.X = X;
    d.y = y;


    fclose(fp);

    بازگشت d;
}


تهی randomize_data(data d)
{
    صحیح i;
    برای(i = d.X.rows-1; i > 0; --i){
        صحیح index = rand()%i;
        اعشاری *swap = d.X.vals[index];
        d.X.vals[index] = d.X.vals[i];
        d.X.vals[i] = swap;

        swap = d.y.vals[index];
        d.y.vals[index] = d.y.vals[i];
        d.y.vals[i] = swap;
    }
}

تهی scale_data_rows(data d, اعشاری s)
{
    صحیح i;
    برای(i = 0; i < d.X.rows; ++i){
        scale_array(d.X.vals[i], d.X.cols, s);
    }
}

تهی translate_data_rows(data d, اعشاری s)
{
    صحیح i;
    برای(i = 0; i < d.X.rows; ++i){
        translate_array(d.X.vals[i], d.X.cols, s);
    }
}

data copy_data(data d)
{
    data c = {0};
    c.w = d.w;
    c.h = d.h;
    c.shallow = 0;
    c.رقم_boxes = d.رقم_boxes;
    c.boxes = d.boxes;
    c.X = copy_matrix(d.X);
    c.y = copy_matrix(d.y);
    بازگشت c;
}

تهی normalize_data_rows(data d)
{
    صحیح i;
    برای(i = 0; i < d.X.rows; ++i){
        normalize_array(d.X.vals[i], d.X.cols);
    }
}

data get_data_part(data d, صحیح part, صحیح total)
{
    data p = {0};
    p.shallow = 1;
    p.X.rows = d.X.rows * (part + 1) / total - d.X.rows * part / total;
    p.y.rows = d.y.rows * (part + 1) / total - d.y.rows * part / total;
    p.X.cols = d.X.cols;
    p.y.cols = d.y.cols;
    p.X.vals = d.X.vals + d.X.rows * part / total;
    p.y.vals = d.y.vals + d.y.rows * part / total;
    بازگشت p;
}

data get_random_data(data d, صحیح رقم)
{
    data r = {0};
    r.shallow = 1;

    r.X.rows = رقم;
    r.y.rows = رقم;

    r.X.cols = d.X.cols;
    r.y.cols = d.y.cols;

    r.X.vals = calloc(رقم, اندازه(اعشاری *));
    r.y.vals = calloc(رقم, اندازه(اعشاری *));

    صحیح i;
    برای(i = 0; i < رقم; ++i){
        صحیح index = rand()%d.X.rows;
        r.X.vals[i] = d.X.vals[index];
        r.y.vals[i] = d.y.vals[index];
    }
    بازگشت r;
}

data *split_data(data d, صحیح part, صحیح total)
{
    data *split = calloc(2, اندازه(data));
    صحیح i;
    صحیح start = part*d.X.rows/total;
    صحیح end = (part+1)*d.X.rows/total;
    data train;
    data test;
    train.shallow = test.shallow = 1;

    test.X.rows = test.y.rows = end-start;
    train.X.rows = train.y.rows = d.X.rows - (end-start);
    train.X.cols = test.X.cols = d.X.cols;
    train.y.cols = test.y.cols = d.y.cols;

    train.X.vals = calloc(train.X.rows, اندازه(اعشاری*));
    test.X.vals = calloc(test.X.rows, اندازه(اعشاری*));
    train.y.vals = calloc(train.y.rows, اندازه(اعشاری*));
    test.y.vals = calloc(test.y.rows, اندازه(اعشاری*));

    برای(i = 0; i < start; ++i){
        train.X.vals[i] = d.X.vals[i];
        train.y.vals[i] = d.y.vals[i];
    }
    برای(i = start; i < end; ++i){
        test.X.vals[i-start] = d.X.vals[i];
        test.y.vals[i-start] = d.y.vals[i];
    }
    برای(i = end; i < d.X.rows; ++i){
        train.X.vals[i-(end-start)] = d.X.vals[i];
        train.y.vals[i-(end-start)] = d.y.vals[i];
    }
    split[0] = train;
    split[1] = test;
    بازگشت split;
}

