#ਅਨ੍ਤਰ੍ਭਾਵ "data.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "image.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cuda.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

list *get_paths(ਅੱਖਰ *filename)
{
    ਅੱਖਰ *ਰਸਤਾ;
    FILE *file = fopen(filename, "r");
    ਜੇ(!file) file_error(filename);
    list *ਸਤਰਾਂ = make_list();
    ਜਦੋਂ((ਰਸਤਾ=fgetl(file))){
        list_ਪਾਓ(ਸਤਰਾਂ, ਰਸਤਾ);
    }
    fclose(file);
    ਵਾਪਸ ਸਤਰਾਂ;
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

ਅੱਖਰ **get_random_paths(ਅੱਖਰ **paths, ਪੂਰਨ n, ਪੂਰਨ m)
{
    ਅੱਖਰ **random_paths = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(n, ਆਕਾਰ(ਅੱਖਰ*));
    ਪੂਰਨ i;
    pthread_mutex_lock(&mutex);
    ਲਈ(i = 0; i < n; ++i){
        ਪੂਰਨ ਸੂਚਕਾਂਕ = rand()%m;
        random_paths[i] = paths[ਸੂਚਕਾਂਕ];
        //if(i == 0) printf("%s\n", paths[index]);
    }
    pthread_mutex_unlock(&mutex);
    ਵਾਪਸ random_paths;
}

ਅੱਖਰ **ਲੱਭੋ_replace_paths(ਅੱਖਰ **paths, ਪੂਰਨ n, ਅੱਖਰ *ਲੱਭੋ, ਅੱਖਰ *replace)
{
    ਅੱਖਰ **replace_paths = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(n, ਆਕਾਰ(ਅੱਖਰ*));
    ਪੂਰਨ i;
    ਲਈ(i = 0; i < n; ++i){
        ਅੱਖਰ replaced[4096];
        ਲੱਭੋ_replace(paths[i], ਲੱਭੋ, replace, replaced);
        replace_paths[i] = copy_string(replaced);
    }
    ਵਾਪਸ replace_paths;
}

ਮੈਟ੍ਰਿਕਸ ਲੱਦੋ_image_paths_ਸਲੇਟੀ(ਅੱਖਰ **paths, ਪੂਰਨ n, ਪੂਰਨ w, ਪੂਰਨ h)
{
    ਪੂਰਨ i;
    ਮੈਟ੍ਰਿਕਸ X;
    X.ਪੰਕਤੀਆਂ = n;
    X.vals = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(X.ਪੰਕਤੀਆਂ, ਆਕਾਰ(ਦਸ਼ਮਲਵ*));
    X.ਖਾਨੇ_ਗਿਣਤੀ = 0;

    ਲਈ(i = 0; i < n; ++i){
        image im = ਲੱਦੋ_image(paths[i], w, h, 3);

        image ਸਲੇਟੀ = grayscale_image(im);
        free_image(im);
        im = ਸਲੇਟੀ;

        X.vals[i] = im.ਅੰਕੜੇ;
        X.ਖਾਨੇ_ਗਿਣਤੀ = im.h*im.w*im.c;
    }
    ਵਾਪਸ X;
}

ਮੈਟ੍ਰਿਕਸ ਲੱਦੋ_image_paths(ਅੱਖਰ **paths, ਪੂਰਨ n, ਪੂਰਨ w, ਪੂਰਨ h)
{
    ਪੂਰਨ i;
    ਮੈਟ੍ਰਿਕਸ X;
    X.ਪੰਕਤੀਆਂ = n;
    X.vals = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(X.ਪੰਕਤੀਆਂ, ਆਕਾਰ(ਦਸ਼ਮਲਵ*));
    X.ਖਾਨੇ_ਗਿਣਤੀ = 0;

    ਲਈ(i = 0; i < n; ++i){
        image im = ਲੱਦੋ_image_color(paths[i], w, h);
        X.vals[i] = im.ਅੰਕੜੇ;
        X.ਖਾਨੇ_ਗਿਣਤੀ = im.h*im.w*im.c;
    }
    ਵਾਪਸ X;
}

ਮੈਟ੍ਰਿਕਸ ਲੱਦੋ_image_augment_paths(ਅੱਖਰ **paths, ਪੂਰਨ n, ਪੂਰਨ min, ਪੂਰਨ max, ਪੂਰਨ ਮਾਪ, ਦਸ਼ਮਲਵ angle, ਦਸ਼ਮਲਵ aspect, ਦਸ਼ਮਲਵ hue, ਦਸ਼ਮਲਵ saturation, ਦਸ਼ਮਲਵ exposure, ਪੂਰਨ center)
{
    ਪੂਰਨ i;
    ਮੈਟ੍ਰਿਕਸ X;
    X.ਪੰਕਤੀਆਂ = n;
    X.vals = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(X.ਪੰਕਤੀਆਂ, ਆਕਾਰ(ਦਸ਼ਮਲਵ*));
    X.ਖਾਨੇ_ਗਿਣਤੀ = 0;

    ਲਈ(i = 0; i < n; ++i){
        image im = ਲੱਦੋ_image_color(paths[i], 0, 0);
        image crop;
        ਜੇ(center){
            crop = center_crop_image(im, ਮਾਪ, ਮਾਪ);
        } ਨਹੀਂਤਾਂ {
            crop = random_augment_image(im, angle, aspect, min, max, ਮਾਪ, ਮਾਪ);
        }
        ਪੂਰਨ flip = rand()%2;
        ਜੇ (flip) flip_image(crop);
        random_distort_image(crop, hue, saturation, exposure);

        /*
        show_image(im, "orig");
        show_image(crop, "crop");
        cvWaitKey(0);
        */
        //grayscale_image_3c(crop);
        free_image(im);
        X.vals[i] = crop.ਅੰਕੜੇ;
        X.ਖਾਨੇ_ਗਿਣਤੀ = crop.h*crop.w*crop.c;
    }
    ਵਾਪਸ X;
}


box_label *read_boxes(ਅੱਖਰ *filename, ਪੂਰਨ *n)
{
    FILE *file = fopen(filename, "r");
    ਜੇ(!file) file_error(filename);
    ਦਸ਼ਮਲਵ x, y, h, w;
    ਪੂਰਨ id;
    ਪੂਰਨ ਗਿਣਤੀ_2 = 0;
    ਪੂਰਨ ਮਾਪ = 64;
    box_label *boxes = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਮਾਪ, ਆਕਾਰ(box_label));
    ਜਦੋਂ(fscanf(file, "%d %f %f %f %f", &id, &x, &y, &w, &h) == 5){
        ਜੇ(ਗਿਣਤੀ_2 == ਮਾਪ) {
            ਮਾਪ = ਮਾਪ * 2;
            boxes = realloc(boxes, ਮਾਪ*ਆਕਾਰ(box_label));
        }
        boxes[ਗਿਣਤੀ_2].id = id;
        boxes[ਗਿਣਤੀ_2].x = x;
        boxes[ਗਿਣਤੀ_2].y = y;
        boxes[ਗਿਣਤੀ_2].h = h;
        boxes[ਗਿਣਤੀ_2].w = w;
        boxes[ਗਿਣਤੀ_2].ਖੱਬਾ   = x - w/2;
        boxes[ਗਿਣਤੀ_2].ਸੱਜਾ  = x + w/2;
        boxes[ਗਿਣਤੀ_2].ਸਿਖਰ    = y - h/2;
        boxes[ਗਿਣਤੀ_2].bottom = y + h/2;
        ++ਗਿਣਤੀ_2;
    }
    fclose(file);
    *n = ਗਿਣਤੀ_2;
    ਵਾਪਸ boxes;
}

ਖਾਲੀ randomize_boxes(box_label *b, ਪੂਰਨ n)
{
    ਪੂਰਨ i;
    ਲਈ(i = 0; i < n; ++i){
        box_label ਅਦਲਾ_ਬਦਲੀ = b[i];
        ਪੂਰਨ ਸੂਚਕਾਂਕ = rand()%n;
        b[i] = b[ਸੂਚਕਾਂਕ];
        b[ਸੂਚਕਾਂਕ] = ਅਦਲਾ_ਬਦਲੀ;
    }
}

ਖਾਲੀ correct_boxes(box_label *boxes, ਪੂਰਨ n, ਦਸ਼ਮਲਵ dx, ਦਸ਼ਮਲਵ dy, ਦਸ਼ਮਲਵ sx, ਦਸ਼ਮਲਵ sy, ਪੂਰਨ flip)
{
    ਪੂਰਨ i;
    ਲਈ(i = 0; i < n; ++i){
        ਜੇ(boxes[i].x == 0 && boxes[i].y == 0) {
            boxes[i].x = 999999;
            boxes[i].y = 999999;
            boxes[i].w = 999999;
            boxes[i].h = 999999;
            ਜਾਰੀ;
        }
        boxes[i].ਖੱਬਾ   = boxes[i].ਖੱਬਾ  * sx - dx;
        boxes[i].ਸੱਜਾ  = boxes[i].ਸੱਜਾ * sx - dx;
        boxes[i].ਸਿਖਰ    = boxes[i].ਸਿਖਰ   * sy - dy;
        boxes[i].bottom = boxes[i].bottom* sy - dy;

        ਜੇ(flip){
            ਦਸ਼ਮਲਵ ਅਦਲਾ_ਬਦਲੀ = boxes[i].ਖੱਬਾ;
            boxes[i].ਖੱਬਾ = 1. - boxes[i].ਸੱਜਾ;
            boxes[i].ਸੱਜਾ = 1. - ਅਦਲਾ_ਬਦਲੀ;
        }

        boxes[i].ਖੱਬਾ =  constrain(0, 1, boxes[i].ਖੱਬਾ);
        boxes[i].ਸੱਜਾ = constrain(0, 1, boxes[i].ਸੱਜਾ);
        boxes[i].ਸਿਖਰ =   constrain(0, 1, boxes[i].ਸਿਖਰ);
        boxes[i].bottom =   constrain(0, 1, boxes[i].bottom);

        boxes[i].x = (boxes[i].ਖੱਬਾ+boxes[i].ਸੱਜਾ)/2;
        boxes[i].y = (boxes[i].ਸਿਖਰ+boxes[i].bottom)/2;
        boxes[i].w = (boxes[i].ਸੱਜਾ - boxes[i].ਖੱਬਾ);
        boxes[i].h = (boxes[i].bottom - boxes[i].ਸਿਖਰ);

        boxes[i].w = constrain(0, 1, boxes[i].w);
        boxes[i].h = constrain(0, 1, boxes[i].h);
    }
}

ਖਾਲੀ fill_truth_swag(ਅੱਖਰ *ਰਸਤਾ, ਦਸ਼ਮਲਵ *truth, ਪੂਰਨ classes, ਪੂਰਨ flip, ਦਸ਼ਮਲਵ dx, ਦਸ਼ਮਲਵ dy, ਦਸ਼ਮਲਵ sx, ਦਸ਼ਮਲਵ sy)
{
    ਅੱਖਰ labelpath[4096];
    ਲੱਭੋ_replace(ਰਸਤਾ, "images", "labels", labelpath);
    ਲੱਭੋ_replace(labelpath, "JPEGImages", "labels", labelpath);
    ਲੱਭੋ_replace(labelpath, ".jpg", ".txt", labelpath);
    ਲੱਭੋ_replace(labelpath, ".JPG", ".txt", labelpath);
    ਲੱਭੋ_replace(labelpath, ".JPEG", ".txt", labelpath);

    ਪੂਰਨ ਗਿਣਤੀ_2 = 0;
    box_label *boxes = read_boxes(labelpath, &ਗਿਣਤੀ_2);
    randomize_boxes(boxes, ਗਿਣਤੀ_2);
    correct_boxes(boxes, ਗਿਣਤੀ_2, dx, dy, sx, sy, flip);
    ਦਸ਼ਮਲਵ x,y,w,h;
    ਪੂਰਨ id;
    ਪੂਰਨ i;

    ਲਈ (i = 0; i < ਗਿਣਤੀ_2 && i < 90; ++i) {
        x =  boxes[i].x;
        y =  boxes[i].y;
        w =  boxes[i].w;
        h =  boxes[i].h;
        id = boxes[i].id;

        ਜੇ (w < .0 || h < .0) ਜਾਰੀ;

        ਪੂਰਨ ਸੂਚਕਾਂਕ = (4+classes) * i;

        truth[ਸੂਚਕਾਂਕ++] = x;
        truth[ਸੂਚਕਾਂਕ++] = y;
        truth[ਸੂਚਕਾਂਕ++] = w;
        truth[ਸੂਚਕਾਂਕ++] = h;

        ਜੇ (id < classes) truth[ਸੂਚਕਾਂਕ+id] = 1;
    }
    ਮੁਕਤ(boxes);
}

