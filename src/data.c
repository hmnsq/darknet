#അന്തര്ഭാവ "data.h"
#അന്തര്ഭാവ "utils.h"
#അന്തര്ഭാവ "image.h"
#അന്തര്ഭാവ "cuda.h"

#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ <string.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

list *get_paths(അക്ഷരം *filename)
{
    അക്ഷരം *path;
    FILE *file = fopen(filename, "r");
    എങ്കിൽ(!file) file_error(filename);
    list *വരികൾ = make_list();
    ആകുമ്പോൾ((path=fgetl(file))){
        list_തിരുകുക(വരികൾ, path);
    }
    fclose(file);
    തിരികെ വരികൾ;
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

അക്ഷരം **get_random_paths(അക്ഷരം **paths, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ m)
{
    അക്ഷരം **random_paths = calloc(n, വലുപ്പം(അക്ഷരം*));
    പൂർണ്ണസംഖ്യ i;
    pthread_mutex_lock(&mutex);
    വേണ്ടി(i = 0; i < n; ++i){
        പൂർണ്ണസംഖ്യ index = rand()%m;
        random_paths[i] = paths[index];
        //if(i == 0) printf("%s\n", paths[index]);
    }
    pthread_mutex_unlock(&mutex);
    തിരികെ random_paths;
}

അക്ഷരം **find_replace_paths(അക്ഷരം **paths, പൂർണ്ണസംഖ്യ n, അക്ഷരം *find, അക്ഷരം *replace)
{
    അക്ഷരം **replace_paths = calloc(n, വലുപ്പം(അക്ഷരം*));
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < n; ++i){
        അക്ഷരം replaced[4096];
        find_replace(paths[i], find, replace, replaced);
        replace_paths[i] = copy_string(replaced);
    }
    തിരികെ replace_paths;
}

matrix load_image_paths_gray(അക്ഷരം **paths, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h)
{
    പൂർണ്ണസംഖ്യ i;
    matrix X;
    X.rows = n;
    X.vals = calloc(X.rows, വലുപ്പം(ദശാംശം*));
    X.cols = 0;

    വേണ്ടി(i = 0; i < n; ++i){
        image im = load_image(paths[i], w, h, 3);

        image gray = grayscale_image(im);
        free_image(im);
        im = gray;

        X.vals[i] = im.data;
        X.cols = im.h*im.w*im.c;
    }
    തിരികെ X;
}

matrix load_image_paths(അക്ഷരം **paths, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h)
{
    പൂർണ്ണസംഖ്യ i;
    matrix X;
    X.rows = n;
    X.vals = calloc(X.rows, വലുപ്പം(ദശാംശം*));
    X.cols = 0;

    വേണ്ടി(i = 0; i < n; ++i){
        image im = load_image_color(paths[i], w, h);
        X.vals[i] = im.data;
        X.cols = im.h*im.w*im.c;
    }
    തിരികെ X;
}

matrix load_image_augment_paths(അക്ഷരം **paths, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ min, പൂർണ്ണസംഖ്യ max, പൂർണ്ണസംഖ്യ size, ദശാംശം angle, ദശാംശം aspect, ദശാംശം hue, ദശാംശം saturation, ദശാംശം exposure, പൂർണ്ണസംഖ്യ center)
{
    പൂർണ്ണസംഖ്യ i;
    matrix X;
    X.rows = n;
    X.vals = calloc(X.rows, വലുപ്പം(ദശാംശം*));
    X.cols = 0;

    വേണ്ടി(i = 0; i < n; ++i){
        image im = load_image_color(paths[i], 0, 0);
        image crop;
        എങ്കിൽ(center){
            crop = center_crop_image(im, size, size);
        } അല്ലെങ്കിൽ {
            crop = random_augment_image(im, angle, aspect, min, max, size, size);
        }
        പൂർണ്ണസംഖ്യ flip = rand()%2;
        എങ്കിൽ (flip) flip_image(crop);
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
    തിരികെ X;
}


box_label *read_boxes(അക്ഷരം *filename, പൂർണ്ണസംഖ്യ *n)
{
    FILE *file = fopen(filename, "r");
    എങ്കിൽ(!file) file_error(filename);
    ദശാംശം x, y, h, w;
    പൂർണ്ണസംഖ്യ id;
    പൂർണ്ണസംഖ്യ എണ്ണം = 0;
    പൂർണ്ണസംഖ്യ size = 64;
    box_label *boxes = calloc(size, വലുപ്പം(box_label));
    ആകുമ്പോൾ(fscanf(file, "%d %f %f %f %f", &id, &x, &y, &w, &h) == 5){
        എങ്കിൽ(എണ്ണം == size) {
            size = size * 2;
            boxes = realloc(boxes, size*വലുപ്പം(box_label));
        }
        boxes[എണ്ണം].id = id;
        boxes[എണ്ണം].x = x;
        boxes[എണ്ണം].y = y;
        boxes[എണ്ണം].h = h;
        boxes[എണ്ണം].w = w;
        boxes[എണ്ണം].ഇടത്   = x - w/2;
        boxes[എണ്ണം].വലത്  = x + w/2;
        boxes[എണ്ണം].മുകൾ    = y - h/2;
        boxes[എണ്ണം].bottom = y + h/2;
        ++എണ്ണം;
    }
    fclose(file);
    *n = എണ്ണം;
    തിരികെ boxes;
}

ശൂന്യം randomize_boxes(box_label *b, പൂർണ്ണസംഖ്യ n)
{
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < n; ++i){
        box_label swap = b[i];
        പൂർണ്ണസംഖ്യ index = rand()%n;
        b[i] = b[index];
        b[index] = swap;
    }
}

ശൂന്യം correct_boxes(box_label *boxes, പൂർണ്ണസംഖ്യ n, ദശാംശം dx, ദശാംശം dy, ദശാംശം sx, ദശാംശം sy, പൂർണ്ണസംഖ്യ flip)
{
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < n; ++i){
        എങ്കിൽ(boxes[i].x == 0 && boxes[i].y == 0) {
            boxes[i].x = 999999;
            boxes[i].y = 999999;
            boxes[i].w = 999999;
            boxes[i].h = 999999;
            തുടരുക;
        }
        boxes[i].ഇടത്   = boxes[i].ഇടത്  * sx - dx;
        boxes[i].വലത്  = boxes[i].വലത് * sx - dx;
        boxes[i].മുകൾ    = boxes[i].മുകൾ   * sy - dy;
        boxes[i].bottom = boxes[i].bottom* sy - dy;

        എങ്കിൽ(flip){
            ദശാംശം swap = boxes[i].ഇടത്;
            boxes[i].ഇടത് = 1. - boxes[i].വലത്;
            boxes[i].വലത് = 1. - swap;
        }

        boxes[i].ഇടത് =  constrain(0, 1, boxes[i].ഇടത്);
        boxes[i].വലത് = constrain(0, 1, boxes[i].വലത്);
        boxes[i].മുകൾ =   constrain(0, 1, boxes[i].മുകൾ);
        boxes[i].bottom =   constrain(0, 1, boxes[i].bottom);

        boxes[i].x = (boxes[i].ഇടത്+boxes[i].വലത്)/2;
        boxes[i].y = (boxes[i].മുകൾ+boxes[i].bottom)/2;
        boxes[i].w = (boxes[i].വലത് - boxes[i].ഇടത്);
        boxes[i].h = (boxes[i].bottom - boxes[i].മുകൾ);

        boxes[i].w = constrain(0, 1, boxes[i].w);
        boxes[i].h = constrain(0, 1, boxes[i].h);
    }
}

ശൂന്യം fill_truth_swag(അക്ഷരം *path, ദശാംശം *truth, പൂർണ്ണസംഖ്യ classes, പൂർണ്ണസംഖ്യ flip, ദശാംശം dx, ദശാംശം dy, ദശാംശം sx, ദശാംശം sy)
{
    അക്ഷരം labelpath[4096];
    find_replace(path, "images", "labels", labelpath);
    find_replace(labelpath, "JPEGImages", "labels", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);

    പൂർണ്ണസംഖ്യ എണ്ണം = 0;
    box_label *boxes = read_boxes(labelpath, &എണ്ണം);
    randomize_boxes(boxes, എണ്ണം);
    correct_boxes(boxes, എണ്ണം, dx, dy, sx, sy, flip);
    ദശാംശം x,y,w,h;
    പൂർണ്ണസംഖ്യ id;
    പൂർണ്ണസംഖ്യ i;

    വേണ്ടി (i = 0; i < എണ്ണം && i < 90; ++i) {
        x =  boxes[i].x;
        y =  boxes[i].y;
        w =  boxes[i].w;
        h =  boxes[i].h;
        id = boxes[i].id;

        എങ്കിൽ (w < .0 || h < .0) തുടരുക;

        പൂർണ്ണസംഖ്യ index = (4+classes) * i;

        truth[index++] = x;
        truth[index++] = y;
        truth[index++] = w;
        truth[index++] = h;

        എങ്കിൽ (id < classes) truth[index+id] = 1;
    }
    സ്വതന്ത്രം(boxes);
}

