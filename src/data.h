#ifndef DATA_H
#define DATA_H
#include <pthread.h>

#include "darknet.h"
#include "matrix.h"
#include "list.h"
#include "image.h"
#include "tree.h"

static inline float distance_from_edge(целое x, целое max)
{
    целое dx = (max/2) - x;
    если (dx < 0) dx = -dx;
    dx = (max/2) + 1 - dx;
    dx *= 2;
    float dist = (float)dx/max;
    если (dist > 1) dist = 1;
    вернуть dist;
}
void load_data_blocking(load_args args);


void print_letters(float *pred, целое n);
data load_data_captcha(char **paths, целое n, целое m, целое k, целое w, целое h);
data load_data_captcha_encode(char **paths, целое n, целое m, целое w, целое h);
data load_data_detection(целое n, char **paths, целое m, целое w, целое h, целое boxes, целое classes, float jitter, float hue, float saturation, float exposure);
data load_data_tag(char **paths, целое n, целое m, целое k, целое min, целое max, целое size, float angle, float aspect, float hue, float saturation, float exposure);
matrix load_image_augment_paths(char **paths, целое n, целое min, целое max, целое size, float angle, float aspect, float hue, float saturation, float exposure, целое center);
data load_data_super(char **paths, целое n, целое m, целое w, целое h, целое scale);
data load_data_augment(char **paths, целое n, целое m, char **labels, целое k, tree *hierarchy, целое min, целое max, целое size, float angle, float aspect, float hue, float saturation, float exposure, целое center);
data load_data_regression(char **paths, целое n, целое m, целое classes, целое min, целое max, целое size, float angle, float aspect, float hue, float saturation, float exposure);
data load_go(char *filename);


data load_data_writing(char **paths, целое n, целое m, целое w, целое h, целое out_w, целое out_h);

void get_random_batch(data d, целое n, float *X, float *y);
data get_data_part(data d, целое part, целое total);
data get_random_data(data d, целое num);
data load_categorical_data_csv(char *filename, целое target, целое k);
void normalize_data_rows(data d);
void scale_data_rows(data d, float s);
void translate_data_rows(data d, float s);
void randomize_data(data d);
data *split_data(data d, целое part, целое total);
data concat_datas(data *d, целое n);
void fill_truth(char *path, char **labels, целое k, float *truth);

#endif
