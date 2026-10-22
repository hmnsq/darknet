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
void *open_video_stream(const var_na *f, ကိန်းပြည့် c, ကိန်းပြည့် w, ကိန်းပြည့် h, ကိန်းပြည့် fps);
image get_image_from_stream(void *p);
image load_image_cv(var_na *filename, ကိန်းပြည့် channels);
ကိန်းပြည့် show_image_cv(image im, const var_na* name, ကိန်းပြည့် ms);
#endif

float get_color(ကိန်းပြည့် c, ကိန်းပြည့် x, ကိန်းပြည့် max);
void draw_box(image a, ကိန်းပြည့် x1, ကိန်းပြည့် y1, ကိန်းပြည့် x2, ကိန်းပြည့် y2, float r, float g, float b);
void draw_bbox(image a, box bbox, ကိန်းပြည့် w, float r, float g, float b);
void write_label(image a, ကိန်းပြည့် r, ကိန်းပြည့် c, image *characters, var_na *string, float *rgb);
image image_distance(image a, image b);
void scale_image(image m, float s);
image rotate_crop_image(image im, float rad, float s, ကိန်းပြည့် w, ကိန်းပြည့် h, float dx, float dy, float aspect);
image random_crop_image(image im, ကိန်းပြည့် w, ကိန်းပြည့် h);
image random_augment_image(image im, float angle, float aspect, ကိန်းပြည့် low, ကိန်းပြည့် high, ကိန်းပြည့် w, ကိန်းပြည့် h);
augment_args random_augment_args(image im, float angle, float aspect, ကိန်းပြည့် low, ကိန်းပြည့် high, ကိန်းပြည့် w, ကိန်းပြည့် h);
void letterbox_image_into(image im, ကိန်းပြည့် w, ကိန်းပြည့် h, image boxed);
image resize_max(image im, ကိန်းပြည့် max);
void translate_image(image m, float s);
void embed_image(image source, image dest, ကိန်းပြည့် dx, ကိန်းပြည့် dy);
void place_image(image im, ကိန်းပြည့် w, ကိန်းပြည့် h, ကိန်းပြည့် dx, ကိန်းပြည့် dy, image canvas);
void saturate_image(image im, float sat);
void exposure_image(image im, float sat);
void distort_image(image im, float hue, float sat, float val);
void saturate_exposure_image(image im, float sat, float exposure);
void rgb_to_hsv(image im);
void hsv_to_rgb(image im);
void yuv_to_rgb(image im);
void rgb_to_yuv(image im);


image collapse_image_layers(image source, ကိန်းပြည့် border);
image collapse_images_horz(image *ims, ကိန်းပြည့် n);
image collapse_images_vert(image *ims, ကိန်းပြည့် n);

void show_image_normalized(image im, const var_na *name);
void show_images(image *ims, ကိန်းပြည့် n, var_na *window);
void show_image_layers(image p, var_na *name);
void show_image_collapsed(image p, var_na *name);

void print_image(image m);

image make_empty_image(ကိန်းပြည့် w, ကိန်းပြည့် h, ကိန်းပြည့် c);
void copy_image_into(image src, image dest);

image get_image_layer(image m, ကိန်းပြည့် l);

#ifdef __cplusplus
}
#endif

#endif

