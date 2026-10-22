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
void *open_video_stream(const char *f, số_nguyên c, số_nguyên w, số_nguyên h, số_nguyên fps);
image get_image_from_stream(void *p);
image load_image_cv(char *filename, số_nguyên channels);
số_nguyên show_image_cv(image im, const char* name, số_nguyên ms);
#endif

float get_color(số_nguyên c, số_nguyên x, số_nguyên max);
void draw_box(image a, số_nguyên x1, số_nguyên y1, số_nguyên x2, số_nguyên y2, float r, float g, float b);
void draw_bbox(image a, box bbox, số_nguyên w, float r, float g, float b);
void write_label(image a, số_nguyên r, số_nguyên c, image *characters, char *string, float *rgb);
image image_distance(image a, image b);
void scale_image(image m, float s);
image rotate_crop_image(image im, float rad, float s, số_nguyên w, số_nguyên h, float dx, float dy, float aspect);
image random_crop_image(image im, số_nguyên w, số_nguyên h);
image random_augment_image(image im, float angle, float aspect, số_nguyên low, số_nguyên high, số_nguyên w, số_nguyên h);
augment_args random_augment_args(image im, float angle, float aspect, số_nguyên low, số_nguyên high, số_nguyên w, số_nguyên h);
void letterbox_image_into(image im, số_nguyên w, số_nguyên h, image boxed);
image resize_max(image im, số_nguyên max);
void translate_image(image m, float s);
void embed_image(image source, image dest, số_nguyên dx, số_nguyên dy);
void place_image(image im, số_nguyên w, số_nguyên h, số_nguyên dx, số_nguyên dy, image canvas);
void saturate_image(image im, float sat);
void exposure_image(image im, float sat);
void distort_image(image im, float hue, float sat, float val);
void saturate_exposure_image(image im, float sat, float exposure);
void rgb_to_hsv(image im);
void hsv_to_rgb(image im);
void yuv_to_rgb(image im);
void rgb_to_yuv(image im);


image collapse_image_layers(image source, số_nguyên border);
image collapse_images_horz(image *ims, số_nguyên n);
image collapse_images_vert(image *ims, số_nguyên n);

void show_image_normalized(image im, const char *name);
void show_images(image *ims, số_nguyên n, char *window);
void show_image_layers(image p, char *name);
void show_image_collapsed(image p, char *name);

void print_image(image m);

image make_empty_image(số_nguyên w, số_nguyên h, số_nguyên c);
void copy_image_into(image src, image dest);

image get_image_layer(image m, số_nguyên l);

#ifdef __cplusplus
}
#endif

#endif

