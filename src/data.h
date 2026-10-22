#ifndef DATA_H
#define DATA_H
#include <pthread.h>

#include "darknet.h"
#include "matrix.h"
#include "list.h"
#include "image.h"
#include "tree.h"

static inline float distance_from_edge(цяло x, цяло max)
{
    цяло dx = (max/2) - x;
    ако (dx < 0) dx = -dx;
    dx = (max/2) + 1 - dx;
    dx *= 2;
    float dist = (float)dx/max;
    ако (dist > 1) dist = 1;
    върни dist;
}
void load_data_blocking(load_args args);


void print_letters(float *pred, цяло n);
data load_data_captcha(char **paths, цяло n, цяло m, цяло k, цяло w, цяло h);
data load_data_captcha_encode(char **paths, цяло n, цяло m, цяло w, цяло h);
data load_data_detection(цяло n, char **paths, цяло m, цяло w, цяло h, цяло boxes, цяло classes, float jitter, float hue, float saturation, float exposure);
data load_data_tag(char **paths, цяло n, цяло m, цяло k, цяло min, цяло max, цяло size, float angle, float aspect, float hue, float saturation, float exposure);
matrix load_image_augment_paths(char **paths, цяло n, цяло min, цяло max, цяло size, float angle, float aspect, float hue, float saturation, float exposure, цяло center);
data load_data_super(char **paths, цяло n, цяло m, цяло w, цяло h, цяло scale);
data load_data_augment(char **paths, цяло n, цяло m, char **labels, цяло k, tree *hierarchy, цяло min, цяло max, цяло size, float angle, float aspect, float hue, float saturation, float exposure, цяло center);
data load_data_regression(char **paths, цяло n, цяло m, цяло classes, цяло min, цяло max, цяло size, float angle, float aspect, float hue, float saturation, float exposure);
data load_go(char *filename);


data load_data_writing(char **paths, цяло n, цяло m, цяло w, цяло h, цяло out_w, цяло out_h);

void get_random_batch(data d, цяло n, float *X, float *y);
data get_data_part(data d, цяло part, цяло total);
data get_random_data(data d, цяло num);
data load_categorical_data_csv(char *filename, цяло target, цяло k);
void normalize_data_rows(data d);
void scale_data_rows(data d, float s);
void translate_data_rows(data d, float s);
void randomize_data(data d);
data *split_data(data d, цяло part, цяло total);
data concat_datas(data *d, цяло n);
void fill_truth(char *path, char **labels, цяло k, float *truth);

#endif