ਖਾਲੀ fill_truth_region(ਅੱਖਰ *ਰਸਤਾ, ਦਸ਼ਮਲਵ *truth, ਪੂਰਨ classes, ਪੂਰਨ ਅੰਕ_boxes, ਪੂਰਨ flip, ਦਸ਼ਮਲਵ dx, ਦਸ਼ਮਲਵ dy, ਦਸ਼ਮਲਵ sx, ਦਸ਼ਮਲਵ sy)
{
    ਅੱਖਰ labelpath[4096];
    ਲੱਭੋ_replace(ਰਸਤਾ, "images", "labels", labelpath);
    ਲੱਭੋ_replace(labelpath, "JPEGImages", "labels", labelpath);

    ਲੱਭੋ_replace(labelpath, ".jpg", ".txt", labelpath);
    ਲੱਭੋ_replace(labelpath, ".png", ".txt", labelpath);
    ਲੱਭੋ_replace(labelpath, ".JPG", ".txt", labelpath);
    ਲੱਭੋ_replace(labelpath, ".JPEG", ".txt", labelpath);
    ਪੂਰਨ ਗਿਣਤੀ_2 = 0;
    box_label *boxes = read_boxes(labelpath, &ਗਿਣਤੀ_2);
    randomize_boxes(boxes, ਗਿਣਤੀ_2);
    correct_boxes(boxes, ਗਿਣਤੀ_2, dx, dy, sx, sy, flip);
    ਦਸ਼ਮਲਵ x,y,w,h;
    ਪੂਰਨ id;
    ਪੂਰਨ i;

    ਲਈ (i = 0; i < ਗਿਣਤੀ_2; ++i) {
        x =  boxes[i].x;
        y =  boxes[i].y;
        w =  boxes[i].w;
        h =  boxes[i].h;
        id = boxes[i].id;

        ਜੇ (w < .005 || h < .005) ਜਾਰੀ;

        ਪੂਰਨ ਖਾਨਾ = (ਪੂਰਨ)(x*ਅੰਕ_boxes);
        ਪੂਰਨ ਪੰਕਤੀ = (ਪੂਰਨ)(y*ਅੰਕ_boxes);

        x = x*ਅੰਕ_boxes - ਖਾਨਾ;
        y = y*ਅੰਕ_boxes - ਪੰਕਤੀ;

        ਪੂਰਨ ਸੂਚਕਾਂਕ = (ਖਾਨਾ+ਪੰਕਤੀ*ਅੰਕ_boxes)*(5+classes);
        ਜੇ (truth[ਸੂਚਕਾਂਕ]) ਜਾਰੀ;
        truth[ਸੂਚਕਾਂਕ++] = 1;

        ਜੇ (id < classes) truth[ਸੂਚਕਾਂਕ+id] = 1;
        ਸੂਚਕਾਂਕ += classes;

        truth[ਸੂਚਕਾਂਕ++] = x;
        truth[ਸੂਚਕਾਂਕ++] = y;
        truth[ਸੂਚਕਾਂਕ++] = w;
        truth[ਸੂਚਕਾਂਕ++] = h;
    }
    ਮੁਕਤ(boxes);
}

ਖਾਲੀ ਲੱਦੋ_rle(image im, ਪੂਰਨ *rle, ਪੂਰਨ n)
{
    ਪੂਰਨ ਗਿਣਤੀ_2 = 0;
    ਪੂਰਨ curr = 0;
    ਪੂਰਨ i,j;
    ਲਈ(i = 0; i < n; ++i){
        ਲਈ(j = 0; j < rle[i]; ++j){
            im.ਅੰਕੜੇ[ਗਿਣਤੀ_2++] = curr;
        }
        curr = 1 - curr;
    }
    ਲਈ(; ਗਿਣਤੀ_2 < im.h*im.w*im.c; ++ਗਿਣਤੀ_2){
        im.ਅੰਕੜੇ[ਗਿਣਤੀ_2] = curr;
    }
}

ਖਾਲੀ or_image(image src, image dest, ਪੂਰਨ c)
{
    ਪੂਰਨ i;
    ਲਈ(i = 0; i < src.w*src.h; ++i){
        ਜੇ(src.ਅੰਕੜੇ[i]) dest.ਅੰਕੜੇ[dest.w*dest.h*c + i] = 1;
    }
}

ਖਾਲੀ exclusive_image(image src)
{
    ਪੂਰਨ k, j, i;
    ਪੂਰਨ s = src.w*src.h;
    ਲਈ(k = 0; k < src.c-1; ++k){
        ਲਈ(i = 0; i < s; ++i){
            ਜੇ (src.ਅੰਕੜੇ[k*s + i]){
                ਲਈ(j = k+1; j < src.c; ++j){
                    src.ਅੰਕੜੇ[j*s + i] = 0;
                }
            }
        }
    }
}

box bound_image(image im)
{
    ਪੂਰਨ x,y;
    ਪੂਰਨ minx = im.w;
    ਪੂਰਨ miny = im.h;
    ਪੂਰਨ maxx = 0;
    ਪੂਰਨ maxy = 0;
    ਲਈ(y = 0; y < im.h; ++y){
        ਲਈ(x = 0; x < im.w; ++x){
            ਜੇ(im.ਅੰਕੜੇ[y*im.w + x]){
                minx = (x < minx) ? x : minx;
                miny = (y < miny) ? y : miny;
                maxx = (x > maxx) ? x : maxx;
                maxy = (y > maxy) ? y : maxy;
            }
        }
    }
    box b = {minx, miny, maxx-minx + 1, maxy-miny + 1};
    //printf("%f %f %f %f\n", b.x, b.y, b.w, b.h);
    ਵਾਪਸ b;
}

ਖਾਲੀ fill_truth_iseg(ਅੱਖਰ *ਰਸਤਾ, ਪੂਰਨ ਅੰਕ_boxes, ਦਸ਼ਮਲਵ *truth, ਪੂਰਨ classes, ਪੂਰਨ w, ਪੂਰਨ h, augment_ਦਲੀਲਾਂ aug, ਪੂਰਨ flip, ਪੂਰਨ mw, ਪੂਰਨ mh)
{
    ਅੱਖਰ labelpath[4096];
    ਲੱਭੋ_replace(ਰਸਤਾ, "images", "mask", labelpath);
    ਲੱਭੋ_replace(labelpath, "JPEGImages", "mask", labelpath);
    ਲੱਭੋ_replace(labelpath, ".jpg", ".txt", labelpath);
    ਲੱਭੋ_replace(labelpath, ".JPG", ".txt", labelpath);
    ਲੱਭੋ_replace(labelpath, ".JPEG", ".txt", labelpath);
    FILE *file = fopen(labelpath, "r");
    ਜੇ(!file) file_error(labelpath);
    ਅੱਖਰ buff[32788];
    ਪੂਰਨ id;
    ਪੂਰਨ i = 0;
    ਪੂਰਨ j;
    image part = make_image(w, h, 1);
    ਜਦੋਂ((fscanf(file, "%d %s", &id, buff) == 2) && i < ਅੰਕ_boxes){
        ਪੂਰਨ n = 0;
        ਪੂਰਨ *rle = read_intlist(buff, &n, 0);
        ਲੱਦੋ_rle(part, rle, n);
        image sized = rotate_crop_image(part, aug.rad, aug.scale, aug.w, aug.h, aug.dx, aug.dy, aug.aspect);
        ਜੇ(flip) flip_image(sized);

        image ਨਕਾਬ = resize_image(sized, mw, mh);
        truth[i*(mw*mh+1)] = id;
        ਲਈ(j = 0; j < mw*mh; ++j){
            truth[i*(mw*mh + 1) + 1 + j] = ਨਕਾਬ.ਅੰਕੜੇ[j];
        }
        ++i;

        free_image(ਨਕਾਬ);
        free_image(sized);
        ਮੁਕਤ(rle);
    }
    ਜੇ(i < ਅੰਕ_boxes) truth[i*(mw*mh+1)] = -1;
    fclose(file);
    free_image(part);
}

ਖਾਲੀ fill_truth_ਨਕਾਬ(ਅੱਖਰ *ਰਸਤਾ, ਪੂਰਨ ਅੰਕ_boxes, ਦਸ਼ਮਲਵ *truth, ਪੂਰਨ classes, ਪੂਰਨ w, ਪੂਰਨ h, augment_ਦਲੀਲਾਂ aug, ਪੂਰਨ flip, ਪੂਰਨ mw, ਪੂਰਨ mh)
{
    ਅੱਖਰ labelpath[4096];
    ਲੱਭੋ_replace(ਰਸਤਾ, "images", "mask", labelpath);
    ਲੱਭੋ_replace(labelpath, "JPEGImages", "mask", labelpath);
    ਲੱਭੋ_replace(labelpath, ".jpg", ".txt", labelpath);
    ਲੱਭੋ_replace(labelpath, ".JPG", ".txt", labelpath);
    ਲੱਭੋ_replace(labelpath, ".JPEG", ".txt", labelpath);
    FILE *file = fopen(labelpath, "r");
    ਜੇ(!file) file_error(labelpath);
    ਅੱਖਰ buff[32788];
    ਪੂਰਨ id;
    ਪੂਰਨ i = 0;
    image part = make_image(w, h, 1);
    ਜਦੋਂ((fscanf(file, "%d %s", &id, buff) == 2) && i < ਅੰਕ_boxes){
        ਪੂਰਨ n = 0;
        ਪੂਰਨ *rle = read_intlist(buff, &n, 0);
        ਲੱਦੋ_rle(part, rle, n);
        image sized = rotate_crop_image(part, aug.rad, aug.scale, aug.w, aug.h, aug.dx, aug.dy, aug.aspect);
        ਜੇ(flip) flip_image(sized);
        box b = bound_image(sized);
        ਜੇ(b.w > 0){
            image crop = crop_image(sized, b.x, b.y, b.w, b.h);
            image ਨਕਾਬ = resize_image(crop, mw, mh);
            truth[i*(4 + mw*mh + 1) + 0] = (b.x + b.w/2.)/sized.w;
            truth[i*(4 + mw*mh + 1) + 1] = (b.y + b.h/2.)/sized.h;
            truth[i*(4 + mw*mh + 1) + 2] = b.w/sized.w;
            truth[i*(4 + mw*mh + 1) + 3] = b.h/sized.h;
            ਪੂਰਨ j;
            ਲਈ(j = 0; j < mw*mh; ++j){
                truth[i*(4 + mw*mh + 1) + 4 + j] = ਨਕਾਬ.ਅੰਕੜੇ[j];
            }
            truth[i*(4 + mw*mh + 1) + 4 + mw*mh] = id;
            free_image(crop);
            free_image(ਨਕਾਬ);
            ++i;
        }
        free_image(sized);
        ਮੁਕਤ(rle);
    }
    fclose(file);
    free_image(part);
}


ਖਾਲੀ fill_truth_detection(ਅੱਖਰ *ਰਸਤਾ, ਪੂਰਨ ਅੰਕ_boxes, ਦਸ਼ਮਲਵ *truth, ਪੂਰਨ classes, ਪੂਰਨ flip, ਦਸ਼ਮਲਵ dx, ਦਸ਼ਮਲਵ dy, ਦਸ਼ਮਲਵ sx, ਦਸ਼ਮਲਵ sy)
{
    ਅੱਖਰ labelpath[4096];
    ਲੱਭੋ_replace(ਰਸਤਾ, "images", "labels", labelpath);
    ਲੱਭੋ_replace(labelpath, "JPEGImages", "labels", labelpath);

    ਲੱਭੋ_replace(labelpath, "raw", "labels", labelpath);
    ਲੱਭੋ_replace(labelpath, ".jpg", ".txt", labelpath);
    ਲੱਭੋ_replace(labelpath, ".png", ".txt", labelpath);
    ਲੱਭੋ_replace(labelpath, ".JPG", ".txt", labelpath);
    ਲੱਭੋ_replace(labelpath, ".JPEG", ".txt", labelpath);
    ਪੂਰਨ ਗਿਣਤੀ_2 = 0;
    box_label *boxes = read_boxes(labelpath, &ਗਿਣਤੀ_2);
    randomize_boxes(boxes, ਗਿਣਤੀ_2);
    correct_boxes(boxes, ਗਿਣਤੀ_2, dx, dy, sx, sy, flip);
    ਜੇ(ਗਿਣਤੀ_2 > ਅੰਕ_boxes) ਗਿਣਤੀ_2 = ਅੰਕ_boxes;
    ਦਸ਼ਮਲਵ x,y,w,h;
    ਪੂਰਨ id;
    ਪੂਰਨ i;
    ਪੂਰਨ sub = 0;

    ਲਈ (i = 0; i < ਗਿਣਤੀ_2; ++i) {
        x =  boxes[i].x;
        y =  boxes[i].y;
        w =  boxes[i].w;
        h =  boxes[i].h;
        id = boxes[i].id;

        ਜੇ ((w < .001 || h < .001)) {
            ++sub;
            ਜਾਰੀ;
        }

        truth[(i-sub)*5+0] = x;
        truth[(i-sub)*5+1] = y;
        truth[(i-sub)*5+2] = w;
        truth[(i-sub)*5+3] = h;
        truth[(i-sub)*5+4] = id;
    }
    ਮੁਕਤ(boxes);
}

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NUMCHARS 37

