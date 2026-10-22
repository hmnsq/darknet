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
void *open_video_stream(const var_na *f, ចំនួនគត់ c, ចំនួនគត់ w, ចំនួនគត់ h, ចំនួនគត់ fps);
image get_image_from_stream(void *p);
image load_image_cv(var_na *filename, ចំនួនគត់ channels);
ចំនួនគត់ show_image_cv(image im, const var_na* name, ចំនួនគត់ ms);
#endif

float get_color(ចំនួនគត់ c, ចំនួនគត់ x, ចំនួនគត់ max);
void draw_box(image a, ចំនួនគត់ x1, ចំនួនគត់ y1, ចំនួនគត់ x2, ចំនួនគត់ y2, float r, float g, float b);
void draw_bbox(image a, box bbox, ចំនួនគត់ w, float r, float g, float b);
void write_label(image a, ចំនួនគត់ r, ចំនួនគត់ c, image *characters, var_na *string, float *rgb);
image image_distance(image a, image b);
void scale_image(image m, float s);
image rotate_crop_image(image im, float rad, float s, ចំនួនគត់ w, ចំនួនគត់ h, float dx, float dy, float aspect);
image random_crop_image(image im, ចំនួនគត់ w, ចំនួនគត់ h);
image random_augment_image(image im, float angle, float aspect, ចំនួនគត់ low, ចំនួនគត់ high, ចំនួនគត់ w, ចំនួនគត់ h);
augment_args random_augment_args(image im, float angle, float aspect, ចំនួនគត់ low, ចំនួនគត់ high, ចំនួនគត់ w, ចំនួនគត់ h);
void letterbox_image_into(image im, ចំនួនគត់ w, ចំនួនគត់ h, image boxed);
image resize_max(image im, ចំនួនគត់ max);
void translate_image(image m, float s);
void embed_image(image source, image dest, ចំនួនគត់ dx, ចំនួនគត់ dy);
void place_image(image im, ចំនួនគត់ w, ចំនួនគត់ h, ចំនួនគត់ dx, ចំនួនគត់ dy, image canvas);
void saturate_image(image im, float sat);
void exposure_image(image im, float sat);
void distort_image(image im, float hue, float sat, float val);
void saturate_exposure_image(image im, float sat, float exposure);
void rgb_to_hsv(image im);
void hsv_to_rgb(image im);
void yuv_to_rgb(image im);
void rgb_to_yuv(image im);


image collapse_image_layers(image source, ចំនួនគត់ border);
image collapse_images_horz(image *ims, ចំនួនគត់ n);
image collapse_images_vert(image *ims, ចំនួនគត់ n);

void show_image_normalized(image im, const var_na *name);
void show_images(image *ims, ចំនួនគត់ n, var_na *window);
void show_image_layers(image p, var_na *name);
void show_image_collapsed(image p, var_na *name);

void print_image(image m);

image make_empty_image(ចំនួនគត់ w, ចំនួនគត់ h, ចំនួនគត់ c);
void copy_image_into(image src, image dest);

image get_image_layer(image m, ចំនួនគត់ l);

#ifdef __cplusplus
}
#endif

#endif

