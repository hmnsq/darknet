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
void *open_video_stream(const char *f, цео c, цео w, цео h, цео fps);
image get_image_from_stream(void *p);
image load_image_cv(char *filename, цео channels);
цео show_image_cv(image im, const char* name, цео ms);
#endif

float get_color(цео c, цео x, цео max);
void draw_box(image a, цео x1, цео y1, цео x2, цео y2, float r, float g, float b);
void draw_bbox(image a, box bbox, цео w, float r, float g, float b);
void write_label(image a, цео r, цео c, image *characters, char *string, float *rgb);
image image_distance(image a, image b);
void scale_image(image m, float s);
image rotate_crop_image(image im, float rad, float s, цео w, цео h, float dx, float dy, float aspect);
image random_crop_image(image im, цео w, цео h);
image random_augment_image(image im, float angle, float aspect, цео low, цео high, цео w, цео h);
augment_args random_augment_args(image im, float angle, float aspect, цео low, цео high, цео w, цео h);
void letterbox_image_into(image im, цео w, цео h, image boxed);
image resize_max(image im, цео max);
void translate_image(image m, float s);
void embed_image(image source, image dest, цео dx, цео dy);
void place_image(image im, цео w, цео h, цео dx, цео dy, image canvas);
void saturate_image(image im, float sat);
void exposure_image(image im, float sat);
void distort_image(image im, float hue, float sat, float val);
void saturate_exposure_image(image im, float sat, float exposure);
void rgb_to_hsv(image im);
void hsv_to_rgb(image im);
void yuv_to_rgb(image im);
void rgb_to_yuv(image im);


image collapse_image_layers(image source, цео border);
image collapse_images_horz(image *ims, цео n);
image collapse_images_vert(image *ims, цео n);

void show_image_normalized(image im, const char *name);
void show_images(image *ims, цео n, char *window);
void show_image_layers(image p, char *name);
void show_image_collapsed(image p, char *name);

void print_image(image m);

image make_empty_image(цео w, цео h, цео c);
void copy_image_into(image src, image dest);

image get_image_layer(image m, цео l);

#ifdef __cplusplus
}
#endif

#endif

