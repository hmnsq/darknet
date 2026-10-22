#ಅನ್ತರ್ಭಾವ "data.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "image.h"
#ಅನ್ತರ್ಭಾವ "cuda.h"

#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <string.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

list *get_paths(ಅಕ್ಷರ *filename)
{
    ಅಕ್ಷರ *ಮಾರ್ಗ;
    FILE *file = fopen(filename, "r");
    ಆದರೆ(!file) file_error(filename);
    list *ಸಾಲುಗಳು = make_list();
    ಆಗ((ಮಾರ್ಗ=fgetl(file))){
        list_ಸೇರಿಸು(ಸಾಲುಗಳು, ಮಾರ್ಗ);
    }
    fclose(file);
    ಹಿಂತಿರುಗಿ ಸಾಲುಗಳು;
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

ಅಕ್ಷರ **get_random_paths(ಅಕ್ಷರ **paths, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ m)
{
    ಅಕ್ಷರ **random_paths = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(n, ಗಾತ್ರ(ಅಕ್ಷರ*));
    ಪೂರ್ಣಾಂಕ i;
    pthread_mutex_lock(&mutex);
    ಗಾಗಿ(i = 0; i < n; ++i){
        ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ = rand()%m;
        random_paths[i] = paths[ಸೂಚ್ಯಂಕ];
        //if(i == 0) printf("%s\n", paths[index]);
    }
    pthread_mutex_unlock(&mutex);
    ಹಿಂತಿರುಗಿ random_paths;
}

ಅಕ್ಷರ **ಕಂಡುಹಿಡಿ_replace_paths(ಅಕ್ಷರ **paths, ಪೂರ್ಣಾಂಕ n, ಅಕ್ಷರ *ಕಂಡುಹಿಡಿ, ಅಕ್ಷರ *replace)
{
    ಅಕ್ಷರ **replace_paths = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(n, ಗಾತ್ರ(ಅಕ್ಷರ*));
    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ(i = 0; i < n; ++i){
        ಅಕ್ಷರ replaced[4096];
        ಕಂಡುಹಿಡಿ_replace(paths[i], ಕಂಡುಹಿಡಿ, replace, replaced);
        replace_paths[i] = copy_string(replaced);
    }
    ಹಿಂತಿರುಗಿ replace_paths;
}

ಮಾತೃಕೆ ತುಂಬು_image_paths_ಬೂದು(ಅಕ್ಷರ **paths, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h)
{
    ಪೂರ್ಣಾಂಕ i;
    ಮಾತೃಕೆ X;
    X.ಅಡ್ಡಸಾಲುಗಳು = n;
    X.vals = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(X.ಅಡ್ಡಸಾಲುಗಳು, ಗಾತ್ರ(ದಶಮಾಂಶ*));
    X.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = 0;

    ಗಾಗಿ(i = 0; i < n; ++i){
        image im = ತುಂಬು_image(paths[i], w, h, 3);

        image ಬೂದು = grayscale_image(im);
        free_image(im);
        im = ಬೂದು;

        X.vals[i] = im.ದತ್ತಾಂಶ;
        X.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = im.h*im.w*im.c;
    }
    ಹಿಂತಿರುಗಿ X;
}

ಮಾತೃಕೆ ತುಂಬು_image_paths(ಅಕ್ಷರ **paths, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h)
{
    ಪೂರ್ಣಾಂಕ i;
    ಮಾತೃಕೆ X;
    X.ಅಡ್ಡಸಾಲುಗಳು = n;
    X.vals = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(X.ಅಡ್ಡಸಾಲುಗಳು, ಗಾತ್ರ(ದಶಮಾಂಶ*));
    X.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = 0;

    ಗಾಗಿ(i = 0; i < n; ++i){
        image im = ತುಂಬು_image_color(paths[i], w, h);
        X.vals[i] = im.ದತ್ತಾಂಶ;
        X.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = im.h*im.w*im.c;
    }
    ಹಿಂತಿರುಗಿ X;
}

ಮಾತೃಕೆ ತುಂಬು_image_augment_paths(ಅಕ್ಷರ **paths, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ min, ಪೂರ್ಣಾಂಕ max, ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2, ದಶಮಾಂಶ angle, ದಶಮಾಂಶ aspect, ದಶಮಾಂಶ hue, ದಶಮಾಂಶ saturation, ದಶಮಾಂಶ exposure, ಪೂರ್ಣಾಂಕ center)
{
    ಪೂರ್ಣಾಂಕ i;
    ಮಾತೃಕೆ X;
    X.ಅಡ್ಡಸಾಲುಗಳು = n;
    X.vals = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(X.ಅಡ್ಡಸಾಲುಗಳು, ಗಾತ್ರ(ದಶಮಾಂಶ*));
    X.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = 0;

    ಗಾಗಿ(i = 0; i < n; ++i){
        image im = ತುಂಬು_image_color(paths[i], 0, 0);
        image crop;
        ಆದರೆ(center){
            crop = center_crop_image(im, ಗಾತ್ರ_2, ಗಾತ್ರ_2);
        } ಇಲ್ಲದಿದ್ದರೆ {
            crop = random_augment_image(im, angle, aspect, min, max, ಗಾತ್ರ_2, ಗಾತ್ರ_2);
        }
        ಪೂರ್ಣಾಂಕ flip = rand()%2;
        ಆದರೆ (flip) flip_image(crop);
        random_distort_image(crop, hue, saturation, exposure);

        /*
        show_image(im, "orig");
        show_image(crop, "crop");
        cvWaitKey(0);
        */
        //grayscale_image_3c(crop);
        free_image(im);
        X.vals[i] = crop.ದತ್ತಾಂಶ;
        X.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = crop.h*crop.w*crop.c;
    }
    ಹಿಂತಿರುಗಿ X;
}


box_label *read_boxes(ಅಕ್ಷರ *filename, ಪೂರ್ಣಾಂಕ *n)
{
    FILE *file = fopen(filename, "r");
    ಆದರೆ(!file) file_error(filename);
    ದಶಮಾಂಶ x, y, h, w;
    ಪೂರ್ಣಾಂಕ id;
    ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2 = 0;
    ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2 = 64;
    box_label *boxes = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(ಗಾತ್ರ_2, ಗಾತ್ರ(box_label));
    ಆಗ(fscanf(file, "%d %f %f %f %f", &id, &x, &y, &w, &h) == 5){
        ಆದರೆ(ಎಣಿಕೆ_2 == ಗಾತ್ರ_2) {
            ಗಾತ್ರ_2 = ಗಾತ್ರ_2 * 2;
            boxes = realloc(boxes, ಗಾತ್ರ_2*ಗಾತ್ರ(box_label));
        }
        boxes[ಎಣಿಕೆ_2].id = id;
        boxes[ಎಣಿಕೆ_2].x = x;
        boxes[ಎಣಿಕೆ_2].y = y;
        boxes[ಎಣಿಕೆ_2].h = h;
        boxes[ಎಣಿಕೆ_2].w = w;
        boxes[ಎಣಿಕೆ_2].ಎಡ   = x - w/2;
        boxes[ಎಣಿಕೆ_2].ಬಲ  = x + w/2;
        boxes[ಎಣಿಕೆ_2].ತುದಿ    = y - h/2;
        boxes[ಎಣಿಕೆ_2].bottom = y + h/2;
        ++ಎಣಿಕೆ_2;
    }
    fclose(file);
    *n = ಎಣಿಕೆ_2;
    ಹಿಂತಿರುಗಿ boxes;
}

ಶೂನ್ಯ randomize_boxes(box_label *b, ಪೂರ್ಣಾಂಕ n)
{
    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ(i = 0; i < n; ++i){
        box_label ಅದಲುಬದಲು = b[i];
        ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ = rand()%n;
        b[i] = b[ಸೂಚ್ಯಂಕ];
        b[ಸೂಚ್ಯಂಕ] = ಅದಲುಬದಲು;
    }
}

ಶೂನ್ಯ correct_boxes(box_label *boxes, ಪೂರ್ಣಾಂಕ n, ದಶಮಾಂಶ dx, ದಶಮಾಂಶ dy, ದಶಮಾಂಶ sx, ದಶಮಾಂಶ sy, ಪೂರ್ಣಾಂಕ flip)
{
    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ(i = 0; i < n; ++i){
        ಆದರೆ(boxes[i].x == 0 && boxes[i].y == 0) {
            boxes[i].x = 999999;
            boxes[i].y = 999999;
            boxes[i].w = 999999;
            boxes[i].h = 999999;
            ಮುಂದುವರಿ;
        }
        boxes[i].ಎಡ   = boxes[i].ಎಡ  * sx - dx;
        boxes[i].ಬಲ  = boxes[i].ಬಲ * sx - dx;
        boxes[i].ತುದಿ    = boxes[i].ತುದಿ   * sy - dy;
        boxes[i].bottom = boxes[i].bottom* sy - dy;

        ಆದರೆ(flip){
            ದಶಮಾಂಶ ಅದಲುಬದಲು = boxes[i].ಎಡ;
            boxes[i].ಎಡ = 1. - boxes[i].ಬಲ;
            boxes[i].ಬಲ = 1. - ಅದಲುಬದಲು;
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

ಶೂನ್ಯ fill_truth_swag(ಅಕ್ಷರ *ಮಾರ್ಗ, ದಶಮಾಂಶ *truth, ಪೂರ್ಣಾಂಕ classes, ಪೂರ್ಣಾಂಕ flip, ದಶಮಾಂಶ dx, ದಶಮಾಂಶ dy, ದಶಮಾಂಶ sx, ದಶಮಾಂಶ sy)
{
    ಅಕ್ಷರ labelpath[4096];
    ಕಂಡುಹಿಡಿ_replace(ಮಾರ್ಗ, "images", "labels", labelpath);
    ಕಂಡುಹಿಡಿ_replace(labelpath, "JPEGImages", "labels", labelpath);
    ಕಂಡುಹಿಡಿ_replace(labelpath, ".jpg", ".txt", labelpath);
    ಕಂಡುಹಿಡಿ_replace(labelpath, ".JPG", ".txt", labelpath);
    ಕಂಡುಹಿಡಿ_replace(labelpath, ".JPEG", ".txt", labelpath);

    ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2 = 0;
    box_label *boxes = read_boxes(labelpath, &ಎಣಿಕೆ_2);
    randomize_boxes(boxes, ಎಣಿಕೆ_2);
    correct_boxes(boxes, ಎಣಿಕೆ_2, dx, dy, sx, sy, flip);
    ದಶಮಾಂಶ x,y,w,h;
    ಪೂರ್ಣಾಂಕ id;
    ಪೂರ್ಣಾಂಕ i;

    ಗಾಗಿ (i = 0; i < ಎಣಿಕೆ_2 && i < 90; ++i) {
        x =  boxes[i].x;
        y =  boxes[i].y;
        w =  boxes[i].w;
        h =  boxes[i].h;
        id = boxes[i].id;

        ಆದರೆ (w < .0 || h < .0) ಮುಂದುವರಿ;

        ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ = (4+classes) * i;

        truth[ಸೂಚ್ಯಂಕ++] = x;
        truth[ಸೂಚ್ಯಂಕ++] = y;
        truth[ಸೂಚ್ಯಂಕ++] = w;
        truth[ಸೂಚ್ಯಂಕ++] = h;

        ಆದರೆ (id < classes) truth[ಸೂಚ್ಯಂಕ+id] = 1;
    }
    ಬಿಡುಗಡೆ(boxes);
}

ಶೂನ್ಯ fill_truth_region(ಅಕ್ಷರ *ಮಾರ್ಗ, ದಶಮಾಂಶ *truth, ಪೂರ್ಣಾಂಕ classes, ಪೂರ್ಣಾಂಕ ಅಂಕೆ_boxes, ಪೂರ್ಣಾಂಕ flip, ದಶಮಾಂಶ dx, ದಶಮಾಂಶ dy, ದಶಮಾಂಶ sx, ದಶಮಾಂಶ sy)
{
    ಅಕ್ಷರ labelpath[4096];
    ಕಂಡುಹಿಡಿ_replace(ಮಾರ್ಗ, "images", "labels", labelpath);
    ಕಂಡುಹಿಡಿ_replace(labelpath, "JPEGImages", "labels", labelpath);

    ಕಂಡುಹಿಡಿ_replace(labelpath, ".jpg", ".txt", labelpath);
    ಕಂಡುಹಿಡಿ_replace(labelpath, ".png", ".txt", labelpath);
    ಕಂಡುಹಿಡಿ_replace(labelpath, ".JPG", ".txt", labelpath);
    ಕಂಡುಹಿಡಿ_replace(labelpath, ".JPEG", ".txt", labelpath);
    ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2 = 0;
    box_label *boxes = read_boxes(labelpath, &ಎಣಿಕೆ_2);
    randomize_boxes(boxes, ಎಣಿಕೆ_2);
    correct_boxes(boxes, ಎಣಿಕೆ_2, dx, dy, sx, sy, flip);
    ದಶಮಾಂಶ x,y,w,h;
    ಪೂರ್ಣಾಂಕ id;
    ಪೂರ್ಣಾಂಕ i;

    ಗಾಗಿ (i = 0; i < ಎಣಿಕೆ_2; ++i) {
        x =  boxes[i].x;
        y =  boxes[i].y;
        w =  boxes[i].w;
        h =  boxes[i].h;
        id = boxes[i].id;

        ಆದರೆ (w < .005 || h < .005) ಮುಂದುವರಿ;

        ಪೂರ್ಣಾಂಕ ಲಂಬಸಾಲು = (ಪೂರ್ಣಾಂಕ)(x*ಅಂಕೆ_boxes);
        ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲು = (ಪೂರ್ಣಾಂಕ)(y*ಅಂಕೆ_boxes);

        x = x*ಅಂಕೆ_boxes - ಲಂಬಸಾಲು;
        y = y*ಅಂಕೆ_boxes - ಅಡ್ಡಸಾಲು;

        ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ = (ಲಂಬಸಾಲು+ಅಡ್ಡಸಾಲು*ಅಂಕೆ_boxes)*(5+classes);
        ಆದರೆ (truth[ಸೂಚ್ಯಂಕ]) ಮುಂದುವರಿ;
        truth[ಸೂಚ್ಯಂಕ++] = 1;

        ಆದರೆ (id < classes) truth[ಸೂಚ್ಯಂಕ+id] = 1;
        ಸೂಚ್ಯಂಕ += classes;

        truth[ಸೂಚ್ಯಂಕ++] = x;
        truth[ಸೂಚ್ಯಂಕ++] = y;
        truth[ಸೂಚ್ಯಂಕ++] = w;
        truth[ಸೂಚ್ಯಂಕ++] = h;
    }
    ಬಿಡುಗಡೆ(boxes);
}

ಶೂನ್ಯ ತುಂಬು_rle(image im, ಪೂರ್ಣಾಂಕ *rle, ಪೂರ್ಣಾಂಕ n)
{
    ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2 = 0;
    ಪೂರ್ಣಾಂಕ curr = 0;
    ಪೂರ್ಣಾಂಕ i,j;
    ಗಾಗಿ(i = 0; i < n; ++i){
        ಗಾಗಿ(j = 0; j < rle[i]; ++j){
            im.ದತ್ತಾಂಶ[ಎಣಿಕೆ_2++] = curr;
        }
        curr = 1 - curr;
    }
    ಗಾಗಿ(; ಎಣಿಕೆ_2 < im.h*im.w*im.c; ++ಎಣಿಕೆ_2){
        im.ದತ್ತಾಂಶ[ಎಣಿಕೆ_2] = curr;
    }
}

ಶೂನ್ಯ or_image(image src, image dest, ಪೂರ್ಣಾಂಕ c)
{
    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ(i = 0; i < src.w*src.h; ++i){
        ಆದರೆ(src.ದತ್ತಾಂಶ[i]) dest.ದತ್ತಾಂಶ[dest.w*dest.h*c + i] = 1;
    }
}

ಶೂನ್ಯ exclusive_image(image src)
{
    ಪೂರ್ಣಾಂಕ k, j, i;
    ಪೂರ್ಣಾಂಕ s = src.w*src.h;
    ಗಾಗಿ(k = 0; k < src.c-1; ++k){
        ಗಾಗಿ(i = 0; i < s; ++i){
            ಆದರೆ (src.ದತ್ತಾಂಶ[k*s + i]){
                ಗಾಗಿ(j = k+1; j < src.c; ++j){
                    src.ದತ್ತಾಂಶ[j*s + i] = 0;
                }
            }
        }
    }
}

box bound_image(image im)
{
    ಪೂರ್ಣಾಂಕ x,y;
    ಪೂರ್ಣಾಂಕ minx = im.w;
    ಪೂರ್ಣಾಂಕ miny = im.h;
    ಪೂರ್ಣಾಂಕ maxx = 0;
    ಪೂರ್ಣಾಂಕ maxy = 0;
    ಗಾಗಿ(y = 0; y < im.h; ++y){
        ಗಾಗಿ(x = 0; x < im.w; ++x){
            ಆದರೆ(im.ದತ್ತಾಂಶ[y*im.w + x]){
                minx = (x < minx) ? x : minx;
                miny = (y < miny) ? y : miny;
                maxx = (x > maxx) ? x : maxx;
                maxy = (y > maxy) ? y : maxy;
            }
        }
    }
    box b = {minx, miny, maxx-minx + 1, maxy-miny + 1};
    //printf("%f %f %f %f\n", b.x, b.y, b.w, b.h);
    ಹಿಂತಿರುಗಿ b;
}

ಶೂನ್ಯ fill_truth_iseg(ಅಕ್ಷರ *ಮಾರ್ಗ, ಪೂರ್ಣಾಂಕ ಅಂಕೆ_boxes, ದಶಮಾಂಶ *truth, ಪೂರ್ಣಾಂಕ classes, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h, augment_ವಾದಗಳು aug, ಪೂರ್ಣಾಂಕ flip, ಪೂರ್ಣಾಂಕ mw, ಪೂರ್ಣಾಂಕ mh)
{
    ಅಕ್ಷರ labelpath[4096];
    ಕಂಡುಹಿಡಿ_replace(ಮಾರ್ಗ, "images", "mask", labelpath);
    ಕಂಡುಹಿಡಿ_replace(labelpath, "JPEGImages", "mask", labelpath);
    ಕಂಡುಹಿಡಿ_replace(labelpath, ".jpg", ".txt", labelpath);
    ಕಂಡುಹಿಡಿ_replace(labelpath, ".JPG", ".txt", labelpath);
    ಕಂಡುಹಿಡಿ_replace(labelpath, ".JPEG", ".txt", labelpath);
    FILE *file = fopen(labelpath, "r");
    ಆದರೆ(!file) file_error(labelpath);
    ಅಕ್ಷರ buff[32788];
    ಪೂರ್ಣಾಂಕ id;
    ಪೂರ್ಣಾಂಕ i = 0;
    ಪೂರ್ಣಾಂಕ j;
    image part = make_image(w, h, 1);
    ಆಗ((fscanf(file, "%d %s", &id, buff) == 2) && i < ಅಂಕೆ_boxes){
        ಪೂರ್ಣಾಂಕ n = 0;
        ಪೂರ್ಣಾಂಕ *rle = read_intlist(buff, &n, 0);
        ತುಂಬು_rle(part, rle, n);
        image sized = rotate_crop_image(part, aug.rad, aug.scale, aug.w, aug.h, aug.dx, aug.dy, aug.aspect);
        ಆದರೆ(flip) flip_image(sized);

        image ಮುಖವಾಡ = resize_image(sized, mw, mh);
        truth[i*(mw*mh+1)] = id;
        ಗಾಗಿ(j = 0; j < mw*mh; ++j){
            truth[i*(mw*mh + 1) + 1 + j] = ಮುಖವಾಡ.ದತ್ತಾಂಶ[j];
        }
        ++i;

        free_image(ಮುಖವಾಡ);
        free_image(sized);
        ಬಿಡುಗಡೆ(rle);
    }
    ಆದರೆ(i < ಅಂಕೆ_boxes) truth[i*(mw*mh+1)] = -1;
    fclose(file);
    free_image(part);
}

ಶೂನ್ಯ fill_truth_ಮುಖವಾಡ(ಅಕ್ಷರ *ಮಾರ್ಗ, ಪೂರ್ಣಾಂಕ ಅಂಕೆ_boxes, ದಶಮಾಂಶ *truth, ಪೂರ್ಣಾಂಕ classes, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h, augment_ವಾದಗಳು aug, ಪೂರ್ಣಾಂಕ flip, ಪೂರ್ಣಾಂಕ mw, ಪೂರ್ಣಾಂಕ mh)
{
    ಅಕ್ಷರ labelpath[4096];
    ಕಂಡುಹಿಡಿ_replace(ಮಾರ್ಗ, "images", "mask", labelpath);
    ಕಂಡುಹಿಡಿ_replace(labelpath, "JPEGImages", "mask", labelpath);
    ಕಂಡುಹಿಡಿ_replace(labelpath, ".jpg", ".txt", labelpath);
    ಕಂಡುಹಿಡಿ_replace(labelpath, ".JPG", ".txt", labelpath);
    ಕಂಡುಹಿಡಿ_replace(labelpath, ".JPEG", ".txt", labelpath);
    FILE *file = fopen(labelpath, "r");
    ಆದರೆ(!file) file_error(labelpath);
    ಅಕ್ಷರ buff[32788];
    ಪೂರ್ಣಾಂಕ id;
    ಪೂರ್ಣಾಂಕ i = 0;
    image part = make_image(w, h, 1);
    ಆಗ((fscanf(file, "%d %s", &id, buff) == 2) && i < ಅಂಕೆ_boxes){
        ಪೂರ್ಣಾಂಕ n = 0;
        ಪೂರ್ಣಾಂಕ *rle = read_intlist(buff, &n, 0);
        ತುಂಬು_rle(part, rle, n);
        image sized = rotate_crop_image(part, aug.rad, aug.scale, aug.w, aug.h, aug.dx, aug.dy, aug.aspect);
        ಆದರೆ(flip) flip_image(sized);
        box b = bound_image(sized);
        ಆದರೆ(b.w > 0){
            image crop = crop_image(sized, b.x, b.y, b.w, b.h);
            image ಮುಖವಾಡ = resize_image(crop, mw, mh);
            truth[i*(4 + mw*mh + 1) + 0] = (b.x + b.w/2.)/sized.w;
            truth[i*(4 + mw*mh + 1) + 1] = (b.y + b.h/2.)/sized.h;
            truth[i*(4 + mw*mh + 1) + 2] = b.w/sized.w;
            truth[i*(4 + mw*mh + 1) + 3] = b.h/sized.h;
            ಪೂರ್ಣಾಂಕ j;
            ಗಾಗಿ(j = 0; j < mw*mh; ++j){
                truth[i*(4 + mw*mh + 1) + 4 + j] = ಮುಖವಾಡ.ದತ್ತಾಂಶ[j];
            }
            truth[i*(4 + mw*mh + 1) + 4 + mw*mh] = id;
            free_image(crop);
            free_image(ಮುಖವಾಡ);
            ++i;
        }
        free_image(sized);
        ಬಿಡುಗಡೆ(rle);
    }
    fclose(file);
    free_image(part);
}


ಶೂನ್ಯ fill_truth_detection(ಅಕ್ಷರ *ಮಾರ್ಗ, ಪೂರ್ಣಾಂಕ ಅಂಕೆ_boxes, ದಶಮಾಂಶ *truth, ಪೂರ್ಣಾಂಕ classes, ಪೂರ್ಣಾಂಕ flip, ದಶಮಾಂಶ dx, ದಶಮಾಂಶ dy, ದಶಮಾಂಶ sx, ದಶಮಾಂಶ sy)
{
    ಅಕ್ಷರ labelpath[4096];
    ಕಂಡುಹಿಡಿ_replace(ಮಾರ್ಗ, "images", "labels", labelpath);
    ಕಂಡುಹಿಡಿ_replace(labelpath, "JPEGImages", "labels", labelpath);

    ಕಂಡುಹಿಡಿ_replace(labelpath, "raw", "labels", labelpath);
    ಕಂಡುಹಿಡಿ_replace(labelpath, ".jpg", ".txt", labelpath);
    ಕಂಡುಹಿಡಿ_replace(labelpath, ".png", ".txt", labelpath);
    ಕಂಡುಹಿಡಿ_replace(labelpath, ".JPG", ".txt", labelpath);
    ಕಂಡುಹಿಡಿ_replace(labelpath, ".JPEG", ".txt", labelpath);
    ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2 = 0;
    box_label *boxes = read_boxes(labelpath, &ಎಣಿಕೆ_2);
    randomize_boxes(boxes, ಎಣಿಕೆ_2);
    correct_boxes(boxes, ಎಣಿಕೆ_2, dx, dy, sx, sy, flip);
    ಆದರೆ(ಎಣಿಕೆ_2 > ಅಂಕೆ_boxes) ಎಣಿಕೆ_2 = ಅಂಕೆ_boxes;
    ದಶಮಾಂಶ x,y,w,h;
    ಪೂರ್ಣಾಂಕ id;
    ಪೂರ್ಣಾಂಕ i;
    ಪೂರ್ಣಾಂಕ sub = 0;

    ಗಾಗಿ (i = 0; i < ಎಣಿಕೆ_2; ++i) {
        x =  boxes[i].x;
        y =  boxes[i].y;
        w =  boxes[i].w;
        h =  boxes[i].h;
        id = boxes[i].id;

        ಆದರೆ ((w < .001 || h < .001)) {
            ++sub;
            ಮುಂದುವರಿ;
        }

        truth[(i-sub)*5+0] = x;
        truth[(i-sub)*5+1] = y;
        truth[(i-sub)*5+2] = w;
        truth[(i-sub)*5+3] = h;
        truth[(i-sub)*5+4] = id;
    }
    ಬಿಡುಗಡೆ(boxes);
}

#ವ್ಯಾಖ್ಯಾನಿಸು NUMCHARS 37

ಶೂನ್ಯ print_letters(ದಶಮಾಂಶ *pred, ಪೂರ್ಣಾಂಕ n)
{
    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ(i = 0; i < n; ++i){
        ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ = max_ಸೂಚ್ಯಂಕ(pred+i*NUMCHARS, NUMCHARS);
        ಬರೆ("%c", int_ಗಮ್ಯ_alphanum(ಸೂಚ್ಯಂಕ));
    }
    ಬರೆ("\n");
}

ಶೂನ್ಯ fill_truth_captcha(ಅಕ್ಷರ *ಮಾರ್ಗ, ಪೂರ್ಣಾಂಕ n, ದಶಮಾಂಶ *truth)
{
    ಅಕ್ಷರ *begin = strrchr(ಮಾರ್ಗ, '/');
    ++begin;
    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ(i = 0; i < ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(begin) && i < n && begin[i] != '.'; ++i){
        ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ = alphanum_ಗಮ್ಯ_int(begin[i]);
        ಆದರೆ(ಸೂಚ್ಯಂಕ > 35) ಬರೆ("Bad %c\n", begin[i]);
        truth[i*NUMCHARS+ಸೂಚ್ಯಂಕ] = 1;
    }
    ಗಾಗಿ(;i < n; ++i){
        truth[i*NUMCHARS + NUMCHARS-1] = 1;
    }
}

ದತ್ತಾಂಶ ತುಂಬು_ದತ್ತಾಂಶ_captcha(ಅಕ್ಷರ **paths, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ m, ಪೂರ್ಣಾಂಕ k, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h)
{
    ಆದರೆ(m) paths = get_random_paths(paths, n, m);
    ದತ್ತಾಂಶ d = {0};
    d.shallow = 0;
    d.X = ತುಂಬು_image_paths(paths, n, w, h);
    d.y = make_ಮಾತೃಕೆ(n, k*NUMCHARS);
    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ(i = 0; i < n; ++i){
        fill_truth_captcha(paths[i], k, d.y.vals[i]);
    }
    ಆದರೆ(m) ಬಿಡುಗಡೆ(paths);
    ಹಿಂತಿರುಗಿ d;
}

ದತ್ತಾಂಶ ತುಂಬು_ದತ್ತಾಂಶ_captcha_encode(ಅಕ್ಷರ **paths, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ m, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h)
{
    ಆದರೆ(m) paths = get_random_paths(paths, n, m);
    ದತ್ತಾಂಶ d = {0};
    d.shallow = 0;
    d.X = ತುಂಬು_image_paths(paths, n, w, h);
    d.X.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = 17100;
    d.y = d.X;
    ಆದರೆ(m) ಬಿಡುಗಡೆ(paths);
    ಹಿಂತಿರುಗಿ d;
}

ಶೂನ್ಯ fill_truth(ಅಕ್ಷರ *ಮಾರ್ಗ, ಅಕ್ಷರ **labels, ಪೂರ್ಣಾಂಕ k, ದಶಮಾಂಶ *truth)
{
    ಪೂರ್ಣಾಂಕ i;
    memset(truth, 0, k*ಗಾತ್ರ(ದಶಮಾಂಶ));
    ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2 = 0;
    ಗಾಗಿ(i = 0; i < k; ++i){
        ಆದರೆ(ಅಕ್ಷರಸರಣಿ_ಹುಡುಕು(ಮಾರ್ಗ, labels[i])){
            truth[i] = 1;
            ++ಎಣಿಕೆ_2;
            //printf("%s %s %d\n", path, labels[i], i);
        }
    }
    ಆದರೆ(ಎಣಿಕೆ_2 != 1 && (k != 1 || ಎಣಿಕೆ_2 != 0)) ಬರೆ("Too many or too few labels: %d, %s\n", ಎಣಿಕೆ_2, ಮಾರ್ಗ);
}

ಶೂನ್ಯ fill_hierarchy(ದಶಮಾಂಶ *truth, ಪೂರ್ಣಾಂಕ k, ವೃಕ್ಷ *hierarchy)
{
    ಪೂರ್ಣಾಂಕ j;
    ಗಾಗಿ(j = 0; j < k; ++j){
        ಆದರೆ(truth[j]){
            ಪೂರ್ಣಾಂಕ ಪೋಷಕ = hierarchy->ಪೋಷಕ[j];
            ಆಗ(ಪೋಷಕ >= 0){
                truth[ಪೋಷಕ] = 1;
                ಪೋಷಕ = hierarchy->ಪೋಷಕ[ಪೋಷಕ];
            }
        }
    }
    ಪೂರ್ಣಾಂಕ i;
    ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2 = 0;
    ಗಾಗಿ(j = 0; j < hierarchy->groups; ++j){
        //printf("%d\n", count);
        ಪೂರ್ಣಾಂಕ ಮುಖವಾಡ = 1;
        ಗಾಗಿ(i = 0; i < hierarchy->ಗುಂಪು_ಗಾತ್ರ[j]; ++i){
            ಆದರೆ(truth[ಎಣಿಕೆ_2 + i]){
                ಮುಖವಾಡ = 0;
                ನಿಲ್ಲಿಸು;
            }
        }
        ಆದರೆ (ಮುಖವಾಡ) {
            ಗಾಗಿ(i = 0; i < hierarchy->ಗುಂಪು_ಗಾತ್ರ[j]; ++i){
                truth[ಎಣಿಕೆ_2 + i] = SECRET_ಅಂಕೆ;
            }
        }
        ಎಣಿಕೆ_2 += hierarchy->ಗುಂಪು_ಗಾತ್ರ[j];
    }
}

ಮಾತೃಕೆ ತುಂಬು_regression_labels_paths(ಅಕ್ಷರ **paths, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ k)
{
    ಮಾತೃಕೆ y = make_ಮಾತೃಕೆ(n, k);
    ಪೂರ್ಣಾಂಕ i,j;
    ಗಾಗಿ(i = 0; i < n; ++i){
        ಅಕ್ಷರ labelpath[4096];
        ಕಂಡುಹಿಡಿ_replace(paths[i], "images", "labels", labelpath);
        ಕಂಡುಹಿಡಿ_replace(labelpath, "JPEGImages", "labels", labelpath);
        ಕಂಡುಹಿಡಿ_replace(labelpath, ".BMP", ".txt", labelpath);
        ಕಂಡುಹಿಡಿ_replace(labelpath, ".JPEG", ".txt", labelpath);
        ಕಂಡುಹಿಡಿ_replace(labelpath, ".JPG", ".txt", labelpath);
        ಕಂಡುಹಿಡಿ_replace(labelpath, ".JPeG", ".txt", labelpath);
        ಕಂಡುಹಿಡಿ_replace(labelpath, ".Jpeg", ".txt", labelpath);
        ಕಂಡುಹಿಡಿ_replace(labelpath, ".PNG", ".txt", labelpath);
        ಕಂಡುಹಿಡಿ_replace(labelpath, ".TIF", ".txt", labelpath);
        ಕಂಡುಹಿಡಿ_replace(labelpath, ".bmp", ".txt", labelpath);
        ಕಂಡುಹಿಡಿ_replace(labelpath, ".jpeg", ".txt", labelpath);
        ಕಂಡುಹಿಡಿ_replace(labelpath, ".jpg", ".txt", labelpath);
        ಕಂಡುಹಿಡಿ_replace(labelpath, ".png", ".txt", labelpath);
        ಕಂಡುಹಿಡಿ_replace(labelpath, ".tif", ".txt", labelpath);

        FILE *file = fopen(labelpath, "r");
        ಗಾಗಿ(j = 0; j < k; ++j){
            fscanf(file, "%f", &(y.vals[i][j]));
        }
        fclose(file);
    }
    ಹಿಂತಿರುಗಿ y;
}

ಮಾತೃಕೆ ತುಂಬು_labels_paths(ಅಕ್ಷರ **paths, ಪೂರ್ಣಾಂಕ n, ಅಕ್ಷರ **labels, ಪೂರ್ಣಾಂಕ k, ವೃಕ್ಷ *hierarchy)
{
    ಮಾತೃಕೆ y = make_ಮಾತೃಕೆ(n, k);
    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ(i = 0; i < n && labels; ++i){
        fill_truth(paths[i], labels, k, y.vals[i]);
        ಆದರೆ(hierarchy){
            fill_hierarchy(y.vals[i], k, hierarchy);
        }
    }
    ಹಿಂತಿರುಗಿ y;
}

ಮಾತೃಕೆ ತುಂಬು_ಟ್ಯಾಗುಗಳು_paths(ಅಕ್ಷರ **paths, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ k)
{
    ಮಾತೃಕೆ y = make_ಮಾತೃಕೆ(n, k);
    ಪೂರ್ಣಾಂಕ i;
    //int count = 0;
    ಗಾಗಿ(i = 0; i < n; ++i){
        ಅಕ್ಷರ label[4096];
        ಕಂಡುಹಿಡಿ_replace(paths[i], "images", "labels", label);
        ಕಂಡುಹಿಡಿ_replace(label, ".jpg", ".txt", label);
        FILE *file = fopen(label, "r");
        ಆದರೆ (!file) ಮುಂದುವರಿ;
        //++count;
        ಪೂರ್ಣಾಂಕ ಟ್ಯಾಗ್;
        ಆಗ(fscanf(file, "%d", &ಟ್ಯಾಗ್) == 1){
            ಆದರೆ(ಟ್ಯಾಗ್ < k){
                y.vals[i][ಟ್ಯಾಗ್] = 1;
            }
        }
        fclose(file);
    }
    //printf("%d/%d\n", count, n);
    ಹಿಂತಿರುಗಿ y;
}

ಅಕ್ಷರ **get_labels(ಅಕ್ಷರ *filename)
{
    list *plist = get_paths(filename);
    ಅಕ್ಷರ **labels = (ಅಕ್ಷರ **)list_ಗಮ್ಯ_array(plist);
    free_list(plist);
    ಹಿಂತಿರುಗಿ labels;
}

ಶೂನ್ಯ free_ದತ್ತಾಂಶ(ದತ್ತಾಂಶ d)
{
    ಆದರೆ(!d.shallow){
        free_ಮಾತೃಕೆ(d.X);
        free_ಮಾತೃಕೆ(d.y);
    }ಇಲ್ಲದಿದ್ದರೆ{
        ಬಿಡುಗಡೆ(d.X.vals);
        ಬಿಡುಗಡೆ(d.y.vals);
    }
}

image get_segmentation_image(ಅಕ್ಷರ *ಮಾರ್ಗ, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h, ಪೂರ್ಣಾಂಕ classes)
{
    ಅಕ್ಷರ labelpath[4096];
    ಕಂಡುಹಿಡಿ_replace(ಮಾರ್ಗ, "images", "mask", labelpath);
    ಕಂಡುಹಿಡಿ_replace(labelpath, "JPEGImages", "mask", labelpath);
    ಕಂಡುಹಿಡಿ_replace(labelpath, ".jpg", ".txt", labelpath);
    ಕಂಡುಹಿಡಿ_replace(labelpath, ".JPG", ".txt", labelpath);
    ಕಂಡುಹಿಡಿ_replace(labelpath, ".JPEG", ".txt", labelpath);
    image ಮುಖವಾಡ = make_image(w, h, classes);
    FILE *file = fopen(labelpath, "r");
    ಆದರೆ(!file) file_error(labelpath);
    ಅಕ್ಷರ buff[32788];
    ಪೂರ್ಣಾಂಕ id;
    image part = make_image(w, h, 1);
    ಆಗ(fscanf(file, "%d %s", &id, buff) == 2){
        ಪೂರ್ಣಾಂಕ n = 0;
        ಪೂರ್ಣಾಂಕ *rle = read_intlist(buff, &n, 0);
        ತುಂಬು_rle(part, rle, n);
        or_image(part, ಮುಖವಾಡ, id);
        ಬಿಡುಗಡೆ(rle);
    }
    //exclusive_image(mask);
    fclose(file);
    free_image(part);
    ಹಿಂತಿರುಗಿ ಮುಖವಾಡ;
}

image get_segmentation_image2(ಅಕ್ಷರ *ಮಾರ್ಗ, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h, ಪೂರ್ಣಾಂಕ classes)
{
    ಅಕ್ಷರ labelpath[4096];
    ಕಂಡುಹಿಡಿ_replace(ಮಾರ್ಗ, "images", "mask", labelpath);
    ಕಂಡುಹಿಡಿ_replace(labelpath, "JPEGImages", "mask", labelpath);
    ಕಂಡುಹಿಡಿ_replace(labelpath, ".jpg", ".txt", labelpath);
    ಕಂಡುಹಿಡಿ_replace(labelpath, ".JPG", ".txt", labelpath);
    ಕಂಡುಹಿಡಿ_replace(labelpath, ".JPEG", ".txt", labelpath);
    image ಮುಖವಾಡ = make_image(w, h, classes+1);
    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ(i = 0; i < w*h; ++i){
        ಮುಖವಾಡ.ದತ್ತಾಂಶ[w*h*classes + i] = 1;
    }
    FILE *file = fopen(labelpath, "r");
    ಆದರೆ(!file) file_error(labelpath);
    ಅಕ್ಷರ buff[32788];
    ಪೂರ್ಣಾಂಕ id;
    image part = make_image(w, h, 1);
    ಆಗ(fscanf(file, "%d %s", &id, buff) == 2){
        ಪೂರ್ಣಾಂಕ n = 0;
        ಪೂರ್ಣಾಂಕ *rle = read_intlist(buff, &n, 0);
        ತುಂಬು_rle(part, rle, n);
        or_image(part, ಮುಖವಾಡ, id);
        ಗಾಗಿ(i = 0; i < w*h; ++i){
            ಆದರೆ(part.ದತ್ತಾಂಶ[i]) ಮುಖವಾಡ.ದತ್ತಾಂಶ[w*h*classes + i] = 0;
        }
        ಬಿಡುಗಡೆ(rle);
    }
    //exclusive_image(mask);
    fclose(file);
    free_image(part);
    ಹಿಂತಿರುಗಿ ಮುಖವಾಡ;
}

ದತ್ತಾಂಶ ತುಂಬು_ದತ್ತಾಂಶ_seg(ಪೂರ್ಣಾಂಕ n, ಅಕ್ಷರ **paths, ಪೂರ್ಣಾಂಕ m, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h, ಪೂರ್ಣಾಂಕ classes, ಪೂರ್ಣಾಂಕ min, ಪೂರ್ಣಾಂಕ max, ದಶಮಾಂಶ angle, ದಶಮಾಂಶ aspect, ದಶಮಾಂಶ hue, ದಶಮಾಂಶ saturation, ದಶಮಾಂಶ exposure, ಪೂರ್ಣಾಂಕ div)
{
    ಅಕ್ಷರ **random_paths = get_random_paths(paths, n, m);
    ಪೂರ್ಣಾಂಕ i;
    ದತ್ತಾಂಶ d = {0};
    d.shallow = 0;

    d.X.ಅಡ್ಡಸಾಲುಗಳು = n;
    d.X.vals = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(d.X.ಅಡ್ಡಸಾಲುಗಳು, ಗಾತ್ರ(ದಶಮಾಂಶ*));
    d.X.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = h*w*3;


    d.y.ಅಡ್ಡಸಾಲುಗಳು = n;
    d.y.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = h*w*classes/div/div;
    d.y.vals = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(d.X.ಅಡ್ಡಸಾಲುಗಳು, ಗಾತ್ರ(ದಶಮಾಂಶ*));

    ಗಾಗಿ(i = 0; i < n; ++i){
        image orig = ತುಂಬು_image_color(random_paths[i], 0, 0);
        augment_ವಾದಗಳು a = random_augment_ವಾದಗಳು(orig, angle, aspect, min, max, w, h);
        image sized = rotate_crop_image(orig, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);

        ಪೂರ್ಣಾಂಕ flip = rand()%2;
        ಆದರೆ(flip) flip_image(sized);
        random_distort_image(sized, hue, saturation, exposure);
        d.X.vals[i] = sized.ದತ್ತಾಂಶ;

        image ಮುಖವಾಡ = get_segmentation_image(random_paths[i], orig.w, orig.h, classes);
        //image mask = make_image(orig.w, orig.h, classes+1);
        image sized_m = rotate_crop_image(ಮುಖವಾಡ, a.rad, a.scale/div, a.w/div, a.h/div, a.dx/div, a.dy/div, a.aspect);

        ಆದರೆ(flip) flip_image(sized_m);
        d.y.vals[i] = sized_m.ದತ್ತಾಂಶ;

        free_image(orig);
        free_image(ಮುಖವಾಡ);

        /*
           image rgb = mask_to_rgb(sized_m, classes);
           show_image(rgb, "part");
           show_image(sized, "orig");
           cvWaitKey(0);
           free_image(rgb);
         */
    }
    ಬಿಡುಗಡೆ(random_paths);
    ಹಿಂತಿರುಗಿ d;
}

ದತ್ತಾಂಶ ತುಂಬು_ದತ್ತಾಂಶ_iseg(ಪೂರ್ಣಾಂಕ n, ಅಕ್ಷರ **paths, ಪೂರ್ಣಾಂಕ m, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h, ಪೂರ್ಣಾಂಕ classes, ಪೂರ್ಣಾಂಕ boxes, ಪೂರ್ಣಾಂಕ div, ಪೂರ್ಣಾಂಕ min, ಪೂರ್ಣಾಂಕ max, ದಶಮಾಂಶ angle, ದಶಮಾಂಶ aspect, ದಶಮಾಂಶ hue, ದಶಮಾಂಶ saturation, ದಶಮಾಂಶ exposure)
{
    ಅಕ್ಷರ **random_paths = get_random_paths(paths, n, m);
    ಪೂರ್ಣಾಂಕ i;
    ದತ್ತಾಂಶ d = {0};
    d.shallow = 0;

    d.X.ಅಡ್ಡಸಾಲುಗಳು = n;
    d.X.vals = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(d.X.ಅಡ್ಡಸಾಲುಗಳು, ಗಾತ್ರ(ದಶಮಾಂಶ*));
    d.X.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = h*w*3;

    d.y = make_ಮಾತೃಕೆ(n, (((w/div)*(h/div))+1)*boxes);

    ಗಾಗಿ(i = 0; i < n; ++i){
        image orig = ತುಂಬು_image_color(random_paths[i], 0, 0);
        augment_ವಾದಗಳು a = random_augment_ವಾದಗಳು(orig, angle, aspect, min, max, w, h);
        image sized = rotate_crop_image(orig, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);

        ಪೂರ್ಣಾಂಕ flip = rand()%2;
        ಆದರೆ(flip) flip_image(sized);
        random_distort_image(sized, hue, saturation, exposure);
        d.X.vals[i] = sized.ದತ್ತಾಂಶ;
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
    ಬಿಡುಗಡೆ(random_paths);
    ಹಿಂತಿರುಗಿ d;
}

ದತ್ತಾಂಶ ತುಂಬು_ದತ್ತಾಂಶ_ಮುಖವಾಡ(ಪೂರ್ಣಾಂಕ n, ಅಕ್ಷರ **paths, ಪೂರ್ಣಾಂಕ m, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h, ಪೂರ್ಣಾಂಕ classes, ಪೂರ್ಣಾಂಕ boxes, ಪೂರ್ಣಾಂಕ coords, ಪೂರ್ಣಾಂಕ min, ಪೂರ್ಣಾಂಕ max, ದಶಮಾಂಶ angle, ದಶಮಾಂಶ aspect, ದಶಮಾಂಶ hue, ದಶಮಾಂಶ saturation, ದಶಮಾಂಶ exposure)
{
    ಅಕ್ಷರ **random_paths = get_random_paths(paths, n, m);
    ಪೂರ್ಣಾಂಕ i;
    ದತ್ತಾಂಶ d = {0};
    d.shallow = 0;

    d.X.ಅಡ್ಡಸಾಲುಗಳು = n;
    d.X.vals = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(d.X.ಅಡ್ಡಸಾಲುಗಳು, ಗಾತ್ರ(ದಶಮಾಂಶ*));
    d.X.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = h*w*3;

    d.y = make_ಮಾತೃಕೆ(n, (coords+1)*boxes);

    ಗಾಗಿ(i = 0; i < n; ++i){
        image orig = ತುಂಬು_image_color(random_paths[i], 0, 0);
        augment_ವಾದಗಳು a = random_augment_ವಾದಗಳು(orig, angle, aspect, min, max, w, h);
        image sized = rotate_crop_image(orig, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);

        ಪೂರ್ಣಾಂಕ flip = rand()%2;
        ಆದರೆ(flip) flip_image(sized);
        random_distort_image(sized, hue, saturation, exposure);
        d.X.vals[i] = sized.ದತ್ತಾಂಶ;
        //show_image(sized, "image");

        fill_truth_ಮುಖವಾಡ(random_paths[i], boxes, d.y.vals[i], classes, orig.w, orig.h, a, flip, 14, 14);

        free_image(orig);

        /*
           image rgb = mask_to_rgb(sized_m, classes);
           show_image(rgb, "part");
           show_image(sized, "orig");
           cvWaitKey(0);
           free_image(rgb);
         */
    }
    ಬಿಡುಗಡೆ(random_paths);
    ಹಿಂತಿರುಗಿ d;
}

ದತ್ತಾಂಶ ತುಂಬು_ದತ್ತಾಂಶ_region(ಪೂರ್ಣಾಂಕ n, ಅಕ್ಷರ **paths, ಪೂರ್ಣಾಂಕ m, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h, ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2, ಪೂರ್ಣಾಂಕ classes, ದಶಮಾಂಶ jitter, ದಶಮಾಂಶ hue, ದಶಮಾಂಶ saturation, ದಶಮಾಂಶ exposure)
{
    ಅಕ್ಷರ **random_paths = get_random_paths(paths, n, m);
    ಪೂರ್ಣಾಂಕ i;
    ದತ್ತಾಂಶ d = {0};
    d.shallow = 0;

    d.X.ಅಡ್ಡಸಾಲುಗಳು = n;
    d.X.vals = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(d.X.ಅಡ್ಡಸಾಲುಗಳು, ಗಾತ್ರ(ದಶಮಾಂಶ*));
    d.X.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = h*w*3;


    ಪೂರ್ಣಾಂಕ k = ಗಾತ್ರ_2*ಗಾತ್ರ_2*(5+classes);
    d.y = make_ಮಾತೃಕೆ(n, k);
    ಗಾಗಿ(i = 0; i < n; ++i){
        image orig = ತುಂಬು_image_color(random_paths[i], 0, 0);

        ಪೂರ್ಣಾಂಕ oh = orig.h;
        ಪೂರ್ಣಾಂಕ ow = orig.w;

        ಪೂರ್ಣಾಂಕ dw = (ow*jitter);
        ಪೂರ್ಣಾಂಕ dh = (oh*jitter);

        ಪೂರ್ಣಾಂಕ pleft  = rand_ಏಕರೂಪ(-dw, dw);
        ಪೂರ್ಣಾಂಕ pright = rand_ಏಕರೂಪ(-dw, dw);
        ಪೂರ್ಣಾಂಕ ptop   = rand_ಏಕರೂಪ(-dh, dh);
        ಪೂರ್ಣಾಂಕ pbot   = rand_ಏಕರೂಪ(-dh, dh);

        ಪೂರ್ಣಾಂಕ swidth =  ow - pleft - pright;
        ಪೂರ್ಣಾಂಕ sheight = oh - ptop - pbot;

        ದಶಮಾಂಶ sx = (ದಶಮಾಂಶ)swidth  / ow;
        ದಶಮಾಂಶ sy = (ದಶಮಾಂಶ)sheight / oh;

        ಪೂರ್ಣಾಂಕ flip = rand()%2;
        image cropped = crop_image(orig, pleft, ptop, swidth, sheight);

        ದಶಮಾಂಶ dx = ((ದಶಮಾಂಶ)pleft/ow)/sx;
        ದಶಮಾಂಶ dy = ((ದಶಮಾಂಶ)ptop /oh)/sy;

        image sized = resize_image(cropped, w, h);
        ಆದರೆ(flip) flip_image(sized);
        random_distort_image(sized, hue, saturation, exposure);
        d.X.vals[i] = sized.ದತ್ತಾಂಶ;

        fill_truth_region(random_paths[i], d.y.vals[i], classes, ಗಾತ್ರ_2, flip, dx, dy, 1./sx, 1./sy);

        free_image(orig);
        free_image(cropped);
    }
    ಬಿಡುಗಡೆ(random_paths);
    ಹಿಂತಿರುಗಿ d;
}

ದತ್ತಾಂಶ ತುಂಬು_ದತ್ತಾಂಶ_compare(ಪೂರ್ಣಾಂಕ n, ಅಕ್ಷರ **paths, ಪೂರ್ಣಾಂಕ m, ಪೂರ್ಣಾಂಕ classes, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h)
{
    ಆದರೆ(m) paths = get_random_paths(paths, 2*n, m);
    ಪೂರ್ಣಾಂಕ i,j;
    ದತ್ತಾಂಶ d = {0};
    d.shallow = 0;

    d.X.ಅಡ್ಡಸಾಲುಗಳು = n;
    d.X.vals = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(d.X.ಅಡ್ಡಸಾಲುಗಳು, ಗಾತ್ರ(ದಶಮಾಂಶ*));
    d.X.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = h*w*6;

    ಪೂರ್ಣಾಂಕ k = 2*(classes);
    d.y = make_ಮಾತೃಕೆ(n, k);
    ಗಾಗಿ(i = 0; i < n; ++i){
        image im1 = ತುಂಬು_image_color(paths[i*2],   w, h);
        image im2 = ತುಂಬು_image_color(paths[i*2+1], w, h);

        d.X.vals[i] = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(d.X.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, ಗಾತ್ರ(ದಶಮಾಂಶ));
        ಸ್ಮರಣೆ_ನಕಲು(d.X.vals[i],         im1.ದತ್ತಾಂಶ, h*w*3*ಗಾತ್ರ(ದಶಮಾಂಶ));
        ಸ್ಮರಣೆ_ನಕಲು(d.X.vals[i] + h*w*3, im2.ದತ್ತಾಂಶ, h*w*3*ಗಾತ್ರ(ದಶಮಾಂಶ));

        ಪೂರ್ಣಾಂಕ id;
        ದಶಮಾಂಶ iou;

        ಅಕ್ಷರ imlabel1[4096];
        ಅಕ್ಷರ imlabel2[4096];
        ಕಂಡುಹಿಡಿ_replace(paths[i*2],   "imgs", "labels", imlabel1);
        ಕಂಡುಹಿಡಿ_replace(imlabel1, "jpg", "txt", imlabel1);
        FILE *fp1 = fopen(imlabel1, "r");

        ಆಗ(fscanf(fp1, "%d %f", &id, &iou) == 2){
            ಆದರೆ (d.y.vals[i][2*id] < iou) d.y.vals[i][2*id] = iou;
        }

        ಕಂಡುಹಿಡಿ_replace(paths[i*2+1], "imgs", "labels", imlabel2);
        ಕಂಡುಹಿಡಿ_replace(imlabel2, "jpg", "txt", imlabel2);
        FILE *fp2 = fopen(imlabel2, "r");

        ಆಗ(fscanf(fp2, "%d %f", &id, &iou) == 2){
            ಆದರೆ (d.y.vals[i][2*id + 1] < iou) d.y.vals[i][2*id + 1] = iou;
        }

        ಗಾಗಿ (j = 0; j < classes; ++j){
            ಆದರೆ (d.y.vals[i][2*j] > .5 &&  d.y.vals[i][2*j+1] < .5){
                d.y.vals[i][2*j] = 1;
                d.y.vals[i][2*j+1] = 0;
            } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (d.y.vals[i][2*j] < .5 &&  d.y.vals[i][2*j+1] > .5){
                d.y.vals[i][2*j] = 0;
                d.y.vals[i][2*j+1] = 1;
            } ಇಲ್ಲದಿದ್ದರೆ {
                d.y.vals[i][2*j]   = SECRET_ಅಂಕೆ;
                d.y.vals[i][2*j+1] = SECRET_ಅಂಕೆ;
            }
        }
        fclose(fp1);
        fclose(fp2);

        free_image(im1);
        free_image(im2);
    }
    ಆದರೆ(m) ಬಿಡುಗಡೆ(paths);
    ಹಿಂತಿರುಗಿ d;
}

ದತ್ತಾಂಶ ತುಂಬು_ದತ್ತಾಂಶ_swag(ಅಕ್ಷರ **paths, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ classes, ದಶಮಾಂಶ jitter)
{
    ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ = rand()%n;
    ಅಕ್ಷರ *random_ಮಾರ್ಗ = paths[ಸೂಚ್ಯಂಕ];

    image orig = ತುಂಬು_image_color(random_ಮಾರ್ಗ, 0, 0);
    ಪೂರ್ಣಾಂಕ h = orig.h;
    ಪೂರ್ಣಾಂಕ w = orig.w;

    ದತ್ತಾಂಶ d = {0};
    d.shallow = 0;
    d.w = w;
    d.h = h;

    d.X.ಅಡ್ಡಸಾಲುಗಳು = 1;
    d.X.vals = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(d.X.ಅಡ್ಡಸಾಲುಗಳು, ಗಾತ್ರ(ದಶಮಾಂಶ*));
    d.X.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = h*w*3;

    ಪೂರ್ಣಾಂಕ k = (4+classes)*90;
    d.y = make_ಮಾತೃಕೆ(1, k);

    ಪೂರ್ಣಾಂಕ dw = w*jitter;
    ಪೂರ್ಣಾಂಕ dh = h*jitter;

    ಪೂರ್ಣಾಂಕ pleft  = rand_ಏಕರೂಪ(-dw, dw);
    ಪೂರ್ಣಾಂಕ pright = rand_ಏಕರೂಪ(-dw, dw);
    ಪೂರ್ಣಾಂಕ ptop   = rand_ಏಕರೂಪ(-dh, dh);
    ಪೂರ್ಣಾಂಕ pbot   = rand_ಏಕರೂಪ(-dh, dh);

    ಪೂರ್ಣಾಂಕ swidth =  w - pleft - pright;
    ಪೂರ್ಣಾಂಕ sheight = h - ptop - pbot;

    ದಶಮಾಂಶ sx = (ದಶಮಾಂಶ)swidth  / w;
    ದಶಮಾಂಶ sy = (ದಶಮಾಂಶ)sheight / h;

    ಪೂರ್ಣಾಂಕ flip = rand()%2;
    image cropped = crop_image(orig, pleft, ptop, swidth, sheight);

    ದಶಮಾಂಶ dx = ((ದಶಮಾಂಶ)pleft/w)/sx;
    ದಶಮಾಂಶ dy = ((ದಶಮಾಂಶ)ptop /h)/sy;

    image sized = resize_image(cropped, w, h);
    ಆದರೆ(flip) flip_image(sized);
    d.X.vals[0] = sized.ದತ್ತಾಂಶ;

    fill_truth_swag(random_ಮಾರ್ಗ, d.y.vals[0], classes, flip, dx, dy, 1./sx, 1./sy);

    free_image(orig);
    free_image(cropped);

    ಹಿಂತಿರುಗಿ d;
}

ದತ್ತಾಂಶ ತುಂಬು_ದತ್ತಾಂಶ_detection(ಪೂರ್ಣಾಂಕ n, ಅಕ್ಷರ **paths, ಪೂರ್ಣಾಂಕ m, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h, ಪೂರ್ಣಾಂಕ boxes, ಪೂರ್ಣಾಂಕ classes, ದಶಮಾಂಶ jitter, ದಶಮಾಂಶ hue, ದಶಮಾಂಶ saturation, ದಶಮಾಂಶ exposure)
{
    ಅಕ್ಷರ **random_paths = get_random_paths(paths, n, m);
    ಪೂರ್ಣಾಂಕ i;
    ದತ್ತಾಂಶ d = {0};
    d.shallow = 0;

    d.X.ಅಡ್ಡಸಾಲುಗಳು = n;
    d.X.vals = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(d.X.ಅಡ್ಡಸಾಲುಗಳು, ಗಾತ್ರ(ದಶಮಾಂಶ*));
    d.X.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = h*w*3;

    d.y = make_ಮಾತೃಕೆ(n, 5*boxes);
    ಗಾಗಿ(i = 0; i < n; ++i){
        image orig = ತುಂಬು_image_color(random_paths[i], 0, 0);
        image sized = make_image(w, h, orig.c);
        fill_image(sized, .5);

        ದಶಮಾಂಶ dw = jitter * orig.w;
        ದಶಮಾಂಶ dh = jitter * orig.h;

        ದಶಮಾಂಶ new_ar = (orig.w + rand_ಏಕರೂಪ(-dw, dw)) / (orig.h + rand_ಏಕರೂಪ(-dh, dh));
        //float scale = rand_uniform(.25, 2);
        ದಶಮಾಂಶ scale = 1;

        ದಶಮಾಂಶ nw, nh;

        ಆದರೆ(new_ar < 1){
            nh = scale * h;
            nw = nh * new_ar;
        } ಇಲ್ಲದಿದ್ದರೆ {
            nw = scale * w;
            nh = nw / new_ar;
        }

        ದಶಮಾಂಶ dx = rand_ಏಕರೂಪ(0, w - nw);
        ದಶಮಾಂಶ dy = rand_ಏಕರೂಪ(0, h - nh);

        ಇರಿಸು_image(orig, nw, nh, dx, dy, sized);

        random_distort_image(sized, hue, saturation, exposure);

        ಪೂರ್ಣಾಂಕ flip = rand()%2;
        ಆದರೆ(flip) flip_image(sized);
        d.X.vals[i] = sized.ದತ್ತಾಂಶ;


        fill_truth_detection(random_paths[i], boxes, d.y.vals[i], classes, flip, -dx/w, -dy/h, nw/w, nh/h);

        free_image(orig);
    }
    ಬಿಡುಗಡೆ(random_paths);
    ಹಿಂತಿರುಗಿ d;
}

ಶೂನ್ಯ *ತುಂಬು_thread(ಶೂನ್ಯ *ptr)
{
    //printf("Loading data: %d\n", rand());
    ತುಂಬು_ವಾದಗಳು a = *(ರಚನೆ ತುಂಬು_ವಾದಗಳು*)ptr;
    ಆದರೆ(a.exposure == 0) a.exposure = 1;
    ಆದರೆ(a.saturation == 0) a.saturation = 1;
    ಆದರೆ(a.aspect == 0) a.aspect = 1;

    ಆದರೆ (a.type == OLD_CLASSIFICATION_ದತ್ತಾಂಶ){
        *a.d = ತುಂಬು_ದತ್ತಾಂಶ_old(a.paths, a.n, a.m, a.labels, a.classes, a.w, a.h);
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (a.type == REGRESSION_ದತ್ತಾಂಶ){
        *a.d = ತುಂಬು_ದತ್ತಾಂಶ_regression(a.paths, a.n, a.m, a.classes, a.min, a.max, a.ಗಾತ್ರ_2, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (a.type == CLASSIFICATION_ದತ್ತಾಂಶ){
        *a.d = ತುಂಬು_ದತ್ತಾಂಶ_augment(a.paths, a.n, a.m, a.labels, a.classes, a.hierarchy, a.min, a.max, a.ಗಾತ್ರ_2, a.angle, a.aspect, a.hue, a.saturation, a.exposure, a.center);
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (a.type == SUPER_ದತ್ತಾಂಶ){
        *a.d = ತುಂಬು_ದತ್ತಾಂಶ_super(a.paths, a.n, a.m, a.w, a.h, a.scale);
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (a.type == WRITING_ದತ್ತಾಂಶ){
        *a.d = ತುಂಬು_ದತ್ತಾಂಶ_writing(a.paths, a.n, a.m, a.w, a.h, a.ಹೊರಹೋಗು_w, a.ಹೊರಹೋಗು_h);
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (a.type == ISEG_ದತ್ತಾಂಶ){
        *a.d = ತುಂಬು_ದತ್ತಾಂಶ_iseg(a.n, a.paths, a.m, a.w, a.h, a.classes, a.ಅಂಕೆ_boxes, a.scale, a.min, a.max, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (a.type == INSTANCE_ದತ್ತಾಂಶ){
        *a.d = ತುಂಬು_ದತ್ತಾಂಶ_ಮುಖವಾಡ(a.n, a.paths, a.m, a.w, a.h, a.classes, a.ಅಂಕೆ_boxes, a.coords, a.min, a.max, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (a.type == SEGMENTATION_ದತ್ತಾಂಶ){
        *a.d = ತುಂಬು_ದತ್ತಾಂಶ_seg(a.n, a.paths, a.m, a.w, a.h, a.classes, a.min, a.max, a.angle, a.aspect, a.hue, a.saturation, a.exposure, a.scale);
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (a.type == REGION_ದತ್ತಾಂಶ){
        *a.d = ತುಂಬು_ದತ್ತಾಂಶ_region(a.n, a.paths, a.m, a.w, a.h, a.ಅಂಕೆ_boxes, a.classes, a.jitter, a.hue, a.saturation, a.exposure);
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (a.type == DETECTION_ದತ್ತಾಂಶ){
        *a.d = ತುಂಬು_ದತ್ತಾಂಶ_detection(a.n, a.paths, a.m, a.w, a.h, a.ಅಂಕೆ_boxes, a.classes, a.jitter, a.hue, a.saturation, a.exposure);
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (a.type == SWAG_ದತ್ತಾಂಶ){
        *a.d = ತುಂಬು_ದತ್ತಾಂಶ_swag(a.paths, a.n, a.classes, a.jitter);
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (a.type == COMPARE_ದತ್ತಾಂಶ){
        *a.d = ತುಂಬು_ದತ್ತಾಂಶ_compare(a.n, a.paths, a.m, a.classes, a.w, a.h);
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (a.type == IMAGE_ದತ್ತಾಂಶ){
        *(a.im) = ತುಂಬು_image_color(a.ಮಾರ್ಗ, 0, 0);
        *(a.resized) = resize_image(*(a.im), a.w, a.h);
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (a.type == LETTERBOX_ದತ್ತಾಂಶ){
        *(a.im) = ತುಂಬು_image_color(a.ಮಾರ್ಗ, 0, 0);
        *(a.resized) = letterbox_image(*(a.im), a.w, a.h);
    } ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (a.type == ಟ್ಯಾಗ್_ದತ್ತಾಂಶ){
        *a.d = ತುಂಬು_ದತ್ತಾಂಶ_ಟ್ಯಾಗ್(a.paths, a.n, a.m, a.classes, a.min, a.max, a.ಗಾತ್ರ_2, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    }
    ಬಿಡುಗಡೆ(ptr);
    ಹಿಂತಿರುಗಿ 0;
}

pthread_t ತುಂಬು_ದತ್ತಾಂಶ_ಒಳ_thread(ತುಂಬು_ವಾದಗಳು ವಾದಗಳು)
{
    pthread_t thread;
    ರಚನೆ ತುಂಬು_ವಾದಗಳು *ptr = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(1, ಗಾತ್ರ(ರಚನೆ ತುಂಬು_ವಾದಗಳು));
    *ptr = ವಾದಗಳು;
    ಆದರೆ(pthread_create(&thread, 0, ತುಂಬು_thread, ptr)) error("Thread creation failed");
    ಹಿಂತಿರುಗಿ thread;
}

ಶೂನ್ಯ *ತುಂಬು_ಎಳೆಗಳು(ಶೂನ್ಯ *ptr)
{
    ಪೂರ್ಣಾಂಕ i;
    ತುಂಬು_ವಾದಗಳು ವಾದಗಳು = *(ತುಂಬು_ವಾದಗಳು *)ptr;
    ಆದರೆ (ವಾದಗಳು.ಎಳೆಗಳು == 0) ವಾದಗಳು.ಎಳೆಗಳು = 1;
    ದತ್ತಾಂಶ *ಹೊರಹೋಗು = ವಾದಗಳು.d;
    ಪೂರ್ಣಾಂಕ ಒಟ್ಟು = ವಾದಗಳು.n;
    ಬಿಡುಗಡೆ(ptr);
    ದತ್ತಾಂಶ *buffers = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(ವಾದಗಳು.ಎಳೆಗಳು, ಗಾತ್ರ(ದತ್ತಾಂಶ));
    pthread_t *ಎಳೆಗಳು = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(ವಾದಗಳು.ಎಳೆಗಳು, ಗಾತ್ರ(pthread_t));
    ಗಾಗಿ(i = 0; i < ವಾದಗಳು.ಎಳೆಗಳು; ++i){
        ವಾದಗಳು.d = buffers + i;
        ವಾದಗಳು.n = (i+1) * ಒಟ್ಟು/ವಾದಗಳು.ಎಳೆಗಳು - i * ಒಟ್ಟು/ವಾದಗಳು.ಎಳೆಗಳು;
        ಎಳೆಗಳು[i] = ತುಂಬು_ದತ್ತಾಂಶ_ಒಳ_thread(ವಾದಗಳು);
    }
    ಗಾಗಿ(i = 0; i < ವಾದಗಳು.ಎಳೆಗಳು; ++i){
        pthread_join(ಎಳೆಗಳು[i], 0);
    }
    *ಹೊರಹೋಗು = concat_datas(buffers, ವಾದಗಳು.ಎಳೆಗಳು);
    ಹೊರಹೋಗು->shallow = 0;
    ಗಾಗಿ(i = 0; i < ವಾದಗಳು.ಎಳೆಗಳು; ++i){
        buffers[i].shallow = 1;
        free_ದತ್ತಾಂಶ(buffers[i]);
    }
    ಬಿಡುಗಡೆ(buffers);
    ಬಿಡುಗಡೆ(ಎಳೆಗಳು);
    ಹಿಂತಿರುಗಿ 0;
}

ಶೂನ್ಯ ತುಂಬು_ದತ್ತಾಂಶ_blocking(ತುಂಬು_ವಾದಗಳು ವಾದಗಳು)
{
    ರಚನೆ ತುಂಬು_ವಾದಗಳು *ptr = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(1, ಗಾತ್ರ(ರಚನೆ ತುಂಬು_ವಾದಗಳು));
    *ptr = ವಾದಗಳು;
    ತುಂಬು_thread(ptr);
}

pthread_t ತುಂಬು_ದತ್ತಾಂಶ(ತುಂಬು_ವಾದಗಳು ವಾದಗಳು)
{
    pthread_t thread;
    ರಚನೆ ತುಂಬು_ವಾದಗಳು *ptr = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(1, ಗಾತ್ರ(ರಚನೆ ತುಂಬು_ವಾದಗಳು));
    *ptr = ವಾದಗಳು;
    ಆದರೆ(pthread_create(&thread, 0, ತುಂಬು_ಎಳೆಗಳು, ptr)) error("Thread creation failed");
    ಹಿಂತಿರುಗಿ thread;
}

ದತ್ತಾಂಶ ತುಂಬು_ದತ್ತಾಂಶ_writing(ಅಕ್ಷರ **paths, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ m, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h, ಪೂರ್ಣಾಂಕ ಹೊರಹೋಗು_w, ಪೂರ್ಣಾಂಕ ಹೊರಹೋಗು_h)
{
    ಆದರೆ(m) paths = get_random_paths(paths, n, m);
    ಅಕ್ಷರ **replace_paths = ಕಂಡುಹಿಡಿ_replace_paths(paths, n, ".png", "-label.png");
    ದತ್ತಾಂಶ d = {0};
    d.shallow = 0;
    d.X = ತುಂಬು_image_paths(paths, n, w, h);
    d.y = ತುಂಬು_image_paths_ಬೂದು(replace_paths, n, ಹೊರಹೋಗು_w, ಹೊರಹೋಗು_h);
    ಆದರೆ(m) ಬಿಡುಗಡೆ(paths);
    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ(i = 0; i < n; ++i) ಬಿಡುಗಡೆ(replace_paths[i]);
    ಬಿಡುಗಡೆ(replace_paths);
    ಹಿಂತಿರುಗಿ d;
}

ದತ್ತಾಂಶ ತುಂಬು_ದತ್ತಾಂಶ_old(ಅಕ್ಷರ **paths, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ m, ಅಕ್ಷರ **labels, ಪೂರ್ಣಾಂಕ k, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h)
{
    ಆದರೆ(m) paths = get_random_paths(paths, n, m);
    ದತ್ತಾಂಶ d = {0};
    d.shallow = 0;
    d.X = ತುಂಬು_image_paths(paths, n, w, h);
    d.y = ತುಂಬು_labels_paths(paths, n, labels, k, 0);
    ಆದರೆ(m) ಬಿಡುಗಡೆ(paths);
    ಹಿಂತಿರುಗಿ d;
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

ದತ್ತಾಂಶ ತುಂಬು_ದತ್ತಾಂಶ_super(ಅಕ್ಷರ **paths, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ m, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h, ಪೂರ್ಣಾಂಕ scale)
{
    ಆದರೆ(m) paths = get_random_paths(paths, n, m);
    ದತ್ತಾಂಶ d = {0};
    d.shallow = 0;

    ಪೂರ್ಣಾಂಕ i;
    d.X.ಅಡ್ಡಸಾಲುಗಳು = n;
    d.X.vals = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(n, ಗಾತ್ರ(ದಶಮಾಂಶ*));
    d.X.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = w*h*3;

    d.y.ಅಡ್ಡಸಾಲುಗಳು = n;
    d.y.vals = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(n, ಗಾತ್ರ(ದಶಮಾಂಶ*));
    d.y.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = w*scale * h*scale * 3;

    ಗಾಗಿ(i = 0; i < n; ++i){
        image im = ತುಂಬು_image_color(paths[i], 0, 0);
        image crop = random_crop_image(im, w*scale, h*scale);
        ಪೂರ್ಣಾಂಕ flip = rand()%2;
        ಆದರೆ (flip) flip_image(crop);
        image resize = resize_image(crop, w, h);
        d.X.vals[i] = resize.ದತ್ತಾಂಶ;
        d.y.vals[i] = crop.ದತ್ತಾಂಶ;
        free_image(im);
    }

    ಆದರೆ(m) ಬಿಡುಗಡೆ(paths);
    ಹಿಂತಿರುಗಿ d;
}

ದತ್ತಾಂಶ ತುಂಬು_ದತ್ತಾಂಶ_regression(ಅಕ್ಷರ **paths, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ m, ಪೂರ್ಣಾಂಕ k, ಪೂರ್ಣಾಂಕ min, ಪೂರ್ಣಾಂಕ max, ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2, ದಶಮಾಂಶ angle, ದಶಮಾಂಶ aspect, ದಶಮಾಂಶ hue, ದಶಮಾಂಶ saturation, ದಶಮಾಂಶ exposure)
{
    ಆದರೆ(m) paths = get_random_paths(paths, n, m);
    ದತ್ತಾಂಶ d = {0};
    d.shallow = 0;
    d.X = ತುಂಬು_image_augment_paths(paths, n, min, max, ಗಾತ್ರ_2, angle, aspect, hue, saturation, exposure, 0);
    d.y = ತುಂಬು_regression_labels_paths(paths, n, k);
    ಆದರೆ(m) ಬಿಡುಗಡೆ(paths);
    ಹಿಂತಿರುಗಿ d;
}

ದತ್ತಾಂಶ select_ದತ್ತಾಂಶ(ದತ್ತಾಂಶ *orig, ಪೂರ್ಣಾಂಕ *inds)
{
    ದತ್ತಾಂಶ d = {0};
    d.shallow = 1;
    d.w = orig[0].w;
    d.h = orig[0].h;

    d.X.ಅಡ್ಡಸಾಲುಗಳು = orig[0].X.ಅಡ್ಡಸಾಲುಗಳು;
    d.y.ಅಡ್ಡಸಾಲುಗಳು = orig[0].X.ಅಡ್ಡಸಾಲುಗಳು;

    d.X.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = orig[0].X.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
    d.y.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = orig[0].y.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;

    d.X.vals = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(orig[0].X.ಅಡ್ಡಸಾಲುಗಳು, ಗಾತ್ರ(ದಶಮಾಂಶ *));
    d.y.vals = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(orig[0].y.ಅಡ್ಡಸಾಲುಗಳು, ಗಾತ್ರ(ದಶಮಾಂಶ *));
    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ(i = 0; i < d.X.ಅಡ್ಡಸಾಲುಗಳು; ++i){
        d.X.vals[i] = orig[inds[i]].X.vals[i];
        d.y.vals[i] = orig[inds[i]].y.vals[i];
    }
    ಹಿಂತಿರುಗಿ d;
}

ದತ್ತಾಂಶ *tile_ದತ್ತಾಂಶ(ದತ್ತಾಂಶ orig, ಪೂರ್ಣಾಂಕ divs, ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2)
{
    ದತ್ತಾಂಶ *ds = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(divs*divs, ಗಾತ್ರ(ದತ್ತಾಂಶ));
    ಪೂರ್ಣಾಂಕ i, j;
#ಸೂಚನೆ omp parallel ಗಾಗಿ
    ಗಾಗಿ(i = 0; i < divs*divs; ++i){
        ದತ್ತಾಂಶ d;
        d.shallow = 0;
        d.w = orig.w/divs * ಗಾತ್ರ_2;
        d.h = orig.h/divs * ಗಾತ್ರ_2;
        d.X.ಅಡ್ಡಸಾಲುಗಳು = orig.X.ಅಡ್ಡಸಾಲುಗಳು;
        d.X.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = d.w*d.h*3;
        d.X.vals = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(d.X.ಅಡ್ಡಸಾಲುಗಳು, ಗಾತ್ರ(ದಶಮಾಂಶ*));

        d.y = copy_ಮಾತೃಕೆ(orig.y);
#ಸೂಚನೆ omp parallel ಗಾಗಿ
        ಗಾಗಿ(j = 0; j < orig.X.ಅಡ್ಡಸಾಲುಗಳು; ++j){
            ಪೂರ್ಣಾಂಕ x = (i%divs) * orig.w / divs - (d.w - orig.w/divs)/2;
            ಪೂರ್ಣಾಂಕ y = (i/divs) * orig.h / divs - (d.h - orig.h/divs)/2;
            image im = float_ಗಮ್ಯ_image(orig.w, orig.h, 3, orig.X.vals[j]);
            d.X.vals[j] = crop_image(im, x, y, d.w, d.h).ದತ್ತಾಂಶ;
        }
        ds[i] = d;
    }
    ಹಿಂತಿರುಗಿ ds;
}

ದತ್ತಾಂಶ resize_ದತ್ತಾಂಶ(ದತ್ತಾಂಶ orig, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h)
{
    ದತ್ತಾಂಶ d = {0};
    d.shallow = 0;
    d.w = w;
    d.h = h;
    ಪೂರ್ಣಾಂಕ i;
    d.X.ಅಡ್ಡಸಾಲುಗಳು = orig.X.ಅಡ್ಡಸಾಲುಗಳು;
    d.X.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = w*h*3;
    d.X.vals = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(d.X.ಅಡ್ಡಸಾಲುಗಳು, ಗಾತ್ರ(ದಶಮಾಂಶ*));

    d.y = copy_ಮಾತೃಕೆ(orig.y);
#ಸೂಚನೆ omp parallel ಗಾಗಿ
    ಗಾಗಿ(i = 0; i < orig.X.ಅಡ್ಡಸಾಲುಗಳು; ++i){
        image im = float_ಗಮ್ಯ_image(orig.w, orig.h, 3, orig.X.vals[i]);
        d.X.vals[i] = resize_image(im, w, h).ದತ್ತಾಂಶ;
    }
    ಹಿಂತಿರುಗಿ d;
}

ದತ್ತಾಂಶ ತುಂಬು_ದತ್ತಾಂಶ_augment(ಅಕ್ಷರ **paths, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ m, ಅಕ್ಷರ **labels, ಪೂರ್ಣಾಂಕ k, ವೃಕ್ಷ *hierarchy, ಪೂರ್ಣಾಂಕ min, ಪೂರ್ಣಾಂಕ max, ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2, ದಶಮಾಂಶ angle, ದಶಮಾಂಶ aspect, ದಶಮಾಂಶ hue, ದಶಮಾಂಶ saturation, ದಶಮಾಂಶ exposure, ಪೂರ್ಣಾಂಕ center)
{
    ಆದರೆ(m) paths = get_random_paths(paths, n, m);
    ದತ್ತಾಂಶ d = {0};
    d.shallow = 0;
    d.w=ಗಾತ್ರ_2;
    d.h=ಗಾತ್ರ_2;
    d.X = ತುಂಬು_image_augment_paths(paths, n, min, max, ಗಾತ್ರ_2, angle, aspect, hue, saturation, exposure, center);
    d.y = ತುಂಬು_labels_paths(paths, n, labels, k, hierarchy);
    ಆದರೆ(m) ಬಿಡುಗಡೆ(paths);
    ಹಿಂತಿರುಗಿ d;
}

ದತ್ತಾಂಶ ತುಂಬು_ದತ್ತಾಂಶ_ಟ್ಯಾಗ್(ಅಕ್ಷರ **paths, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ m, ಪೂರ್ಣಾಂಕ k, ಪೂರ್ಣಾಂಕ min, ಪೂರ್ಣಾಂಕ max, ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2, ದಶಮಾಂಶ angle, ದಶಮಾಂಶ aspect, ದಶಮಾಂಶ hue, ದಶಮಾಂಶ saturation, ದಶಮಾಂಶ exposure)
{
    ಆದರೆ(m) paths = get_random_paths(paths, n, m);
    ದತ್ತಾಂಶ d = {0};
    d.w = ಗಾತ್ರ_2;
    d.h = ಗಾತ್ರ_2;
    d.shallow = 0;
    d.X = ತುಂಬು_image_augment_paths(paths, n, min, max, ಗಾತ್ರ_2, angle, aspect, hue, saturation, exposure, 0);
    d.y = ತುಂಬು_ಟ್ಯಾಗುಗಳು_paths(paths, n, k);
    ಆದರೆ(m) ಬಿಡುಗಡೆ(paths);
    ಹಿಂತಿರುಗಿ d;
}

ಮಾತೃಕೆ concat_ಮಾತೃಕೆ(ಮಾತೃಕೆ m1, ಮಾತೃಕೆ m2)
{
    ಪೂರ್ಣಾಂಕ i, ಎಣಿಕೆ_2 = 0;
    ಮಾತೃಕೆ m;
    m.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = m1.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
    m.ಅಡ್ಡಸಾಲುಗಳು = m1.ಅಡ್ಡಸಾಲುಗಳು+m2.ಅಡ್ಡಸಾಲುಗಳು;
    m.vals = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(m1.ಅಡ್ಡಸಾಲುಗಳು + m2.ಅಡ್ಡಸಾಲುಗಳು, ಗಾತ್ರ(ದಶಮಾಂಶ*));
    ಗಾಗಿ(i = 0; i < m1.ಅಡ್ಡಸಾಲುಗಳು; ++i){
        m.vals[ಎಣಿಕೆ_2++] = m1.vals[i];
    }
    ಗಾಗಿ(i = 0; i < m2.ಅಡ್ಡಸಾಲುಗಳು; ++i){
        m.vals[ಎಣಿಕೆ_2++] = m2.vals[i];
    }
    ಹಿಂತಿರುಗಿ m;
}

ದತ್ತಾಂಶ concat_ದತ್ತಾಂಶ(ದತ್ತಾಂಶ d1, ದತ್ತಾಂಶ d2)
{
    ದತ್ತಾಂಶ d = {0};
    d.shallow = 1;
    d.X = concat_ಮಾತೃಕೆ(d1.X, d2.X);
    d.y = concat_ಮಾತೃಕೆ(d1.y, d2.y);
    d.w = d1.w;
    d.h = d1.h;
    ಹಿಂತಿರುಗಿ d;
}

ದತ್ತಾಂಶ concat_datas(ದತ್ತಾಂಶ *d, ಪೂರ್ಣಾಂಕ n)
{
    ಪೂರ್ಣಾಂಕ i;
    ದತ್ತಾಂಶ ಹೊರಹೋಗು = {0};
    ಗಾಗಿ(i = 0; i < n; ++i){
        ದತ್ತಾಂಶ new = concat_ದತ್ತಾಂಶ(d[i], ಹೊರಹೋಗು);
        free_ದತ್ತಾಂಶ(ಹೊರಹೋಗು);
        ಹೊರಹೋಗು = new;
    }
    ಹಿಂತಿರುಗಿ ಹೊರಹೋಗು;
}

ದತ್ತಾಂಶ ತುಂಬು_categorical_ದತ್ತಾಂಶ_csv(ಅಕ್ಷರ *filename, ಪೂರ್ಣಾಂಕ ಗುರಿ, ಪೂರ್ಣಾಂಕ k)
{
    ದತ್ತಾಂಶ d = {0};
    d.shallow = 0;
    ಮಾತೃಕೆ X = csv_ಗಮ್ಯ_ಮಾತೃಕೆ(filename);
    ದಶಮಾಂಶ *truth_1d = ತೆಗೆ_ಸ್ತಂಭ(&X, ಗುರಿ);
    ದಶಮಾಂಶ **truth = one_hot_encode(truth_1d, X.ಅಡ್ಡಸಾಲುಗಳು, k);
    ಮಾತೃಕೆ y;
    y.ಅಡ್ಡಸಾಲುಗಳು = X.ಅಡ್ಡಸಾಲುಗಳು;
    y.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = k;
    y.vals = truth;
    d.X = X;
    d.y = y;
    ಬಿಡುಗಡೆ(truth_1d);
    ಹಿಂತಿರುಗಿ d;
}

ದತ್ತಾಂಶ ತುಂಬು_cifar10_ದತ್ತಾಂಶ(ಅಕ್ಷರ *filename)
{
    ದತ್ತಾಂಶ d = {0};
    d.shallow = 0;
    ದೀರ್ಘ i,j;
    ಮಾತೃಕೆ X = make_ಮಾತೃಕೆ(10000, 3072);
    ಮಾತೃಕೆ y = make_ಮಾತೃಕೆ(10000, 10);
    d.X = X;
    d.y = y;

    FILE *fp = fopen(filename, "rb");
    ಆದರೆ(!fp) file_error(filename);
    ಗಾಗಿ(i = 0; i < 10000; ++i){
        ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ ಬೈಟುಗಳು[3073];
        fread(ಬೈಟುಗಳು, 1, 3073, fp);
        ಪೂರ್ಣಾಂಕ class = ಬೈಟುಗಳು[0];
        y.vals[i][class] = 1;
        ಗಾಗಿ(j = 0; j < X.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ; ++j){
            X.vals[i][j] = (ದ್ವಿಗುಣ)ಬೈಟುಗಳು[j+1];
        }
    }
    scale_ದತ್ತಾಂಶ_ಅಡ್ಡಸಾಲುಗಳು(d, 1./255);
    //normalize_data_rows(d);
    fclose(fp);
    ಹಿಂತಿರುಗಿ d;
}

ಶೂನ್ಯ get_random_batch(ದತ್ತಾಂಶ d, ಪೂರ್ಣಾಂಕ n, ದಶಮಾಂಶ *X, ದಶಮಾಂಶ *y)
{
    ಪೂರ್ಣಾಂಕ j;
    ಗಾಗಿ(j = 0; j < n; ++j){
        ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ = rand()%d.X.ಅಡ್ಡಸಾಲುಗಳು;
        ಸ್ಮರಣೆ_ನಕಲು(X+j*d.X.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, d.X.vals[ಸೂಚ್ಯಂಕ], d.X.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ*ಗಾತ್ರ(ದಶಮಾಂಶ));
        ಸ್ಮರಣೆ_ನಕಲು(y+j*d.y.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, d.y.vals[ಸೂಚ್ಯಂಕ], d.y.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ*ಗಾತ್ರ(ದಶಮಾಂಶ));
    }
}

ಶೂನ್ಯ get_ಮುಂದಿನ_batch(ದತ್ತಾಂಶ d, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ ಸ್ಥಾನಾಂತರ, ದಶಮಾಂಶ *X, ದಶಮಾಂಶ *y)
{
    ಪೂರ್ಣಾಂಕ j;
    ಗಾಗಿ(j = 0; j < n; ++j){
        ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ = ಸ್ಥಾನಾಂತರ + j;
        ಸ್ಮರಣೆ_ನಕಲು(X+j*d.X.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, d.X.vals[ಸೂಚ್ಯಂಕ], d.X.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ*ಗಾತ್ರ(ದಶಮಾಂಶ));
        ಆದರೆ(y) ಸ್ಮರಣೆ_ನಕಲು(y+j*d.y.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, d.y.vals[ಸೂಚ್ಯಂಕ], d.y.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ*ಗಾತ್ರ(ದಶಮಾಂಶ));
    }
}

ಶೂನ್ಯ smooth_ದತ್ತಾಂಶ(ದತ್ತಾಂಶ d)
{
    ಪೂರ್ಣಾಂಕ i, j;
    ದಶಮಾಂಶ scale = 1. / d.y.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
    ದಶಮಾಂಶ eps = .1;
    ಗಾಗಿ(i = 0; i < d.y.ಅಡ್ಡಸಾಲುಗಳು; ++i){
        ಗಾಗಿ(j = 0; j < d.y.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ; ++j){
            d.y.vals[i][j] = eps * scale + (1-eps) * d.y.vals[i][j];
        }
    }
}

ದತ್ತಾಂಶ ತುಂಬು_all_cifar10()
{
    ದತ್ತಾಂಶ d = {0};
    d.shallow = 0;
    ಪೂರ್ಣಾಂಕ i,j,b;
    ಮಾತೃಕೆ X = make_ಮಾತೃಕೆ(50000, 3072);
    ಮಾತೃಕೆ y = make_ಮಾತೃಕೆ(50000, 10);
    d.X = X;
    d.y = y;


    ಗಾಗಿ(b = 0; b < 5; ++b){
        ಅಕ್ಷರ buff[256];
        sprintf(buff, "data/cifar/cifar-10-batches-bin/data_batch_%d.bin", b+1);
        FILE *fp = fopen(buff, "rb");
        ಆದರೆ(!fp) file_error(buff);
        ಗಾಗಿ(i = 0; i < 10000; ++i){
            ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ ಬೈಟುಗಳು[3073];
            fread(ಬೈಟುಗಳು, 1, 3073, fp);
            ಪೂರ್ಣಾಂಕ class = ಬೈಟುಗಳು[0];
            y.vals[i+b*10000][class] = 1;
            ಗಾಗಿ(j = 0; j < X.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ; ++j){
                X.vals[i+b*10000][j] = (ದ್ವಿಗುಣ)ಬೈಟುಗಳು[j+1];
            }
        }
        fclose(fp);
    }
    //normalize_data_rows(d);
    scale_ದತ್ತಾಂಶ_ಅಡ್ಡಸಾಲುಗಳು(d, 1./255);
    smooth_ದತ್ತಾಂಶ(d);
    ಹಿಂತಿರುಗಿ d;
}

ದತ್ತಾಂಶ ತುಂಬು_go(ಅಕ್ಷರ *filename)
{
    FILE *fp = fopen(filename, "rb");
    ಮಾತೃಕೆ X = make_ಮಾತೃಕೆ(3363059, 361);
    ಮಾತೃಕೆ y = make_ಮಾತೃಕೆ(3363059, 361);
    ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲು, ಲಂಬಸಾಲು;

    ಆದರೆ(!fp) file_error(filename);
    ಅಕ್ಷರ *label;
    ಪೂರ್ಣಾಂಕ ಎಣಿಕೆ_2 = 0;
    ಆಗ((label = fgetl(fp))){
        ಪೂರ್ಣಾಂಕ i;
        ಆದರೆ(ಎಣಿಕೆ_2 == X.ಅಡ್ಡಸಾಲುಗಳು){
            X = resize_ಮಾತೃಕೆ(X, ಎಣಿಕೆ_2*2);
            y = resize_ಮಾತೃಕೆ(y, ಎಣಿಕೆ_2*2);
        }
        ಅಕ್ಷರಸರಣಿ_ಓದು(label, "%d %d", &ಅಡ್ಡಸಾಲು, &ಲಂಬಸಾಲು);
        ಅಕ್ಷರ *board = fgetl(fp);

        ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ = ಅಡ್ಡಸಾಲು*19 + ಲಂಬಸಾಲು;
        y.vals[ಎಣಿಕೆ_2][ಸೂಚ್ಯಂಕ] = 1;

        ಗಾಗಿ(i = 0; i < 19*19; ++i){
            ದಶಮಾಂಶ val = 0;
            ಆದರೆ(board[i] == '1') val = 1;
            ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ(board[i] == '2') val = -1;
            X.vals[ಎಣಿಕೆ_2][i] = val;
        }
        ++ಎಣಿಕೆ_2;
        ಬಿಡುಗಡೆ(label);
        ಬಿಡುಗಡೆ(board);
    }
    X = resize_ಮಾತೃಕೆ(X, ಎಣಿಕೆ_2);
    y = resize_ಮಾತೃಕೆ(y, ಎಣಿಕೆ_2);

    ದತ್ತಾಂಶ d = {0};
    d.shallow = 0;
    d.X = X;
    d.y = y;


    fclose(fp);

    ಹಿಂತಿರುಗಿ d;
}


ಶೂನ್ಯ randomize_ದತ್ತಾಂಶ(ದತ್ತಾಂಶ d)
{
    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ(i = d.X.ಅಡ್ಡಸಾಲುಗಳು-1; i > 0; --i){
        ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ = rand()%i;
        ದಶಮಾಂಶ *ಅದಲುಬದಲು = d.X.vals[ಸೂಚ್ಯಂಕ];
        d.X.vals[ಸೂಚ್ಯಂಕ] = d.X.vals[i];
        d.X.vals[i] = ಅದಲುಬದಲು;

        ಅದಲುಬದಲು = d.y.vals[ಸೂಚ್ಯಂಕ];
        d.y.vals[ಸೂಚ್ಯಂಕ] = d.y.vals[i];
        d.y.vals[i] = ಅದಲುಬದಲು;
    }
}

ಶೂನ್ಯ scale_ದತ್ತಾಂಶ_ಅಡ್ಡಸಾಲುಗಳು(ದತ್ತಾಂಶ d, ದಶಮಾಂಶ s)
{
    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ(i = 0; i < d.X.ಅಡ್ಡಸಾಲುಗಳು; ++i){
        scale_array(d.X.vals[i], d.X.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, s);
    }
}

ಶೂನ್ಯ translate_ದತ್ತಾಂಶ_ಅಡ್ಡಸಾಲುಗಳು(ದತ್ತಾಂಶ d, ದಶಮಾಂಶ s)
{
    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ(i = 0; i < d.X.ಅಡ್ಡಸಾಲುಗಳು; ++i){
        translate_array(d.X.vals[i], d.X.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, s);
    }
}

ದತ್ತಾಂಶ copy_ದತ್ತಾಂಶ(ದತ್ತಾಂಶ d)
{
    ದತ್ತಾಂಶ c = {0};
    c.w = d.w;
    c.h = d.h;
    c.shallow = 0;
    c.ಅಂಕೆ_boxes = d.ಅಂಕೆ_boxes;
    c.boxes = d.boxes;
    c.X = copy_ಮಾತೃಕೆ(d.X);
    c.y = copy_ಮಾತೃಕೆ(d.y);
    ಹಿಂತಿರುಗಿ c;
}

ಶೂನ್ಯ normalize_ದತ್ತಾಂಶ_ಅಡ್ಡಸಾಲುಗಳು(ದತ್ತಾಂಶ d)
{
    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ(i = 0; i < d.X.ಅಡ್ಡಸಾಲುಗಳು; ++i){
        normalize_array(d.X.vals[i], d.X.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ);
    }
}

ದತ್ತಾಂಶ get_ದತ್ತಾಂಶ_part(ದತ್ತಾಂಶ d, ಪೂರ್ಣಾಂಕ part, ಪೂರ್ಣಾಂಕ ಒಟ್ಟು)
{
    ದತ್ತಾಂಶ p = {0};
    p.shallow = 1;
    p.X.ಅಡ್ಡಸಾಲುಗಳು = d.X.ಅಡ್ಡಸಾಲುಗಳು * (part + 1) / ಒಟ್ಟು - d.X.ಅಡ್ಡಸಾಲುಗಳು * part / ಒಟ್ಟು;
    p.y.ಅಡ್ಡಸಾಲುಗಳು = d.y.ಅಡ್ಡಸಾಲುಗಳು * (part + 1) / ಒಟ್ಟು - d.y.ಅಡ್ಡಸಾಲುಗಳು * part / ಒಟ್ಟು;
    p.X.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = d.X.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
    p.y.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = d.y.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
    p.X.vals = d.X.vals + d.X.ಅಡ್ಡಸಾಲುಗಳು * part / ಒಟ್ಟು;
    p.y.vals = d.y.vals + d.y.ಅಡ್ಡಸಾಲುಗಳು * part / ಒಟ್ಟು;
    ಹಿಂತಿರುಗಿ p;
}

ದತ್ತಾಂಶ get_random_ದತ್ತಾಂಶ(ದತ್ತಾಂಶ d, ಪೂರ್ಣಾಂಕ ಅಂಕೆ)
{
    ದತ್ತಾಂಶ r = {0};
    r.shallow = 1;

    r.X.ಅಡ್ಡಸಾಲುಗಳು = ಅಂಕೆ;
    r.y.ಅಡ್ಡಸಾಲುಗಳು = ಅಂಕೆ;

    r.X.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = d.X.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
    r.y.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = d.y.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;

    r.X.vals = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(ಅಂಕೆ, ಗಾತ್ರ(ದಶಮಾಂಶ *));
    r.y.vals = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(ಅಂಕೆ, ಗಾತ್ರ(ದಶಮಾಂಶ *));

    ಪೂರ್ಣಾಂಕ i;
    ಗಾಗಿ(i = 0; i < ಅಂಕೆ; ++i){
        ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ = rand()%d.X.ಅಡ್ಡಸಾಲುಗಳು;
        r.X.vals[i] = d.X.vals[ಸೂಚ್ಯಂಕ];
        r.y.vals[i] = d.y.vals[ಸೂಚ್ಯಂಕ];
    }
    ಹಿಂತಿರುಗಿ r;
}

ದತ್ತಾಂಶ *ವಿಭಜಿಸು_ದತ್ತಾಂಶ(ದತ್ತಾಂಶ d, ಪೂರ್ಣಾಂಕ part, ಪೂರ್ಣಾಂಕ ಒಟ್ಟು)
{
    ದತ್ತಾಂಶ *ವಿಭಜಿಸು = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(2, ಗಾತ್ರ(ದತ್ತಾಂಶ));
    ಪೂರ್ಣಾಂಕ i;
    ಪೂರ್ಣಾಂಕ ಆರಂಭ = part*d.X.ಅಡ್ಡಸಾಲುಗಳು/ಒಟ್ಟು;
    ಪೂರ್ಣಾಂಕ ಕೊನೆ = (part+1)*d.X.ಅಡ್ಡಸಾಲುಗಳು/ಒಟ್ಟು;
    ದತ್ತಾಂಶ train;
    ದತ್ತಾಂಶ test;
    train.shallow = test.shallow = 1;

    test.X.ಅಡ್ಡಸಾಲುಗಳು = test.y.ಅಡ್ಡಸಾಲುಗಳು = ಕೊನೆ-ಆರಂಭ;
    train.X.ಅಡ್ಡಸಾಲುಗಳು = train.y.ಅಡ್ಡಸಾಲುಗಳು = d.X.ಅಡ್ಡಸಾಲುಗಳು - (ಕೊನೆ-ಆರಂಭ);
    train.X.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = test.X.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = d.X.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;
    train.y.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = test.y.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ = d.y.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ;

    train.X.vals = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(train.X.ಅಡ್ಡಸಾಲುಗಳು, ಗಾತ್ರ(ದಶಮಾಂಶ*));
    test.X.vals = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(test.X.ಅಡ್ಡಸಾಲುಗಳು, ಗಾತ್ರ(ದಶಮಾಂಶ*));
    train.y.vals = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(train.y.ಅಡ್ಡಸಾಲುಗಳು, ಗಾತ್ರ(ದಶಮಾಂಶ*));
    test.y.vals = ಸೊನ್ನೆ_ಸ್ಮರಣೆ_ಕೊಡು(test.y.ಅಡ್ಡಸಾಲುಗಳು, ಗಾತ್ರ(ದಶಮಾಂಶ*));

    ಗಾಗಿ(i = 0; i < ಆರಂಭ; ++i){
        train.X.vals[i] = d.X.vals[i];
        train.y.vals[i] = d.y.vals[i];
    }
    ಗಾಗಿ(i = ಆರಂಭ; i < ಕೊನೆ; ++i){
        test.X.vals[i-ಆರಂಭ] = d.X.vals[i];
        test.y.vals[i-ಆರಂಭ] = d.y.vals[i];
    }
    ಗಾಗಿ(i = ಕೊನೆ; i < d.X.ಅಡ್ಡಸಾಲುಗಳು; ++i){
        train.X.vals[i-(ಕೊನೆ-ಆರಂಭ)] = d.X.vals[i];
        train.y.vals[i-(ಕೊನೆ-ಆರಂಭ)] = d.y.vals[i];
    }
    ವಿಭಜಿಸು[0] = train;
    ವಿಭಜಿಸು[1] = test;
    ಹಿಂತಿರುಗಿ ವಿಭಜಿಸು;
}