ਖਾਲੀ print_letters(ਦਸ਼ਮਲਵ *pred, ਪੂਰਨ n)
{
    ਪੂਰਨ i;
    ਲਈ(i = 0; i < n; ++i){
        ਪੂਰਨ ਸੂਚਕਾਂਕ = max_ਸੂਚਕਾਂਕ(pred+i*NUMCHARS, NUMCHARS);
        ਲਿਖੋ("%c", int_ਮੰਜ਼ਿਲ_alphanum(ਸੂਚਕਾਂਕ));
    }
    ਲਿਖੋ("\n");
}

ਖਾਲੀ fill_truth_captcha(ਅੱਖਰ *ਰਸਤਾ, ਪੂਰਨ n, ਦਸ਼ਮਲਵ *truth)
{
    ਅੱਖਰ *begin = strrchr(ਰਸਤਾ, '/');
    ++begin;
    ਪੂਰਨ i;
    ਲਈ(i = 0; i < ਲੜੀ_ਲੰਬਾਈ(begin) && i < n && begin[i] != '.'; ++i){
        ਪੂਰਨ ਸੂਚਕਾਂਕ = alphanum_ਮੰਜ਼ਿਲ_int(begin[i]);
        ਜੇ(ਸੂਚਕਾਂਕ > 35) ਲਿਖੋ("Bad %c\n", begin[i]);
        truth[i*NUMCHARS+ਸੂਚਕਾਂਕ] = 1;
    }
    ਲਈ(;i < n; ++i){
        truth[i*NUMCHARS + NUMCHARS-1] = 1;
    }
}

ਅੰਕੜੇ ਲੱਦੋ_ਅੰਕੜੇ_captcha(ਅੱਖਰ **paths, ਪੂਰਨ n, ਪੂਰਨ m, ਪੂਰਨ k, ਪੂਰਨ w, ਪੂਰਨ h)
{
    ਜੇ(m) paths = get_random_paths(paths, n, m);
    ਅੰਕੜੇ d = {0};
    d.shallow = 0;
    d.X = ਲੱਦੋ_image_paths(paths, n, w, h);
    d.y = make_ਮੈਟ੍ਰਿਕਸ(n, k*NUMCHARS);
    ਪੂਰਨ i;
    ਲਈ(i = 0; i < n; ++i){
        fill_truth_captcha(paths[i], k, d.y.vals[i]);
    }
    ਜੇ(m) ਮੁਕਤ(paths);
    ਵਾਪਸ d;
}

ਅੰਕੜੇ ਲੱਦੋ_ਅੰਕੜੇ_captcha_encode(ਅੱਖਰ **paths, ਪੂਰਨ n, ਪੂਰਨ m, ਪੂਰਨ w, ਪੂਰਨ h)
{
    ਜੇ(m) paths = get_random_paths(paths, n, m);
    ਅੰਕੜੇ d = {0};
    d.shallow = 0;
    d.X = ਲੱਦੋ_image_paths(paths, n, w, h);
    d.X.ਖਾਨੇ_ਗਿਣਤੀ = 17100;
    d.y = d.X;
    ਜੇ(m) ਮੁਕਤ(paths);
    ਵਾਪਸ d;
}

