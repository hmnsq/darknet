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
void *open_video_stream(const char *f, 整数 c, 整数 w, 整数 h, 整数 fps);
image get_image_from_stream(void *p);
image load_image_cv(char *filename, 整数 channels);
整数 show_image_cv(image im, const char* name, 整数 ms);
#endif

float get_color(整数 c, 整数 x, 整数 max);
void draw_box(image a, 整数 x1, 整数 y1, 整数 x2, 整数 y2, float r, float g, float b);
void draw_bbox(image a, box bbox, 整数 w, float r, float g, float b);
void write_label(image a, 整数 r, 整数 c, image *characters, char *string, float *rgb);
image image_distance(image a, image b);
void scale_image(image m, float s);
image rotate_crop_image(image im, float rad, float s, 整数 w, 整数 h, float dx, float dy, float aspect);
image random_crop_image(image im, 整数 w, 整数 h);
image random_augment_image(image im, float angle, float aspect, 整数 low, 整数 high, 整数 w, 整数 h);
augment_args random_augment_args(image im, float angle, float aspect, 整数 low, 整数 high, 整数 w, 整数 h);
void letterbox_image_into(image im, 整数 w, 整数 h, image boxed);
image resize_max(image im, 整数 max);
void translate_image(image m, float s);
void embed_image(image source, image dest, 整数 dx, 整数 dy);
void place_image(image im, 整数 w, 整数 h, 整数 dx, 整数 dy, image canvas);
void saturate_image(image im, float sat);
void exposure_image(image im, float sat);
void distort_image(image im, float hue, float sat, float val);
void saturate_exposure_image(image im, float sat, float exposure);
void rgb_to_hsv(image im);
void hsv_to_rgb(image im);
void yuv_to_rgb(image im);
void rgb_to_yuv(image im);


image collapse_image_layers(image source, 整数 border);
image collapse_images_horz(image *ims, 整数 n);
image collapse_images_vert(image *ims, 整数 n);

void show_image_normalized(image im, const char *name);
void show_images(image *ims, 整数 n, char *window);
void show_image_layers(image p, char *name);
void show_image_collapsed(image p, char *name);

void print_image(image m);

image make_empty_image(整数 w, 整数 h, 整数 c);
void copy_image_into(image src, image dest);

image get_image_layer(image m, 整数 l);

#ifdef __cplusplus
}
#endif

#endif

