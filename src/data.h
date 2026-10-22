#ifndef DATA_H
#define DATA_H
#include <pthread.h>

#include "darknet.h"
#include "matrix.h"
#include "list.h"
#include "image.h"
#include "tree.h"

static inline float distance_from_edge(số_nguyên x, số_nguyên max)
{
    số_nguyên dx = (max/2) - x;
    nếu (dx < 0) dx = -dx;
    dx = (max/2) + 1 - dx;
    dx *= 2;
    float dist = (float)dx/max;
    nếu (dist > 1) dist = 1;
    trả_về dist;
}
void load_data_blocking(load_args args);


void print_letters(float *pred, số_nguyên n);
data load_data_captcha(char **paths, số_nguyên n, số_nguyên m, số_nguyên k, số_nguyên w, số_nguyên h);
data load_data_captcha_encode(char **paths, số_nguyên n, số_nguyên m, số_nguyên w, số_nguyên h);
data load_data_detection(số_nguyên n, char **paths, số_nguyên m, số_nguyên w, số_nguyên h, số_nguyên boxes, số_nguyên classes, float jitter, float hue, float saturation, float exposure);
data load_data_tag(char **paths, số_nguyên n, số_nguyên m, số_nguyên k, số_nguyên min, số_nguyên max, số_nguyên size, float angle, float aspect, float hue, float saturation, float exposure);
matrix load_image_augment_paths(char **paths, số_nguyên n, số_nguyên min, số_nguyên max, số_nguyên size, float angle, float aspect, float hue, float saturation, float exposure, số_nguyên center);
data load_data_super(char **paths, số_nguyên n, số_nguyên m, số_nguyên w, số_nguyên h, số_nguyên scale);
data load_data_augment(char **paths, số_nguyên n, số_nguyên m, char **labels, số_nguyên k, tree *hierarchy, số_nguyên min, số_nguyên max, số_nguyên size, float angle, float aspect, float hue, float saturation, float exposure, số_nguyên center);
data load_data_regression(char **paths, số_nguyên n, số_nguyên m, số_nguyên classes, số_nguyên min, số_nguyên max, số_nguyên size, float angle, float aspect, float hue, float saturation, float exposure);
data load_go(char *filename);


data load_data_writing(char **paths, số_nguyên n, số_nguyên m, số_nguyên w, số_nguyên h, số_nguyên out_w, số_nguyên out_h);

void get_random_batch(data d, số_nguyên n, float *X, float *y);
data get_data_part(data d, số_nguyên part, số_nguyên total);
data get_random_data(data d, số_nguyên num);
data load_categorical_data_csv(char *filename, số_nguyên target, số_nguyên k);
void normalize_data_rows(data d);
void scale_data_rows(data d, float s);
void translate_data_rows(data d, float s);
void randomize_data(data d);
data *split_data(data d, số_nguyên part, số_nguyên total);
data concat_datas(data *d, số_nguyên n);
void fill_truth(char *path, char **labels, số_nguyên k, float *truth);

#endif
