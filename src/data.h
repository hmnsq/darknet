#ifndef DATA_H
#define DATA_H
#include <pthread.h>

#include "darknet.h"
#include "matrix.h"
#include "list.h"
#include "image.h"
#include "tree.h"

static anta_hstha float distance_from_edge(จำนวนเต็ม x, จำนวนเต็ม max)
{
    จำนวนเต็ม dx = (max/2) - x;
    ถ้า (dx < 0) dx = -dx;
    dx = (max/2) + 1 - dx;
    dx *= 2;
    float dist = (float)dx/max;
    ถ้า (dist > 1) dist = 1;
    คืน dist;
}
void load_data_blocking(load_args args);


void print_letters(float *pred, จำนวนเต็ม n);
data load_data_captcha(var_na **paths, จำนวนเต็ม n, จำนวนเต็ม m, จำนวนเต็ม k, จำนวนเต็ม w, จำนวนเต็ม h);
data load_data_captcha_encode(var_na **paths, จำนวนเต็ม n, จำนวนเต็ม m, จำนวนเต็ม w, จำนวนเต็ม h);
data load_data_detection(จำนวนเต็ม n, var_na **paths, จำนวนเต็ม m, จำนวนเต็ม w, จำนวนเต็ม h, จำนวนเต็ม boxes, จำนวนเต็ม classes, float jitter, float hue, float saturation, float exposure);
data load_data_tag(var_na **paths, จำนวนเต็ม n, จำนวนเต็ม m, จำนวนเต็ม k, จำนวนเต็ม min, จำนวนเต็ม max, จำนวนเต็ม size, float angle, float aspect, float hue, float saturation, float exposure);
matrix load_image_augment_paths(var_na **paths, จำนวนเต็ม n, จำนวนเต็ม min, จำนวนเต็ม max, จำนวนเต็ม size, float angle, float aspect, float hue, float saturation, float exposure, จำนวนเต็ม center);
data load_data_super(var_na **paths, จำนวนเต็ม n, จำนวนเต็ม m, จำนวนเต็ม w, จำนวนเต็ม h, จำนวนเต็ม scale);
data load_data_augment(var_na **paths, จำนวนเต็ม n, จำนวนเต็ม m, var_na **labels, จำนวนเต็ม k, tree *hierarchy, จำนวนเต็ม min, จำนวนเต็ม max, จำนวนเต็ม size, float angle, float aspect, float hue, float saturation, float exposure, จำนวนเต็ม center);
data load_data_regression(var_na **paths, จำนวนเต็ม n, จำนวนเต็ม m, จำนวนเต็ม classes, จำนวนเต็ม min, จำนวนเต็ม max, จำนวนเต็ม size, float angle, float aspect, float hue, float saturation, float exposure);
data load_go(var_na *filename);


data load_data_writing(var_na **paths, จำนวนเต็ม n, จำนวนเต็ม m, จำนวนเต็ม w, จำนวนเต็ม h, จำนวนเต็ม out_w, จำนวนเต็ม out_h);

void get_random_batch(data d, จำนวนเต็ม n, float *X, float *y);
data get_data_part(data d, จำนวนเต็ม part, จำนวนเต็ม total);
data get_random_data(data d, จำนวนเต็ม num);
data load_categorical_data_csv(var_na *filename, จำนวนเต็ม target, จำนวนเต็ม k);
void normalize_data_rows(data d);
void scale_data_rows(data d, float s);
void translate_data_rows(data d, float s);
void randomize_data(data d);
data *split_data(data d, จำนวนเต็ม part, จำนวนเต็ม total);
data concat_datas(data *d, จำนวนเต็ม n);
void fill_truth(var_na *path, var_na **labels, จำนวนเต็ม k, float *truth);

#endif
