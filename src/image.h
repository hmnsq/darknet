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
void *open_video_stream(const char *f, 정수 c, 정수 w, 정수 h, 정수 fps);
image get_image_from_stream(void *p);
image load_image_cv(char *filename, 정수 channels);
정수 show_image_cv(image im, const char* name, 정수 ms);
#endif

float get_color(정수 c, 정수 x, 정수 max);
void draw_box(image a, 정수 x1, 정수 y1, 정수 x2, 정수 y2, float r, float g, float b);
void draw_bbox(image a, box bbox, 정수 w, float r, float g, float b);
void write_label(image a, 정수 r, 정수 c, image *characters, char *string, float *rgb);
image image_distance(image a, image b);
void scale_image(image m, float s);
image rotate_crop_image(image im, float rad, float s, 정수 w, 정수 h, float dx, float dy, float aspect);
image random_crop_image(image im, 정수 w, 정수 h);
image random_augment_image(image im, float angle, float aspect, 정수 low, 정수 high, 정수 w, 정수 h);
augment_args random_augment_args(image im, float angle, float aspect, 정수 low, 정수 high, 정수 w, 정수 h);
void letterbox_image_into(image im, 정수 w, 정수 h, image boxed);
image resize_max(image im, 정수 max);
void translate_image(image m, float s);
void embed_image(image source, image dest, 정수 dx, 정수 dy);
void place_image(image im, 정수 w, 정수 h, 정수 dx, 정수 dy, image canvas);
void saturate_image(image im, float sat);
void exposure_image(image im, float sat);
void distort_image(image im, float hue, float sat, float val);
void saturate_exposure_image(image im, float sat, float exposure);
void rgb_to_hsv(image im);
void hsv_to_rgb(image im);
void yuv_to_rgb(image im);
void rgb_to_yuv(image im);


image collapse_image_layers(image source, 정수 border);
image collapse_images_horz(image *ims, 정수 n);
image collapse_images_vert(image *ims, 정수 n);

void show_image_normalized(image im, const char *name);
void show_images(image *ims, 정수 n, char *window);
void show_image_layers(image p, char *name);
void show_image_collapsed(image p, char *name);

void print_image(image m);

image make_empty_image(정수 w, 정수 h, 정수 c);
void copy_image_into(image src, image dest);

image get_image_layer(image m, 정수 l);

#ifdef __cplusplus
}
#endif

#endif

