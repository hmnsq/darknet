#अन्तर्भाव "data.h"
#अन्तर्भाव "utils.h"
#अन्तर्भाव "image.h"
#अन्तर्भाव "cuda.h"

#अन्तर्भाव <stdio.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <string.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

list *get_paths(वर्ण *filename)
{
    वर्ण *path;
    FILE *file = fopen(filename, "r");
    जइ(!file) file_error(filename);
    list *पङ्क्तयः = make_list();
    जाव((path=fgetl(file))){
        list_निवेशय(पङ्क्तयः, path);
    }
    fclose(file);
    प्रत्यागम पङ्क्तयः;
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

वर्ण **get_random_paths(वर्ण **paths, पूर्णाङ्क n, पूर्णाङ्क m)
{
    वर्ण **random_paths = calloc(n, परिमाण(वर्ण*));
    पूर्णाङ्क i;
    pthread_mutex_lock(&mutex);
    प्रति(i = 0; i < n; ++i){
        पूर्णाङ्क index = rand()%m;
        random_paths[i] = paths[index];
        //if(i == 0) printf("%s\n", paths[index]);
    }
    pthread_mutex_unlock(&mutex);
    प्रत्यागम random_paths;
}

वर्ण **find_replace_paths(वर्ण **paths, पूर्णाङ्क n, वर्ण *find, वर्ण *replace)
{
    वर्ण **replace_paths = calloc(n, परिमाण(वर्ण*));
    पूर्णाङ्क i;
    प्रति(i = 0; i < n; ++i){
        वर्ण replaced[4096];
        find_replace(paths[i], find, replace, replaced);
        replace_paths[i] = copy_string(replaced);
    }
    प्रत्यागम replace_paths;
}

matrix load_image_paths_gray(वर्ण **paths, पूर्णाङ्क n, पूर्णाङ्क w, पूर्णाङ्क h)
{
    पूर्णाङ्क i;
    matrix X;
    X.rows = n;
    X.vals = calloc(X.rows, परिमाण(भिन्न*));
    X.cols = 0;

    प्रति(i = 0; i < n; ++i){
        image im = load_image(paths[i], w, h, 3);

        image gray = grayscale_image(im);
        free_image(im);
        im = gray;

        X.vals[i] = im.data;
        X.cols = im.h*im.w*im.c;
    }
    प्रत्यागम X;
}

matrix load_image_paths(वर्ण **paths, पूर्णाङ्क n, पूर्णाङ्क w, पूर्णाङ्क h)
{
    पूर्णाङ्क i;
    matrix X;
    X.rows = n;
    X.vals = calloc(X.rows, परिमाण(भिन्न*));
    X.cols = 0;

    प्रति(i = 0; i < n; ++i){
        image im = load_image_color(paths[i], w, h);
        X.vals[i] = im.data;
        X.cols = im.h*im.w*im.c;
    }
    प्रत्यागम X;
}

matrix load_image_augment_paths(वर्ण **paths, पूर्णाङ्क n, पूर्णाङ्क min, पूर्णाङ्क max, पूर्णाङ्क size, भिन्न angle, भिन्न aspect, भिन्न hue, भिन्न saturation, भिन्न exposure, पूर्णाङ्क center)
{
    पूर्णाङ्क i;
    matrix X;
    X.rows = n;
    X.vals = calloc(X.rows, परिमाण(भिन्न*));
    X.cols = 0;

    प्रति(i = 0; i < n; ++i){
        image im = load_image_color(paths[i], 0, 0);
        image crop;
        जइ(center){
            crop = center_crop_image(im, size, size);
        } अण्णहा {
            crop = random_augment_image(im, angle, aspect, min, max, size, size);
        }
        पूर्णाङ्क flip = rand()%2;
        जइ (flip) flip_image(crop);
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
    प्रत्यागम X;
}


box_label *read_boxes(वर्ण *filename, पूर्णाङ्क *n)
{
    FILE *file = fopen(filename, "r");
    जइ(!file) file_error(filename);
    भिन्न x, y, h, w;
    पूर्णाङ्क id;
    पूर्णाङ्क गणनम् = 0;
    पूर्णाङ्क size = 64;
    box_label *boxes = calloc(size, परिमाण(box_label));
    जाव(fscanf(file, "%d %f %f %f %f", &id, &x, &y, &w, &h) == 5){
        जइ(गणनम् == size) {
            size = size * 2;
            boxes = realloc(boxes, size*परिमाण(box_label));
        }
        boxes[गणनम्].id = id;
        boxes[गणनम्].x = x;
        boxes[गणनम्].y = y;
        boxes[गणनम्].h = h;
        boxes[गणनम्].w = w;
        boxes[गणनम्].वामः   = x - w/2;
        boxes[गणनम्].दक्षिणः  = x + w/2;
        boxes[गणनम्].शिखरम्    = y - h/2;
        boxes[गणनम्].bottom = y + h/2;
        ++गणनम्;
    }
    fclose(file);
    *n = गणनम्;
    प्रत्यागम boxes;
}

शून्य randomize_boxes(box_label *b, पूर्णाङ्क n)
{
    पूर्णाङ्क i;
    प्रति(i = 0; i < n; ++i){
        box_label swap = b[i];
        पूर्णाङ्क index = rand()%n;
        b[i] = b[index];
        b[index] = swap;
    }
}

शून्य correct_boxes(box_label *boxes, पूर्णाङ्क n, भिन्न dx, भिन्न dy, भिन्न sx, भिन्न sy, पूर्णाङ्क flip)
{
    पूर्णाङ्क i;
    प्रति(i = 0; i < n; ++i){
        जइ(boxes[i].x == 0 && boxes[i].y == 0) {
            boxes[i].x = 999999;
            boxes[i].y = 999999;
            boxes[i].w = 999999;
            boxes[i].h = 999999;
            अनुवर्त;
        }
        boxes[i].वामः   = boxes[i].वामः  * sx - dx;
        boxes[i].दक्षिणः  = boxes[i].दक्षिणः * sx - dx;
        boxes[i].शिखरम्    = boxes[i].शिखरम्   * sy - dy;
        boxes[i].bottom = boxes[i].bottom* sy - dy;

        जइ(flip){
            भिन्न swap = boxes[i].वामः;
            boxes[i].वामः = 1. - boxes[i].दक्षिणः;
            boxes[i].दक्षिणः = 1. - swap;
        }

        boxes[i].वामः =  constrain(0, 1, boxes[i].वामः);
        boxes[i].दक्षिणः = constrain(0, 1, boxes[i].दक्षिणः);
        boxes[i].शिखरम् =   constrain(0, 1, boxes[i].शिखरम्);
        boxes[i].bottom =   constrain(0, 1, boxes[i].bottom);

        boxes[i].x = (boxes[i].वामः+boxes[i].दक्षिणः)/2;
        boxes[i].y = (boxes[i].शिखरम्+boxes[i].bottom)/2;
        boxes[i].w = (boxes[i].दक्षिणः - boxes[i].वामः);
        boxes[i].h = (boxes[i].bottom - boxes[i].शिखरम्);

        boxes[i].w = constrain(0, 1, boxes[i].w);
        boxes[i].h = constrain(0, 1, boxes[i].h);
    }
}

शून्य fill_truth_swag(वर्ण *path, भिन्न *truth, पूर्णाङ्क classes, पूर्णाङ्क flip, भिन्न dx, भिन्न dy, भिन्न sx, भिन्न sy)
{
    वर्ण labelpath[4096];
    find_replace(path, "images", "labels", labelpath);
    find_replace(labelpath, "JPEGImages", "labels", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);

    पूर्णाङ्क गणनम् = 0;
    box_label *boxes = read_boxes(labelpath, &गणनम्);
    randomize_boxes(boxes, गणनम्);
    correct_boxes(boxes, गणनम्, dx, dy, sx, sy, flip);
    भिन्न x,y,w,h;
    पूर्णाङ्क id;
    पूर्णाङ्क i;

    प्रति (i = 0; i < गणनम् && i < 90; ++i) {
        x =  boxes[i].x;
        y =  boxes[i].y;
        w =  boxes[i].w;
        h =  boxes[i].h;
        id = boxes[i].id;

        जइ (w < .0 || h < .0) अनुवर्त;

        पूर्णाङ्क index = (4+classes) * i;

        truth[index++] = x;
        truth[index++] = y;
        truth[index++] = w;
        truth[index++] = h;

        जइ (id < classes) truth[index+id] = 1;
    }
    मुक्त(boxes);
}

शून्य fill_truth_region(वर्ण *path, भिन्न *truth, पूर्णाङ्क classes, पूर्णाङ्क अङ्कः_boxes, पूर्णाङ्क flip, भिन्न dx, भिन्न dy, भिन्न sx, भिन्न sy)
{
    वर्ण labelpath[4096];
    find_replace(path, "images", "labels", labelpath);
    find_replace(labelpath, "JPEGImages", "labels", labelpath);

    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".png", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    पूर्णाङ्क गणनम् = 0;
    box_label *boxes = read_boxes(labelpath, &गणनम्);
    randomize_boxes(boxes, गणनम्);
    correct_boxes(boxes, गणनम्, dx, dy, sx, sy, flip);
    भिन्न x,y,w,h;
    पूर्णाङ्क id;
    पूर्णाङ्क i;

    प्रति (i = 0; i < गणनम्; ++i) {
        x =  boxes[i].x;
        y =  boxes[i].y;
        w =  boxes[i].w;
        h =  boxes[i].h;
        id = boxes[i].id;

        जइ (w < .005 || h < .005) अनुवर्त;

        पूर्णाङ्क col = (पूर्णाङ्क)(x*अङ्कः_boxes);
        पूर्णाङ्क row = (पूर्णाङ्क)(y*अङ्कः_boxes);

        x = x*अङ्कः_boxes - col;
        y = y*अङ्कः_boxes - row;

        पूर्णाङ्क index = (col+row*अङ्कः_boxes)*(5+classes);
        जइ (truth[index]) अनुवर्त;
        truth[index++] = 1;

        जइ (id < classes) truth[index+id] = 1;
        index += classes;

        truth[index++] = x;
        truth[index++] = y;
        truth[index++] = w;
        truth[index++] = h;
    }
    मुक्त(boxes);
}

शून्य load_rle(image im, पूर्णाङ्क *rle, पूर्णाङ्क n)
{
    पूर्णाङ्क गणनम् = 0;
    पूर्णाङ्क curr = 0;
    पूर्णाङ्क i,j;
    प्रति(i = 0; i < n; ++i){
        प्रति(j = 0; j < rle[i]; ++j){
            im.data[गणनम्++] = curr;
        }
        curr = 1 - curr;
    }
    प्रति(; गणनम् < im.h*im.w*im.c; ++गणनम्){
        im.data[गणनम्] = curr;
    }
}

शून्य or_image(image src, image dest, पूर्णाङ्क c)
{
    पूर्णाङ्क i;
    प्रति(i = 0; i < src.w*src.h; ++i){
        जइ(src.data[i]) dest.data[dest.w*dest.h*c + i] = 1;
    }
}

शून्य exclusive_image(image src)
{
    पूर्णाङ्क k, j, i;
    पूर्णाङ्क s = src.w*src.h;
    प्रति(k = 0; k < src.c-1; ++k){
        प्रति(i = 0; i < s; ++i){
            जइ (src.data[k*s + i]){
                प्रति(j = k+1; j < src.c; ++j){
                    src.data[j*s + i] = 0;
                }
            }
        }
    }
}

box bound_image(image im)
{
    पूर्णाङ्क x,y;
    पूर्णाङ्क minx = im.w;
    पूर्णाङ्क miny = im.h;
    पूर्णाङ्क maxx = 0;
    पूर्णाङ्क maxy = 0;
    प्रति(y = 0; y < im.h; ++y){
        प्रति(x = 0; x < im.w; ++x){
            जइ(im.data[y*im.w + x]){
                minx = (x < minx) ? x : minx;
                miny = (y < miny) ? y : miny;
                maxx = (x > maxx) ? x : maxx;
                maxy = (y > maxy) ? y : maxy;
            }
        }
    }
    box b = {minx, miny, maxx-minx + 1, maxy-miny + 1};
    //printf("%f %f %f %f\n", b.x, b.y, b.w, b.h);
    प्रत्यागम b;
}

शून्य fill_truth_iseg(वर्ण *path, पूर्णाङ्क अङ्कः_boxes, भिन्न *truth, पूर्णाङ्क classes, पूर्णाङ्क w, पूर्णाङ्क h, augment_args aug, पूर्णाङ्क flip, पूर्णाङ्क mw, पूर्णाङ्क mh)
{
    वर्ण labelpath[4096];
    find_replace(path, "images", "mask", labelpath);
    find_replace(labelpath, "JPEGImages", "mask", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    FILE *file = fopen(labelpath, "r");
    जइ(!file) file_error(labelpath);
    वर्ण buff[32788];
    पूर्णाङ्क id;
    पूर्णाङ्क i = 0;
    पूर्णाङ्क j;
    image part = make_image(w, h, 1);
    जाव((fscanf(file, "%d %s", &id, buff) == 2) && i < अङ्कः_boxes){
        पूर्णाङ्क n = 0;
        पूर्णाङ्क *rle = read_intlist(buff, &n, 0);
        load_rle(part, rle, n);
        image sized = rotate_crop_image(part, aug.rad, aug.scale, aug.w, aug.h, aug.dx, aug.dy, aug.aspect);
        जइ(flip) flip_image(sized);

        image mask = resize_image(sized, mw, mh);
        truth[i*(mw*mh+1)] = id;
        प्रति(j = 0; j < mw*mh; ++j){
            truth[i*(mw*mh + 1) + 1 + j] = mask.data[j];
        }
        ++i;

        free_image(mask);
        free_image(sized);
        मुक्त(rle);
    }
    जइ(i < अङ्कः_boxes) truth[i*(mw*mh+1)] = -1;
    fclose(file);
    free_image(part);
}

शून्य fill_truth_mask(वर्ण *path, पूर्णाङ्क अङ्कः_boxes, भिन्न *truth, पूर्णाङ्क classes, पूर्णाङ्क w, पूर्णाङ्क h, augment_args aug, पूर्णाङ्क flip, पूर्णाङ्क mw, पूर्णाङ्क mh)
{
    वर्ण labelpath[4096];
    find_replace(path, "images", "mask", labelpath);
    find_replace(labelpath, "JPEGImages", "mask", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    FILE *file = fopen(labelpath, "r");
    जइ(!file) file_error(labelpath);
    वर्ण buff[32788];
    पूर्णाङ्क id;
    पूर्णाङ्क i = 0;
    image part = make_image(w, h, 1);
    जाव((fscanf(file, "%d %s", &id, buff) == 2) && i < अङ्कः_boxes){
        पूर्णाङ्क n = 0;
        पूर्णाङ्क *rle = read_intlist(buff, &n, 0);
        load_rle(part, rle, n);
        image sized = rotate_crop_image(part, aug.rad, aug.scale, aug.w, aug.h, aug.dx, aug.dy, aug.aspect);
        जइ(flip) flip_image(sized);
        box b = bound_image(sized);
        जइ(b.w > 0){
            image crop = crop_image(sized, b.x, b.y, b.w, b.h);
            image mask = resize_image(crop, mw, mh);
            truth[i*(4 + mw*mh + 1) + 0] = (b.x + b.w/2.)/sized.w;
            truth[i*(4 + mw*mh + 1) + 1] = (b.y + b.h/2.)/sized.h;
            truth[i*(4 + mw*mh + 1) + 2] = b.w/sized.w;
            truth[i*(4 + mw*mh + 1) + 3] = b.h/sized.h;
            पूर्णाङ्क j;
            प्रति(j = 0; j < mw*mh; ++j){
                truth[i*(4 + mw*mh + 1) + 4 + j] = mask.data[j];
            }
            truth[i*(4 + mw*mh + 1) + 4 + mw*mh] = id;
            free_image(crop);
            free_image(mask);
            ++i;
        }
        free_image(sized);
        मुक्त(rle);
    }
    fclose(file);
    free_image(part);
}


शून्य fill_truth_detection(वर्ण *path, पूर्णाङ्क अङ्कः_boxes, भिन्न *truth, पूर्णाङ्क classes, पूर्णाङ्क flip, भिन्न dx, भिन्न dy, भिन्न sx, भिन्न sy)
{
    वर्ण labelpath[4096];
    find_replace(path, "images", "labels", labelpath);
    find_replace(labelpath, "JPEGImages", "labels", labelpath);

    find_replace(labelpath, "raw", "labels", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".png", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    पूर्णाङ्क गणनम् = 0;
    box_label *boxes = read_boxes(labelpath, &गणनम्);
    randomize_boxes(boxes, गणनम्);
    correct_boxes(boxes, गणनम्, dx, dy, sx, sy, flip);
    जइ(गणनम् > अङ्कः_boxes) गणनम् = अङ्कः_boxes;
    भिन्न x,y,w,h;
    पूर्णाङ्क id;
    पूर्णाङ्क i;
    पूर्णाङ्क sub = 0;

    प्रति (i = 0; i < गणनम्; ++i) {
        x =  boxes[i].x;
        y =  boxes[i].y;
        w =  boxes[i].w;
        h =  boxes[i].h;
        id = boxes[i].id;

        जइ ((w < .001 || h < .001)) {
            ++sub;
            अनुवर्त;
        }

        truth[(i-sub)*5+0] = x;
        truth[(i-sub)*5+1] = y;
        truth[(i-sub)*5+2] = w;
        truth[(i-sub)*5+3] = h;
        truth[(i-sub)*5+4] = id;
    }
    मुक्त(boxes);
}

#define NUMCHARS 37

शून्य print_letters(भिन्न *pred, पूर्णाङ्क n)
{
    पूर्णाङ्क i;
    प्रति(i = 0; i < n; ++i){
        पूर्णाङ्क index = max_index(pred+i*NUMCHARS, NUMCHARS);
        printf("%c", int_to_alphanum(index));
    }
    printf("\n");
}

शून्य fill_truth_captcha(वर्ण *path, पूर्णाङ्क n, भिन्न *truth)
{
    वर्ण *begin = strrchr(path, '/');
    ++begin;
    पूर्णाङ्क i;
    प्रति(i = 0; i < strlen(begin) && i < n && begin[i] != '.'; ++i){
        पूर्णाङ्क index = alphanum_to_int(begin[i]);
        जइ(index > 35) printf("Bad %c\n", begin[i]);
        truth[i*NUMCHARS+index] = 1;
    }
    प्रति(;i < n; ++i){
        truth[i*NUMCHARS + NUMCHARS-1] = 1;
    }
}

data load_data_captcha(वर्ण **paths, पूर्णाङ्क n, पूर्णाङ्क m, पूर्णाङ्क k, पूर्णाङ्क w, पूर्णाङ्क h)
{
    जइ(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;
    d.X = load_image_paths(paths, n, w, h);
    d.y = make_matrix(n, k*NUMCHARS);
    पूर्णाङ्क i;
    प्रति(i = 0; i < n; ++i){
        fill_truth_captcha(paths[i], k, d.y.vals[i]);
    }
    जइ(m) मुक्त(paths);
    प्रत्यागम d;
}

data load_data_captcha_encode(वर्ण **paths, पूर्णाङ्क n, पूर्णाङ्क m, पूर्णाङ्क w, पूर्णाङ्क h)
{
    जइ(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;
    d.X = load_image_paths(paths, n, w, h);
    d.X.cols = 17100;
    d.y = d.X;
    जइ(m) मुक्त(paths);
    प्रत्यागम d;
}

शून्य fill_truth(वर्ण *path, वर्ण **labels, पूर्णाङ्क k, भिन्न *truth)
{
    पूर्णाङ्क i;
    memset(truth, 0, k*परिमाण(भिन्न));
    पूर्णाङ्क गणनम् = 0;
    प्रति(i = 0; i < k; ++i){
        जइ(strstr(path, labels[i])){
            truth[i] = 1;
            ++गणनम्;
            //printf("%s %s %d\n", path, labels[i], i);
        }
    }
    जइ(गणनम् != 1 && (k != 1 || गणनम् != 0)) printf("Too many or too few labels: %d, %s\n", गणनम्, path);
}

शून्य fill_hierarchy(भिन्न *truth, पूर्णाङ्क k, वृक्षः *hierarchy)
{
    पूर्णाङ्क j;
    प्रति(j = 0; j < k; ++j){
        जइ(truth[j]){
            पूर्णाङ्क parent = hierarchy->parent[j];
            जाव(parent >= 0){
                truth[parent] = 1;
                parent = hierarchy->parent[parent];
            }
        }
    }
    पूर्णाङ्क i;
    पूर्णाङ्क गणनम् = 0;
    प्रति(j = 0; j < hierarchy->groups; ++j){
        //printf("%d\n", count);
        पूर्णाङ्क mask = 1;
        प्रति(i = 0; i < hierarchy->group_size[j]; ++i){
            जइ(truth[गणनम् + i]){
                mask = 0;
                भङ्ग;
            }
        }
        जइ (mask) {
            प्रति(i = 0; i < hierarchy->group_size[j]; ++i){
                truth[गणनम् + i] = SECRET_अङ्कः;
            }
        }
        गणनम् += hierarchy->group_size[j];
    }
}

matrix load_regression_labels_paths(वर्ण **paths, पूर्णाङ्क n, पूर्णाङ्क k)
{
    matrix y = make_matrix(n, k);
    पूर्णाङ्क i,j;
    प्रति(i = 0; i < n; ++i){
        वर्ण labelpath[4096];
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
        प्रति(j = 0; j < k; ++j){
            fscanf(file, "%f", &(y.vals[i][j]));
        }
        fclose(file);
    }
    प्रत्यागम y;
}

matrix load_labels_paths(वर्ण **paths, पूर्णाङ्क n, वर्ण **labels, पूर्णाङ्क k, वृक्षः *hierarchy)
{
    matrix y = make_matrix(n, k);
    पूर्णाङ्क i;
    प्रति(i = 0; i < n && labels; ++i){
        fill_truth(paths[i], labels, k, y.vals[i]);
        जइ(hierarchy){
            fill_hierarchy(y.vals[i], k, hierarchy);
        }
    }
    प्रत्यागम y;
}

matrix load_tags_paths(वर्ण **paths, पूर्णाङ्क n, पूर्णाङ्क k)
{
    matrix y = make_matrix(n, k);
    पूर्णाङ्क i;
    //int count = 0;
    प्रति(i = 0; i < n; ++i){
        वर्ण label[4096];
        find_replace(paths[i], "images", "labels", label);
        find_replace(label, ".jpg", ".txt", label);
        FILE *file = fopen(label, "r");
        जइ (!file) अनुवर्त;
        //++count;
        पूर्णाङ्क tag;
        जाव(fscanf(file, "%d", &tag) == 1){
            जइ(tag < k){
                y.vals[i][tag] = 1;
            }
        }
        fclose(file);
    }
    //printf("%d/%d\n", count, n);
    प्रत्यागम y;
}

वर्ण **get_labels(वर्ण *filename)
{
    list *plist = get_paths(filename);
    वर्ण **labels = (वर्ण **)list_to_array(plist);
    free_list(plist);
    प्रत्यागम labels;
}

शून्य free_data(data d)
{
    जइ(!d.shallow){
        free_matrix(d.X);
        free_matrix(d.y);
    }अण्णहा{
        मुक्त(d.X.vals);
        मुक्त(d.y.vals);
    }
}

image get_segmentation_image(वर्ण *path, पूर्णाङ्क w, पूर्णाङ्क h, पूर्णाङ्क classes)
{
    वर्ण labelpath[4096];
    find_replace(path, "images", "mask", labelpath);
    find_replace(labelpath, "JPEGImages", "mask", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    image mask = make_image(w, h, classes);
    FILE *file = fopen(labelpath, "r");
    जइ(!file) file_error(labelpath);
    वर्ण buff[32788];
    पूर्णाङ्क id;
    image part = make_image(w, h, 1);
    जाव(fscanf(file, "%d %s", &id, buff) == 2){
        पूर्णाङ्क n = 0;
        पूर्णाङ्क *rle = read_intlist(buff, &n, 0);
        load_rle(part, rle, n);
        or_image(part, mask, id);
        मुक्त(rle);
    }
    //exclusive_image(mask);
    fclose(file);
    free_image(part);
    प्रत्यागम mask;
}

image get_segmentation_image2(वर्ण *path, पूर्णाङ्क w, पूर्णाङ्क h, पूर्णाङ्क classes)
{
    वर्ण labelpath[4096];
    find_replace(path, "images", "mask", labelpath);
    find_replace(labelpath, "JPEGImages", "mask", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    image mask = make_image(w, h, classes+1);
    पूर्णाङ्क i;
    प्रति(i = 0; i < w*h; ++i){
        mask.data[w*h*classes + i] = 1;
    }
    FILE *file = fopen(labelpath, "r");
    जइ(!file) file_error(labelpath);
    वर्ण buff[32788];
    पूर्णाङ्क id;
    image part = make_image(w, h, 1);
    जाव(fscanf(file, "%d %s", &id, buff) == 2){
        पूर्णाङ्क n = 0;
        पूर्णाङ्क *rle = read_intlist(buff, &n, 0);
        load_rle(part, rle, n);
        or_image(part, mask, id);
        प्रति(i = 0; i < w*h; ++i){
            जइ(part.data[i]) mask.data[w*h*classes + i] = 0;
        }
        मुक्त(rle);
    }
    //exclusive_image(mask);
    fclose(file);
    free_image(part);
    प्रत्यागम mask;
}

data load_data_seg(पूर्णाङ्क n, वर्ण **paths, पूर्णाङ्क m, पूर्णाङ्क w, पूर्णाङ्क h, पूर्णाङ्क classes, पूर्णाङ्क min, पूर्णाङ्क max, भिन्न angle, भिन्न aspect, भिन्न hue, भिन्न saturation, भिन्न exposure, पूर्णाङ्क div)
{
    वर्ण **random_paths = get_random_paths(paths, n, m);
    पूर्णाङ्क i;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, परिमाण(भिन्न*));
    d.X.cols = h*w*3;


    d.y.rows = n;
    d.y.cols = h*w*classes/div/div;
    d.y.vals = calloc(d.X.rows, परिमाण(भिन्न*));

    प्रति(i = 0; i < n; ++i){
        image orig = load_image_color(random_paths[i], 0, 0);
        augment_args a = random_augment_args(orig, angle, aspect, min, max, w, h);
        image sized = rotate_crop_image(orig, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);

        पूर्णाङ्क flip = rand()%2;
        जइ(flip) flip_image(sized);
        random_distort_image(sized, hue, saturation, exposure);
        d.X.vals[i] = sized.data;

        image mask = get_segmentation_image(random_paths[i], orig.w, orig.h, classes);
        //image mask = make_image(orig.w, orig.h, classes+1);
        image sized_m = rotate_crop_image(mask, a.rad, a.scale/div, a.w/div, a.h/div, a.dx/div, a.dy/div, a.aspect);

        जइ(flip) flip_image(sized_m);
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
    मुक्त(random_paths);
    प्रत्यागम d;
}

data load_data_iseg(पूर्णाङ्क n, वर्ण **paths, पूर्णाङ्क m, पूर्णाङ्क w, पूर्णाङ्क h, पूर्णाङ्क classes, पूर्णाङ्क boxes, पूर्णाङ्क div, पूर्णाङ्क min, पूर्णाङ्क max, भिन्न angle, भिन्न aspect, भिन्न hue, भिन्न saturation, भिन्न exposure)
{
    वर्ण **random_paths = get_random_paths(paths, n, m);
    पूर्णाङ्क i;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, परिमाण(भिन्न*));
    d.X.cols = h*w*3;

    d.y = make_matrix(n, (((w/div)*(h/div))+1)*boxes);

    प्रति(i = 0; i < n; ++i){
        image orig = load_image_color(random_paths[i], 0, 0);
        augment_args a = random_augment_args(orig, angle, aspect, min, max, w, h);
        image sized = rotate_crop_image(orig, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);

        पूर्णाङ्क flip = rand()%2;
        जइ(flip) flip_image(sized);
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
    मुक्त(random_paths);
    प्रत्यागम d;
}

data load_data_mask(पूर्णाङ्क n, वर्ण **paths, पूर्णाङ्क m, पूर्णाङ्क w, पूर्णाङ्क h, पूर्णाङ्क classes, पूर्णाङ्क boxes, पूर्णाङ्क coords, पूर्णाङ्क min, पूर्णाङ्क max, भिन्न angle, भिन्न aspect, भिन्न hue, भिन्न saturation, भिन्न exposure)
{
    वर्ण **random_paths = get_random_paths(paths, n, m);
    पूर्णाङ्क i;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, परिमाण(भिन्न*));
    d.X.cols = h*w*3;

    d.y = make_matrix(n, (coords+1)*boxes);

    प्रति(i = 0; i < n; ++i){
        image orig = load_image_color(random_paths[i], 0, 0);
        augment_args a = random_augment_args(orig, angle, aspect, min, max, w, h);
        image sized = rotate_crop_image(orig, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);

        पूर्णाङ्क flip = rand()%2;
        जइ(flip) flip_image(sized);
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
    मुक्त(random_paths);
    प्रत्यागम d;
}

data load_data_region(पूर्णाङ्क n, वर्ण **paths, पूर्णाङ्क m, पूर्णाङ्क w, पूर्णाङ्क h, पूर्णाङ्क size, पूर्णाङ्क classes, भिन्न jitter, भिन्न hue, भिन्न saturation, भिन्न exposure)
{
    वर्ण **random_paths = get_random_paths(paths, n, m);
    पूर्णाङ्क i;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, परिमाण(भिन्न*));
    d.X.cols = h*w*3;


    पूर्णाङ्क k = size*size*(5+classes);
    d.y = make_matrix(n, k);
    प्रति(i = 0; i < n; ++i){
        image orig = load_image_color(random_paths[i], 0, 0);

        पूर्णाङ्क oh = orig.h;
        पूर्णाङ्क ow = orig.w;

        पूर्णाङ्क dw = (ow*jitter);
        पूर्णाङ्क dh = (oh*jitter);

        पूर्णाङ्क pleft  = rand_uniform(-dw, dw);
        पूर्णाङ्क pright = rand_uniform(-dw, dw);
        पूर्णाङ्क ptop   = rand_uniform(-dh, dh);
        पूर्णाङ्क pbot   = rand_uniform(-dh, dh);

        पूर्णाङ्क swidth =  ow - pleft - pright;
        पूर्णाङ्क sheight = oh - ptop - pbot;

        भिन्न sx = (भिन्न)swidth  / ow;
        भिन्न sy = (भिन्न)sheight / oh;

        पूर्णाङ्क flip = rand()%2;
        image cropped = crop_image(orig, pleft, ptop, swidth, sheight);

        भिन्न dx = ((भिन्न)pleft/ow)/sx;
        भिन्न dy = ((भिन्न)ptop /oh)/sy;

        image sized = resize_image(cropped, w, h);
        जइ(flip) flip_image(sized);
        random_distort_image(sized, hue, saturation, exposure);
        d.X.vals[i] = sized.data;

        fill_truth_region(random_paths[i], d.y.vals[i], classes, size, flip, dx, dy, 1./sx, 1./sy);

        free_image(orig);
        free_image(cropped);
    }
    मुक्त(random_paths);
    प्रत्यागम d;
}

data load_data_compare(पूर्णाङ्क n, वर्ण **paths, पूर्णाङ्क m, पूर्णाङ्क classes, पूर्णाङ्क w, पूर्णाङ्क h)
{
    जइ(m) paths = get_random_paths(paths, 2*n, m);
    पूर्णाङ्क i,j;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, परिमाण(भिन्न*));
    d.X.cols = h*w*6;

    पूर्णाङ्क k = 2*(classes);
    d.y = make_matrix(n, k);
    प्रति(i = 0; i < n; ++i){
        image im1 = load_image_color(paths[i*2],   w, h);
        image im2 = load_image_color(paths[i*2+1], w, h);

        d.X.vals[i] = calloc(d.X.cols, परिमाण(भिन्न));
        memcpy(d.X.vals[i],         im1.data, h*w*3*परिमाण(भिन्न));
        memcpy(d.X.vals[i] + h*w*3, im2.data, h*w*3*परिमाण(भिन्न));

        पूर्णाङ्क id;
        भिन्न iou;

        वर्ण imlabel1[4096];
        वर्ण imlabel2[4096];
        find_replace(paths[i*2],   "imgs", "labels", imlabel1);
        find_replace(imlabel1, "jpg", "txt", imlabel1);
        FILE *fp1 = fopen(imlabel1, "r");

        जाव(fscanf(fp1, "%d %f", &id, &iou) == 2){
            जइ (d.y.vals[i][2*id] < iou) d.y.vals[i][2*id] = iou;
        }

        find_replace(paths[i*2+1], "imgs", "labels", imlabel2);
        find_replace(imlabel2, "jpg", "txt", imlabel2);
        FILE *fp2 = fopen(imlabel2, "r");

        जाव(fscanf(fp2, "%d %f", &id, &iou) == 2){
            जइ (d.y.vals[i][2*id + 1] < iou) d.y.vals[i][2*id + 1] = iou;
        }

        प्रति (j = 0; j < classes; ++j){
            जइ (d.y.vals[i][2*j] > .5 &&  d.y.vals[i][2*j+1] < .5){
                d.y.vals[i][2*j] = 1;
                d.y.vals[i][2*j+1] = 0;
            } अण्णहा जइ (d.y.vals[i][2*j] < .5 &&  d.y.vals[i][2*j+1] > .5){
                d.y.vals[i][2*j] = 0;
                d.y.vals[i][2*j+1] = 1;
            } अण्णहा {
                d.y.vals[i][2*j]   = SECRET_अङ्कः;
                d.y.vals[i][2*j+1] = SECRET_अङ्कः;
            }
        }
        fclose(fp1);
        fclose(fp2);

        free_image(im1);
        free_image(im2);
    }
    जइ(m) मुक्त(paths);
    प्रत्यागम d;
}

data load_data_swag(वर्ण **paths, पूर्णाङ्क n, पूर्णाङ्क classes, भिन्न jitter)
{
    पूर्णाङ्क index = rand()%n;
    वर्ण *random_path = paths[index];

    image orig = load_image_color(random_path, 0, 0);
    पूर्णाङ्क h = orig.h;
    पूर्णाङ्क w = orig.w;

    data d = {0};
    d.shallow = 0;
    d.w = w;
    d.h = h;

    d.X.rows = 1;
    d.X.vals = calloc(d.X.rows, परिमाण(भिन्न*));
    d.X.cols = h*w*3;

    पूर्णाङ्क k = (4+classes)*90;
    d.y = make_matrix(1, k);

    पूर्णाङ्क dw = w*jitter;
    पूर्णाङ्क dh = h*jitter;

    पूर्णाङ्क pleft  = rand_uniform(-dw, dw);
    पूर्णाङ्क pright = rand_uniform(-dw, dw);
    पूर्णाङ्क ptop   = rand_uniform(-dh, dh);
    पूर्णाङ्क pbot   = rand_uniform(-dh, dh);

    पूर्णाङ्क swidth =  w - pleft - pright;
    पूर्णाङ्क sheight = h - ptop - pbot;

    भिन्न sx = (भिन्न)swidth  / w;
    भिन्न sy = (भिन्न)sheight / h;

    पूर्णाङ्क flip = rand()%2;
    image cropped = crop_image(orig, pleft, ptop, swidth, sheight);

    भिन्न dx = ((भिन्न)pleft/w)/sx;
    भिन्न dy = ((भिन्न)ptop /h)/sy;

    image sized = resize_image(cropped, w, h);
    जइ(flip) flip_image(sized);
    d.X.vals[0] = sized.data;

    fill_truth_swag(random_path, d.y.vals[0], classes, flip, dx, dy, 1./sx, 1./sy);

    free_image(orig);
    free_image(cropped);

    प्रत्यागम d;
}

data load_data_detection(पूर्णाङ्क n, वर्ण **paths, पूर्णाङ्क m, पूर्णाङ्क w, पूर्णाङ्क h, पूर्णाङ्क boxes, पूर्णाङ्क classes, भिन्न jitter, भिन्न hue, भिन्न saturation, भिन्न exposure)
{
    वर्ण **random_paths = get_random_paths(paths, n, m);
    पूर्णाङ्क i;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, परिमाण(भिन्न*));
    d.X.cols = h*w*3;

    d.y = make_matrix(n, 5*boxes);
    प्रति(i = 0; i < n; ++i){
        image orig = load_image_color(random_paths[i], 0, 0);
        image sized = make_image(w, h, orig.c);
        fill_image(sized, .5);

        भिन्न dw = jitter * orig.w;
        भिन्न dh = jitter * orig.h;

        भिन्न new_ar = (orig.w + rand_uniform(-dw, dw)) / (orig.h + rand_uniform(-dh, dh));
        //float scale = rand_uniform(.25, 2);
        भिन्न scale = 1;

        भिन्न nw, nh;

        जइ(new_ar < 1){
            nh = scale * h;
            nw = nh * new_ar;
        } अण्णहा {
            nw = scale * w;
            nh = nw / new_ar;
        }

        भिन्न dx = rand_uniform(0, w - nw);
        भिन्न dy = rand_uniform(0, h - nh);

        place_image(orig, nw, nh, dx, dy, sized);

        random_distort_image(sized, hue, saturation, exposure);

        पूर्णाङ्क flip = rand()%2;
        जइ(flip) flip_image(sized);
        d.X.vals[i] = sized.data;


        fill_truth_detection(random_paths[i], boxes, d.y.vals[i], classes, flip, -dx/w, -dy/h, nw/w, nh/h);

        free_image(orig);
    }
    मुक्त(random_paths);
    प्रत्यागम d;
}

शून्य *load_thread(शून्य *ptr)
{
    //printf("Loading data: %d\n", rand());
    load_args a = *(संरचना load_args*)ptr;
    जइ(a.exposure == 0) a.exposure = 1;
    जइ(a.saturation == 0) a.saturation = 1;
    जइ(a.aspect == 0) a.aspect = 1;

    जइ (a.type == OLD_CLASSIFICATION_DATA){
        *a.d = load_data_old(a.paths, a.n, a.m, a.labels, a.classes, a.w, a.h);
    } अण्णहा जइ (a.type == REGRESSION_DATA){
        *a.d = load_data_regression(a.paths, a.n, a.m, a.classes, a.min, a.max, a.size, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    } अण्णहा जइ (a.type == CLASSIFICATION_DATA){
        *a.d = load_data_augment(a.paths, a.n, a.m, a.labels, a.classes, a.hierarchy, a.min, a.max, a.size, a.angle, a.aspect, a.hue, a.saturation, a.exposure, a.center);
    } अण्णहा जइ (a.type == SUPER_DATA){
        *a.d = load_data_super(a.paths, a.n, a.m, a.w, a.h, a.scale);
    } अण्णहा जइ (a.type == WRITING_DATA){
        *a.d = load_data_writing(a.paths, a.n, a.m, a.w, a.h, a.out_w, a.out_h);
    } अण्णहा जइ (a.type == ISEG_DATA){
        *a.d = load_data_iseg(a.n, a.paths, a.m, a.w, a.h, a.classes, a.अङ्कः_boxes, a.scale, a.min, a.max, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    } अण्णहा जइ (a.type == INSTANCE_DATA){
        *a.d = load_data_mask(a.n, a.paths, a.m, a.w, a.h, a.classes, a.अङ्कः_boxes, a.coords, a.min, a.max, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    } अण्णहा जइ (a.type == SEGMENTATION_DATA){
        *a.d = load_data_seg(a.n, a.paths, a.m, a.w, a.h, a.classes, a.min, a.max, a.angle, a.aspect, a.hue, a.saturation, a.exposure, a.scale);
    } अण्णहा जइ (a.type == REGION_DATA){
        *a.d = load_data_region(a.n, a.paths, a.m, a.w, a.h, a.अङ्कः_boxes, a.classes, a.jitter, a.hue, a.saturation, a.exposure);
    } अण्णहा जइ (a.type == DETECTION_DATA){
        *a.d = load_data_detection(a.n, a.paths, a.m, a.w, a.h, a.अङ्कः_boxes, a.classes, a.jitter, a.hue, a.saturation, a.exposure);
    } अण्णहा जइ (a.type == SWAG_DATA){
        *a.d = load_data_swag(a.paths, a.n, a.classes, a.jitter);
    } अण्णहा जइ (a.type == COMPARE_DATA){
        *a.d = load_data_compare(a.n, a.paths, a.m, a.classes, a.w, a.h);
    } अण्णहा जइ (a.type == IMAGE_DATA){
        *(a.im) = load_image_color(a.path, 0, 0);
        *(a.resized) = resize_image(*(a.im), a.w, a.h);
    } अण्णहा जइ (a.type == LETTERBOX_DATA){
        *(a.im) = load_image_color(a.path, 0, 0);
        *(a.resized) = letterbox_image(*(a.im), a.w, a.h);
    } अण्णहा जइ (a.type == TAG_DATA){
        *a.d = load_data_tag(a.paths, a.n, a.m, a.classes, a.min, a.max, a.size, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    }
    मुक्त(ptr);
    प्रत्यागम 0;
}

pthread_t load_data_in_thread(load_args args)
{
    pthread_t thread;
    संरचना load_args *ptr = calloc(1, परिमाण(संरचना load_args));
    *ptr = args;
    जइ(pthread_create(&thread, 0, load_thread, ptr)) error("Thread creation failed");
    प्रत्यागम thread;
}

शून्य *load_threads(शून्य *ptr)
{
    पूर्णाङ्क i;
    load_args args = *(load_args *)ptr;
    जइ (args.threads == 0) args.threads = 1;
    data *out = args.d;
    पूर्णाङ्क total = args.n;
    मुक्त(ptr);
    data *buffers = calloc(args.threads, परिमाण(data));
    pthread_t *threads = calloc(args.threads, परिमाण(pthread_t));
    प्रति(i = 0; i < args.threads; ++i){
        args.d = buffers + i;
        args.n = (i+1) * total/args.threads - i * total/args.threads;
        threads[i] = load_data_in_thread(args);
    }
    प्रति(i = 0; i < args.threads; ++i){
        pthread_join(threads[i], 0);
    }
    *out = concat_datas(buffers, args.threads);
    out->shallow = 0;
    प्रति(i = 0; i < args.threads; ++i){
        buffers[i].shallow = 1;
        free_data(buffers[i]);
    }
    मुक्त(buffers);
    मुक्त(threads);
    प्रत्यागम 0;
}

शून्य load_data_blocking(load_args args)
{
    संरचना load_args *ptr = calloc(1, परिमाण(संरचना load_args));
    *ptr = args;
    load_thread(ptr);
}

pthread_t load_data(load_args args)
{
    pthread_t thread;
    संरचना load_args *ptr = calloc(1, परिमाण(संरचना load_args));
    *ptr = args;
    जइ(pthread_create(&thread, 0, load_threads, ptr)) error("Thread creation failed");
    प्रत्यागम thread;
}

data load_data_writing(वर्ण **paths, पूर्णाङ्क n, पूर्णाङ्क m, पूर्णाङ्क w, पूर्णाङ्क h, पूर्णाङ्क out_w, पूर्णाङ्क out_h)
{
    जइ(m) paths = get_random_paths(paths, n, m);
    वर्ण **replace_paths = find_replace_paths(paths, n, ".png", "-label.png");
    data d = {0};
    d.shallow = 0;
    d.X = load_image_paths(paths, n, w, h);
    d.y = load_image_paths_gray(replace_paths, n, out_w, out_h);
    जइ(m) मुक्त(paths);
    पूर्णाङ्क i;
    प्रति(i = 0; i < n; ++i) मुक्त(replace_paths[i]);
    मुक्त(replace_paths);
    प्रत्यागम d;
}

data load_data_old(वर्ण **paths, पूर्णाङ्क n, पूर्णाङ्क m, वर्ण **labels, पूर्णाङ्क k, पूर्णाङ्क w, पूर्णाङ्क h)
{
    जइ(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;
    d.X = load_image_paths(paths, n, w, h);
    d.y = load_labels_paths(paths, n, labels, k, 0);
    जइ(m) मुक्त(paths);
    प्रत्यागम d;
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

data load_data_super(वर्ण **paths, पूर्णाङ्क n, पूर्णाङ्क m, पूर्णाङ्क w, पूर्णाङ्क h, पूर्णाङ्क scale)
{
    जइ(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;

    पूर्णाङ्क i;
    d.X.rows = n;
    d.X.vals = calloc(n, परिमाण(भिन्न*));
    d.X.cols = w*h*3;

    d.y.rows = n;
    d.y.vals = calloc(n, परिमाण(भिन्न*));
    d.y.cols = w*scale * h*scale * 3;

    प्रति(i = 0; i < n; ++i){
        image im = load_image_color(paths[i], 0, 0);
        image crop = random_crop_image(im, w*scale, h*scale);
        पूर्णाङ्क flip = rand()%2;
        जइ (flip) flip_image(crop);
        image resize = resize_image(crop, w, h);
        d.X.vals[i] = resize.data;
        d.y.vals[i] = crop.data;
        free_image(im);
    }

    जइ(m) मुक्त(paths);
    प्रत्यागम d;
}

data load_data_regression(वर्ण **paths, पूर्णाङ्क n, पूर्णाङ्क m, पूर्णाङ्क k, पूर्णाङ्क min, पूर्णाङ्क max, पूर्णाङ्क size, भिन्न angle, भिन्न aspect, भिन्न hue, भिन्न saturation, भिन्न exposure)
{
    जइ(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;
    d.X = load_image_augment_paths(paths, n, min, max, size, angle, aspect, hue, saturation, exposure, 0);
    d.y = load_regression_labels_paths(paths, n, k);
    जइ(m) मुक्त(paths);
    प्रत्यागम d;
}

data select_data(data *orig, पूर्णाङ्क *inds)
{
    data d = {0};
    d.shallow = 1;
    d.w = orig[0].w;
    d.h = orig[0].h;

    d.X.rows = orig[0].X.rows;
    d.y.rows = orig[0].X.rows;

    d.X.cols = orig[0].X.cols;
    d.y.cols = orig[0].y.cols;

    d.X.vals = calloc(orig[0].X.rows, परिमाण(भिन्न *));
    d.y.vals = calloc(orig[0].y.rows, परिमाण(भिन्न *));
    पूर्णाङ्क i;
    प्रति(i = 0; i < d.X.rows; ++i){
        d.X.vals[i] = orig[inds[i]].X.vals[i];
        d.y.vals[i] = orig[inds[i]].y.vals[i];
    }
    प्रत्यागम d;
}

data *tile_data(data orig, पूर्णाङ्क divs, पूर्णाङ्क size)
{
    data *ds = calloc(divs*divs, परिमाण(data));
    पूर्णाङ्क i, j;
#pragma omp parallel प्रति
    प्रति(i = 0; i < divs*divs; ++i){
        data d;
        d.shallow = 0;
        d.w = orig.w/divs * size;
        d.h = orig.h/divs * size;
        d.X.rows = orig.X.rows;
        d.X.cols = d.w*d.h*3;
        d.X.vals = calloc(d.X.rows, परिमाण(भिन्न*));

        d.y = copy_matrix(orig.y);
#pragma omp parallel प्रति
        प्रति(j = 0; j < orig.X.rows; ++j){
            पूर्णाङ्क x = (i%divs) * orig.w / divs - (d.w - orig.w/divs)/2;
            पूर्णाङ्क y = (i/divs) * orig.h / divs - (d.h - orig.h/divs)/2;
            image im = float_to_image(orig.w, orig.h, 3, orig.X.vals[j]);
            d.X.vals[j] = crop_image(im, x, y, d.w, d.h).data;
        }
        ds[i] = d;
    }
    प्रत्यागम ds;
}

data resize_data(data orig, पूर्णाङ्क w, पूर्णाङ्क h)
{
    data d = {0};
    d.shallow = 0;
    d.w = w;
    d.h = h;
    पूर्णाङ्क i;
    d.X.rows = orig.X.rows;
    d.X.cols = w*h*3;
    d.X.vals = calloc(d.X.rows, परिमाण(भिन्न*));

    d.y = copy_matrix(orig.y);
#pragma omp parallel प्रति
    प्रति(i = 0; i < orig.X.rows; ++i){
        image im = float_to_image(orig.w, orig.h, 3, orig.X.vals[i]);
        d.X.vals[i] = resize_image(im, w, h).data;
    }
    प्रत्यागम d;
}

data load_data_augment(वर्ण **paths, पूर्णाङ्क n, पूर्णाङ्क m, वर्ण **labels, पूर्णाङ्क k, वृक्षः *hierarchy, पूर्णाङ्क min, पूर्णाङ्क max, पूर्णाङ्क size, भिन्न angle, भिन्न aspect, भिन्न hue, भिन्न saturation, भिन्न exposure, पूर्णाङ्क center)
{
    जइ(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;
    d.w=size;
    d.h=size;
    d.X = load_image_augment_paths(paths, n, min, max, size, angle, aspect, hue, saturation, exposure, center);
    d.y = load_labels_paths(paths, n, labels, k, hierarchy);
    जइ(m) मुक्त(paths);
    प्रत्यागम d;
}

data load_data_tag(वर्ण **paths, पूर्णाङ्क n, पूर्णाङ्क m, पूर्णाङ्क k, पूर्णाङ्क min, पूर्णाङ्क max, पूर्णाङ्क size, भिन्न angle, भिन्न aspect, भिन्न hue, भिन्न saturation, भिन्न exposure)
{
    जइ(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.w = size;
    d.h = size;
    d.shallow = 0;
    d.X = load_image_augment_paths(paths, n, min, max, size, angle, aspect, hue, saturation, exposure, 0);
    d.y = load_tags_paths(paths, n, k);
    जइ(m) मुक्त(paths);
    प्रत्यागम d;
}

matrix concat_matrix(matrix m1, matrix m2)
{
    पूर्णाङ्क i, गणनम् = 0;
    matrix m;
    m.cols = m1.cols;
    m.rows = m1.rows+m2.rows;
    m.vals = calloc(m1.rows + m2.rows, परिमाण(भिन्न*));
    प्रति(i = 0; i < m1.rows; ++i){
        m.vals[गणनम्++] = m1.vals[i];
    }
    प्रति(i = 0; i < m2.rows; ++i){
        m.vals[गणनम्++] = m2.vals[i];
    }
    प्रत्यागम m;
}

data concat_data(data d1, data d2)
{
    data d = {0};
    d.shallow = 1;
    d.X = concat_matrix(d1.X, d2.X);
    d.y = concat_matrix(d1.y, d2.y);
    d.w = d1.w;
    d.h = d1.h;
    प्रत्यागम d;
}

data concat_datas(data *d, पूर्णाङ्क n)
{
    पूर्णाङ्क i;
    data out = {0};
    प्रति(i = 0; i < n; ++i){
        data new = concat_data(d[i], out);
        free_data(out);
        out = new;
    }
    प्रत्यागम out;
}

data load_categorical_data_csv(वर्ण *filename, पूर्णाङ्क target, पूर्णाङ्क k)
{
    data d = {0};
    d.shallow = 0;
    matrix X = csv_to_matrix(filename);
    भिन्न *truth_1d = निष्कासय_column(&X, target);
    भिन्न **truth = one_hot_encode(truth_1d, X.rows, k);
    matrix y;
    y.rows = X.rows;
    y.cols = k;
    y.vals = truth;
    d.X = X;
    d.y = y;
    मुक्त(truth_1d);
    प्रत्यागम d;
}

data load_cifar10_data(वर्ण *filename)
{
    data d = {0};
    d.shallow = 0;
    long i,j;
    matrix X = make_matrix(10000, 3072);
    matrix y = make_matrix(10000, 10);
    d.X = X;
    d.y = y;

    FILE *fp = fopen(filename, "rb");
    जइ(!fp) file_error(filename);
    प्रति(i = 0; i < 10000; ++i){
        unsigned वर्ण bytes[3073];
        fread(bytes, 1, 3073, fp);
        पूर्णाङ्क class = bytes[0];
        y.vals[i][class] = 1;
        प्रति(j = 0; j < X.cols; ++j){
            X.vals[i][j] = (द्विगुण)bytes[j+1];
        }
    }
    scale_data_rows(d, 1./255);
    //normalize_data_rows(d);
    fclose(fp);
    प्रत्यागम d;
}

शून्य get_random_batch(data d, पूर्णाङ्क n, भिन्न *X, भिन्न *y)
{
    पूर्णाङ्क j;
    प्रति(j = 0; j < n; ++j){
        पूर्णाङ्क index = rand()%d.X.rows;
        memcpy(X+j*d.X.cols, d.X.vals[index], d.X.cols*परिमाण(भिन्न));
        memcpy(y+j*d.y.cols, d.y.vals[index], d.y.cols*परिमाण(भिन्न));
    }
}

शून्य get_next_batch(data d, पूर्णाङ्क n, पूर्णाङ्क offset, भिन्न *X, भिन्न *y)
{
    पूर्णाङ्क j;
    प्रति(j = 0; j < n; ++j){
        पूर्णाङ्क index = offset + j;
        memcpy(X+j*d.X.cols, d.X.vals[index], d.X.cols*परिमाण(भिन्न));
        जइ(y) memcpy(y+j*d.y.cols, d.y.vals[index], d.y.cols*परिमाण(भिन्न));
    }
}

शून्य smooth_data(data d)
{
    पूर्णाङ्क i, j;
    भिन्न scale = 1. / d.y.cols;
    भिन्न eps = .1;
    प्रति(i = 0; i < d.y.rows; ++i){
        प्रति(j = 0; j < d.y.cols; ++j){
            d.y.vals[i][j] = eps * scale + (1-eps) * d.y.vals[i][j];
        }
    }
}

data load_all_cifar10()
{
    data d = {0};
    d.shallow = 0;
    पूर्णाङ्क i,j,b;
    matrix X = make_matrix(50000, 3072);
    matrix y = make_matrix(50000, 10);
    d.X = X;
    d.y = y;


    प्रति(b = 0; b < 5; ++b){
        वर्ण buff[256];
        sprintf(buff, "data/cifar/cifar-10-batches-bin/data_batch_%d.bin", b+1);
        FILE *fp = fopen(buff, "rb");
        जइ(!fp) file_error(buff);
        प्रति(i = 0; i < 10000; ++i){
            unsigned वर्ण bytes[3073];
            fread(bytes, 1, 3073, fp);
            पूर्णाङ्क class = bytes[0];
            y.vals[i+b*10000][class] = 1;
            प्रति(j = 0; j < X.cols; ++j){
                X.vals[i+b*10000][j] = (द्विगुण)bytes[j+1];
            }
        }
        fclose(fp);
    }
    //normalize_data_rows(d);
    scale_data_rows(d, 1./255);
    smooth_data(d);
    प्रत्यागम d;
}

data load_go(वर्ण *filename)
{
    FILE *fp = fopen(filename, "rb");
    matrix X = make_matrix(3363059, 361);
    matrix y = make_matrix(3363059, 361);
    पूर्णाङ्क row, col;

    जइ(!fp) file_error(filename);
    वर्ण *label;
    पूर्णाङ्क गणनम् = 0;
    जाव((label = fgetl(fp))){
        पूर्णाङ्क i;
        जइ(गणनम् == X.rows){
            X = resize_matrix(X, गणनम्*2);
            y = resize_matrix(y, गणनम्*2);
        }
        sscanf(label, "%d %d", &row, &col);
        वर्ण *board = fgetl(fp);

        पूर्णाङ्क index = row*19 + col;
        y.vals[गणनम्][index] = 1;

        प्रति(i = 0; i < 19*19; ++i){
            भिन्न val = 0;
            जइ(board[i] == '1') val = 1;
            अण्णहा जइ(board[i] == '2') val = -1;
            X.vals[गणनम्][i] = val;
        }
        ++गणनम्;
        मुक्त(label);
        मुक्त(board);
    }
    X = resize_matrix(X, गणनम्);
    y = resize_matrix(y, गणनम्);

    data d = {0};
    d.shallow = 0;
    d.X = X;
    d.y = y;


    fclose(fp);

    प्रत्यागम d;
}


शून्य randomize_data(data d)
{
    पूर्णाङ्क i;
    प्रति(i = d.X.rows-1; i > 0; --i){
        पूर्णाङ्क index = rand()%i;
        भिन्न *swap = d.X.vals[index];
        d.X.vals[index] = d.X.vals[i];
        d.X.vals[i] = swap;

        swap = d.y.vals[index];
        d.y.vals[index] = d.y.vals[i];
        d.y.vals[i] = swap;
    }
}

शून्य scale_data_rows(data d, भिन्न s)
{
    पूर्णाङ्क i;
    प्रति(i = 0; i < d.X.rows; ++i){
        scale_array(d.X.vals[i], d.X.cols, s);
    }
}

शून्य translate_data_rows(data d, भिन्न s)
{
    पूर्णाङ्क i;
    प्रति(i = 0; i < d.X.rows; ++i){
        translate_array(d.X.vals[i], d.X.cols, s);
    }
}

data copy_data(data d)
{
    data c = {0};
    c.w = d.w;
    c.h = d.h;
    c.shallow = 0;
    c.अङ्कः_boxes = d.अङ्कः_boxes;
    c.boxes = d.boxes;
    c.X = copy_matrix(d.X);
    c.y = copy_matrix(d.y);
    प्रत्यागम c;
}

शून्य normalize_data_rows(data d)
{
    पूर्णाङ्क i;
    प्रति(i = 0; i < d.X.rows; ++i){
        normalize_array(d.X.vals[i], d.X.cols);
    }
}

data get_data_part(data d, पूर्णाङ्क part, पूर्णाङ्क total)
{
    data p = {0};
    p.shallow = 1;
    p.X.rows = d.X.rows * (part + 1) / total - d.X.rows * part / total;
    p.y.rows = d.y.rows * (part + 1) / total - d.y.rows * part / total;
    p.X.cols = d.X.cols;
    p.y.cols = d.y.cols;
    p.X.vals = d.X.vals + d.X.rows * part / total;
    p.y.vals = d.y.vals + d.y.rows * part / total;
    प्रत्यागम p;
}

data get_random_data(data d, पूर्णाङ्क अङ्कः)
{
    data r = {0};
    r.shallow = 1;

    r.X.rows = अङ्कः;
    r.y.rows = अङ्कः;

    r.X.cols = d.X.cols;
    r.y.cols = d.y.cols;

    r.X.vals = calloc(अङ्कः, परिमाण(भिन्न *));
    r.y.vals = calloc(अङ्कः, परिमाण(भिन्न *));

    पूर्णाङ्क i;
    प्रति(i = 0; i < अङ्कः; ++i){
        पूर्णाङ्क index = rand()%d.X.rows;
        r.X.vals[i] = d.X.vals[index];
        r.y.vals[i] = d.y.vals[index];
    }
    प्रत्यागम r;
}

data *split_data(data d, पूर्णाङ्क part, पूर्णाङ्क total)
{
    data *split = calloc(2, परिमाण(data));
    पूर्णाङ्क i;
    पूर्णाङ्क start = part*d.X.rows/total;
    पूर्णाङ्क end = (part+1)*d.X.rows/total;
    data train;
    data test;
    train.shallow = test.shallow = 1;

    test.X.rows = test.y.rows = end-start;
    train.X.rows = train.y.rows = d.X.rows - (end-start);
    train.X.cols = test.X.cols = d.X.cols;
    train.y.cols = test.y.cols = d.y.cols;

    train.X.vals = calloc(train.X.rows, परिमाण(भिन्न*));
    test.X.vals = calloc(test.X.rows, परिमाण(भिन्न*));
    train.y.vals = calloc(train.y.rows, परिमाण(भिन्न*));
    test.y.vals = calloc(test.y.rows, परिमाण(भिन्न*));

    प्रति(i = 0; i < start; ++i){
        train.X.vals[i] = d.X.vals[i];
        train.y.vals[i] = d.y.vals[i];
    }
    प्रति(i = start; i < end; ++i){
        test.X.vals[i-start] = d.X.vals[i];
        test.y.vals[i-start] = d.y.vals[i];
    }
    प्रति(i = end; i < d.X.rows; ++i){
        train.X.vals[i-(end-start)] = d.X.vals[i];
        train.y.vals[i-(end-start)] = d.y.vals[i];
    }
    split[0] = train;
    split[1] = test;
    प्रत्यागम split;
}