ശൂന്യം fill_truth_region(അക്ഷരം *path, ദശാംശം *truth, പൂർണ്ണസംഖ്യ classes, പൂർണ്ണസംഖ്യ അക്കം_boxes, പൂർണ്ണസംഖ്യ flip, ദശാംശം dx, ദശാംശം dy, ദശാംശം sx, ദശാംശം sy)
{
    അക്ഷരം labelpath[4096];
    find_replace(path, "images", "labels", labelpath);
    find_replace(labelpath, "JPEGImages", "labels", labelpath);

    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".png", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    പൂർണ്ണസംഖ്യ എണ്ണം = 0;
    box_label *boxes = read_boxes(labelpath, &എണ്ണം);
    randomize_boxes(boxes, എണ്ണം);
    correct_boxes(boxes, എണ്ണം, dx, dy, sx, sy, flip);
    ദശാംശം x,y,w,h;
    പൂർണ്ണസംഖ്യ id;
    പൂർണ്ണസംഖ്യ i;

    വേണ്ടി (i = 0; i < എണ്ണം; ++i) {
        x =  boxes[i].x;
        y =  boxes[i].y;
        w =  boxes[i].w;
        h =  boxes[i].h;
        id = boxes[i].id;

        എങ്കിൽ (w < .005 || h < .005) തുടരുക;

        പൂർണ്ണസംഖ്യ col = (പൂർണ്ണസംഖ്യ)(x*അക്കം_boxes);
        പൂർണ്ണസംഖ്യ row = (പൂർണ്ണസംഖ്യ)(y*അക്കം_boxes);

        x = x*അക്കം_boxes - col;
        y = y*അക്കം_boxes - row;

        പൂർണ്ണസംഖ്യ index = (col+row*അക്കം_boxes)*(5+classes);
        എങ്കിൽ (truth[index]) തുടരുക;
        truth[index++] = 1;

        എങ്കിൽ (id < classes) truth[index+id] = 1;
        index += classes;

        truth[index++] = x;
        truth[index++] = y;
        truth[index++] = w;
        truth[index++] = h;
    }
    സ്വതന്ത്രം(boxes);
}

ശൂന്യം load_rle(image im, പൂർണ്ണസംഖ്യ *rle, പൂർണ്ണസംഖ്യ n)
{
    പൂർണ്ണസംഖ്യ എണ്ണം = 0;
    പൂർണ്ണസംഖ്യ curr = 0;
    പൂർണ്ണസംഖ്യ i,j;
    വേണ്ടി(i = 0; i < n; ++i){
        വേണ്ടി(j = 0; j < rle[i]; ++j){
            im.data[എണ്ണം++] = curr;
        }
        curr = 1 - curr;
    }
    വേണ്ടി(; എണ്ണം < im.h*im.w*im.c; ++എണ്ണം){
        im.data[എണ്ണം] = curr;
    }
}

ശൂന്യം or_image(image src, image dest, പൂർണ്ണസംഖ്യ c)
{
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < src.w*src.h; ++i){
        എങ്കിൽ(src.data[i]) dest.data[dest.w*dest.h*c + i] = 1;
    }
}

ശൂന്യം exclusive_image(image src)
{
    പൂർണ്ണസംഖ്യ k, j, i;
    പൂർണ്ണസംഖ്യ s = src.w*src.h;
    വേണ്ടി(k = 0; k < src.c-1; ++k){
        വേണ്ടി(i = 0; i < s; ++i){
            എങ്കിൽ (src.data[k*s + i]){
                വേണ്ടി(j = k+1; j < src.c; ++j){
                    src.data[j*s + i] = 0;
                }
            }
        }
    }
}

box bound_image(image im)
{
    പൂർണ്ണസംഖ്യ x,y;
    പൂർണ്ണസംഖ്യ minx = im.w;
    പൂർണ്ണസംഖ്യ miny = im.h;
    പൂർണ്ണസംഖ്യ maxx = 0;
    പൂർണ്ണസംഖ്യ maxy = 0;
    വേണ്ടി(y = 0; y < im.h; ++y){
        വേണ്ടി(x = 0; x < im.w; ++x){
            എങ്കിൽ(im.data[y*im.w + x]){
                minx = (x < minx) ? x : minx;
                miny = (y < miny) ? y : miny;
                maxx = (x > maxx) ? x : maxx;
                maxy = (y > maxy) ? y : maxy;
            }
        }
    }
    box b = {minx, miny, maxx-minx + 1, maxy-miny + 1};
    //printf("%f %f %f %f\n", b.x, b.y, b.w, b.h);
    തിരികെ b;
}

