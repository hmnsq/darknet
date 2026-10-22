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
void *open_video_stream(const char *f, целое c, целое w, целое h, целое fps);
image get_image_from_stream(void *p);
image load_image_cv(char *filename, целое channels);
целое show_image_cv(image im, const char* name, целое ms);
#endif

float get_color(целое c, целое x, целое max);
void draw_box(image a, целое x1, целое y1, целое x2, целое y2, float r, float g, float b);
void draw_bbox(image a, box bbox, целое w, float r, float g, float b);
void write_label(image a, целое r, целое c, image *characters, char *string, float *rgb);
image image_distance(image a, image b);
void scale_image(image m, float s);
image rotate_crop_image(image im, float rad, float s, целое w, целое h, float dx, float dy, float aspect);
image random_crop_image(image im, целое w, целое h);
image random_augment_image(image im, float angle, float aspect, целое low, целое high, целое w, целое h);
augment_args random_augment_args(image im, float angle, float aspect, целое low, целое high, целое w, целое h);
void letterbox_image_into(image im, целое w, целое h, image boxed);
image resize_max(image im, целое max);
void translate_image(image m, float s);
void embed_image(image source, image dest, целое dx, целое dy);
void place_image(image im, целое w, целое h, целое dx, целое dy, image canvas);
void saturate_image(image im, float sat);
void exposure_image(image im, float sat);
void distort_image(image im, float hue, float sat, float val);
void saturate_exposure_image(image im, float sat, float exposure);
void rgb_to_hsv(image im);
void hsv_to_rgb(image im);
void yuv_to_rgb(image im);
void rgb_to_yuv(image im);


image collapse_image_layers(image source, целое border);
image collapse_images_horz(image *ims, целое n);
image collapse_images_vert(image *ims, целое n);

void show_image_normalized(image im, const char *name);
void show_images(image *ims, целое n, char *window);
void show_image_layers(image p, char *name);
void show_image_collapsed(image p, char *name);

void print_image(image m);

image make_empty_image(целое w, целое h, целое c);
void copy_image_into(image src, image dest);

image get_image_layer(image m, целое l);

#ifdef __cplusplus
}
#endif

#endif

