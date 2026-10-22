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
void *open_video_stream(const var_na *f, ຈຳນວນເຕັມ c, ຈຳນວນເຕັມ w, ຈຳນວນເຕັມ h, ຈຳນວນເຕັມ fps);
image get_image_from_stream(void *p);
image load_image_cv(var_na *filename, ຈຳນວນເຕັມ channels);
ຈຳນວນເຕັມ show_image_cv(image im, const var_na* name, ຈຳນວນເຕັມ ms);
#endif

float get_color(ຈຳນວນເຕັມ c, ຈຳນວນເຕັມ x, ຈຳນວນເຕັມ max);
void draw_box(image a, ຈຳນວນເຕັມ x1, ຈຳນວນເຕັມ y1, ຈຳນວນເຕັມ x2, ຈຳນວນເຕັມ y2, float r, float g, float b);
void draw_bbox(image a, box bbox, ຈຳນວນເຕັມ w, float r, float g, float b);
void write_label(image a, ຈຳນວນເຕັມ r, ຈຳນວນເຕັມ c, image *characters, var_na *string, float *rgb);
image image_distance(image a, image b);
void scale_image(image m, float s);
image rotate_crop_image(image im, float rad, float s, ຈຳນວນເຕັມ w, ຈຳນວນເຕັມ h, float dx, float dy, float aspect);
image random_crop_image(image im, ຈຳນວນເຕັມ w, ຈຳນວນເຕັມ h);
image random_augment_image(image im, float angle, float aspect, ຈຳນວນເຕັມ low, ຈຳນວນເຕັມ high, ຈຳນວນເຕັມ w, ຈຳນວນເຕັມ h);
augment_args random_augment_args(image im, float angle, float aspect, ຈຳນວນເຕັມ low, ຈຳນວນເຕັມ high, ຈຳນວນເຕັມ w, ຈຳນວນເຕັມ h);
void letterbox_image_into(image im, ຈຳນວນເຕັມ w, ຈຳນວນເຕັມ h, image boxed);
image resize_max(image im, ຈຳນວນເຕັມ max);
void translate_image(image m, float s);
void embed_image(image source, image dest, ຈຳນວນເຕັມ dx, ຈຳນວນເຕັມ dy);
void place_image(image im, ຈຳນວນເຕັມ w, ຈຳນວນເຕັມ h, ຈຳນວນເຕັມ dx, ຈຳນວນເຕັມ dy, image canvas);
void saturate_image(image im, float sat);
void exposure_image(image im, float sat);
void distort_image(image im, float hue, float sat, float val);
void saturate_exposure_image(image im, float sat, float exposure);
void rgb_to_hsv(image im);
void hsv_to_rgb(image im);
void yuv_to_rgb(image im);
void rgb_to_yuv(image im);


image collapse_image_layers(image source, ຈຳນວນເຕັມ border);
image collapse_images_horz(image *ims, ຈຳນວນເຕັມ n);
image collapse_images_vert(image *ims, ຈຳນວນເຕັມ n);

void show_image_normalized(image im, const var_na *name);
void show_images(image *ims, ຈຳນວນເຕັມ n, var_na *window);
void show_image_layers(image p, var_na *name);
void show_image_collapsed(image p, var_na *name);

void print_image(image m);

image make_empty_image(ຈຳນວນເຕັມ w, ຈຳນວນເຕັມ h, ຈຳນວນເຕັມ c);
void copy_image_into(image src, image dest);

image get_image_layer(image m, ຈຳນວນເຕັມ l);

#ifdef __cplusplus
}
#endif

#endif