ਖਾਲੀ fill_truth(ਅੱਖਰ *ਰਸਤਾ, ਅੱਖਰ **labels, ਪੂਰਨ k, ਦਸ਼ਮਲਵ *truth)
{
    ਪੂਰਨ i;
    memset(truth, 0, k*ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    ਪੂਰਨ ਗਿਣਤੀ_2 = 0;
    ਲਈ(i = 0; i < k; ++i){
        ਜੇ(ਲੜੀ_ਖੋਜ(ਰਸਤਾ, labels[i])){
            truth[i] = 1;
            ++ਗਿਣਤੀ_2;
            //printf("%s %s %d\n", path, labels[i], i);
        }
    }
    ਜੇ(ਗਿਣਤੀ_2 != 1 && (k != 1 || ਗਿਣਤੀ_2 != 0)) ਲਿਖੋ("Too many or too few labels: %d, %s\n", ਗਿਣਤੀ_2, ਰਸਤਾ);
}

ਖਾਲੀ fill_hierarchy(ਦਸ਼ਮਲਵ *truth, ਪੂਰਨ k, ਰੁੱਖ *hierarchy)
{
    ਪੂਰਨ j;
    ਲਈ(j = 0; j < k; ++j){
        ਜੇ(truth[j]){
            ਪੂਰਨ ਮਾਪਾ = hierarchy->ਮਾਪਾ[j];
            ਜਦੋਂ(ਮਾਪਾ >= 0){
                truth[ਮਾਪਾ] = 1;
                ਮਾਪਾ = hierarchy->ਮਾਪਾ[ਮਾਪਾ];
            }
        }
    }
    ਪੂਰਨ i;
    ਪੂਰਨ ਗਿਣਤੀ_2 = 0;
    ਲਈ(j = 0; j < hierarchy->groups; ++j){
        //printf("%d\n", count);
        ਪੂਰਨ ਨਕਾਬ = 1;
        ਲਈ(i = 0; i < hierarchy->ਸਮੂਹ_ਮਾਪ[j]; ++i){
            ਜੇ(truth[ਗਿਣਤੀ_2 + i]){
                ਨਕਾਬ = 0;
                ਰੋਕੋ;
            }
        }
        ਜੇ (ਨਕਾਬ) {
            ਲਈ(i = 0; i < hierarchy->ਸਮੂਹ_ਮਾਪ[j]; ++i){
                truth[ਗਿਣਤੀ_2 + i] = SECRET_ਅੰਕ;
            }
        }
        ਗਿਣਤੀ_2 += hierarchy->ਸਮੂਹ_ਮਾਪ[j];
    }
}

ਮੈਟ੍ਰਿਕਸ ਲੱਦੋ_regression_labels_paths(ਅੱਖਰ **paths, ਪੂਰਨ n, ਪੂਰਨ k)
{
    ਮੈਟ੍ਰਿਕਸ y = make_ਮੈਟ੍ਰਿਕਸ(n, k);
    ਪੂਰਨ i,j;
    ਲਈ(i = 0; i < n; ++i){
        ਅੱਖਰ labelpath[4096];
        ਲੱਭੋ_replace(paths[i], "images", "labels", labelpath);
        ਲੱਭੋ_replace(labelpath, "JPEGImages", "labels", labelpath);
        ਲੱਭੋ_replace(labelpath, ".BMP", ".txt", labelpath);
        ਲੱਭੋ_replace(labelpath, ".JPEG", ".txt", labelpath);
        ਲੱਭੋ_replace(labelpath, ".JPG", ".txt", labelpath);
        ਲੱਭੋ_replace(labelpath, ".JPeG", ".txt", labelpath);
        ਲੱਭੋ_replace(labelpath, ".Jpeg", ".txt", labelpath);
        ਲੱਭੋ_replace(labelpath, ".PNG", ".txt", labelpath);
        ਲੱਭੋ_replace(labelpath, ".TIF", ".txt", labelpath);
        ਲੱਭੋ_replace(labelpath, ".bmp", ".txt", labelpath);
        ਲੱਭੋ_replace(labelpath, ".jpeg", ".txt", labelpath);
        ਲੱਭੋ_replace(labelpath, ".jpg", ".txt", labelpath);
        ਲੱਭੋ_replace(labelpath, ".png", ".txt", labelpath);
        ਲੱਭੋ_replace(labelpath, ".tif", ".txt", labelpath);

        FILE *file = fopen(labelpath, "r");
        ਲਈ(j = 0; j < k; ++j){
            fscanf(file, "%f", &(y.vals[i][j]));
        }
        fclose(file);
    }
    ਵਾਪਸ y;
}

ਮੈਟ੍ਰਿਕਸ ਲੱਦੋ_labels_paths(ਅੱਖਰ **paths, ਪੂਰਨ n, ਅੱਖਰ **labels, ਪੂਰਨ k, ਰੁੱਖ *hierarchy)
{
    ਮੈਟ੍ਰਿਕਸ y = make_ਮੈਟ੍ਰਿਕਸ(n, k);
    ਪੂਰਨ i;
    ਲਈ(i = 0; i < n && labels; ++i){
        fill_truth(paths[i], labels, k, y.vals[i]);
        ਜੇ(hierarchy){
            fill_hierarchy(y.vals[i], k, hierarchy);
        }
    }
    ਵਾਪਸ y;
}

ਮੈਟ੍ਰਿਕਸ ਲੱਦੋ_ਟੈਗ_ਸੂਚੀ_paths(ਅੱਖਰ **paths, ਪੂਰਨ n, ਪੂਰਨ k)
{
    ਮੈਟ੍ਰਿਕਸ y = make_ਮੈਟ੍ਰਿਕਸ(n, k);
    ਪੂਰਨ i;
    //int count = 0;
    ਲਈ(i = 0; i < n; ++i){
        ਅੱਖਰ label[4096];
        ਲੱਭੋ_replace(paths[i], "images", "labels", label);
        ਲੱਭੋ_replace(label, ".jpg", ".txt", label);
        FILE *file = fopen(label, "r");
        ਜੇ (!file) ਜਾਰੀ;
        //++count;
        ਪੂਰਨ ਟੈਗ;
        ਜਦੋਂ(fscanf(file, "%d", &ਟੈਗ) == 1){
            ਜੇ(ਟੈਗ < k){
                y.vals[i][ਟੈਗ] = 1;
            }
        }
        fclose(file);
    }
    //printf("%d/%d\n", count, n);
    ਵਾਪਸ y;
}

ਅੱਖਰ **get_labels(ਅੱਖਰ *filename)
{
    list *plist = get_paths(filename);
    ਅੱਖਰ **labels = (ਅੱਖਰ **)list_ਮੰਜ਼ਿਲ_array(plist);
    free_list(plist);
    ਵਾਪਸ labels;
}

ਖਾਲੀ free_ਅੰਕੜੇ(ਅੰਕੜੇ d)
{
    ਜੇ(!d.shallow){
        free_ਮੈਟ੍ਰਿਕਸ(d.X);
        free_ਮੈਟ੍ਰਿਕਸ(d.y);
    }ਨਹੀਂਤਾਂ{
        ਮੁਕਤ(d.X.vals);
        ਮੁਕਤ(d.y.vals);
    }
}

image get_segmentation_image(ਅੱਖਰ *ਰਸਤਾ, ਪੂਰਨ w, ਪੂਰਨ h, ਪੂਰਨ classes)
{
    ਅੱਖਰ labelpath[4096];
    ਲੱਭੋ_replace(ਰਸਤਾ, "images", "mask", labelpath);
    ਲੱਭੋ_replace(labelpath, "JPEGImages", "mask", labelpath);
    ਲੱਭੋ_replace(labelpath, ".jpg", ".txt", labelpath);
    ਲੱਭੋ_replace(labelpath, ".JPG", ".txt", labelpath);
    ਲੱਭੋ_replace(labelpath, ".JPEG", ".txt", labelpath);
    image ਨਕਾਬ = make_image(w, h, classes);
    FILE *file = fopen(labelpath, "r");
    ਜੇ(!file) file_error(labelpath);
    ਅੱਖਰ buff[32788];
    ਪੂਰਨ id;
    image part = make_image(w, h, 1);
    ਜਦੋਂ(fscanf(file, "%d %s", &id, buff) == 2){
        ਪੂਰਨ n = 0;
        ਪੂਰਨ *rle = read_intlist(buff, &n, 0);
        ਲੱਦੋ_rle(part, rle, n);
        or_image(part, ਨਕਾਬ, id);
        ਮੁਕਤ(rle);
    }
    //exclusive_image(mask);
    fclose(file);
    free_image(part);
    ਵਾਪਸ ਨਕਾਬ;
}

image get_segmentation_image2(ਅੱਖਰ *ਰਸਤਾ, ਪੂਰਨ w, ਪੂਰਨ h, ਪੂਰਨ classes)
{
    ਅੱਖਰ labelpath[4096];
    ਲੱਭੋ_replace(ਰਸਤਾ, "images", "mask", labelpath);
    ਲੱਭੋ_replace(labelpath, "JPEGImages", "mask", labelpath);
    ਲੱਭੋ_replace(labelpath, ".jpg", ".txt", labelpath);
    ਲੱਭੋ_replace(labelpath, ".JPG", ".txt", labelpath);
    ਲੱਭੋ_replace(labelpath, ".JPEG", ".txt", labelpath);
    image ਨਕਾਬ = make_image(w, h, classes+1);
    ਪੂਰਨ i;
    ਲਈ(i = 0; i < w*h; ++i){
        ਨਕਾਬ.ਅੰਕੜੇ[w*h*classes + i] = 1;
    }
    FILE *file = fopen(labelpath, "r");
    ਜੇ(!file) file_error(labelpath);
    ਅੱਖਰ buff[32788];
    ਪੂਰਨ id;
    image part = make_image(w, h, 1);
    ਜਦੋਂ(fscanf(file, "%d %s", &id, buff) == 2){
        ਪੂਰਨ n = 0;
        ਪੂਰਨ *rle = read_intlist(buff, &n, 0);
        ਲੱਦੋ_rle(part, rle, n);
        or_image(part, ਨਕਾਬ, id);
        ਲਈ(i = 0; i < w*h; ++i){
            ਜੇ(part.ਅੰਕੜੇ[i]) ਨਕਾਬ.ਅੰਕੜੇ[w*h*classes + i] = 0;
        }
        ਮੁਕਤ(rle);
    }
    //exclusive_image(mask);
    fclose(file);
    free_image(part);
    ਵਾਪਸ ਨਕਾਬ;
}

ਅੰਕੜੇ ਲੱਦੋ_ਅੰਕੜੇ_seg(ਪੂਰਨ n, ਅੱਖਰ **paths, ਪੂਰਨ m, ਪੂਰਨ w, ਪੂਰਨ h, ਪੂਰਨ classes, ਪੂਰਨ min, ਪੂਰਨ max, ਦਸ਼ਮਲਵ angle, ਦਸ਼ਮਲਵ aspect, ਦਸ਼ਮਲਵ hue, ਦਸ਼ਮਲਵ saturation, ਦਸ਼ਮਲਵ exposure, ਪੂਰਨ div)
{
    ਅੱਖਰ **random_paths = get_random_paths(paths, n, m);
    ਪੂਰਨ i;
    ਅੰਕੜੇ d = {0};
    d.shallow = 0;

    d.X.ਪੰਕਤੀਆਂ = n;
    d.X.vals = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(d.X.ਪੰਕਤੀਆਂ, ਆਕਾਰ(ਦਸ਼ਮਲਵ*));
    d.X.ਖਾਨੇ_ਗਿਣਤੀ = h*w*3;


    d.y.ਪੰਕਤੀਆਂ = n;
    d.y.ਖਾਨੇ_ਗਿਣਤੀ = h*w*classes/div/div;
    d.y.vals = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(d.X.ਪੰਕਤੀਆਂ, ਆਕਾਰ(ਦਸ਼ਮਲਵ*));

    ਲਈ(i = 0; i < n; ++i){
        image orig = ਲੱਦੋ_image_color(random_paths[i], 0, 0);
        augment_ਦਲੀਲਾਂ a = random_augment_ਦਲੀਲਾਂ(orig, angle, aspect, min, max, w, h);
        image sized = rotate_crop_image(orig, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);

        ਪੂਰਨ flip = rand()%2;
        ਜੇ(flip) flip_image(sized);
        random_distort_image(sized, hue, saturation, exposure);
        d.X.vals[i] = sized.ਅੰਕੜੇ;

        image ਨਕਾਬ = get_segmentation_image(random_paths[i], orig.w, orig.h, classes);
        //image mask = make_image(orig.w, orig.h, classes+1);
        image sized_m = rotate_crop_image(ਨਕਾਬ, a.rad, a.scale/div, a.w/div, a.h/div, a.dx/div, a.dy/div, a.aspect);

        ਜੇ(flip) flip_image(sized_m);
        d.y.vals[i] = sized_m.ਅੰਕੜੇ;

        free_image(orig);
        free_image(ਨਕਾਬ);

        /*
           image rgb = mask_to_rgb(sized_m, classes);
           show_image(rgb, "part");
           show_image(sized, "orig");
           cvWaitKey(0);
           free_image(rgb);
         */
    }
    ਮੁਕਤ(random_paths);
    ਵਾਪਸ d;
}

ਅੰਕੜੇ ਲੱਦੋ_ਅੰਕੜੇ_iseg(ਪੂਰਨ n, ਅੱਖਰ **paths, ਪੂਰਨ m, ਪੂਰਨ w, ਪੂਰਨ h, ਪੂਰਨ classes, ਪੂਰਨ boxes, ਪੂਰਨ div, ਪੂਰਨ min, ਪੂਰਨ max, ਦਸ਼ਮਲਵ angle, ਦਸ਼ਮਲਵ aspect, ਦਸ਼ਮਲਵ hue, ਦਸ਼ਮਲਵ saturation, ਦਸ਼ਮਲਵ exposure)
{
    ਅੱਖਰ **random_paths = get_random_paths(paths, n, m);
    ਪੂਰਨ i;
    ਅੰਕੜੇ d = {0};
    d.shallow = 0;

    d.X.ਪੰਕਤੀਆਂ = n;
    d.X.vals = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(d.X.ਪੰਕਤੀਆਂ, ਆਕਾਰ(ਦਸ਼ਮਲਵ*));
    d.X.ਖਾਨੇ_ਗਿਣਤੀ = h*w*3;

    d.y = make_ਮੈਟ੍ਰਿਕਸ(n, (((w/div)*(h/div))+1)*boxes);

    ਲਈ(i = 0; i < n; ++i){
        image orig = ਲੱਦੋ_image_color(random_paths[i], 0, 0);
        augment_ਦਲੀਲਾਂ a = random_augment_ਦਲੀਲਾਂ(orig, angle, aspect, min, max, w, h);
        image sized = rotate_crop_image(orig, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);

        ਪੂਰਨ flip = rand()%2;
        ਜੇ(flip) flip_image(sized);
        random_distort_image(sized, hue, saturation, exposure);
        d.X.vals[i] = sized.ਅੰਕੜੇ;
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
    ਮੁਕਤ(random_paths);
    ਵਾਪਸ d;
}

ਅੰਕੜੇ ਲੱਦੋ_ਅੰਕੜੇ_ਨਕਾਬ(ਪੂਰਨ n, ਅੱਖਰ **paths, ਪੂਰਨ m, ਪੂਰਨ w, ਪੂਰਨ h, ਪੂਰਨ classes, ਪੂਰਨ boxes, ਪੂਰਨ coords, ਪੂਰਨ min, ਪੂਰਨ max, ਦਸ਼ਮਲਵ angle, ਦਸ਼ਮਲਵ aspect, ਦਸ਼ਮਲਵ hue, ਦਸ਼ਮਲਵ saturation, ਦਸ਼ਮਲਵ exposure)
{
    ਅੱਖਰ **random_paths = get_random_paths(paths, n, m);
    ਪੂਰਨ i;
    ਅੰਕੜੇ d = {0};
    d.shallow = 0;

    d.X.ਪੰਕਤੀਆਂ = n;
    d.X.vals = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(d.X.ਪੰਕਤੀਆਂ, ਆਕਾਰ(ਦਸ਼ਮਲਵ*));
    d.X.ਖਾਨੇ_ਗਿਣਤੀ = h*w*3;

    d.y = make_ਮੈਟ੍ਰਿਕਸ(n, (coords+1)*boxes);

    ਲਈ(i = 0; i < n; ++i){
        image orig = ਲੱਦੋ_image_color(random_paths[i], 0, 0);
        augment_ਦਲੀਲਾਂ a = random_augment_ਦਲੀਲਾਂ(orig, angle, aspect, min, max, w, h);
        image sized = rotate_crop_image(orig, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);

        ਪੂਰਨ flip = rand()%2;
        ਜੇ(flip) flip_image(sized);
        random_distort_image(sized, hue, saturation, exposure);
        d.X.vals[i] = sized.ਅੰਕੜੇ;
        //show_image(sized, "image");

        fill_truth_ਨਕਾਬ(random_paths[i], boxes, d.y.vals[i], classes, orig.w, orig.h, a, flip, 14, 14);

        free_image(orig);

        /*
           image rgb = mask_to_rgb(sized_m, classes);
           show_image(rgb, "part");
           show_image(sized, "orig");
           cvWaitKey(0);
           free_image(rgb);
         */
    }
    ਮੁਕਤ(random_paths);
    ਵਾਪਸ d;
}

ਅੰਕੜੇ ਲੱਦੋ_ਅੰਕੜੇ_region(ਪੂਰਨ n, ਅੱਖਰ **paths, ਪੂਰਨ m, ਪੂਰਨ w, ਪੂਰਨ h, ਪੂਰਨ ਮਾਪ, ਪੂਰਨ classes, ਦਸ਼ਮਲਵ jitter, ਦਸ਼ਮਲਵ hue, ਦਸ਼ਮਲਵ saturation, ਦਸ਼ਮਲਵ exposure)
{
    ਅੱਖਰ **random_paths = get_random_paths(paths, n, m);
    ਪੂਰਨ i;
    ਅੰਕੜੇ d = {0};
    d.shallow = 0;

    d.X.ਪੰਕਤੀਆਂ = n;
    d.X.vals = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(d.X.ਪੰਕਤੀਆਂ, ਆਕਾਰ(ਦਸ਼ਮਲਵ*));
    d.X.ਖਾਨੇ_ਗਿਣਤੀ = h*w*3;


    ਪੂਰਨ k = ਮਾਪ*ਮਾਪ*(5+classes);
    d.y = make_ਮੈਟ੍ਰਿਕਸ(n, k);
    ਲਈ(i = 0; i < n; ++i){
        image orig = ਲੱਦੋ_image_color(random_paths[i], 0, 0);

        ਪੂਰਨ oh = orig.h;
        ਪੂਰਨ ow = orig.w;

        ਪੂਰਨ dw = (ow*jitter);
        ਪੂਰਨ dh = (oh*jitter);

        ਪੂਰਨ pleft  = rand_ਇਕਸਾਰ(-dw, dw);
        ਪੂਰਨ pright = rand_ਇਕਸਾਰ(-dw, dw);
        ਪੂਰਨ ptop   = rand_ਇਕਸਾਰ(-dh, dh);
        ਪੂਰਨ pbot   = rand_ਇਕਸਾਰ(-dh, dh);

        ਪੂਰਨ swidth =  ow - pleft - pright;
        ਪੂਰਨ sheight = oh - ptop - pbot;

        ਦਸ਼ਮਲਵ sx = (ਦਸ਼ਮਲਵ)swidth  / ow;
        ਦਸ਼ਮਲਵ sy = (ਦਸ਼ਮਲਵ)sheight / oh;

        ਪੂਰਨ flip = rand()%2;
        image cropped = crop_image(orig, pleft, ptop, swidth, sheight);

        ਦਸ਼ਮਲਵ dx = ((ਦਸ਼ਮਲਵ)pleft/ow)/sx;
        ਦਸ਼ਮਲਵ dy = ((ਦਸ਼ਮਲਵ)ptop /oh)/sy;

        image sized = resize_image(cropped, w, h);
        ਜੇ(flip) flip_image(sized);
        random_distort_image(sized, hue, saturation, exposure);
        d.X.vals[i] = sized.ਅੰਕੜੇ;

        fill_truth_region(random_paths[i], d.y.vals[i], classes, ਮਾਪ, flip, dx, dy, 1./sx, 1./sy);

        free_image(orig);
        free_image(cropped);
    }
    ਮੁਕਤ(random_paths);
    ਵਾਪਸ d;
}

ਅੰਕੜੇ ਲੱਦੋ_ਅੰਕੜੇ_compare(ਪੂਰਨ n, ਅੱਖਰ **paths, ਪੂਰਨ m, ਪੂਰਨ classes, ਪੂਰਨ w, ਪੂਰਨ h)
{
    ਜੇ(m) paths = get_random_paths(paths, 2*n, m);
    ਪੂਰਨ i,j;
    ਅੰਕੜੇ d = {0};
    d.shallow = 0;

    d.X.ਪੰਕਤੀਆਂ = n;
    d.X.vals = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(d.X.ਪੰਕਤੀਆਂ, ਆਕਾਰ(ਦਸ਼ਮਲਵ*));
    d.X.ਖਾਨੇ_ਗਿਣਤੀ = h*w*6;

    ਪੂਰਨ k = 2*(classes);
    d.y = make_ਮੈਟ੍ਰਿਕਸ(n, k);
    ਲਈ(i = 0; i < n; ++i){
        image im1 = ਲੱਦੋ_image_color(paths[i*2],   w, h);
        image im2 = ਲੱਦੋ_image_color(paths[i*2+1], w, h);

        d.X.vals[i] = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(d.X.ਖਾਨੇ_ਗਿਣਤੀ, ਆਕਾਰ(ਦਸ਼ਮਲਵ));
        ਸਮ੍ਰਿਤੀ_ਨਕਲ(d.X.vals[i],         im1.ਅੰਕੜੇ, h*w*3*ਆਕਾਰ(ਦਸ਼ਮਲਵ));
        ਸਮ੍ਰਿਤੀ_ਨਕਲ(d.X.vals[i] + h*w*3, im2.ਅੰਕੜੇ, h*w*3*ਆਕਾਰ(ਦਸ਼ਮਲਵ));

        ਪੂਰਨ id;
        ਦਸ਼ਮਲਵ iou;

        ਅੱਖਰ imlabel1[4096];
        ਅੱਖਰ imlabel2[4096];
        ਲੱਭੋ_replace(paths[i*2],   "imgs", "labels", imlabel1);
        ਲੱਭੋ_replace(imlabel1, "jpg", "txt", imlabel1);
        FILE *fp1 = fopen(imlabel1, "r");

        ਜਦੋਂ(fscanf(fp1, "%d %f", &id, &iou) == 2){
            ਜੇ (d.y.vals[i][2*id] < iou) d.y.vals[i][2*id] = iou;
        }

        ਲੱਭੋ_replace(paths[i*2+1], "imgs", "labels", imlabel2);
        ਲੱਭੋ_replace(imlabel2, "jpg", "txt", imlabel2);
        FILE *fp2 = fopen(imlabel2, "r");

        ਜਦੋਂ(fscanf(fp2, "%d %f", &id, &iou) == 2){
            ਜੇ (d.y.vals[i][2*id + 1] < iou) d.y.vals[i][2*id + 1] = iou;
        }

        ਲਈ (j = 0; j < classes; ++j){
            ਜੇ (d.y.vals[i][2*j] > .5 &&  d.y.vals[i][2*j+1] < .5){
                d.y.vals[i][2*j] = 1;
                d.y.vals[i][2*j+1] = 0;
            } ਨਹੀਂਤਾਂ ਜੇ (d.y.vals[i][2*j] < .5 &&  d.y.vals[i][2*j+1] > .5){
                d.y.vals[i][2*j] = 0;
                d.y.vals[i][2*j+1] = 1;
            } ਨਹੀਂਤਾਂ {
                d.y.vals[i][2*j]   = SECRET_ਅੰਕ;
                d.y.vals[i][2*j+1] = SECRET_ਅੰਕ;
            }
        }
        fclose(fp1);
        fclose(fp2);

        free_image(im1);
        free_image(im2);
    }
    ਜੇ(m) ਮੁਕਤ(paths);
    ਵਾਪਸ d;
}

ਅੰਕੜੇ ਲੱਦੋ_ਅੰਕੜੇ_swag(ਅੱਖਰ **paths, ਪੂਰਨ n, ਪੂਰਨ classes, ਦਸ਼ਮਲਵ jitter)
{
    ਪੂਰਨ ਸੂਚਕਾਂਕ = rand()%n;
    ਅੱਖਰ *random_ਰਸਤਾ = paths[ਸੂਚਕਾਂਕ];

    image orig = ਲੱਦੋ_image_color(random_ਰਸਤਾ, 0, 0);
    ਪੂਰਨ h = orig.h;
    ਪੂਰਨ w = orig.w;

    ਅੰਕੜੇ d = {0};
    d.shallow = 0;
    d.w = w;
    d.h = h;

    d.X.ਪੰਕਤੀਆਂ = 1;
    d.X.vals = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(d.X.ਪੰਕਤੀਆਂ, ਆਕਾਰ(ਦਸ਼ਮਲਵ*));
    d.X.ਖਾਨੇ_ਗਿਣਤੀ = h*w*3;

    ਪੂਰਨ k = (4+classes)*90;
    d.y = make_ਮੈਟ੍ਰਿਕਸ(1, k);

    ਪੂਰਨ dw = w*jitter;
    ਪੂਰਨ dh = h*jitter;

    ਪੂਰਨ pleft  = rand_ਇਕਸਾਰ(-dw, dw);
    ਪੂਰਨ pright = rand_ਇਕਸਾਰ(-dw, dw);
    ਪੂਰਨ ptop   = rand_ਇਕਸਾਰ(-dh, dh);
    ਪੂਰਨ pbot   = rand_ਇਕਸਾਰ(-dh, dh);

    ਪੂਰਨ swidth =  w - pleft - pright;
    ਪੂਰਨ sheight = h - ptop - pbot;

    ਦਸ਼ਮਲਵ sx = (ਦਸ਼ਮਲਵ)swidth  / w;
    ਦਸ਼ਮਲਵ sy = (ਦਸ਼ਮਲਵ)sheight / h;

    ਪੂਰਨ flip = rand()%2;
    image cropped = crop_image(orig, pleft, ptop, swidth, sheight);

    ਦਸ਼ਮਲਵ dx = ((ਦਸ਼ਮਲਵ)pleft/w)/sx;
    ਦਸ਼ਮਲਵ dy = ((ਦਸ਼ਮਲਵ)ptop /h)/sy;

    image sized = resize_image(cropped, w, h);
    ਜੇ(flip) flip_image(sized);
    d.X.vals[0] = sized.ਅੰਕੜੇ;

    fill_truth_swag(random_ਰਸਤਾ, d.y.vals[0], classes, flip, dx, dy, 1./sx, 1./sy);

    free_image(orig);
    free_image(cropped);

    ਵਾਪਸ d;
}

ਅੰਕੜੇ ਲੱਦੋ_ਅੰਕੜੇ_detection(ਪੂਰਨ n, ਅੱਖਰ **paths, ਪੂਰਨ m, ਪੂਰਨ w, ਪੂਰਨ h, ਪੂਰਨ boxes, ਪੂਰਨ classes, ਦਸ਼ਮਲਵ jitter, ਦਸ਼ਮਲਵ hue, ਦਸ਼ਮਲਵ saturation, ਦਸ਼ਮਲਵ exposure)
{
    ਅੱਖਰ **random_paths = get_random_paths(paths, n, m);
    ਪੂਰਨ i;
    ਅੰਕੜੇ d = {0};
    d.shallow = 0;

    d.X.ਪੰਕਤੀਆਂ = n;
    d.X.vals = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(d.X.ਪੰਕਤੀਆਂ, ਆਕਾਰ(ਦਸ਼ਮਲਵ*));
    d.X.ਖਾਨੇ_ਗਿਣਤੀ = h*w*3;

    d.y = make_ਮੈਟ੍ਰਿਕਸ(n, 5*boxes);
    ਲਈ(i = 0; i < n; ++i){
        image orig = ਲੱਦੋ_image_color(random_paths[i], 0, 0);
        image sized = make_image(w, h, orig.c);
        fill_image(sized, .5);

        ਦਸ਼ਮਲਵ dw = jitter * orig.w;
        ਦਸ਼ਮਲਵ dh = jitter * orig.h;

        ਦਸ਼ਮਲਵ new_ar = (orig.w + rand_ਇਕਸਾਰ(-dw, dw)) / (orig.h + rand_ਇਕਸਾਰ(-dh, dh));
        //float scale = rand_uniform(.25, 2);
        ਦਸ਼ਮਲਵ scale = 1;

        ਦਸ਼ਮਲਵ nw, nh;

        ਜੇ(new_ar < 1){
            nh = scale * h;
            nw = nh * new_ar;
        } ਨਹੀਂਤਾਂ {
            nw = scale * w;
            nh = nw / new_ar;
        }

        ਦਸ਼ਮਲਵ dx = rand_ਇਕਸਾਰ(0, w - nw);
        ਦਸ਼ਮਲਵ dy = rand_ਇਕਸਾਰ(0, h - nh);

        ਰੱਖੋ_image(orig, nw, nh, dx, dy, sized);

        random_distort_image(sized, hue, saturation, exposure);

        ਪੂਰਨ flip = rand()%2;
        ਜੇ(flip) flip_image(sized);
        d.X.vals[i] = sized.ਅੰਕੜੇ;


        fill_truth_detection(random_paths[i], boxes, d.y.vals[i], classes, flip, -dx/w, -dy/h, nw/w, nh/h);

        free_image(orig);
    }
    ਮੁਕਤ(random_paths);
    ਵਾਪਸ d;
}

ਖਾਲੀ *ਲੱਦੋ_thread(ਖਾਲੀ *ptr)
{
    //printf("Loading data: %d\n", rand());
    ਲੱਦੋ_ਦਲੀਲਾਂ a = *(ਬਣਤਰ ਲੱਦੋ_ਦਲੀਲਾਂ*)ptr;
    ਜੇ(a.exposure == 0) a.exposure = 1;
    ਜੇ(a.saturation == 0) a.saturation = 1;
    ਜੇ(a.aspect == 0) a.aspect = 1;

    ਜੇ (a.type == OLD_CLASSIFICATION_ਅੰਕੜੇ){
        *a.d = ਲੱਦੋ_ਅੰਕੜੇ_old(a.paths, a.n, a.m, a.labels, a.classes, a.w, a.h);
    } ਨਹੀਂਤਾਂ ਜੇ (a.type == REGRESSION_ਅੰਕੜੇ){
        *a.d = ਲੱਦੋ_ਅੰਕੜੇ_regression(a.paths, a.n, a.m, a.classes, a.min, a.max, a.ਮਾਪ, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    } ਨਹੀਂਤਾਂ ਜੇ (a.type == CLASSIFICATION_ਅੰਕੜੇ){
        *a.d = ਲੱਦੋ_ਅੰਕੜੇ_augment(a.paths, a.n, a.m, a.labels, a.classes, a.hierarchy, a.min, a.max, a.ਮਾਪ, a.angle, a.aspect, a.hue, a.saturation, a.exposure, a.center);
    } ਨਹੀਂਤਾਂ ਜੇ (a.type == SUPER_ਅੰਕੜੇ){
        *a.d = ਲੱਦੋ_ਅੰਕੜੇ_super(a.paths, a.n, a.m, a.w, a.h, a.scale);
    } ਨਹੀਂਤਾਂ ਜੇ (a.type == WRITING_ਅੰਕੜੇ){
        *a.d = ਲੱਦੋ_ਅੰਕੜੇ_writing(a.paths, a.n, a.m, a.w, a.h, a.ਬਾਹਰਲਾ_w, a.ਬਾਹਰਲਾ_h);
    } ਨਹੀਂਤਾਂ ਜੇ (a.type == ISEG_ਅੰਕੜੇ){
        *a.d = ਲੱਦੋ_ਅੰਕੜੇ_iseg(a.n, a.paths, a.m, a.w, a.h, a.classes, a.ਅੰਕ_boxes, a.scale, a.min, a.max, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    } ਨਹੀਂਤਾਂ ਜੇ (a.type == INSTANCE_ਅੰਕੜੇ){
        *a.d = ਲੱਦੋ_ਅੰਕੜੇ_ਨਕਾਬ(a.n, a.paths, a.m, a.w, a.h, a.classes, a.ਅੰਕ_boxes, a.coords, a.min, a.max, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    } ਨਹੀਂਤਾਂ ਜੇ (a.type == SEGMENTATION_ਅੰਕੜੇ){
        *a.d = ਲੱਦੋ_ਅੰਕੜੇ_seg(a.n, a.paths, a.m, a.w, a.h, a.classes, a.min, a.max, a.angle, a.aspect, a.hue, a.saturation, a.exposure, a.scale);
    } ਨਹੀਂਤਾਂ ਜੇ (a.type == REGION_ਅੰਕੜੇ){
        *a.d = ਲੱਦੋ_ਅੰਕੜੇ_region(a.n, a.paths, a.m, a.w, a.h, a.ਅੰਕ_boxes, a.classes, a.jitter, a.hue, a.saturation, a.exposure);
    } ਨਹੀਂਤਾਂ ਜੇ (a.type == DETECTION_ਅੰਕੜੇ){
        *a.d = ਲੱਦੋ_ਅੰਕੜੇ_detection(a.n, a.paths, a.m, a.w, a.h, a.ਅੰਕ_boxes, a.classes, a.jitter, a.hue, a.saturation, a.exposure);
    } ਨਹੀਂਤਾਂ ਜੇ (a.type == SWAG_ਅੰਕੜੇ){
        *a.d = ਲੱਦੋ_ਅੰਕੜੇ_swag(a.paths, a.n, a.classes, a.jitter);
    } ਨਹੀਂਤਾਂ ਜੇ (a.type == COMPARE_ਅੰਕੜੇ){
        *a.d = ਲੱਦੋ_ਅੰਕੜੇ_compare(a.n, a.paths, a.m, a.classes, a.w, a.h);
    } ਨਹੀਂਤਾਂ ਜੇ (a.type == IMAGE_ਅੰਕੜੇ){
        *(a.im) = ਲੱਦੋ_image_color(a.ਰਸਤਾ, 0, 0);
        *(a.resized) = resize_image(*(a.im), a.w, a.h);
    } ਨਹੀਂਤਾਂ ਜੇ (a.type == LETTERBOX_ਅੰਕੜੇ){
        *(a.im) = ਲੱਦੋ_image_color(a.ਰਸਤਾ, 0, 0);
        *(a.resized) = letterbox_image(*(a.im), a.w, a.h);
    } ਨਹੀਂਤਾਂ ਜੇ (a.type == ਟੈਗ_ਅੰਕੜੇ){
        *a.d = ਲੱਦੋ_ਅੰਕੜੇ_ਟੈਗ(a.paths, a.n, a.m, a.classes, a.min, a.max, a.ਮਾਪ, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    }
    ਮੁਕਤ(ptr);
    ਵਾਪਸ 0;
}

pthread_t ਲੱਦੋ_ਅੰਕੜੇ_ਵਿੱਚ_thread(ਲੱਦੋ_ਦਲੀਲਾਂ ਦਲੀਲਾਂ)
{
    pthread_t thread;
    ਬਣਤਰ ਲੱਦੋ_ਦਲੀਲਾਂ *ptr = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(1, ਆਕਾਰ(ਬਣਤਰ ਲੱਦੋ_ਦਲੀਲਾਂ));
    *ptr = ਦਲੀਲਾਂ;
    ਜੇ(pthread_create(&thread, 0, ਲੱਦੋ_thread, ptr)) error("Thread creation failed");
    ਵਾਪਸ thread;
}

ਖਾਲੀ *ਲੱਦੋ_ਧਾਗੇ(ਖਾਲੀ *ptr)
{
    ਪੂਰਨ i;
    ਲੱਦੋ_ਦਲੀਲਾਂ ਦਲੀਲਾਂ = *(ਲੱਦੋ_ਦਲੀਲਾਂ *)ptr;
    ਜੇ (ਦਲੀਲਾਂ.ਧਾਗੇ == 0) ਦਲੀਲਾਂ.ਧਾਗੇ = 1;
    ਅੰਕੜੇ *ਬਾਹਰਲਾ = ਦਲੀਲਾਂ.d;
    ਪੂਰਨ ਕੁੱਲ = ਦਲੀਲਾਂ.n;
    ਮੁਕਤ(ptr);
    ਅੰਕੜੇ *buffers = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਦਲੀਲਾਂ.ਧਾਗੇ, ਆਕਾਰ(ਅੰਕੜੇ));
    pthread_t *ਧਾਗੇ = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਦਲੀਲਾਂ.ਧਾਗੇ, ਆਕਾਰ(pthread_t));
    ਲਈ(i = 0; i < ਦਲੀਲਾਂ.ਧਾਗੇ; ++i){
        ਦਲੀਲਾਂ.d = buffers + i;
        ਦਲੀਲਾਂ.n = (i+1) * ਕੁੱਲ/ਦਲੀਲਾਂ.ਧਾਗੇ - i * ਕੁੱਲ/ਦਲੀਲਾਂ.ਧਾਗੇ;
        ਧਾਗੇ[i] = ਲੱਦੋ_ਅੰਕੜੇ_ਵਿੱਚ_thread(ਦਲੀਲਾਂ);
    }
    ਲਈ(i = 0; i < ਦਲੀਲਾਂ.ਧਾਗੇ; ++i){
        pthread_join(ਧਾਗੇ[i], 0);
    }
    *ਬਾਹਰਲਾ = concat_datas(buffers, ਦਲੀਲਾਂ.ਧਾਗੇ);
    ਬਾਹਰਲਾ->shallow = 0;
    ਲਈ(i = 0; i < ਦਲੀਲਾਂ.ਧਾਗੇ; ++i){
        buffers[i].shallow = 1;
        free_ਅੰਕੜੇ(buffers[i]);
    }
    ਮੁਕਤ(buffers);
    ਮੁਕਤ(ਧਾਗੇ);
    ਵਾਪਸ 0;
}

ਖਾਲੀ ਲੱਦੋ_ਅੰਕੜੇ_blocking(ਲੱਦੋ_ਦਲੀਲਾਂ ਦਲੀਲਾਂ)
{
    ਬਣਤਰ ਲੱਦੋ_ਦਲੀਲਾਂ *ptr = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(1, ਆਕਾਰ(ਬਣਤਰ ਲੱਦੋ_ਦਲੀਲਾਂ));
    *ptr = ਦਲੀਲਾਂ;
    ਲੱਦੋ_thread(ptr);
}

pthread_t ਲੱਦੋ_ਅੰਕੜੇ(ਲੱਦੋ_ਦਲੀਲਾਂ ਦਲੀਲਾਂ)
{
    pthread_t thread;
    ਬਣਤਰ ਲੱਦੋ_ਦਲੀਲਾਂ *ptr = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(1, ਆਕਾਰ(ਬਣਤਰ ਲੱਦੋ_ਦਲੀਲਾਂ));
    *ptr = ਦਲੀਲਾਂ;
    ਜੇ(pthread_create(&thread, 0, ਲੱਦੋ_ਧਾਗੇ, ptr)) error("Thread creation failed");
    ਵਾਪਸ thread;
}

ਅੰਕੜੇ ਲੱਦੋ_ਅੰਕੜੇ_writing(ਅੱਖਰ **paths, ਪੂਰਨ n, ਪੂਰਨ m, ਪੂਰਨ w, ਪੂਰਨ h, ਪੂਰਨ ਬਾਹਰਲਾ_w, ਪੂਰਨ ਬਾਹਰਲਾ_h)
{
    ਜੇ(m) paths = get_random_paths(paths, n, m);
    ਅੱਖਰ **replace_paths = ਲੱਭੋ_replace_paths(paths, n, ".png", "-label.png");
    ਅੰਕੜੇ d = {0};
    d.shallow = 0;
    d.X = ਲੱਦੋ_image_paths(paths, n, w, h);
    d.y = ਲੱਦੋ_image_paths_ਸਲੇਟੀ(replace_paths, n, ਬਾਹਰਲਾ_w, ਬਾਹਰਲਾ_h);
    ਜੇ(m) ਮੁਕਤ(paths);
    ਪੂਰਨ i;
    ਲਈ(i = 0; i < n; ++i) ਮੁਕਤ(replace_paths[i]);
    ਮੁਕਤ(replace_paths);
    ਵਾਪਸ d;
}

ਅੰਕੜੇ ਲੱਦੋ_ਅੰਕੜੇ_old(ਅੱਖਰ **paths, ਪੂਰਨ n, ਪੂਰਨ m, ਅੱਖਰ **labels, ਪੂਰਨ k, ਪੂਰਨ w, ਪੂਰਨ h)
{
    ਜੇ(m) paths = get_random_paths(paths, n, m);
    ਅੰਕੜੇ d = {0};
    d.shallow = 0;
    d.X = ਲੱਦੋ_image_paths(paths, n, w, h);
    d.y = ਲੱਦੋ_labels_paths(paths, n, labels, k, 0);
    ਜੇ(m) ਮੁਕਤ(paths);
    ਵਾਪਸ d;
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

ਅੰਕੜੇ ਲੱਦੋ_ਅੰਕੜੇ_super(ਅੱਖਰ **paths, ਪੂਰਨ n, ਪੂਰਨ m, ਪੂਰਨ w, ਪੂਰਨ h, ਪੂਰਨ scale)
{
    ਜੇ(m) paths = get_random_paths(paths, n, m);
    ਅੰਕੜੇ d = {0};
    d.shallow = 0;

    ਪੂਰਨ i;
    d.X.ਪੰਕਤੀਆਂ = n;
    d.X.vals = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(n, ਆਕਾਰ(ਦਸ਼ਮਲਵ*));
    d.X.ਖਾਨੇ_ਗਿਣਤੀ = w*h*3;

    d.y.ਪੰਕਤੀਆਂ = n;
    d.y.vals = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(n, ਆਕਾਰ(ਦਸ਼ਮਲਵ*));
    d.y.ਖਾਨੇ_ਗਿਣਤੀ = w*scale * h*scale * 3;

    ਲਈ(i = 0; i < n; ++i){
        image im = ਲੱਦੋ_image_color(paths[i], 0, 0);
        image crop = random_crop_image(im, w*scale, h*scale);
        ਪੂਰਨ flip = rand()%2;
        ਜੇ (flip) flip_image(crop);
        image resize = resize_image(crop, w, h);
        d.X.vals[i] = resize.ਅੰਕੜੇ;
        d.y.vals[i] = crop.ਅੰਕੜੇ;
        free_image(im);
    }

    ਜੇ(m) ਮੁਕਤ(paths);
    ਵਾਪਸ d;
}

ਅੰਕੜੇ ਲੱਦੋ_ਅੰਕੜੇ_regression(ਅੱਖਰ **paths, ਪੂਰਨ n, ਪੂਰਨ m, ਪੂਰਨ k, ਪੂਰਨ min, ਪੂਰਨ max, ਪੂਰਨ ਮਾਪ, ਦਸ਼ਮਲਵ angle, ਦਸ਼ਮਲਵ aspect, ਦਸ਼ਮਲਵ hue, ਦਸ਼ਮਲਵ saturation, ਦਸ਼ਮਲਵ exposure)
{
    ਜੇ(m) paths = get_random_paths(paths, n, m);
    ਅੰਕੜੇ d = {0};
    d.shallow = 0;
    d.X = ਲੱਦੋ_image_augment_paths(paths, n, min, max, ਮਾਪ, angle, aspect, hue, saturation, exposure, 0);
    d.y = ਲੱਦੋ_regression_labels_paths(paths, n, k);
    ਜੇ(m) ਮੁਕਤ(paths);
    ਵਾਪਸ d;
}

ਅੰਕੜੇ select_ਅੰਕੜੇ(ਅੰਕੜੇ *orig, ਪੂਰਨ *inds)
{
    ਅੰਕੜੇ d = {0};
    d.shallow = 1;
    d.w = orig[0].w;
    d.h = orig[0].h;

    d.X.ਪੰਕਤੀਆਂ = orig[0].X.ਪੰਕਤੀਆਂ;
    d.y.ਪੰਕਤੀਆਂ = orig[0].X.ਪੰਕਤੀਆਂ;

    d.X.ਖਾਨੇ_ਗਿਣਤੀ = orig[0].X.ਖਾਨੇ_ਗਿਣਤੀ;
    d.y.ਖਾਨੇ_ਗਿਣਤੀ = orig[0].y.ਖਾਨੇ_ਗਿਣਤੀ;

    d.X.vals = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(orig[0].X.ਪੰਕਤੀਆਂ, ਆਕਾਰ(ਦਸ਼ਮਲਵ *));
    d.y.vals = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(orig[0].y.ਪੰਕਤੀਆਂ, ਆਕਾਰ(ਦਸ਼ਮਲਵ *));
    ਪੂਰਨ i;
    ਲਈ(i = 0; i < d.X.ਪੰਕਤੀਆਂ; ++i){
        d.X.vals[i] = orig[inds[i]].X.vals[i];
        d.y.vals[i] = orig[inds[i]].y.vals[i];
    }
    ਵਾਪਸ d;
}

ਅੰਕੜੇ *tile_ਅੰਕੜੇ(ਅੰਕੜੇ orig, ਪੂਰਨ divs, ਪੂਰਨ ਮਾਪ)
{
    ਅੰਕੜੇ *ds = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(divs*divs, ਆਕਾਰ(ਅੰਕੜੇ));
    ਪੂਰਨ i, j;
#ਹਦਾਇਤ omp parallel ਲਈ
    ਲਈ(i = 0; i < divs*divs; ++i){
        ਅੰਕੜੇ d;
        d.shallow = 0;
        d.w = orig.w/divs * ਮਾਪ;
        d.h = orig.h/divs * ਮਾਪ;
        d.X.ਪੰਕਤੀਆਂ = orig.X.ਪੰਕਤੀਆਂ;
        d.X.ਖਾਨੇ_ਗਿਣਤੀ = d.w*d.h*3;
        d.X.vals = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(d.X.ਪੰਕਤੀਆਂ, ਆਕਾਰ(ਦਸ਼ਮਲਵ*));

        d.y = copy_ਮੈਟ੍ਰਿਕਸ(orig.y);
#ਹਦਾਇਤ omp parallel ਲਈ
        ਲਈ(j = 0; j < orig.X.ਪੰਕਤੀਆਂ; ++j){
            ਪੂਰਨ x = (i%divs) * orig.w / divs - (d.w - orig.w/divs)/2;
            ਪੂਰਨ y = (i/divs) * orig.h / divs - (d.h - orig.h/divs)/2;
            image im = float_ਮੰਜ਼ਿਲ_image(orig.w, orig.h, 3, orig.X.vals[j]);
            d.X.vals[j] = crop_image(im, x, y, d.w, d.h).ਅੰਕੜੇ;
        }
        ds[i] = d;
    }
    ਵਾਪਸ ds;
}

ਅੰਕੜੇ resize_ਅੰਕੜੇ(ਅੰਕੜੇ orig, ਪੂਰਨ w, ਪੂਰਨ h)
{
    ਅੰਕੜੇ d = {0};
    d.shallow = 0;
    d.w = w;
    d.h = h;
    ਪੂਰਨ i;
    d.X.ਪੰਕਤੀਆਂ = orig.X.ਪੰਕਤੀਆਂ;
    d.X.ਖਾਨੇ_ਗਿਣਤੀ = w*h*3;
    d.X.vals = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(d.X.ਪੰਕਤੀਆਂ, ਆਕਾਰ(ਦਸ਼ਮਲਵ*));

    d.y = copy_ਮੈਟ੍ਰਿਕਸ(orig.y);
#ਹਦਾਇਤ omp parallel ਲਈ
    ਲਈ(i = 0; i < orig.X.ਪੰਕਤੀਆਂ; ++i){
        image im = float_ਮੰਜ਼ਿਲ_image(orig.w, orig.h, 3, orig.X.vals[i]);
        d.X.vals[i] = resize_image(im, w, h).ਅੰਕੜੇ;
    }
    ਵਾਪਸ d;
}

ਅੰਕੜੇ ਲੱਦੋ_ਅੰਕੜੇ_augment(ਅੱਖਰ **paths, ਪੂਰਨ n, ਪੂਰਨ m, ਅੱਖਰ **labels, ਪੂਰਨ k, ਰੁੱਖ *hierarchy, ਪੂਰਨ min, ਪੂਰਨ max, ਪੂਰਨ ਮਾਪ, ਦਸ਼ਮਲਵ angle, ਦਸ਼ਮਲਵ aspect, ਦਸ਼ਮਲਵ hue, ਦਸ਼ਮਲਵ saturation, ਦਸ਼ਮਲਵ exposure, ਪੂਰਨ center)
{
    ਜੇ(m) paths = get_random_paths(paths, n, m);
    ਅੰਕੜੇ d = {0};
    d.shallow = 0;
    d.w=ਮਾਪ;
    d.h=ਮਾਪ;
    d.X = ਲੱਦੋ_image_augment_paths(paths, n, min, max, ਮਾਪ, angle, aspect, hue, saturation, exposure, center);
    d.y = ਲੱਦੋ_labels_paths(paths, n, labels, k, hierarchy);
    ਜੇ(m) ਮੁਕਤ(paths);
    ਵਾਪਸ d;
}

ਅੰਕੜੇ ਲੱਦੋ_ਅੰਕੜੇ_ਟੈਗ(ਅੱਖਰ **paths, ਪੂਰਨ n, ਪੂਰਨ m, ਪੂਰਨ k, ਪੂਰਨ min, ਪੂਰਨ max, ਪੂਰਨ ਮਾਪ, ਦਸ਼ਮਲਵ angle, ਦਸ਼ਮਲਵ aspect, ਦਸ਼ਮਲਵ hue, ਦਸ਼ਮਲਵ saturation, ਦਸ਼ਮਲਵ exposure)
{
    ਜੇ(m) paths = get_random_paths(paths, n, m);
    ਅੰਕੜੇ d = {0};
    d.w = ਮਾਪ;
    d.h = ਮਾਪ;
    d.shallow = 0;
    d.X = ਲੱਦੋ_image_augment_paths(paths, n, min, max, ਮਾਪ, angle, aspect, hue, saturation, exposure, 0);
    d.y = ਲੱਦੋ_ਟੈਗ_ਸੂਚੀ_paths(paths, n, k);
    ਜੇ(m) ਮੁਕਤ(paths);
    ਵਾਪਸ d;
}

ਮੈਟ੍ਰਿਕਸ concat_ਮੈਟ੍ਰਿਕਸ(ਮੈਟ੍ਰਿਕਸ m1, ਮੈਟ੍ਰਿਕਸ m2)
{
    ਪੂਰਨ i, ਗਿਣਤੀ_2 = 0;
    ਮੈਟ੍ਰਿਕਸ m;
    m.ਖਾਨੇ_ਗਿਣਤੀ = m1.ਖਾਨੇ_ਗਿਣਤੀ;
    m.ਪੰਕਤੀਆਂ = m1.ਪੰਕਤੀਆਂ+m2.ਪੰਕਤੀਆਂ;
    m.vals = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(m1.ਪੰਕਤੀਆਂ + m2.ਪੰਕਤੀਆਂ, ਆਕਾਰ(ਦਸ਼ਮਲਵ*));
    ਲਈ(i = 0; i < m1.ਪੰਕਤੀਆਂ; ++i){
        m.vals[ਗਿਣਤੀ_2++] = m1.vals[i];
    }
    ਲਈ(i = 0; i < m2.ਪੰਕਤੀਆਂ; ++i){
        m.vals[ਗਿਣਤੀ_2++] = m2.vals[i];
    }
    ਵਾਪਸ m;
}

ਅੰਕੜੇ concat_ਅੰਕੜੇ(ਅੰਕੜੇ d1, ਅੰਕੜੇ d2)
{
    ਅੰਕੜੇ d = {0};
    d.shallow = 1;
    d.X = concat_ਮੈਟ੍ਰਿਕਸ(d1.X, d2.X);
    d.y = concat_ਮੈਟ੍ਰਿਕਸ(d1.y, d2.y);
    d.w = d1.w;
    d.h = d1.h;
    ਵਾਪਸ d;
}

ਅੰਕੜੇ concat_datas(ਅੰਕੜੇ *d, ਪੂਰਨ n)
{
    ਪੂਰਨ i;
    ਅੰਕੜੇ ਬਾਹਰਲਾ = {0};
    ਲਈ(i = 0; i < n; ++i){
        ਅੰਕੜੇ new = concat_ਅੰਕੜੇ(d[i], ਬਾਹਰਲਾ);
        free_ਅੰਕੜੇ(ਬਾਹਰਲਾ);
        ਬਾਹਰਲਾ = new;
    }
    ਵਾਪਸ ਬਾਹਰਲਾ;
}

ਅੰਕੜੇ ਲੱਦੋ_categorical_ਅੰਕੜੇ_csv(ਅੱਖਰ *filename, ਪੂਰਨ ਨਿਸ਼ਾਨਾ, ਪੂਰਨ k)
{
    ਅੰਕੜੇ d = {0};
    d.shallow = 0;
    ਮੈਟ੍ਰਿਕਸ X = csv_ਮੰਜ਼ਿਲ_ਮੈਟ੍ਰਿਕਸ(filename);
    ਦਸ਼ਮਲਵ *truth_1d = ਕੱਢੋ_ਕਾਲਮ(&X, ਨਿਸ਼ਾਨਾ);
    ਦਸ਼ਮਲਵ **truth = one_hot_encode(truth_1d, X.ਪੰਕਤੀਆਂ, k);
    ਮੈਟ੍ਰਿਕਸ y;
    y.ਪੰਕਤੀਆਂ = X.ਪੰਕਤੀਆਂ;
    y.ਖਾਨੇ_ਗਿਣਤੀ = k;
    y.vals = truth;
    d.X = X;
    d.y = y;
    ਮੁਕਤ(truth_1d);
    ਵਾਪਸ d;
}

ਅੰਕੜੇ ਲੱਦੋ_cifar10_ਅੰਕੜੇ(ਅੱਖਰ *filename)
{
    ਅੰਕੜੇ d = {0};
    d.shallow = 0;
    ਲੰਮਾ i,j;
    ਮੈਟ੍ਰਿਕਸ X = make_ਮੈਟ੍ਰਿਕਸ(10000, 3072);
    ਮੈਟ੍ਰਿਕਸ y = make_ਮੈਟ੍ਰਿਕਸ(10000, 10);
    d.X = X;
    d.y = y;

    FILE *fp = fopen(filename, "rb");
    ਜੇ(!fp) file_error(filename);
    ਲਈ(i = 0; i < 10000; ++i){
        ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ ਬਾਈਟਾਂ[3073];
        fread(ਬਾਈਟਾਂ, 1, 3073, fp);
        ਪੂਰਨ class = ਬਾਈਟਾਂ[0];
        y.vals[i][class] = 1;
        ਲਈ(j = 0; j < X.ਖਾਨੇ_ਗਿਣਤੀ; ++j){
            X.vals[i][j] = (ਦੁੱਗਣਾ)ਬਾਈਟਾਂ[j+1];
        }
    }
    scale_ਅੰਕੜੇ_ਪੰਕਤੀਆਂ(d, 1./255);
    //normalize_data_rows(d);
    fclose(fp);
    ਵਾਪਸ d;
}

ਖਾਲੀ get_random_batch(ਅੰਕੜੇ d, ਪੂਰਨ n, ਦਸ਼ਮਲਵ *X, ਦਸ਼ਮਲਵ *y)
{
    ਪੂਰਨ j;
    ਲਈ(j = 0; j < n; ++j){
        ਪੂਰਨ ਸੂਚਕਾਂਕ = rand()%d.X.ਪੰਕਤੀਆਂ;
        ਸਮ੍ਰਿਤੀ_ਨਕਲ(X+j*d.X.ਖਾਨੇ_ਗਿਣਤੀ, d.X.vals[ਸੂਚਕਾਂਕ], d.X.ਖਾਨੇ_ਗਿਣਤੀ*ਆਕਾਰ(ਦਸ਼ਮਲਵ));
        ਸਮ੍ਰਿਤੀ_ਨਕਲ(y+j*d.y.ਖਾਨੇ_ਗਿਣਤੀ, d.y.vals[ਸੂਚਕਾਂਕ], d.y.ਖਾਨੇ_ਗਿਣਤੀ*ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    }
}

ਖਾਲੀ get_ਅਗਲਾ_batch(ਅੰਕੜੇ d, ਪੂਰਨ n, ਪੂਰਨ ਵਿੱਥ, ਦਸ਼ਮਲਵ *X, ਦਸ਼ਮਲਵ *y)
{
    ਪੂਰਨ j;
    ਲਈ(j = 0; j < n; ++j){
        ਪੂਰਨ ਸੂਚਕਾਂਕ = ਵਿੱਥ + j;
        ਸਮ੍ਰਿਤੀ_ਨਕਲ(X+j*d.X.ਖਾਨੇ_ਗਿਣਤੀ, d.X.vals[ਸੂਚਕਾਂਕ], d.X.ਖਾਨੇ_ਗਿਣਤੀ*ਆਕਾਰ(ਦਸ਼ਮਲਵ));
        ਜੇ(y) ਸਮ੍ਰਿਤੀ_ਨਕਲ(y+j*d.y.ਖਾਨੇ_ਗਿਣਤੀ, d.y.vals[ਸੂਚਕਾਂਕ], d.y.ਖਾਨੇ_ਗਿਣਤੀ*ਆਕਾਰ(ਦਸ਼ਮਲਵ));
    }
}

ਖਾਲੀ smooth_ਅੰਕੜੇ(ਅੰਕੜੇ d)
{
    ਪੂਰਨ i, j;
    ਦਸ਼ਮਲਵ scale = 1. / d.y.ਖਾਨੇ_ਗਿਣਤੀ;
    ਦਸ਼ਮਲਵ eps = .1;
    ਲਈ(i = 0; i < d.y.ਪੰਕਤੀਆਂ; ++i){
        ਲਈ(j = 0; j < d.y.ਖਾਨੇ_ਗਿਣਤੀ; ++j){
            d.y.vals[i][j] = eps * scale + (1-eps) * d.y.vals[i][j];
        }
    }
}

ਅੰਕੜੇ ਲੱਦੋ_all_cifar10()
{
    ਅੰਕੜੇ d = {0};
    d.shallow = 0;
    ਪੂਰਨ i,j,b;
    ਮੈਟ੍ਰਿਕਸ X = make_ਮੈਟ੍ਰਿਕਸ(50000, 3072);
    ਮੈਟ੍ਰਿਕਸ y = make_ਮੈਟ੍ਰਿਕਸ(50000, 10);
    d.X = X;
    d.y = y;


    ਲਈ(b = 0; b < 5; ++b){
        ਅੱਖਰ buff[256];
        sprintf(buff, "data/cifar/cifar-10-batches-bin/data_batch_%d.bin", b+1);
        FILE *fp = fopen(buff, "rb");
        ਜੇ(!fp) file_error(buff);
        ਲਈ(i = 0; i < 10000; ++i){
            ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ ਬਾਈਟਾਂ[3073];
            fread(ਬਾਈਟਾਂ, 1, 3073, fp);
            ਪੂਰਨ class = ਬਾਈਟਾਂ[0];
            y.vals[i+b*10000][class] = 1;
            ਲਈ(j = 0; j < X.ਖਾਨੇ_ਗਿਣਤੀ; ++j){
                X.vals[i+b*10000][j] = (ਦੁੱਗਣਾ)ਬਾਈਟਾਂ[j+1];
            }
        }
        fclose(fp);
    }
    //normalize_data_rows(d);
    scale_ਅੰਕੜੇ_ਪੰਕਤੀਆਂ(d, 1./255);
    smooth_ਅੰਕੜੇ(d);
    ਵਾਪਸ d;
}

ਅੰਕੜੇ ਲੱਦੋ_go(ਅੱਖਰ *filename)
{
    FILE *fp = fopen(filename, "rb");
    ਮੈਟ੍ਰਿਕਸ X = make_ਮੈਟ੍ਰਿਕਸ(3363059, 361);
    ਮੈਟ੍ਰਿਕਸ y = make_ਮੈਟ੍ਰਿਕਸ(3363059, 361);
    ਪੂਰਨ ਪੰਕਤੀ, ਖਾਨਾ;

    ਜੇ(!fp) file_error(filename);
    ਅੱਖਰ *label;
    ਪੂਰਨ ਗਿਣਤੀ_2 = 0;
    ਜਦੋਂ((label = fgetl(fp))){
        ਪੂਰਨ i;
        ਜੇ(ਗਿਣਤੀ_2 == X.ਪੰਕਤੀਆਂ){
            X = resize_ਮੈਟ੍ਰਿਕਸ(X, ਗਿਣਤੀ_2*2);
            y = resize_ਮੈਟ੍ਰਿਕਸ(y, ਗਿਣਤੀ_2*2);
        }
        ਲੜੀ_ਤੋਂ_ਪੜ੍ਹੋ(label, "%d %d", &ਪੰਕਤੀ, &ਖਾਨਾ);
        ਅੱਖਰ *board = fgetl(fp);

        ਪੂਰਨ ਸੂਚਕਾਂਕ = ਪੰਕਤੀ*19 + ਖਾਨਾ;
        y.vals[ਗਿਣਤੀ_2][ਸੂਚਕਾਂਕ] = 1;

        ਲਈ(i = 0; i < 19*19; ++i){
            ਦਸ਼ਮਲਵ val = 0;
            ਜੇ(board[i] == '1') val = 1;
            ਨਹੀਂਤਾਂ ਜੇ(board[i] == '2') val = -1;
            X.vals[ਗਿਣਤੀ_2][i] = val;
        }
        ++ਗਿਣਤੀ_2;
        ਮੁਕਤ(label);
        ਮੁਕਤ(board);
    }
    X = resize_ਮੈਟ੍ਰਿਕਸ(X, ਗਿਣਤੀ_2);
    y = resize_ਮੈਟ੍ਰਿਕਸ(y, ਗਿਣਤੀ_2);

    ਅੰਕੜੇ d = {0};
    d.shallow = 0;
    d.X = X;
    d.y = y;


    fclose(fp);

    ਵਾਪਸ d;
}


ਖਾਲੀ randomize_ਅੰਕੜੇ(ਅੰਕੜੇ d)
{
    ਪੂਰਨ i;
    ਲਈ(i = d.X.ਪੰਕਤੀਆਂ-1; i > 0; --i){
        ਪੂਰਨ ਸੂਚਕਾਂਕ = rand()%i;
        ਦਸ਼ਮਲਵ *ਅਦਲਾ_ਬਦਲੀ = d.X.vals[ਸੂਚਕਾਂਕ];
        d.X.vals[ਸੂਚਕਾਂਕ] = d.X.vals[i];
        d.X.vals[i] = ਅਦਲਾ_ਬਦਲੀ;

        ਅਦਲਾ_ਬਦਲੀ = d.y.vals[ਸੂਚਕਾਂਕ];
        d.y.vals[ਸੂਚਕਾਂਕ] = d.y.vals[i];
        d.y.vals[i] = ਅਦਲਾ_ਬਦਲੀ;
    }
}

ਖਾਲੀ scale_ਅੰਕੜੇ_ਪੰਕਤੀਆਂ(ਅੰਕੜੇ d, ਦਸ਼ਮਲਵ s)
{
    ਪੂਰਨ i;
    ਲਈ(i = 0; i < d.X.ਪੰਕਤੀਆਂ; ++i){
        scale_array(d.X.vals[i], d.X.ਖਾਨੇ_ਗਿਣਤੀ, s);
    }
}

ਖਾਲੀ translate_ਅੰਕੜੇ_ਪੰਕਤੀਆਂ(ਅੰਕੜੇ d, ਦਸ਼ਮਲਵ s)
{
    ਪੂਰਨ i;
    ਲਈ(i = 0; i < d.X.ਪੰਕਤੀਆਂ; ++i){
        translate_array(d.X.vals[i], d.X.ਖਾਨੇ_ਗਿਣਤੀ, s);
    }
}

ਅੰਕੜੇ copy_ਅੰਕੜੇ(ਅੰਕੜੇ d)
{
    ਅੰਕੜੇ c = {0};
    c.w = d.w;
    c.h = d.h;
    c.shallow = 0;
    c.ਅੰਕ_boxes = d.ਅੰਕ_boxes;
    c.boxes = d.boxes;
    c.X = copy_ਮੈਟ੍ਰਿਕਸ(d.X);
    c.y = copy_ਮੈਟ੍ਰਿਕਸ(d.y);
    ਵਾਪਸ c;
}

ਖਾਲੀ normalize_ਅੰਕੜੇ_ਪੰਕਤੀਆਂ(ਅੰਕੜੇ d)
{
    ਪੂਰਨ i;
    ਲਈ(i = 0; i < d.X.ਪੰਕਤੀਆਂ; ++i){
        normalize_array(d.X.vals[i], d.X.ਖਾਨੇ_ਗਿਣਤੀ);
    }
}

ਅੰਕੜੇ get_ਅੰਕੜੇ_part(ਅੰਕੜੇ d, ਪੂਰਨ part, ਪੂਰਨ ਕੁੱਲ)
{
    ਅੰਕੜੇ p = {0};
    p.shallow = 1;
    p.X.ਪੰਕਤੀਆਂ = d.X.ਪੰਕਤੀਆਂ * (part + 1) / ਕੁੱਲ - d.X.ਪੰਕਤੀਆਂ * part / ਕੁੱਲ;
    p.y.ਪੰਕਤੀਆਂ = d.y.ਪੰਕਤੀਆਂ * (part + 1) / ਕੁੱਲ - d.y.ਪੰਕਤੀਆਂ * part / ਕੁੱਲ;
    p.X.ਖਾਨੇ_ਗਿਣਤੀ = d.X.ਖਾਨੇ_ਗਿਣਤੀ;
    p.y.ਖਾਨੇ_ਗਿਣਤੀ = d.y.ਖਾਨੇ_ਗਿਣਤੀ;
    p.X.vals = d.X.vals + d.X.ਪੰਕਤੀਆਂ * part / ਕੁੱਲ;
    p.y.vals = d.y.vals + d.y.ਪੰਕਤੀਆਂ * part / ਕੁੱਲ;
    ਵਾਪਸ p;
}

ਅੰਕੜੇ get_random_ਅੰਕੜੇ(ਅੰਕੜੇ d, ਪੂਰਨ ਅੰਕ)
{
    ਅੰਕੜੇ r = {0};
    r.shallow = 1;

    r.X.ਪੰਕਤੀਆਂ = ਅੰਕ;
    r.y.ਪੰਕਤੀਆਂ = ਅੰਕ;

    r.X.ਖਾਨੇ_ਗਿਣਤੀ = d.X.ਖਾਨੇ_ਗਿਣਤੀ;
    r.y.ਖਾਨੇ_ਗਿਣਤੀ = d.y.ਖਾਨੇ_ਗਿਣਤੀ;

    r.X.vals = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਅੰਕ, ਆਕਾਰ(ਦਸ਼ਮਲਵ *));
    r.y.vals = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(ਅੰਕ, ਆਕਾਰ(ਦਸ਼ਮਲਵ *));

    ਪੂਰਨ i;
    ਲਈ(i = 0; i < ਅੰਕ; ++i){
        ਪੂਰਨ ਸੂਚਕਾਂਕ = rand()%d.X.ਪੰਕਤੀਆਂ;
        r.X.vals[i] = d.X.vals[ਸੂਚਕਾਂਕ];
        r.y.vals[i] = d.y.vals[ਸੂਚਕਾਂਕ];
    }
    ਵਾਪਸ r;
}

ਅੰਕੜੇ *ਵੰਡੋ_ਅੰਕੜੇ(ਅੰਕੜੇ d, ਪੂਰਨ part, ਪੂਰਨ ਕੁੱਲ)
{
    ਅੰਕੜੇ *ਵੰਡੋ = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(2, ਆਕਾਰ(ਅੰਕੜੇ));
    ਪੂਰਨ i;
    ਪੂਰਨ ਸ਼ੁਰੂ = part*d.X.ਪੰਕਤੀਆਂ/ਕੁੱਲ;
    ਪੂਰਨ ਸਿਰਾ = (part+1)*d.X.ਪੰਕਤੀਆਂ/ਕੁੱਲ;
    ਅੰਕੜੇ train;
    ਅੰਕੜੇ test;
    train.shallow = test.shallow = 1;

    test.X.ਪੰਕਤੀਆਂ = test.y.ਪੰਕਤੀਆਂ = ਸਿਰਾ-ਸ਼ੁਰੂ;
    train.X.ਪੰਕਤੀਆਂ = train.y.ਪੰਕਤੀਆਂ = d.X.ਪੰਕਤੀਆਂ - (ਸਿਰਾ-ਸ਼ੁਰੂ);
    train.X.ਖਾਨੇ_ਗਿਣਤੀ = test.X.ਖਾਨੇ_ਗਿਣਤੀ = d.X.ਖਾਨੇ_ਗਿਣਤੀ;
    train.y.ਖਾਨੇ_ਗਿਣਤੀ = test.y.ਖਾਨੇ_ਗਿਣਤੀ = d.y.ਖਾਨੇ_ਗਿਣਤੀ;

    train.X.vals = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(train.X.ਪੰਕਤੀਆਂ, ਆਕਾਰ(ਦਸ਼ਮਲਵ*));
    test.X.vals = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(test.X.ਪੰਕਤੀਆਂ, ਆਕਾਰ(ਦਸ਼ਮਲਵ*));
    train.y.vals = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(train.y.ਪੰਕਤੀਆਂ, ਆਕਾਰ(ਦਸ਼ਮਲਵ*));
    test.y.vals = ਸਿਫਰ_ਸਮ੍ਰਿਤੀ_ਦਿਓ(test.y.ਪੰਕਤੀਆਂ, ਆਕਾਰ(ਦਸ਼ਮਲਵ*));

    ਲਈ(i = 0; i < ਸ਼ੁਰੂ; ++i){
        train.X.vals[i] = d.X.vals[i];
        train.y.vals[i] = d.y.vals[i];
    }
    ਲਈ(i = ਸ਼ੁਰੂ; i < ਸਿਰਾ; ++i){
        test.X.vals[i-ਸ਼ੁਰੂ] = d.X.vals[i];
        test.y.vals[i-ਸ਼ੁਰੂ] = d.y.vals[i];
    }
    ਲਈ(i = ਸਿਰਾ; i < d.X.ਪੰਕਤੀਆਂ; ++i){
        train.X.vals[i-(ਸਿਰਾ-ਸ਼ੁਰੂ)] = d.X.vals[i];
        train.y.vals[i-(ਸਿਰਾ-ਸ਼ੁਰੂ)] = d.y.vals[i];
    }
    ਵੰਡੋ[0] = train;
    ਵੰਡੋ[1] = test;
    ਵਾਪਸ ਵੰਡੋ;
}

