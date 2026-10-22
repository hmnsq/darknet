#અન્તર્ભાવ "data.h"
#અન્તર્ભાવ "utils.h"
#અન્તર્ભાવ "image.h"
#અન્તર્ભાવ "cuda.h"

#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <stdlib.h>
#અન્તર્ભાવ <string.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

list *get_paths(અક્ષર *filename)
{
    અક્ષર *path;
    FILE *file = fopen(filename, "r");
    જો(!file) file_error(filename);
    list *લીટીઓ = make_list();
    જ્યારે((path=fgetl(file))){
        list_ઉમેરો(લીટીઓ, path);
    }
    fclose(file);
    પાછા લીટીઓ;
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

અક્ષર **get_random_paths(અક્ષર **paths, પૂર્ણાંક n, પૂર્ણાંક m)
{
    અક્ષર **random_paths = calloc(n, કદ(અક્ષર*));
    પૂર્ણાંક i;
    pthread_mutex_lock(&mutex);
    માટે(i = 0; i < n; ++i){
        પૂર્ણાંક index = rand()%m;
        random_paths[i] = paths[index];
        //if(i == 0) printf("%s\n", paths[index]);
    }
    pthread_mutex_unlock(&mutex);
    પાછા random_paths;
}

અક્ષર **find_replace_paths(અક્ષર **paths, પૂર્ણાંક n, અક્ષર *find, અક્ષર *replace)
{
    અક્ષર **replace_paths = calloc(n, કદ(અક્ષર*));
    પૂર્ણાંક i;
    માટે(i = 0; i < n; ++i){
        અક્ષર replaced[4096];
        find_replace(paths[i], find, replace, replaced);
        replace_paths[i] = copy_string(replaced);
    }
    પાછા replace_paths;
}

matrix load_image_paths_gray(અક્ષર **paths, પૂર્ણાંક n, પૂર્ણાંક w, પૂર્ણાંક h)
{
    પૂર્ણાંક i;
    matrix X;
    X.rows = n;
    X.vals = calloc(X.rows, કદ(દશાંશ*));
    X.cols = 0;

    માટે(i = 0; i < n; ++i){
        image im = load_image(paths[i], w, h, 3);

        image gray = grayscale_image(im);
        free_image(im);
        im = gray;

        X.vals[i] = im.data;
        X.cols = im.h*im.w*im.c;
    }
    પાછા X;
}

matrix load_image_paths(અક્ષર **paths, પૂર્ણાંક n, પૂર્ણાંક w, પૂર્ણાંક h)
{
    પૂર્ણાંક i;
    matrix X;
    X.rows = n;
    X.vals = calloc(X.rows, કદ(દશાંશ*));
    X.cols = 0;

    માટે(i = 0; i < n; ++i){
        image im = load_image_color(paths[i], w, h);
        X.vals[i] = im.data;
        X.cols = im.h*im.w*im.c;
    }
    પાછા X;
}

matrix load_image_augment_paths(અક્ષર **paths, પૂર્ણાંક n, પૂર્ણાંક min, પૂર્ણાંક max, પૂર્ણાંક size, દશાંશ angle, દશાંશ aspect, દશાંશ hue, દશાંશ saturation, દશાંશ exposure, પૂર્ણાંક center)
{
    પૂર્ણાંક i;
    matrix X;
    X.rows = n;
    X.vals = calloc(X.rows, કદ(દશાંશ*));
    X.cols = 0;

    માટે(i = 0; i < n; ++i){
        image im = load_image_color(paths[i], 0, 0);
        image crop;
        જો(center){
            crop = center_crop_image(im, size, size);
        } નહીંતર {
            crop = random_augment_image(im, angle, aspect, min, max, size, size);
        }
        પૂર્ણાંક flip = rand()%2;
        જો (flip) flip_image(crop);
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
    પાછા X;
}


box_label *read_boxes(અક્ષર *filename, પૂર્ણાંક *n)
{
    FILE *file = fopen(filename, "r");
    જો(!file) file_error(filename);
    દશાંશ x, y, h, w;
    પૂર્ણાંક id;
    પૂર્ણાંક ગણતરી = 0;
    પૂર્ણાંક size = 64;
    box_label *boxes = calloc(size, કદ(box_label));
    જ્યારે(fscanf(file, "%d %f %f %f %f", &id, &x, &y, &w, &h) == 5){
        જો(ગણતરી == size) {
            size = size * 2;
            boxes = realloc(boxes, size*કદ(box_label));
        }
        boxes[ગણતરી].id = id;
        boxes[ગણતરી].x = x;
        boxes[ગણતરી].y = y;
        boxes[ગણતરી].h = h;
        boxes[ગણતરી].w = w;
        boxes[ગણતરી].ડાબું   = x - w/2;
        boxes[ગણતરી].જમણું  = x + w/2;
        boxes[ગણતરી].ટોચ    = y - h/2;
        boxes[ગણતરી].bottom = y + h/2;
        ++ગણતરી;
    }
    fclose(file);
    *n = ગણતરી;
    પાછા boxes;
}

શૂન્ય randomize_boxes(box_label *b, પૂર્ણાંક n)
{
    પૂર્ણાંક i;
    માટે(i = 0; i < n; ++i){
        box_label swap = b[i];
        પૂર્ણાંક index = rand()%n;
        b[i] = b[index];
        b[index] = swap;
    }
}

શૂન્ય correct_boxes(box_label *boxes, પૂર્ણાંક n, દશાંશ dx, દશાંશ dy, દશાંશ sx, દશાંશ sy, પૂર્ણાંક flip)
{
    પૂર્ણાંક i;
    માટે(i = 0; i < n; ++i){
        જો(boxes[i].x == 0 && boxes[i].y == 0) {
            boxes[i].x = 999999;
            boxes[i].y = 999999;
            boxes[i].w = 999999;
            boxes[i].h = 999999;
            ચાલુ;
        }
        boxes[i].ડાબું   = boxes[i].ડાબું  * sx - dx;
        boxes[i].જમણું  = boxes[i].જમણું * sx - dx;
        boxes[i].ટોચ    = boxes[i].ટોચ   * sy - dy;
        boxes[i].bottom = boxes[i].bottom* sy - dy;

        જો(flip){
            દશાંશ swap = boxes[i].ડાબું;
            boxes[i].ડાબું = 1. - boxes[i].જમણું;
            boxes[i].જમણું = 1. - swap;
        }

        boxes[i].ડાબું =  constrain(0, 1, boxes[i].ડાબું);
        boxes[i].જમણું = constrain(0, 1, boxes[i].જમણું);
        boxes[i].ટોચ =   constrain(0, 1, boxes[i].ટોચ);
        boxes[i].bottom =   constrain(0, 1, boxes[i].bottom);

        boxes[i].x = (boxes[i].ડાબું+boxes[i].જમણું)/2;
        boxes[i].y = (boxes[i].ટોચ+boxes[i].bottom)/2;
        boxes[i].w = (boxes[i].જમણું - boxes[i].ડાબું);
        boxes[i].h = (boxes[i].bottom - boxes[i].ટોચ);

        boxes[i].w = constrain(0, 1, boxes[i].w);
        boxes[i].h = constrain(0, 1, boxes[i].h);
    }
}

શૂન્ય fill_truth_swag(અક્ષર *path, દશાંશ *truth, પૂર્ણાંક classes, પૂર્ણાંક flip, દશાંશ dx, દશાંશ dy, દશાંશ sx, દશાંશ sy)
{
    અક્ષર labelpath[4096];
    find_replace(path, "images", "labels", labelpath);
    find_replace(labelpath, "JPEGImages", "labels", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);

    પૂર્ણાંક ગણતરી = 0;
    box_label *boxes = read_boxes(labelpath, &ગણતરી);
    randomize_boxes(boxes, ગણતરી);
    correct_boxes(boxes, ગણતરી, dx, dy, sx, sy, flip);
    દશાંશ x,y,w,h;
    પૂર્ણાંક id;
    પૂર્ણાંક i;

    માટે (i = 0; i < ગણતરી && i < 90; ++i) {
        x =  boxes[i].x;
        y =  boxes[i].y;
        w =  boxes[i].w;
        h =  boxes[i].h;
        id = boxes[i].id;

        જો (w < .0 || h < .0) ચાલુ;

        પૂર્ણાંક index = (4+classes) * i;

        truth[index++] = x;
        truth[index++] = y;
        truth[index++] = w;
        truth[index++] = h;

        જો (id < classes) truth[index+id] = 1;
    }
    મુક્ત(boxes);
}

શૂન્ય fill_truth_region(અક્ષર *path, દશાંશ *truth, પૂર્ણાંક classes, પૂર્ણાંક અંક_boxes, પૂર્ણાંક flip, દશાંશ dx, દશાંશ dy, દશાંશ sx, દશાંશ sy)
{
    અક્ષર labelpath[4096];
    find_replace(path, "images", "labels", labelpath);
    find_replace(labelpath, "JPEGImages", "labels", labelpath);

    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".png", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    પૂર્ણાંક ગણતરી = 0;
    box_label *boxes = read_boxes(labelpath, &ગણતરી);
    randomize_boxes(boxes, ગણતરી);
    correct_boxes(boxes, ગણતરી, dx, dy, sx, sy, flip);
    દશાંશ x,y,w,h;
    પૂર્ણાંક id;
    પૂર્ણાંક i;

    માટે (i = 0; i < ગણતરી; ++i) {
        x =  boxes[i].x;
        y =  boxes[i].y;
        w =  boxes[i].w;
        h =  boxes[i].h;
        id = boxes[i].id;

        જો (w < .005 || h < .005) ચાલુ;

        પૂર્ણાંક col = (પૂર્ણાંક)(x*અંક_boxes);
        પૂર્ણાંક row = (પૂર્ણાંક)(y*અંક_boxes);

        x = x*અંક_boxes - col;
        y = y*અંક_boxes - row;

        પૂર્ણાંક index = (col+row*અંક_boxes)*(5+classes);
        જો (truth[index]) ચાલુ;
        truth[index++] = 1;

        જો (id < classes) truth[index+id] = 1;
        index += classes;

        truth[index++] = x;
        truth[index++] = y;
        truth[index++] = w;
        truth[index++] = h;
    }
    મુક્ત(boxes);
}

શૂન્ય load_rle(image im, પૂર્ણાંક *rle, પૂર્ણાંક n)
{
    પૂર્ણાંક ગણતરી = 0;
    પૂર્ણાંક curr = 0;
    પૂર્ણાંક i,j;
    માટે(i = 0; i < n; ++i){
        માટે(j = 0; j < rle[i]; ++j){
            im.data[ગણતરી++] = curr;
        }
        curr = 1 - curr;
    }
    માટે(; ગણતરી < im.h*im.w*im.c; ++ગણતરી){
        im.data[ગણતરી] = curr;
    }
}

શૂન્ય or_image(image src, image dest, પૂર્ણાંક c)
{
    પૂર્ણાંક i;
    માટે(i = 0; i < src.w*src.h; ++i){
        જો(src.data[i]) dest.data[dest.w*dest.h*c + i] = 1;
    }
}

શૂન્ય exclusive_image(image src)
{
    પૂર્ણાંક k, j, i;
    પૂર્ણાંક s = src.w*src.h;
    માટે(k = 0; k < src.c-1; ++k){
        માટે(i = 0; i < s; ++i){
            જો (src.data[k*s + i]){
                માટે(j = k+1; j < src.c; ++j){
                    src.data[j*s + i] = 0;
                }
            }
        }
    }
}

box bound_image(image im)
{
    પૂર્ણાંક x,y;
    પૂર્ણાંક minx = im.w;
    પૂર્ણાંક miny = im.h;
    પૂર્ણાંક maxx = 0;
    પૂર્ણાંક maxy = 0;
    માટે(y = 0; y < im.h; ++y){
        માટે(x = 0; x < im.w; ++x){
            જો(im.data[y*im.w + x]){
                minx = (x < minx) ? x : minx;
                miny = (y < miny) ? y : miny;
                maxx = (x > maxx) ? x : maxx;
                maxy = (y > maxy) ? y : maxy;
            }
        }
    }
    box b = {minx, miny, maxx-minx + 1, maxy-miny + 1};
    //printf("%f %f %f %f\n", b.x, b.y, b.w, b.h);
    પાછા b;
}

શૂન્ય fill_truth_iseg(અક્ષર *path, પૂર્ણાંક અંક_boxes, દશાંશ *truth, પૂર્ણાંક classes, પૂર્ણાંક w, પૂર્ણાંક h, augment_args aug, પૂર્ણાંક flip, પૂર્ણાંક mw, પૂર્ણાંક mh)
{
    અક્ષર labelpath[4096];
    find_replace(path, "images", "mask", labelpath);
    find_replace(labelpath, "JPEGImages", "mask", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    FILE *file = fopen(labelpath, "r");
    જો(!file) file_error(labelpath);
    અક્ષર buff[32788];
    પૂર્ણાંક id;
    પૂર્ણાંક i = 0;
    પૂર્ણાંક j;
    image part = make_image(w, h, 1);
    જ્યારે((fscanf(file, "%d %s", &id, buff) == 2) && i < અંક_boxes){
        પૂર્ણાંક n = 0;
        પૂર્ણાંક *rle = read_intlist(buff, &n, 0);
        load_rle(part, rle, n);
        image sized = rotate_crop_image(part, aug.rad, aug.scale, aug.w, aug.h, aug.dx, aug.dy, aug.aspect);
        જો(flip) flip_image(sized);

        image mask = resize_image(sized, mw, mh);
        truth[i*(mw*mh+1)] = id;
        માટે(j = 0; j < mw*mh; ++j){
            truth[i*(mw*mh + 1) + 1 + j] = mask.data[j];
        }
        ++i;

        free_image(mask);
        free_image(sized);
        મુક્ત(rle);
    }
    જો(i < અંક_boxes) truth[i*(mw*mh+1)] = -1;
    fclose(file);
    free_image(part);
}

શૂન્ય fill_truth_mask(અક્ષર *path, પૂર્ણાંક અંક_boxes, દશાંશ *truth, પૂર્ણાંક classes, પૂર્ણાંક w, પૂર્ણાંક h, augment_args aug, પૂર્ણાંક flip, પૂર્ણાંક mw, પૂર્ણાંક mh)
{
    અક્ષર labelpath[4096];
    find_replace(path, "images", "mask", labelpath);
    find_replace(labelpath, "JPEGImages", "mask", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    FILE *file = fopen(labelpath, "r");
    જો(!file) file_error(labelpath);
    અક્ષર buff[32788];
    પૂર્ણાંક id;
    પૂર્ણાંક i = 0;
    image part = make_image(w, h, 1);
    જ્યારે((fscanf(file, "%d %s", &id, buff) == 2) && i < અંક_boxes){
        પૂર્ણાંક n = 0;
        પૂર્ણાંક *rle = read_intlist(buff, &n, 0);
        load_rle(part, rle, n);
        image sized = rotate_crop_image(part, aug.rad, aug.scale, aug.w, aug.h, aug.dx, aug.dy, aug.aspect);
        જો(flip) flip_image(sized);
        box b = bound_image(sized);
        જો(b.w > 0){
            image crop = crop_image(sized, b.x, b.y, b.w, b.h);
            image mask = resize_image(crop, mw, mh);
            truth[i*(4 + mw*mh + 1) + 0] = (b.x + b.w/2.)/sized.w;
            truth[i*(4 + mw*mh + 1) + 1] = (b.y + b.h/2.)/sized.h;
            truth[i*(4 + mw*mh + 1) + 2] = b.w/sized.w;
            truth[i*(4 + mw*mh + 1) + 3] = b.h/sized.h;
            પૂર્ણાંક j;
            માટે(j = 0; j < mw*mh; ++j){
                truth[i*(4 + mw*mh + 1) + 4 + j] = mask.data[j];
            }
            truth[i*(4 + mw*mh + 1) + 4 + mw*mh] = id;
            free_image(crop);
            free_image(mask);
            ++i;
        }
        free_image(sized);
        મુક્ત(rle);
    }
    fclose(file);
    free_image(part);
}


શૂન્ય fill_truth_detection(અક્ષર *path, પૂર્ણાંક અંક_boxes, દશાંશ *truth, પૂર્ણાંક classes, પૂર્ણાંક flip, દશાંશ dx, દશાંશ dy, દશાંશ sx, દશાંશ sy)
{
    અક્ષર labelpath[4096];
    find_replace(path, "images", "labels", labelpath);
    find_replace(labelpath, "JPEGImages", "labels", labelpath);

    find_replace(labelpath, "raw", "labels", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".png", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    પૂર્ણાંક ગણતરી = 0;
    box_label *boxes = read_boxes(labelpath, &ગણતરી);
    randomize_boxes(boxes, ગણતરી);
    correct_boxes(boxes, ગણતરી, dx, dy, sx, sy, flip);
    જો(ગણતરી > અંક_boxes) ગણતરી = અંક_boxes;
    દશાંશ x,y,w,h;
    પૂર્ણાંક id;
    પૂર્ણાંક i;
    પૂર્ણાંક sub = 0;

    માટે (i = 0; i < ગણતરી; ++i) {
        x =  boxes[i].x;
        y =  boxes[i].y;
        w =  boxes[i].w;
        h =  boxes[i].h;
        id = boxes[i].id;

        જો ((w < .001 || h < .001)) {
            ++sub;
            ચાલુ;
        }

        truth[(i-sub)*5+0] = x;
        truth[(i-sub)*5+1] = y;
        truth[(i-sub)*5+2] = w;
        truth[(i-sub)*5+3] = h;
        truth[(i-sub)*5+4] = id;
    }
    મુક્ત(boxes);
}

#define NUMCHARS 37

શૂન્ય print_letters(દશાંશ *pred, પૂર્ણાંક n)
{
    પૂર્ણાંક i;
    માટે(i = 0; i < n; ++i){
        પૂર્ણાંક index = max_index(pred+i*NUMCHARS, NUMCHARS);
        printf("%c", int_to_alphanum(index));
    }
    printf("\n");
}

શૂન્ય fill_truth_captcha(અક્ષર *path, પૂર્ણાંક n, દશાંશ *truth)
{
    અક્ષર *begin = strrchr(path, '/');
    ++begin;
    પૂર્ણાંક i;
    માટે(i = 0; i < strlen(begin) && i < n && begin[i] != '.'; ++i){
        પૂર્ણાંક index = alphanum_to_int(begin[i]);
        જો(index > 35) printf("Bad %c\n", begin[i]);
        truth[i*NUMCHARS+index] = 1;
    }
    માટે(;i < n; ++i){
        truth[i*NUMCHARS + NUMCHARS-1] = 1;
    }
}

data load_data_captcha(અક્ષર **paths, પૂર્ણાંક n, પૂર્ણાંક m, પૂર્ણાંક k, પૂર્ણાંક w, પૂર્ણાંક h)
{
    જો(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;
    d.X = load_image_paths(paths, n, w, h);
    d.y = make_matrix(n, k*NUMCHARS);
    પૂર્ણાંક i;
    માટે(i = 0; i < n; ++i){
        fill_truth_captcha(paths[i], k, d.y.vals[i]);
    }
    જો(m) મુક્ત(paths);
    પાછા d;
}

data load_data_captcha_encode(અક્ષર **paths, પૂર્ણાંક n, પૂર્ણાંક m, પૂર્ણાંક w, પૂર્ણાંક h)
{
    જો(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;
    d.X = load_image_paths(paths, n, w, h);
    d.X.cols = 17100;
    d.y = d.X;
    જો(m) મુક્ત(paths);
    પાછા d;
}

શૂન્ય fill_truth(અક્ષર *path, અક્ષર **labels, પૂર્ણાંક k, દશાંશ *truth)
{
    પૂર્ણાંક i;
    memset(truth, 0, k*કદ(દશાંશ));
    પૂર્ણાંક ગણતરી = 0;
    માટે(i = 0; i < k; ++i){
        જો(strstr(path, labels[i])){
            truth[i] = 1;
            ++ગણતરી;
            //printf("%s %s %d\n", path, labels[i], i);
        }
    }
    જો(ગણતરી != 1 && (k != 1 || ગણતરી != 0)) printf("Too many or too few labels: %d, %s\n", ગણતરી, path);
}

શૂન્ય fill_hierarchy(દશાંશ *truth, પૂર્ણાંક k, વૃક્ષ *hierarchy)
{
    પૂર્ણાંક j;
    માટે(j = 0; j < k; ++j){
        જો(truth[j]){
            પૂર્ણાંક parent = hierarchy->parent[j];
            જ્યારે(parent >= 0){
                truth[parent] = 1;
                parent = hierarchy->parent[parent];
            }
        }
    }
    પૂર્ણાંક i;
    પૂર્ણાંક ગણતરી = 0;
    માટે(j = 0; j < hierarchy->groups; ++j){
        //printf("%d\n", count);
        પૂર્ણાંક mask = 1;
        માટે(i = 0; i < hierarchy->group_size[j]; ++i){
            જો(truth[ગણતરી + i]){
                mask = 0;
                તોડો;
            }
        }
        જો (mask) {
            માટે(i = 0; i < hierarchy->group_size[j]; ++i){
                truth[ગણતરી + i] = SECRET_અંક;
            }
        }
        ગણતરી += hierarchy->group_size[j];
    }
}

matrix load_regression_labels_paths(અક્ષર **paths, પૂર્ણાંક n, પૂર્ણાંક k)
{
    matrix y = make_matrix(n, k);
    પૂર્ણાંક i,j;
    માટે(i = 0; i < n; ++i){
        અક્ષર labelpath[4096];
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
        માટે(j = 0; j < k; ++j){
            fscanf(file, "%f", &(y.vals[i][j]));
        }
        fclose(file);
    }
    પાછા y;
}

matrix load_labels_paths(અક્ષર **paths, પૂર્ણાંક n, અક્ષર **labels, પૂર્ણાંક k, વૃક્ષ *hierarchy)
{
    matrix y = make_matrix(n, k);
    પૂર્ણાંક i;
    માટે(i = 0; i < n && labels; ++i){
        fill_truth(paths[i], labels, k, y.vals[i]);
        જો(hierarchy){
            fill_hierarchy(y.vals[i], k, hierarchy);
        }
    }
    પાછા y;
}

matrix load_tags_paths(અક્ષર **paths, પૂર્ણાંક n, પૂર્ણાંક k)
{
    matrix y = make_matrix(n, k);
    પૂર્ણાંક i;
    //int count = 0;
    માટે(i = 0; i < n; ++i){
        અક્ષર label[4096];
        find_replace(paths[i], "images", "labels", label);
        find_replace(label, ".jpg", ".txt", label);
        FILE *file = fopen(label, "r");
        જો (!file) ચાલુ;
        //++count;
        પૂર્ણાંક tag;
        જ્યારે(fscanf(file, "%d", &tag) == 1){
            જો(tag < k){
                y.vals[i][tag] = 1;
            }
        }
        fclose(file);
    }
    //printf("%d/%d\n", count, n);
    પાછા y;
}

અક્ષર **get_labels(અક્ષર *filename)
{
    list *plist = get_paths(filename);
    અક્ષર **labels = (અક્ષર **)list_to_array(plist);
    free_list(plist);
    પાછા labels;
}

શૂન્ય free_data(data d)
{
    જો(!d.shallow){
        free_matrix(d.X);
        free_matrix(d.y);
    }નહીંતર{
        મુક્ત(d.X.vals);
        મુક્ત(d.y.vals);
    }
}

image get_segmentation_image(અક્ષર *path, પૂર્ણાંક w, પૂર્ણાંક h, પૂર્ણાંક classes)
{
    અક્ષર labelpath[4096];
    find_replace(path, "images", "mask", labelpath);
    find_replace(labelpath, "JPEGImages", "mask", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    image mask = make_image(w, h, classes);
    FILE *file = fopen(labelpath, "r");
    જો(!file) file_error(labelpath);
    અક્ષર buff[32788];
    પૂર્ણાંક id;
    image part = make_image(w, h, 1);
    જ્યારે(fscanf(file, "%d %s", &id, buff) == 2){
        પૂર્ણાંક n = 0;
        પૂર્ણાંક *rle = read_intlist(buff, &n, 0);
        load_rle(part, rle, n);
        or_image(part, mask, id);
        મુક્ત(rle);
    }
    //exclusive_image(mask);
    fclose(file);
    free_image(part);
    પાછા mask;
}

image get_segmentation_image2(અક્ષર *path, પૂર્ણાંક w, પૂર્ણાંક h, પૂર્ણાંક classes)
{
    અક્ષર labelpath[4096];
    find_replace(path, "images", "mask", labelpath);
    find_replace(labelpath, "JPEGImages", "mask", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    image mask = make_image(w, h, classes+1);
    પૂર્ણાંક i;
    માટે(i = 0; i < w*h; ++i){
        mask.data[w*h*classes + i] = 1;
    }
    FILE *file = fopen(labelpath, "r");
    જો(!file) file_error(labelpath);
    અક્ષર buff[32788];
    પૂર્ણાંક id;
    image part = make_image(w, h, 1);
    જ્યારે(fscanf(file, "%d %s", &id, buff) == 2){
        પૂર્ણાંક n = 0;
        પૂર્ણાંક *rle = read_intlist(buff, &n, 0);
        load_rle(part, rle, n);
        or_image(part, mask, id);
        માટે(i = 0; i < w*h; ++i){
            જો(part.data[i]) mask.data[w*h*classes + i] = 0;
        }
        મુક્ત(rle);
    }
    //exclusive_image(mask);
    fclose(file);
    free_image(part);
    પાછા mask;
}

data load_data_seg(પૂર્ણાંક n, અક્ષર **paths, પૂર્ણાંક m, પૂર્ણાંક w, પૂર્ણાંક h, પૂર્ણાંક classes, પૂર્ણાંક min, પૂર્ણાંક max, દશાંશ angle, દશાંશ aspect, દશાંશ hue, દશાંશ saturation, દશાંશ exposure, પૂર્ણાંક div)
{
    અક્ષર **random_paths = get_random_paths(paths, n, m);
    પૂર્ણાંક i;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, કદ(દશાંશ*));
    d.X.cols = h*w*3;


    d.y.rows = n;
    d.y.cols = h*w*classes/div/div;
    d.y.vals = calloc(d.X.rows, કદ(દશાંશ*));

    માટે(i = 0; i < n; ++i){
        image orig = load_image_color(random_paths[i], 0, 0);
        augment_args a = random_augment_args(orig, angle, aspect, min, max, w, h);
        image sized = rotate_crop_image(orig, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);

        પૂર્ણાંક flip = rand()%2;
        જો(flip) flip_image(sized);
        random_distort_image(sized, hue, saturation, exposure);
        d.X.vals[i] = sized.data;

        image mask = get_segmentation_image(random_paths[i], orig.w, orig.h, classes);
        //image mask = make_image(orig.w, orig.h, classes+1);
        image sized_m = rotate_crop_image(mask, a.rad, a.scale/div, a.w/div, a.h/div, a.dx/div, a.dy/div, a.aspect);

        જો(flip) flip_image(sized_m);
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
    મુક્ત(random_paths);
    પાછા d;
}

data load_data_iseg(પૂર્ણાંક n, અક્ષર **paths, પૂર્ણાંક m, પૂર્ણાંક w, પૂર્ણાંક h, પૂર્ણાંક classes, પૂર્ણાંક boxes, પૂર્ણાંક div, પૂર્ણાંક min, પૂર્ણાંક max, દશાંશ angle, દશાંશ aspect, દશાંશ hue, દશાંશ saturation, દશાંશ exposure)
{
    અક્ષર **random_paths = get_random_paths(paths, n, m);
    પૂર્ણાંક i;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, કદ(દશાંશ*));
    d.X.cols = h*w*3;

    d.y = make_matrix(n, (((w/div)*(h/div))+1)*boxes);

    માટે(i = 0; i < n; ++i){
        image orig = load_image_color(random_paths[i], 0, 0);
        augment_args a = random_augment_args(orig, angle, aspect, min, max, w, h);
        image sized = rotate_crop_image(orig, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);

        પૂર્ણાંક flip = rand()%2;
        જો(flip) flip_image(sized);
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
    મુક્ત(random_paths);
    પાછા d;
}

data load_data_mask(પૂર્ણાંક n, અક્ષર **paths, પૂર્ણાંક m, પૂર્ણાંક w, પૂર્ણાંક h, પૂર્ણાંક classes, પૂર્ણાંક boxes, પૂર્ણાંક coords, પૂર્ણાંક min, પૂર્ણાંક max, દશાંશ angle, દશાંશ aspect, દશાંશ hue, દશાંશ saturation, દશાંશ exposure)
{
    અક્ષર **random_paths = get_random_paths(paths, n, m);
    પૂર્ણાંક i;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, કદ(દશાંશ*));
    d.X.cols = h*w*3;

    d.y = make_matrix(n, (coords+1)*boxes);

    માટે(i = 0; i < n; ++i){
        image orig = load_image_color(random_paths[i], 0, 0);
        augment_args a = random_augment_args(orig, angle, aspect, min, max, w, h);
        image sized = rotate_crop_image(orig, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);

        પૂર્ણાંક flip = rand()%2;
        જો(flip) flip_image(sized);
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
    મુક્ત(random_paths);
    પાછા d;
}

data load_data_region(પૂર્ણાંક n, અક્ષર **paths, પૂર્ણાંક m, પૂર્ણાંક w, પૂર્ણાંક h, પૂર્ણાંક size, પૂર્ણાંક classes, દશાંશ jitter, દશાંશ hue, દશાંશ saturation, દશાંશ exposure)
{
    અક્ષર **random_paths = get_random_paths(paths, n, m);
    પૂર્ણાંક i;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, કદ(દશાંશ*));
    d.X.cols = h*w*3;


    પૂર્ણાંક k = size*size*(5+classes);
    d.y = make_matrix(n, k);
    માટે(i = 0; i < n; ++i){
        image orig = load_image_color(random_paths[i], 0, 0);

        પૂર્ણાંક oh = orig.h;
        પૂર્ણાંક ow = orig.w;

        પૂર્ણાંક dw = (ow*jitter);
        પૂર્ણાંક dh = (oh*jitter);

        પૂર્ણાંક pleft  = rand_uniform(-dw, dw);
        પૂર્ણાંક pright = rand_uniform(-dw, dw);
        પૂર્ણાંક ptop   = rand_uniform(-dh, dh);
        પૂર્ણાંક pbot   = rand_uniform(-dh, dh);

        પૂર્ણાંક swidth =  ow - pleft - pright;
        પૂર્ણાંક sheight = oh - ptop - pbot;

        દશાંશ sx = (દશાંશ)swidth  / ow;
        દશાંશ sy = (દશાંશ)sheight / oh;

        પૂર્ણાંક flip = rand()%2;
        image cropped = crop_image(orig, pleft, ptop, swidth, sheight);

        દશાંશ dx = ((દશાંશ)pleft/ow)/sx;
        દશાંશ dy = ((દશાંશ)ptop /oh)/sy;

        image sized = resize_image(cropped, w, h);
        જો(flip) flip_image(sized);
        random_distort_image(sized, hue, saturation, exposure);
        d.X.vals[i] = sized.data;

        fill_truth_region(random_paths[i], d.y.vals[i], classes, size, flip, dx, dy, 1./sx, 1./sy);

        free_image(orig);
        free_image(cropped);
    }
    મુક્ત(random_paths);
    પાછા d;
}

data load_data_compare(પૂર્ણાંક n, અક્ષર **paths, પૂર્ણાંક m, પૂર્ણાંક classes, પૂર્ણાંક w, પૂર્ણાંક h)
{
    જો(m) paths = get_random_paths(paths, 2*n, m);
    પૂર્ણાંક i,j;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, કદ(દશાંશ*));
    d.X.cols = h*w*6;

    પૂર્ણાંક k = 2*(classes);
    d.y = make_matrix(n, k);
    માટે(i = 0; i < n; ++i){
        image im1 = load_image_color(paths[i*2],   w, h);
        image im2 = load_image_color(paths[i*2+1], w, h);

        d.X.vals[i] = calloc(d.X.cols, કદ(દશાંશ));
        memcpy(d.X.vals[i],         im1.data, h*w*3*કદ(દશાંશ));
        memcpy(d.X.vals[i] + h*w*3, im2.data, h*w*3*કદ(દશાંશ));

        પૂર્ણાંક id;
        દશાંશ iou;

        અક્ષર imlabel1[4096];
        અક્ષર imlabel2[4096];
        find_replace(paths[i*2],   "imgs", "labels", imlabel1);
        find_replace(imlabel1, "jpg", "txt", imlabel1);
        FILE *fp1 = fopen(imlabel1, "r");

        જ્યારે(fscanf(fp1, "%d %f", &id, &iou) == 2){
            જો (d.y.vals[i][2*id] < iou) d.y.vals[i][2*id] = iou;
        }

        find_replace(paths[i*2+1], "imgs", "labels", imlabel2);
        find_replace(imlabel2, "jpg", "txt", imlabel2);
        FILE *fp2 = fopen(imlabel2, "r");

        જ્યારે(fscanf(fp2, "%d %f", &id, &iou) == 2){
            જો (d.y.vals[i][2*id + 1] < iou) d.y.vals[i][2*id + 1] = iou;
        }

        માટે (j = 0; j < classes; ++j){
            જો (d.y.vals[i][2*j] > .5 &&  d.y.vals[i][2*j+1] < .5){
                d.y.vals[i][2*j] = 1;
                d.y.vals[i][2*j+1] = 0;
            } નહીંતર જો (d.y.vals[i][2*j] < .5 &&  d.y.vals[i][2*j+1] > .5){
                d.y.vals[i][2*j] = 0;
                d.y.vals[i][2*j+1] = 1;
            } નહીંતર {
                d.y.vals[i][2*j]   = SECRET_અંક;
                d.y.vals[i][2*j+1] = SECRET_અંક;
            }
        }
        fclose(fp1);
        fclose(fp2);

        free_image(im1);
        free_image(im2);
    }
    જો(m) મુક્ત(paths);
    પાછા d;
}

data load_data_swag(અક્ષર **paths, પૂર્ણાંક n, પૂર્ણાંક classes, દશાંશ jitter)
{
    પૂર્ણાંક index = rand()%n;
    અક્ષર *random_path = paths[index];

    image orig = load_image_color(random_path, 0, 0);
    પૂર્ણાંક h = orig.h;
    પૂર્ણાંક w = orig.w;

    data d = {0};
    d.shallow = 0;
    d.w = w;
    d.h = h;

    d.X.rows = 1;
    d.X.vals = calloc(d.X.rows, કદ(દશાંશ*));
    d.X.cols = h*w*3;

    પૂર્ણાંક k = (4+classes)*90;
    d.y = make_matrix(1, k);

    પૂર્ણાંક dw = w*jitter;
    પૂર્ણાંક dh = h*jitter;

    પૂર્ણાંક pleft  = rand_uniform(-dw, dw);
    પૂર્ણાંક pright = rand_uniform(-dw, dw);
    પૂર્ણાંક ptop   = rand_uniform(-dh, dh);
    પૂર્ણાંક pbot   = rand_uniform(-dh, dh);

    પૂર્ણાંક swidth =  w - pleft - pright;
    પૂર્ણાંક sheight = h - ptop - pbot;

    દશાંશ sx = (દશાંશ)swidth  / w;
    દશાંશ sy = (દશાંશ)sheight / h;

    પૂર્ણાંક flip = rand()%2;
    image cropped = crop_image(orig, pleft, ptop, swidth, sheight);

    દશાંશ dx = ((દશાંશ)pleft/w)/sx;
    દશાંશ dy = ((દશાંશ)ptop /h)/sy;

    image sized = resize_image(cropped, w, h);
    જો(flip) flip_image(sized);
    d.X.vals[0] = sized.data;

    fill_truth_swag(random_path, d.y.vals[0], classes, flip, dx, dy, 1./sx, 1./sy);

    free_image(orig);
    free_image(cropped);

    પાછા d;
}

data load_data_detection(પૂર્ણાંક n, અક્ષર **paths, પૂર્ણાંક m, પૂર્ણાંક w, પૂર્ણાંક h, પૂર્ણાંક boxes, પૂર્ણાંક classes, દશાંશ jitter, દશાંશ hue, દશાંશ saturation, દશાંશ exposure)
{
    અક્ષર **random_paths = get_random_paths(paths, n, m);
    પૂર્ણાંક i;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, કદ(દશાંશ*));
    d.X.cols = h*w*3;

    d.y = make_matrix(n, 5*boxes);
    માટે(i = 0; i < n; ++i){
        image orig = load_image_color(random_paths[i], 0, 0);
        image sized = make_image(w, h, orig.c);
        fill_image(sized, .5);

        દશાંશ dw = jitter * orig.w;
        દશાંશ dh = jitter * orig.h;

        દશાંશ new_ar = (orig.w + rand_uniform(-dw, dw)) / (orig.h + rand_uniform(-dh, dh));
        //float scale = rand_uniform(.25, 2);
        દશાંશ scale = 1;

        દશાંશ nw, nh;

        જો(new_ar < 1){
            nh = scale * h;
            nw = nh * new_ar;
        } નહીંતર {
            nw = scale * w;
            nh = nw / new_ar;
        }

        દશાંશ dx = rand_uniform(0, w - nw);
        દશાંશ dy = rand_uniform(0, h - nh);

        place_image(orig, nw, nh, dx, dy, sized);

        random_distort_image(sized, hue, saturation, exposure);

        પૂર્ણાંક flip = rand()%2;
        જો(flip) flip_image(sized);
        d.X.vals[i] = sized.data;


        fill_truth_detection(random_paths[i], boxes, d.y.vals[i], classes, flip, -dx/w, -dy/h, nw/w, nh/h);

        free_image(orig);
    }
    મુક્ત(random_paths);
    પાછા d;
}

શૂન્ય *load_thread(શૂન્ય *ptr)
{
    //printf("Loading data: %d\n", rand());
    load_args a = *(સંરચના load_args*)ptr;
    જો(a.exposure == 0) a.exposure = 1;
    જો(a.saturation == 0) a.saturation = 1;
    જો(a.aspect == 0) a.aspect = 1;

    જો (a.type == OLD_CLASSIFICATION_DATA){
        *a.d = load_data_old(a.paths, a.n, a.m, a.labels, a.classes, a.w, a.h);
    } નહીંતર જો (a.type == REGRESSION_DATA){
        *a.d = load_data_regression(a.paths, a.n, a.m, a.classes, a.min, a.max, a.size, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    } નહીંતર જો (a.type == CLASSIFICATION_DATA){
        *a.d = load_data_augment(a.paths, a.n, a.m, a.labels, a.classes, a.hierarchy, a.min, a.max, a.size, a.angle, a.aspect, a.hue, a.saturation, a.exposure, a.center);
    } નહીંતર જો (a.type == SUPER_DATA){
        *a.d = load_data_super(a.paths, a.n, a.m, a.w, a.h, a.scale);
    } નહીંતર જો (a.type == WRITING_DATA){
        *a.d = load_data_writing(a.paths, a.n, a.m, a.w, a.h, a.out_w, a.out_h);
    } નહીંતર જો (a.type == ISEG_DATA){
        *a.d = load_data_iseg(a.n, a.paths, a.m, a.w, a.h, a.classes, a.અંક_boxes, a.scale, a.min, a.max, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    } નહીંતર જો (a.type == INSTANCE_DATA){
        *a.d = load_data_mask(a.n, a.paths, a.m, a.w, a.h, a.classes, a.અંક_boxes, a.coords, a.min, a.max, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    } નહીંતર જો (a.type == SEGMENTATION_DATA){
        *a.d = load_data_seg(a.n, a.paths, a.m, a.w, a.h, a.classes, a.min, a.max, a.angle, a.aspect, a.hue, a.saturation, a.exposure, a.scale);
    } નહીંતર જો (a.type == REGION_DATA){
        *a.d = load_data_region(a.n, a.paths, a.m, a.w, a.h, a.અંક_boxes, a.classes, a.jitter, a.hue, a.saturation, a.exposure);
    } નહીંતર જો (a.type == DETECTION_DATA){
        *a.d = load_data_detection(a.n, a.paths, a.m, a.w, a.h, a.અંક_boxes, a.classes, a.jitter, a.hue, a.saturation, a.exposure);
    } નહીંતર જો (a.type == SWAG_DATA){
        *a.d = load_data_swag(a.paths, a.n, a.classes, a.jitter);
    } નહીંતર જો (a.type == COMPARE_DATA){
        *a.d = load_data_compare(a.n, a.paths, a.m, a.classes, a.w, a.h);
    } નહીંતર જો (a.type == IMAGE_DATA){
        *(a.im) = load_image_color(a.path, 0, 0);
        *(a.resized) = resize_image(*(a.im), a.w, a.h);
    } નહીંતર જો (a.type == LETTERBOX_DATA){
        *(a.im) = load_image_color(a.path, 0, 0);
        *(a.resized) = letterbox_image(*(a.im), a.w, a.h);
    } નહીંતર જો (a.type == TAG_DATA){
        *a.d = load_data_tag(a.paths, a.n, a.m, a.classes, a.min, a.max, a.size, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    }
    મુક્ત(ptr);
    પાછા 0;
}

pthread_t load_data_in_thread(load_args args)
{
    pthread_t thread;
    સંરચના load_args *ptr = calloc(1, કદ(સંરચના load_args));
    *ptr = args;
    જો(pthread_create(&thread, 0, load_thread, ptr)) error("Thread creation failed");
    પાછા thread;
}

શૂન્ય *load_threads(શૂન્ય *ptr)
{
    પૂર્ણાંક i;
    load_args args = *(load_args *)ptr;
    જો (args.threads == 0) args.threads = 1;
    data *out = args.d;
    પૂર્ણાંક total = args.n;
    મુક્ત(ptr);
    data *buffers = calloc(args.threads, કદ(data));
    pthread_t *threads = calloc(args.threads, કદ(pthread_t));
    માટે(i = 0; i < args.threads; ++i){
        args.d = buffers + i;
        args.n = (i+1) * total/args.threads - i * total/args.threads;
        threads[i] = load_data_in_thread(args);
    }
    માટે(i = 0; i < args.threads; ++i){
        pthread_join(threads[i], 0);
    }
    *out = concat_datas(buffers, args.threads);
    out->shallow = 0;
    માટે(i = 0; i < args.threads; ++i){
        buffers[i].shallow = 1;
        free_data(buffers[i]);
    }
    મુક્ત(buffers);
    મુક્ત(threads);
    પાછા 0;
}

શૂન્ય load_data_blocking(load_args args)
{
    સંરચના load_args *ptr = calloc(1, કદ(સંરચના load_args));
    *ptr = args;
    load_thread(ptr);
}

pthread_t load_data(load_args args)
{
    pthread_t thread;
    સંરચના load_args *ptr = calloc(1, કદ(સંરચના load_args));
    *ptr = args;
    જો(pthread_create(&thread, 0, load_threads, ptr)) error("Thread creation failed");
    પાછા thread;
}

data load_data_writing(અક્ષર **paths, પૂર્ણાંક n, પૂર્ણાંક m, પૂર્ણાંક w, પૂર્ણાંક h, પૂર્ણાંક out_w, પૂર્ણાંક out_h)
{
    જો(m) paths = get_random_paths(paths, n, m);
    અક્ષર **replace_paths = find_replace_paths(paths, n, ".png", "-label.png");
    data d = {0};
    d.shallow = 0;
    d.X = load_image_paths(paths, n, w, h);
    d.y = load_image_paths_gray(replace_paths, n, out_w, out_h);
    જો(m) મુક્ત(paths);
    પૂર્ણાંક i;
    માટે(i = 0; i < n; ++i) મુક્ત(replace_paths[i]);
    મુક્ત(replace_paths);
    પાછા d;
}

data load_data_old(અક્ષર **paths, પૂર્ણાંક n, પૂર્ણાંક m, અક્ષર **labels, પૂર્ણાંક k, પૂર્ણાંક w, પૂર્ણાંક h)
{
    જો(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;
    d.X = load_image_paths(paths, n, w, h);
    d.y = load_labels_paths(paths, n, labels, k, 0);
    જો(m) મુક્ત(paths);
    પાછા d;
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

data load_data_super(અક્ષર **paths, પૂર્ણાંક n, પૂર્ણાંક m, પૂર્ણાંક w, પૂર્ણાંક h, પૂર્ણાંક scale)
{
    જો(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;

    પૂર્ણાંક i;
    d.X.rows = n;
    d.X.vals = calloc(n, કદ(દશાંશ*));
    d.X.cols = w*h*3;

    d.y.rows = n;
    d.y.vals = calloc(n, કદ(દશાંશ*));
    d.y.cols = w*scale * h*scale * 3;

    માટે(i = 0; i < n; ++i){
        image im = load_image_color(paths[i], 0, 0);
        image crop = random_crop_image(im, w*scale, h*scale);
        પૂર્ણાંક flip = rand()%2;
        જો (flip) flip_image(crop);
        image resize = resize_image(crop, w, h);
        d.X.vals[i] = resize.data;
        d.y.vals[i] = crop.data;
        free_image(im);
    }

    જો(m) મુક્ત(paths);
    પાછા d;
}

data load_data_regression(અક્ષર **paths, પૂર્ણાંક n, પૂર્ણાંક m, પૂર્ણાંક k, પૂર્ણાંક min, પૂર્ણાંક max, પૂર્ણાંક size, દશાંશ angle, દશાંશ aspect, દશાંશ hue, દશાંશ saturation, દશાંશ exposure)
{
    જો(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;
    d.X = load_image_augment_paths(paths, n, min, max, size, angle, aspect, hue, saturation, exposure, 0);
    d.y = load_regression_labels_paths(paths, n, k);
    જો(m) મુક્ત(paths);
    પાછા d;
}

data select_data(data *orig, પૂર્ણાંક *inds)
{
    data d = {0};
    d.shallow = 1;
    d.w = orig[0].w;
    d.h = orig[0].h;

    d.X.rows = orig[0].X.rows;
    d.y.rows = orig[0].X.rows;

    d.X.cols = orig[0].X.cols;
    d.y.cols = orig[0].y.cols;

    d.X.vals = calloc(orig[0].X.rows, કદ(દશાંશ *));
    d.y.vals = calloc(orig[0].y.rows, કદ(દશાંશ *));
    પૂર્ણાંક i;
    માટે(i = 0; i < d.X.rows; ++i){
        d.X.vals[i] = orig[inds[i]].X.vals[i];
        d.y.vals[i] = orig[inds[i]].y.vals[i];
    }
    પાછા d;
}

data *tile_data(data orig, પૂર્ણાંક divs, પૂર્ણાંક size)
{
    data *ds = calloc(divs*divs, કદ(data));
    પૂર્ણાંક i, j;
#pragma omp parallel માટે
    માટે(i = 0; i < divs*divs; ++i){
        data d;
        d.shallow = 0;
        d.w = orig.w/divs * size;
        d.h = orig.h/divs * size;
        d.X.rows = orig.X.rows;
        d.X.cols = d.w*d.h*3;
        d.X.vals = calloc(d.X.rows, કદ(દશાંશ*));

        d.y = copy_matrix(orig.y);
#pragma omp parallel માટે
        માટે(j = 0; j < orig.X.rows; ++j){
            પૂર્ણાંક x = (i%divs) * orig.w / divs - (d.w - orig.w/divs)/2;
            પૂર્ણાંક y = (i/divs) * orig.h / divs - (d.h - orig.h/divs)/2;
            image im = float_to_image(orig.w, orig.h, 3, orig.X.vals[j]);
            d.X.vals[j] = crop_image(im, x, y, d.w, d.h).data;
        }
        ds[i] = d;
    }
    પાછા ds;
}

data resize_data(data orig, પૂર્ણાંક w, પૂર્ણાંક h)
{
    data d = {0};
    d.shallow = 0;
    d.w = w;
    d.h = h;
    પૂર્ણાંક i;
    d.X.rows = orig.X.rows;
    d.X.cols = w*h*3;
    d.X.vals = calloc(d.X.rows, કદ(દશાંશ*));

    d.y = copy_matrix(orig.y);
#pragma omp parallel માટે
    માટે(i = 0; i < orig.X.rows; ++i){
        image im = float_to_image(orig.w, orig.h, 3, orig.X.vals[i]);
        d.X.vals[i] = resize_image(im, w, h).data;
    }
    પાછા d;
}

data load_data_augment(અક્ષર **paths, પૂર્ણાંક n, પૂર્ણાંક m, અક્ષર **labels, પૂર્ણાંક k, વૃક્ષ *hierarchy, પૂર્ણાંક min, પૂર્ણાંક max, પૂર્ણાંક size, દશાંશ angle, દશાંશ aspect, દશાંશ hue, દશાંશ saturation, દશાંશ exposure, પૂર્ણાંક center)
{
    જો(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;
    d.w=size;
    d.h=size;
    d.X = load_image_augment_paths(paths, n, min, max, size, angle, aspect, hue, saturation, exposure, center);
    d.y = load_labels_paths(paths, n, labels, k, hierarchy);
    જો(m) મુક્ત(paths);
    પાછા d;
}

data load_data_tag(અક્ષર **paths, પૂર્ણાંક n, પૂર્ણાંક m, પૂર્ણાંક k, પૂર્ણાંક min, પૂર્ણાંક max, પૂર્ણાંક size, દશાંશ angle, દશાંશ aspect, દશાંશ hue, દશાંશ saturation, દશાંશ exposure)
{
    જો(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.w = size;
    d.h = size;
    d.shallow = 0;
    d.X = load_image_augment_paths(paths, n, min, max, size, angle, aspect, hue, saturation, exposure, 0);
    d.y = load_tags_paths(paths, n, k);
    જો(m) મુક્ત(paths);
    પાછા d;
}

matrix concat_matrix(matrix m1, matrix m2)
{
    પૂર્ણાંક i, ગણતરી = 0;
    matrix m;
    m.cols = m1.cols;
    m.rows = m1.rows+m2.rows;
    m.vals = calloc(m1.rows + m2.rows, કદ(દશાંશ*));
    માટે(i = 0; i < m1.rows; ++i){
        m.vals[ગણતરી++] = m1.vals[i];
    }
    માટે(i = 0; i < m2.rows; ++i){
        m.vals[ગણતરી++] = m2.vals[i];
    }
    પાછા m;
}

data concat_data(data d1, data d2)
{
    data d = {0};
    d.shallow = 1;
    d.X = concat_matrix(d1.X, d2.X);
    d.y = concat_matrix(d1.y, d2.y);
    d.w = d1.w;
    d.h = d1.h;
    પાછા d;
}

data concat_datas(data *d, પૂર્ણાંક n)
{
    પૂર્ણાંક i;
    data out = {0};
    માટે(i = 0; i < n; ++i){
        data new = concat_data(d[i], out);
        free_data(out);
        out = new;
    }
    પાછા out;
}

data load_categorical_data_csv(અક્ષર *filename, પૂર્ણાંક target, પૂર્ણાંક k)
{
    data d = {0};
    d.shallow = 0;
    matrix X = csv_to_matrix(filename);
    દશાંશ *truth_1d = કાઢો_column(&X, target);
    દશાંશ **truth = one_hot_encode(truth_1d, X.rows, k);
    matrix y;
    y.rows = X.rows;
    y.cols = k;
    y.vals = truth;
    d.X = X;
    d.y = y;
    મુક્ત(truth_1d);
    પાછા d;
}

data load_cifar10_data(અક્ષર *filename)
{
    data d = {0};
    d.shallow = 0;
    long i,j;
    matrix X = make_matrix(10000, 3072);
    matrix y = make_matrix(10000, 10);
    d.X = X;
    d.y = y;

    FILE *fp = fopen(filename, "rb");
    જો(!fp) file_error(filename);
    માટે(i = 0; i < 10000; ++i){
        unsigned અક્ષર bytes[3073];
        fread(bytes, 1, 3073, fp);
        પૂર્ણાંક class = bytes[0];
        y.vals[i][class] = 1;
        માટે(j = 0; j < X.cols; ++j){
            X.vals[i][j] = (દ્વિગુણ)bytes[j+1];
        }
    }
    scale_data_rows(d, 1./255);
    //normalize_data_rows(d);
    fclose(fp);
    પાછા d;
}

શૂન્ય get_random_batch(data d, પૂર્ણાંક n, દશાંશ *X, દશાંશ *y)
{
    પૂર્ણાંક j;
    માટે(j = 0; j < n; ++j){
        પૂર્ણાંક index = rand()%d.X.rows;
        memcpy(X+j*d.X.cols, d.X.vals[index], d.X.cols*કદ(દશાંશ));
        memcpy(y+j*d.y.cols, d.y.vals[index], d.y.cols*કદ(દશાંશ));
    }
}

શૂન્ય get_next_batch(data d, પૂર્ણાંક n, પૂર્ણાંક offset, દશાંશ *X, દશાંશ *y)
{
    પૂર્ણાંક j;
    માટે(j = 0; j < n; ++j){
        પૂર્ણાંક index = offset + j;
        memcpy(X+j*d.X.cols, d.X.vals[index], d.X.cols*કદ(દશાંશ));
        જો(y) memcpy(y+j*d.y.cols, d.y.vals[index], d.y.cols*કદ(દશાંશ));
    }
}

શૂન્ય smooth_data(data d)
{
    પૂર્ણાંક i, j;
    દશાંશ scale = 1. / d.y.cols;
    દશાંશ eps = .1;
    માટે(i = 0; i < d.y.rows; ++i){
        માટે(j = 0; j < d.y.cols; ++j){
            d.y.vals[i][j] = eps * scale + (1-eps) * d.y.vals[i][j];
        }
    }
}

data load_all_cifar10()
{
    data d = {0};
    d.shallow = 0;
    પૂર્ણાંક i,j,b;
    matrix X = make_matrix(50000, 3072);
    matrix y = make_matrix(50000, 10);
    d.X = X;
    d.y = y;


    માટે(b = 0; b < 5; ++b){
        અક્ષર buff[256];
        sprintf(buff, "data/cifar/cifar-10-batches-bin/data_batch_%d.bin", b+1);
        FILE *fp = fopen(buff, "rb");
        જો(!fp) file_error(buff);
        માટે(i = 0; i < 10000; ++i){
            unsigned અક્ષર bytes[3073];
            fread(bytes, 1, 3073, fp);
            પૂર્ણાંક class = bytes[0];
            y.vals[i+b*10000][class] = 1;
            માટે(j = 0; j < X.cols; ++j){
                X.vals[i+b*10000][j] = (દ્વિગુણ)bytes[j+1];
            }
        }
        fclose(fp);
    }
    //normalize_data_rows(d);
    scale_data_rows(d, 1./255);
    smooth_data(d);
    પાછા d;
}

data load_go(અક્ષર *filename)
{
    FILE *fp = fopen(filename, "rb");
    matrix X = make_matrix(3363059, 361);
    matrix y = make_matrix(3363059, 361);
    પૂર્ણાંક row, col;

    જો(!fp) file_error(filename);
    અક્ષર *label;
    પૂર્ણાંક ગણતરી = 0;
    જ્યારે((label = fgetl(fp))){
        પૂર્ણાંક i;
        જો(ગણતરી == X.rows){
            X = resize_matrix(X, ગણતરી*2);
            y = resize_matrix(y, ગણતરી*2);
        }
        sscanf(label, "%d %d", &row, &col);
        અક્ષર *board = fgetl(fp);

        પૂર્ણાંક index = row*19 + col;
        y.vals[ગણતરી][index] = 1;

        માટે(i = 0; i < 19*19; ++i){
            દશાંશ val = 0;
            જો(board[i] == '1') val = 1;
            નહીંતર જો(board[i] == '2') val = -1;
            X.vals[ગણતરી][i] = val;
        }
        ++ગણતરી;
        મુક્ત(label);
        મુક્ત(board);
    }
    X = resize_matrix(X, ગણતરી);
    y = resize_matrix(y, ગણતરી);

    data d = {0};
    d.shallow = 0;
    d.X = X;
    d.y = y;


    fclose(fp);

    પાછા d;
}


શૂન્ય randomize_data(data d)
{
    પૂર્ણાંક i;
    માટે(i = d.X.rows-1; i > 0; --i){
        પૂર્ણાંક index = rand()%i;
        દશાંશ *swap = d.X.vals[index];
        d.X.vals[index] = d.X.vals[i];
        d.X.vals[i] = swap;

        swap = d.y.vals[index];
        d.y.vals[index] = d.y.vals[i];
        d.y.vals[i] = swap;
    }
}

શૂન્ય scale_data_rows(data d, દશાંશ s)
{
    પૂર્ણાંક i;
    માટે(i = 0; i < d.X.rows; ++i){
        scale_array(d.X.vals[i], d.X.cols, s);
    }
}

શૂન્ય translate_data_rows(data d, દશાંશ s)
{
    પૂર્ણાંક i;
    માટે(i = 0; i < d.X.rows; ++i){
        translate_array(d.X.vals[i], d.X.cols, s);
    }
}

data copy_data(data d)
{
    data c = {0};
    c.w = d.w;
    c.h = d.h;
    c.shallow = 0;
    c.અંક_boxes = d.અંક_boxes;
    c.boxes = d.boxes;
    c.X = copy_matrix(d.X);
    c.y = copy_matrix(d.y);
    પાછા c;
}

શૂન્ય normalize_data_rows(data d)
{
    પૂર્ણાંક i;
    માટે(i = 0; i < d.X.rows; ++i){
        normalize_array(d.X.vals[i], d.X.cols);
    }
}

data get_data_part(data d, પૂર્ણાંક part, પૂર્ણાંક total)
{
    data p = {0};
    p.shallow = 1;
    p.X.rows = d.X.rows * (part + 1) / total - d.X.rows * part / total;
    p.y.rows = d.y.rows * (part + 1) / total - d.y.rows * part / total;
    p.X.cols = d.X.cols;
    p.y.cols = d.y.cols;
    p.X.vals = d.X.vals + d.X.rows * part / total;
    p.y.vals = d.y.vals + d.y.rows * part / total;
    પાછા p;
}

data get_random_data(data d, પૂર્ણાંક અંક)
{
    data r = {0};
    r.shallow = 1;

    r.X.rows = અંક;
    r.y.rows = અંક;

    r.X.cols = d.X.cols;
    r.y.cols = d.y.cols;

    r.X.vals = calloc(અંક, કદ(દશાંશ *));
    r.y.vals = calloc(અંક, કદ(દશાંશ *));

    પૂર્ણાંક i;
    માટે(i = 0; i < અંક; ++i){
        પૂર્ણાંક index = rand()%d.X.rows;
        r.X.vals[i] = d.X.vals[index];
        r.y.vals[i] = d.y.vals[index];
    }
    પાછા r;
}

data *split_data(data d, પૂર્ણાંક part, પૂર્ણાંક total)
{
    data *split = calloc(2, કદ(data));
    પૂર્ણાંક i;
    પૂર્ણાંક start = part*d.X.rows/total;
    પૂર્ણાંક end = (part+1)*d.X.rows/total;
    data train;
    data test;
    train.shallow = test.shallow = 1;

    test.X.rows = test.y.rows = end-start;
    train.X.rows = train.y.rows = d.X.rows - (end-start);
    train.X.cols = test.X.cols = d.X.cols;
    train.y.cols = test.y.cols = d.y.cols;

    train.X.vals = calloc(train.X.rows, કદ(દશાંશ*));
    test.X.vals = calloc(test.X.rows, કદ(દશાંશ*));
    train.y.vals = calloc(train.y.rows, કદ(દશાંશ*));
    test.y.vals = calloc(test.y.rows, કદ(દશાંશ*));

    માટે(i = 0; i < start; ++i){
        train.X.vals[i] = d.X.vals[i];
        train.y.vals[i] = d.y.vals[i];
    }
    માટે(i = start; i < end; ++i){
        test.X.vals[i-start] = d.X.vals[i];
        test.y.vals[i-start] = d.y.vals[i];
    }
    માટે(i = end; i < d.X.rows; ++i){
        train.X.vals[i-(end-start)] = d.X.vals[i];
        train.y.vals[i-(end-start)] = d.y.vals[i];
    }
    split[0] = train;
    split[1] = test;
    પાછા split;
}

