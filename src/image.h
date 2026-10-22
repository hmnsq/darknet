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
void *open_video_stream(const var_na *f, puur_naa_nka c, puur_naa_nka w, puur_naa_nka h, puur_naa_nka fps);
image get_image_from_stream(void *p);
image load_image_cv(var_na *filename, puur_naa_nka channels);
puur_naa_nka show_image_cv(image im, const var_na* name, puur_naa_nka ms);
#endif

float get_color(puur_naa_nka c, puur_naa_nka x, puur_naa_nka max);
void draw_box(image a, puur_naa_nka x1, puur_naa_nka y1, puur_naa_nka x2, puur_naa_nka y2, float r, float g, float b);
void draw_bbox(image a, box bbox, puur_naa_nka w, float r, float g, float b);
void write_label(image a, puur_naa_nka r, puur_naa_nka c, image *characters, var_na *string, float *rgb);
image image_distance(image a, image b);
void scale_image(image m, float s);
image rotate_crop_image(image im, float rad, float s, puur_naa_nka w, puur_naa_nka h, float dx, float dy, float aspect);
image random_crop_image(image im, puur_naa_nka w, puur_naa_nka h);
image random_augment_image(image im, float angle, float aspect, puur_naa_nka low, puur_naa_nka high, puur_naa_nka w, puur_naa_nka h);
augment_args random_augment_args(image im, float angle, float aspect, puur_naa_nka low, puur_naa_nka high, puur_naa_nka w, puur_naa_nka h);
void letterbox_image_into(image im, puur_naa_nka w, puur_naa_nka h, image boxed);
image resize_max(image im, puur_naa_nka max);
void translate_image(image m, float s);
void embed_image(image source, image dest, puur_naa_nka dx, puur_naa_nka dy);
void place_image(image im, puur_naa_nka w, puur_naa_nka h, puur_naa_nka dx, puur_naa_nka dy, image canvas);
void saturate_image(image im, float sat);
void exposure_image(image im, float sat);
void distort_image(image im, float hue, float sat, float val);
void saturate_exposure_image(image im, float sat, float exposure);
void rgb_to_hsv(image im);
void hsv_to_rgb(image im);
void yuv_to_rgb(image im);
void rgb_to_yuv(image im);


image collapse_image_layers(image source, puur_naa_nka border);
image collapse_images_horz(image *ims, puur_naa_nka n);
image collapse_images_vert(image *ims, puur_naa_nka n);

void show_image_normalized(image im, const var_na *name);
void show_images(image *ims, puur_naa_nka n, var_na *window);
void show_image_layers(image p, var_na *name);
void show_image_collapsed(image p, var_na *name);

void print_image(image m);

image make_empty_image(puur_naa_nka w, puur_naa_nka h, puur_naa_nka c);
void copy_image_into(image src, image dest);

image get_image_layer(image m, puur_naa_nka l);

#ifdef __cplusplus
}
#endif

#endif

