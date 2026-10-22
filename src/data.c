#అన్తర్భావ "data.h"
#అన్తర్భావ "utils.h"
#అన్తర్భావ "image.h"
#అన్తర్భావ "cuda.h"

#అన్తర్భావ <stdio.h>
#అన్తర్భావ <stdlib.h>
#అన్తర్భావ <string.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

list *get_paths(అక్షరం *filename)
{
    అక్షరం *మార్గం;
    FILE *file = fopen(filename, "r");
    ఒకవేళ(!file) file_error(filename);
    list *పంక్తులు = make_list();
    అయితే((మార్గం=fgetl(file))){
        list_చొప్పించు(పంక్తులు, మార్గం);
    }
    fclose(file);
    తిరిగి పంక్తులు;
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

అక్షరం **get_random_paths(అక్షరం **paths, పూర్ణాంకం n, పూర్ణాంకం m)
{
    అక్షరం **random_paths = సున్నా_జ్ఞాపకం_ఇవ్వు(n, పరిమాణం(అక్షరం*));
    పూర్ణాంకం i;
    pthread_mutex_lock(&mutex);
    కొరకు(i = 0; i < n; ++i){
        పూర్ణాంకం సూచీ = rand()%m;
        random_paths[i] = paths[సూచీ];
        //if(i == 0) printf("%s\n", paths[index]);
    }
    pthread_mutex_unlock(&mutex);
    తిరిగి random_paths;
}

అక్షరం **కనుగొను_replace_paths(అక్షరం **paths, పూర్ణాంకం n, అక్షరం *కనుగొను, అక్షరం *replace)
{
    అక్షరం **replace_paths = సున్నా_జ్ఞాపకం_ఇవ్వు(n, పరిమాణం(అక్షరం*));
    పూర్ణాంకం i;
    కొరకు(i = 0; i < n; ++i){
        అక్షరం replaced[4096];
        కనుగొను_replace(paths[i], కనుగొను, replace, replaced);
        replace_paths[i] = copy_string(replaced);
    }
    తిరిగి replace_paths;
}

మాతృక ఎక్కించు_image_paths_బూడిద(అక్షరం **paths, పూర్ణాంకం n, పూర్ణాంకం w, పూర్ణాంకం h)
{
    పూర్ణాంకం i;
    మాతృక X;
    X.అడ్డువరుసలు = n;
    X.vals = సున్నా_జ్ఞాపకం_ఇవ్వు(X.అడ్డువరుసలు, పరిమాణం(దశాంశం*));
    X.నిలువుల_సంఖ్య = 0;

    కొరకు(i = 0; i < n; ++i){
        image im = ఎక్కించు_image(paths[i], w, h, 3);

        image బూడిద = grayscale_image(im);
        free_image(im);
        im = బూడిద;

        X.vals[i] = im.దత్తాంశం;
        X.నిలువుల_సంఖ్య = im.h*im.w*im.c;
    }
    తిరిగి X;
}

మాతృక ఎక్కించు_image_paths(అక్షరం **paths, పూర్ణాంకం n, పూర్ణాంకం w, పూర్ణాంకం h)
{
    పూర్ణాంకం i;
    మాతృక X;
    X.అడ్డువరుసలు = n;
    X.vals = సున్నా_జ్ఞాపకం_ఇవ్వు(X.అడ్డువరుసలు, పరిమాణం(దశాంశం*));
    X.నిలువుల_సంఖ్య = 0;

    కొరకు(i = 0; i < n; ++i){
        image im = ఎక్కించు_image_color(paths[i], w, h);
        X.vals[i] = im.దత్తాంశం;
        X.నిలువుల_సంఖ్య = im.h*im.w*im.c;
    }
    తిరిగి X;
}

మాతృక ఎక్కించు_image_augment_paths(అక్షరం **paths, పూర్ణాంకం n, పూర్ణాంకం min, పూర్ణాంకం max, పూర్ణాంకం కొలత, దశాంశం angle, దశాంశం aspect, దశాంశం hue, దశాంశం saturation, దశాంశం exposure, పూర్ణాంకం center)
{
    పూర్ణాంకం i;
    మాతృక X;
    X.అడ్డువరుసలు = n;
    X.vals = సున్నా_జ్ఞాపకం_ఇవ్వు(X.అడ్డువరుసలు, పరిమాణం(దశాంశం*));
    X.నిలువుల_సంఖ్య = 0;

    కొరకు(i = 0; i < n; ++i){
        image im = ఎక్కించు_image_color(paths[i], 0, 0);
        image crop;
        ఒకవేళ(center){
            crop = center_crop_image(im, కొలత, కొలత);
        } లేకపోతే {
            crop = random_augment_image(im, angle, aspect, min, max, కొలత, కొలత);
        }
        పూర్ణాంకం flip = rand()%2;
        ఒకవేళ (flip) flip_image(crop);
        random_distort_image(crop, hue, saturation, exposure);

        /*
        show_image(im, "orig");
        show_image(crop, "crop");
        cvWaitKey(0);
        */
        //grayscale_image_3c(crop);
        free_image(im);
        X.vals[i] = crop.దత్తాంశం;
        X.నిలువుల_సంఖ్య = crop.h*crop.w*crop.c;
    }
    తిరిగి X;
}


box_label *read_boxes(అక్షరం *filename, పూర్ణాంకం *n)
{
    FILE *file = fopen(filename, "r");
    ఒకవేళ(!file) file_error(filename);
    దశాంశం x, y, h, w;
    పూర్ణాంకం id;
    పూర్ణాంకం లెక్క = 0;
    పూర్ణాంకం కొలత = 64;
    box_label *boxes = సున్నా_జ్ఞాపకం_ఇవ్వు(కొలత, పరిమాణం(box_label));
    అయితే(fscanf(file, "%d %f %f %f %f", &id, &x, &y, &w, &h) == 5){
        ఒకవేళ(లెక్క == కొలత) {
            కొలత = కొలత * 2;
            boxes = realloc(boxes, కొలత*పరిమాణం(box_label));
        }
        boxes[లెక్క].id = id;
        boxes[లెక్క].x = x;
        boxes[లెక్క].y = y;
        boxes[లెక్క].h = h;
        boxes[లెక్క].w = w;
        boxes[లెక్క].ఎడమ   = x - w/2;
        boxes[లెక్క].కుడి  = x + w/2;
        boxes[లెక్క].శిఖరం    = y - h/2;
        boxes[లెక్క].bottom = y + h/2;
        ++లెక్క;
    }
    fclose(file);
    *n = లెక్క;
    తిరిగి boxes;
}

శూన్య randomize_boxes(box_label *b, పూర్ణాంకం n)
{
    పూర్ణాంకం i;
    కొరకు(i = 0; i < n; ++i){
        box_label మార్పిడి = b[i];
        పూర్ణాంకం సూచీ = rand()%n;
        b[i] = b[సూచీ];
        b[సూచీ] = మార్పిడి;
    }
}

శూన్య correct_boxes(box_label *boxes, పూర్ణాంకం n, దశాంశం dx, దశాంశం dy, దశాంశం sx, దశాంశం sy, పూర్ణాంకం flip)
{
    పూర్ణాంకం i;
    కొరకు(i = 0; i < n; ++i){
        ఒకవేళ(boxes[i].x == 0 && boxes[i].y == 0) {
            boxes[i].x = 999999;
            boxes[i].y = 999999;
            boxes[i].w = 999999;
            boxes[i].h = 999999;
            కొనసాగు;
        }
        boxes[i].ఎడమ   = boxes[i].ఎడమ  * sx - dx;
        boxes[i].కుడి  = boxes[i].కుడి * sx - dx;
        boxes[i].శిఖరం    = boxes[i].శిఖరం   * sy - dy;
        boxes[i].bottom = boxes[i].bottom* sy - dy;

        ఒకవేళ(flip){
            దశాంశం మార్పిడి = boxes[i].ఎడమ;
            boxes[i].ఎడమ = 1. - boxes[i].కుడి;
            boxes[i].కుడి = 1. - మార్పిడి;
        }

        boxes[i].ఎడమ =  constrain(0, 1, boxes[i].ఎడమ);
        boxes[i].కుడి = constrain(0, 1, boxes[i].కుడి);
        boxes[i].శిఖరం =   constrain(0, 1, boxes[i].శిఖరం);
        boxes[i].bottom =   constrain(0, 1, boxes[i].bottom);

        boxes[i].x = (boxes[i].ఎడమ+boxes[i].కుడి)/2;
        boxes[i].y = (boxes[i].శిఖరం+boxes[i].bottom)/2;
        boxes[i].w = (boxes[i].కుడి - boxes[i].ఎడమ);
        boxes[i].h = (boxes[i].bottom - boxes[i].శిఖరం);

        boxes[i].w = constrain(0, 1, boxes[i].w);
        boxes[i].h = constrain(0, 1, boxes[i].h);
    }
}

శూన్య fill_truth_swag(అక్షరం *మార్గం, దశాంశం *truth, పూర్ణాంకం classes, పూర్ణాంకం flip, దశాంశం dx, దశాంశం dy, దశాంశం sx, దశాంశం sy)
{
    అక్షరం labelpath[4096];
    కనుగొను_replace(మార్గం, "images", "labels", labelpath);
    కనుగొను_replace(labelpath, "JPEGImages", "labels", labelpath);
    కనుగొను_replace(labelpath, ".jpg", ".txt", labelpath);
    కనుగొను_replace(labelpath, ".JPG", ".txt", labelpath);
    కనుగొను_replace(labelpath, ".JPEG", ".txt", labelpath);

    పూర్ణాంకం లెక్క = 0;
    box_label *boxes = read_boxes(labelpath, &లెక్క);
    randomize_boxes(boxes, లెక్క);
    correct_boxes(boxes, లెక్క, dx, dy, sx, sy, flip);
    దశాంశం x,y,w,h;
    పూర్ణాంకం id;
    పూర్ణాంకం i;

    కొరకు (i = 0; i < లెక్క && i < 90; ++i) {
        x =  boxes[i].x;
        y =  boxes[i].y;
        w =  boxes[i].w;
        h =  boxes[i].h;
        id = boxes[i].id;

        ఒకవేళ (w < .0 || h < .0) కొనసాగు;

        పూర్ణాంకం సూచీ = (4+classes) * i;

        truth[సూచీ++] = x;
        truth[సూచీ++] = y;
        truth[సూచీ++] = w;
        truth[సూచీ++] = h;

        ఒకవేళ (id < classes) truth[సూచీ+id] = 1;
    }
    విడుదల(boxes);
}

శూన్య fill_truth_region(అక్షరం *మార్గం, దశాంశం *truth, పూర్ణాంకం classes, పూర్ణాంకం అంకె_boxes, పూర్ణాంకం flip, దశాంశం dx, దశాంశం dy, దశాంశం sx, దశాంశం sy)
{
    అక్షరం labelpath[4096];
    కనుగొను_replace(మార్గం, "images", "labels", labelpath);
    కనుగొను_replace(labelpath, "JPEGImages", "labels", labelpath);

    కనుగొను_replace(labelpath, ".jpg", ".txt", labelpath);
    కనుగొను_replace(labelpath, ".png", ".txt", labelpath);
    కనుగొను_replace(labelpath, ".JPG", ".txt", labelpath);
    కనుగొను_replace(labelpath, ".JPEG", ".txt", labelpath);
    పూర్ణాంకం లెక్క = 0;
    box_label *boxes = read_boxes(labelpath, &లెక్క);
    randomize_boxes(boxes, లెక్క);
    correct_boxes(boxes, లెక్క, dx, dy, sx, sy, flip);
    దశాంశం x,y,w,h;
    పూర్ణాంకం id;
    పూర్ణాంకం i;

    కొరకు (i = 0; i < లెక్క; ++i) {
        x =  boxes[i].x;
        y =  boxes[i].y;
        w =  boxes[i].w;
        h =  boxes[i].h;
        id = boxes[i].id;

        ఒకవేళ (w < .005 || h < .005) కొనసాగు;

        పూర్ణాంకం నిలువు = (పూర్ణాంకం)(x*అంకె_boxes);
        పూర్ణాంకం అడ్డువరుస = (పూర్ణాంకం)(y*అంకె_boxes);

        x = x*అంకె_boxes - నిలువు;
        y = y*అంకె_boxes - అడ్డువరుస;

        పూర్ణాంకం సూచీ = (నిలువు+అడ్డువరుస*అంకె_boxes)*(5+classes);
        ఒకవేళ (truth[సూచీ]) కొనసాగు;
        truth[సూచీ++] = 1;

        ఒకవేళ (id < classes) truth[సూచీ+id] = 1;
        సూచీ += classes;

        truth[సూచీ++] = x;
        truth[సూచీ++] = y;
        truth[సూచీ++] = w;
        truth[సూచీ++] = h;
    }
    విడుదల(boxes);
}

శూన్య ఎక్కించు_rle(image im, పూర్ణాంకం *rle, పూర్ణాంకం n)
{
    పూర్ణాంకం లెక్క = 0;
    పూర్ణాంకం curr = 0;
    పూర్ణాంకం i,j;
    కొరకు(i = 0; i < n; ++i){
        కొరకు(j = 0; j < rle[i]; ++j){
            im.దత్తాంశం[లెక్క++] = curr;
        }
        curr = 1 - curr;
    }
    కొరకు(; లెక్క < im.h*im.w*im.c; ++లెక్క){
        im.దత్తాంశం[లెక్క] = curr;
    }
}

శూన్య or_image(image src, image dest, పూర్ణాంకం c)
{
    పూర్ణాంకం i;
    కొరకు(i = 0; i < src.w*src.h; ++i){
        ఒకవేళ(src.దత్తాంశం[i]) dest.దత్తాంశం[dest.w*dest.h*c + i] = 1;
    }
}

శూన్య exclusive_image(image src)
{
    పూర్ణాంకం k, j, i;
    పూర్ణాంకం s = src.w*src.h;
    కొరకు(k = 0; k < src.c-1; ++k){
        కొరకు(i = 0; i < s; ++i){
            ఒకవేళ (src.దత్తాంశం[k*s + i]){
                కొరకు(j = k+1; j < src.c; ++j){
                    src.దత్తాంశం[j*s + i] = 0;
                }
            }
        }
    }
}

box bound_image(image im)
{
    పూర్ణాంకం x,y;
    పూర్ణాంకం minx = im.w;
    పూర్ణాంకం miny = im.h;
    పూర్ణాంకం maxx = 0;
    పూర్ణాంకం maxy = 0;
    కొరకు(y = 0; y < im.h; ++y){
        కొరకు(x = 0; x < im.w; ++x){
            ఒకవేళ(im.దత్తాంశం[y*im.w + x]){
                minx = (x < minx) ? x : minx;
                miny = (y < miny) ? y : miny;
                maxx = (x > maxx) ? x : maxx;
                maxy = (y > maxy) ? y : maxy;
            }
        }
    }
    box b = {minx, miny, maxx-minx + 1, maxy-miny + 1};
    //printf("%f %f %f %f\n", b.x, b.y, b.w, b.h);
    తిరిగి b;
}

శూన్య fill_truth_iseg(అక్షరం *మార్గం, పూర్ణాంకం అంకె_boxes, దశాంశం *truth, పూర్ణాంకం classes, పూర్ణాంకం w, పూర్ణాంకం h, augment_వాదనలు aug, పూర్ణాంకం flip, పూర్ణాంకం mw, పూర్ణాంకం mh)
{
    అక్షరం labelpath[4096];
    కనుగొను_replace(మార్గం, "images", "mask", labelpath);
    కనుగొను_replace(labelpath, "JPEGImages", "mask", labelpath);
    కనుగొను_replace(labelpath, ".jpg", ".txt", labelpath);
    కనుగొను_replace(labelpath, ".JPG", ".txt", labelpath);
    కనుగొను_replace(labelpath, ".JPEG", ".txt", labelpath);
    FILE *file = fopen(labelpath, "r");
    ఒకవేళ(!file) file_error(labelpath);
    అక్షరం buff[32788];
    పూర్ణాంకం id;
    పూర్ణాంకం i = 0;
    పూర్ణాంకం j;
    image part = make_image(w, h, 1);
    అయితే((fscanf(file, "%d %s", &id, buff) == 2) && i < అంకె_boxes){
        పూర్ణాంకం n = 0;
        పూర్ణాంకం *rle = read_intlist(buff, &n, 0);
        ఎక్కించు_rle(part, rle, n);
        image sized = rotate_crop_image(part, aug.rad, aug.scale, aug.w, aug.h, aug.dx, aug.dy, aug.aspect);
        ఒకవేళ(flip) flip_image(sized);

        image ముసుగు = resize_image(sized, mw, mh);
        truth[i*(mw*mh+1)] = id;
        కొరకు(j = 0; j < mw*mh; ++j){
            truth[i*(mw*mh + 1) + 1 + j] = ముసుగు.దత్తాంశం[j];
        }
        ++i;

        free_image(ముసుగు);
        free_image(sized);
        విడుదల(rle);
    }
    ఒకవేళ(i < అంకె_boxes) truth[i*(mw*mh+1)] = -1;
    fclose(file);
    free_image(part);
}

శూన్య fill_truth_ముసుగు(అక్షరం *మార్గం, పూర్ణాంకం అంకె_boxes, దశాంశం *truth, పూర్ణాంకం classes, పూర్ణాంకం w, పూర్ణాంకం h, augment_వాదనలు aug, పూర్ణాంకం flip, పూర్ణాంకం mw, పూర్ణాంకం mh)
{
    అక్షరం labelpath[4096];
    కనుగొను_replace(మార్గం, "images", "mask", labelpath);
    కనుగొను_replace(labelpath, "JPEGImages", "mask", labelpath);
    కనుగొను_replace(labelpath, ".jpg", ".txt", labelpath);
    కనుగొను_replace(labelpath, ".JPG", ".txt", labelpath);
    కనుగొను_replace(labelpath, ".JPEG", ".txt", labelpath);
    FILE *file = fopen(labelpath, "r");
    ఒకవేళ(!file) file_error(labelpath);
    అక్షరం buff[32788];
    పూర్ణాంకం id;
    పూర్ణాంకం i = 0;
    image part = make_image(w, h, 1);
    అయితే((fscanf(file, "%d %s", &id, buff) == 2) && i < అంకె_boxes){
        పూర్ణాంకం n = 0;
        పూర్ణాంకం *rle = read_intlist(buff, &n, 0);
        ఎక్కించు_rle(part, rle, n);
        image sized = rotate_crop_image(part, aug.rad, aug.scale, aug.w, aug.h, aug.dx, aug.dy, aug.aspect);
        ఒకవేళ(flip) flip_image(sized);
        box b = bound_image(sized);
        ఒకవేళ(b.w > 0){
            image crop = crop_image(sized, b.x, b.y, b.w, b.h);
            image ముసుగు = resize_image(crop, mw, mh);
            truth[i*(4 + mw*mh + 1) + 0] = (b.x + b.w/2.)/sized.w;
            truth[i*(4 + mw*mh + 1) + 1] = (b.y + b.h/2.)/sized.h;
            truth[i*(4 + mw*mh + 1) + 2] = b.w/sized.w;
            truth[i*(4 + mw*mh + 1) + 3] = b.h/sized.h;
            పూర్ణాంకం j;
            కొరకు(j = 0; j < mw*mh; ++j){
                truth[i*(4 + mw*mh + 1) + 4 + j] = ముసుగు.దత్తాంశం[j];
            }
            truth[i*(4 + mw*mh + 1) + 4 + mw*mh] = id;
            free_image(crop);
            free_image(ముసుగు);
            ++i;
        }
        free_image(sized);
        విడుదల(rle);
    }
    fclose(file);
    free_image(part);
}


శూన్య fill_truth_detection(అక్షరం *మార్గం, పూర్ణాంకం అంకె_boxes, దశాంశం *truth, పూర్ణాంకం classes, పూర్ణాంకం flip, దశాంశం dx, దశాంశం dy, దశాంశం sx, దశాంశం sy)
{
    అక్షరం labelpath[4096];
    కనుగొను_replace(మార్గం, "images", "labels", labelpath);
    కనుగొను_replace(labelpath, "JPEGImages", "labels", labelpath);

    కనుగొను_replace(labelpath, "raw", "labels", labelpath);
    కనుగొను_replace(labelpath, ".jpg", ".txt", labelpath);
    కనుగొను_replace(labelpath, ".png", ".txt", labelpath);
    కనుగొను_replace(labelpath, ".JPG", ".txt", labelpath);
    కనుగొను_replace(labelpath, ".JPEG", ".txt", labelpath);
    పూర్ణాంకం లెక్క = 0;
    box_label *boxes = read_boxes(labelpath, &లెక్క);
    randomize_boxes(boxes, లెక్క);
    correct_boxes(boxes, లెక్క, dx, dy, sx, sy, flip);
    ఒకవేళ(లెక్క > అంకె_boxes) లెక్క = అంకె_boxes;
    దశాంశం x,y,w,h;
    పూర్ణాంకం id;
    పూర్ణాంకం i;
    పూర్ణాంకం sub = 0;

    కొరకు (i = 0; i < లెక్క; ++i) {
        x =  boxes[i].x;
        y =  boxes[i].y;
        w =  boxes[i].w;
        h =  boxes[i].h;
        id = boxes[i].id;

        ఒకవేళ ((w < .001 || h < .001)) {
            ++sub;
            కొనసాగు;
        }

        truth[(i-sub)*5+0] = x;
        truth[(i-sub)*5+1] = y;
        truth[(i-sub)*5+2] = w;
        truth[(i-sub)*5+3] = h;
        truth[(i-sub)*5+4] = id;
    }
    విడుదల(boxes);
}

#నిర్వచించు NUMCHARS 37

శూన్య print_letters(దశాంశం *pred, పూర్ణాంకం n)
{
    పూర్ణాంకం i;
    కొరకు(i = 0; i < n; ++i){
        పూర్ణాంకం సూచీ = max_సూచీ(pred+i*NUMCHARS, NUMCHARS);
        రాయి("%c", int_గమ్యం_alphanum(సూచీ));
    }
    రాయి("\n");
}

శూన్య fill_truth_captcha(అక్షరం *మార్గం, పూర్ణాంకం n, దశాంశం *truth)
{
    అక్షరం *begin = strrchr(మార్గం, '/');
    ++begin;
    పూర్ణాంకం i;
    కొరకు(i = 0; i < అక్షరశ్రేణి_పొడవు(begin) && i < n && begin[i] != '.'; ++i){
        పూర్ణాంకం సూచీ = alphanum_గమ్యం_int(begin[i]);
        ఒకవేళ(సూచీ > 35) రాయి("Bad %c\n", begin[i]);
        truth[i*NUMCHARS+సూచీ] = 1;
    }
    కొరకు(;i < n; ++i){
        truth[i*NUMCHARS + NUMCHARS-1] = 1;
    }
}

దత్తాంశం ఎక్కించు_దత్తాంశం_captcha(అక్షరం **paths, పూర్ణాంకం n, పూర్ణాంకం m, పూర్ణాంకం k, పూర్ణాంకం w, పూర్ణాంకం h)
{
    ఒకవేళ(m) paths = get_random_paths(paths, n, m);
    దత్తాంశం d = {0};
    d.shallow = 0;
    d.X = ఎక్కించు_image_paths(paths, n, w, h);
    d.y = make_మాతృక(n, k*NUMCHARS);
    పూర్ణాంకం i;
    కొరకు(i = 0; i < n; ++i){
        fill_truth_captcha(paths[i], k, d.y.vals[i]);
    }
    ఒకవేళ(m) విడుదల(paths);
    తిరిగి d;
}

దత్తాంశం ఎక్కించు_దత్తాంశం_captcha_encode(అక్షరం **paths, పూర్ణాంకం n, పూర్ణాంకం m, పూర్ణాంకం w, పూర్ణాంకం h)
{
    ఒకవేళ(m) paths = get_random_paths(paths, n, m);
    దత్తాంశం d = {0};
    d.shallow = 0;
    d.X = ఎక్కించు_image_paths(paths, n, w, h);
    d.X.నిలువుల_సంఖ్య = 17100;
    d.y = d.X;
    ఒకవేళ(m) విడుదల(paths);
    తిరిగి d;
}

శూన్య fill_truth(అక్షరం *మార్గం, అక్షరం **labels, పూర్ణాంకం k, దశాంశం *truth)
{
    పూర్ణాంకం i;
    memset(truth, 0, k*పరిమాణం(దశాంశం));
    పూర్ణాంకం లెక్క = 0;
    కొరకు(i = 0; i < k; ++i){
        ఒకవేళ(అక్షరశ్రేణి_శోధన(మార్గం, labels[i])){
            truth[i] = 1;
            ++లెక్క;
            //printf("%s %s %d\n", path, labels[i], i);
        }
    }
    ఒకవేళ(లెక్క != 1 && (k != 1 || లెక్క != 0)) రాయి("Too many or too few labels: %d, %s\n", లెక్క, మార్గం);
}

శూన్య fill_hierarchy(దశాంశం *truth, పూర్ణాంకం k, వృక్షం *hierarchy)
{
    పూర్ణాంకం j;
    కొరకు(j = 0; j < k; ++j){
        ఒకవేళ(truth[j]){
            పూర్ణాంకం మాతృ = hierarchy->మాతృ[j];
            అయితే(మాతృ >= 0){
                truth[మాతృ] = 1;
                మాతృ = hierarchy->మాతృ[మాతృ];
            }
        }
    }
    పూర్ణాంకం i;
    పూర్ణాంకం లెక్క = 0;
    కొరకు(j = 0; j < hierarchy->groups; ++j){
        //printf("%d\n", count);
        పూర్ణాంకం ముసుగు = 1;
        కొరకు(i = 0; i < hierarchy->సమూహం_కొలత[j]; ++i){
            ఒకవేళ(truth[లెక్క + i]){
                ముసుగు = 0;
                ఆపు;
            }
        }
        ఒకవేళ (ముసుగు) {
            కొరకు(i = 0; i < hierarchy->సమూహం_కొలత[j]; ++i){
                truth[లెక్క + i] = SECRET_అంకె;
            }
        }
        లెక్క += hierarchy->సమూహం_కొలత[j];
    }
}

మాతృక ఎక్కించు_regression_labels_paths(అక్షరం **paths, పూర్ణాంకం n, పూర్ణాంకం k)
{
    మాతృక y = make_మాతృక(n, k);
    పూర్ణాంకం i,j;
    కొరకు(i = 0; i < n; ++i){
        అక్షరం labelpath[4096];
        కనుగొను_replace(paths[i], "images", "labels", labelpath);
        కనుగొను_replace(labelpath, "JPEGImages", "labels", labelpath);
        కనుగొను_replace(labelpath, ".BMP", ".txt", labelpath);
        కనుగొను_replace(labelpath, ".JPEG", ".txt", labelpath);
        కనుగొను_replace(labelpath, ".JPG", ".txt", labelpath);
        కనుగొను_replace(labelpath, ".JPeG", ".txt", labelpath);
        కనుగొను_replace(labelpath, ".Jpeg", ".txt", labelpath);
        కనుగొను_replace(labelpath, ".PNG", ".txt", labelpath);
        కనుగొను_replace(labelpath, ".TIF", ".txt", labelpath);
        కనుగొను_replace(labelpath, ".bmp", ".txt", labelpath);
        కనుగొను_replace(labelpath, ".jpeg", ".txt", labelpath);
        కనుగొను_replace(labelpath, ".jpg", ".txt", labelpath);
        కనుగొను_replace(labelpath, ".png", ".txt", labelpath);
        కనుగొను_replace(labelpath, ".tif", ".txt", labelpath);

        FILE *file = fopen(labelpath, "r");
        కొరకు(j = 0; j < k; ++j){
            fscanf(file, "%f", &(y.vals[i][j]));
        }
        fclose(file);
    }
    తిరిగి y;
}

మాతృక ఎక్కించు_labels_paths(అక్షరం **paths, పూర్ణాంకం n, అక్షరం **labels, పూర్ణాంకం k, వృక్షం *hierarchy)
{
    మాతృక y = make_మాతృక(n, k);
    పూర్ణాంకం i;
    కొరకు(i = 0; i < n && labels; ++i){
        fill_truth(paths[i], labels, k, y.vals[i]);
        ఒకవేళ(hierarchy){
            fill_hierarchy(y.vals[i], k, hierarchy);
        }
    }
    తిరిగి y;
}

మాతృక ఎక్కించు_ట్యాగులు_paths(అక్షరం **paths, పూర్ణాంకం n, పూర్ణాంకం k)
{
    మాతృక y = make_మాతృక(n, k);
    పూర్ణాంకం i;
    //int count = 0;
    కొరకు(i = 0; i < n; ++i){
        అక్షరం label[4096];
        కనుగొను_replace(paths[i], "images", "labels", label);
        కనుగొను_replace(label, ".jpg", ".txt", label);
        FILE *file = fopen(label, "r");
        ఒకవేళ (!file) కొనసాగు;
        //++count;
        పూర్ణాంకం ట్యాగ్;
        అయితే(fscanf(file, "%d", &ట్యాగ్) == 1){
            ఒకవేళ(ట్యాగ్ < k){
                y.vals[i][ట్యాగ్] = 1;
            }
        }
        fclose(file);
    }
    //printf("%d/%d\n", count, n);
    తిరిగి y;
}

అక్షరం **get_labels(అక్షరం *filename)
{
    list *plist = get_paths(filename);
    అక్షరం **labels = (అక్షరం **)list_గమ్యం_array(plist);
    free_list(plist);
    తిరిగి labels;
}

శూన్య free_దత్తాంశం(దత్తాంశం d)
{
    ఒకవేళ(!d.shallow){
        free_మాతృక(d.X);
        free_మాతృక(d.y);
    }లేకపోతే{
        విడుదల(d.X.vals);
        విడుదల(d.y.vals);
    }
}

image get_segmentation_image(అక్షరం *మార్గం, పూర్ణాంకం w, పూర్ణాంకం h, పూర్ణాంకం classes)
{
    అక్షరం labelpath[4096];
    కనుగొను_replace(మార్గం, "images", "mask", labelpath);
    కనుగొను_replace(labelpath, "JPEGImages", "mask", labelpath);
    కనుగొను_replace(labelpath, ".jpg", ".txt", labelpath);
    కనుగొను_replace(labelpath, ".JPG", ".txt", labelpath);
    కనుగొను_replace(labelpath, ".JPEG", ".txt", labelpath);
    image ముసుగు = make_image(w, h, classes);
    FILE *file = fopen(labelpath, "r");
    ఒకవేళ(!file) file_error(labelpath);
    అక్షరం buff[32788];
    పూర్ణాంకం id;
    image part = make_image(w, h, 1);
    అయితే(fscanf(file, "%d %s", &id, buff) == 2){
        పూర్ణాంకం n = 0;
        పూర్ణాంకం *rle = read_intlist(buff, &n, 0);
        ఎక్కించు_rle(part, rle, n);
        or_image(part, ముసుగు, id);
        విడుదల(rle);
    }
    //exclusive_image(mask);
    fclose(file);
    free_image(part);
    తిరిగి ముసుగు;
}

image get_segmentation_image2(అక్షరం *మార్గం, పూర్ణాంకం w, పూర్ణాంకం h, పూర్ణాంకం classes)
{
    అక్షరం labelpath[4096];
    కనుగొను_replace(మార్గం, "images", "mask", labelpath);
    కనుగొను_replace(labelpath, "JPEGImages", "mask", labelpath);
    కనుగొను_replace(labelpath, ".jpg", ".txt", labelpath);
    కనుగొను_replace(labelpath, ".JPG", ".txt", labelpath);
    కనుగొను_replace(labelpath, ".JPEG", ".txt", labelpath);
    image ముసుగు = make_image(w, h, classes+1);
    పూర్ణాంకం i;
    కొరకు(i = 0; i < w*h; ++i){
        ముసుగు.దత్తాంశం[w*h*classes + i] = 1;
    }
    FILE *file = fopen(labelpath, "r");
    ఒకవేళ(!file) file_error(labelpath);
    అక్షరం buff[32788];
    పూర్ణాంకం id;
    image part = make_image(w, h, 1);
    అయితే(fscanf(file, "%d %s", &id, buff) == 2){
        పూర్ణాంకం n = 0;
        పూర్ణాంకం *rle = read_intlist(buff, &n, 0);
        ఎక్కించు_rle(part, rle, n);
        or_image(part, ముసుగు, id);
        కొరకు(i = 0; i < w*h; ++i){
            ఒకవేళ(part.దత్తాంశం[i]) ముసుగు.దత్తాంశం[w*h*classes + i] = 0;
        }
        విడుదల(rle);
    }
    //exclusive_image(mask);
    fclose(file);
    free_image(part);
    తిరిగి ముసుగు;
}

దత్తాంశం ఎక్కించు_దత్తాంశం_seg(పూర్ణాంకం n, అక్షరం **paths, పూర్ణాంకం m, పూర్ణాంకం w, పూర్ణాంకం h, పూర్ణాంకం classes, పూర్ణాంకం min, పూర్ణాంకం max, దశాంశం angle, దశాంశం aspect, దశాంశం hue, దశాంశం saturation, దశాంశం exposure, పూర్ణాంకం div)
{
    అక్షరం **random_paths = get_random_paths(paths, n, m);
    పూర్ణాంకం i;
    దత్తాంశం d = {0};
    d.shallow = 0;

    d.X.అడ్డువరుసలు = n;
    d.X.vals = సున్నా_జ్ఞాపకం_ఇవ్వు(d.X.అడ్డువరుసలు, పరిమాణం(దశాంశం*));
    d.X.నిలువుల_సంఖ్య = h*w*3;


    d.y.అడ్డువరుసలు = n;
    d.y.నిలువుల_సంఖ్య = h*w*classes/div/div;
    d.y.vals = సున్నా_జ్ఞాపకం_ఇవ్వు(d.X.అడ్డువరుసలు, పరిమాణం(దశాంశం*));

    కొరకు(i = 0; i < n; ++i){
        image orig = ఎక్కించు_image_color(random_paths[i], 0, 0);
        augment_వాదనలు a = random_augment_వాదనలు(orig, angle, aspect, min, max, w, h);
        image sized = rotate_crop_image(orig, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);

        పూర్ణాంకం flip = rand()%2;
        ఒకవేళ(flip) flip_image(sized);
        random_distort_image(sized, hue, saturation, exposure);
        d.X.vals[i] = sized.దత్తాంశం;

        image ముసుగు = get_segmentation_image(random_paths[i], orig.w, orig.h, classes);
        //image mask = make_image(orig.w, orig.h, classes+1);
        image sized_m = rotate_crop_image(ముసుగు, a.rad, a.scale/div, a.w/div, a.h/div, a.dx/div, a.dy/div, a.aspect);

        ఒకవేళ(flip) flip_image(sized_m);
        d.y.vals[i] = sized_m.దత్తాంశం;

        free_image(orig);
        free_image(ముసుగు);

        /*
           image rgb = mask_to_rgb(sized_m, classes);
           show_image(rgb, "part");
           show_image(sized, "orig");
           cvWaitKey(0);
           free_image(rgb);
         */
    }
    విడుదల(random_paths);
    తిరిగి d;
}

దత్తాంశం ఎక్కించు_దత్తాంశం_iseg(పూర్ణాంకం n, అక్షరం **paths, పూర్ణాంకం m, పూర్ణాంకం w, పూర్ణాంకం h, పూర్ణాంకం classes, పూర్ణాంకం boxes, పూర్ణాంకం div, పూర్ణాంకం min, పూర్ణాంకం max, దశాంశం angle, దశాంశం aspect, దశాంశం hue, దశాంశం saturation, దశాంశం exposure)
{
    అక్షరం **random_paths = get_random_paths(paths, n, m);
    పూర్ణాంకం i;
    దత్తాంశం d = {0};
    d.shallow = 0;

    d.X.అడ్డువరుసలు = n;
    d.X.vals = సున్నా_జ్ఞాపకం_ఇవ్వు(d.X.అడ్డువరుసలు, పరిమాణం(దశాంశం*));
    d.X.నిలువుల_సంఖ్య = h*w*3;

    d.y = make_మాతృక(n, (((w/div)*(h/div))+1)*boxes);

    కొరకు(i = 0; i < n; ++i){
        image orig = ఎక్కించు_image_color(random_paths[i], 0, 0);
        augment_వాదనలు a = random_augment_వాదనలు(orig, angle, aspect, min, max, w, h);
        image sized = rotate_crop_image(orig, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);

        పూర్ణాంకం flip = rand()%2;
        ఒకవేళ(flip) flip_image(sized);
        random_distort_image(sized, hue, saturation, exposure);
        d.X.vals[i] = sized.దత్తాంశం;
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
    విడుదల(random_paths);
    తిరిగి d;
}

దత్తాంశం ఎక్కించు_దత్తాంశం_ముసుగు(పూర్ణాంకం n, అక్షరం **paths, పూర్ణాంకం m, పూర్ణాంకం w, పూర్ణాంకం h, పూర్ణాంకం classes, పూర్ణాంకం boxes, పూర్ణాంకం coords, పూర్ణాంకం min, పూర్ణాంకం max, దశాంశం angle, దశాంశం aspect, దశాంశం hue, దశాంశం saturation, దశాంశం exposure)
{
    అక్షరం **random_paths = get_random_paths(paths, n, m);
    పూర్ణాంకం i;
    దత్తాంశం d = {0};
    d.shallow = 0;

    d.X.అడ్డువరుసలు = n;
    d.X.vals = సున్నా_జ్ఞాపకం_ఇవ్వు(d.X.అడ్డువరుసలు, పరిమాణం(దశాంశం*));
    d.X.నిలువుల_సంఖ్య = h*w*3;

    d.y = make_మాతృక(n, (coords+1)*boxes);

    కొరకు(i = 0; i < n; ++i){
        image orig = ఎక్కించు_image_color(random_paths[i], 0, 0);
        augment_వాదనలు a = random_augment_వాదనలు(orig, angle, aspect, min, max, w, h);
        image sized = rotate_crop_image(orig, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);

        పూర్ణాంకం flip = rand()%2;
        ఒకవేళ(flip) flip_image(sized);
        random_distort_image(sized, hue, saturation, exposure);
        d.X.vals[i] = sized.దత్తాంశం;
        //show_image(sized, "image");

        fill_truth_ముసుగు(random_paths[i], boxes, d.y.vals[i], classes, orig.w, orig.h, a, flip, 14, 14);

        free_image(orig);

        /*
           image rgb = mask_to_rgb(sized_m, classes);
           show_image(rgb, "part");
           show_image(sized, "orig");
           cvWaitKey(0);
           free_image(rgb);
         */
    }
    విడుదల(random_paths);
    తిరిగి d;
}

దత్తాంశం ఎక్కించు_దత్తాంశం_region(పూర్ణాంకం n, అక్షరం **paths, పూర్ణాంకం m, పూర్ణాంకం w, పూర్ణాంకం h, పూర్ణాంకం కొలత, పూర్ణాంకం classes, దశాంశం jitter, దశాంశం hue, దశాంశం saturation, దశాంశం exposure)
{
    అక్షరం **random_paths = get_random_paths(paths, n, m);
    పూర్ణాంకం i;
    దత్తాంశం d = {0};
    d.shallow = 0;

    d.X.అడ్డువరుసలు = n;
    d.X.vals = సున్నా_జ్ఞాపకం_ఇవ్వు(d.X.అడ్డువరుసలు, పరిమాణం(దశాంశం*));
    d.X.నిలువుల_సంఖ్య = h*w*3;


    పూర్ణాంకం k = కొలత*కొలత*(5+classes);
    d.y = make_మాతృక(n, k);
    కొరకు(i = 0; i < n; ++i){
        image orig = ఎక్కించు_image_color(random_paths[i], 0, 0);

        పూర్ణాంకం oh = orig.h;
        పూర్ణాంకం ow = orig.w;

        పూర్ణాంకం dw = (ow*jitter);
        పూర్ణాంకం dh = (oh*jitter);

        పూర్ణాంకం pleft  = rand_ఏకరీతి(-dw, dw);
        పూర్ణాంకం pright = rand_ఏకరీతి(-dw, dw);
        పూర్ణాంకం ptop   = rand_ఏకరీతి(-dh, dh);
        పూర్ణాంకం pbot   = rand_ఏకరీతి(-dh, dh);

        పూర్ణాంకం swidth =  ow - pleft - pright;
        పూర్ణాంకం sheight = oh - ptop - pbot;

        దశాంశం sx = (దశాంశం)swidth  / ow;
        దశాంశం sy = (దశాంశం)sheight / oh;

        పూర్ణాంకం flip = rand()%2;
        image cropped = crop_image(orig, pleft, ptop, swidth, sheight);

        దశాంశం dx = ((దశాంశం)pleft/ow)/sx;
        దశాంశం dy = ((దశాంశం)ptop /oh)/sy;

        image sized = resize_image(cropped, w, h);
        ఒకవేళ(flip) flip_image(sized);
        random_distort_image(sized, hue, saturation, exposure);
        d.X.vals[i] = sized.దత్తాంశం;

        fill_truth_region(random_paths[i], d.y.vals[i], classes, కొలత, flip, dx, dy, 1./sx, 1./sy);

        free_image(orig);
        free_image(cropped);
    }
    విడుదల(random_paths);
    తిరిగి d;
}

దత్తాంశం ఎక్కించు_దత్తాంశం_compare(పూర్ణాంకం n, అక్షరం **paths, పూర్ణాంకం m, పూర్ణాంకం classes, పూర్ణాంకం w, పూర్ణాంకం h)
{
    ఒకవేళ(m) paths = get_random_paths(paths, 2*n, m);
    పూర్ణాంకం i,j;
    దత్తాంశం d = {0};
    d.shallow = 0;

    d.X.అడ్డువరుసలు = n;
    d.X.vals = సున్నా_జ్ఞాపకం_ఇవ్వు(d.X.అడ్డువరుసలు, పరిమాణం(దశాంశం*));
    d.X.నిలువుల_సంఖ్య = h*w*6;

    పూర్ణాంకం k = 2*(classes);
    d.y = make_మాతృక(n, k);
    కొరకు(i = 0; i < n; ++i){
        image im1 = ఎక్కించు_image_color(paths[i*2],   w, h);
        image im2 = ఎక్కించు_image_color(paths[i*2+1], w, h);

        d.X.vals[i] = సున్నా_జ్ఞాపకం_ఇవ్వు(d.X.నిలువుల_సంఖ్య, పరిమాణం(దశాంశం));
        జ్ఞాపకం_నకలు(d.X.vals[i],         im1.దత్తాంశం, h*w*3*పరిమాణం(దశాంశం));
        జ్ఞాపకం_నకలు(d.X.vals[i] + h*w*3, im2.దత్తాంశం, h*w*3*పరిమాణం(దశాంశం));

        పూర్ణాంకం id;
        దశాంశం iou;

        అక్షరం imlabel1[4096];
        అక్షరం imlabel2[4096];
        కనుగొను_replace(paths[i*2],   "imgs", "labels", imlabel1);
        కనుగొను_replace(imlabel1, "jpg", "txt", imlabel1);
        FILE *fp1 = fopen(imlabel1, "r");

        అయితే(fscanf(fp1, "%d %f", &id, &iou) == 2){
            ఒకవేళ (d.y.vals[i][2*id] < iou) d.y.vals[i][2*id] = iou;
        }

        కనుగొను_replace(paths[i*2+1], "imgs", "labels", imlabel2);
        కనుగొను_replace(imlabel2, "jpg", "txt", imlabel2);
        FILE *fp2 = fopen(imlabel2, "r");

        అయితే(fscanf(fp2, "%d %f", &id, &iou) == 2){
            ఒకవేళ (d.y.vals[i][2*id + 1] < iou) d.y.vals[i][2*id + 1] = iou;
        }

        కొరకు (j = 0; j < classes; ++j){
            ఒకవేళ (d.y.vals[i][2*j] > .5 &&  d.y.vals[i][2*j+1] < .5){
                d.y.vals[i][2*j] = 1;
                d.y.vals[i][2*j+1] = 0;
            } లేకపోతే ఒకవేళ (d.y.vals[i][2*j] < .5 &&  d.y.vals[i][2*j+1] > .5){
                d.y.vals[i][2*j] = 0;
                d.y.vals[i][2*j+1] = 1;
            } లేకపోతే {
                d.y.vals[i][2*j]   = SECRET_అంకె;
                d.y.vals[i][2*j+1] = SECRET_అంకె;
            }
        }
        fclose(fp1);
        fclose(fp2);

        free_image(im1);
        free_image(im2);
    }
    ఒకవేళ(m) విడుదల(paths);
    తిరిగి d;
}

దత్తాంశం ఎక్కించు_దత్తాంశం_swag(అక్షరం **paths, పూర్ణాంకం n, పూర్ణాంకం classes, దశాంశం jitter)
{
    పూర్ణాంకం సూచీ = rand()%n;
    అక్షరం *random_మార్గం = paths[సూచీ];

    image orig = ఎక్కించు_image_color(random_మార్గం, 0, 0);
    పూర్ణాంకం h = orig.h;
    పూర్ణాంకం w = orig.w;

    దత్తాంశం d = {0};
    d.shallow = 0;
    d.w = w;
    d.h = h;

    d.X.అడ్డువరుసలు = 1;
    d.X.vals = సున్నా_జ్ఞాపకం_ఇవ్వు(d.X.అడ్డువరుసలు, పరిమాణం(దశాంశం*));
    d.X.నిలువుల_సంఖ్య = h*w*3;

    పూర్ణాంకం k = (4+classes)*90;
    d.y = make_మాతృక(1, k);

    పూర్ణాంకం dw = w*jitter;
    పూర్ణాంకం dh = h*jitter;

    పూర్ణాంకం pleft  = rand_ఏకరీతి(-dw, dw);
    పూర్ణాంకం pright = rand_ఏకరీతి(-dw, dw);
    పూర్ణాంకం ptop   = rand_ఏకరీతి(-dh, dh);
    పూర్ణాంకం pbot   = rand_ఏకరీతి(-dh, dh);

    పూర్ణాంకం swidth =  w - pleft - pright;
    పూర్ణాంకం sheight = h - ptop - pbot;

    దశాంశం sx = (దశాంశం)swidth  / w;
    దశాంశం sy = (దశాంశం)sheight / h;

    పూర్ణాంకం flip = rand()%2;
    image cropped = crop_image(orig, pleft, ptop, swidth, sheight);

    దశాంశం dx = ((దశాంశం)pleft/w)/sx;
    దశాంశం dy = ((దశాంశం)ptop /h)/sy;

    image sized = resize_image(cropped, w, h);
    ఒకవేళ(flip) flip_image(sized);
    d.X.vals[0] = sized.దత్తాంశం;

    fill_truth_swag(random_మార్గం, d.y.vals[0], classes, flip, dx, dy, 1./sx, 1./sy);

    free_image(orig);
    free_image(cropped);

    తిరిగి d;
}

దత్తాంశం ఎక్కించు_దత్తాంశం_detection(పూర్ణాంకం n, అక్షరం **paths, పూర్ణాంకం m, పూర్ణాంకం w, పూర్ణాంకం h, పూర్ణాంకం boxes, పూర్ణాంకం classes, దశాంశం jitter, దశాంశం hue, దశాంశం saturation, దశాంశం exposure)
{
    అక్షరం **random_paths = get_random_paths(paths, n, m);
    పూర్ణాంకం i;
    దత్తాంశం d = {0};
    d.shallow = 0;

    d.X.అడ్డువరుసలు = n;
    d.X.vals = సున్నా_జ్ఞాపకం_ఇవ్వు(d.X.అడ్డువరుసలు, పరిమాణం(దశాంశం*));
    d.X.నిలువుల_సంఖ్య = h*w*3;

    d.y = make_మాతృక(n, 5*boxes);
    కొరకు(i = 0; i < n; ++i){
        image orig = ఎక్కించు_image_color(random_paths[i], 0, 0);
        image sized = make_image(w, h, orig.c);
        fill_image(sized, .5);

        దశాంశం dw = jitter * orig.w;
        దశాంశం dh = jitter * orig.h;

        దశాంశం new_ar = (orig.w + rand_ఏకరీతి(-dw, dw)) / (orig.h + rand_ఏకరీతి(-dh, dh));
        //float scale = rand_uniform(.25, 2);
        దశాంశం scale = 1;

        దశాంశం nw, nh;

        ఒకవేళ(new_ar < 1){
            nh = scale * h;
            nw = nh * new_ar;
        } లేకపోతే {
            nw = scale * w;
            nh = nw / new_ar;
        }

        దశాంశం dx = rand_ఏకరీతి(0, w - nw);
        దశాంశం dy = rand_ఏకరీతి(0, h - nh);

        ఉంచు_image(orig, nw, nh, dx, dy, sized);

        random_distort_image(sized, hue, saturation, exposure);

        పూర్ణాంకం flip = rand()%2;
        ఒకవేళ(flip) flip_image(sized);
        d.X.vals[i] = sized.దత్తాంశం;


        fill_truth_detection(random_paths[i], boxes, d.y.vals[i], classes, flip, -dx/w, -dy/h, nw/w, nh/h);

        free_image(orig);
    }
    విడుదల(random_paths);
    తిరిగి d;
}

శూన్య *ఎక్కించు_thread(శూన్య *ptr)
{
    //printf("Loading data: %d\n", rand());
    ఎక్కించు_వాదనలు a = *(నిర్మాణం ఎక్కించు_వాదనలు*)ptr;
    ఒకవేళ(a.exposure == 0) a.exposure = 1;
    ఒకవేళ(a.saturation == 0) a.saturation = 1;
    ఒకవేళ(a.aspect == 0) a.aspect = 1;

    ఒకవేళ (a.type == OLD_CLASSIFICATION_దత్తాంశం){
        *a.d = ఎక్కించు_దత్తాంశం_old(a.paths, a.n, a.m, a.labels, a.classes, a.w, a.h);
    } లేకపోతే ఒకవేళ (a.type == REGRESSION_దత్తాంశం){
        *a.d = ఎక్కించు_దత్తాంశం_regression(a.paths, a.n, a.m, a.classes, a.min, a.max, a.కొలత, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    } లేకపోతే ఒకవేళ (a.type == CLASSIFICATION_దత్తాంశం){
        *a.d = ఎక్కించు_దత్తాంశం_augment(a.paths, a.n, a.m, a.labels, a.classes, a.hierarchy, a.min, a.max, a.కొలత, a.angle, a.aspect, a.hue, a.saturation, a.exposure, a.center);
    } లేకపోతే ఒకవేళ (a.type == SUPER_దత్తాంశం){
        *a.d = ఎక్కించు_దత్తాంశం_super(a.paths, a.n, a.m, a.w, a.h, a.scale);
    } లేకపోతే ఒకవేళ (a.type == WRITING_దత్తాంశం){
        *a.d = ఎక్కించు_దత్తాంశం_writing(a.paths, a.n, a.m, a.w, a.h, a.బహిర్గతం_w, a.బహిర్గతం_h);
    } లేకపోతే ఒకవేళ (a.type == ISEG_దత్తాంశం){
        *a.d = ఎక్కించు_దత్తాంశం_iseg(a.n, a.paths, a.m, a.w, a.h, a.classes, a.అంకె_boxes, a.scale, a.min, a.max, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    } లేకపోతే ఒకవేళ (a.type == INSTANCE_దత్తాంశం){
        *a.d = ఎక్కించు_దత్తాంశం_ముసుగు(a.n, a.paths, a.m, a.w, a.h, a.classes, a.అంకె_boxes, a.coords, a.min, a.max, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    } లేకపోతే ఒకవేళ (a.type == SEGMENTATION_దత్తాంశం){
        *a.d = ఎక్కించు_దత్తాంశం_seg(a.n, a.paths, a.m, a.w, a.h, a.classes, a.min, a.max, a.angle, a.aspect, a.hue, a.saturation, a.exposure, a.scale);
    } లేకపోతే ఒకవేళ (a.type == REGION_దత్తాంశం){
        *a.d = ఎక్కించు_దత్తాంశం_region(a.n, a.paths, a.m, a.w, a.h, a.అంకె_boxes, a.classes, a.jitter, a.hue, a.saturation, a.exposure);
    } లేకపోతే ఒకవేళ (a.type == DETECTION_దత్తాంశం){
        *a.d = ఎక్కించు_దత్తాంశం_detection(a.n, a.paths, a.m, a.w, a.h, a.అంకె_boxes, a.classes, a.jitter, a.hue, a.saturation, a.exposure);
    } లేకపోతే ఒకవేళ (a.type == SWAG_దత్తాంశం){
        *a.d = ఎక్కించు_దత్తాంశం_swag(a.paths, a.n, a.classes, a.jitter);
    } లేకపోతే ఒకవేళ (a.type == COMPARE_దత్తాంశం){
        *a.d = ఎక్కించు_దత్తాంశం_compare(a.n, a.paths, a.m, a.classes, a.w, a.h);
    } లేకపోతే ఒకవేళ (a.type == IMAGE_దత్తాంశం){
        *(a.im) = ఎక్కించు_image_color(a.మార్గం, 0, 0);
        *(a.resized) = resize_image(*(a.im), a.w, a.h);
    } లేకపోతే ఒకవేళ (a.type == LETTERBOX_దత్తాంశం){
        *(a.im) = ఎక్కించు_image_color(a.మార్గం, 0, 0);
        *(a.resized) = letterbox_image(*(a.im), a.w, a.h);
    } లేకపోతే ఒకవేళ (a.type == ట్యాగ్_దత్తాంశం){
        *a.d = ఎక్కించు_దత్తాంశం_ట్యాగ్(a.paths, a.n, a.m, a.classes, a.min, a.max, a.కొలత, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    }
    విడుదల(ptr);
    తిరిగి 0;
}

pthread_t ఎక్కించు_దత్తాంశం_లోపల_thread(ఎక్కించు_వాదనలు వాదనలు)
{
    pthread_t thread;
    నిర్మాణం ఎక్కించు_వాదనలు *ptr = సున్నా_జ్ఞాపకం_ఇవ్వు(1, పరిమాణం(నిర్మాణం ఎక్కించు_వాదనలు));
    *ptr = వాదనలు;
    ఒకవేళ(pthread_create(&thread, 0, ఎక్కించు_thread, ptr)) error("Thread creation failed");
    తిరిగి thread;
}

శూన్య *ఎక్కించు_దారాలు(శూన్య *ptr)
{
    పూర్ణాంకం i;
    ఎక్కించు_వాదనలు వాదనలు = *(ఎక్కించు_వాదనలు *)ptr;
    ఒకవేళ (వాదనలు.దారాలు == 0) వాదనలు.దారాలు = 1;
    దత్తాంశం *బహిర్గతం = వాదనలు.d;
    పూర్ణాంకం మొత్తం = వాదనలు.n;
    విడుదల(ptr);
    దత్తాంశం *buffers = సున్నా_జ్ఞాపకం_ఇవ్వు(వాదనలు.దారాలు, పరిమాణం(దత్తాంశం));
    pthread_t *దారాలు = సున్నా_జ్ఞాపకం_ఇవ్వు(వాదనలు.దారాలు, పరిమాణం(pthread_t));
    కొరకు(i = 0; i < వాదనలు.దారాలు; ++i){
        వాదనలు.d = buffers + i;
        వాదనలు.n = (i+1) * మొత్తం/వాదనలు.దారాలు - i * మొత్తం/వాదనలు.దారాలు;
        దారాలు[i] = ఎక్కించు_దత్తాంశం_లోపల_thread(వాదనలు);
    }
    కొరకు(i = 0; i < వాదనలు.దారాలు; ++i){
        pthread_join(దారాలు[i], 0);
    }
    *బహిర్గతం = concat_datas(buffers, వాదనలు.దారాలు);
    బహిర్గతం->shallow = 0;
    కొరకు(i = 0; i < వాదనలు.దారాలు; ++i){
        buffers[i].shallow = 1;
        free_దత్తాంశం(buffers[i]);
    }
    విడుదల(buffers);
    విడుదల(దారాలు);
    తిరిగి 0;
}

శూన్య ఎక్కించు_దత్తాంశం_blocking(ఎక్కించు_వాదనలు వాదనలు)
{
    నిర్మాణం ఎక్కించు_వాదనలు *ptr = సున్నా_జ్ఞాపకం_ఇవ్వు(1, పరిమాణం(నిర్మాణం ఎక్కించు_వాదనలు));
    *ptr = వాదనలు;
    ఎక్కించు_thread(ptr);
}

pthread_t ఎక్కించు_దత్తాంశం(ఎక్కించు_వాదనలు వాదనలు)
{
    pthread_t thread;
    నిర్మాణం ఎక్కించు_వాదనలు *ptr = సున్నా_జ్ఞాపకం_ఇవ్వు(1, పరిమాణం(నిర్మాణం ఎక్కించు_వాదనలు));
    *ptr = వాదనలు;
    ఒకవేళ(pthread_create(&thread, 0, ఎక్కించు_దారాలు, ptr)) error("Thread creation failed");
    తిరిగి thread;
}

దత్తాంశం ఎక్కించు_దత్తాంశం_writing(అక్షరం **paths, పూర్ణాంకం n, పూర్ణాంకం m, పూర్ణాంకం w, పూర్ణాంకం h, పూర్ణాంకం బహిర్గతం_w, పూర్ణాంకం బహిర్గతం_h)
{
    ఒకవేళ(m) paths = get_random_paths(paths, n, m);
    అక్షరం **replace_paths = కనుగొను_replace_paths(paths, n, ".png", "-label.png");
    దత్తాంశం d = {0};
    d.shallow = 0;
    d.X = ఎక్కించు_image_paths(paths, n, w, h);
    d.y = ఎక్కించు_image_paths_బూడిద(replace_paths, n, బహిర్గతం_w, బహిర్గతం_h);
    ఒకవేళ(m) విడుదల(paths);
    పూర్ణాంకం i;
    కొరకు(i = 0; i < n; ++i) విడుదల(replace_paths[i]);
    విడుదల(replace_paths);
    తిరిగి d;
}

దత్తాంశం ఎక్కించు_దత్తాంశం_old(అక్షరం **paths, పూర్ణాంకం n, పూర్ణాంకం m, అక్షరం **labels, పూర్ణాంకం k, పూర్ణాంకం w, పూర్ణాంకం h)
{
    ఒకవేళ(m) paths = get_random_paths(paths, n, m);
    దత్తాంశం d = {0};
    d.shallow = 0;
    d.X = ఎక్కించు_image_paths(paths, n, w, h);
    d.y = ఎక్కించు_labels_paths(paths, n, labels, k, 0);
    ఒకవేళ(m) విడుదల(paths);
    తిరిగి d;
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

దత్తాంశం ఎక్కించు_దత్తాంశం_super(అక్షరం **paths, పూర్ణాంకం n, పూర్ణాంకం m, పూర్ణాంకం w, పూర్ణాంకం h, పూర్ణాంకం scale)
{
    ఒకవేళ(m) paths = get_random_paths(paths, n, m);
    దత్తాంశం d = {0};
    d.shallow = 0;

    పూర్ణాంకం i;
    d.X.అడ్డువరుసలు = n;
    d.X.vals = సున్నా_జ్ఞాపకం_ఇవ్వు(n, పరిమాణం(దశాంశం*));
    d.X.నిలువుల_సంఖ్య = w*h*3;

    d.y.అడ్డువరుసలు = n;
    d.y.vals = సున్నా_జ్ఞాపకం_ఇవ్వు(n, పరిమాణం(దశాంశం*));
    d.y.నిలువుల_సంఖ్య = w*scale * h*scale * 3;

    కొరకు(i = 0; i < n; ++i){
        image im = ఎక్కించు_image_color(paths[i], 0, 0);
        image crop = random_crop_image(im, w*scale, h*scale);
        పూర్ణాంకం flip = rand()%2;
        ఒకవేళ (flip) flip_image(crop);
        image resize = resize_image(crop, w, h);
        d.X.vals[i] = resize.దత్తాంశం;
        d.y.vals[i] = crop.దత్తాంశం;
        free_image(im);
    }

    ఒకవేళ(m) విడుదల(paths);
    తిరిగి d;
}

దత్తాంశం ఎక్కించు_దత్తాంశం_regression(అక్షరం **paths, పూర్ణాంకం n, పూర్ణాంకం m, పూర్ణాంకం k, పూర్ణాంకం min, పూర్ణాంకం max, పూర్ణాంకం కొలత, దశాంశం angle, దశాంశం aspect, దశాంశం hue, దశాంశం saturation, దశాంశం exposure)
{
    ఒకవేళ(m) paths = get_random_paths(paths, n, m);
    దత్తాంశం d = {0};
    d.shallow = 0;
    d.X = ఎక్కించు_image_augment_paths(paths, n, min, max, కొలత, angle, aspect, hue, saturation, exposure, 0);
    d.y = ఎక్కించు_regression_labels_paths(paths, n, k);
    ఒకవేళ(m) విడుదల(paths);
    తిరిగి d;
}

దత్తాంశం select_దత్తాంశం(దత్తాంశం *orig, పూర్ణాంకం *inds)
{
    దత్తాంశం d = {0};
    d.shallow = 1;
    d.w = orig[0].w;
    d.h = orig[0].h;

    d.X.అడ్డువరుసలు = orig[0].X.అడ్డువరుసలు;
    d.y.అడ్డువరుసలు = orig[0].X.అడ్డువరుసలు;

    d.X.నిలువుల_సంఖ్య = orig[0].X.నిలువుల_సంఖ్య;
    d.y.నిలువుల_సంఖ్య = orig[0].y.నిలువుల_సంఖ్య;

    d.X.vals = సున్నా_జ్ఞాపకం_ఇవ్వు(orig[0].X.అడ్డువరుసలు, పరిమాణం(దశాంశం *));
    d.y.vals = సున్నా_జ్ఞాపకం_ఇవ్వు(orig[0].y.అడ్డువరుసలు, పరిమాణం(దశాంశం *));
    పూర్ణాంకం i;
    కొరకు(i = 0; i < d.X.అడ్డువరుసలు; ++i){
        d.X.vals[i] = orig[inds[i]].X.vals[i];
        d.y.vals[i] = orig[inds[i]].y.vals[i];
    }
    తిరిగి d;
}

దత్తాంశం *tile_దత్తాంశం(దత్తాంశం orig, పూర్ణాంకం divs, పూర్ణాంకం కొలత)
{
    దత్తాంశం *ds = సున్నా_జ్ఞాపకం_ఇవ్వు(divs*divs, పరిమాణం(దత్తాంశం));
    పూర్ణాంకం i, j;
#సూచన omp parallel కొరకు
    కొరకు(i = 0; i < divs*divs; ++i){
        దత్తాంశం d;
        d.shallow = 0;
        d.w = orig.w/divs * కొలత;
        d.h = orig.h/divs * కొలత;
        d.X.అడ్డువరుసలు = orig.X.అడ్డువరుసలు;
        d.X.నిలువుల_సంఖ్య = d.w*d.h*3;
        d.X.vals = సున్నా_జ్ఞాపకం_ఇవ్వు(d.X.అడ్డువరుసలు, పరిమాణం(దశాంశం*));

        d.y = copy_మాతృక(orig.y);
#సూచన omp parallel కొరకు
        కొరకు(j = 0; j < orig.X.అడ్డువరుసలు; ++j){
            పూర్ణాంకం x = (i%divs) * orig.w / divs - (d.w - orig.w/divs)/2;
            పూర్ణాంకం y = (i/divs) * orig.h / divs - (d.h - orig.h/divs)/2;
            image im = float_గమ్యం_image(orig.w, orig.h, 3, orig.X.vals[j]);
            d.X.vals[j] = crop_image(im, x, y, d.w, d.h).దత్తాంశం;
        }
        ds[i] = d;
    }
    తిరిగి ds;
}

దత్తాంశం resize_దత్తాంశం(దత్తాంశం orig, పూర్ణాంకం w, పూర్ణాంకం h)
{
    దత్తాంశం d = {0};
    d.shallow = 0;
    d.w = w;
    d.h = h;
    పూర్ణాంకం i;
    d.X.అడ్డువరుసలు = orig.X.అడ్డువరుసలు;
    d.X.నిలువుల_సంఖ్య = w*h*3;
    d.X.vals = సున్నా_జ్ఞాపకం_ఇవ్వు(d.X.అడ్డువరుసలు, పరిమాణం(దశాంశం*));

    d.y = copy_మాతృక(orig.y);
#సూచన omp parallel కొరకు
    కొరకు(i = 0; i < orig.X.అడ్డువరుసలు; ++i){
        image im = float_గమ్యం_image(orig.w, orig.h, 3, orig.X.vals[i]);
        d.X.vals[i] = resize_image(im, w, h).దత్తాంశం;
    }
    తిరిగి d;
}

దత్తాంశం ఎక్కించు_దత్తాంశం_augment(అక్షరం **paths, పూర్ణాంకం n, పూర్ణాంకం m, అక్షరం **labels, పూర్ణాంకం k, వృక్షం *hierarchy, పూర్ణాంకం min, పూర్ణాంకం max, పూర్ణాంకం కొలత, దశాంశం angle, దశాంశం aspect, దశాంశం hue, దశాంశం saturation, దశాంశం exposure, పూర్ణాంకం center)
{
    ఒకవేళ(m) paths = get_random_paths(paths, n, m);
    దత్తాంశం d = {0};
    d.shallow = 0;
    d.w=కొలత;
    d.h=కొలత;
    d.X = ఎక్కించు_image_augment_paths(paths, n, min, max, కొలత, angle, aspect, hue, saturation, exposure, center);
    d.y = ఎక్కించు_labels_paths(paths, n, labels, k, hierarchy);
    ఒకవేళ(m) విడుదల(paths);
    తిరిగి d;
}

దత్తాంశం ఎక్కించు_దత్తాంశం_ట్యాగ్(అక్షరం **paths, పూర్ణాంకం n, పూర్ణాంకం m, పూర్ణాంకం k, పూర్ణాంకం min, పూర్ణాంకం max, పూర్ణాంకం కొలత, దశాంశం angle, దశాంశం aspect, దశాంశం hue, దశాంశం saturation, దశాంశం exposure)
{
    ఒకవేళ(m) paths = get_random_paths(paths, n, m);
    దత్తాంశం d = {0};
    d.w = కొలత;
    d.h = కొలత;
    d.shallow = 0;
    d.X = ఎక్కించు_image_augment_paths(paths, n, min, max, కొలత, angle, aspect, hue, saturation, exposure, 0);
    d.y = ఎక్కించు_ట్యాగులు_paths(paths, n, k);
    ఒకవేళ(m) విడుదల(paths);
    తిరిగి d;
}

మాతృక concat_మాతృక(మాతృక m1, మాతృక m2)
{
    పూర్ణాంకం i, లెక్క = 0;
    మాతృక m;
    m.నిలువుల_సంఖ్య = m1.నిలువుల_సంఖ్య;
    m.అడ్డువరుసలు = m1.అడ్డువరుసలు+m2.అడ్డువరుసలు;
    m.vals = సున్నా_జ్ఞాపకం_ఇవ్వు(m1.అడ్డువరుసలు + m2.అడ్డువరుసలు, పరిమాణం(దశాంశం*));
    కొరకు(i = 0; i < m1.అడ్డువరుసలు; ++i){
        m.vals[లెక్క++] = m1.vals[i];
    }
    కొరకు(i = 0; i < m2.అడ్డువరుసలు; ++i){
        m.vals[లెక్క++] = m2.vals[i];
    }
    తిరిగి m;
}

దత్తాంశం concat_దత్తాంశం(దత్తాంశం d1, దత్తాంశం d2)
{
    దత్తాంశం d = {0};
    d.shallow = 1;
    d.X = concat_మాతృక(d1.X, d2.X);
    d.y = concat_మాతృక(d1.y, d2.y);
    d.w = d1.w;
    d.h = d1.h;
    తిరిగి d;
}

దత్తాంశం concat_datas(దత్తాంశం *d, పూర్ణాంకం n)
{
    పూర్ణాంకం i;
    దత్తాంశం బహిర్గతం = {0};
    కొరకు(i = 0; i < n; ++i){
        దత్తాంశం new = concat_దత్తాంశం(d[i], బహిర్గతం);
        free_దత్తాంశం(బహిర్గతం);
        బహిర్గతం = new;
    }
    తిరిగి బహిర్గతం;
}

దత్తాంశం ఎక్కించు_categorical_దత్తాంశం_csv(అక్షరం *filename, పూర్ణాంకం లక్ష్యం, పూర్ణాంకం k)
{
    దత్తాంశం d = {0};
    d.shallow = 0;
    మాతృక X = csv_గమ్యం_మాతృక(filename);
    దశాంశం *truth_1d = తీయి_స్తంభం(&X, లక్ష్యం);
    దశాంశం **truth = one_hot_encode(truth_1d, X.అడ్డువరుసలు, k);
    మాతృక y;
    y.అడ్డువరుసలు = X.అడ్డువరుసలు;
    y.నిలువుల_సంఖ్య = k;
    y.vals = truth;
    d.X = X;
    d.y = y;
    విడుదల(truth_1d);
    తిరిగి d;
}

దత్తాంశం ఎక్కించు_cifar10_దత్తాంశం(అక్షరం *filename)
{
    దత్తాంశం d = {0};
    d.shallow = 0;
    దీర్ఘ i,j;
    మాతృక X = make_మాతృక(10000, 3072);
    మాతృక y = make_మాతృక(10000, 10);
    d.X = X;
    d.y = y;

    FILE *fp = fopen(filename, "rb");
    ఒకవేళ(!fp) file_error(filename);
    కొరకు(i = 0; i < 10000; ++i){
        గుర్తులేని అక్షరం బైట్లు[3073];
        fread(బైట్లు, 1, 3073, fp);
        పూర్ణాంకం class = బైట్లు[0];
        y.vals[i][class] = 1;
        కొరకు(j = 0; j < X.నిలువుల_సంఖ్య; ++j){
            X.vals[i][j] = (ద్విగుణ)బైట్లు[j+1];
        }
    }
    scale_దత్తాంశం_అడ్డువరుసలు(d, 1./255);
    //normalize_data_rows(d);
    fclose(fp);
    తిరిగి d;
}

శూన్య get_random_batch(దత్తాంశం d, పూర్ణాంకం n, దశాంశం *X, దశాంశం *y)
{
    పూర్ణాంకం j;
    కొరకు(j = 0; j < n; ++j){
        పూర్ణాంకం సూచీ = rand()%d.X.అడ్డువరుసలు;
        జ్ఞాపకం_నకలు(X+j*d.X.నిలువుల_సంఖ్య, d.X.vals[సూచీ], d.X.నిలువుల_సంఖ్య*పరిమాణం(దశాంశం));
        జ్ఞాపకం_నకలు(y+j*d.y.నిలువుల_సంఖ్య, d.y.vals[సూచీ], d.y.నిలువుల_సంఖ్య*పరిమాణం(దశాంశం));
    }
}

శూన్య get_తదుపరి_batch(దత్తాంశం d, పూర్ణాంకం n, పూర్ణాంకం స్థానభ్రంశం, దశాంశం *X, దశాంశం *y)
{
    పూర్ణాంకం j;
    కొరకు(j = 0; j < n; ++j){
        పూర్ణాంకం సూచీ = స్థానభ్రంశం + j;
        జ్ఞాపకం_నకలు(X+j*d.X.నిలువుల_సంఖ్య, d.X.vals[సూచీ], d.X.నిలువుల_సంఖ్య*పరిమాణం(దశాంశం));
        ఒకవేళ(y) జ్ఞాపకం_నకలు(y+j*d.y.నిలువుల_సంఖ్య, d.y.vals[సూచీ], d.y.నిలువుల_సంఖ్య*పరిమాణం(దశాంశం));
    }
}

శూన్య smooth_దత్తాంశం(దత్తాంశం d)
{
    పూర్ణాంకం i, j;
    దశాంశం scale = 1. / d.y.నిలువుల_సంఖ్య;
    దశాంశం eps = .1;
    కొరకు(i = 0; i < d.y.అడ్డువరుసలు; ++i){
        కొరకు(j = 0; j < d.y.నిలువుల_సంఖ్య; ++j){
            d.y.vals[i][j] = eps * scale + (1-eps) * d.y.vals[i][j];
        }
    }
}

దత్తాంశం ఎక్కించు_all_cifar10()
{
    దత్తాంశం d = {0};
    d.shallow = 0;
    పూర్ణాంకం i,j,b;
    మాతృక X = make_మాతృక(50000, 3072);
    మాతృక y = make_మాతృక(50000, 10);
    d.X = X;
    d.y = y;


    కొరకు(b = 0; b < 5; ++b){
        అక్షరం buff[256];
        sprintf(buff, "data/cifar/cifar-10-batches-bin/data_batch_%d.bin", b+1);
        FILE *fp = fopen(buff, "rb");
        ఒకవేళ(!fp) file_error(buff);
        కొరకు(i = 0; i < 10000; ++i){
            గుర్తులేని అక్షరం బైట్లు[3073];
            fread(బైట్లు, 1, 3073, fp);
            పూర్ణాంకం class = బైట్లు[0];
            y.vals[i+b*10000][class] = 1;
            కొరకు(j = 0; j < X.నిలువుల_సంఖ్య; ++j){
                X.vals[i+b*10000][j] = (ద్విగుణ)బైట్లు[j+1];
            }
        }
        fclose(fp);
    }
    //normalize_data_rows(d);
    scale_దత్తాంశం_అడ్డువరుసలు(d, 1./255);
    smooth_దత్తాంశం(d);
    తిరిగి d;
}

దత్తాంశం ఎక్కించు_go(అక్షరం *filename)
{
    FILE *fp = fopen(filename, "rb");
    మాతృక X = make_మాతృక(3363059, 361);
    మాతృక y = make_మాతృక(3363059, 361);
    పూర్ణాంకం అడ్డువరుస, నిలువు;

    ఒకవేళ(!fp) file_error(filename);
    అక్షరం *label;
    పూర్ణాంకం లెక్క = 0;
    అయితే((label = fgetl(fp))){
        పూర్ణాంకం i;
        ఒకవేళ(లెక్క == X.అడ్డువరుసలు){
            X = resize_మాతృక(X, లెక్క*2);
            y = resize_మాతృక(y, లెక్క*2);
        }
        అక్షరశ్రేణి_చదువు(label, "%d %d", &అడ్డువరుస, &నిలువు);
        అక్షరం *board = fgetl(fp);

        పూర్ణాంకం సూచీ = అడ్డువరుస*19 + నిలువు;
        y.vals[లెక్క][సూచీ] = 1;

        కొరకు(i = 0; i < 19*19; ++i){
            దశాంశం val = 0;
            ఒకవేళ(board[i] == '1') val = 1;
            లేకపోతే ఒకవేళ(board[i] == '2') val = -1;
            X.vals[లెక్క][i] = val;
        }
        ++లెక్క;
        విడుదల(label);
        విడుదల(board);
    }
    X = resize_మాతృక(X, లెక్క);
    y = resize_మాతృక(y, లెక్క);

    దత్తాంశం d = {0};
    d.shallow = 0;
    d.X = X;
    d.y = y;


    fclose(fp);

    తిరిగి d;
}


శూన్య randomize_దత్తాంశం(దత్తాంశం d)
{
    పూర్ణాంకం i;
    కొరకు(i = d.X.అడ్డువరుసలు-1; i > 0; --i){
        పూర్ణాంకం సూచీ = rand()%i;
        దశాంశం *మార్పిడి = d.X.vals[సూచీ];
        d.X.vals[సూచీ] = d.X.vals[i];
        d.X.vals[i] = మార్పిడి;

        మార్పిడి = d.y.vals[సూచీ];
        d.y.vals[సూచీ] = d.y.vals[i];
        d.y.vals[i] = మార్పిడి;
    }
}

శూన్య scale_దత్తాంశం_అడ్డువరుసలు(దత్తాంశం d, దశాంశం s)
{
    పూర్ణాంకం i;
    కొరకు(i = 0; i < d.X.అడ్డువరుసలు; ++i){
        scale_array(d.X.vals[i], d.X.నిలువుల_సంఖ్య, s);
    }
}

శూన్య translate_దత్తాంశం_అడ్డువరుసలు(దత్తాంశం d, దశాంశం s)
{
    పూర్ణాంకం i;
    కొరకు(i = 0; i < d.X.అడ్డువరుసలు; ++i){
        translate_array(d.X.vals[i], d.X.నిలువుల_సంఖ్య, s);
    }
}

దత్తాంశం copy_దత్తాంశం(దత్తాంశం d)
{
    దత్తాంశం c = {0};
    c.w = d.w;
    c.h = d.h;
    c.shallow = 0;
    c.అంకె_boxes = d.అంకె_boxes;
    c.boxes = d.boxes;
    c.X = copy_మాతృక(d.X);
    c.y = copy_మాతృక(d.y);
    తిరిగి c;
}

శూన్య normalize_దత్తాంశం_అడ్డువరుసలు(దత్తాంశం d)
{
    పూర్ణాంకం i;
    కొరకు(i = 0; i < d.X.అడ్డువరుసలు; ++i){
        normalize_array(d.X.vals[i], d.X.నిలువుల_సంఖ్య);
    }
}

దత్తాంశం get_దత్తాంశం_part(దత్తాంశం d, పూర్ణాంకం part, పూర్ణాంకం మొత్తం)
{
    దత్తాంశం p = {0};
    p.shallow = 1;
    p.X.అడ్డువరుసలు = d.X.అడ్డువరుసలు * (part + 1) / మొత్తం - d.X.అడ్డువరుసలు * part / మొత్తం;
    p.y.అడ్డువరుసలు = d.y.అడ్డువరుసలు * (part + 1) / మొత్తం - d.y.అడ్డువరుసలు * part / మొత్తం;
    p.X.నిలువుల_సంఖ్య = d.X.నిలువుల_సంఖ్య;
    p.y.నిలువుల_సంఖ్య = d.y.నిలువుల_సంఖ్య;
    p.X.vals = d.X.vals + d.X.అడ్డువరుసలు * part / మొత్తం;
    p.y.vals = d.y.vals + d.y.అడ్డువరుసలు * part / మొత్తం;
    తిరిగి p;
}

దత్తాంశం get_random_దత్తాంశం(దత్తాంశం d, పూర్ణాంకం అంకె)
{
    దత్తాంశం r = {0};
    r.shallow = 1;

    r.X.అడ్డువరుసలు = అంకె;
    r.y.అడ్డువరుసలు = అంకె;

    r.X.నిలువుల_సంఖ్య = d.X.నిలువుల_సంఖ్య;
    r.y.నిలువుల_సంఖ్య = d.y.నిలువుల_సంఖ్య;

    r.X.vals = సున్నా_జ్ఞాపకం_ఇవ్వు(అంకె, పరిమాణం(దశాంశం *));
    r.y.vals = సున్నా_జ్ఞాపకం_ఇవ్వు(అంకె, పరిమాణం(దశాంశం *));

    పూర్ణాంకం i;
    కొరకు(i = 0; i < అంకె; ++i){
        పూర్ణాంకం సూచీ = rand()%d.X.అడ్డువరుసలు;
        r.X.vals[i] = d.X.vals[సూచీ];
        r.y.vals[i] = d.y.vals[సూచీ];
    }
    తిరిగి r;
}

దత్తాంశం *విభజించు_దత్తాంశం(దత్తాంశం d, పూర్ణాంకం part, పూర్ణాంకం మొత్తం)
{
    దత్తాంశం *విభజించు = సున్నా_జ్ఞాపకం_ఇవ్వు(2, పరిమాణం(దత్తాంశం));
    పూర్ణాంకం i;
    పూర్ణాంకం ప్రారంభం = part*d.X.అడ్డువరుసలు/మొత్తం;
    పూర్ణాంకం చివర = (part+1)*d.X.అడ్డువరుసలు/మొత్తం;
    దత్తాంశం train;
    దత్తాంశం test;
    train.shallow = test.shallow = 1;

    test.X.అడ్డువరుసలు = test.y.అడ్డువరుసలు = చివర-ప్రారంభం;
    train.X.అడ్డువరుసలు = train.y.అడ్డువరుసలు = d.X.అడ్డువరుసలు - (చివర-ప్రారంభం);
    train.X.నిలువుల_సంఖ్య = test.X.నిలువుల_సంఖ్య = d.X.నిలువుల_సంఖ్య;
    train.y.నిలువుల_సంఖ్య = test.y.నిలువుల_సంఖ్య = d.y.నిలువుల_సంఖ్య;

    train.X.vals = సున్నా_జ్ఞాపకం_ఇవ్వు(train.X.అడ్డువరుసలు, పరిమాణం(దశాంశం*));
    test.X.vals = సున్నా_జ్ఞాపకం_ఇవ్వు(test.X.అడ్డువరుసలు, పరిమాణం(దశాంశం*));
    train.y.vals = సున్నా_జ్ఞాపకం_ఇవ్వు(train.y.అడ్డువరుసలు, పరిమాణం(దశాంశం*));
    test.y.vals = సున్నా_జ్ఞాపకం_ఇవ్వు(test.y.అడ్డువరుసలు, పరిమాణం(దశాంశం*));

    కొరకు(i = 0; i < ప్రారంభం; ++i){
        train.X.vals[i] = d.X.vals[i];
        train.y.vals[i] = d.y.vals[i];
    }
    కొరకు(i = ప్రారంభం; i < చివర; ++i){
        test.X.vals[i-ప్రారంభం] = d.X.vals[i];
        test.y.vals[i-ప్రారంభం] = d.y.vals[i];
    }
    కొరకు(i = చివర; i < d.X.అడ్డువరుసలు; ++i){
        train.X.vals[i-(చివర-ప్రారంభం)] = d.X.vals[i];
        train.y.vals[i-(చివర-ప్రారంభం)] = d.y.vals[i];
    }
    విభజించు[0] = train;
    విభజించు[1] = test;
    తిరిగి విభజించు;
}

