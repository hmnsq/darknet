#ifndef DATA_H
#define DATA_H
#include <pthread.h>

#include "darknet.h"
#include "matrix.h"
#include "list.h"
#include "image.h"
#include "tree.h"

static anta_hstha float distance_from_edge(ចំនួនគត់ x, ចំនួនគត់ max)
{
    ចំនួនគត់ dx = (max/2) - x;
    ប្រសិនបើ (dx < 0) dx = -dx;
    dx = (max/2) + 1 - dx;
    dx *= 2;
    float dist = (float)dx/max;
    ប្រសិនបើ (dist > 1) dist = 1;
    ត្រឡប់ dist;
}
void load_data_blocking(load_args args);


void print_letters(float *pred, ចំនួនគត់ n);
data load_data_captcha(var_na **paths, ចំនួនគត់ n, ចំនួនគត់ m, ចំនួនគត់ k, ចំនួនគត់ w, ចំនួនគត់ h);
data load_data_captcha_encode(var_na **paths, ចំនួនគត់ n, ចំនួនគត់ m, ចំនួនគត់ w, ចំនួនគត់ h);
data load_data_detection(ចំនួនគត់ n, var_na **paths, ចំនួនគត់ m, ចំនួនគត់ w, ចំនួនគត់ h, ចំនួនគត់ boxes, ចំនួនគត់ classes, float jitter, float hue, float saturation, float exposure);
data load_data_tag(var_na **paths, ចំនួនគត់ n, ចំនួនគត់ m, ចំនួនគត់ k, ចំនួនគត់ min, ចំនួនគត់ max, ចំនួនគត់ size, float angle, float aspect, float hue, float saturation, float exposure);
matrix load_image_augment_paths(var_na **paths, ចំនួនគត់ n, ចំនួនគត់ min, ចំនួនគត់ max, ចំនួនគត់ size, float angle, float aspect, float hue, float saturation, float exposure, ចំនួនគត់ center);
data load_data_super(var_na **paths, ចំនួនគត់ n, ចំនួនគត់ m, ចំនួនគត់ w, ចំនួនគត់ h, ចំនួនគត់ scale);
data load_data_augment(var_na **paths, ចំនួនគត់ n, ចំនួនគត់ m, var_na **labels, ចំនួនគត់ k, tree *hierarchy, ចំនួនគត់ min, ចំនួនគត់ max, ចំនួនគត់ size, float angle, float aspect, float hue, float saturation, float exposure, ចំនួនគត់ center);
data load_data_regression(var_na **paths, ចំនួនគត់ n, ចំនួនគត់ m, ចំនួនគត់ classes, ចំនួនគត់ min, ចំនួនគត់ max, ចំនួនគត់ size, float angle, float aspect, float hue, float saturation, float exposure);
data load_go(var_na *filename);


data load_data_writing(var_na **paths, ចំនួនគត់ n, ចំនួនគត់ m, ចំនួនគត់ w, ចំនួនគត់ h, ចំនួនគត់ out_w, ចំនួនគត់ out_h);

void get_random_batch(data d, ចំនួនគត់ n, float *X, float *y);
data get_data_part(data d, ចំនួនគត់ part, ចំនួនគត់ total);
data get_random_data(data d, ចំនួនគត់ num);
data load_categorical_data_csv(var_na *filename, ចំនួនគត់ target, ចំនួនគត់ k);
void normalize_data_rows(data d);
void scale_data_rows(data d, float s);
void translate_data_rows(data d, float s);
void randomize_data(data d);
data *split_data(data d, ចំនួនគត់ part, ចំនួនគត់ total);
data concat_datas(data *d, ចំនួនគត់ n);
void fill_truth(var_na *path, var_na **labels, ចំនួនគត់ k, float *truth);

#endif
