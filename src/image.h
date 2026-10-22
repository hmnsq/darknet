#ifndef IMAGE_H
#define IMAGE_H

#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <string.h>
#include <math.h>
#include "box.h"
#include "darknet.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef OPENCV
void *open_video_stream(const char *f, ціле c, ціле w, ціле h, ціле fps);
image get_image_from_stream(void *p);
image load_image_cv(char *filename, ціле channels);
ціле show_image_cv(image im, const char* name, ціле ms);
#endif

float get_color(ціле c, ціле x, ціле max);
void draw_box(image a, ціле x1, ціле y1, ціле x2, ціле y2, float r, float g, float b);
void draw_bbox(image a, box bbox, ціле w, float r, float g, float b);
void write_label(image a, ціле r, ціле c, image *characters, char *string, float *rgb);
image image_distance(image a, image b);
void scale_image(image m, float s);
image rotate_crop_image(image im, float rad, float s, ціле w, ціле h, float dx, float dy, float aspect);
image random_crop_image(image im, ціле w, ціле h);
image random_augment_image(image im, float angle, float aspect, ціле low, ціле high, ціле w, ціле h);
augment_args random_augment_args(image im, float angle, float aspect, ціле low, ціле high, ціле w, ціле h);
void letterbox_image_into(image im, ціле w, ціле h, image boxed);
image resize_max(image im, ціле max);
void translate_image(image m, float s);
void embed_image(image source, image dest, ціле dx, ціле dy);
void place_image(image im, ціле w, ціле h, ціле dx, ціле dy, image canvas);
void saturate_image(image im, float sat);
void exposure_image(image im, float sat);
void distort_image(image im, float hue, float sat, float val);
void saturate_exposure_image(image im, float sat, float exposure);
void rgb_to_hsv(image im);
void hsv_to_rgb(image im);
void yuv_to_rgb(image im);
void rgb_to_yuv(image im);


image collapse_image_layers(image source, ціле border);
image collapse_images_horz(image *ims, ціле n);
image collapse_images_vert(image *ims, ціле n);

void show_image_normalized(image im, const char *name);
void show_images(image *ims, ціле n, char *window);
void show_image_layers(image p, char *name);
void show_image_collapsed(image p, char *name);

void print_image(image m);

image make_empty_image(ціле w, ціле h, ціле c);
void copy_image_into(image src, image dest);

image get_image_layer(image m, ціле l);

#ifdef __cplusplus
}
#endif

#endif