ശൂന്യം fill_truth_iseg(അക്ഷരം *path, പൂർണ്ണസംഖ്യ അക്കം_boxes, ദശാംശം *truth, പൂർണ്ണസംഖ്യ classes, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h, augment_args aug, പൂർണ്ണസംഖ്യ flip, പൂർണ്ണസംഖ്യ mw, പൂർണ്ണസംഖ്യ mh)
{
    അക്ഷരം labelpath[4096];
    find_replace(path, "images", "mask", labelpath);
    find_replace(labelpath, "JPEGImages", "mask", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    FILE *file = fopen(labelpath, "r");
    എങ്കിൽ(!file) file_error(labelpath);
    അക്ഷരം buff[32788];
    പൂർണ്ണസംഖ്യ id;
    പൂർണ്ണസംഖ്യ i = 0;
    പൂർണ്ണസംഖ്യ j;
    image part = make_image(w, h, 1);
    ആകുമ്പോൾ((fscanf(file, "%d %s", &id, buff) == 2) && i < അക്കം_boxes){
        പൂർണ്ണസംഖ്യ n = 0;
        പൂർണ്ണസംഖ്യ *rle = read_intlist(buff, &n, 0);
        load_rle(part, rle, n);
        image sized = rotate_crop_image(part, aug.rad, aug.scale, aug.w, aug.h, aug.dx, aug.dy, aug.aspect);
        എങ്കിൽ(flip) flip_image(sized);

        image mask = resize_image(sized, mw, mh);
        truth[i*(mw*mh+1)] = id;
        വേണ്ടി(j = 0; j < mw*mh; ++j){
            truth[i*(mw*mh + 1) + 1 + j] = mask.data[j];
        }
        ++i;

        free_image(mask);
        free_image(sized);
        സ്വതന്ത്രം(rle);
    }
    എങ്കിൽ(i < അക്കം_boxes) truth[i*(mw*mh+1)] = -1;
    fclose(file);
    free_image(part);
}

ശൂന്യം fill_truth_mask(അക്ഷരം *path, പൂർണ്ണസംഖ്യ അക്കം_boxes, ദശാംശം *truth, പൂർണ്ണസംഖ്യ classes, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h, augment_args aug, പൂർണ്ണസംഖ്യ flip, പൂർണ്ണസംഖ്യ mw, പൂർണ്ണസംഖ്യ mh)
{
    അക്ഷരം labelpath[4096];
    find_replace(path, "images", "mask", labelpath);
    find_replace(labelpath, "JPEGImages", "mask", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    FILE *file = fopen(labelpath, "r");
    എങ്കിൽ(!file) file_error(labelpath);
    അക്ഷരം buff[32788];
    പൂർണ്ണസംഖ്യ id;
    പൂർണ്ണസംഖ്യ i = 0;
    image part = make_image(w, h, 1);
    ആകുമ്പോൾ((fscanf(file, "%d %s", &id, buff) == 2) && i < അക്കം_boxes){
        പൂർണ്ണസംഖ്യ n = 0;
        പൂർണ്ണസംഖ്യ *rle = read_intlist(buff, &n, 0);
        load_rle(part, rle, n);
        image sized = rotate_crop_image(part, aug.rad, aug.scale, aug.w, aug.h, aug.dx, aug.dy, aug.aspect);
        എങ്കിൽ(flip) flip_image(sized);
        box b = bound_image(sized);
        എങ്കിൽ(b.w > 0){
            image crop = crop_image(sized, b.x, b.y, b.w, b.h);
            image mask = resize_image(crop, mw, mh);
            truth[i*(4 + mw*mh + 1) + 0] = (b.x + b.w/2.)/sized.w;
            truth[i*(4 + mw*mh + 1) + 1] = (b.y + b.h/2.)/sized.h;
            truth[i*(4 + mw*mh + 1) + 2] = b.w/sized.w;
            truth[i*(4 + mw*mh + 1) + 3] = b.h/sized.h;
            പൂർണ്ണസംഖ്യ j;
            വേണ്ടി(j = 0; j < mw*mh; ++j){
                truth[i*(4 + mw*mh + 1) + 4 + j] = mask.data[j];
            }
            truth[i*(4 + mw*mh + 1) + 4 + mw*mh] = id;
            free_image(crop);
            free_image(mask);
            ++i;
        }
        free_image(sized);
        സ്വതന്ത്രം(rle);
    }
    fclose(file);
    free_image(part);
}


ശൂന്യം fill_truth_detection(അക്ഷരം *path, പൂർണ്ണസംഖ്യ അക്കം_boxes, ദശാംശം *truth, പൂർണ്ണസംഖ്യ classes, പൂർണ്ണസംഖ്യ flip, ദശാംശം dx, ദശാംശം dy, ദശാംശം sx, ദശാംശം sy)
{
    അക്ഷരം labelpath[4096];
    find_replace(path, "images", "labels", labelpath);
    find_replace(labelpath, "JPEGImages", "labels", labelpath);

    find_replace(labelpath, "raw", "labels", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".png", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    പൂർണ്ണസംഖ്യ എണ്ണം = 0;
    box_label *boxes = read_boxes(labelpath, &എണ്ണം);
    randomize_boxes(boxes, എണ്ണം);
    correct_boxes(boxes, എണ്ണം, dx, dy, sx, sy, flip);
    എങ്കിൽ(എണ്ണം > അക്കം_boxes) എണ്ണം = അക്കം_boxes;
    ദശാംശം x,y,w,h;
    പൂർണ്ണസംഖ്യ id;
    പൂർണ്ണസംഖ്യ i;
    പൂർണ്ണസംഖ്യ sub = 0;

    വേണ്ടി (i = 0; i < എണ്ണം; ++i) {
        x =  boxes[i].x;
        y =  boxes[i].y;
        w =  boxes[i].w;
        h =  boxes[i].h;
        id = boxes[i].id;

        എങ്കിൽ ((w < .001 || h < .001)) {
            ++sub;
            തുടരുക;
        }

        truth[(i-sub)*5+0] = x;
        truth[(i-sub)*5+1] = y;
        truth[(i-sub)*5+2] = w;
        truth[(i-sub)*5+3] = h;
        truth[(i-sub)*5+4] = id;
    }
    സ്വതന്ത്രം(boxes);
}

#define NUMCHARS 37

ശൂന്യം print_letters(ദശാംശം *pred, പൂർണ്ണസംഖ്യ n)
{
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < n; ++i){
        പൂർണ്ണസംഖ്യ index = max_index(pred+i*NUMCHARS, NUMCHARS);
        printf("%c", int_to_alphanum(index));
    }
    printf("\n");
}

ശൂന്യം fill_truth_captcha(അക്ഷരം *path, പൂർണ്ണസംഖ്യ n, ദശാംശം *truth)
{
    അക്ഷരം *begin = strrchr(path, '/');
    ++begin;
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < strlen(begin) && i < n && begin[i] != '.'; ++i){
        പൂർണ്ണസംഖ്യ index = alphanum_to_int(begin[i]);
        എങ്കിൽ(index > 35) printf("Bad %c\n", begin[i]);
        truth[i*NUMCHARS+index] = 1;
    }
    വേണ്ടി(;i < n; ++i){
        truth[i*NUMCHARS + NUMCHARS-1] = 1;
    }
}

data load_data_captcha(അക്ഷരം **paths, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ m, പൂർണ്ണസംഖ്യ k, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h)
{
    എങ്കിൽ(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;
    d.X = load_image_paths(paths, n, w, h);
    d.y = make_matrix(n, k*NUMCHARS);
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < n; ++i){
        fill_truth_captcha(paths[i], k, d.y.vals[i]);
    }
    എങ്കിൽ(m) സ്വതന്ത്രം(paths);
    തിരികെ d;
}

data load_data_captcha_encode(അക്ഷരം **paths, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ m, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h)
{
    എങ്കിൽ(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;
    d.X = load_image_paths(paths, n, w, h);
    d.X.cols = 17100;
    d.y = d.X;
    എങ്കിൽ(m) സ്വതന്ത്രം(paths);
    തിരികെ d;
}

ശൂന്യം fill_truth(അക്ഷരം *path, അക്ഷരം **labels, പൂർണ്ണസംഖ്യ k, ദശാംശം *truth)
{
    പൂർണ്ണസംഖ്യ i;
    memset(truth, 0, k*വലുപ്പം(ദശാംശം));
    പൂർണ്ണസംഖ്യ എണ്ണം = 0;
    വേണ്ടി(i = 0; i < k; ++i){
        എങ്കിൽ(strstr(path, labels[i])){
            truth[i] = 1;
            ++എണ്ണം;
            //printf("%s %s %d\n", path, labels[i], i);
        }
    }
    എങ്കിൽ(എണ്ണം != 1 && (k != 1 || എണ്ണം != 0)) printf("Too many or too few labels: %d, %s\n", എണ്ണം, path);
}

ശൂന്യം fill_hierarchy(ദശാംശം *truth, പൂർണ്ണസംഖ്യ k, വൃക്ഷം *hierarchy)
{
    പൂർണ്ണസംഖ്യ j;
    വേണ്ടി(j = 0; j < k; ++j){
        എങ്കിൽ(truth[j]){
            പൂർണ്ണസംഖ്യ parent = hierarchy->parent[j];
            ആകുമ്പോൾ(parent >= 0){
                truth[parent] = 1;
                parent = hierarchy->parent[parent];
            }
        }
    }
    പൂർണ്ണസംഖ്യ i;
    പൂർണ്ണസംഖ്യ എണ്ണം = 0;
    വേണ്ടി(j = 0; j < hierarchy->groups; ++j){
        //printf("%d\n", count);
        പൂർണ്ണസംഖ്യ mask = 1;
        വേണ്ടി(i = 0; i < hierarchy->group_size[j]; ++i){
            എങ്കിൽ(truth[എണ്ണം + i]){
                mask = 0;
                നിർത്തുക;
            }
        }
        എങ്കിൽ (mask) {
            വേണ്ടി(i = 0; i < hierarchy->group_size[j]; ++i){
                truth[എണ്ണം + i] = SECRET_അക്കം;
            }
        }
        എണ്ണം += hierarchy->group_size[j];
    }
}

matrix load_regression_labels_paths(അക്ഷരം **paths, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ k)
{
    matrix y = make_matrix(n, k);
    പൂർണ്ണസംഖ്യ i,j;
    വേണ്ടി(i = 0; i < n; ++i){
        അക്ഷരം labelpath[4096];
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
        വേണ്ടി(j = 0; j < k; ++j){
            fscanf(file, "%f", &(y.vals[i][j]));
        }
        fclose(file);
    }
    തിരികെ y;
}

matrix load_labels_paths(അക്ഷരം **paths, പൂർണ്ണസംഖ്യ n, അക്ഷരം **labels, പൂർണ്ണസംഖ്യ k, വൃക്ഷം *hierarchy)
{
    matrix y = make_matrix(n, k);
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < n && labels; ++i){
        fill_truth(paths[i], labels, k, y.vals[i]);
        എങ്കിൽ(hierarchy){
            fill_hierarchy(y.vals[i], k, hierarchy);
        }
    }
    തിരികെ y;
}

matrix load_tags_paths(അക്ഷരം **paths, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ k)
{
    matrix y = make_matrix(n, k);
    പൂർണ്ണസംഖ്യ i;
    //int count = 0;
    വേണ്ടി(i = 0; i < n; ++i){
        അക്ഷരം label[4096];
        find_replace(paths[i], "images", "labels", label);
        find_replace(label, ".jpg", ".txt", label);
        FILE *file = fopen(label, "r");
        എങ്കിൽ (!file) തുടരുക;
        //++count;
        പൂർണ്ണസംഖ്യ tag;
        ആകുമ്പോൾ(fscanf(file, "%d", &tag) == 1){
            എങ്കിൽ(tag < k){
                y.vals[i][tag] = 1;
            }
        }
        fclose(file);
    }
    //printf("%d/%d\n", count, n);
    തിരികെ y;
}

അക്ഷരം **get_labels(അക്ഷരം *filename)
{
    list *plist = get_paths(filename);
    അക്ഷരം **labels = (അക്ഷരം **)list_to_array(plist);
    free_list(plist);
    തിരികെ labels;
}

ശൂന്യം free_data(data d)
{
    എങ്കിൽ(!d.shallow){
        free_matrix(d.X);
        free_matrix(d.y);
    }അല്ലെങ്കിൽ{
        സ്വതന്ത്രം(d.X.vals);
        സ്വതന്ത്രം(d.y.vals);
    }
}

image get_segmentation_image(അക്ഷരം *path, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h, പൂർണ്ണസംഖ്യ classes)
{
    അക്ഷരം labelpath[4096];
    find_replace(path, "images", "mask", labelpath);
    find_replace(labelpath, "JPEGImages", "mask", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    image mask = make_image(w, h, classes);
    FILE *file = fopen(labelpath, "r");
    എങ്കിൽ(!file) file_error(labelpath);
    അക്ഷരം buff[32788];
    പൂർണ്ണസംഖ്യ id;
    image part = make_image(w, h, 1);
    ആകുമ്പോൾ(fscanf(file, "%d %s", &id, buff) == 2){
        പൂർണ്ണസംഖ്യ n = 0;
        പൂർണ്ണസംഖ്യ *rle = read_intlist(buff, &n, 0);
        load_rle(part, rle, n);
        or_image(part, mask, id);
        സ്വതന്ത്രം(rle);
    }
    //exclusive_image(mask);
    fclose(file);
    free_image(part);
    തിരികെ mask;
}

image get_segmentation_image2(അക്ഷരം *path, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h, പൂർണ്ണസംഖ്യ classes)
{
    അക്ഷരം labelpath[4096];
    find_replace(path, "images", "mask", labelpath);
    find_replace(labelpath, "JPEGImages", "mask", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    image mask = make_image(w, h, classes+1);
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < w*h; ++i){
        mask.data[w*h*classes + i] = 1;
    }
    FILE *file = fopen(labelpath, "r");
    എങ്കിൽ(!file) file_error(labelpath);
    അക്ഷരം buff[32788];
    പൂർണ്ണസംഖ്യ id;
    image part = make_image(w, h, 1);
    ആകുമ്പോൾ(fscanf(file, "%d %s", &id, buff) == 2){
        പൂർണ്ണസംഖ്യ n = 0;
        പൂർണ്ണസംഖ്യ *rle = read_intlist(buff, &n, 0);
        load_rle(part, rle, n);
        or_image(part, mask, id);
        വേണ്ടി(i = 0; i < w*h; ++i){
            എങ്കിൽ(part.data[i]) mask.data[w*h*classes + i] = 0;
        }
        സ്വതന്ത്രം(rle);
    }
    //exclusive_image(mask);
    fclose(file);
    free_image(part);
    തിരികെ mask;
}

data load_data_seg(പൂർണ്ണസംഖ്യ n, അക്ഷരം **paths, പൂർണ്ണസംഖ്യ m, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h, പൂർണ്ണസംഖ്യ classes, പൂർണ്ണസംഖ്യ min, പൂർണ്ണസംഖ്യ max, ദശാംശം angle, ദശാംശം aspect, ദശാംശം hue, ദശാംശം saturation, ദശാംശം exposure, പൂർണ്ണസംഖ്യ div)
{
    അക്ഷരം **random_paths = get_random_paths(paths, n, m);
    പൂർണ്ണസംഖ്യ i;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, വലുപ്പം(ദശാംശം*));
    d.X.cols = h*w*3;


    d.y.rows = n;
    d.y.cols = h*w*classes/div/div;
    d.y.vals = calloc(d.X.rows, വലുപ്പം(ദശാംശം*));

    വേണ്ടി(i = 0; i < n; ++i){
        image orig = load_image_color(random_paths[i], 0, 0);
        augment_args a = random_augment_args(orig, angle, aspect, min, max, w, h);
        image sized = rotate_crop_image(orig, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);

        പൂർണ്ണസംഖ്യ flip = rand()%2;
        എങ്കിൽ(flip) flip_image(sized);
        random_distort_image(sized, hue, saturation, exposure);
        d.X.vals[i] = sized.data;

        image mask = get_segmentation_image(random_paths[i], orig.w, orig.h, classes);
        //image mask = make_image(orig.w, orig.h, classes+1);
        image sized_m = rotate_crop_image(mask, a.rad, a.scale/div, a.w/div, a.h/div, a.dx/div, a.dy/div, a.aspect);

        എങ്കിൽ(flip) flip_image(sized_m);
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
    സ്വതന്ത്രം(random_paths);
    തിരികെ d;
}

data load_data_iseg(പൂർണ്ണസംഖ്യ n, അക്ഷരം **paths, പൂർണ്ണസംഖ്യ m, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h, പൂർണ്ണസംഖ്യ classes, പൂർണ്ണസംഖ്യ boxes, പൂർണ്ണസംഖ്യ div, പൂർണ്ണസംഖ്യ min, പൂർണ്ണസംഖ്യ max, ദശാംശം angle, ദശാംശം aspect, ദശാംശം hue, ദശാംശം saturation, ദശാംശം exposure)
{
    അക്ഷരം **random_paths = get_random_paths(paths, n, m);
    പൂർണ്ണസംഖ്യ i;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, വലുപ്പം(ദശാംശം*));
    d.X.cols = h*w*3;

    d.y = make_matrix(n, (((w/div)*(h/div))+1)*boxes);

    വേണ്ടി(i = 0; i < n; ++i){
        image orig = load_image_color(random_paths[i], 0, 0);
        augment_args a = random_augment_args(orig, angle, aspect, min, max, w, h);
        image sized = rotate_crop_image(orig, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);

        പൂർണ്ണസംഖ്യ flip = rand()%2;
        എങ്കിൽ(flip) flip_image(sized);
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
    സ്വതന്ത്രം(random_paths);
    തിരികെ d;
}

data load_data_mask(പൂർണ്ണസംഖ്യ n, അക്ഷരം **paths, പൂർണ്ണസംഖ്യ m, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h, പൂർണ്ണസംഖ്യ classes, പൂർണ്ണസംഖ്യ boxes, പൂർണ്ണസംഖ്യ coords, പൂർണ്ണസംഖ്യ min, പൂർണ്ണസംഖ്യ max, ദശാംശം angle, ദശാംശം aspect, ദശാംശം hue, ദശാംശം saturation, ദശാംശം exposure)
{
    അക്ഷരം **random_paths = get_random_paths(paths, n, m);
    പൂർണ്ണസംഖ്യ i;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, വലുപ്പം(ദശാംശം*));
    d.X.cols = h*w*3;

    d.y = make_matrix(n, (coords+1)*boxes);

    വേണ്ടി(i = 0; i < n; ++i){
        image orig = load_image_color(random_paths[i], 0, 0);
        augment_args a = random_augment_args(orig, angle, aspect, min, max, w, h);
        image sized = rotate_crop_image(orig, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);

        പൂർണ്ണസംഖ്യ flip = rand()%2;
        എങ്കിൽ(flip) flip_image(sized);
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
    സ്വതന്ത്രം(random_paths);
    തിരികെ d;
}

data load_data_region(പൂർണ്ണസംഖ്യ n, അക്ഷരം **paths, പൂർണ്ണസംഖ്യ m, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h, പൂർണ്ണസംഖ്യ size, പൂർണ്ണസംഖ്യ classes, ദശാംശം jitter, ദശാംശം hue, ദശാംശം saturation, ദശാംശം exposure)
{
    അക്ഷരം **random_paths = get_random_paths(paths, n, m);
    പൂർണ്ണസംഖ്യ i;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, വലുപ്പം(ദശാംശം*));
    d.X.cols = h*w*3;


    പൂർണ്ണസംഖ്യ k = size*size*(5+classes);
    d.y = make_matrix(n, k);
    വേണ്ടി(i = 0; i < n; ++i){
        image orig = load_image_color(random_paths[i], 0, 0);

        പൂർണ്ണസംഖ്യ oh = orig.h;
        പൂർണ്ണസംഖ്യ ow = orig.w;

        പൂർണ്ണസംഖ്യ dw = (ow*jitter);
        പൂർണ്ണസംഖ്യ dh = (oh*jitter);

        പൂർണ്ണസംഖ്യ pleft  = rand_uniform(-dw, dw);
        പൂർണ്ണസംഖ്യ pright = rand_uniform(-dw, dw);
        പൂർണ്ണസംഖ്യ ptop   = rand_uniform(-dh, dh);
        പൂർണ്ണസംഖ്യ pbot   = rand_uniform(-dh, dh);

        പൂർണ്ണസംഖ്യ swidth =  ow - pleft - pright;
        പൂർണ്ണസംഖ്യ sheight = oh - ptop - pbot;

        ദശാംശം sx = (ദശാംശം)swidth  / ow;
        ദശാംശം sy = (ദശാംശം)sheight / oh;

        പൂർണ്ണസംഖ്യ flip = rand()%2;
        image cropped = crop_image(orig, pleft, ptop, swidth, sheight);

        ദശാംശം dx = ((ദശാംശം)pleft/ow)/sx;
        ദശാംശം dy = ((ദശാംശം)ptop /oh)/sy;

        image sized = resize_image(cropped, w, h);
        എങ്കിൽ(flip) flip_image(sized);
        random_distort_image(sized, hue, saturation, exposure);
        d.X.vals[i] = sized.data;

        fill_truth_region(random_paths[i], d.y.vals[i], classes, size, flip, dx, dy, 1./sx, 1./sy);

        free_image(orig);
        free_image(cropped);
    }
    സ്വതന്ത്രം(random_paths);
    തിരികെ d;
}

data load_data_compare(പൂർണ്ണസംഖ്യ n, അക്ഷരം **paths, പൂർണ്ണസംഖ്യ m, പൂർണ്ണസംഖ്യ classes, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h)
{
    എങ്കിൽ(m) paths = get_random_paths(paths, 2*n, m);
    പൂർണ്ണസംഖ്യ i,j;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, വലുപ്പം(ദശാംശം*));
    d.X.cols = h*w*6;

    പൂർണ്ണസംഖ്യ k = 2*(classes);
    d.y = make_matrix(n, k);
    വേണ്ടി(i = 0; i < n; ++i){
        image im1 = load_image_color(paths[i*2],   w, h);
        image im2 = load_image_color(paths[i*2+1], w, h);

        d.X.vals[i] = calloc(d.X.cols, വലുപ്പം(ദശാംശം));
        memcpy(d.X.vals[i],         im1.data, h*w*3*വലുപ്പം(ദശാംശം));
        memcpy(d.X.vals[i] + h*w*3, im2.data, h*w*3*വലുപ്പം(ദശാംശം));

        പൂർണ്ണസംഖ്യ id;
        ദശാംശം iou;

        അക്ഷരം imlabel1[4096];
        അക്ഷരം imlabel2[4096];
        find_replace(paths[i*2],   "imgs", "labels", imlabel1);
        find_replace(imlabel1, "jpg", "txt", imlabel1);
        FILE *fp1 = fopen(imlabel1, "r");

        ആകുമ്പോൾ(fscanf(fp1, "%d %f", &id, &iou) == 2){
            എങ്കിൽ (d.y.vals[i][2*id] < iou) d.y.vals[i][2*id] = iou;
        }

        find_replace(paths[i*2+1], "imgs", "labels", imlabel2);
        find_replace(imlabel2, "jpg", "txt", imlabel2);
        FILE *fp2 = fopen(imlabel2, "r");

        ആകുമ്പോൾ(fscanf(fp2, "%d %f", &id, &iou) == 2){
            എങ്കിൽ (d.y.vals[i][2*id + 1] < iou) d.y.vals[i][2*id + 1] = iou;
        }

        വേണ്ടി (j = 0; j < classes; ++j){
            എങ്കിൽ (d.y.vals[i][2*j] > .5 &&  d.y.vals[i][2*j+1] < .5){
                d.y.vals[i][2*j] = 1;
                d.y.vals[i][2*j+1] = 0;
            } അല്ലെങ്കിൽ എങ്കിൽ (d.y.vals[i][2*j] < .5 &&  d.y.vals[i][2*j+1] > .5){
                d.y.vals[i][2*j] = 0;
                d.y.vals[i][2*j+1] = 1;
            } അല്ലെങ്കിൽ {
                d.y.vals[i][2*j]   = SECRET_അക്കം;
                d.y.vals[i][2*j+1] = SECRET_അക്കം;
            }
        }
        fclose(fp1);
        fclose(fp2);

        free_image(im1);
        free_image(im2);
    }
    എങ്കിൽ(m) സ്വതന്ത്രം(paths);
    തിരികെ d;
}

