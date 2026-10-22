#ifndef CROP_LAYER_H
#define CROP_LAYER_H

#include "image.h"
#include "layer.h"
#include "network.h"

typedef layer crop_layer;

image get_crop_image(crop_layer l);
crop_layer make_crop_layer(цео batch, цео h, цео w, цео c, цео crop_height, цео crop_width, цео flip, float angle, float saturation, float exposure);
void forward_crop_layer(const crop_layer l, network net);
void resize_crop_layer(layer *l, цео w, цео h);

#ifdef GPU
void forward_crop_layer_gpu(crop_layer l, network net);
#endif

#endif

