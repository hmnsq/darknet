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
void *open_video_stream(const var_na *f, පූර්ණ_සංඛ්‍යා c, පූර්ණ_සංඛ්‍යා w, පූර්ණ_සංඛ්‍යා h, පූර්ණ_සංඛ්‍යා fps);
image get_image_from_stream(void *p);
image load_image_cv(var_na *filename, පූර්ණ_සංඛ්‍යා channels);
පූර්ණ_සංඛ්‍යා show_image_cv(image im, const var_na* name, පූර්ණ_සංඛ්‍යා ms);
#endif

float get_color(පූර්ණ_සංඛ්‍යා c, පූර්ණ_සංඛ්‍යා x, පූර්ණ_සංඛ්‍යා max);
void draw_box(image a, පූර්ණ_සංඛ්‍යා x1, පූර්ණ_සංඛ්‍යා y1, පූර්ණ_සංඛ්‍යා x2, පූර්ණ_සංඛ්‍යා y2, float r, float g, float b);
void draw_bbox(image a, box bbox, පූර්ණ_සංඛ්‍යා w, float r, float g, float b);
void write_label(image a, පූර්ණ_සංඛ්‍යා r, පූර්ණ_සංඛ්‍යා c, image *characters, var_na *string, float *rgb);
image image_distance(image a, image b);
void scale_image(image m, float s);
image rotate_crop_image(image im, float rad, float s, පූර්ණ_සංඛ්‍යා w, පූර්ණ_සංඛ්‍යා h, float dx, float dy, float aspect);
image random_crop_image(image im, පූර්ණ_සංඛ්‍යා w, පූර්ණ_සංඛ්‍යා h);
image random_augment_image(image im, float angle, float aspect, පූර්ණ_සංඛ්‍යා low, පූර්ණ_සංඛ්‍යා high, පූර්ණ_සංඛ්‍යා w, පූර්ණ_සංඛ්‍යා h);
augment_args random_augment_args(image im, float angle, float aspect, පූර්ණ_සංඛ්‍යා low, පූර්ණ_සංඛ්‍යා high, පූර්ණ_සංඛ්‍යා w, පූර්ණ_සංඛ්‍යා h);
void letterbox_image_into(image im, පූර්ණ_සංඛ්‍යා w, පූර්ණ_සංඛ්‍යා h, image boxed);
image resize_max(image im, පූර්ණ_සංඛ්‍යා max);
void translate_image(image m, float s);
void embed_image(image source, image dest, පූර්ණ_සංඛ්‍යා dx, පූර්ණ_සංඛ්‍යා dy);
void place_image(image im, පූර්ණ_සංඛ්‍යා w, පූර්ණ_සංඛ්‍යා h, පූර්ණ_සංඛ්‍යා dx, පූර්ණ_සංඛ්‍යා dy, image canvas);
void saturate_image(image im, float sat);
void exposure_image(image im, float sat);
void distort_image(image im, float hue, float sat, float val);
void saturate_exposure_image(image im, float sat, float exposure);
void rgb_to_hsv(image im);
void hsv_to_rgb(image im);
void yuv_to_rgb(image im);
void rgb_to_yuv(image im);


image collapse_image_layers(image source, පූර්ණ_සංඛ්‍යා border);
image collapse_images_horz(image *ims, පූර්ණ_සංඛ්‍යා n);
image collapse_images_vert(image *ims, පූර්ණ_සංඛ්‍යා n);

void show_image_normalized(image im, const var_na *name);
void show_images(image *ims, පූර්ණ_සංඛ්‍යා n, var_na *window);
void show_image_layers(image p, var_na *name);
void show_image_collapsed(image p, var_na *name);

void print_image(image m);

image make_empty_image(පූර්ණ_සංඛ්‍යා w, පූර්ණ_සංඛ්‍යා h, පූර්ණ_සංඛ්‍යා c);
void copy_image_into(image src, image dest);

image get_image_layer(image m, පූර්ණ_සංඛ්‍යා l);

#ifdef __cplusplus
}
#endif

#endif

