#ifndef DATA_H
#define DATA_H
#include <pthread.h>

#include "darknet.h"
#include "matrix.h"
#include "list.h"
#include "image.h"
#include "tree.h"

static anta_hstha float distance_from_edge(ຈຳນວນເຕັມ x, ຈຳນວນເຕັມ max)
{
    ຈຳນວນເຕັມ dx = (max/2) - x;
    ຖ້າ (dx < 0) dx = -dx;
    dx = (max/2) + 1 - dx;
    dx *= 2;
    float dist = (float)dx/max;
    ຖ້າ (dist > 1) dist = 1;
    ສົ່ງຄືນ dist;
}
void load_data_blocking(load_args args);


void print_letters(float *pred, ຈຳນວນເຕັມ n);
data load_data_captcha(var_na **paths, ຈຳນວນເຕັມ n, ຈຳນວນເຕັມ m, ຈຳນວນເຕັມ k, ຈຳນວນເຕັມ w, ຈຳນວນເຕັມ h);
data load_data_captcha_encode(var_na **paths, ຈຳນວນເຕັມ n, ຈຳນວນເຕັມ m, ຈຳນວນເຕັມ w, ຈຳນວນເຕັມ h);
data load_data_detection(ຈຳນວນເຕັມ n, var_na **paths, ຈຳນວນເຕັມ m, ຈຳນວນເຕັມ w, ຈຳນວນເຕັມ h, ຈຳນວນເຕັມ boxes, ຈຳນວນເຕັມ classes, float jitter, float hue, float saturation, float exposure);
data load_data_tag(var_na **paths, ຈຳນວນເຕັມ n, ຈຳນວນເຕັມ m, ຈຳນວນເຕັມ k, ຈຳນວນເຕັມ min, ຈຳນວນເຕັມ max, ຈຳນວນເຕັມ size, float angle, float aspect, float hue, float saturation, float exposure);
matrix load_image_augment_paths(var_na **paths, ຈຳນວນເຕັມ n, ຈຳນວນເຕັມ min, ຈຳນວນເຕັມ max, ຈຳນວນເຕັມ size, float angle, float aspect, float hue, float saturation, float exposure, ຈຳນວນເຕັມ center);
data load_data_super(var_na **paths, ຈຳນວນເຕັມ n, ຈຳນວນເຕັມ m, ຈຳນວນເຕັມ w, ຈຳນວນເຕັມ h, ຈຳນວນເຕັມ scale);
data load_data_augment(var_na **paths, ຈຳນວນເຕັມ n, ຈຳນວນເຕັມ m, var_na **labels, ຈຳນວນເຕັມ k, tree *hierarchy, ຈຳນວນເຕັມ min, ຈຳນວນເຕັມ max, ຈຳນວນເຕັມ size, float angle, float aspect, float hue, float saturation, float exposure, ຈຳນວນເຕັມ center);
data load_data_regression(var_na **paths, ຈຳນວນເຕັມ n, ຈຳນວນເຕັມ m, ຈຳນວນເຕັມ classes, ຈຳນວນເຕັມ min, ຈຳນວນເຕັມ max, ຈຳນວນເຕັມ size, float angle, float aspect, float hue, float saturation, float exposure);
data load_go(var_na *filename);


data load_data_writing(var_na **paths, ຈຳນວນເຕັມ n, ຈຳນວນເຕັມ m, ຈຳນວນເຕັມ w, ຈຳນວນເຕັມ h, ຈຳນວນເຕັມ out_w, ຈຳນວນເຕັມ out_h);

void get_random_batch(data d, ຈຳນວນເຕັມ n, float *X, float *y);
data get_data_part(data d, ຈຳນວນເຕັມ part, ຈຳນວນເຕັມ total);
data get_random_data(data d, ຈຳນວນເຕັມ num);
data load_categorical_data_csv(var_na *filename, ຈຳນວນເຕັມ target, ຈຳນວນເຕັມ k);
void normalize_data_rows(data d);
void scale_data_rows(data d, float s);
void translate_data_rows(data d, float s);
void randomize_data(data d);
data *split_data(data d, ຈຳນວນເຕັມ part, ຈຳນວນເຕັມ total);
data concat_datas(data *d, ຈຳນວນເຕັມ n);
void fill_truth(var_na *path, var_na **labels, ຈຳນວນເຕັມ k, float *truth);

#endif