data load_data_swag(അക്ഷരം **paths, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ classes, ദശാംശം jitter)
{
    പൂർണ്ണസംഖ്യ index = rand()%n;
    അക്ഷരം *random_path = paths[index];

    image orig = load_image_color(random_path, 0, 0);
    പൂർണ്ണസംഖ്യ h = orig.h;
    പൂർണ്ണസംഖ്യ w = orig.w;

    data d = {0};
    d.shallow = 0;
    d.w = w;
    d.h = h;

    d.X.rows = 1;
    d.X.vals = calloc(d.X.rows, വലുപ്പം(ദശാംശം*));
    d.X.cols = h*w*3;

    പൂർണ്ണസംഖ്യ k = (4+classes)*90;
    d.y = make_matrix(1, k);

    പൂർണ്ണസംഖ്യ dw = w*jitter;
    പൂർണ്ണസംഖ്യ dh = h*jitter;

    പൂർണ്ണസംഖ്യ pleft  = rand_uniform(-dw, dw);
    പൂർണ്ണസംഖ്യ pright = rand_uniform(-dw, dw);
    പൂർണ്ണസംഖ്യ ptop   = rand_uniform(-dh, dh);
    പൂർണ്ണസംഖ്യ pbot   = rand_uniform(-dh, dh);

    പൂർണ്ണസംഖ്യ swidth =  w - pleft - pright;
    പൂർണ്ണസംഖ്യ sheight = h - ptop - pbot;

    ദശാംശം sx = (ദശാംശം)swidth  / w;
    ദശാംശം sy = (ദശാംശം)sheight / h;

    പൂർണ്ണസംഖ്യ flip = rand()%2;
    image cropped = crop_image(orig, pleft, ptop, swidth, sheight);

    ദശാംശം dx = ((ദശാംശം)pleft/w)/sx;
    ദശാംശം dy = ((ദശാംശം)ptop /h)/sy;

    image sized = resize_image(cropped, w, h);
    എങ്കിൽ(flip) flip_image(sized);
    d.X.vals[0] = sized.data;

    fill_truth_swag(random_path, d.y.vals[0], classes, flip, dx, dy, 1./sx, 1./sy);

    free_image(orig);
    free_image(cropped);

    തിരികെ d;
}

