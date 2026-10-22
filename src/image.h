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
void *open_video_stream(const var_na *f, จำนวนเต็ม c, จำนวนเต็ม w, จำนวนเต็ม h, จำนวนเต็ม fps);
image get_image_from_stream(void *p);
image load_image_cv(var_na *filename, จำนวนเต็ม channels);
จำนวนเต็ม show_image_cv(image im, const var_na* name, จำนวนเต็ม ms);
#endif

float get_color(จำนวนเต็ม c, จำนวนเต็ม x, จำนวนเต็ม max);
void draw_box(image a, จำนวนเต็ม x1, จำนวนเต็ม y1, จำนวนเต็ม x2, จำนวนเต็ม y2, float r, float g, float b);
void draw_bbox(image a, box bbox, จำนวนเต็ม w, float r, float g, float b);
void write_label(image a, จำนวนเต็ม r, จำนวนเต็ม c, image *characters, var_na *string, float *rgb);
image image_distance(image a, image b);
void scale_image(image m, float s);
image rotate_crop_image(image im, float rad, float s, จำนวนเต็ม w, จำนวนเต็ม h, float dx, float dy, float aspect);
image random_crop_image(image im, จำนวนเต็ม w, จำนวนเต็ม h);
image random_augment_image(image im, float angle, float aspect, จำนวนเต็ม low, จำนวนเต็ม high, จำนวนเต็ม w, จำนวนเต็ม h);
augment_args random_augment_args(image im, float angle, float aspect, จำนวนเต็ม low, จำนวนเต็ม high, จำนวนเต็ม w, จำนวนเต็ม h);
void letterbox_image_into(image im, จำนวนเต็ม w, จำนวนเต็ม h, image boxed);
image resize_max(image im, จำนวนเต็ม max);
void translate_image(image m, float s);
void embed_image(image source, image dest, จำนวนเต็ม dx, จำนวนเต็ม dy);
void place_image(image im, จำนวนเต็ม w, จำนวนเต็ม h, จำนวนเต็ม dx, จำนวนเต็ม dy, image canvas);
void saturate_image(image im, float sat);
void exposure_image(image im, float sat);
void distort_image(image im, float hue, float sat, float val);
void saturate_exposure_image(image im, float sat, float exposure);
void rgb_to_hsv(image im);
void hsv_to_rgb(image im);
void yuv_to_rgb(image im);
void rgb_to_yuv(image im);


image collapse_image_layers(image source, จำนวนเต็ม border);
image collapse_images_horz(image *ims, จำนวนเต็ม n);
image collapse_images_vert(image *ims, จำนวนเต็ม n);

void show_image_normalized(image im, const var_na *name);
void show_images(image *ims, จำนวนเต็ม n, var_na *window);
void show_image_layers(image p, var_na *name);
void show_image_collapsed(image p, var_na *name);

void print_image(image m);

image make_empty_image(จำนวนเต็ม w, จำนวนเต็ม h, จำนวนเต็ม c);
void copy_image_into(image src, image dest);

image get_image_layer(image m, จำนวนเต็ม l);

#ifdef __cplusplus
}
#endif

#endif

