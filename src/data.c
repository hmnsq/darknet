#समावेश "data.h"
#समावेश "utils.h"
#समावेश "image.h"
#समावेश "cuda.h"

#समावेश <stdio.h>
#समावेश <stdlib.h>
#समावेश <string.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

list *get_paths(अक्षर *filename)
{
    अक्षर *path;
    खाता *file = ख_खोलो(filename, "r");
    अगर(!file) file_error(filename);
    list *पंक्तियाँ = make_list();
    जबतक((path=fgetl(file))){
        list_जोड़ो(पंक्तियाँ, path);
    }
    ख_बंद(file);
    वापस पंक्तियाँ;
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

अक्षर **get_random_paths(अक्षर **paths, पूर्णांक n, पूर्णांक m)
{
    अक्षर **random_paths = सुस्मृति(n, माप(अक्षर*));
    पूर्णांक i;
    pthread_mutex_lock(&mutex);
    क्रम(i = 0; i < n; ++i){
        पूर्णांक सूचकांक = अक्रम()%m;
        random_paths[i] = paths[सूचकांक];
        //if(i == 0) printf("%s\n", paths[index]);
    }
    pthread_mutex_unlock(&mutex);
    वापस random_paths;
}

अक्षर **find_replace_paths(अक्षर **paths, पूर्णांक n, अक्षर *find, अक्षर *replace)
{
    अक्षर **replace_paths = सुस्मृति(n, माप(अक्षर*));
    पूर्णांक i;
    क्रम(i = 0; i < n; ++i){
        अक्षर replaced[4096];
        find_replace(paths[i], find, replace, replaced);
        replace_paths[i] = copy_string(replaced);
    }
    वापस replace_paths;
}

matrix load_image_paths_gray(अक्षर **paths, पूर्णांक n, पूर्णांक w, पूर्णांक h)
{
    पूर्णांक i;
    matrix X;
    X.rows = n;
    X.vals = सुस्मृति(X.rows, माप(भग्न*));
    X.cols = 0;

    क्रम(i = 0; i < n; ++i){
        image im = load_image(paths[i], w, h, 3);

        image gray = grayscale_image(im);
        free_image(im);
        im = gray;

        X.vals[i] = im.तथ्य;
        X.cols = im.h*im.w*im.c;
    }
    वापस X;
}

matrix load_image_paths(अक्षर **paths, पूर्णांक n, पूर्णांक w, पूर्णांक h)
{
    पूर्णांक i;
    matrix X;
    X.rows = n;
    X.vals = सुस्मृति(X.rows, माप(भग्न*));
    X.cols = 0;

    क्रम(i = 0; i < n; ++i){
        image im = load_image_color(paths[i], w, h);
        X.vals[i] = im.तथ्य;
        X.cols = im.h*im.w*im.c;
    }
    वापस X;
}

matrix load_image_augment_paths(अक्षर **paths, पूर्णांक n, पूर्णांक min, पूर्णांक max, पूर्णांक आकार, भग्न angle, भग्न aspect, भग्न hue, भग्न saturation, भग्न exposure, पूर्णांक center)
{
    पूर्णांक i;
    matrix X;
    X.rows = n;
    X.vals = सुस्मृति(X.rows, माप(भग्न*));
    X.cols = 0;

    क्रम(i = 0; i < n; ++i){
        image im = load_image_color(paths[i], 0, 0);
        image crop;
        अगर(center){
            crop = center_crop_image(im, आकार, आकार);
        } अन्यथा {
            crop = random_augment_image(im, angle, aspect, min, max, आकार, आकार);
        }
        पूर्णांक flip = अक्रम()%2;
        अगर (flip) flip_image(crop);
        random_distort_image(crop, hue, saturation, exposure);

        /*
        show_image(im, "orig");
        show_image(crop, "crop");
        cvWaitKey(0);
        */
        //grayscale_image_3c(crop);
        free_image(im);
        X.vals[i] = crop.तथ्य;
        X.cols = crop.h*crop.w*crop.c;
    }
    वापस X;
}


box_label *read_boxes(अक्षर *filename, पूर्णांक *n)
{
    खाता *file = ख_खोलो(filename, "r");
    अगर(!file) file_error(filename);
    भग्न x, y, h, w;
    पूर्णांक id;
    पूर्णांक गिनती = 0;
    पूर्णांक आकार = 64;
    box_label *boxes = सुस्मृति(आकार, माप(box_label));
    जबतक(ख_पूछो(file, "%d %f %f %f %f", &id, &x, &y, &w, &h) == 5){
        अगर(गिनती == आकार) {
            आकार = आकार * 2;
            boxes = पुनः_स्मृति(boxes, आकार*माप(box_label));
        }
        boxes[गिनती].id = id;
        boxes[गिनती].x = x;
        boxes[गिनती].y = y;
        boxes[गिनती].h = h;
        boxes[गिनती].w = w;
        boxes[गिनती].बायाँ   = x - w/2;
        boxes[गिनती].दायाँ  = x + w/2;
        boxes[गिनती].शीर्ष    = y - h/2;
        boxes[गिनती].bottom = y + h/2;
        ++गिनती;
    }
    ख_बंद(file);
    *n = गिनती;
    वापस boxes;
}

व्योम randomize_boxes(box_label *b, पूर्णांक n)
{
    पूर्णांक i;
    क्रम(i = 0; i < n; ++i){
        box_label swap = b[i];
        पूर्णांक सूचकांक = अक्रम()%n;
        b[i] = b[सूचकांक];
        b[सूचकांक] = swap;
    }
}

व्योम correct_boxes(box_label *boxes, पूर्णांक n, भग्न dx, भग्न dy, भग्न sx, भग्न sy, पूर्णांक flip)
{
    पूर्णांक i;
    क्रम(i = 0; i < n; ++i){
        अगर(boxes[i].x == 0 && boxes[i].y == 0) {
            boxes[i].x = 999999;
            boxes[i].y = 999999;
            boxes[i].w = 999999;
            boxes[i].h = 999999;
            जारी;
        }
        boxes[i].बायाँ   = boxes[i].बायाँ  * sx - dx;
        boxes[i].दायाँ  = boxes[i].दायाँ * sx - dx;
        boxes[i].शीर्ष    = boxes[i].शीर्ष   * sy - dy;
        boxes[i].bottom = boxes[i].bottom* sy - dy;

        अगर(flip){
            भग्न swap = boxes[i].बायाँ;
            boxes[i].बायाँ = 1. - boxes[i].दायाँ;
            boxes[i].दायाँ = 1. - swap;
        }

        boxes[i].बायाँ =  constrain(0, 1, boxes[i].बायाँ);
        boxes[i].दायाँ = constrain(0, 1, boxes[i].दायाँ);
        boxes[i].शीर्ष =   constrain(0, 1, boxes[i].शीर्ष);
        boxes[i].bottom =   constrain(0, 1, boxes[i].bottom);

        boxes[i].x = (boxes[i].बायाँ+boxes[i].दायाँ)/2;
        boxes[i].y = (boxes[i].शीर्ष+boxes[i].bottom)/2;
        boxes[i].w = (boxes[i].दायाँ - boxes[i].बायाँ);
        boxes[i].h = (boxes[i].bottom - boxes[i].शीर्ष);

        boxes[i].w = constrain(0, 1, boxes[i].w);
        boxes[i].h = constrain(0, 1, boxes[i].h);
    }
}

व्योम fill_truth_swag(अक्षर *path, भग्न *truth, पूर्णांक classes, पूर्णांक flip, भग्न dx, भग्न dy, भग्न sx, भग्न sy)
{
    अक्षर labelpath[4096];
    find_replace(path, "images", "labels", labelpath);
    find_replace(labelpath, "JPEGImages", "labels", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);

    पूर्णांक गिनती = 0;
    box_label *boxes = read_boxes(labelpath, &गिनती);
    randomize_boxes(boxes, गिनती);
    correct_boxes(boxes, गिनती, dx, dy, sx, sy, flip);
    भग्न x,y,w,h;
    पूर्णांक id;
    पूर्णांक i;

    क्रम (i = 0; i < गिनती && i < 90; ++i) {
        x =  boxes[i].x;
        y =  boxes[i].y;
        w =  boxes[i].w;
        h =  boxes[i].h;
        id = boxes[i].id;

        अगर (w < .0 || h < .0) जारी;

        पूर्णांक सूचकांक = (4+classes) * i;

        truth[सूचकांक++] = x;
        truth[सूचकांक++] = y;
        truth[सूचकांक++] = w;
        truth[सूचकांक++] = h;

        अगर (id < classes) truth[सूचकांक+id] = 1;
    }
    मुक्त(boxes);
}

व्योम fill_truth_region(अक्षर *path, भग्न *truth, पूर्णांक classes, पूर्णांक अंक_boxes, पूर्णांक flip, भग्न dx, भग्न dy, भग्न sx, भग्न sy)
{
    अक्षर labelpath[4096];
    find_replace(path, "images", "labels", labelpath);
    find_replace(labelpath, "JPEGImages", "labels", labelpath);

    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".png", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    पूर्णांक गिनती = 0;
    box_label *boxes = read_boxes(labelpath, &गिनती);
    randomize_boxes(boxes, गिनती);
    correct_boxes(boxes, गिनती, dx, dy, sx, sy, flip);
    भग्न x,y,w,h;
    पूर्णांक id;
    पूर्णांक i;

    क्रम (i = 0; i < गिनती; ++i) {
        x =  boxes[i].x;
        y =  boxes[i].y;
        w =  boxes[i].w;
        h =  boxes[i].h;
        id = boxes[i].id;

        अगर (w < .005 || h < .005) जारी;

        पूर्णांक col = (पूर्णांक)(x*अंक_boxes);
        पूर्णांक row = (पूर्णांक)(y*अंक_boxes);

        x = x*अंक_boxes - col;
        y = y*अंक_boxes - row;

        पूर्णांक सूचकांक = (col+row*अंक_boxes)*(5+classes);
        अगर (truth[सूचकांक]) जारी;
        truth[सूचकांक++] = 1;

        अगर (id < classes) truth[सूचकांक+id] = 1;
        सूचकांक += classes;

        truth[सूचकांक++] = x;
        truth[सूचकांक++] = y;
        truth[सूचकांक++] = w;
        truth[सूचकांक++] = h;
    }
    मुक्त(boxes);
}

व्योम load_rle(image im, पूर्णांक *rle, पूर्णांक n)
{
    पूर्णांक गिनती = 0;
    पूर्णांक curr = 0;
    पूर्णांक i,j;
    क्रम(i = 0; i < n; ++i){
        क्रम(j = 0; j < rle[i]; ++j){
            im.तथ्य[गिनती++] = curr;
        }
        curr = 1 - curr;
    }
    क्रम(; गिनती < im.h*im.w*im.c; ++गिनती){
        im.तथ्य[गिनती] = curr;
    }
}

व्योम or_image(image src, image dest, पूर्णांक c)
{
    पूर्णांक i;
    क्रम(i = 0; i < src.w*src.h; ++i){
        अगर(src.तथ्य[i]) dest.तथ्य[dest.w*dest.h*c + i] = 1;
    }
}

व्योम exclusive_image(image src)
{
    पूर्णांक k, j, i;
    पूर्णांक s = src.w*src.h;
    क्रम(k = 0; k < src.c-1; ++k){
        क्रम(i = 0; i < s; ++i){
            अगर (src.तथ्य[k*s + i]){
                क्रम(j = k+1; j < src.c; ++j){
                    src.तथ्य[j*s + i] = 0;
                }
            }
        }
    }
}

box bound_image(image im)
{
    पूर्णांक x,y;
    पूर्णांक minx = im.w;
    पूर्णांक miny = im.h;
    पूर्णांक maxx = 0;
    पूर्णांक maxy = 0;
    क्रम(y = 0; y < im.h; ++y){
        क्रम(x = 0; x < im.w; ++x){
            अगर(im.तथ्य[y*im.w + x]){
                minx = (x < minx) ? x : minx;
                miny = (y < miny) ? y : miny;
                maxx = (x > maxx) ? x : maxx;
                maxy = (y > maxy) ? y : maxy;
            }
        }
    }
    box b = {minx, miny, maxx-minx + 1, maxy-miny + 1};
    //printf("%f %f %f %f\n", b.x, b.y, b.w, b.h);
    वापस b;
}

व्योम fill_truth_iseg(अक्षर *path, पूर्णांक अंक_boxes, भग्न *truth, पूर्णांक classes, पूर्णांक w, पूर्णांक h, augment_args aug, पूर्णांक flip, पूर्णांक mw, पूर्णांक mh)
{
    अक्षर labelpath[4096];
    find_replace(path, "images", "mask", labelpath);
    find_replace(labelpath, "JPEGImages", "mask", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    खाता *file = ख_खोलो(labelpath, "r");
    अगर(!file) file_error(labelpath);
    अक्षर buff[32788];
    पूर्णांक id;
    पूर्णांक i = 0;
    पूर्णांक j;
    image part = make_image(w, h, 1);
    जबतक((ख_पूछो(file, "%d %s", &id, buff) == 2) && i < अंक_boxes){
        पूर्णांक n = 0;
        पूर्णांक *rle = read_intlist(buff, &n, 0);
        load_rle(part, rle, n);
        image sized = rotate_crop_image(part, aug.rad, aug.scale, aug.w, aug.h, aug.dx, aug.dy, aug.aspect);
        अगर(flip) flip_image(sized);

        image mask = resize_image(sized, mw, mh);
        truth[i*(mw*mh+1)] = id;
        क्रम(j = 0; j < mw*mh; ++j){
            truth[i*(mw*mh + 1) + 1 + j] = mask.तथ्य[j];
        }
        ++i;

        free_image(mask);
        free_image(sized);
        मुक्त(rle);
    }
    अगर(i < अंक_boxes) truth[i*(mw*mh+1)] = -1;
    ख_बंद(file);
    free_image(part);
}

व्योम fill_truth_mask(अक्षर *path, पूर्णांक अंक_boxes, भग्न *truth, पूर्णांक classes, पूर्णांक w, पूर्णांक h, augment_args aug, पूर्णांक flip, पूर्णांक mw, पूर्णांक mh)
{
    अक्षर labelpath[4096];
    find_replace(path, "images", "mask", labelpath);
    find_replace(labelpath, "JPEGImages", "mask", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    खाता *file = ख_खोलो(labelpath, "r");
    अगर(!file) file_error(labelpath);
    अक्षर buff[32788];
    पूर्णांक id;
    पूर्णांक i = 0;
    image part = make_image(w, h, 1);
    जबतक((ख_पूछो(file, "%d %s", &id, buff) == 2) && i < अंक_boxes){
        पूर्णांक n = 0;
        पूर्णांक *rle = read_intlist(buff, &n, 0);
        load_rle(part, rle, n);
        image sized = rotate_crop_image(part, aug.rad, aug.scale, aug.w, aug.h, aug.dx, aug.dy, aug.aspect);
        अगर(flip) flip_image(sized);
        box b = bound_image(sized);
        अगर(b.w > 0){
            image crop = crop_image(sized, b.x, b.y, b.w, b.h);
            image mask = resize_image(crop, mw, mh);
            truth[i*(4 + mw*mh + 1) + 0] = (b.x + b.w/2.)/sized.w;
            truth[i*(4 + mw*mh + 1) + 1] = (b.y + b.h/2.)/sized.h;
            truth[i*(4 + mw*mh + 1) + 2] = b.w/sized.w;
            truth[i*(4 + mw*mh + 1) + 3] = b.h/sized.h;
            पूर्णांक j;
            क्रम(j = 0; j < mw*mh; ++j){
                truth[i*(4 + mw*mh + 1) + 4 + j] = mask.तथ्य[j];
            }
            truth[i*(4 + mw*mh + 1) + 4 + mw*mh] = id;
            free_image(crop);
            free_image(mask);
            ++i;
        }
        free_image(sized);
        मुक्त(rle);
    }
    ख_बंद(file);
    free_image(part);
}


व्योम fill_truth_detection(अक्षर *path, पूर्णांक अंक_boxes, भग्न *truth, पूर्णांक classes, पूर्णांक flip, भग्न dx, भग्न dy, भग्न sx, भग्न sy)
{
    अक्षर labelpath[4096];
    find_replace(path, "images", "labels", labelpath);
    find_replace(labelpath, "JPEGImages", "labels", labelpath);

    find_replace(labelpath, "raw", "labels", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".png", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    पूर्णांक गिनती = 0;
    box_label *boxes = read_boxes(labelpath, &गिनती);
    randomize_boxes(boxes, गिनती);
    correct_boxes(boxes, गिनती, dx, dy, sx, sy, flip);
    अगर(गिनती > अंक_boxes) गिनती = अंक_boxes;
    भग्न x,y,w,h;
    पूर्णांक id;
    पूर्णांक i;
    पूर्णांक sub = 0;

    क्रम (i = 0; i < गिनती; ++i) {
        x =  boxes[i].x;
        y =  boxes[i].y;
        w =  boxes[i].w;
        h =  boxes[i].h;
        id = boxes[i].id;

        अगर ((w < .001 || h < .001)) {
            ++sub;
            जारी;
        }

        truth[(i-sub)*5+0] = x;
        truth[(i-sub)*5+1] = y;
        truth[(i-sub)*5+2] = w;
        truth[(i-sub)*5+3] = h;
        truth[(i-sub)*5+4] = id;
    }
    मुक्त(boxes);
}

#घोषणा NUMCHARS 37

व्योम print_letters(भग्न *pred, पूर्णांक n)
{
    पूर्णांक i;
    क्रम(i = 0; i < n; ++i){
        पूर्णांक सूचकांक = max_सूचकांक(pred+i*NUMCHARS, NUMCHARS);
        म_लिखो("%c", int_to_alphanum(सूचकांक));
    }
    म_लिखो("\n");
}

व्योम fill_truth_captcha(अक्षर *path, पूर्णांक n, भग्न *truth)
{
    अक्षर *begin = म_खोजप(path, '/');
    ++begin;
    पूर्णांक i;
    क्रम(i = 0; i < म_माप(begin) && i < n && begin[i] != '.'; ++i){
        पूर्णांक सूचकांक = alphanum_to_int(begin[i]);
        अगर(सूचकांक > 35) म_लिखो("Bad %c\n", begin[i]);
        truth[i*NUMCHARS+सूचकांक] = 1;
    }
    क्रम(;i < n; ++i){
        truth[i*NUMCHARS + NUMCHARS-1] = 1;
    }
}

तथ्य load_तथ्य_captcha(अक्षर **paths, पूर्णांक n, पूर्णांक m, पूर्णांक k, पूर्णांक w, पूर्णांक h)
{
    अगर(m) paths = get_random_paths(paths, n, m);
    तथ्य d = {0};
    d.shallow = 0;
    d.X = load_image_paths(paths, n, w, h);
    d.y = make_matrix(n, k*NUMCHARS);
    पूर्णांक i;
    क्रम(i = 0; i < n; ++i){
        fill_truth_captcha(paths[i], k, d.y.vals[i]);
    }
    अगर(m) मुक्त(paths);
    वापस d;
}

तथ्य load_तथ्य_captcha_encode(अक्षर **paths, पूर्णांक n, पूर्णांक m, पूर्णांक w, पूर्णांक h)
{
    अगर(m) paths = get_random_paths(paths, n, m);
    तथ्य d = {0};
    d.shallow = 0;
    d.X = load_image_paths(paths, n, w, h);
    d.X.cols = 17100;
    d.y = d.X;
    अगर(m) मुक्त(paths);
    वापस d;
}

व्योम fill_truth(अक्षर *path, अक्षर **labels, पूर्णांक k, भग्न *truth)
{
    पूर्णांक i;
    स_रखो(truth, 0, k*माप(भग्न));
    पूर्णांक गिनती = 0;
    क्रम(i = 0; i < k; ++i){
        अगर(म_माला(path, labels[i])){
            truth[i] = 1;
            ++गिनती;
            //printf("%s %s %d\n", path, labels[i], i);
        }
    }
    अगर(गिनती != 1 && (k != 1 || गिनती != 0)) म_लिखो("Too many or too few labels: %d, %s\n", गिनती, path);
}

व्योम fill_hierarchy(भग्न *truth, पूर्णांक k, वृक्ष *hierarchy)
{
    पूर्णांक j;
    क्रम(j = 0; j < k; ++j){
        अगर(truth[j]){
            पूर्णांक parent = hierarchy->parent[j];
            जबतक(parent >= 0){
                truth[parent] = 1;
                parent = hierarchy->parent[parent];
            }
        }
    }
    पूर्णांक i;
    पूर्णांक गिनती = 0;
    क्रम(j = 0; j < hierarchy->groups; ++j){
        //printf("%d\n", count);
        पूर्णांक mask = 1;
        क्रम(i = 0; i < hierarchy->group_आकार[j]; ++i){
            अगर(truth[गिनती + i]){
                mask = 0;
                अवरोध;
            }
        }
        अगर (mask) {
            क्रम(i = 0; i < hierarchy->group_आकार[j]; ++i){
                truth[गिनती + i] = SECRET_अंक;
            }
        }
        गिनती += hierarchy->group_आकार[j];
    }
}

matrix load_regression_labels_paths(अक्षर **paths, पूर्णांक n, पूर्णांक k)
{
    matrix y = make_matrix(n, k);
    पूर्णांक i,j;
    क्रम(i = 0; i < n; ++i){
        अक्षर labelpath[4096];
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

        खाता *file = ख_खोलो(labelpath, "r");
        क्रम(j = 0; j < k; ++j){
            ख_पूछो(file, "%f", &(y.vals[i][j]));
        }
        ख_बंद(file);
    }
    वापस y;
}

matrix load_labels_paths(अक्षर **paths, पूर्णांक n, अक्षर **labels, पूर्णांक k, वृक्ष *hierarchy)
{
    matrix y = make_matrix(n, k);
    पूर्णांक i;
    क्रम(i = 0; i < n && labels; ++i){
        fill_truth(paths[i], labels, k, y.vals[i]);
        अगर(hierarchy){
            fill_hierarchy(y.vals[i], k, hierarchy);
        }
    }
    वापस y;
}

matrix load_tags_paths(अक्षर **paths, पूर्णांक n, पूर्णांक k)
{
    matrix y = make_matrix(n, k);
    पूर्णांक i;
    //int count = 0;
    क्रम(i = 0; i < n; ++i){
        अक्षर label[4096];
        find_replace(paths[i], "images", "labels", label);
        find_replace(label, ".jpg", ".txt", label);
        खाता *file = ख_खोलो(label, "r");
        अगर (!file) जारी;
        //++count;
        पूर्णांक tag;
        जबतक(ख_पूछो(file, "%d", &tag) == 1){
            अगर(tag < k){
                y.vals[i][tag] = 1;
            }
        }
        ख_बंद(file);
    }
    //printf("%d/%d\n", count, n);
    वापस y;
}

अक्षर **get_labels(अक्षर *filename)
{
    list *plist = get_paths(filename);
    अक्षर **labels = (अक्षर **)list_to_array(plist);
    free_list(plist);
    वापस labels;
}

व्योम free_तथ्य(तथ्य d)
{
    अगर(!d.shallow){
        free_matrix(d.X);
        free_matrix(d.y);
    }अन्यथा{
        मुक्त(d.X.vals);
        मुक्त(d.y.vals);
    }
}

image get_segmentation_image(अक्षर *path, पूर्णांक w, पूर्णांक h, पूर्णांक classes)
{
    अक्षर labelpath[4096];
    find_replace(path, "images", "mask", labelpath);
    find_replace(labelpath, "JPEGImages", "mask", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    image mask = make_image(w, h, classes);
    खाता *file = ख_खोलो(labelpath, "r");
    अगर(!file) file_error(labelpath);
    अक्षर buff[32788];
    पूर्णांक id;
    image part = make_image(w, h, 1);
    जबतक(ख_पूछो(file, "%d %s", &id, buff) == 2){
        पूर्णांक n = 0;
        पूर्णांक *rle = read_intlist(buff, &n, 0);
        load_rle(part, rle, n);
        or_image(part, mask, id);
        मुक्त(rle);
    }
    //exclusive_image(mask);
    ख_बंद(file);
    free_image(part);
    वापस mask;
}

image get_segmentation_image2(अक्षर *path, पूर्णांक w, पूर्णांक h, पूर्णांक classes)
{
    अक्षर labelpath[4096];
    find_replace(path, "images", "mask", labelpath);
    find_replace(labelpath, "JPEGImages", "mask", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    image mask = make_image(w, h, classes+1);
    पूर्णांक i;
    क्रम(i = 0; i < w*h; ++i){
        mask.तथ्य[w*h*classes + i] = 1;
    }
    खाता *file = ख_खोलो(labelpath, "r");
    अगर(!file) file_error(labelpath);
    अक्षर buff[32788];
    पूर्णांक id;
    image part = make_image(w, h, 1);
    जबतक(ख_पूछो(file, "%d %s", &id, buff) == 2){
        पूर्णांक n = 0;
        पूर्णांक *rle = read_intlist(buff, &n, 0);
        load_rle(part, rle, n);
        or_image(part, mask, id);
        क्रम(i = 0; i < w*h; ++i){
            अगर(part.तथ्य[i]) mask.तथ्य[w*h*classes + i] = 0;
        }
        मुक्त(rle);
    }
    //exclusive_image(mask);
    ख_बंद(file);
    free_image(part);
    वापस mask;
}

तथ्य load_तथ्य_seg(पूर्णांक n, अक्षर **paths, पूर्णांक m, पूर्णांक w, पूर्णांक h, पूर्णांक classes, पूर्णांक min, पूर्णांक max, भग्न angle, भग्न aspect, भग्न hue, भग्न saturation, भग्न exposure, पूर्णांक भाग)
{
    अक्षर **random_paths = get_random_paths(paths, n, m);
    पूर्णांक i;
    तथ्य d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = सुस्मृति(d.X.rows, माप(भग्न*));
    d.X.cols = h*w*3;


    d.y.rows = n;
    d.y.cols = h*w*classes/भाग/भाग;
    d.y.vals = सुस्मृति(d.X.rows, माप(भग्न*));

    क्रम(i = 0; i < n; ++i){
        image orig = load_image_color(random_paths[i], 0, 0);
        augment_args a = random_augment_args(orig, angle, aspect, min, max, w, h);
        image sized = rotate_crop_image(orig, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);

        पूर्णांक flip = अक्रम()%2;
        अगर(flip) flip_image(sized);
        random_distort_image(sized, hue, saturation, exposure);
        d.X.vals[i] = sized.तथ्य;

        image mask = get_segmentation_image(random_paths[i], orig.w, orig.h, classes);
        //image mask = make_image(orig.w, orig.h, classes+1);
        image sized_m = rotate_crop_image(mask, a.rad, a.scale/भाग, a.w/भाग, a.h/भाग, a.dx/भाग, a.dy/भाग, a.aspect);

        अगर(flip) flip_image(sized_m);
        d.y.vals[i] = sized_m.तथ्य;

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
    वापस d;
}

तथ्य load_तथ्य_iseg(पूर्णांक n, अक्षर **paths, पूर्णांक m, पूर्णांक w, पूर्णांक h, पूर्णांक classes, पूर्णांक boxes, पूर्णांक भाग, पूर्णांक min, पूर्णांक max, भग्न angle, भग्न aspect, भग्न hue, भग्न saturation, भग्न exposure)
{
    अक्षर **random_paths = get_random_paths(paths, n, m);
    पूर्णांक i;
    तथ्य d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = सुस्मृति(d.X.rows, माप(भग्न*));
    d.X.cols = h*w*3;

    d.y = make_matrix(n, (((w/भाग)*(h/भाग))+1)*boxes);

    क्रम(i = 0; i < n; ++i){
        image orig = load_image_color(random_paths[i], 0, 0);
        augment_args a = random_augment_args(orig, angle, aspect, min, max, w, h);
        image sized = rotate_crop_image(orig, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);

        पूर्णांक flip = अक्रम()%2;
        अगर(flip) flip_image(sized);
        random_distort_image(sized, hue, saturation, exposure);
        d.X.vals[i] = sized.तथ्य;
        //show_image(sized, "image");

        fill_truth_iseg(random_paths[i], boxes, d.y.vals[i], classes, orig.w, orig.h, a, flip, w/भाग, h/भाग);

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
    वापस d;
}

तथ्य load_तथ्य_mask(पूर्णांक n, अक्षर **paths, पूर्णांक m, पूर्णांक w, पूर्णांक h, पूर्णांक classes, पूर्णांक boxes, पूर्णांक coords, पूर्णांक min, पूर्णांक max, भग्न angle, भग्न aspect, भग्न hue, भग्न saturation, भग्न exposure)
{
    अक्षर **random_paths = get_random_paths(paths, n, m);
    पूर्णांक i;
    तथ्य d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = सुस्मृति(d.X.rows, माप(भग्न*));
    d.X.cols = h*w*3;

    d.y = make_matrix(n, (coords+1)*boxes);

    क्रम(i = 0; i < n; ++i){
        image orig = load_image_color(random_paths[i], 0, 0);
        augment_args a = random_augment_args(orig, angle, aspect, min, max, w, h);
        image sized = rotate_crop_image(orig, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);

        पूर्णांक flip = अक्रम()%2;
        अगर(flip) flip_image(sized);
        random_distort_image(sized, hue, saturation, exposure);
        d.X.vals[i] = sized.तथ्य;
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
    वापस d;
}

तथ्य load_तथ्य_region(पूर्णांक n, अक्षर **paths, पूर्णांक m, पूर्णांक w, पूर्णांक h, पूर्णांक आकार, पूर्णांक classes, भग्न jitter, भग्न hue, भग्न saturation, भग्न exposure)
{
    अक्षर **random_paths = get_random_paths(paths, n, m);
    पूर्णांक i;
    तथ्य d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = सुस्मृति(d.X.rows, माप(भग्न*));
    d.X.cols = h*w*3;


    पूर्णांक k = आकार*आकार*(5+classes);
    d.y = make_matrix(n, k);
    क्रम(i = 0; i < n; ++i){
        image orig = load_image_color(random_paths[i], 0, 0);

        पूर्णांक oh = orig.h;
        पूर्णांक ow = orig.w;

        पूर्णांक dw = (ow*jitter);
        पूर्णांक dh = (oh*jitter);

        पूर्णांक pleft  = rand_uniform(-dw, dw);
        पूर्णांक pright = rand_uniform(-dw, dw);
        पूर्णांक ptop   = rand_uniform(-dh, dh);
        पूर्णांक pbot   = rand_uniform(-dh, dh);

        पूर्णांक swidth =  ow - pleft - pright;
        पूर्णांक sheight = oh - ptop - pbot;

        भग्न sx = (भग्न)swidth  / ow;
        भग्न sy = (भग्न)sheight / oh;

        पूर्णांक flip = अक्रम()%2;
        image cropped = crop_image(orig, pleft, ptop, swidth, sheight);

        भग्न dx = ((भग्न)pleft/ow)/sx;
        भग्न dy = ((भग्न)ptop /oh)/sy;

        image sized = resize_image(cropped, w, h);
        अगर(flip) flip_image(sized);
        random_distort_image(sized, hue, saturation, exposure);
        d.X.vals[i] = sized.तथ्य;

        fill_truth_region(random_paths[i], d.y.vals[i], classes, आकार, flip, dx, dy, 1./sx, 1./sy);

        free_image(orig);
        free_image(cropped);
    }
    मुक्त(random_paths);
    वापस d;
}

तथ्य load_तथ्य_compare(पूर्णांक n, अक्षर **paths, पूर्णांक m, पूर्णांक classes, पूर्णांक w, पूर्णांक h)
{
    अगर(m) paths = get_random_paths(paths, 2*n, m);
    पूर्णांक i,j;
    तथ्य d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = सुस्मृति(d.X.rows, माप(भग्न*));
    d.X.cols = h*w*6;

    पूर्णांक k = 2*(classes);
    d.y = make_matrix(n, k);
    क्रम(i = 0; i < n; ++i){
        image im1 = load_image_color(paths[i*2],   w, h);
        image im2 = load_image_color(paths[i*2+1], w, h);

        d.X.vals[i] = सुस्मृति(d.X.cols, माप(भग्न));
        स_नकल(d.X.vals[i],         im1.तथ्य, h*w*3*माप(भग्न));
        स_नकल(d.X.vals[i] + h*w*3, im2.तथ्य, h*w*3*माप(भग्न));

        पूर्णांक id;
        भग्न iou;

        अक्षर imlabel1[4096];
        अक्षर imlabel2[4096];
        find_replace(paths[i*2],   "imgs", "labels", imlabel1);
        find_replace(imlabel1, "jpg", "txt", imlabel1);
        खाता *fp1 = ख_खोलो(imlabel1, "r");

        जबतक(ख_पूछो(fp1, "%d %f", &id, &iou) == 2){
            अगर (d.y.vals[i][2*id] < iou) d.y.vals[i][2*id] = iou;
        }

        find_replace(paths[i*2+1], "imgs", "labels", imlabel2);
        find_replace(imlabel2, "jpg", "txt", imlabel2);
        खाता *fp2 = ख_खोलो(imlabel2, "r");

        जबतक(ख_पूछो(fp2, "%d %f", &id, &iou) == 2){
            अगर (d.y.vals[i][2*id + 1] < iou) d.y.vals[i][2*id + 1] = iou;
        }

        क्रम (j = 0; j < classes; ++j){
            अगर (d.y.vals[i][2*j] > .5 &&  d.y.vals[i][2*j+1] < .5){
                d.y.vals[i][2*j] = 1;
                d.y.vals[i][2*j+1] = 0;
            } अन्यथा अगर (d.y.vals[i][2*j] < .5 &&  d.y.vals[i][2*j+1] > .5){
                d.y.vals[i][2*j] = 0;
                d.y.vals[i][2*j+1] = 1;
            } अन्यथा {
                d.y.vals[i][2*j]   = SECRET_अंक;
                d.y.vals[i][2*j+1] = SECRET_अंक;
            }
        }
        ख_बंद(fp1);
        ख_बंद(fp2);

        free_image(im1);
        free_image(im2);
    }
    अगर(m) मुक्त(paths);
    वापस d;
}

तथ्य load_तथ्य_swag(अक्षर **paths, पूर्णांक n, पूर्णांक classes, भग्न jitter)
{
    पूर्णांक सूचकांक = अक्रम()%n;
    अक्षर *random_path = paths[सूचकांक];

    image orig = load_image_color(random_path, 0, 0);
    पूर्णांक h = orig.h;
    पूर्णांक w = orig.w;

    तथ्य d = {0};
    d.shallow = 0;
    d.w = w;
    d.h = h;

    d.X.rows = 1;
    d.X.vals = सुस्मृति(d.X.rows, माप(भग्न*));
    d.X.cols = h*w*3;

    पूर्णांक k = (4+classes)*90;
    d.y = make_matrix(1, k);

    पूर्णांक dw = w*jitter;
    पूर्णांक dh = h*jitter;

    पूर्णांक pleft  = rand_uniform(-dw, dw);
    पूर्णांक pright = rand_uniform(-dw, dw);
    पूर्णांक ptop   = rand_uniform(-dh, dh);
    पूर्णांक pbot   = rand_uniform(-dh, dh);

    पूर्णांक swidth =  w - pleft - pright;
    पूर्णांक sheight = h - ptop - pbot;

    भग्न sx = (भग्न)swidth  / w;
    भग्न sy = (भग्न)sheight / h;

    पूर्णांक flip = अक्रम()%2;
    image cropped = crop_image(orig, pleft, ptop, swidth, sheight);

    भग्न dx = ((भग्न)pleft/w)/sx;
    भग्न dy = ((भग्न)ptop /h)/sy;

    image sized = resize_image(cropped, w, h);
    अगर(flip) flip_image(sized);
    d.X.vals[0] = sized.तथ्य;

    fill_truth_swag(random_path, d.y.vals[0], classes, flip, dx, dy, 1./sx, 1./sy);

    free_image(orig);
    free_image(cropped);

    वापस d;
}

तथ्य load_तथ्य_detection(पूर्णांक n, अक्षर **paths, पूर्णांक m, पूर्णांक w, पूर्णांक h, पूर्णांक boxes, पूर्णांक classes, भग्न jitter, भग्न hue, भग्न saturation, भग्न exposure)
{
    अक्षर **random_paths = get_random_paths(paths, n, m);
    पूर्णांक i;
    तथ्य d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = सुस्मृति(d.X.rows, माप(भग्न*));
    d.X.cols = h*w*3;

    d.y = make_matrix(n, 5*boxes);
    क्रम(i = 0; i < n; ++i){
        image orig = load_image_color(random_paths[i], 0, 0);
        image sized = make_image(w, h, orig.c);
        fill_image(sized, .5);

        भग्न dw = jitter * orig.w;
        भग्न dh = jitter * orig.h;

        भग्न new_ar = (orig.w + rand_uniform(-dw, dw)) / (orig.h + rand_uniform(-dh, dh));
        //float scale = rand_uniform(.25, 2);
        भग्न scale = 1;

        भग्न nw, nh;

        अगर(new_ar < 1){
            nh = scale * h;
            nw = nh * new_ar;
        } अन्यथा {
            nw = scale * w;
            nh = nw / new_ar;
        }

        भग्न dx = rand_uniform(0, w - nw);
        भग्न dy = rand_uniform(0, h - nh);

        place_image(orig, nw, nh, dx, dy, sized);

        random_distort_image(sized, hue, saturation, exposure);

        पूर्णांक flip = अक्रम()%2;
        अगर(flip) flip_image(sized);
        d.X.vals[i] = sized.तथ्य;


        fill_truth_detection(random_paths[i], boxes, d.y.vals[i], classes, flip, -dx/w, -dy/h, nw/w, nh/h);

        free_image(orig);
    }
    मुक्त(random_paths);
    वापस d;
}

व्योम *load_thread(व्योम *ptr)
{
    //printf("Loading data: %d\n", rand());
    load_args a = *(काष्ठा load_args*)ptr;
    अगर(a.exposure == 0) a.exposure = 1;
    अगर(a.saturation == 0) a.saturation = 1;
    अगर(a.aspect == 0) a.aspect = 1;

    अगर (a.type == OLD_CLASSIFICATION_तथ्य){
        *a.d = load_तथ्य_old(a.paths, a.n, a.m, a.labels, a.classes, a.w, a.h);
    } अन्यथा अगर (a.type == REGRESSION_तथ्य){
        *a.d = load_तथ्य_regression(a.paths, a.n, a.m, a.classes, a.min, a.max, a.आकार, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    } अन्यथा अगर (a.type == CLASSIFICATION_तथ्य){
        *a.d = load_तथ्य_augment(a.paths, a.n, a.m, a.labels, a.classes, a.hierarchy, a.min, a.max, a.आकार, a.angle, a.aspect, a.hue, a.saturation, a.exposure, a.center);
    } अन्यथा अगर (a.type == SUPER_तथ्य){
        *a.d = load_तथ्य_super(a.paths, a.n, a.m, a.w, a.h, a.scale);
    } अन्यथा अगर (a.type == WRITING_तथ्य){
        *a.d = load_तथ्य_writing(a.paths, a.n, a.m, a.w, a.h, a.out_w, a.out_h);
    } अन्यथा अगर (a.type == ISEG_तथ्य){
        *a.d = load_तथ्य_iseg(a.n, a.paths, a.m, a.w, a.h, a.classes, a.अंक_boxes, a.scale, a.min, a.max, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    } अन्यथा अगर (a.type == INSTANCE_तथ्य){
        *a.d = load_तथ्य_mask(a.n, a.paths, a.m, a.w, a.h, a.classes, a.अंक_boxes, a.coords, a.min, a.max, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    } अन्यथा अगर (a.type == SEGMENTATION_तथ्य){
        *a.d = load_तथ्य_seg(a.n, a.paths, a.m, a.w, a.h, a.classes, a.min, a.max, a.angle, a.aspect, a.hue, a.saturation, a.exposure, a.scale);
    } अन्यथा अगर (a.type == REGION_तथ्य){
        *a.d = load_तथ्य_region(a.n, a.paths, a.m, a.w, a.h, a.अंक_boxes, a.classes, a.jitter, a.hue, a.saturation, a.exposure);
    } अन्यथा अगर (a.type == DETECTION_तथ्य){
        *a.d = load_तथ्य_detection(a.n, a.paths, a.m, a.w, a.h, a.अंक_boxes, a.classes, a.jitter, a.hue, a.saturation, a.exposure);
    } अन्यथा अगर (a.type == SWAG_तथ्य){
        *a.d = load_तथ्य_swag(a.paths, a.n, a.classes, a.jitter);
    } अन्यथा अगर (a.type == COMPARE_तथ्य){
        *a.d = load_तथ्य_compare(a.n, a.paths, a.m, a.classes, a.w, a.h);
    } अन्यथा अगर (a.type == IMAGE_तथ्य){
        *(a.im) = load_image_color(a.path, 0, 0);
        *(a.resized) = resize_image(*(a.im), a.w, a.h);
    } अन्यथा अगर (a.type == LETTERBOX_तथ्य){
        *(a.im) = load_image_color(a.path, 0, 0);
        *(a.resized) = letterbox_image(*(a.im), a.w, a.h);
    } अन्यथा अगर (a.type == TAG_तथ्य){
        *a.d = load_तथ्य_tag(a.paths, a.n, a.m, a.classes, a.min, a.max, a.आकार, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    }
    मुक्त(ptr);
    वापस 0;
}

pthread_t load_तथ्य_in_thread(load_args args)
{
    pthread_t thread;
    काष्ठा load_args *ptr = सुस्मृति(1, माप(काष्ठा load_args));
    *ptr = args;
    अगर(pthread_create(&thread, 0, load_thread, ptr)) error("Thread creation failed");
    वापस thread;
}

व्योम *load_threads(व्योम *ptr)
{
    पूर्णांक i;
    load_args args = *(load_args *)ptr;
    अगर (args.threads == 0) args.threads = 1;
    तथ्य *out = args.d;
    पूर्णांक योग_2 = args.n;
    मुक्त(ptr);
    तथ्य *buffers = सुस्मृति(args.threads, माप(तथ्य));
    pthread_t *threads = सुस्मृति(args.threads, माप(pthread_t));
    क्रम(i = 0; i < args.threads; ++i){
        args.d = buffers + i;
        args.n = (i+1) * योग_2/args.threads - i * योग_2/args.threads;
        threads[i] = load_तथ्य_in_thread(args);
    }
    क्रम(i = 0; i < args.threads; ++i){
        pthread_join(threads[i], 0);
    }
    *out = concat_datas(buffers, args.threads);
    out->shallow = 0;
    क्रम(i = 0; i < args.threads; ++i){
        buffers[i].shallow = 1;
        free_तथ्य(buffers[i]);
    }
    मुक्त(buffers);
    मुक्त(threads);
    वापस 0;
}

व्योम load_तथ्य_blocking(load_args args)
{
    काष्ठा load_args *ptr = सुस्मृति(1, माप(काष्ठा load_args));
    *ptr = args;
    load_thread(ptr);
}

pthread_t load_तथ्य(load_args args)
{
    pthread_t thread;
    काष्ठा load_args *ptr = सुस्मृति(1, माप(काष्ठा load_args));
    *ptr = args;
    अगर(pthread_create(&thread, 0, load_threads, ptr)) error("Thread creation failed");
    वापस thread;
}

तथ्य load_तथ्य_writing(अक्षर **paths, पूर्णांक n, पूर्णांक m, पूर्णांक w, पूर्णांक h, पूर्णांक out_w, पूर्णांक out_h)
{
    अगर(m) paths = get_random_paths(paths, n, m);
    अक्षर **replace_paths = find_replace_paths(paths, n, ".png", "-label.png");
    तथ्य d = {0};
    d.shallow = 0;
    d.X = load_image_paths(paths, n, w, h);
    d.y = load_image_paths_gray(replace_paths, n, out_w, out_h);
    अगर(m) मुक्त(paths);
    पूर्णांक i;
    क्रम(i = 0; i < n; ++i) मुक्त(replace_paths[i]);
    मुक्त(replace_paths);
    वापस d;
}

तथ्य load_तथ्य_old(अक्षर **paths, पूर्णांक n, पूर्णांक m, अक्षर **labels, पूर्णांक k, पूर्णांक w, पूर्णांक h)
{
    अगर(m) paths = get_random_paths(paths, n, m);
    तथ्य d = {0};
    d.shallow = 0;
    d.X = load_image_paths(paths, n, w, h);
    d.y = load_labels_paths(paths, n, labels, k, 0);
    अगर(m) मुक्त(paths);
    वापस d;
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

तथ्य load_तथ्य_super(अक्षर **paths, पूर्णांक n, पूर्णांक m, पूर्णांक w, पूर्णांक h, पूर्णांक scale)
{
    अगर(m) paths = get_random_paths(paths, n, m);
    तथ्य d = {0};
    d.shallow = 0;

    पूर्णांक i;
    d.X.rows = n;
    d.X.vals = सुस्मृति(n, माप(भग्न*));
    d.X.cols = w*h*3;

    d.y.rows = n;
    d.y.vals = सुस्मृति(n, माप(भग्न*));
    d.y.cols = w*scale * h*scale * 3;

    क्रम(i = 0; i < n; ++i){
        image im = load_image_color(paths[i], 0, 0);
        image crop = random_crop_image(im, w*scale, h*scale);
        पूर्णांक flip = अक्रम()%2;
        अगर (flip) flip_image(crop);
        image resize = resize_image(crop, w, h);
        d.X.vals[i] = resize.तथ्य;
        d.y.vals[i] = crop.तथ्य;
        free_image(im);
    }

    अगर(m) मुक्त(paths);
    वापस d;
}

तथ्य load_तथ्य_regression(अक्षर **paths, पूर्णांक n, पूर्णांक m, पूर्णांक k, पूर्णांक min, पूर्णांक max, पूर्णांक आकार, भग्न angle, भग्न aspect, भग्न hue, भग्न saturation, भग्न exposure)
{
    अगर(m) paths = get_random_paths(paths, n, m);
    तथ्य d = {0};
    d.shallow = 0;
    d.X = load_image_augment_paths(paths, n, min, max, आकार, angle, aspect, hue, saturation, exposure, 0);
    d.y = load_regression_labels_paths(paths, n, k);
    अगर(m) मुक्त(paths);
    वापस d;
}

तथ्य select_तथ्य(तथ्य *orig, पूर्णांक *inds)
{
    तथ्य d = {0};
    d.shallow = 1;
    d.w = orig[0].w;
    d.h = orig[0].h;

    d.X.rows = orig[0].X.rows;
    d.y.rows = orig[0].X.rows;

    d.X.cols = orig[0].X.cols;
    d.y.cols = orig[0].y.cols;

    d.X.vals = सुस्मृति(orig[0].X.rows, माप(भग्न *));
    d.y.vals = सुस्मृति(orig[0].y.rows, माप(भग्न *));
    पूर्णांक i;
    क्रम(i = 0; i < d.X.rows; ++i){
        d.X.vals[i] = orig[inds[i]].X.vals[i];
        d.y.vals[i] = orig[inds[i]].y.vals[i];
    }
    वापस d;
}

तथ्य *tile_तथ्य(तथ्य orig, पूर्णांक divs, पूर्णांक आकार)
{
    तथ्य *ds = सुस्मृति(divs*divs, माप(तथ्य));
    पूर्णांक i, j;
#आशय omp parallel क्रम
    क्रम(i = 0; i < divs*divs; ++i){
        तथ्य d;
        d.shallow = 0;
        d.w = orig.w/divs * आकार;
        d.h = orig.h/divs * आकार;
        d.X.rows = orig.X.rows;
        d.X.cols = d.w*d.h*3;
        d.X.vals = सुस्मृति(d.X.rows, माप(भग्न*));

        d.y = copy_matrix(orig.y);
#आशय omp parallel क्रम
        क्रम(j = 0; j < orig.X.rows; ++j){
            पूर्णांक x = (i%divs) * orig.w / divs - (d.w - orig.w/divs)/2;
            पूर्णांक y = (i/divs) * orig.h / divs - (d.h - orig.h/divs)/2;
            image im = float_to_image(orig.w, orig.h, 3, orig.X.vals[j]);
            d.X.vals[j] = crop_image(im, x, y, d.w, d.h).तथ्य;
        }
        ds[i] = d;
    }
    वापस ds;
}

तथ्य resize_तथ्य(तथ्य orig, पूर्णांक w, पूर्णांक h)
{
    तथ्य d = {0};
    d.shallow = 0;
    d.w = w;
    d.h = h;
    पूर्णांक i;
    d.X.rows = orig.X.rows;
    d.X.cols = w*h*3;
    d.X.vals = सुस्मृति(d.X.rows, माप(भग्न*));

    d.y = copy_matrix(orig.y);
#आशय omp parallel क्रम
    क्रम(i = 0; i < orig.X.rows; ++i){
        image im = float_to_image(orig.w, orig.h, 3, orig.X.vals[i]);
        d.X.vals[i] = resize_image(im, w, h).तथ्य;
    }
    वापस d;
}

तथ्य load_तथ्य_augment(अक्षर **paths, पूर्णांक n, पूर्णांक m, अक्षर **labels, पूर्णांक k, वृक्ष *hierarchy, पूर्णांक min, पूर्णांक max, पूर्णांक आकार, भग्न angle, भग्न aspect, भग्न hue, भग्न saturation, भग्न exposure, पूर्णांक center)
{
    अगर(m) paths = get_random_paths(paths, n, m);
    तथ्य d = {0};
    d.shallow = 0;
    d.w=आकार;
    d.h=आकार;
    d.X = load_image_augment_paths(paths, n, min, max, आकार, angle, aspect, hue, saturation, exposure, center);
    d.y = load_labels_paths(paths, n, labels, k, hierarchy);
    अगर(m) मुक्त(paths);
    वापस d;
}

तथ्य load_तथ्य_tag(अक्षर **paths, पूर्णांक n, पूर्णांक m, पूर्णांक k, पूर्णांक min, पूर्णांक max, पूर्णांक आकार, भग्न angle, भग्न aspect, भग्न hue, भग्न saturation, भग्न exposure)
{
    अगर(m) paths = get_random_paths(paths, n, m);
    तथ्य d = {0};
    d.w = आकार;
    d.h = आकार;
    d.shallow = 0;
    d.X = load_image_augment_paths(paths, n, min, max, आकार, angle, aspect, hue, saturation, exposure, 0);
    d.y = load_tags_paths(paths, n, k);
    अगर(m) मुक्त(paths);
    वापस d;
}

matrix concat_matrix(matrix m1, matrix m2)
{
    पूर्णांक i, गिनती = 0;
    matrix m;
    m.cols = m1.cols;
    m.rows = m1.rows+m2.rows;
    m.vals = सुस्मृति(m1.rows + m2.rows, माप(भग्न*));
    क्रम(i = 0; i < m1.rows; ++i){
        m.vals[गिनती++] = m1.vals[i];
    }
    क्रम(i = 0; i < m2.rows; ++i){
        m.vals[गिनती++] = m2.vals[i];
    }
    वापस m;
}

तथ्य concat_तथ्य(तथ्य d1, तथ्य d2)
{
    तथ्य d = {0};
    d.shallow = 1;
    d.X = concat_matrix(d1.X, d2.X);
    d.y = concat_matrix(d1.y, d2.y);
    d.w = d1.w;
    d.h = d1.h;
    वापस d;
}

तथ्य concat_datas(तथ्य *d, पूर्णांक n)
{
    पूर्णांक i;
    तथ्य out = {0};
    क्रम(i = 0; i < n; ++i){
        तथ्य new = concat_तथ्य(d[i], out);
        free_तथ्य(out);
        out = new;
    }
    वापस out;
}

तथ्य load_categorical_तथ्य_csv(अक्षर *filename, पूर्णांक target, पूर्णांक k)
{
    तथ्य d = {0};
    d.shallow = 0;
    matrix X = csv_to_matrix(filename);
    भग्न *truth_1d = निकालो_column(&X, target);
    भग्न **truth = one_hot_encode(truth_1d, X.rows, k);
    matrix y;
    y.rows = X.rows;
    y.cols = k;
    y.vals = truth;
    d.X = X;
    d.y = y;
    मुक्त(truth_1d);
    वापस d;
}

तथ्य load_cifar10_तथ्य(अक्षर *filename)
{
    तथ्य d = {0};
    d.shallow = 0;
    दीर्घ i,j;
    matrix X = make_matrix(10000, 3072);
    matrix y = make_matrix(10000, 10);
    d.X = X;
    d.y = y;

    खाता *fp = ख_खोलो(filename, "rb");
    अगर(!fp) file_error(filename);
    क्रम(i = 0; i < 10000; ++i){
        अचिन्हित अक्षर bytes[3073];
        ख_पढ़ो(bytes, 1, 3073, fp);
        पूर्णांक class = bytes[0];
        y.vals[i][class] = 1;
        क्रम(j = 0; j < X.cols; ++j){
            X.vals[i][j] = (द्विगुन)bytes[j+1];
        }
    }
    scale_तथ्य_rows(d, 1./255);
    //normalize_data_rows(d);
    ख_बंद(fp);
    वापस d;
}

व्योम get_random_batch(तथ्य d, पूर्णांक n, भग्न *X, भग्न *y)
{
    पूर्णांक j;
    क्रम(j = 0; j < n; ++j){
        पूर्णांक सूचकांक = अक्रम()%d.X.rows;
        स_नकल(X+j*d.X.cols, d.X.vals[सूचकांक], d.X.cols*माप(भग्न));
        स_नकल(y+j*d.y.cols, d.y.vals[सूचकांक], d.y.cols*माप(भग्न));
    }
}

व्योम get_अग्र_batch(तथ्य d, पूर्णांक n, पूर्णांक offset, भग्न *X, भग्न *y)
{
    पूर्णांक j;
    क्रम(j = 0; j < n; ++j){
        पूर्णांक सूचकांक = offset + j;
        स_नकल(X+j*d.X.cols, d.X.vals[सूचकांक], d.X.cols*माप(भग्न));
        अगर(y) स_नकल(y+j*d.y.cols, d.y.vals[सूचकांक], d.y.cols*माप(भग्न));
    }
}

व्योम smooth_तथ्य(तथ्य d)
{
    पूर्णांक i, j;
    भग्न scale = 1. / d.y.cols;
    भग्न eps = .1;
    क्रम(i = 0; i < d.y.rows; ++i){
        क्रम(j = 0; j < d.y.cols; ++j){
            d.y.vals[i][j] = eps * scale + (1-eps) * d.y.vals[i][j];
        }
    }
}

तथ्य load_all_cifar10()
{
    तथ्य d = {0};
    d.shallow = 0;
    पूर्णांक i,j,b;
    matrix X = make_matrix(50000, 3072);
    matrix y = make_matrix(50000, 10);
    d.X = X;
    d.y = y;


    क्रम(b = 0; b < 5; ++b){
        अक्षर buff[256];
        प्र_लिखो(buff, "data/cifar/cifar-10-batches-bin/data_batch_%d.bin", b+1);
        खाता *fp = ख_खोलो(buff, "rb");
        अगर(!fp) file_error(buff);
        क्रम(i = 0; i < 10000; ++i){
            अचिन्हित अक्षर bytes[3073];
            ख_पढ़ो(bytes, 1, 3073, fp);
            पूर्णांक class = bytes[0];
            y.vals[i+b*10000][class] = 1;
            क्रम(j = 0; j < X.cols; ++j){
                X.vals[i+b*10000][j] = (द्विगुन)bytes[j+1];
            }
        }
        ख_बंद(fp);
    }
    //normalize_data_rows(d);
    scale_तथ्य_rows(d, 1./255);
    smooth_तथ्य(d);
    वापस d;
}

तथ्य load_go(अक्षर *filename)
{
    खाता *fp = ख_खोलो(filename, "rb");
    matrix X = make_matrix(3363059, 361);
    matrix y = make_matrix(3363059, 361);
    पूर्णांक row, col;

    अगर(!fp) file_error(filename);
    अक्षर *label;
    पूर्णांक गिनती = 0;
    जबतक((label = fgetl(fp))){
        पूर्णांक i;
        अगर(गिनती == X.rows){
            X = resize_matrix(X, गिनती*2);
            y = resize_matrix(y, गिनती*2);
        }
        माला_पूछो(label, "%d %d", &row, &col);
        अक्षर *board = fgetl(fp);

        पूर्णांक सूचकांक = row*19 + col;
        y.vals[गिनती][सूचकांक] = 1;

        क्रम(i = 0; i < 19*19; ++i){
            भग्न val = 0;
            अगर(board[i] == '1') val = 1;
            अन्यथा अगर(board[i] == '2') val = -1;
            X.vals[गिनती][i] = val;
        }
        ++गिनती;
        मुक्त(label);
        मुक्त(board);
    }
    X = resize_matrix(X, गिनती);
    y = resize_matrix(y, गिनती);

    तथ्य d = {0};
    d.shallow = 0;
    d.X = X;
    d.y = y;


    ख_बंद(fp);

    वापस d;
}


व्योम randomize_तथ्य(तथ्य d)
{
    पूर्णांक i;
    क्रम(i = d.X.rows-1; i > 0; --i){
        पूर्णांक सूचकांक = अक्रम()%i;
        भग्न *swap = d.X.vals[सूचकांक];
        d.X.vals[सूचकांक] = d.X.vals[i];
        d.X.vals[i] = swap;

        swap = d.y.vals[सूचकांक];
        d.y.vals[सूचकांक] = d.y.vals[i];
        d.y.vals[i] = swap;
    }
}

व्योम scale_तथ्य_rows(तथ्य d, भग्न s)
{
    पूर्णांक i;
    क्रम(i = 0; i < d.X.rows; ++i){
        scale_array(d.X.vals[i], d.X.cols, s);
    }
}

व्योम translate_तथ्य_rows(तथ्य d, भग्न s)
{
    पूर्णांक i;
    क्रम(i = 0; i < d.X.rows; ++i){
        translate_array(d.X.vals[i], d.X.cols, s);
    }
}

तथ्य copy_तथ्य(तथ्य d)
{
    तथ्य c = {0};
    c.w = d.w;
    c.h = d.h;
    c.shallow = 0;
    c.अंक_boxes = d.अंक_boxes;
    c.boxes = d.boxes;
    c.X = copy_matrix(d.X);
    c.y = copy_matrix(d.y);
    वापस c;
}

व्योम normalize_तथ्य_rows(तथ्य d)
{
    पूर्णांक i;
    क्रम(i = 0; i < d.X.rows; ++i){
        normalize_array(d.X.vals[i], d.X.cols);
    }
}

तथ्य get_तथ्य_part(तथ्य d, पूर्णांक part, पूर्णांक योग_2)
{
    तथ्य p = {0};
    p.shallow = 1;
    p.X.rows = d.X.rows * (part + 1) / योग_2 - d.X.rows * part / योग_2;
    p.y.rows = d.y.rows * (part + 1) / योग_2 - d.y.rows * part / योग_2;
    p.X.cols = d.X.cols;
    p.y.cols = d.y.cols;
    p.X.vals = d.X.vals + d.X.rows * part / योग_2;
    p.y.vals = d.y.vals + d.y.rows * part / योग_2;
    वापस p;
}

तथ्य get_random_तथ्य(तथ्य d, पूर्णांक अंक)
{
    तथ्य r = {0};
    r.shallow = 1;

    r.X.rows = अंक;
    r.y.rows = अंक;

    r.X.cols = d.X.cols;
    r.y.cols = d.y.cols;

    r.X.vals = सुस्मृति(अंक, माप(भग्न *));
    r.y.vals = सुस्मृति(अंक, माप(भग्न *));

    पूर्णांक i;
    क्रम(i = 0; i < अंक; ++i){
        पूर्णांक सूचकांक = अक्रम()%d.X.rows;
        r.X.vals[i] = d.X.vals[सूचकांक];
        r.y.vals[i] = d.y.vals[सूचकांक];
    }
    वापस r;
}

तथ्य *split_तथ्य(तथ्य d, पूर्णांक part, पूर्णांक योग_2)
{
    तथ्य *split = सुस्मृति(2, माप(तथ्य));
    पूर्णांक i;
    पूर्णांक start = part*d.X.rows/योग_2;
    पूर्णांक end = (part+1)*d.X.rows/योग_2;
    तथ्य train;
    तथ्य test;
    train.shallow = test.shallow = 1;

    test.X.rows = test.y.rows = end-start;
    train.X.rows = train.y.rows = d.X.rows - (end-start);
    train.X.cols = test.X.cols = d.X.cols;
    train.y.cols = test.y.cols = d.y.cols;

    train.X.vals = सुस्मृति(train.X.rows, माप(भग्न*));
    test.X.vals = सुस्मृति(test.X.rows, माप(भग्न*));
    train.y.vals = सुस्मृति(train.y.rows, माप(भग्न*));
    test.y.vals = सुस्मृति(test.y.rows, माप(भग्न*));

    क्रम(i = 0; i < start; ++i){
        train.X.vals[i] = d.X.vals[i];
        train.y.vals[i] = d.y.vals[i];
    }
    क्रम(i = start; i < end; ++i){
        test.X.vals[i-start] = d.X.vals[i];
        test.y.vals[i-start] = d.y.vals[i];
    }
    क्रम(i = end; i < d.X.rows; ++i){
        train.X.vals[i-(end-start)] = d.X.vals[i];
        train.y.vals[i-(end-start)] = d.y.vals[i];
    }
    split[0] = train;
    split[1] = test;
    वापस split;
}

