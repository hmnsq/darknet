#ifndef DATA_H
#define DATA_H
#include <pthread.h>

#include "darknet.h"
#include "matrix.h"
#include "list.h"
#include "image.h"
#include "tree.h"

static inline float distance_from_edge(цео x, цео max)
{
    цео dx = (max/2) - x;
    ако (dx < 0) dx = -dx;
    dx = (max/2) + 1 - dx;
    dx *= 2;
    float dist = (float)dx/max;
    ако (dist > 1) dist = 1;
    врати dist;
}
void load_data_blocking(load_args args);


void print_letters(float *pred, цео n);
data load_data_captcha(char **paths, цео n, цео m, цео k, цео w, цео h);
data load_data_captcha_encode(char **paths, цео n, цео m, цео w, цео h);
data load_data_detection(цео n, char **paths, цео m, цео w, цео h, цео boxes, цео classes, float jitter, float hue, float saturation, float exposure);
data load_data_tag(char **paths, цео n, цео m, цео k, цео min, цео max, цео size, float angle, float aspect, float hue, float saturation, float exposure);
matrix load_image_augment_paths(char **paths, цео n, цео min, цео max, цео size, float angle, float aspect, float hue, float saturation, float exposure, цео center);
data load_data_super(char **paths, цео n, цео m, цео w, цео h, цео scale);
data load_data_augment(char **paths, цео n, цео m, char **labels, цео k, tree *hierarchy, цео min, цео max, цео size, float angle, float aspect, float hue, float saturation, float exposure, цео center);
data load_data_regression(char **paths, цео n, цео m, цео classes, цео min, цео max, цео size, float angle, float aspect, float hue, float saturation, float exposure);
data load_go(char *filename);


data load_data_writing(char **paths, цео n, цео m, цео w, цео h, цео out_w, цео out_h);

void get_random_batch(data d, цео n, float *X, float *y);
data get_data_part(data d, цео part, цео total);
data get_random_data(data d, цео num);
data load_categorical_data_csv(char *filename, цео target, цео k);
void normalize_data_rows(data d);
void scale_data_rows(data d, float s);
void translate_data_rows(data d, float s);
void randomize_data(data d);
data *split_data(data d, цео part, цео total);
data concat_datas(data *d, цео n);
void fill_truth(char *path, char **labels, цео k, float *truth);

#endif
