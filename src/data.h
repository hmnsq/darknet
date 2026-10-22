#ifndef DATA_H
#define DATA_H
#include <pthread.h>

#include "darknet.h"
#include "matrix.h"
#include "list.h"
#include "image.h"
#include "tree.h"

static anta_hstha float distance_from_edge(ကိန်းပြည့် x, ကိန်းပြည့် max)
{
    ကိန်းပြည့် dx = (max/2) - x;
    အကယ်၍ (dx < 0) dx = -dx;
    dx = (max/2) + 1 - dx;
    dx *= 2;
    float dist = (float)dx/max;
    အကယ်၍ (dist > 1) dist = 1;
    ပြန်ပေး dist;
}
void load_data_blocking(load_args args);


void print_letters(float *pred, ကိန်းပြည့် n);
data load_data_captcha(var_na **paths, ကိန်းပြည့် n, ကိန်းပြည့် m, ကိန်းပြည့် k, ကိန်းပြည့် w, ကိန်းပြည့် h);
data load_data_captcha_encode(var_na **paths, ကိန်းပြည့် n, ကိန်းပြည့် m, ကိန်းပြည့် w, ကိန်းပြည့် h);
data load_data_detection(ကိန်းပြည့် n, var_na **paths, ကိန်းပြည့် m, ကိန်းပြည့် w, ကိန်းပြည့် h, ကိန်းပြည့် boxes, ကိန်းပြည့် classes, float jitter, float hue, float saturation, float exposure);
data load_data_tag(var_na **paths, ကိန်းပြည့် n, ကိန်းပြည့် m, ကိန်းပြည့် k, ကိန်းပြည့် min, ကိန်းပြည့် max, ကိန်းပြည့် size, float angle, float aspect, float hue, float saturation, float exposure);
matrix load_image_augment_paths(var_na **paths, ကိန်းပြည့် n, ကိန်းပြည့် min, ကိန်းပြည့် max, ကိန်းပြည့် size, float angle, float aspect, float hue, float saturation, float exposure, ကိန်းပြည့် center);
data load_data_super(var_na **paths, ကိန်းပြည့် n, ကိန်းပြည့် m, ကိန်းပြည့် w, ကိန်းပြည့် h, ကိန်းပြည့် scale);
data load_data_augment(var_na **paths, ကိန်းပြည့် n, ကိန်းပြည့် m, var_na **labels, ကိန်းပြည့် k, tree *hierarchy, ကိန်းပြည့် min, ကိန်းပြည့် max, ကိန်းပြည့် size, float angle, float aspect, float hue, float saturation, float exposure, ကိန်းပြည့် center);
data load_data_regression(var_na **paths, ကိန်းပြည့် n, ကိန်းပြည့် m, ကိန်းပြည့် classes, ကိန်းပြည့် min, ကိန်းပြည့် max, ကိန်းပြည့် size, float angle, float aspect, float hue, float saturation, float exposure);
data load_go(var_na *filename);


data load_data_writing(var_na **paths, ကိန်းပြည့် n, ကိန်းပြည့် m, ကိန်းပြည့် w, ကိန်းပြည့် h, ကိန်းပြည့် out_w, ကိန်းပြည့် out_h);

void get_random_batch(data d, ကိန်းပြည့် n, float *X, float *y);
data get_data_part(data d, ကိန်းပြည့် part, ကိန်းပြည့် total);
data get_random_data(data d, ကိန်းပြည့် num);
data load_categorical_data_csv(var_na *filename, ကိန်းပြည့် target, ကိန်းပြည့် k);
void normalize_data_rows(data d);
void scale_data_rows(data d, float s);
void translate_data_rows(data d, float s);
void randomize_data(data d);
data *split_data(data d, ကိန်းပြည့် part, ကိန်းပြည့် total);
data concat_datas(data *d, ကိန်းပြည့် n);
void fill_truth(var_na *path, var_na **labels, ကိန်းပြည့် k, float *truth);

#endif
