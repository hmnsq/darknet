#ifndef DATA_H
#define DATA_H
#include <pthread.h>

#include "darknet.h"
#include "matrix.h"
#include "list.h"
#include "image.h"
#include "tree.h"

static inline float distance_from_edge(整数 x, 整数 max)
{
    整数 dx = (max/2) - x;
    もし (dx < 0) dx = -dx;
    dx = (max/2) + 1 - dx;
    dx *= 2;
    float dist = (float)dx/max;
    もし (dist > 1) dist = 1;
    戻る dist;
}
void load_data_blocking(load_args args);


void print_letters(float *pred, 整数 n);
data load_data_captcha(char **paths, 整数 n, 整数 m, 整数 k, 整数 w, 整数 h);
data load_data_captcha_encode(char **paths, 整数 n, 整数 m, 整数 w, 整数 h);
data load_data_detection(整数 n, char **paths, 整数 m, 整数 w, 整数 h, 整数 boxes, 整数 classes, float jitter, float hue, float saturation, float exposure);
data load_data_tag(char **paths, 整数 n, 整数 m, 整数 k, 整数 min, 整数 max, 整数 size, float angle, float aspect, float hue, float saturation, float exposure);
matrix load_image_augment_paths(char **paths, 整数 n, 整数 min, 整数 max, 整数 size, float angle, float aspect, float hue, float saturation, float exposure, 整数 center);
data load_data_super(char **paths, 整数 n, 整数 m, 整数 w, 整数 h, 整数 scale);
data load_data_augment(char **paths, 整数 n, 整数 m, char **labels, 整数 k, tree *hierarchy, 整数 min, 整数 max, 整数 size, float angle, float aspect, float hue, float saturation, float exposure, 整数 center);
data load_data_regression(char **paths, 整数 n, 整数 m, 整数 classes, 整数 min, 整数 max, 整数 size, float angle, float aspect, float hue, float saturation, float exposure);
data load_go(char *filename);


data load_data_writing(char **paths, 整数 n, 整数 m, 整数 w, 整数 h, 整数 out_w, 整数 out_h);

void get_random_batch(data d, 整数 n, float *X, float *y);
data get_data_part(data d, 整数 part, 整数 total);
data get_random_data(data d, 整数 num);
data load_categorical_data_csv(char *filename, 整数 target, 整数 k);
void normalize_data_rows(data d);
void scale_data_rows(data d, float s);
void translate_data_rows(data d, float s);
void randomize_data(data d);
data *split_data(data d, 整数 part, 整数 total);
data concat_datas(data *d, 整数 n);
void fill_truth(char *path, char **labels, 整数 k, float *truth);

#endif
