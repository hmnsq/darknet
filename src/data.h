#ifndef DATA_H
#define DATA_H
#include <pthread.h>

#include "darknet.h"
#include "matrix.h"
#include "list.h"
#include "image.h"
#include "tree.h"

static anta_hstha float distance_from_edge(ཧྲིལ x, ཧྲིལ max)
{
    ཧྲིལ dx = (max/2) - x;
    གལ་སྲིད (dx < 0) dx = -dx;
    dx = (max/2) + 1 - dx;
    dx *= 2;
    float dist = (float)dx/max;
    གལ་སྲིད (dist > 1) dist = 1;
    ལོག dist;
}
void load_data_blocking(load_args args);


void print_letters(float *pred, ཧྲིལ n);
data load_data_captcha(var_na **paths, ཧྲིལ n, ཧྲིལ m, ཧྲིལ k, ཧྲིལ w, ཧྲིལ h);
data load_data_captcha_encode(var_na **paths, ཧྲིལ n, ཧྲིལ m, ཧྲིལ w, ཧྲིལ h);
data load_data_detection(ཧྲིལ n, var_na **paths, ཧྲིལ m, ཧྲིལ w, ཧྲིལ h, ཧྲིལ boxes, ཧྲིལ classes, float jitter, float hue, float saturation, float exposure);
data load_data_tag(var_na **paths, ཧྲིལ n, ཧྲིལ m, ཧྲིལ k, ཧྲིལ min, ཧྲིལ max, ཧྲིལ size, float angle, float aspect, float hue, float saturation, float exposure);
matrix load_image_augment_paths(var_na **paths, ཧྲིལ n, ཧྲིལ min, ཧྲིལ max, ཧྲིལ size, float angle, float aspect, float hue, float saturation, float exposure, ཧྲིལ center);
data load_data_super(var_na **paths, ཧྲིལ n, ཧྲིལ m, ཧྲིལ w, ཧྲིལ h, ཧྲིལ scale);
data load_data_augment(var_na **paths, ཧྲིལ n, ཧྲིལ m, var_na **labels, ཧྲིལ k, tree *hierarchy, ཧྲིལ min, ཧྲིལ max, ཧྲིལ size, float angle, float aspect, float hue, float saturation, float exposure, ཧྲིལ center);
data load_data_regression(var_na **paths, ཧྲིལ n, ཧྲིལ m, ཧྲིལ classes, ཧྲིལ min, ཧྲིལ max, ཧྲིལ size, float angle, float aspect, float hue, float saturation, float exposure);
data load_go(var_na *filename);


data load_data_writing(var_na **paths, ཧྲིལ n, ཧྲིལ m, ཧྲིལ w, ཧྲིལ h, ཧྲིལ out_w, ཧྲིལ out_h);

void get_random_batch(data d, ཧྲིལ n, float *X, float *y);
data get_data_part(data d, ཧྲིལ part, ཧྲིལ total);
data get_random_data(data d, ཧྲིལ num);
data load_categorical_data_csv(var_na *filename, ཧྲིལ target, ཧྲིལ k);
void normalize_data_rows(data d);
void scale_data_rows(data d, float s);
void translate_data_rows(data d, float s);
void randomize_data(data d);
data *split_data(data d, ཧྲིལ part, ཧྲིལ total);
data concat_datas(data *d, ཧྲིལ n);
void fill_truth(var_na *path, var_na **labels, ཧྲིལ k, float *truth);

#endif
