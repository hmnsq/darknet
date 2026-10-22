#ifndef DATA_H
#define DATA_H
#include <pthread.h>

#include "darknet.h"
#include "matrix.h"
#include "list.h"
#include "image.h"
#include "tree.h"

static inline float distance_from_edge(정수 x, 정수 max)
{
    정수 dx = (max/2) - x;
    만약 (dx < 0) dx = -dx;
    dx = (max/2) + 1 - dx;
    dx *= 2;
    float dist = (float)dx/max;
    만약 (dist > 1) dist = 1;
    반환 dist;
}
void load_data_blocking(load_args args);


void print_letters(float *pred, 정수 n);
data load_data_captcha(char **paths, 정수 n, 정수 m, 정수 k, 정수 w, 정수 h);
data load_data_captcha_encode(char **paths, 정수 n, 정수 m, 정수 w, 정수 h);
data load_data_detection(정수 n, char **paths, 정수 m, 정수 w, 정수 h, 정수 boxes, 정수 classes, float jitter, float hue, float saturation, float exposure);
data load_data_tag(char **paths, 정수 n, 정수 m, 정수 k, 정수 min, 정수 max, 정수 size, float angle, float aspect, float hue, float saturation, float exposure);
matrix load_image_augment_paths(char **paths, 정수 n, 정수 min, 정수 max, 정수 size, float angle, float aspect, float hue, float saturation, float exposure, 정수 center);
data load_data_super(char **paths, 정수 n, 정수 m, 정수 w, 정수 h, 정수 scale);
data load_data_augment(char **paths, 정수 n, 정수 m, char **labels, 정수 k, tree *hierarchy, 정수 min, 정수 max, 정수 size, float angle, float aspect, float hue, float saturation, float exposure, 정수 center);
data load_data_regression(char **paths, 정수 n, 정수 m, 정수 classes, 정수 min, 정수 max, 정수 size, float angle, float aspect, float hue, float saturation, float exposure);
data load_go(char *filename);


data load_data_writing(char **paths, 정수 n, 정수 m, 정수 w, 정수 h, 정수 out_w, 정수 out_h);

void get_random_batch(data d, 정수 n, float *X, float *y);
data get_data_part(data d, 정수 part, 정수 total);
data get_random_data(data d, 정수 num);
data load_categorical_data_csv(char *filename, 정수 target, 정수 k);
void normalize_data_rows(data d);
void scale_data_rows(data d, float s);
void translate_data_rows(data d, float s);
void randomize_data(data d);
data *split_data(data d, 정수 part, 정수 total);
data concat_datas(data *d, 정수 n);
void fill_truth(char *path, char **labels, 정수 k, float *truth);

#endif
