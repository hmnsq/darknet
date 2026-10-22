#अन्तर्भाव "data.h"
#अन्तर्भाव "utils.h"
#अन्तर्भाव "image.h"
#अन्तर्भाव "cuda.h"

#अन्तर्भाव <stdio.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <string.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

list *get_paths(अक्षर *filename)
{
    अक्षर *path;
    खाता *file = ख_खोलो(filename, "r");
    जर(!file) file_error(filename);
    list *ओळी = make_list();
    पर्यंत((path=fgetl(file))){
        list_घाला(ओळी, path);
    }
    ख_बंद(file);
    परत ओळी;
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
    अक्षर **random_paths = सुस्मृति(n, आकार(अक्षर*));
    पूर्णांक i;
    pthread_mutex_lock(&mutex);
    साठी(i = 0; i < n; ++i){
        पूर्णांक index = अक्रम()%m;
        random_paths[i] = paths[index];
        //if(i == 0) printf("%s\n", paths[index]);
    }
    pthread_mutex_unlock(&mutex);
    परत random_paths;
}

अक्षर **find_replace_paths(अक्षर **paths, पूर्णांक n, अक्षर *find, अक्षर *replace)
{
    अक्षर **replace_paths = सुस्मृति(n, आकार(अक्षर*));
    पूर्णांक i;
    साठी(i = 0; i < n; ++i){
        अक्षर replaced[4096];
        find_replace(paths[i], find, replace, replaced);
        replace_paths[i] = copy_string(replaced);
    }
    परत replace_paths;
}

matrix load_image_paths_gray(अक्षर **paths, पूर्णांक n, पूर्णांक w, पूर्णांक h)
{
    पूर्णांक i;
    matrix X;
    X.rows = n;
    X.vals = सुस्मृति(X.rows, आकार(दशांश*));
    X.cols = 0;

    साठी(i = 0; i < n; ++i){
        image im = load_image(paths[i], w, h, 3);

        image gray = grayscale_image(im);
        free_image(im);
        im = gray;

        X.vals[i] = im.data;
        X.cols = im.h*im.w*im.c;
    }
    परत X;
}

matrix load_image_paths(अक्षर **paths, पूर्णांक n, पूर्णांक w, पूर्णांक h)
{
    पूर्णांक i;
    matrix X;
    X.rows = n;
    X.vals = सुस्मृति(X.rows, आकार(दशांश*));
    X.cols = 0;

    साठी(i = 0; i < n; ++i){
        image im = load_image_color(paths[i], w, h);
        X.vals[i] = im.data;
        X.cols = im.h*im.w*im.c;
    }
    परत X;
}