data load_data_detection(പൂർണ്ണസംഖ്യ n, അക്ഷരം **paths, പൂർണ്ണസംഖ്യ m, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h, പൂർണ്ണസംഖ്യ boxes, പൂർണ്ണസംഖ്യ classes, ദശാംശം jitter, ദശാംശം hue, ദശാംശം saturation, ദശാംശം exposure)
{
    അക്ഷരം **random_paths = get_random_paths(paths, n, m);
    പൂർണ്ണസംഖ്യ i;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = calloc(d.X.rows, വലുപ്പം(ദശാംശം*));
    d.X.cols = h*w*3;

    d.y = make_matrix(n, 5*boxes);
    വേണ്ടി(i = 0; i < n; ++i){
        image orig = load_image_color(random_paths[i], 0, 0);
        image sized = make_image(w, h, orig.c);
        fill_image(sized, .5);

        ദശാംശം dw = jitter * orig.w;
        ദശാംശം dh = jitter * orig.h;

        ദശാംശം new_ar = (orig.w + rand_uniform(-dw, dw)) / (orig.h + rand_uniform(-dh, dh));
        //float scale = rand_uniform(.25, 2);
        ദശാംശം scale = 1;

        ദശാംശം nw, nh;

        എങ്കിൽ(new_ar < 1){
            nh = scale * h;
            nw = nh * new_ar;
        } അല്ലെങ്കിൽ {
            nw = scale * w;
            nh = nw / new_ar;
        }

        ദശാംശം dx = rand_uniform(0, w - nw);
        ദശാംശം dy = rand_uniform(0, h - nh);

        place_image(orig, nw, nh, dx, dy, sized);

        random_distort_image(sized, hue, saturation, exposure);

        പൂർണ്ണസംഖ്യ flip = rand()%2;
        എങ്കിൽ(flip) flip_image(sized);
        d.X.vals[i] = sized.data;


        fill_truth_detection(random_paths[i], boxes, d.y.vals[i], classes, flip, -dx/w, -dy/h, nw/w, nh/h);

        free_image(orig);
    }
    സ്വതന്ത്രം(random_paths);
    തിരികെ d;
}

