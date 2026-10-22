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
    वर्ण *मार्गः;
    FILE *file = fopen(filename, "r");
    यदि(!file) file_error(filename);
    list *पङ्क्तयः = make_list();
    यावत्((मार्गः=fgetl(file))){
        list_निवेशय(पङ्क्तयः, मार्गः);
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
    वर्ण **random_paths = शून्यस्मृतिं_देहि(n, परिमाण(वर्ण*));
    पूर्णाङ्क i;
    pthread_mutex_lock(&mutex);
    प्रति(i = 0; i < n; ++i){
        पूर्णाङ्क सूचकांक = यादृच्छिकम्()%m;
        random_paths[i] = paths[सूचकांक];
        //if(i == 0) printf("%s\n", paths[index]);
    }
    pthread_mutex_unlock(&mutex);
    प्रत्यागम random_paths;
}

वर्ण **find_replace_paths(वर्ण **paths, पूर्णाङ्क n, वर्ण *find, वर्ण *replace)
{
    वर्ण **replace_paths = शून्यस्मृतिं_देहि(n, परिमाण(वर्ण*));
    पूर्णाङ्क i;
    प्रति(i = 0; i < n; ++i){
        वर्ण replaced[4096];
        find_replace(paths[i], find, replace, replaced);
        replace_paths[i] = copy_string(replaced);
    }
    प्रत्यागम replace_paths;
}

matrix आरोपय_image_paths_gray(वर्ण **paths, पूर्णाङ्क n, पूर्णाङ्क w, पूर्णाङ्क h)
{
    पूर्णाङ्क i;
    matrix X;
    X.पङ्क्तिकाः = n;
    X.vals = शून्यस्मृतिं_देहि(X.पङ्क्तिकाः, परिमाण(भिन्न*));
    X.स्तम्भसङ्ख्या = 0;

    प्रति(i = 0; i < n; ++i){
        image im = आरोपय_image(paths[i], w, h, 3);

        image gray = grayscale_image(im);
        free_image(im);
        im = gray;

        X.vals[i] = im.दत्त;
        X.स्तम्भसङ्ख्या = im.h*im.w*im.c;
    }
    प्रत्यागम X;
}

matrix आरोपय_image_paths(वर्ण **paths, पूर्णाङ्क n, पूर्णाङ्क w, पूर्णाङ्क h)
{
    पूर्णाङ्क i;
    matrix X;
    X.पङ्क्तिकाः = n;
    X.vals = शून्यस्मृतिं_देहि(X.पङ्क्तिकाः, परिमाण(भिन्न*));
    X.स्तम्भसङ्ख्या = 0;

    प्रति(i = 0; i < n; ++i){
        image im = आरोपय_image_color(paths[i], w, h);
        X.vals[i] = im.दत्त;
        X.स्तम्भसङ्ख्या = im.h*im.w*im.c;
    }
    प्रत्यागम X;
}

matrix आरोपय_image_augment_paths(वर्ण **paths, पूर्णाङ्क n, पूर्णाङ्क min, पूर्णाङ्क max, पूर्णाङ्क आकार, भिन्न angle, भिन्न aspect, भिन्न hue, भिन्न saturation, भिन्न exposure, पूर्णाङ्क center)
{
    पूर्णाङ्क i;
    matrix X;
    X.पङ्क्तिकाः = n;
    X.vals = शून्यस्मृतिं_देहि(X.पङ्क्तिकाः, परिमाण(भिन्न*));
    X.स्तम्भसङ्ख्या = 0;

    प्रति(i = 0; i < n; ++i){
        image im = आरोपय_image_color(paths[i], 0, 0);
        image crop;
        यदि(center){
            crop = center_crop_image(im, आकार, आकार);
        } अन्यथा {
            crop = random_augment_image(im, angle, aspect, min, max, आकार, आकार);
        }
        पूर्णाङ्क flip = यादृच्छिकम्()%2;
        यदि (flip) flip_image(crop);
        random_distort_image(crop, hue, saturation, exposure);

        /*
        show_image(im, "orig");
        show_image(crop, "crop");
        cvWaitKey(0);
        */
        //grayscale_image_3c(crop);
        free_image(im);
        X.vals[i] = crop.दत्त;
        X.स्तम्भसङ्ख्या = crop.h*crop.w*crop.c;
    }
    प्रत्यागम X;
}