matrix load_image_augment_paths(अक्षर **paths, पूर्णांक n, पूर्णांक min, पूर्णांक max, पूर्णांक size, दशांश angle, दशांश aspect, दशांश hue, दशांश saturation, दशांश exposure, पूर्णांक center)
{
    पूर्णांक i;
    matrix X;
    X.rows = n;
    X.vals = सुस्मृति(X.rows, आकार(दशांश*));
    X.cols = 0;

    साठी(i = 0; i < n; ++i){
        image im = load_image_color(paths[i], 0, 0);
        image crop;
        जर(center){
            crop = center_crop_image(im, size, size);
        } अन्यथा {
            crop = random_augment_image(im, angle, aspect, min, max, size, size);
        }
        पूर्णांक flip = अक्रम()%2;
        जर (flip) flip_image(crop);
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
    परत X;
}


box_label *read_boxes(अक्षर *filename, पूर्णांक *n)
{
    खाता *file = ख_खोलो(filename, "r");
    जर(!file) file_error(filename);
    दशांश x, y, h, w;
    पूर्णांक id;
    पूर्णांक मोजणी = 0;
    पूर्णांक size = 64;
    box_label *boxes = सुस्मृति(size, आकार(box_label));
    पर्यंत(ख_पूछो(file, "%d %f %f %f %f", &id, &x, &y, &w, &h) == 5){
        जर(मोजणी == size) {
            size = size * 2;
            boxes = पुनः_स्मृति(boxes, size*आकार(box_label));
        }
        boxes[मोजणी].id = id;
        boxes[मोजणी].x = x;
        boxes[मोजणी].y = y;
        boxes[मोजणी].h = h;
        boxes[मोजणी].w = w;
        boxes[मोजणी].डावा   = x - w/2;
        boxes[मोजणी].उजवा  = x + w/2;
        boxes[मोजणी].शीर्ष    = y - h/2;
        boxes[मोजणी].bottom = y + h/2;
        ++मोजणी;
    }
    ख_बंद(file);
    *n = मोजणी;
    परत boxes;
}

शून्य randomize_boxes(box_label *b, पूर्णांक n)
{
    पूर्णांक i;
    साठी(i = 0; i < n; ++i){
        box_label swap = b[i];
        पूर्णांक index = अक्रम()%n;
        b[i] = b[index];
        b[index] = swap;
    }
}

शून्य correct_boxes(box_label *boxes, पूर्णांक n, दशांश dx, दशांश dy, दशांश sx, दशांश sy, पूर्णांक flip)
{
    पूर्णांक i;
    साठी(i = 0; i < n; ++i){
        जर(boxes[i].x == 0 && boxes[i].y == 0) {
            boxes[i].x = 999999;
            boxes[i].y = 999999;
            boxes[i].w = 999999;
            boxes[i].h = 999999;
            सुरूठेवा;
        }
        boxes[i].डावा   = boxes[i].डावा  * sx - dx;
        boxes[i].उजवा  = boxes[i].उजवा * sx - dx;
        boxes[i].शीर्ष    = boxes[i].शीर्ष   * sy - dy;
        boxes[i].bottom = boxes[i].bottom* sy - dy;

        जर(flip){
            दशांश swap = boxes[i].डावा;
            boxes[i].डावा = 1. - boxes[i].उजवा;
            boxes[i].उजवा = 1. - swap;
        }

        boxes[i].डावा =  constrain(0, 1, boxes[i].डावा);
        boxes[i].उजवा = constrain(0, 1, boxes[i].उजवा);
        boxes[i].शीर्ष =   constrain(0, 1, boxes[i].शीर्ष);
        boxes[i].bottom =   constrain(0, 1, boxes[i].bottom);

        boxes[i].x = (boxes[i].डावा+boxes[i].उजवा)/2;
        boxes[i].y = (boxes[i].शीर्ष+boxes[i].bottom)/2;
        boxes[i].w = (boxes[i].उजवा - boxes[i].डावा);
        boxes[i].h = (boxes[i].bottom - boxes[i].शीर्ष);

        boxes[i].w = constrain(0, 1, boxes[i].w);
        boxes[i].h = constrain(0, 1, boxes[i].h);
    }
}

शून्य fill_truth_swag(अक्षर *path, दशांश *truth, पूर्णांक classes, पूर्णांक flip, दशांश dx, दशांश dy, दशांश sx, दशांश sy)
{
    अक्षर labelpath[4096];
    find_replace(path, "images", "labels", labelpath);
    find_replace(labelpath, "JPEGImages", "labels", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);

    पूर्णांक मोजणी = 0;
    box_label *boxes = read_boxes(labelpath, &मोजणी);
    randomize_boxes(boxes, मोजणी);
    correct_boxes(boxes, मोजणी, dx, dy, sx, sy, flip);
    दशांश x,y,w,h;
    पूर्णांक id;
    पूर्णांक i;

    साठी (i = 0; i < मोजणी && i < 90; ++i) {
        x =  boxes[i].x;
        y =  boxes[i].y;
        w =  boxes[i].w;
        h =  boxes[i].h;
        id = boxes[i].id;

        जर (w < .0 || h < .0) सुरूठेवा;

        पूर्णांक index = (4+classes) * i;

        truth[index++] = x;
        truth[index++] = y;
        truth[index++] = w;
        truth[index++] = h;

        जर (id < classes) truth[index+id] = 1;
    }
    मुक्त(boxes);
}

शून्य fill_truth_region(अक्षर *path, दशांश *truth, पूर्णांक classes, पूर्णांक अंक_boxes, पूर्णांक flip, दशांश dx, दशांश dy, दशांश sx, दशांश sy)
{
    अक्षर labelpath[4096];
    find_replace(path, "images", "labels", labelpath);
    find_replace(labelpath, "JPEGImages", "labels", labelpath);

    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".png", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    पूर्णांक मोजणी = 0;
    box_label *boxes = read_boxes(labelpath, &मोजणी);
    randomize_boxes(boxes, मोजणी);
    correct_boxes(boxes, मोजणी, dx, dy, sx, sy, flip);
    दशांश x,y,w,h;
    पूर्णांक id;
    पूर्णांक i;

    साठी (i = 0; i < मोजणी; ++i) {
        x =  boxes[i].x;
        y =  boxes[i].y;
        w =  boxes[i].w;
        h =  boxes[i].h;
        id = boxes[i].id;

        जर (w < .005 || h < .005) सुरूठेवा;

        पूर्णांक col = (पूर्णांक)(x*अंक_boxes);
        पूर्णांक row = (पूर्णांक)(y*अंक_boxes);

        x = x*अंक_boxes - col;
        y = y*अंक_boxes - row;

        पूर्णांक index = (col+row*अंक_boxes)*(5+classes);
        जर (truth[index]) सुरूठेवा;
        truth[index++] = 1;

        जर (id < classes) truth[index+id] = 1;
        index += classes;

        truth[index++] = x;
        truth[index++] = y;
        truth[index++] = w;
        truth[index++] = h;
    }
    मुक्त(boxes);
}

शून्य load_rle(image im, पूर्णांक *rle, पूर्णांक n)
{
    पूर्णांक मोजणी = 0;
    पूर्णांक curr = 0;
    पूर्णांक i,j;
    साठी(i = 0; i < n; ++i){
        साठी(j = 0; j < rle[i]; ++j){
            im.data[मोजणी++] = curr;
        }
        curr = 1 - curr;
    }
    साठी(; मोजणी < im.h*im.w*im.c; ++मोजणी){
        im.data[मोजणी] = curr;
    }
}

शून्य or_image(image src, image dest, पूर्णांक c)
{
    पूर्णांक i;
    साठी(i = 0; i < src.w*src.h; ++i){
        जर(src.data[i]) dest.data[dest.w*dest.h*c + i] = 1;
    }
}

शून्य exclusive_image(image src)
{
    पूर्णांक k, j, i;
    पूर्णांक s = src.w*src.h;
    साठी(k = 0; k < src.c-1; ++k){
        साठी(i = 0; i < s; ++i){
            जर (src.data[k*s + i]){
                साठी(j = k+1; j < src.c; ++j){
                    src.data[j*s + i] = 0;
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
    साठी(y = 0; y < im.h; ++y){
        साठी(x = 0; x < im.w; ++x){
            जर(im.data[y*im.w + x]){
                minx = (x < minx) ? x : minx;
                miny = (y < miny) ? y : miny;
                maxx = (x > maxx) ? x : maxx;
                maxy = (y > maxy) ? y : maxy;
            }
        }
    }
    box b = {minx, miny, maxx-minx + 1, maxy-miny + 1};
    //printf("%f %f %f %f\n", b.x, b.y, b.w, b.h);
    परत b;
}

शून्य fill_truth_iseg(अक्षर *path, पूर्णांक अंक_boxes, दशांश *truth, पूर्णांक classes, पूर्णांक w, पूर्णांक h, augment_args aug, पूर्णांक flip, पूर्णांक mw, पूर्णांक mh)
{
    अक्षर labelpath[4096];
    find_replace(path, "images", "mask", labelpath);
    find_replace(labelpath, "JPEGImages", "mask", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    खाता *file = ख_खोलो(labelpath, "r");
    जर(!file) file_error(labelpath);
    अक्षर buff[32788];
    पूर्णांक id;
    पूर्णांक i = 0;
    पूर्णांक j;
    image part = make_image(w, h, 1);
    पर्यंत((ख_पूछो(file, "%d %s", &id, buff) == 2) && i < अंक_boxes){
        पूर्णांक n = 0;
        पूर्णांक *rle = read_intlist(buff, &n, 0);
        load_rle(part, rle, n);
        image sized = rotate_crop_image(part, aug.rad, aug.scale, aug.w, aug.h, aug.dx, aug.dy, aug.aspect);
        जर(flip) flip_image(sized);

        image mask = resize_image(sized, mw, mh);
        truth[i*(mw*mh+1)] = id;
        साठी(j = 0; j < mw*mh; ++j){
            truth[i*(mw*mh + 1) + 1 + j] = mask.data[j];
        }
        ++i;

        free_image(mask);
        free_image(sized);
        मुक्त(rle);
    }
    जर(i < अंक_boxes) truth[i*(mw*mh+1)] = -1;
    ख_बंद(file);
    free_image(part);
}

शून्य fill_truth_mask(अक्षर *path, पूर्णांक अंक_boxes, दशांश *truth, पूर्णांक classes, पूर्णांक w, पूर्णांक h, augment_args aug, पूर्णांक flip, पूर्णांक mw, पूर्णांक mh)
{
    अक्षर labelpath[4096];
    find_replace(path, "images", "mask", labelpath);
    find_replace(labelpath, "JPEGImages", "mask", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    खाता *file = ख_खोलो(labelpath, "r");
    जर(!file) file_error(labelpath);
    अक्षर buff[32788];
    पूर्णांक id;
    पूर्णांक i = 0;
    image part = make_image(w, h, 1);
    पर्यंत((ख_पूछो(file, "%d %s", &id, buff) == 2) && i < अंक_boxes){
        पूर्णांक n = 0;
        पूर्णांक *rle = read_intlist(buff, &n, 0);
        load_rle(part, rle, n);
        image sized = rotate_crop_image(part, aug.rad, aug.scale, aug.w, aug.h, aug.dx, aug.dy, aug.aspect);
        जर(flip) flip_image(sized);
        box b = bound_image(sized);
        जर(b.w > 0){
            image crop = crop_image(sized, b.x, b.y, b.w, b.h);
            image mask = resize_image(crop, mw, mh);
            truth[i*(4 + mw*mh + 1) + 0] = (b.x + b.w/2.)/sized.w;
            truth[i*(4 + mw*mh + 1) + 1] = (b.y + b.h/2.)/sized.h;
            truth[i*(4 + mw*mh + 1) + 2] = b.w/sized.w;
            truth[i*(4 + mw*mh + 1) + 3] = b.h/sized.h;
            पूर्णांक j;
            साठी(j = 0; j < mw*mh; ++j){
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
    ख_बंद(file);
    free_image(part);
}


शून्य fill_truth_detection(अक्षर *path, पूर्णांक अंक_boxes, दशांश *truth, पूर्णांक classes, पूर्णांक flip, दशांश dx, दशांश dy, दशांश sx, दशांश sy)
{
    अक्षर labelpath[4096];
    find_replace(path, "images", "labels", labelpath);
    find_replace(labelpath, "JPEGImages", "labels", labelpath);

    find_replace(labelpath, "raw", "labels", labelpath);
    find_replace(labelpath, ".jpg", ".txt", labelpath);
    find_replace(labelpath, ".png", ".txt", labelpath);
    find_replace(labelpath, ".JPG", ".txt", labelpath);
    find_replace(labelpath, ".JPEG", ".txt", labelpath);
    पूर्णांक मोजणी = 0;
    box_label *boxes = read_boxes(labelpath, &मोजणी);
    randomize_boxes(boxes, मोजणी);
    correct_boxes(boxes, मोजणी, dx, dy, sx, sy, flip);
    जर(मोजणी > अंक_boxes) मोजणी = अंक_boxes;
    दशांश x,y,w,h;
    पूर्णांक id;
    पूर्णांक i;
    पूर्णांक sub = 0;

    साठी (i = 0; i < मोजणी; ++i) {
        x =  boxes[i].x;
        y =  boxes[i].y;
        w =  boxes[i].w;
        h =  boxes[i].h;
        id = boxes[i].id;

        जर ((w < .001 || h < .001)) {
            ++sub;
            सुरूठेवा;
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

शून्य print_letters(दशांश *pred, पूर्णांक n)
{
    पूर्णांक i;
    साठी(i = 0; i < n; ++i){
        पूर्णांक index = max_index(pred+i*NUMCHARS, NUMCHARS);
        म_लिखो("%c", int_to_alphanum(index));
    }
    म_लिखो("\n");
}

शून्य fill_truth_captcha(अक्षर *path, पूर्णांक n, दशांश *truth)
{
    अक्षर *begin = म_खोजप(path, '/');
    ++begin;
    पूर्णांक i;
    साठी(i = 0; i < म_माप(begin) && i < n && begin[i] != '.'; ++i){
        पूर्णांक index = alphanum_to_int(begin[i]);
        जर(index > 35) म_लिखो("Bad %c\n", begin[i]);
        truth[i*NUMCHARS+index] = 1;
    }
    साठी(;i < n; ++i){
        truth[i*NUMCHARS + NUMCHARS-1] = 1;
    }
}

data load_data_captcha(अक्षर **paths, पूर्णांक n, पूर्णांक m, पूर्णांक k, पूर्णांक w, पूर्णांक h)
{
    जर(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;
    d.X = load_image_paths(paths, n, w, h);
    d.y = make_matrix(n, k*NUMCHARS);
    पूर्णांक i;
    साठी(i = 0; i < n; ++i){
        fill_truth_captcha(paths[i], k, d.y.vals[i]);
    }
    जर(m) मुक्त(paths);
    परत d;
}

data load_data_captcha_encode(अक्षर **paths, पूर्णांक n, पूर्णांक m, पूर्णांक w, पूर्णांक h)
{
    जर(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;
    d.X = load_image_paths(paths, n, w, h);
    d.X.cols = 17100;
    d.y = d.X;
    जर(m) मुक्त(paths);
    परत d;
}

शून्य fill_truth(अक्षर *path, अक्षर **labels, पूर्णांक k, दशांश *truth)
{
    पूर्णांक i;
    स_रखो(truth, 0, k*आकार(दशांश));
    पूर्णांक मोजणी = 0;
    साठी(i = 0; i < k; ++i){
        जर(म_माला(path, labels[i])){
            truth[i] = 1;
            ++मोजणी;
            //printf("%s %s %d\n", path, labels[i], i);
        }
    }
    जर(मोजणी != 1 && (k != 1 || मोजणी != 0)) म_लिखो("Too many or too few labels: %d, %s\n", मोजणी, path);
}

शून्य fill_hierarchy(दशांश *truth, पूर्णांक k, वृक्ष *hierarchy)
{
    पूर्णांक j;
    साठी(j = 0; j < k; ++j){
        जर(truth[j]){
            पूर्णांक parent = hierarchy->parent[j];
            पर्यंत(parent >= 0){
                truth[parent] = 1;
                parent = hierarchy->parent[parent];
            }
        }
    }
    पूर्णांक i;
    पूर्णांक मोजणी = 0;
    साठी(j = 0; j < hierarchy->groups; ++j){
        //printf("%d\n", count);
        पूर्णांक mask = 1;
        साठी(i = 0; i < hierarchy->group_size[j]; ++i){
            जर(truth[मोजणी + i]){
                mask = 0;
                थांबा;
            }
        }
        जर (mask) {
            साठी(i = 0; i < hierarchy->group_size[j]; ++i){
                truth[मोजणी + i] = SECRET_अंक;
            }
        }
        मोजणी += hierarchy->group_size[j];
    }
}

matrix load_regression_labels_paths(अक्षर **paths, पूर्णांक n, पूर्णांक k)
{
    matrix y = make_matrix(n, k);
    पूर्णांक i,j;
    साठी(i = 0; i < n; ++i){
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
        साठी(j = 0; j < k; ++j){
            ख_पूछो(file, "%f", &(y.vals[i][j]));
        }
        ख_बंद(file);
    }
    परत y;
}

matrix load_labels_paths(अक्षर **paths, पूर्णांक n, अक्षर **labels, पूर्णांक k, वृक्ष *hierarchy)
{
    matrix y = make_matrix(n, k);
    पूर्णांक i;
    साठी(i = 0; i < n && labels; ++i){
        fill_truth(paths[i], labels, k, y.vals[i]);
        जर(hierarchy){
            fill_hierarchy(y.vals[i], k, hierarchy);
        }
    }
    परत y;
}

matrix load_tags_paths(अक्षर **paths, पूर्णांक n, पूर्णांक k)
{
    matrix y = make_matrix(n, k);
    पूर्णांक i;
    //int count = 0;
    साठी(i = 0; i < n; ++i){
        अक्षर label[4096];
        find_replace(paths[i], "images", "labels", label);
        find_replace(label, ".jpg", ".txt", label);
        खाता *file = ख_खोलो(label, "r");
        जर (!file) सुरूठेवा;
        //++count;
        पूर्णांक tag;
        पर्यंत(ख_पूछो(file, "%d", &tag) == 1){
            जर(tag < k){
                y.vals[i][tag] = 1;
            }
        }
        ख_बंद(file);
    }
    //printf("%d/%d\n", count, n);
    परत y;
}

अक्षर **get_labels(अक्षर *filename)
{
    list *plist = get_paths(filename);
    अक्षर **labels = (अक्षर **)list_to_array(plist);
    free_list(plist);
    परत labels;
}

शून्य free_data(data d)
{
    जर(!d.shallow){
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
    जर(!file) file_error(labelpath);
    अक्षर buff[32788];
    पूर्णांक id;
    image part = make_image(w, h, 1);
    पर्यंत(ख_पूछो(file, "%d %s", &id, buff) == 2){
        पूर्णांक n = 0;
        पूर्णांक *rle = read_intlist(buff, &n, 0);
        load_rle(part, rle, n);
        or_image(part, mask, id);
        मुक्त(rle);
    }
    //exclusive_image(mask);
    ख_बंद(file);
    free_image(part);
    परत mask;
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
    साठी(i = 0; i < w*h; ++i){
        mask.data[w*h*classes + i] = 1;
    }
    खाता *file = ख_खोलो(labelpath, "r");
    जर(!file) file_error(labelpath);
    अक्षर buff[32788];
    पूर्णांक id;
    image part = make_image(w, h, 1);
    पर्यंत(ख_पूछो(file, "%d %s", &id, buff) == 2){
        पूर्णांक n = 0;
        पूर्णांक *rle = read_intlist(buff, &n, 0);
        load_rle(part, rle, n);
        or_image(part, mask, id);
        साठी(i = 0; i < w*h; ++i){
            जर(part.data[i]) mask.data[w*h*classes + i] = 0;
        }
        मुक्त(rle);
    }
    //exclusive_image(mask);
    ख_बंद(file);
    free_image(part);
    परत mask;
}

data load_data_seg(पूर्णांक n, अक्षर **paths, पूर्णांक m, पूर्णांक w, पूर्णांक h, पूर्णांक classes, पूर्णांक min, पूर्णांक max, दशांश angle, दशांश aspect, दशांश hue, दशांश saturation, दशांश exposure, पूर्णांक भाग)
{
    अक्षर **random_paths = get_random_paths(paths, n, m);
    पूर्णांक i;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = सुस्मृति(d.X.rows, आकार(दशांश*));
    d.X.cols = h*w*3;


    d.y.rows = n;
    d.y.cols = h*w*classes/भाग/भाग;
    d.y.vals = सुस्मृति(d.X.rows, आकार(दशांश*));

    साठी(i = 0; i < n; ++i){
        image orig = load_image_color(random_paths[i], 0, 0);
        augment_args a = random_augment_args(orig, angle, aspect, min, max, w, h);
        image sized = rotate_crop_image(orig, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);

        पूर्णांक flip = अक्रम()%2;
        जर(flip) flip_image(sized);
        random_distort_image(sized, hue, saturation, exposure);
        d.X.vals[i] = sized.data;

        image mask = get_segmentation_image(random_paths[i], orig.w, orig.h, classes);
        //image mask = make_image(orig.w, orig.h, classes+1);
        image sized_m = rotate_crop_image(mask, a.rad, a.scale/भाग, a.w/भाग, a.h/भाग, a.dx/भाग, a.dy/भाग, a.aspect);

        जर(flip) flip_image(sized_m);
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
    परत d;
}

data load_data_iseg(पूर्णांक n, अक्षर **paths, पूर्णांक m, पूर्णांक w, पूर्णांक h, पूर्णांक classes, पूर्णांक boxes, पूर्णांक भाग, पूर्णांक min, पूर्णांक max, दशांश angle, दशांश aspect, दशांश hue, दशांश saturation, दशांश exposure)
{
    अक्षर **random_paths = get_random_paths(paths, n, m);
    पूर्णांक i;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = सुस्मृति(d.X.rows, आकार(दशांश*));
    d.X.cols = h*w*3;

    d.y = make_matrix(n, (((w/भाग)*(h/भाग))+1)*boxes);

    साठी(i = 0; i < n; ++i){
        image orig = load_image_color(random_paths[i], 0, 0);
        augment_args a = random_augment_args(orig, angle, aspect, min, max, w, h);
        image sized = rotate_crop_image(orig, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);

        पूर्णांक flip = अक्रम()%2;
        जर(flip) flip_image(sized);
        random_distort_image(sized, hue, saturation, exposure);
        d.X.vals[i] = sized.data;
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
    परत d;
}

data load_data_mask(पूर्णांक n, अक्षर **paths, पूर्णांक m, पूर्णांक w, पूर्णांक h, पूर्णांक classes, पूर्णांक boxes, पूर्णांक coords, पूर्णांक min, पूर्णांक max, दशांश angle, दशांश aspect, दशांश hue, दशांश saturation, दशांश exposure)
{
    अक्षर **random_paths = get_random_paths(paths, n, m);
    पूर्णांक i;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = सुस्मृति(d.X.rows, आकार(दशांश*));
    d.X.cols = h*w*3;

    d.y = make_matrix(n, (coords+1)*boxes);

    साठी(i = 0; i < n; ++i){
        image orig = load_image_color(random_paths[i], 0, 0);
        augment_args a = random_augment_args(orig, angle, aspect, min, max, w, h);
        image sized = rotate_crop_image(orig, a.rad, a.scale, a.w, a.h, a.dx, a.dy, a.aspect);

        पूर्णांक flip = अक्रम()%2;
        जर(flip) flip_image(sized);
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
    परत d;
}

data load_data_region(पूर्णांक n, अक्षर **paths, पूर्णांक m, पूर्णांक w, पूर्णांक h, पूर्णांक size, पूर्णांक classes, दशांश jitter, दशांश hue, दशांश saturation, दशांश exposure)
{
    अक्षर **random_paths = get_random_paths(paths, n, m);
    पूर्णांक i;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = सुस्मृति(d.X.rows, आकार(दशांश*));
    d.X.cols = h*w*3;


    पूर्णांक k = size*size*(5+classes);
    d.y = make_matrix(n, k);
    साठी(i = 0; i < n; ++i){
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

        दशांश sx = (दशांश)swidth  / ow;
        दशांश sy = (दशांश)sheight / oh;

        पूर्णांक flip = अक्रम()%2;
        image cropped = crop_image(orig, pleft, ptop, swidth, sheight);

        दशांश dx = ((दशांश)pleft/ow)/sx;
        दशांश dy = ((दशांश)ptop /oh)/sy;

        image sized = resize_image(cropped, w, h);
        जर(flip) flip_image(sized);
        random_distort_image(sized, hue, saturation, exposure);
        d.X.vals[i] = sized.data;

        fill_truth_region(random_paths[i], d.y.vals[i], classes, size, flip, dx, dy, 1./sx, 1./sy);

        free_image(orig);
        free_image(cropped);
    }
    मुक्त(random_paths);
    परत d;
}

data load_data_compare(पूर्णांक n, अक्षर **paths, पूर्णांक m, पूर्णांक classes, पूर्णांक w, पूर्णांक h)
{
    जर(m) paths = get_random_paths(paths, 2*n, m);
    पूर्णांक i,j;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = सुस्मृति(d.X.rows, आकार(दशांश*));
    d.X.cols = h*w*6;

    पूर्णांक k = 2*(classes);
    d.y = make_matrix(n, k);
    साठी(i = 0; i < n; ++i){
        image im1 = load_image_color(paths[i*2],   w, h);
        image im2 = load_image_color(paths[i*2+1], w, h);

        d.X.vals[i] = सुस्मृति(d.X.cols, आकार(दशांश));
        स_नकल(d.X.vals[i],         im1.data, h*w*3*आकार(दशांश));
        स_नकल(d.X.vals[i] + h*w*3, im2.data, h*w*3*आकार(दशांश));

        पूर्णांक id;
        दशांश iou;

        अक्षर imlabel1[4096];
        अक्षर imlabel2[4096];
        find_replace(paths[i*2],   "imgs", "labels", imlabel1);
        find_replace(imlabel1, "jpg", "txt", imlabel1);
        खाता *fp1 = ख_खोलो(imlabel1, "r");

        पर्यंत(ख_पूछो(fp1, "%d %f", &id, &iou) == 2){
            जर (d.y.vals[i][2*id] < iou) d.y.vals[i][2*id] = iou;
        }

        find_replace(paths[i*2+1], "imgs", "labels", imlabel2);
        find_replace(imlabel2, "jpg", "txt", imlabel2);
        खाता *fp2 = ख_खोलो(imlabel2, "r");

        पर्यंत(ख_पूछो(fp2, "%d %f", &id, &iou) == 2){
            जर (d.y.vals[i][2*id + 1] < iou) d.y.vals[i][2*id + 1] = iou;
        }

        साठी (j = 0; j < classes; ++j){
            जर (d.y.vals[i][2*j] > .5 &&  d.y.vals[i][2*j+1] < .5){
                d.y.vals[i][2*j] = 1;
                d.y.vals[i][2*j+1] = 0;
            } अन्यथा जर (d.y.vals[i][2*j] < .5 &&  d.y.vals[i][2*j+1] > .5){
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
    जर(m) मुक्त(paths);
    परत d;
}

data load_data_swag(अक्षर **paths, पूर्णांक n, पूर्णांक classes, दशांश jitter)
{
    पूर्णांक index = अक्रम()%n;
    अक्षर *random_path = paths[index];

    image orig = load_image_color(random_path, 0, 0);
    पूर्णांक h = orig.h;
    पूर्णांक w = orig.w;

    data d = {0};
    d.shallow = 0;
    d.w = w;
    d.h = h;

    d.X.rows = 1;
    d.X.vals = सुस्मृति(d.X.rows, आकार(दशांश*));
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

    दशांश sx = (दशांश)swidth  / w;
    दशांश sy = (दशांश)sheight / h;

    पूर्णांक flip = अक्रम()%2;
    image cropped = crop_image(orig, pleft, ptop, swidth, sheight);

    दशांश dx = ((दशांश)pleft/w)/sx;
    दशांश dy = ((दशांश)ptop /h)/sy;

    image sized = resize_image(cropped, w, h);
    जर(flip) flip_image(sized);
    d.X.vals[0] = sized.data;

    fill_truth_swag(random_path, d.y.vals[0], classes, flip, dx, dy, 1./sx, 1./sy);

    free_image(orig);
    free_image(cropped);

    परत d;
}

data load_data_detection(पूर्णांक n, अक्षर **paths, पूर्णांक m, पूर्णांक w, पूर्णांक h, पूर्णांक boxes, पूर्णांक classes, दशांश jitter, दशांश hue, दशांश saturation, दशांश exposure)
{
    अक्षर **random_paths = get_random_paths(paths, n, m);
    पूर्णांक i;
    data d = {0};
    d.shallow = 0;

    d.X.rows = n;
    d.X.vals = सुस्मृति(d.X.rows, आकार(दशांश*));
    d.X.cols = h*w*3;

    d.y = make_matrix(n, 5*boxes);
    साठी(i = 0; i < n; ++i){
        image orig = load_image_color(random_paths[i], 0, 0);
        image sized = make_image(w, h, orig.c);
        fill_image(sized, .5);

        दशांश dw = jitter * orig.w;
        दशांश dh = jitter * orig.h;

        दशांश new_ar = (orig.w + rand_uniform(-dw, dw)) / (orig.h + rand_uniform(-dh, dh));
        //float scale = rand_uniform(.25, 2);
        दशांश scale = 1;

        दशांश nw, nh;

        जर(new_ar < 1){
            nh = scale * h;
            nw = nh * new_ar;
        } अन्यथा {
            nw = scale * w;
            nh = nw / new_ar;
        }

        दशांश dx = rand_uniform(0, w - nw);
        दशांश dy = rand_uniform(0, h - nh);

        place_image(orig, nw, nh, dx, dy, sized);

        random_distort_image(sized, hue, saturation, exposure);

        पूर्णांक flip = अक्रम()%2;
        जर(flip) flip_image(sized);
        d.X.vals[i] = sized.data;


        fill_truth_detection(random_paths[i], boxes, d.y.vals[i], classes, flip, -dx/w, -dy/h, nw/w, nh/h);

        free_image(orig);
    }
    मुक्त(random_paths);
    परत d;
}

शून्य *load_thread(शून्य *ptr)
{
    //printf("Loading data: %d\n", rand());
    load_args a = *(संरचना load_args*)ptr;
    जर(a.exposure == 0) a.exposure = 1;
    जर(a.saturation == 0) a.saturation = 1;
    जर(a.aspect == 0) a.aspect = 1;

    जर (a.type == OLD_CLASSIFICATION_DATA){
        *a.d = load_data_old(a.paths, a.n, a.m, a.labels, a.classes, a.w, a.h);
    } अन्यथा जर (a.type == REGRESSION_DATA){
        *a.d = load_data_regression(a.paths, a.n, a.m, a.classes, a.min, a.max, a.size, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    } अन्यथा जर (a.type == CLASSIFICATION_DATA){
        *a.d = load_data_augment(a.paths, a.n, a.m, a.labels, a.classes, a.hierarchy, a.min, a.max, a.size, a.angle, a.aspect, a.hue, a.saturation, a.exposure, a.center);
    } अन्यथा जर (a.type == SUPER_DATA){
        *a.d = load_data_super(a.paths, a.n, a.m, a.w, a.h, a.scale);
    } अन्यथा जर (a.type == WRITING_DATA){
        *a.d = load_data_writing(a.paths, a.n, a.m, a.w, a.h, a.out_w, a.out_h);
    } अन्यथा जर (a.type == ISEG_DATA){
        *a.d = load_data_iseg(a.n, a.paths, a.m, a.w, a.h, a.classes, a.अंक_boxes, a.scale, a.min, a.max, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    } अन्यथा जर (a.type == INSTANCE_DATA){
        *a.d = load_data_mask(a.n, a.paths, a.m, a.w, a.h, a.classes, a.अंक_boxes, a.coords, a.min, a.max, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    } अन्यथा जर (a.type == SEGMENTATION_DATA){
        *a.d = load_data_seg(a.n, a.paths, a.m, a.w, a.h, a.classes, a.min, a.max, a.angle, a.aspect, a.hue, a.saturation, a.exposure, a.scale);
    } अन्यथा जर (a.type == REGION_DATA){
        *a.d = load_data_region(a.n, a.paths, a.m, a.w, a.h, a.अंक_boxes, a.classes, a.jitter, a.hue, a.saturation, a.exposure);
    } अन्यथा जर (a.type == DETECTION_DATA){
        *a.d = load_data_detection(a.n, a.paths, a.m, a.w, a.h, a.अंक_boxes, a.classes, a.jitter, a.hue, a.saturation, a.exposure);
    } अन्यथा जर (a.type == SWAG_DATA){
        *a.d = load_data_swag(a.paths, a.n, a.classes, a.jitter);
    } अन्यथा जर (a.type == COMPARE_DATA){
        *a.d = load_data_compare(a.n, a.paths, a.m, a.classes, a.w, a.h);
    } अन्यथा जर (a.type == IMAGE_DATA){
        *(a.im) = load_image_color(a.path, 0, 0);
        *(a.resized) = resize_image(*(a.im), a.w, a.h);
    } अन्यथा जर (a.type == LETTERBOX_DATA){
        *(a.im) = load_image_color(a.path, 0, 0);
        *(a.resized) = letterbox_image(*(a.im), a.w, a.h);
    } अन्यथा जर (a.type == TAG_DATA){
        *a.d = load_data_tag(a.paths, a.n, a.m, a.classes, a.min, a.max, a.size, a.angle, a.aspect, a.hue, a.saturation, a.exposure);
    }
    मुक्त(ptr);
    परत 0;
}

pthread_t load_data_in_thread(load_args args)
{
    pthread_t thread;
    संरचना load_args *ptr = सुस्मृति(1, आकार(संरचना load_args));
    *ptr = args;
    जर(pthread_create(&thread, 0, load_thread, ptr)) error("Thread creation failed");
    परत thread;
}

शून्य *load_threads(शून्य *ptr)
{
    पूर्णांक i;
    load_args args = *(load_args *)ptr;
    जर (args.threads == 0) args.threads = 1;
    data *out = args.d;
    पूर्णांक total = args.n;
    मुक्त(ptr);
    data *buffers = सुस्मृति(args.threads, आकार(data));
    pthread_t *threads = सुस्मृति(args.threads, आकार(pthread_t));
    साठी(i = 0; i < args.threads; ++i){
        args.d = buffers + i;
        args.n = (i+1) * total/args.threads - i * total/args.threads;
        threads[i] = load_data_in_thread(args);
    }
    साठी(i = 0; i < args.threads; ++i){
        pthread_join(threads[i], 0);
    }
    *out = concat_datas(buffers, args.threads);
    out->shallow = 0;
    साठी(i = 0; i < args.threads; ++i){
        buffers[i].shallow = 1;
        free_data(buffers[i]);
    }
    मुक्त(buffers);
    मुक्त(threads);
    परत 0;
}

शून्य load_data_blocking(load_args args)
{
    संरचना load_args *ptr = सुस्मृति(1, आकार(संरचना load_args));
    *ptr = args;
    load_thread(ptr);
}

pthread_t load_data(load_args args)
{
    pthread_t thread;
    संरचना load_args *ptr = सुस्मृति(1, आकार(संरचना load_args));
    *ptr = args;
    जर(pthread_create(&thread, 0, load_threads, ptr)) error("Thread creation failed");
    परत thread;
}

data load_data_writing(अक्षर **paths, पूर्णांक n, पूर्णांक m, पूर्णांक w, पूर्णांक h, पूर्णांक out_w, पूर्णांक out_h)
{
    जर(m) paths = get_random_paths(paths, n, m);
    अक्षर **replace_paths = find_replace_paths(paths, n, ".png", "-label.png");
    data d = {0};
    d.shallow = 0;
    d.X = load_image_paths(paths, n, w, h);
    d.y = load_image_paths_gray(replace_paths, n, out_w, out_h);
    जर(m) मुक्त(paths);
    पूर्णांक i;
    साठी(i = 0; i < n; ++i) मुक्त(replace_paths[i]);
    मुक्त(replace_paths);
    परत d;
}

data load_data_old(अक्षर **paths, पूर्णांक n, पूर्णांक m, अक्षर **labels, पूर्णांक k, पूर्णांक w, पूर्णांक h)
{
    जर(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;
    d.X = load_image_paths(paths, n, w, h);
    d.y = load_labels_paths(paths, n, labels, k, 0);
    जर(m) मुक्त(paths);
    परत d;
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

data load_data_super(अक्षर **paths, पूर्णांक n, पूर्णांक m, पूर्णांक w, पूर्णांक h, पूर्णांक scale)
{
    जर(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;

    पूर्णांक i;
    d.X.rows = n;
    d.X.vals = सुस्मृति(n, आकार(दशांश*));
    d.X.cols = w*h*3;

    d.y.rows = n;
    d.y.vals = सुस्मृति(n, आकार(दशांश*));
    d.y.cols = w*scale * h*scale * 3;

    साठी(i = 0; i < n; ++i){
        image im = load_image_color(paths[i], 0, 0);
        image crop = random_crop_image(im, w*scale, h*scale);
        पूर्णांक flip = अक्रम()%2;
        जर (flip) flip_image(crop);
        image resize = resize_image(crop, w, h);
        d.X.vals[i] = resize.data;
        d.y.vals[i] = crop.data;
        free_image(im);
    }

    जर(m) मुक्त(paths);
    परत d;
}

data load_data_regression(अक्षर **paths, पूर्णांक n, पूर्णांक m, पूर्णांक k, पूर्णांक min, पूर्णांक max, पूर्णांक size, दशांश angle, दशांश aspect, दशांश hue, दशांश saturation, दशांश exposure)
{
    जर(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;
    d.X = load_image_augment_paths(paths, n, min, max, size, angle, aspect, hue, saturation, exposure, 0);
    d.y = load_regression_labels_paths(paths, n, k);
    जर(m) मुक्त(paths);
    परत d;
}

data select_data(data *orig, पूर्णांक *inds)
{
    data d = {0};
    d.shallow = 1;
    d.w = orig[0].w;
    d.h = orig[0].h;

    d.X.rows = orig[0].X.rows;
    d.y.rows = orig[0].X.rows;

    d.X.cols = orig[0].X.cols;
    d.y.cols = orig[0].y.cols;

    d.X.vals = सुस्मृति(orig[0].X.rows, आकार(दशांश *));
    d.y.vals = सुस्मृति(orig[0].y.rows, आकार(दशांश *));
    पूर्णांक i;
    साठी(i = 0; i < d.X.rows; ++i){
        d.X.vals[i] = orig[inds[i]].X.vals[i];
        d.y.vals[i] = orig[inds[i]].y.vals[i];
    }
    परत d;
}

data *tile_data(data orig, पूर्णांक divs, पूर्णांक size)
{
    data *ds = सुस्मृति(divs*divs, आकार(data));
    पूर्णांक i, j;
#pragma omp parallel साठी
    साठी(i = 0; i < divs*divs; ++i){
        data d;
        d.shallow = 0;
        d.w = orig.w/divs * size;
        d.h = orig.h/divs * size;
        d.X.rows = orig.X.rows;
        d.X.cols = d.w*d.h*3;
        d.X.vals = सुस्मृति(d.X.rows, आकार(दशांश*));

        d.y = copy_matrix(orig.y);
#pragma omp parallel साठी
        साठी(j = 0; j < orig.X.rows; ++j){
            पूर्णांक x = (i%divs) * orig.w / divs - (d.w - orig.w/divs)/2;
            पूर्णांक y = (i/divs) * orig.h / divs - (d.h - orig.h/divs)/2;
            image im = float_to_image(orig.w, orig.h, 3, orig.X.vals[j]);
            d.X.vals[j] = crop_image(im, x, y, d.w, d.h).data;
        }
        ds[i] = d;
    }
    परत ds;
}

data resize_data(data orig, पूर्णांक w, पूर्णांक h)
{
    data d = {0};
    d.shallow = 0;
    d.w = w;
    d.h = h;
    पूर्णांक i;
    d.X.rows = orig.X.rows;
    d.X.cols = w*h*3;
    d.X.vals = सुस्मृति(d.X.rows, आकार(दशांश*));

    d.y = copy_matrix(orig.y);
#pragma omp parallel साठी
    साठी(i = 0; i < orig.X.rows; ++i){
        image im = float_to_image(orig.w, orig.h, 3, orig.X.vals[i]);
        d.X.vals[i] = resize_image(im, w, h).data;
    }
    परत d;
}

data load_data_augment(अक्षर **paths, पूर्णांक n, पूर्णांक m, अक्षर **labels, पूर्णांक k, वृक्ष *hierarchy, पूर्णांक min, पूर्णांक max, पूर्णांक size, दशांश angle, दशांश aspect, दशांश hue, दशांश saturation, दशांश exposure, पूर्णांक center)
{
    जर(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.shallow = 0;
    d.w=size;
    d.h=size;
    d.X = load_image_augment_paths(paths, n, min, max, size, angle, aspect, hue, saturation, exposure, center);
    d.y = load_labels_paths(paths, n, labels, k, hierarchy);
    जर(m) मुक्त(paths);
    परत d;
}

data load_data_tag(अक्षर **paths, पूर्णांक n, पूर्णांक m, पूर्णांक k, पूर्णांक min, पूर्णांक max, पूर्णांक size, दशांश angle, दशांश aspect, दशांश hue, दशांश saturation, दशांश exposure)
{
    जर(m) paths = get_random_paths(paths, n, m);
    data d = {0};
    d.w = size;
    d.h = size;
    d.shallow = 0;
    d.X = load_image_augment_paths(paths, n, min, max, size, angle, aspect, hue, saturation, exposure, 0);
    d.y = load_tags_paths(paths, n, k);
    जर(m) मुक्त(paths);
    परत d;
}

matrix concat_matrix(matrix m1, matrix m2)
{
    पूर्णांक i, मोजणी = 0;
    matrix m;
    m.cols = m1.cols;
    m.rows = m1.rows+m2.rows;
    m.vals = सुस्मृति(m1.rows + m2.rows, आकार(दशांश*));
    साठी(i = 0; i < m1.rows; ++i){
        m.vals[मोजणी++] = m1.vals[i];
    }
    साठी(i = 0; i < m2.rows; ++i){
        m.vals[मोजणी++] = m2.vals[i];
    }
    परत m;
}

data concat_data(data d1, data d2)
{
    data d = {0};
    d.shallow = 1;
    d.X = concat_matrix(d1.X, d2.X);
    d.y = concat_matrix(d1.y, d2.y);
    d.w = d1.w;
    d.h = d1.h;
    परत d;
}

data concat_datas(data *d, पूर्णांक n)
{
    पूर्णांक i;
    data out = {0};
    साठी(i = 0; i < n; ++i){
        data new = concat_data(d[i], out);
        free_data(out);
        out = new;
    }
    परत out;
}

data load_categorical_data_csv(अक्षर *filename, पूर्णांक target, पूर्णांक k)
{
    data d = {0};
    d.shallow = 0;
    matrix X = csv_to_matrix(filename);
    दशांश *truth_1d = काढा_column(&X, target);
    दशांश **truth = one_hot_encode(truth_1d, X.rows, k);
    matrix y;
    y.rows = X.rows;
    y.cols = k;
    y.vals = truth;
    d.X = X;
    d.y = y;
    मुक्त(truth_1d);
    परत d;
}

data load_cifar10_data(अक्षर *filename)
{
    data d = {0};
    d.shallow = 0;
    दीर्घ i,j;
    matrix X = make_matrix(10000, 3072);
    matrix y = make_matrix(10000, 10);
    d.X = X;
    d.y = y;

    खाता *fp = ख_खोलो(filename, "rb");
    जर(!fp) file_error(filename);
    साठी(i = 0; i < 10000; ++i){
        अचिन्हित अक्षर bytes[3073];
        ख_पढ़ो(bytes, 1, 3073, fp);
        पूर्णांक class = bytes[0];
        y.vals[i][class] = 1;
        साठी(j = 0; j < X.cols; ++j){
            X.vals[i][j] = (द्विगुण)bytes[j+1];
        }
    }
    scale_data_rows(d, 1./255);
    //normalize_data_rows(d);
    ख_बंद(fp);
    परत d;
}

शून्य get_random_batch(data d, पूर्णांक n, दशांश *X, दशांश *y)
{
    पूर्णांक j;
    साठी(j = 0; j < n; ++j){
        पूर्णांक index = अक्रम()%d.X.rows;
        स_नकल(X+j*d.X.cols, d.X.vals[index], d.X.cols*आकार(दशांश));
        स_नकल(y+j*d.y.cols, d.y.vals[index], d.y.cols*आकार(दशांश));
    }
}

शून्य get_next_batch(data d, पूर्णांक n, पूर्णांक offset, दशांश *X, दशांश *y)
{
    पूर्णांक j;
    साठी(j = 0; j < n; ++j){
        पूर्णांक index = offset + j;
        स_नकल(X+j*d.X.cols, d.X.vals[index], d.X.cols*आकार(दशांश));
        जर(y) स_नकल(y+j*d.y.cols, d.y.vals[index], d.y.cols*आकार(दशांश));
    }
}

शून्य smooth_data(data d)
{
    पूर्णांक i, j;
    दशांश scale = 1. / d.y.cols;
    दशांश eps = .1;
    साठी(i = 0; i < d.y.rows; ++i){
        साठी(j = 0; j < d.y.cols; ++j){
            d.y.vals[i][j] = eps * scale + (1-eps) * d.y.vals[i][j];
        }
    }
}

data load_all_cifar10()
{
    data d = {0};
    d.shallow = 0;
    पूर्णांक i,j,b;
    matrix X = make_matrix(50000, 3072);
    matrix y = make_matrix(50000, 10);
    d.X = X;
    d.y = y;


    साठी(b = 0; b < 5; ++b){
        अक्षर buff[256];
        प्र_लिखो(buff, "data/cifar/cifar-10-batches-bin/data_batch_%d.bin", b+1);
        खाता *fp = ख_खोलो(buff, "rb");
        जर(!fp) file_error(buff);
        साठी(i = 0; i < 10000; ++i){
            अचिन्हित अक्षर bytes[3073];
            ख_पढ़ो(bytes, 1, 3073, fp);
            पूर्णांक class = bytes[0];
            y.vals[i+b*10000][class] = 1;
            साठी(j = 0; j < X.cols; ++j){
                X.vals[i+b*10000][j] = (द्विगुण)bytes[j+1];
            }
        }
        ख_बंद(fp);
    }
    //normalize_data_rows(d);
    scale_data_rows(d, 1./255);
    smooth_data(d);
    परत d;
}

data load_go(अक्षर *filename)
{
    खाता *fp = ख_खोलो(filename, "rb");
    matrix X = make_matrix(3363059, 361);
    matrix y = make_matrix(3363059, 361);
    पूर्णांक row, col;

    जर(!fp) file_error(filename);
    अक्षर *label;
    पूर्णांक मोजणी = 0;
    पर्यंत((label = fgetl(fp))){
        पूर्णांक i;
        जर(मोजणी == X.rows){
            X = resize_matrix(X, मोजणी*2);
            y = resize_matrix(y, मोजणी*2);
        }
        माला_पूछो(label, "%d %d", &row, &col);
        अक्षर *board = fgetl(fp);

        पूर्णांक index = row*19 + col;
        y.vals[मोजणी][index] = 1;

        साठी(i = 0; i < 19*19; ++i){
            दशांश val = 0;
            जर(board[i] == '1') val = 1;
            अन्यथा जर(board[i] == '2') val = -1;
            X.vals[मोजणी][i] = val;
        }
        ++मोजणी;
        मुक्त(label);
        मुक्त(board);
    }
    X = resize_matrix(X, मोजणी);
    y = resize_matrix(y, मोजणी);

    data d = {0};
    d.shallow = 0;
    d.X = X;
    d.y = y;


    ख_बंद(fp);

    परत d;
}


शून्य randomize_data(data d)
{
    पूर्णांक i;
    साठी(i = d.X.rows-1; i > 0; --i){
        पूर्णांक index = अक्रम()%i;
        दशांश *swap = d.X.vals[index];
        d.X.vals[index] = d.X.vals[i];
        d.X.vals[i] = swap;

        swap = d.y.vals[index];
        d.y.vals[index] = d.y.vals[i];
        d.y.vals[i] = swap;
    }
}

शून्य scale_data_rows(data d, दशांश s)
{
    पूर्णांक i;
    साठी(i = 0; i < d.X.rows; ++i){
        scale_array(d.X.vals[i], d.X.cols, s);
    }
}

शून्य translate_data_rows(data d, दशांश s)
{
    पूर्णांक i;
    साठी(i = 0; i < d.X.rows; ++i){
        translate_array(d.X.vals[i], d.X.cols, s);
    }
}

data copy_data(data d)
{
    data c = {0};
    c.w = d.w;
    c.h = d.h;
    c.shallow = 0;
    c.अंक_boxes = d.अंक_boxes;
    c.boxes = d.boxes;
    c.X = copy_matrix(d.X);
    c.y = copy_matrix(d.y);
    परत c;
}

शून्य normalize_data_rows(data d)
{
    पूर्णांक i;
    साठी(i = 0; i < d.X.rows; ++i){
        normalize_array(d.X.vals[i], d.X.cols);
    }
}

data get_data_part(data d, पूर्णांक part, पूर्णांक total)
{
    data p = {0};
    p.shallow = 1;
    p.X.rows = d.X.rows * (part + 1) / total - d.X.rows * part / total;
    p.y.rows = d.y.rows * (part + 1) / total - d.y.rows * part / total;
    p.X.cols = d.X.cols;
    p.y.cols = d.y.cols;
    p.X.vals = d.X.vals + d.X.rows * part / total;
    p.y.vals = d.y.vals + d.y.rows * part / total;
    परत p;
}

data get_random_data(data d, पूर्णांक अंक)
{
    data r = {0};
    r.shallow = 1;

    r.X.rows = अंक;
    r.y.rows = अंक;

    r.X.cols = d.X.cols;
    r.y.cols = d.y.cols;

    r.X.vals = सुस्मृति(अंक, आकार(दशांश *));
    r.y.vals = सुस्मृति(अंक, आकार(दशांश *));

    पूर्णांक i;
    साठी(i = 0; i < अंक; ++i){
        पूर्णांक index = अक्रम()%d.X.rows;
        r.X.vals[i] = d.X.vals[index];
        r.y.vals[i] = d.y.vals[index];
    }
    परत r;
}

data *split_data(data d, पूर्णांक part, पूर्णांक total)
{
    data *split = सुस्मृति(2, आकार(data));
    पूर्णांक i;
    पूर्णांक start = part*d.X.rows/total;
    पूर्णांक end = (part+1)*d.X.rows/total;
    data train;
    data test;
    train.shallow = test.shallow = 1;

    test.X.rows = test.y.rows = end-start;
    train.X.rows = train.y.rows = d.X.rows - (end-start);
    train.X.cols = test.X.cols = d.X.cols;
    train.y.cols = test.y.cols = d.y.cols;

    train.X.vals = सुस्मृति(train.X.rows, आकार(दशांश*));
    test.X.vals = सुस्मृति(test.X.rows, आकार(दशांश*));
    train.y.vals = सुस्मृति(train.y.rows, आकार(दशांश*));
    test.y.vals = सुस्मृति(test.y.rows, आकार(दशांश*));

    साठी(i = 0; i < start; ++i){
        train.X.vals[i] = d.X.vals[i];
        train.y.vals[i] = d.y.vals[i];
    }
    साठी(i = start; i < end; ++i){
        test.X.vals[i-start] = d.X.vals[i];
        test.y.vals[i-start] = d.y.vals[i];
    }
    साठी(i = end; i < d.X.rows; ++i){
        train.X.vals[i-(end-start)] = d.X.vals[i];
        train.y.vals[i-(end-start)] = d.y.vals[i];
    }
    split[0] = train;
    split[1] = test;
    परत split;
}

