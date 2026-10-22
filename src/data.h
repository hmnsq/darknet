#ifndef DATA_H
#define DATA_H
#include <pthread.h>

#include "darknet.h"
#include "matrix.h"
#include "list.h"
#include "image.h"
#include "tree.h"

static inline float distance_from_edge(ціле x, ціле max)
{
    ціле dx = (max/2) - x;
    якщо (dx < 0) dx = -dx;
    dx = (max/2) + 1 - dx;
    dx *= 2;
    float dist = (float)dx/max;
    якщо (dist > 1) dist = 1;
    повернути dist;
}
void load_data_blocking(load_args args);


void print_letters(float *pred, ціле n);
data load_data_captcha(char **paths, ціле n, ціле m, ціле k, ціле w, ціле h);
data load_data_captcha_encode(char **paths, ціле n, ціле m, ціле w, ціле h);
data load_data_detection(ціле n, char **paths, ціле m, ціле w, ціле h, ціле boxes, ціле classes, float jitter, float hue, float saturation, float exposure);
data load_data_tag(char **paths, ціле n, ціле m, ціле k, ціле min, ціле max, ціле size, float angle, float aspect, float hue, float saturation, float exposure);
matrix load_image_augment_paths(char **paths, ціле n, ціле min, ціле max, ціле size, float angle, float aspect, float hue, float saturation, float exposure, ціле center);
data load_data_super(char **paths, ціле n, ціле m, ціле w, ціле h, ціле scale);
data load_data_augment(char **paths, ціле n, ціле m, char **labels, ціле k, tree *hierarchy, ціле min, ціле max, ціле size, float angle, float aspect, float hue, float saturation, float exposure, ціле center);
data load_data_regression(char **paths, ціле n, ціле m, ціле classes, ціле min, ціле max, ціле size, float angle, float aspect, float hue, float saturation, float exposure);
data load_go(char *filename);


data load_data_writing(char **paths, ціле n, ціле m, ціле w, ціле h, ціле out_w, ціле out_h);

void get_random_batch(data d, ціле n, float *X, float *y);
data get_data_part(data d, ціле part, ціле total);
data get_random_data(data d, ціле num);
data load_categorical_data_csv(char *filename, ціле target, ціле k);
void normalize_data_rows(data d);
void scale_data_rows(data d, float s);
void translate_data_rows(data d, float s);
void randomize_data(data d);
data *split_data(data d, ціле part, ціле total);
data concat_datas(data *d, ціле n);
void fill_truth(char *path, char **labels, ціле k, float *truth);

#endif