box_label *read_boxes(वर्ण *filename, पूर्णाङ्क *n)
{
    FILE *file = fopen(filename, "r");
    यदि(!file) file_error(filename);
    भिन्न x, y, h, w;
    पूर्णाङ्क id;
    पूर्णाङ्क गणनम् = 0;
    पूर्णाङ्क आकार = 64;
    box_label *boxes = शून्यस्मृतिं_देहि(आकार, परिमाण(box_label));
    यावत्(fscanf(file, "%d %f %f %f %f", &id, &x, &y, &w, &h) == 5){
        यदि(गणनम् == आकार) {
            आकार = आकार * 2;
            boxes = realloc(boxes, आकार*परिमाण(box_label));
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
        पूर्णाङ्क सूचकांक = यादृच्छिकम्()%n;
        b[i] = b[सूचकांक];
        b[सूचकांक] = swap;
    }
}

शून्य correct_boxes(box_label *boxes, पूर्णाङ्क n, भिन्न dx, भिन्न dy, भिन्न sx, भिन्न sy, पूर्णाङ्क flip)
{
    पूर्णाङ्क i;
    प्रति(i = 0; i < n; ++i){
        यदि(boxes[i].x == 0 && boxes[i].y == 0) {
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

        यदि(flip){
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

शून्य fill_truth_swag(वर्ण *मार्गः, भिन्न *truth, पूर्णाङ्क classes, पूर्णाङ्क flip, भिन्न dx, भिन्न dy, भिन्न sx, भिन्न sy)
{
    वर्ण labelpath[4096];
    find_replace(मार्गः, "images", "labels", labelpath);
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

        यदि (w < .0 || h < .0) अनुवर्त;

        पूर्णाङ्क सूचकांक = (4+classes) * i;

        truth[सूचकांक++] = x;
        truth[सूचकांक++] = y;
        truth[सूचकांक++] = w;
        truth[सूचकांक++] = h;

        यदि (id < classes) truth[सूचकांक+id] = 1;
    }
    मुक्त(boxes);
}

शून्य fill_truth_region(वर्ण *मार्गः, भिन्न *truth, पूर्णाङ्क classes, पूर्णाङ्क अङ्कः_boxes, पूर्णाङ्क flip, भिन्न dx, भिन्न dy, भिन्न sx, भिन्न sy)
{
    वर्ण labelpath[4096];
    find_replace(मार्गः, "images", "labels", labelpath);
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

        यदि (w < .005 || h < .005) अनुवर्त;

        पूर्णाङ्क col = (पूर्णाङ्क)(x*अङ्कः_boxes);
        पूर्णाङ्क row = (पूर्णाङ्क)(y*अङ्कः_boxes);

        x = x*अङ्कः_boxes - col;
        y = y*अङ्कः_boxes - row;

        पूर्णाङ्क सूचकांक = (col+row*अङ्कः_boxes)*(5+classes);
        यदि (truth[सूचकांक]) अनुवर्त;
        truth[सूचकांक++] = 1;

        यदि (id < classes) truth[सूचकांक+id] = 1;
        सूचकांक += classes;

        truth[सूचकांक++] = x;
        truth[सूचकांक++] = y;
        truth[सूचकांक++] = w;
        truth[सूचकांक++] = h;
    }
    मुक्त(boxes);
}

शून्य आरोपय_rle(image im, पूर्णाङ्क *rle, पूर्णाङ्क n)
{
    पूर्णाङ्क गणनम् = 0;
    पूर्णाङ्क curr = 0;
    पूर्णाङ्क i,j;
    प्रति(i = 0; i < n; ++i){
        प्रति(j = 0; j < rle[i]; ++j){
            im.दत्त[गणनम्++] = curr;
        }
        curr = 1 - curr;
    }
    प्रति(; गणनम् < im.h*im.w*im.c; ++गणनम्){
        im.दत्त[गणनम्] = curr;
    }
}

शून्य or_image(image src, image dest, पूर्णाङ्क c)
{
    पूर्णाङ्क i;
    प्रति(i = 0; i < src.w*src.h; ++i){
        यदि(src.दत्त[i]) dest.दत्त[dest.w*dest.h*c + i] = 1;
    }
}

शून्य exclusive_image(image src)
{
    पूर्णाङ्क k, j, i;
    पूर्णाङ्क s = src.w*src.h;
    प्रति(k = 0; k < src.c-1; ++k){
        प्रति(i = 0; i < s; ++i){
            यदि (src.दत्त[k*s + i]){
                प्रति(j = k+1; j < src.c; ++j){
                    src.दत्त[j*s + i] = 0;
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
            यदि(im.दत्त[y*im.w + x]){
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

शून्य fill_truth_iseg(वर्ण *मार्गः, पूर्णाङ्क अङ्कः_boxes, भिन्न *truth, पूर्णाङ्क classes, पूर्णाङ्क w, पूर्णाङ्क h, augment_तर्काः aug, पूर्णाङ्क flip, पूर्णाङ्क mw, पूर्णाङ्क mh)
{
    वर्ण labelpath[4096];
    find_replace(मार्गः, "images", "mask", labelpath);
    find_replace(labelpath, "JPEGImages", "mask", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    FILE *file = fopen(labelpath, "r");
    यदि(!file) file_error(labelpath);
    वर्ण buff[32788];
    पूर्णाङ्क id;
    पूर्णाङ्क i = 0;
    पूर्णाङ्क j;
    image part = make_image(w, h, 1);
    यावत्((fscanf(file, "%d %s", &id, buff) == 2) && i < अङ्कः_boxes){
        पूर्णाङ्क n = 0;
        पूर्णाङ्क *rle = read_intlist(buff, &n, 0);
        आरोपय_rle(part, rle, n);
        image sized = rotate_crop_image(part, aug.rad, aug.scale, aug.w, aug.h, aug.dx, aug.dy, aug.aspect);
        यदि(flip) flip_image(sized);

        image आवरणम् = resize_image(sized, mw, mh);
        truth[i*(mw*mh+1)] = id;
        प्रति(j = 0; j < mw*mh; ++j){
            truth[i*(mw*mh + 1) + 1 + j] = आवरणम्.दत्त[j];
        }
        ++i;

        free_image(आवरणम्);
        free_image(sized);
        मुक्त(rle);
    }
    यदि(i < अङ्कः_boxes) truth[i*(mw*mh+1)] = -1;
    fclose(file);
    free_image(part);
}

शून्य fill_truth_आवरणम्(वर्ण *मार्गः, पूर्णाङ्क अङ्कः_boxes, भिन्न *truth, पूर्णाङ्क classes, पूर्णाङ्क w, पूर्णाङ्क h, augment_तर्काः aug, पूर्णाङ्क flip, पूर्णाङ्क mw, पूर्णाङ्क mh)
{
    वर्ण labelpath[4096];
    find_replace(मार्गः, "images", "mask", labelpath);
    find_replace(labelpath, "JPEGImages", "mask", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    FILE *file = fopen(labelpath, "r");
    यदि(!file) file_error(labelpath);
    वर्ण buff[32788];
    पूर्णाङ्क id;
    पूर्णाङ्क i = 0;
    image part = make_image(w, h, 1);
    यावत्((fscanf(file, "%d %s", &id, buff) == 2) && i < अङ्कः_boxes){
        पूर्णाङ्क n = 0;
        पूर्णाङ्क *rle = read_intlist(buff, &n, 0);
        आरोपय_rle(part, rle, n);
        image sized = rotate_crop_image(part, aug.rad, aug.scale, aug.w, aug.h, aug.dx, aug.dy, aug.aspect);
        यदि(flip) flip_image(sized);
        box b = bound_image(sized);
        यदि(b.w > 0){
            image crop = crop_image(sized, b.x, b.y, b.w, b.h);
            image आवरणम् = resize_image(crop, mw, mh);
            truth[i*(4 + mw*mh + 1) + 0] = (b.x + b.w/2.)/sized.w;
            truth[i*(4 + mw*mh + 1) + 1] = (b.y + b.h/2.)/sized.h;
            truth[i*(4 + mw*mh + 1) + 2] = b.w/sized.w;
            truth[i*(4 + mw*mh + 1) + 3] = b.h/sized.h;
            पूर्णाङ्क j;
            प्रति(j = 0; j < mw*mh; ++j){
                truth[i*(4 + mw*mh + 1) + 4 + j] = आवरणम्.दत्त[j];
            }
            truth[i*(4 + mw*mh + 1) + 4 + mw*mh] = id;
            free_image(crop);
            free_image(आवरणम्);
            ++i;
        }
        free_image(sized);
        मुक्त(rle);
    }
    fclose(file);
    free_image(part);
}


शून्य fill_truth_detection(वर्ण *मार्गः, पूर्णाङ्क अङ्कः_boxes, भिन्न *truth, पूर्णाङ्क classes, पूर्णाङ्क flip, भिन्न dx, भिन्न dy, भिन्न sx, भिन्न sy)
{
    वर्ण labelpath[4096];
    find_replace(मार्गः, "images", "labels", labelpath);
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
    यदि(गणनम् > अङ्कः_boxes) गणनम् = अङ्कः_boxes;
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

        यदि ((w < .001 || h < .001)) {
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

#परिभाषय NUMCHARS 37

शून्य print_letters(भिन्न *pred, पूर्णाङ्क n)
{
    पूर्णाङ्क i;
    प्रति(i = 0; i < n; ++i){
        पूर्णाङ्क सूचकांक = max_सूचकांक(pred+i*NUMCHARS, NUMCHARS);
        प्रारूप_लिख("%c", int_गन्तव्यम्_alphanum(सूचकांक));
    }
    प्रारूप_लिख("\n");
}

शून्य fill_truth_captcha(वर्ण *मार्गः, पूर्णाङ्क n, भिन्न *truth)
{
    वर्ण *begin = strrchr(मार्गः, '/');
    ++begin;
    पूर्णाङ्क i;
    प्रति(i = 0; i < वाक्यदैर्घ्यम्(begin) && i < n && begin[i] != '.'; ++i){
        पूर्णाङ्क सूचकांक = alphanum_गन्तव्यम्_int(begin[i]);
        यदि(सूचकांक > 35) प्रारूप_लिख("Bad %c\n", begin[i]);
        truth[i*NUMCHARS+सूचकांक] = 1;
    }
    प्रति(;i < n; ++i){
        truth[i*NUMCHARS + NUMCHARS-1] = 1;
    }
}

दत्त आरोपय_दत्त_captcha(वर्ण **paths, पूर्णाङ्क n, पूर्णाङ्क m, पूर्णाङ्क k, पूर्णाङ्क w, पूर्णाङ्क h)
{
    यदि(m) paths = get_random_paths(paths, n, m);
    दत्त d = {0};
    d.shallow = 0;
    d.X = आरोपय_image_paths(paths, n, w, h);
    d.y = make_matrix(n, k*NUMCHARS);
    पूर्णाङ्क i;
    प्रति(i = 0; i < n; ++i){
        fill_truth_captcha(paths[i], k, d.y.vals[i]);
    }
    यदि(m) मुक्त(paths);
    प्रत्यागम d;
}

दत्त आरोपय_दत्त_captcha_encode(वर्ण **paths, पूर्णाङ्क n, पूर्णाङ्क m, पूर्णाङ्क w, पूर्णाङ्क h)
{
    यदि(m) paths = get_random_paths(paths, n, m);
    दत्त d = {0};
    d.shallow = 0;
    d.X = आरोपय_image_paths(paths, n, w, h);
    d.X.स्तम्भसङ्ख्या = 17100;
    d.y = d.X;
    यदि(m) मुक्त(paths);
    प्रत्यागम d;
}

शून्य fill_truth(वर्ण *मार्गः, वर्ण **labels, पूर्णाङ्क k, भिन्न *truth)
{
    पूर्णाङ्क i;
    स्मृतिं_पूरय(truth, 0, k*परिमाण(भिन्न));
    पूर्णाङ्क गणनम् = 0;
    प्रति(i = 0; i < k; ++i){
        यदि(वाक्ये_अन्विष(मार्गः, labels[i])){
            truth[i] = 1;
            ++गणनम्;
            //printf("%s %s %d\n", path, labels[i], i);
        }
    }
    यदि(गणनम् != 1 && (k != 1 || गणनम् != 0)) प्रारूप_लिख("Too many or too few labels: %d, %s\n", गणनम्, मार्गः);
}

शून्य fill_hierarchy(भिन्न *truth, पूर्णाङ्क k, वृक्षः *hierarchy)
{
    पूर्णाङ्क j;
    प्रति(j = 0; j < k; ++j){
        यदि(truth[j]){
            पूर्णाङ्क parent = hierarchy->parent[j];
            यावत्(parent >= 0){
                truth[parent] = 1;
                parent = hierarchy->parent[parent];
            }
        }
    }
    पूर्णाङ्क i;
    पूर्णाङ्क गणनम् = 0;
    प्रति(j = 0; j < hierarchy->groups; ++j){
        //printf("%d\n", count);
        पूर्णाङ्क आवरणम् = 1;
        प्रति(i = 0; i < hierarchy->गणः_आकार[j]; ++i){
            यदि(truth[गणनम् + i]){
                आवरणम् = 0;
                भङ्ग;
            }
        }
        यदि (आवरणम्) {
            प्रति(i = 0; i < hierarchy->गणः_आकार[j]; ++i){
                truth[गणनम् + i] = SECRET_अङ्कः;
            }
        }
        गणनम् += hierarchy->गणः_आकार[j];
    }
}

matrix आरोपय_regression_labels_paths(वर्ण **paths, पूर्णाङ्क n, पूर्णाङ्क k)
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

matrix आरोपय_labels_paths(वर्ण **paths, पूर्णाङ्क n, वर्ण **labels, पूर्णाङ्क k, वृक्षः *hierarchy)
{
    matrix y = make_matrix(n, k);
    पूर्णाङ्क i;
    प्रति(i = 0; i < n && labels; ++i){
        fill_truth(paths[i], labels, k, y.vals[i]);
        यदि(hierarchy){
            fill_hierarchy(y.vals[i], k, hierarchy);
        }
    }
    प्रत्यागम y;
}

matrix आरोपय_चिह्नकानि_paths(वर्ण **paths, पूर्णाङ्क n, पूर्णाङ्क k)
{
    matrix y = make_matrix(n, k);
    पूर्णाङ्क i;
    //int count = 0;
    प्रति(i = 0; i < n; ++i){
        वर्ण label[4096];
        find_replace(paths[i], "images", "labels", label);
        find_replace(label, ".jpg", ".txt", label);
        FILE *file = fopen(label, "r");
        यदि (!file) अनुवर्त;
        //++count;
        पूर्णाङ्क चिह्नकम्;
        यावत्(fscanf(file, "%d", &चिह्नकम्) == 1){
            यदि(चिह्नकम् < k){
                y.vals[i][चिह्नकम्] = 1;
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
    वर्ण **labels = (वर्ण **)list_गन्तव्यम्_array(plist);
    free_list(plist);
    प्रत्यागम labels;
}

शून्य free_दत्त(दत्त d)
{
    यदि(!d.shallow){
        free_matrix(d.X);
        free_matrix(d.y);
    }अन्यथा{
        मुक्त(d.X.vals);
        मुक्त(d.y.vals);
    }
}

image get_segmentation_image(वर्ण *मार्गः, पूर्णाङ्क w, पूर्णाङ्क h, पूर्णाङ्क classes)
{
    वर्ण labelpath[4096];
    find_replace(मार्गः, "images", "mask", labelpath);
    find_replace(labelpath, "JPEGImages", "mask", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    image आवरणम् = make_image(w, h, classes);
    FILE *file = fopen(labelpath, "r");
    यदि(!file) file_error(labelpath);
    वर्ण buff[32788];
    पूर्णाङ्क id;
    image part = make_image(w, h, 1);
    यावत्(fscanf(file, "%d %s", &id, buff) == 2){
        पूर्णाङ्क n = 0;
        पूर्णाङ्क *rle = read_intlist(buff, &n, 0);
        आरोपय_rle(part, rle, n);
        or_image(part, आवरणम्, id);
        मुक्त(rle);
    }
    //exclusive_image(mask);
    fclose(file);
    free_image(part);
    प्रत्यागम आवरणम्;
}

image get_segmentation_image2(वर्ण *मार्गः, पूर्णाङ्क w, पूर्णाङ्क h, पूर्णाङ्क classes)
{
    वर्ण labelpath[4096];
    find_replace(मार्गः, "images", "mask", labelpath);
    find_replace(labelpath, "JPEGImages", "mask", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    image आवरणम् = make_image(w, h, classes+1);
    पूर्णाङ्क i;
    प्रति(i = 0; i < w*h; ++i){
        आवरणम्.दत्त[w*h*classes + i] = 1;
    }
    FILE *file = fopen(labelpath, "r");
    यदि(!file) file_error(labelpath);
    वर्ण buff[32788];
    पूर्णाङ्क id;
    image part = make_image(w, h, 1);
    यावत्(fscanf(file, "%d %s", &id, buff) == 2){
        पूर्णाङ्क n = 0;
        पूर्णाङ्क *rle = read_intlist(buff, &n, 0);
        आरोपय_rle(part, rle, n);
        or_image(part, आवरणम्, id);
        प्रति(i = 0; i < w*h; ++i){
            यदि(part.दत्त[i]) आवरणम्.दत्त[w*h*classes + i] = 0;
        }
        मुक्त(rle);
    }
    //exclusive_image(mask);
    fclose(file);
    free_image(part);
    प्रत्यागम आवरणम्;
}

दत्त आरोपय_दत्त_seg(पूर्णाङ्क n, वर्ण **paths, पूर्णाङ्क m, पूर्णाङ्क w, पूर्णाङ्क h, पूर्णाङ्क classes, पूर्णाङ्क min, पूर्णाङ्क max, भिन्न angle, भिन्न aspect, भिन्न hue, भिन्न saturation, भिन्न exposure, पूर्णाङ्क भाजय)
{
    वर्ण **random_paths = get_random_paths(paths, n, m);
    पूर्णाङ्क i;
    दत्त d = {0};
    d.shallow = 0;

    d.X.पङ्क्तिकाः = n;
    d.X.vals = शून्यस्मृतिं_देहि(d.X.पङ्क्तिकाः, परिमाण(भिन्न*));
    d.X.स्तम्भसङ्ख्या = h*w*3;


    d.y.पङ्क्तिकाः = n;
    d.y.स्तम्भसङ्ख्या = h*w*classes/भाजय/भाजय;
    d.y.vals = शून्यस्मृतिं_देहि(d.X.पङ्क्तिकाः, परिमाण(भिन्न*));

    प्रति(i = 0; i < n; ++i){
        image orig = आरोपय_image_color(random_paths[i], 0, 0);
        augment_तर्काः a = random_augment_तर्काः(orig, angle, aspect, min, max, w, h);
        image sized = rotate_crop_image(orig, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);

        पूर्णाङ्क flip = यादृच्छिकम्()%2;
        यदि(flip) flip_image(sized);
        random_distort_image(sized, hue, saturation, exposure);
        d.X.vals[i] = sized.दत्त;

        image आवरणम् = get_segmentation_image(random_paths[i], orig.w, orig.h, classes);
        //image mask = make_image(orig.w, orig.h, classes+1);
        image sized_m = rotate_crop_image(आवरणम्, a.rad, a.scale/भाजय, a.w/भाजय, a.h/भाजय, a.dx/भाजय, a.dy/भाजय, a.aspect);

        यदि(flip) flip_image(sized_m);
        d.y.vals[i] = sized_m.दत्त;

        free_image(orig);
        free_image(आवरणम्);

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

दत्त आरोपय_दत्त_iseg(पूर्णाङ्क n, वर्ण **paths, पूर्णाङ्क m, पूर्णाङ्क w, पूर्णाङ्क h, पूर्णाङ्क classes, पूर्णाङ्क boxes, पूर्णाङ्क भाजय, पूर्णाङ्क min, पूर्णाङ्क max, भिन्न angle, भिन्न aspect, भिन्न hue, भिन्न saturation, भिन्न exposure)
{
    वर्ण **random_paths = get_random_paths(paths, n, m);
    पूर्णाङ्क i;
    दत्त d = {0};
    d.shallow = 0;

    d.X.पङ्क्तिकाः = n;
    d.X.vals = शून्यस्मृतिं_देहि(d.X.पङ्क्तिकाः, परिमाण(भिन्न*));
    d.X.स्तम्भसङ्ख्या = h*w*3;

    d.y = make_matrix(n, (((w/भाजय)*(h/भाजय))+1)*boxes);

    प्रति(i = 0; i < n; ++i){
        image orig = आरोपय_image_color(random_paths[i], 0, 0);
        augment_तर्काः a = random_augment_तर्काः(orig, angle, aspect, min, max, w, h);
        image sized = rotate_crop_image(orig, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);

        पूर्णाङ्क flip = यादृच्छिकम्()%2;
        यदि(flip) flip_image(sized);
        random_distort_image(sized, hue, saturation, exposure);
        d.X.vals[i] = sized.दत्त;
        //show_image(sized, "image");

        fill_truth_iseg(random_paths[i], boxes, d.y.vals[i], classes, orig.w, orig.h, a, flip, w/भाजय, h/भाजय);

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

दत्त आरोपय_दत्त_आवरणम्(पूर्णाङ्क n, वर्ण **paths, पूर्णाङ्क m, पूर्णाङ्क w, पूर्णाङ्क h, पूर्णाङ्क classes, पूर्णाङ्क boxes, पूर्णाङ्क coords, पूर्णाङ्क min, पूर्णाङ्क max, भिन्न angle, भिन्न aspect, भिन्न hue, भिन्न saturation, भिन्न exposure)
{
    वर्ण **random_paths = get_random_paths(paths, n, m);
    पूर्णाङ्क i;
    दत्त d = {0};
    d.shallow = 0;

    d.X.पङ्क्तिकाः = n;
    d.X.vals = शून्यस्मृतिं_देहि(d.X.पङ्क्तिकाः, परिमाण(भिन्न*));
    d.X.स्तम्भसङ्ख्या = h*w*3;

    d.y = make_matrix(n, (coords+1)*boxes);

    प्रति(i = 0; i < n; ++i){
        image orig = आरोपय_image_color(random_paths[i], 0, 0);
        augment_तर्काः a = random_augment_तर्काः(orig, angle, aspect, min, max, w, h);
        image sized = rotate_crop_image(orig, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);

        पूर्णाङ्क flip = यादृच्छिकम्()%2;
        यदि(flip) flip_image(sized);
        random_distort_image(sized, hue, saturation, exposure);
        d.X.vals[i] = sized.दत्त;
        //show_image(sized, "image");

        fill_truth_आवरणम्(random_paths[i], boxes, d.y.vals[i], classes, orig.w, orig.h, a, flip, 14, 14);

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

दत्त आरोपय_दत्त_region(पूर्णाङ्क n, वर्ण **paths, पूर्णाङ्क m, पूर्णाङ्क w, पूर्णाङ्क h, पूर्णाङ्क आकार, पूर्णाङ्क classes, भिन्न jitter, भिन्न hue, भिन्न saturation, भिन्न exposure)
{
    वर्ण **random_paths = get_random_paths(paths, n, m);
    पूर्णाङ्क i;
    दत्त d = {0};
    d.shallow = 0;

    d.X.पङ्क्तिकाः = n;
    d.X.vals = शून्यस्मृतिं_देहि(d.X.पङ्क्तिकाः, परिमाण(भिन्न*));
    d.X.स्तम्भसङ्ख्या = h*w*3;


    पूर्णाङ्क k = आकार*आकार*(5+classes);
    d.y = make_matrix(n, k);
    प्रति(i = 0; i < n; ++i){
        image orig = आरोपय_image_color(random_paths[i], 0, 0);

        पूर्णाङ्क oh = orig.h;
        पूर्णाङ्क ow = orig.w;

        पूर्णाङ्क dw = (ow*jitter);
        पूर्णाङ्क dh = (oh*jitter);

        पूर्णाङ्क pleft  = rand_एकरूपम्(-dw, dw);
        पूर्णाङ्क pright = rand_एकरूपम्(-dw, dw);
        पूर्णाङ्क ptop   = rand_एकरूपम्(-dh, dh);
        पूर्णाङ्क pbot   = rand_एकरूपम्(-dh, dh);

        पूर्णाङ्क swidth =  ow - pleft - pright;
        पूर्णाङ्क sheight = oh - ptop - pbot;

        भिन्न sx = (भिन्न)swidth  / ow;
        भिन्न sy = (भिन्न)sheight / oh;

        पूर्णाङ्क flip = यादृच्छिकम्()%2;
        image cropped = crop_image(orig, pleft, ptop, swidth, sheight);

        भिन्न dx = ((भिन्न)pleft/ow)/sx;
        भिन्न dy = ((भिन्न)ptop /oh)/sy;

        image sized = resize_image(cropped, w, h);
        यदि(flip) flip_image(sized);
        random_distort_image(sized, hue, saturation, exposure);
        d.X.vals[i] = sized.दत्त;

        fill_truth_region(random_paths[i], d.y.vals[i], classes, आकार, flip, dx, dy, 1./sx, 1./sy);

        free_image(orig);
        free_image(cropped);
    }
    मुक्त(random_paths);
    प्रत्यागम d;
}

दत्त आरोपय_दत्त_compare(पूर्णाङ्क n, वर्ण **paths, पूर्णाङ्क m, पूर्णाङ्क classes, पूर्णाङ्क w, पूर्णाङ्क h)
{
    यदि(m) paths = get_random_paths(paths, 2*n, m);
    पूर्णाङ्क i,j;
    दत्त d = {0};
    d.shallow = 0;

    d.X.पङ्क्तिकाः = n;
    d.X.vals = शून्यस्मृतिं_देहि(d.X.पङ्क्तिकाः, परिमाण(भिन्न*));
    d.X.स्तम्भसङ्ख्या = h*w*6;

    पूर्णाङ्क k = 2*(classes);
    d.y = make_matrix(n, k);
    प्रति(i = 0; i < n; ++i){
        image im1 = आरोपय_image_color(paths[i*2],   w, h);
        image im2 = आरोपय_image_color(paths[i*2+1], w, h);

        d.X.vals[i] = शून्यस्मृतिं_देहि(d.X.स्तम्भसङ्ख्या, परिमाण(भिन्न));
        स्मृतिं_प्रतिलिख(d.X.vals[i],         im1.दत्त, h*w*3*परिमाण(भिन्न));
        स्मृतिं_प्रतिलिख(d.X.vals[i] + h*w*3, im2.दत्त, h*w*3*परिमाण(भिन्न));

        पूर्णाङ्क id;
        भिन्न iou;

        वर्ण imlabel1[4096];
        वर्ण imlabel2[4096];
        find_replace(paths[i*2],   "imgs", "labels", imlabel1);
        find_replace(imlabel1, "jpg", "txt", imlabel1);
        FILE *fp1 = fopen(imlabel1, "r");

        यावत्(fscanf(fp1, "%d %f", &id, &iou) == 2){
            यदि (d.y.vals[i][2*id] < iou) d.y.vals[i][2*id] = iou;
        }

        find_replace(paths[i*2+1], "imgs", "labels", imlabel2);
        find_replace(imlabel2, "jpg", "txt", imlabel2);
        FILE *fp2 = fopen(imlabel2, "r");

        यावत्(fscanf(fp2, "%d %f", &id, &iou) == 2){
            यदि (d.y.vals[i][2*id + 1] < iou) d.y.vals[i][2*id + 1] = iou;
        }

        प्रति (j = 0; j < classes; ++j){
            यदि (d.y.vals[i][2*j] > .5 &&  d.y.vals[i][2*j+1] < .5){
                d.y.vals[i][2*j] = 1;
                d.y.vals[i][2*j+1] = 0;
            } अन्यथा यदि (d.y.vals[i][2*j] < .5 &&  d.y.vals[i][2*j+1] > .5){
                d.y.vals[i][2*j] = 0;
                d.y.vals[i][2*j+1] = 1;
            } अन्यथा {
                d.y.vals[i][2*j]   = SECRET_अङ्कः;
                d.y.vals[i][2*j+1] = SECRET_अङ्कः;
            }
        }
        fclose(fp1);
        fclose(fp2);

        free_image(im1);
        free_image(im2);
    }
    यदि(m) मुक्त(paths);
    प्रत्यागम d;
}

दत्त आरोपय_दत्त_swag(वर्ण **paths, पूर्णाङ्क n, पूर्णाङ्क classes, भिन्न jitter)
{
    पूर्णाङ्क सूचकांक = यादृच्छिकम्()%n;
    वर्ण *random_मार्गः = paths[सूचकांक];

    image orig = आरोपय_image_color(random_मार्गः, 0, 0);
    पूर्णाङ्क h = orig.h;
    पूर्णाङ्क w = orig.w;

    दत्त d = {0};
    d.shallow = 0;
    d.w = w;
    d.h = h;

    d.X.पङ्क्तिकाः = 1;
    d.X.vals = शून्यस्मृतिं_देहि(d.X.पङ्क्तिकाः, परिमाण(भिन्न*));
    d.X.स्तम्भसङ्ख्या = h*w*3;

    पूर्णाङ्क k = (4+classes)*90;
    d.y = make_matrix(1, k);

    पूर्णाङ्क dw = w*jitter;
    पूर्णाङ्क dh = h*jitter;

    पूर्णाङ्क pleft  = rand_एकरूपम्(-dw, dw);
    पूर्णाङ्क pright = rand_एकरूपम्(-dw, dw);
    पूर्णाङ्क ptop   = rand_एकरूपम्(-dh, dh);
    पूर्णाङ्क pbot   = rand_एकरूपम्(-dh, dh);

    पूर्णाङ्क swidth =  w - pleft - pright;
    पूर्णाङ्क sheight = h - ptop - pbot;

    भिन्न sx = (भिन्न)swidth  / w;
    भिन्न sy = (भिन्न)sheight / h;

    पूर्णाङ्क flip = यादृच्छिकम्()%2;
    image cropped = crop_image(orig, pleft, ptop, swidth, sheight);

    भिन्न dx = ((भिन्न)pleft/w)/sx;
    भिन्न dy = ((भिन्न)ptop /h)/sy;

    image sized = resize_image(cropped, w, h);
    यदि(flip) flip_image(sized);
    d.X.vals[0] = sized.दत्त;

    fill_truth_swag(random_मार्गः, d.y.vals[0], classes, flip, dx, dy, 1./sx, 1./sy);

    free_image(orig);
    free_image(cropped);

    प्रत्यागम d;
}

दत्त आरोपय_दत्त_detection(पूर्णाङ्क n, वर्ण **paths, पूर्णाङ्क m, पूर्णाङ्क w, पूर्णाङ्क h, पूर्णाङ्क boxes, पूर्णाङ्क classes, भिन्न jitter, भिन्न hue, भिन्न saturation, भिन्न exposure)
{
    वर्ण **random_paths = get_random_paths(paths, n, m);
    पूर्णाङ्क i;
    दत्त d = {0};
    d.shallow = 0;

    d.X.पङ्क्तिकाः = n;
    d.X.vals = शून्यस्मृतिं_देहि(d.X.पङ्क्तिकाः, परिमाण(भिन्न*));
    d.X.स्तम्भसङ्ख्या = h*w*3;

    d.y = make_matrix(n, 5*boxes);
    प्रति(i = 0; i < n; ++i){
        image orig = आरोपय_image_color(random_paths[i], 0, 0);
        image sized = make_image(w, h, orig.c);
        fill_image(sized, .5);

        भिन्न dw = jitter * orig.w;
        भिन्न dh = jitter * orig.h;

        भिन्न new_ar = (orig.w + rand_एकरूपम्(-dw, dw)) / (orig.h + rand_एकरूपम्(-dh, dh));
        //float scale = rand_uniform(.25, 2);
        भिन्न scale = 1;

        भिन्न nw, nh;

        यदि(new_ar < 1){
            nh = scale * h;
            nw = nh * new_ar;
        } अन्यथा {
            nw = scale * w;
            nh = nw / new_ar;
        }

        भिन्न dx = rand_एकरूपम्(0, w - nw);
        भिन्न dy = rand_एकरूपम्(0, h - nh);

        place_image(orig, nw, nh, dx, dy, sized);

        random_distort_image(sized, hue, saturation, exposure);

        पूर्णाङ्क flip = यादृच्छिकम्()%2;
        यदि(flip) flip_image(sized);
        d.X.vals[i] = sized.दत्त;


        fill_truth_detection(random_paths[i], boxes, d.y.vals[i], classes, flip, -dx/w, -dy/h, nw/w, nh/h);

        free_image(orig);
    }
    मुक्त(random_paths);
    प्रत्यागम d;
}

शून्य *आरोपय_thread(शून्य *ptr)
{
    //printf("Loading data: %d\n", rand());
    आरोपय_तर्काः a = *(संरचना आरोपय_तर्काः*)ptr;
    यदि(a.exposure == 0) a.exposure = 1;
    यदि(a.saturation == 0) a.saturation = 1;
    यदि(a.aspect == 0) a.aspect = 1;

    यदि (a.type == OLD_CLASSIFICATION_दत्त){
        *a.d = आरोपय_दत्त_old(a.paths, a.n, a.m, a.labels, a.classes, a.w, a.h);
    } अन्यथा यदि (a.type == REGRESSION_दत्त){
        *a.d = आरोपय_दत्त_regression(a.paths, a.n, a.m, a.classes, a.min, a.max, a.आकार, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    } अन्यथा यदि (a.type == CLASSIFICATION_दत्त){
        *a.d = आरोपय_दत्त_augment(a.paths, a.n, a.m, a.labels, a.classes, a.hierarchy, a.min, a.max, a.आकार, a.angle, a.aspect, a.hue, a.saturation, a.exposure, a.center);
    } अन्यथा यदि (a.type == SUPER_दत्त){
        *a.d = आरोपय_दत्त_super(a.paths, a.n, a.m, a.w, a.h, a.scale);
    } अन्यथा यदि (a.type == WRITING_दत्त){
        *a.d = आरोपय_दत्त_writing(a.paths, a.n, a.m, a.w, a.h, a.बहिर्गमनम्_w, a.बहिर्गमनम्_h);
    } अन्यथा यदि (a.type == ISEG_दत्त){
        *a.d = आरोपय_दत्त_iseg(a.n, a.paths, a.m, a.w, a.h, a.classes, a.अङ्कः_boxes, a.scale, a.min, a.max, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    } अन्यथा यदि (a.type == INSTANCE_दत्त){
        *a.d = आरोपय_दत्त_आवरणम्(a.n, a.paths, a.m, a.w, a.h, a.classes, a.अङ्कः_boxes, a.coords, a.min, a.max, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    } अन्यथा यदि (a.type == SEGMENTATION_दत्त){
        *a.d = आरोपय_दत्त_seg(a.n, a.paths, a.m, a.w, a.h, a.classes, a.min, a.max, a.angle, a.aspect, a.hue, a.saturation, a.exposure, a.scale);
    } अन्यथा यदि (a.type == REGION_दत्त){
        *a.d = आरोपय_दत्त_region(a.n, a.paths, a.m, a.w, a.h, a.अङ्कः_boxes, a.classes, a.jitter, a.hue, a.saturation, a.exposure);
    } अन्यथा यदि (a.type == DETECTION_दत्त){
        *a.d = आरोपय_दत्त_detection(a.n, a.paths, a.m, a.w, a.h, a.अङ्कः_boxes, a.classes, a.jitter, a.hue, a.saturation, a.exposure);
    } अन्यथा यदि (a.type == SWAG_दत्त){
        *a.d = आरोपय_दत्त_swag(a.paths, a.n, a.classes, a.jitter);
    } अन्यथा यदि (a.type == COMPARE_दत्त){
        *a.d = आरोपय_दत्त_compare(a.n, a.paths, a.m, a.classes, a.w, a.h);
    } अन्यथा यदि (a.type == IMAGE_दत्त){
        *(a.im) = आरोपय_image_color(a.मार्गः, 0, 0);
        *(a.resized) = resize_image(*(a.im), a.w, a.h);
    } अन्यथा यदि (a.type == LETTERBOX_दत्त){
        *(a.im) = आरोपय_image_color(a.मार्गः, 0, 0);
        *(a.resized) = letterbox_image(*(a.im), a.w, a.h);
    } अन्यथा यदि (a.type == चिह्नकम्_दत्त){
        *a.d = आरोपय_दत्त_चिह्नकम्(a.paths, a.n, a.m, a.classes, a.min, a.max, a.आकार, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    }
    मुक्त(ptr);
    प्रत्यागम 0;
}

pthread_t आरोपय_दत्त_अन्तर्_thread(आरोपय_तर्काः तर्काः)
{
    pthread_t thread;
    संरचना आरोपय_तर्काः *ptr = शून्यस्मृतिं_देहि(1, परिमाण(संरचना आरोपय_तर्काः));
    *ptr = तर्काः;
    यदि(pthread_create(&thread, 0, आरोपय_thread, ptr)) error("Thread creation failed");
    प्रत्यागम thread;
}

शून्य *आरोपय_तन्तवः(शून्य *ptr)
{
    पूर्णाङ्क i;
    आरोपय_तर्काः तर्काः = *(आरोपय_तर्काः *)ptr;
    यदि (तर्काः.तन्तवः == 0) तर्काः.तन्तवः = 1;
    दत्त *बहिर्गमनम् = तर्काः.d;
    पूर्णाङ्क योग_2 = तर्काः.n;
    मुक्त(ptr);
    दत्त *buffers = शून्यस्मृतिं_देहि(तर्काः.तन्तवः, परिमाण(दत्त));
    pthread_t *तन्तवः = शून्यस्मृतिं_देहि(तर्काः.तन्तवः, परिमाण(pthread_t));
    प्रति(i = 0; i < तर्काः.तन्तवः; ++i){
        तर्काः.d = buffers + i;
        तर्काः.n = (i+1) * योग_2/तर्काः.तन्तवः - i * योग_2/तर्काः.तन्तवः;
        तन्तवः[i] = आरोपय_दत्त_अन्तर्_thread(तर्काः);
    }
    प्रति(i = 0; i < तर्काः.तन्तवः; ++i){
        pthread_join(तन्तवः[i], 0);
    }
    *बहिर्गमनम् = concat_datas(buffers, तर्काः.तन्तवः);
    बहिर्गमनम्->shallow = 0;
    प्रति(i = 0; i < तर्काः.तन्तवः; ++i){
        buffers[i].shallow = 1;
        free_दत्त(buffers[i]);
    }
    मुक्त(buffers);
    मुक्त(तन्तवः);
    प्रत्यागम 0;
}

शून्य आरोपय_दत्त_blocking(आरोपय_तर्काः तर्काः)
{
    संरचना आरोपय_तर्काः *ptr = शून्यस्मृतिं_देहि(1, परिमाण(संरचना आरोपय_तर्काः));
    *ptr = तर्काः;
    आरोपय_thread(ptr);
}

pthread_t आरोपय_दत्त(आरोपय_तर्काः तर्काः)
{
    pthread_t thread;
    संरचना आरोपय_तर्काः *ptr = शून्यस्मृतिं_देहि(1, परिमाण(संरचना आरोपय_तर्काः));
    *ptr = तर्काः;
    यदि(pthread_create(&thread, 0, आरोपय_तन्तवः, ptr)) error("Thread creation failed");
    प्रत्यागम thread;
}

दत्त आरोपय_दत्त_writing(वर्ण **paths, पूर्णाङ्क n, पूर्णाङ्क m, पूर्णाङ्क w, पूर्णाङ्क h, पूर्णाङ्क बहिर्गमनम्_w, पूर्णाङ्क बहिर्गमनम्_h)
{
    यदि(m) paths = get_random_paths(paths, n, m);
    वर्ण **replace_paths = find_replace_paths(paths, n, ".png", "-label.png");
    दत्त d = {0};
    d.shallow = 0;
    d.X = आरोपय_image_paths(paths, n, w, h);
    d.y = आरोपय_image_paths_gray(replace_paths, n, बहिर्गमनम्_w, बहिर्गमनम्_h);
    यदि(m) मुक्त(paths);
    पूर्णाङ्क i;
    प्रति(i = 0; i < n; ++i) मुक्त(replace_paths[i]);
    मुक्त(replace_paths);
    प्रत्यागम d;
}

दत्त आरोपय_दत्त_old(वर्ण **paths, पूर्णाङ्क n, पूर्णाङ्क m, वर्ण **labels, पूर्णाङ्क k, पूर्णाङ्क w, पूर्णाङ्क h)
{
    यदि(m) paths = get_random_paths(paths, n, m);
    दत्त d = {0};
    d.shallow = 0;
    d.X = आरोपय_image_paths(paths, n, w, h);
    d.y = आरोपय_labels_paths(paths, n, labels, k, 0);
    यदि(m) मुक्त(paths);
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

दत्त आरोपय_दत्त_super(वर्ण **paths, पूर्णाङ्क n, पूर्णाङ्क m, पूर्णाङ्क w, पूर्णाङ्क h, पूर्णाङ्क scale)
{
    यदि(m) paths = get_random_paths(paths, n, m);
    दत्त d = {0};
    d.shallow = 0;

    पूर्णाङ्क i;
    d.X.पङ्क्तिकाः = n;
    d.X.vals = शून्यस्मृतिं_देहि(n, परिमाण(भिन्न*));
    d.X.स्तम्भसङ्ख्या = w*h*3;

    d.y.पङ्क्तिकाः = n;
    d.y.vals = शून्यस्मृतिं_देहि(n, परिमाण(भिन्न*));
    d.y.स्तम्भसङ्ख्या = w*scale * h*scale * 3;

    प्रति(i = 0; i < n; ++i){
        image im = आरोपय_image_color(paths[i], 0, 0);
        image crop = random_crop_image(im, w*scale, h*scale);
        पूर्णाङ्क flip = यादृच्छिकम्()%2;
        यदि (flip) flip_image(crop);
        image resize = resize_image(crop, w, h);
        d.X.vals[i] = resize.दत्त;
        d.y.vals[i] = crop.दत्त;
        free_image(im);
    }

    यदि(m) मुक्त(paths);
    प्रत्यागम d;
}

दत्त आरोपय_दत्त_regression(वर्ण **paths, पूर्णाङ्क n, पूर्णाङ्क m, पूर्णाङ्क k, पूर्णाङ्क min, पूर्णाङ्क max, पूर्णाङ्क आकार, भिन्न angle, भिन्न aspect, भिन्न hue, भिन्न saturation, भिन्न exposure)
{
    यदि(m) paths = get_random_paths(paths, n, m);
    दत्त d = {0};
    d.shallow = 0;
    d.X = आरोपय_image_augment_paths(paths, n, min, max, आकार, angle, aspect, hue, saturation, exposure, 0);
    d.y = आरोपय_regression_labels_paths(paths, n, k);
    यदि(m) मुक्त(paths);
    प्रत्यागम d;
}

दत्त select_दत्त(दत्त *orig, पूर्णाङ्क *inds)
{
    दत्त d = {0};
    d.shallow = 1;
    d.w = orig[0].w;
    d.h = orig[0].h;

    d.X.पङ्क्तिकाः = orig[0].X.पङ्क्तिकाः;
    d.y.पङ्क्तिकाः = orig[0].X.पङ्क्तिकाः;

    d.X.स्तम्भसङ्ख्या = orig[0].X.स्तम्भसङ्ख्या;
    d.y.स्तम्भसङ्ख्या = orig[0].y.स्तम्भसङ्ख्या;

    d.X.vals = शून्यस्मृतिं_देहि(orig[0].X.पङ्क्तिकाः, परिमाण(भिन्न *));
    d.y.vals = शून्यस्मृतिं_देहि(orig[0].y.पङ्क्तिकाः, परिमाण(भिन्न *));
    पूर्णाङ्क i;
    प्रति(i = 0; i < d.X.पङ्क्तिकाः; ++i){
        d.X.vals[i] = orig[inds[i]].X.vals[i];
        d.y.vals[i] = orig[inds[i]].y.vals[i];
    }
    प्रत्यागम d;
}

दत्त *tile_दत्त(दत्त orig, पूर्णाङ्क divs, पूर्णाङ्क आकार)
{
    दत्त *ds = शून्यस्मृतिं_देहि(divs*divs, परिमाण(दत्त));
    पूर्णाङ्क i, j;
#निर्देशः omp parallel प्रति
    प्रति(i = 0; i < divs*divs; ++i){
        दत्त d;
        d.shallow = 0;
        d.w = orig.w/divs * आकार;
        d.h = orig.h/divs * आकार;
        d.X.पङ्क्तिकाः = orig.X.पङ्क्तिकाः;
        d.X.स्तम्भसङ्ख्या = d.w*d.h*3;
        d.X.vals = शून्यस्मृतिं_देहि(d.X.पङ्क्तिकाः, परिमाण(भिन्न*));

        d.y = copy_matrix(orig.y);
#निर्देशः omp parallel प्रति
        प्रति(j = 0; j < orig.X.पङ्क्तिकाः; ++j){
            पूर्णाङ्क x = (i%divs) * orig.w / divs - (d.w - orig.w/divs)/2;
            पूर्णाङ्क y = (i/divs) * orig.h / divs - (d.h - orig.h/divs)/2;
            image im = float_गन्तव्यम्_image(orig.w, orig.h, 3, orig.X.vals[j]);
            d.X.vals[j] = crop_image(im, x, y, d.w, d.h).दत्त;
        }
        ds[i] = d;
    }
    प्रत्यागम ds;
}

दत्त resize_दत्त(दत्त orig, पूर्णाङ्क w, पूर्णाङ्क h)
{
    दत्त d = {0};
    d.shallow = 0;
    d.w = w;
    d.h = h;
    पूर्णाङ्क i;
    d.X.पङ्क्तिकाः = orig.X.पङ्क्तिकाः;
    d.X.स्तम्भसङ्ख्या = w*h*3;
    d.X.vals = शून्यस्मृतिं_देहि(d.X.पङ्क्तिकाः, परिमाण(भिन्न*));

    d.y = copy_matrix(orig.y);
#निर्देशः omp parallel प्रति
    प्रति(i = 0; i < orig.X.पङ्क्तिकाः; ++i){
        image im = float_गन्तव्यम्_image(orig.w, orig.h, 3, orig.X.vals[i]);
        d.X.vals[i] = resize_image(im, w, h).दत्त;
    }
    प्रत्यागम d;
}

दत्त आरोपय_दत्त_augment(वर्ण **paths, पूर्णाङ्क n, पूर्णाङ्क m, वर्ण **labels, पूर्णाङ्क k, वृक्षः *hierarchy, पूर्णाङ्क min, पूर्णाङ्क max, पूर्णाङ्क आकार, भिन्न angle, भिन्न aspect, भिन्न hue, भिन्न saturation, भिन्न exposure, पूर्णाङ्क center)
{
    यदि(m) paths = get_random_paths(paths, n, m);
    दत्त d = {0};
    d.shallow = 0;
    d.w=आकार;
    d.h=आकार;
    d.X = आरोपय_image_augment_paths(paths, n, min, max, आकार, angle, aspect, hue, saturation, exposure, center);
    d.y = आरोपय_labels_paths(paths, n, labels, k, hierarchy);
    यदि(m) मुक्त(paths);
    प्रत्यागम d;
}

दत्त आरोपय_दत्त_चिह्नकम्(वर्ण **paths, पूर्णाङ्क n, पूर्णाङ्क m, पूर्णाङ्क k, पूर्णाङ्क min, पूर्णाङ्क max, पूर्णाङ्क आकार, भिन्न angle, भिन्न aspect, भिन्न hue, भिन्न saturation, भिन्न exposure)
{
    यदि(m) paths = get_random_paths(paths, n, m);
    दत्त d = {0};
    d.w = आकार;
    d.h = आकार;
    d.shallow = 0;
    d.X = आरोपय_image_augment_paths(paths, n, min, max, आकार, angle, aspect, hue, saturation, exposure, 0);
    d.y = आरोपय_चिह्नकानि_paths(paths, n, k);
    यदि(m) मुक्त(paths);
    प्रत्यागम d;
}

matrix concat_matrix(matrix m1, matrix m2)
{
    पूर्णाङ्क i, गणनम् = 0;
    matrix m;
    m.स्तम्भसङ्ख्या = m1.स्तम्भसङ्ख्या;
    m.पङ्क्तिकाः = m1.पङ्क्तिकाः+m2.पङ्क्तिकाः;
    m.vals = शून्यस्मृतिं_देहि(m1.पङ्क्तिकाः + m2.पङ्क्तिकाः, परिमाण(भिन्न*));
    प्रति(i = 0; i < m1.पङ्क्तिकाः; ++i){
        m.vals[गणनम्++] = m1.vals[i];
    }
    प्रति(i = 0; i < m2.पङ्क्तिकाः; ++i){
        m.vals[गणनम्++] = m2.vals[i];
    }
    प्रत्यागम m;
}

दत्त concat_दत्त(दत्त d1, दत्त d2)
{
    दत्त d = {0};
    d.shallow = 1;
    d.X = concat_matrix(d1.X, d2.X);
    d.y = concat_matrix(d1.y, d2.y);
    d.w = d1.w;
    d.h = d1.h;
    प्रत्यागम d;
}

दत्त concat_datas(दत्त *d, पूर्णाङ्क n)
{
    पूर्णाङ्क i;
    दत्त बहिर्गमनम् = {0};
    प्रति(i = 0; i < n; ++i){
        दत्त new = concat_दत्त(d[i], बहिर्गमनम्);
        free_दत्त(बहिर्गमनम्);
        बहिर्गमनम् = new;
    }
    प्रत्यागम बहिर्गमनम्;
}

दत्त आरोपय_categorical_दत्त_csv(वर्ण *filename, पूर्णाङ्क target, पूर्णाङ्क k)
{
    दत्त d = {0};
    d.shallow = 0;
    matrix X = csv_गन्तव्यम्_matrix(filename);
    भिन्न *truth_1d = निष्कासय_column(&X, target);
    भिन्न **truth = one_hot_encode(truth_1d, X.पङ्क्तिकाः, k);
    matrix y;
    y.पङ्क्तिकाः = X.पङ्क्तिकाः;
    y.स्तम्भसङ्ख्या = k;
    y.vals = truth;
    d.X = X;
    d.y = y;
    मुक्त(truth_1d);
    प्रत्यागम d;
}

दत्त आरोपय_cifar10_दत्त(वर्ण *filename)
{
    दत्त d = {0};
    d.shallow = 0;
    दीर्घ i,j;
    matrix X = make_matrix(10000, 3072);
    matrix y = make_matrix(10000, 10);
    d.X = X;
    d.y = y;

    FILE *fp = fopen(filename, "rb");
    यदि(!fp) file_error(filename);
    प्रति(i = 0; i < 10000; ++i){
        अचिह्नित वर्ण अष्टकानि[3073];
        fread(अष्टकानि, 1, 3073, fp);
        पूर्णाङ्क class = अष्टकानि[0];
        y.vals[i][class] = 1;
        प्रति(j = 0; j < X.स्तम्भसङ्ख्या; ++j){
            X.vals[i][j] = (द्विगुण)अष्टकानि[j+1];
        }
    }
    scale_दत्त_पङ्क्तिकाः(d, 1./255);
    //normalize_data_rows(d);
    fclose(fp);
    प्रत्यागम d;
}

शून्य get_random_batch(दत्त d, पूर्णाङ्क n, भिन्न *X, भिन्न *y)
{
    पूर्णाङ्क j;
    प्रति(j = 0; j < n; ++j){
        पूर्णाङ्क सूचकांक = यादृच्छिकम्()%d.X.पङ्क्तिकाः;
        स्मृतिं_प्रतिलिख(X+j*d.X.स्तम्भसङ्ख्या, d.X.vals[सूचकांक], d.X.स्तम्भसङ्ख्या*परिमाण(भिन्न));
        स्मृतिं_प्रतिलिख(y+j*d.y.स्तम्भसङ्ख्या, d.y.vals[सूचकांक], d.y.स्तम्भसङ्ख्या*परिमाण(भिन्न));
    }
}

शून्य get_अग्र_batch(दत्त d, पूर्णाङ्क n, पूर्णाङ्क विस्थापनम्, भिन्न *X, भिन्न *y)
{
    पूर्णाङ्क j;
    प्रति(j = 0; j < n; ++j){
        पूर्णाङ्क सूचकांक = विस्थापनम् + j;
        स्मृतिं_प्रतिलिख(X+j*d.X.स्तम्भसङ्ख्या, d.X.vals[सूचकांक], d.X.स्तम्भसङ्ख्या*परिमाण(भिन्न));
        यदि(y) स्मृतिं_प्रतिलिख(y+j*d.y.स्तम्भसङ्ख्या, d.y.vals[सूचकांक], d.y.स्तम्भसङ्ख्या*परिमाण(भिन्न));
    }
}

शून्य smooth_दत्त(दत्त d)
{
    पूर्णाङ्क i, j;
    भिन्न scale = 1. / d.y.स्तम्भसङ्ख्या;
    भिन्न eps = .1;
    प्रति(i = 0; i < d.y.पङ्क्तिकाः; ++i){
        प्रति(j = 0; j < d.y.स्तम्भसङ्ख्या; ++j){
            d.y.vals[i][j] = eps * scale + (1-eps) * d.y.vals[i][j];
        }
    }
}

दत्त आरोपय_all_cifar10()
{
    दत्त d = {0};
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
        यदि(!fp) file_error(buff);
        प्रति(i = 0; i < 10000; ++i){
            अचिह्नित वर्ण अष्टकानि[3073];
            fread(अष्टकानि, 1, 3073, fp);
            पूर्णाङ्क class = अष्टकानि[0];
            y.vals[i+b*10000][class] = 1;
            प्रति(j = 0; j < X.स्तम्भसङ्ख्या; ++j){
                X.vals[i+b*10000][j] = (द्विगुण)अष्टकानि[j+1];
            }
        }
        fclose(fp);
    }
    //normalize_data_rows(d);
    scale_दत्त_पङ्क्तिकाः(d, 1./255);
    smooth_दत्त(d);
    प्रत्यागम d;
}

दत्त आरोपय_go(वर्ण *filename)
{
    FILE *fp = fopen(filename, "rb");
    matrix X = make_matrix(3363059, 361);
    matrix y = make_matrix(3363059, 361);
    पूर्णाङ्क row, col;

    यदि(!fp) file_error(filename);
    वर्ण *label;
    पूर्णाङ्क गणनम् = 0;
    यावत्((label = fgetl(fp))){
        पूर्णाङ्क i;
        यदि(गणनम् == X.पङ्क्तिकाः){
            X = resize_matrix(X, गणनम्*2);
            y = resize_matrix(y, गणनम्*2);
        }
        वाक्यात्_पठ(label, "%d %d", &row, &col);
        वर्ण *board = fgetl(fp);

        पूर्णाङ्क सूचकांक = row*19 + col;
        y.vals[गणनम्][सूचकांक] = 1;

        प्रति(i = 0; i < 19*19; ++i){
            भिन्न val = 0;
            यदि(board[i] == '1') val = 1;
            अन्यथा यदि(board[i] == '2') val = -1;
            X.vals[गणनम्][i] = val;
        }
        ++गणनम्;
        मुक्त(label);
        मुक्त(board);
    }
    X = resize_matrix(X, गणनम्);
    y = resize_matrix(y, गणनम्);

    दत्त d = {0};
    d.shallow = 0;
    d.X = X;
    d.y = y;


    fclose(fp);

    प्रत्यागम d;
}


शून्य randomize_दत्त(दत्त d)
{
    पूर्णाङ्क i;
    प्रति(i = d.X.पङ्क्तिकाः-1; i > 0; --i){
        पूर्णाङ्क सूचकांक = यादृच्छिकम्()%i;
        भिन्न *swap = d.X.vals[सूचकांक];
        d.X.vals[सूचकांक] = d.X.vals[i];
        d.X.vals[i] = swap;

        swap = d.y.vals[सूचकांक];
        d.y.vals[सूचकांक] = d.y.vals[i];
        d.y.vals[i] = swap;
    }
}

शून्य scale_दत्त_पङ्क्तिकाः(दत्त d, भिन्न s)
{
    पूर्णाङ्क i;
    प्रति(i = 0; i < d.X.पङ्क्तिकाः; ++i){
        scale_array(d.X.vals[i], d.X.स्तम्भसङ्ख्या, s);
    }
}

शून्य translate_दत्त_पङ्क्तिकाः(दत्त d, भिन्न s)
{
    पूर्णाङ्क i;
    प्रति(i = 0; i < d.X.पङ्क्तिकाः; ++i){
        translate_array(d.X.vals[i], d.X.स्तम्भसङ्ख्या, s);
    }
}

दत्त copy_दत्त(दत्त d)
{
    दत्त c = {0};
    c.w = d.w;
    c.h = d.h;
    c.shallow = 0;
    c.अङ्कः_boxes = d.अङ्कः_boxes;
    c.boxes = d.boxes;
    c.X = copy_matrix(d.X);
    c.y = copy_matrix(d.y);
    प्रत्यागम c;
}

शून्य normalize_दत्त_पङ्क्तिकाः(दत्त d)
{
    पूर्णाङ्क i;
    प्रति(i = 0; i < d.X.पङ्क्तिकाः; ++i){
        normalize_array(d.X.vals[i], d.X.स्तम्भसङ्ख्या);
    }
}

दत्त get_दत्त_part(दत्त d, पूर्णाङ्क part, पूर्णाङ्क योग_2)
{
    दत्त p = {0};
    p.shallow = 1;
    p.X.पङ्क्तिकाः = d.X.पङ्क्तिकाः * (part + 1) / योग_2 - d.X.पङ्क्तिकाः * part / योग_2;
    p.y.पङ्क्तिकाः = d.y.पङ्क्तिकाः * (part + 1) / योग_2 - d.y.पङ्क्तिकाः * part / योग_2;
    p.X.स्तम्भसङ्ख्या = d.X.स्तम्भसङ्ख्या;
    p.y.स्तम्भसङ्ख्या = d.y.स्तम्भसङ्ख्या;
    p.X.vals = d.X.vals + d.X.पङ्क्तिकाः * part / योग_2;
    p.y.vals = d.y.vals + d.y.पङ्क्तिकाः * part / योग_2;
    प्रत्यागम p;
}

दत्त get_random_दत्त(दत्त d, पूर्णाङ्क अङ्कः)
{
    दत्त r = {0};
    r.shallow = 1;

    r.X.पङ्क्तिकाः = अङ्कः;
    r.y.पङ्क्तिकाः = अङ्कः;

    r.X.स्तम्भसङ्ख्या = d.X.स्तम्भसङ्ख्या;
    r.y.स्तम्भसङ्ख्या = d.y.स्तम्भसङ्ख्या;

    r.X.vals = शून्यस्मृतिं_देहि(अङ्कः, परिमाण(भिन्न *));
    r.y.vals = शून्यस्मृतिं_देहि(अङ्कः, परिमाण(भिन्न *));

    पूर्णाङ्क i;
    प्रति(i = 0; i < अङ्कः; ++i){
        पूर्णाङ्क सूचकांक = यादृच्छिकम्()%d.X.पङ्क्तिकाः;
        r.X.vals[i] = d.X.vals[सूचकांक];
        r.y.vals[i] = d.y.vals[सूचकांक];
    }
    प्रत्यागम r;
}

दत्त *विभज_दत्त(दत्त d, पूर्णाङ्क part, पूर्णाङ्क योग_2)
{
    दत्त *विभज = शून्यस्मृतिं_देहि(2, परिमाण(दत्त));
    पूर्णाङ्क i;
    पूर्णाङ्क start = part*d.X.पङ्क्तिकाः/योग_2;
    पूर्णाङ्क अन्तः = (part+1)*d.X.पङ्क्तिकाः/योग_2;
    दत्त train;
    दत्त test;
    train.shallow = test.shallow = 1;

    test.X.पङ्क्तिकाः = test.y.पङ्क्तिकाः = अन्तः-start;
    train.X.पङ्क्तिकाः = train.y.पङ्क्तिकाः = d.X.पङ्क्तिकाः - (अन्तः-start);
    train.X.स्तम्भसङ्ख्या = test.X.स्तम्भसङ्ख्या = d.X.स्तम्भसङ्ख्या;
    train.y.स्तम्भसङ्ख्या = test.y.स्तम्भसङ्ख्या = d.y.स्तम्भसङ्ख्या;

    train.X.vals = शून्यस्मृतिं_देहि(train.X.पङ्क्तिकाः, परिमाण(भिन्न*));
    test.X.vals = शून्यस्मृतिं_देहि(test.X.पङ्क्तिकाः, परिमाण(भिन्न*));
    train.y.vals = शून्यस्मृतिं_देहि(train.y.पङ्क्तिकाः, परिमाण(भिन्न*));
    test.y.vals = शून्यस्मृतिं_देहि(test.y.पङ्क्तिकाः, परिमाण(भिन्न*));

    प्रति(i = 0; i < start; ++i){
        train.X.vals[i] = d.X.vals[i];
        train.y.vals[i] = d.y.vals[i];
    }
    प्रति(i = start; i < अन्तः; ++i){
        test.X.vals[i-start] = d.X.vals[i];
        test.y.vals[i-start] = d.y.vals[i];
    }
    प्रति(i = अन्तः; i < d.X.पङ्क्तिकाः; ++i){
        train.X.vals[i-(अन्तः-start)] = d.X.vals[i];
        train.y.vals[i-(अन्तः-start)] = d.y.vals[i];
    }
    विभज[0] = train;
    विभज[1] = test;
    प्रत्यागम विभज;
}