ശൂന്യം *load_thread(ശൂന്യം *ptr)
{
    //printf("Loading data: %d\n", rand());
    load_args a = *(ഘടന load_args*)ptr;
    എങ്കിൽ(a.exposure == 0) a.exposure = 1;
    എങ്കിൽ(a.saturation == 0) a.saturation = 1;
    എങ്കിൽ(a.aspect == 0) a.aspect = 1;

    എങ്കിൽ (a.type == OLD_CLASSIFICATION_DATA){
        *a.d = load_data_old(a.paths, a.n, a.m, a.labels, a.classes, a.w, a.h);
    } അല്ലെങ്കിൽ എങ്കിൽ (a.type == REGRESSION_DATA){
        *a.d = load_data_regression(a.paths, a.n, a.m, a.classes, a.min, a.max, a.size, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    } അല്ലെങ്കിൽ എങ്കിൽ (a.type == CLASSIFICATION_DATA){
        *a.d = load_data_augment(a.paths, a.n, a.m, a.labels, a.classes, a.hierarchy, a.min, a.max, a.size, a.angle, a.aspect, a.hue, a.saturation, a.exposure, a.center);
    } അല്ലെങ്കിൽ എങ്കിൽ (a.type == SUPER_DATA){
        *a.d = load_data_super(a.paths, a.n, a.m, a.w, a.h, a.scale);
    } അല്ലെങ്കിൽ എങ്കിൽ (a.type == WRITING_DATA){
        *a.d = load_data_writing(a.paths, a.n, a.m, a.w, a.h, a.out_w, a.out_h);
    } അല്ലെങ്കിൽ എങ്കിൽ (a.type == ISEG_DATA){
        *a.d = load_data_iseg(a.n, a.paths, a.m, a.w, a.h, a.classes, a.അക്കം_boxes, a.scale, a.min, a.max, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    } അല്ലെങ്കിൽ എങ്കിൽ (a.type == INSTANCE_DATA){
        *a.d = load_data_mask(a.n, a.paths, a.m, a.w, a.h, a.classes, a.അക്കം_boxes, a.coords, a.min, a.max, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    } അല്ലെങ്കിൽ എങ്കിൽ (a.type == SEGMENTATION_DATA){
        *a.d = load_data_seg(a.n, a.paths, a.m, a.w, a.h, a.classes, a.min, a.max, a.angle, a.aspect, a.hue, a.saturation, a.exposure, a.scale);
    } അല്ലെങ്കിൽ എങ്കിൽ (a.type == REGION_DATA){
        *a.d = load_data_region(a.n, a.paths, a.m, a.w, a.h, a.അക്കം_boxes, a.classes, a.jitter, a.hue, a.saturation, a.exposure);
    } അല്ലെങ്കിൽ എങ്കിൽ (a.type == DETECTION_DATA){
        *a.d = load_data_detection(a.n, a.paths, a.m, a.w, a.h, a.അക്കം_boxes, a.classes, a.jitter, a.hue, a.saturation, a.exposure);
    } അല്ലെങ്കിൽ എങ്കിൽ (a.type == SWAG_DATA){
        *a.d = load_data_swag(a.paths, a.n, a.classes, a.jitter);
    } അല്ലെങ്കിൽ എങ്കിൽ (a.type == COMPARE_DATA){
        *a.d = load_data_compare(a.n, a.paths, a.m, a.classes, a.w, a.h);
    } അല്ലെങ്കിൽ എങ്കിൽ (a.type == IMAGE_DATA){
        *(a.im) = load_image_color(a.path, 0, 0);
        *(a.resized) = resize_image(*(a.im), a.w, a.h);
    } അല്ലെങ്കിൽ എങ്കിൽ (a.type == LETTERBOX_DATA){
        *(a.im) = load_image_color(a.path, 0, 0);
        *(a.resized) = letterbox_image(*(a.im), a.w, a.h);
    } അല്ലെങ്കിൽ എങ്കിൽ (a.type == TAG_DATA){
        *a.d = load_data_tag(a.paths, a.n, a.m, a.classes, a.min, a.max, a.size, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    }
    സ്വതന്ത്രം(ptr);
    തിരികെ 0;
}

pthread_t load_data_in_thread(load_args args)
{
    pthread_t thread;
    ഘടന load_args *ptr = calloc(1, വലുപ്പം(ഘടന load_args));
    *ptr = args;
    എങ്കിൽ(pthread_create(&thread, 0, load_thread, ptr)) error("Thread creation failed");
    തിരികെ thread;
}

ശൂന്യം *load_threads(ശൂന്യം *ptr)
{
    പൂർണ്ണസംഖ്യ i;
    load_args args = *(load_args *)ptr;
    എങ്കിൽ (args.threads == 0) args.threads = 1;
    data *out = args.d;
    പൂർണ്ണസംഖ്യ total = args.n;
    സ്വതന്ത്രം(ptr);
    data *buffers = calloc(args.threads, വലുപ്പം(data));
    pthread_t *threads = calloc(args.threads, വലുപ്പം(pthread_t));
    വേണ്ടി(i = 0; i < args.threads; ++i){
        args.d = buffers + i;
        args.n = (i+1) * total/args.threads - i * total/args.threads;
        threads[i] = load_data_in_thread(args);
    }
    വേണ്ടി(i = 0; i < args.threads; ++i){
        pthread_join(threads[i], 0);
    }
    *out = concat_datas(buffers, args.threads);
    out->shallow = 0;
    വേണ്ടി(i = 0; i < args.threads; ++i){
        buffers[i].shallow = 1;
        free_data(buffers[i]);
    }
    സ്വതന്ത്രം(buffers);
    സ്വതന്ത്രം(threads);
    തിരികെ 0;
}

ശൂന്യം load_data_blocking(load_args args)
{
    ഘടന load_args *ptr = calloc(1, വലുപ്പം(ഘടന load_args));
    *ptr = args;
    load_thread(ptr);
}

pthread_t load_data(load_args args)
{
    pthread_t thread;
    ഘടന load_args *ptr = calloc(1, വലുപ്പം(ഘടന load_args));
    *ptr = args;
    എങ്കിൽ(pthread_create(&thread, 0, load_threads, ptr)) error("Thread creation failed");
    തിരികെ thread;
}

data load_data_writing(അക്ഷരം **paths, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ m, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h, പൂർണ്ണസംഖ്യ out_w, പൂർണ്ണസംഖ്യ out_h)
{
    എങ്കിൽ(m) paths = get_random_paths(paths, n, m);
    അക്ഷരം **replace_paths = find_replace_paths(paths, n, ".png", "-label.png");
    data d = {0};
    d.shallow = 0;
    d.X = load_image_paths(paths, n, w, h);
    d.y = load_image_paths_gray(replace_paths, n, out_w, out_h);
    എങ്കിൽ(m) സ്വതന്ത്രം(paths);
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < n; ++i) സ്വതന്ത്രം(replace_paths[i]);
    സ്വതന്ത്രം(replace_paths);
    തിരികെ d;
}

data load_data_old(അക്ഷരം **paths, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ m, അക്ഷരം **labels, പൂർണ്ണസംഖ്യ k, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h)
{
    എങ്കിൽ(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;
    d.X = load_image_paths(paths, n, w, h);
    d.y = load_labels_paths(paths, n, labels, k, 0);
    എങ്കിൽ(m) സ്വതന്ത്രം(paths);
    തിരികെ d;
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

data load_data_super(അക്ഷരം **paths, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ m, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h, പൂർണ്ണസംഖ്യ scale)
{
    എങ്കിൽ(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;

    പൂർണ്ണസംഖ്യ i;
    d.X.rows = n;
    d.X.vals = calloc(n, വലുപ്പം(ദശാംശം*));
    d.X.cols = w*h*3;

    d.y.rows = n;
    d.y.vals = calloc(n, വലുപ്പം(ദശാംശം*));
    d.y.cols = w*scale * h*scale * 3;

    വേണ്ടി(i = 0; i < n; ++i){
        image im = load_image_color(paths[i], 0, 0);
        image crop = random_crop_image(im, w*scale, h*scale);
        പൂർണ്ണസംഖ്യ flip = rand()%2;
        എങ്കിൽ (flip) flip_image(crop);
        image resize = resize_image(crop, w, h);
        d.X.vals[i] = resize.data;
        d.y.vals[i] = crop.data;
        free_image(im);
    }

    എങ്കിൽ(m) സ്വതന്ത്രം(paths);
    തിരികെ d;
}

data load_data_regression(അക്ഷരം **paths, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ m, പൂർണ്ണസംഖ്യ k, പൂർണ്ണസംഖ്യ min, പൂർണ്ണസംഖ്യ max, പൂർണ്ണസംഖ്യ size, ദശാംശം angle, ദശാംശം aspect, ദശാംശം hue, ദശാംശം saturation, ദശാംശം exposure)
{
    എങ്കിൽ(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;
    d.X = load_image_augment_paths(paths, n, min, max, size, angle, aspect, hue, saturation, exposure, 0);
    d.y = load_regression_labels_paths(paths, n, k);
    എങ്കിൽ(m) സ്വതന്ത്രം(paths);
    തിരികെ d;
}

data select_data(data *orig, പൂർണ്ണസംഖ്യ *inds)
{
    data d = {0};
    d.shallow = 1;
    d.w = orig[0].w;
    d.h = orig[0].h;

    d.X.rows = orig[0].X.rows;
    d.y.rows = orig[0].X.rows;

    d.X.cols = orig[0].X.cols;
    d.y.cols = orig[0].y.cols;

    d.X.vals = calloc(orig[0].X.rows, വലുപ്പം(ദശാംശം *));
    d.y.vals = calloc(orig[0].y.rows, വലുപ്പം(ദശാംശം *));
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < d.X.rows; ++i){
        d.X.vals[i] = orig[inds[i]].X.vals[i];
        d.y.vals[i] = orig[inds[i]].y.vals[i];
    }
    തിരികെ d;
}

data *tile_data(data orig, പൂർണ്ണസംഖ്യ divs, പൂർണ്ണസംഖ്യ size)
{
    data *ds = calloc(divs*divs, വലുപ്പം(data));
    പൂർണ്ണസംഖ്യ i, j;
#pragma omp parallel വേണ്ടി
    വേണ്ടി(i = 0; i < divs*divs; ++i){
        data d;
        d.shallow = 0;
        d.w = orig.w/divs * size;
        d.h = orig.h/divs * size;
        d.X.rows = orig.X.rows;
        d.X.cols = d.w*d.h*3;
        d.X.vals = calloc(d.X.rows, വലുപ്പം(ദശാംശം*));

        d.y = copy_matrix(orig.y);
#pragma omp parallel വേണ്ടി
        വേണ്ടി(j = 0; j < orig.X.rows; ++j){
            പൂർണ്ണസംഖ്യ x = (i%divs) * orig.w / divs - (d.w - orig.w/divs)/2;
            പൂർണ്ണസംഖ്യ y = (i/divs) * orig.h / divs - (d.h - orig.h/divs)/2;
            image im = float_to_image(orig.w, orig.h, 3, orig.X.vals[j]);
            d.X.vals[j] = crop_image(im, x, y, d.w, d.h).data;
        }
        ds[i] = d;
    }
    തിരികെ ds;
}

data resize_data(data orig, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h)
{
    data d = {0};
    d.shallow = 0;
    d.w = w;
    d.h = h;
    പൂർണ്ണസംഖ്യ i;
    d.X.rows = orig.X.rows;
    d.X.cols = w*h*3;
    d.X.vals = calloc(d.X.rows, വലുപ്പം(ദശാംശം*));

    d.y = copy_matrix(orig.y);
#pragma omp parallel വേണ്ടി
    വേണ്ടി(i = 0; i < orig.X.rows; ++i){
        image im = float_to_image(orig.w, orig.h, 3, orig.X.vals[i]);
        d.X.vals[i] = resize_image(im, w, h).data;
    }
    തിരികെ d;
}

data load_data_augment(അക്ഷരം **paths, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ m, അക്ഷരം **labels, പൂർണ്ണസംഖ്യ k, വൃക്ഷം *hierarchy, പൂർണ്ണസംഖ്യ min, പൂർണ്ണസംഖ്യ max, പൂർണ്ണസംഖ്യ size, ദശാംശം angle, ദശാംശം aspect, ദശാംശം hue, ദശാംശം saturation, ദശാംശം exposure, പൂർണ്ണസംഖ്യ center)
{
    എങ്കിൽ(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;
    d.w=size;
    d.h=size;
    d.X = load_image_augment_paths(paths, n, min, max, size, angle, aspect, hue, saturation, exposure, center);
    d.y = load_labels_paths(paths, n, labels, k, hierarchy);
    എങ്കിൽ(m) സ്വതന്ത്രം(paths);
    തിരികെ d;
}

data load_data_tag(അക്ഷരം **paths, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ m, പൂർണ്ണസംഖ്യ k, പൂർണ്ണസംഖ്യ min, പൂർണ്ണസംഖ്യ max, പൂർണ്ണസംഖ്യ size, ദശാംശം angle, ദശാംശം aspect, ദശാംശം hue, ദശാംശം saturation, ദശാംശം exposure)
{
    എങ്കിൽ(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.w = size;
    d.h = size;
    d.shallow = 0;
    d.X = load_image_augment_paths(paths, n, min, max, size, angle, aspect, hue, saturation, exposure, 0);
    d.y = load_tags_paths(paths, n, k);
    എങ്കിൽ(m) സ്വതന്ത്രം(paths);
    തിരികെ d;
}

matrix concat_matrix(matrix m1, matrix m2)
{
    പൂർണ്ണസംഖ്യ i, എണ്ണം = 0;
    matrix m;
    m.cols = m1.cols;
    m.rows = m1.rows+m2.rows;
    m.vals = calloc(m1.rows + m2.rows, വലുപ്പം(ദശാംശം*));
    വേണ്ടി(i = 0; i < m1.rows; ++i){
        m.vals[എണ്ണം++] = m1.vals[i];
    }
    വേണ്ടി(i = 0; i < m2.rows; ++i){
        m.vals[എണ്ണം++] = m2.vals[i];
    }
    തിരികെ m;
}

data concat_data(data d1, data d2)
{
    data d = {0};
    d.shallow = 1;
    d.X = concat_matrix(d1.X, d2.X);
    d.y = concat_matrix(d1.y, d2.y);
    d.w = d1.w;
    d.h = d1.h;
    തിരികെ d;
}

data concat_datas(data *d, പൂർണ്ണസംഖ്യ n)
{
    പൂർണ്ണസംഖ്യ i;
    data out = {0};
    വേണ്ടി(i = 0; i < n; ++i){
        data new = concat_data(d[i], out);
        free_data(out);
        out = new;
    }
    തിരികെ out;
}

data load_categorical_data_csv(അക്ഷരം *filename, പൂർണ്ണസംഖ്യ target, പൂർണ്ണസംഖ്യ k)
{
    data d = {0};
    d.shallow = 0;
    matrix X = csv_to_matrix(filename);
    ദശാംശം *truth_1d = എടുക്കുക_column(&X, target);
    ദശാംശം **truth = one_hot_encode(truth_1d, X.rows, k);
    matrix y;
    y.rows = X.rows;
    y.cols = k;
    y.vals = truth;
    d.X = X;
    d.y = y;
    സ്വതന്ത്രം(truth_1d);
    തിരികെ d;
}

data load_cifar10_data(അക്ഷരം *filename)
{
    data d = {0};
    d.shallow = 0;
    long i,j;
    matrix X = make_matrix(10000, 3072);
    matrix y = make_matrix(10000, 10);
    d.X = X;
    d.y = y;

    FILE *fp = fopen(filename, "rb");
    എങ്കിൽ(!fp) file_error(filename);
    വേണ്ടി(i = 0; i < 10000; ++i){
        unsigned അക്ഷരം bytes[3073];
        fread(bytes, 1, 3073, fp);
        പൂർണ്ണസംഖ്യ class = bytes[0];
        y.vals[i][class] = 1;
        വേണ്ടി(j = 0; j < X.cols; ++j){
            X.vals[i][j] = (ഇരട്ട)bytes[j+1];
        }
    }
    scale_data_rows(d, 1./255);
    //normalize_data_rows(d);
    fclose(fp);
    തിരികെ d;
}

ശൂന്യം get_random_batch(data d, പൂർണ്ണസംഖ്യ n, ദശാംശം *X, ദശാംശം *y)
{
    പൂർണ്ണസംഖ്യ j;
    വേണ്ടി(j = 0; j < n; ++j){
        പൂർണ്ണസംഖ്യ index = rand()%d.X.rows;
        memcpy(X+j*d.X.cols, d.X.vals[index], d.X.cols*വലുപ്പം(ദശാംശം));
        memcpy(y+j*d.y.cols, d.y.vals[index], d.y.cols*വലുപ്പം(ദശാംശം));
    }
}

ശൂന്യം get_next_batch(data d, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ offset, ദശാംശം *X, ദശാംശം *y)
{
    പൂർണ്ണസംഖ്യ j;
    വേണ്ടി(j = 0; j < n; ++j){
        പൂർണ്ണസംഖ്യ index = offset + j;
        memcpy(X+j*d.X.cols, d.X.vals[index], d.X.cols*വലുപ്പം(ദശാംശം));
        എങ്കിൽ(y) memcpy(y+j*d.y.cols, d.y.vals[index], d.y.cols*വലുപ്പം(ദശാംശം));
    }
}

ശൂന്യം smooth_data(data d)
{
    പൂർണ്ണസംഖ്യ i, j;
    ദശാംശം scale = 1. / d.y.cols;
    ദശാംശം eps = .1;
    വേണ്ടി(i = 0; i < d.y.rows; ++i){
        വേണ്ടി(j = 0; j < d.y.cols; ++j){
            d.y.vals[i][j] = eps * scale + (1-eps) * d.y.vals[i][j];
        }
    }
}

data load_all_cifar10()
{
    data d = {0};
    d.shallow = 0;
    പൂർണ്ണസംഖ്യ i,j,b;
    matrix X = make_matrix(50000, 3072);
    matrix y = make_matrix(50000, 10);
    d.X = X;
    d.y = y;


    വേണ്ടി(b = 0; b < 5; ++b){
        അക്ഷരം buff[256];
        sprintf(buff, "data/cifar/cifar-10-batches-bin/data_batch_%d.bin", b+1);
        FILE *fp = fopen(buff, "rb");
        എങ്കിൽ(!fp) file_error(buff);
        വേണ്ടി(i = 0; i < 10000; ++i){
            unsigned അക്ഷരം bytes[3073];
            fread(bytes, 1, 3073, fp);
            പൂർണ്ണസംഖ്യ class = bytes[0];
            y.vals[i+b*10000][class] = 1;
            വേണ്ടി(j = 0; j < X.cols; ++j){
                X.vals[i+b*10000][j] = (ഇരട്ട)bytes[j+1];
            }
        }
        fclose(fp);
    }
    //normalize_data_rows(d);
    scale_data_rows(d, 1./255);
    smooth_data(d);
    തിരികെ d;
}

data load_go(അക്ഷരം *filename)
{
    FILE *fp = fopen(filename, "rb");
    matrix X = make_matrix(3363059, 361);
    matrix y = make_matrix(3363059, 361);
    പൂർണ്ണസംഖ്യ row, col;

    എങ്കിൽ(!fp) file_error(filename);
    അക്ഷരം *label;
    പൂർണ്ണസംഖ്യ എണ്ണം = 0;
    ആകുമ്പോൾ((label = fgetl(fp))){
        പൂർണ്ണസംഖ്യ i;
        എങ്കിൽ(എണ്ണം == X.rows){
            X = resize_matrix(X, എണ്ണം*2);
            y = resize_matrix(y, എണ്ണം*2);
        }
        sscanf(label, "%d %d", &row, &col);
        അക്ഷരം *board = fgetl(fp);

        പൂർണ്ണസംഖ്യ index = row*19 + col;
        y.vals[എണ്ണം][index] = 1;

        വേണ്ടി(i = 0; i < 19*19; ++i){
            ദശാംശം val = 0;
            എങ്കിൽ(board[i] == '1') val = 1;
            അല്ലെങ്കിൽ എങ്കിൽ(board[i] == '2') val = -1;
            X.vals[എണ്ണം][i] = val;
        }
        ++എണ്ണം;
        സ്വതന്ത്രം(label);
        സ്വതന്ത്രം(board);
    }
    X = resize_matrix(X, എണ്ണം);
    y = resize_matrix(y, എണ്ണം);

    data d = {0};
    d.shallow = 0;
    d.X = X;
    d.y = y;


    fclose(fp);

    തിരികെ d;
}


ശൂന്യം randomize_data(data d)
{
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = d.X.rows-1; i > 0; --i){
        പൂർണ്ണസംഖ്യ index = rand()%i;
        ദശാംശം *swap = d.X.vals[index];
        d.X.vals[index] = d.X.vals[i];
        d.X.vals[i] = swap;

        swap = d.y.vals[index];
        d.y.vals[index] = d.y.vals[i];
        d.y.vals[i] = swap;
    }
}

ശൂന്യം scale_data_rows(data d, ദശാംശം s)
{
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < d.X.rows; ++i){
        scale_array(d.X.vals[i], d.X.cols, s);
    }
}

ശൂന്യം translate_data_rows(data d, ദശാംശം s)
{
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < d.X.rows; ++i){
        translate_array(d.X.vals[i], d.X.cols, s);
    }
}

data copy_data(data d)
{
    data c = {0};
    c.w = d.w;
    c.h = d.h;
    c.shallow = 0;
    c.അക്കം_boxes = d.അക്കം_boxes;
    c.boxes = d.boxes;
    c.X = copy_matrix(d.X);
    c.y = copy_matrix(d.y);
    തിരികെ c;
}

ശൂന്യം normalize_data_rows(data d)
{
    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < d.X.rows; ++i){
        normalize_array(d.X.vals[i], d.X.cols);
    }
}

data get_data_part(data d, പൂർണ്ണസംഖ്യ part, പൂർണ്ണസംഖ്യ total)
{
    data p = {0};
    p.shallow = 1;
    p.X.rows = d.X.rows * (part + 1) / total - d.X.rows * part / total;
    p.y.rows = d.y.rows * (part + 1) / total - d.y.rows * part / total;
    p.X.cols = d.X.cols;
    p.y.cols = d.y.cols;
    p.X.vals = d.X.vals + d.X.rows * part / total;
    p.y.vals = d.y.vals + d.y.rows * part / total;
    തിരികെ p;
}

data get_random_data(data d, പൂർണ്ണസംഖ്യ അക്കം)
{
    data r = {0};
    r.shallow = 1;

    r.X.rows = അക്കം;
    r.y.rows = അക്കം;

    r.X.cols = d.X.cols;
    r.y.cols = d.y.cols;

    r.X.vals = calloc(അക്കം, വലുപ്പം(ദശാംശം *));
    r.y.vals = calloc(അക്കം, വലുപ്പം(ദശാംശം *));

    പൂർണ്ണസംഖ്യ i;
    വേണ്ടി(i = 0; i < അക്കം; ++i){
        പൂർണ്ണസംഖ്യ index = rand()%d.X.rows;
        r.X.vals[i] = d.X.vals[index];
        r.y.vals[i] = d.y.vals[index];
    }
    തിരികെ r;
}

data *split_data(data d, പൂർണ്ണസംഖ്യ part, പൂർണ്ണസംഖ്യ total)
{
    data *split = calloc(2, വലുപ്പം(data));
    പൂർണ്ണസംഖ്യ i;
    പൂർണ്ണസംഖ്യ start = part*d.X.rows/total;
    പൂർണ്ണസംഖ്യ end = (part+1)*d.X.rows/total;
    data train;
    data test;
    train.shallow = test.shallow = 1;

    test.X.rows = test.y.rows = end-start;
    train.X.rows = train.y.rows = d.X.rows - (end-start);
    train.X.cols = test.X.cols = d.X.cols;
    train.y.cols = test.y.cols = d.y.cols;

    train.X.vals = calloc(train.X.rows, വലുപ്പം(ദശാംശം*));
    test.X.vals = calloc(test.X.rows, വലുപ്പം(ദശാംശം*));
    train.y.vals = calloc(train.y.rows, വലുപ്പം(ദശാംശം*));
    test.y.vals = calloc(test.y.rows, വലുപ്പം(ദശാംശം*));

    വേണ്ടി(i = 0; i < start; ++i){
        train.X.vals[i] = d.X.vals[i];
        train.y.vals[i] = d.y.vals[i];
    }
    വേണ്ടി(i = start; i < end; ++i){
        test.X.vals[i-start] = d.X.vals[i];
        test.y.vals[i-start] = d.y.vals[i];
    }
    വേണ്ടി(i = end; i < d.X.rows; ++i){
        train.X.vals[i-(end-start)] = d.X.vals[i];
        train.y.vals[i-(end-start)] = d.y.vals[i];
    }
    split[0] = train;
    split[1] = test;
    തിരികെ split;
}

