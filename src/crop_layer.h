#ifndef CROP_LAYER_H
#define CROP_LAYER_H

#include "image.h"
#include "layer.h"
#include "network.h"

typedef layer crop_layer;

image get_crop_image(crop_layer l);
crop_layer make_crop_layer(จำนวนเต็ม batch, จำนวนเต็ม h, จำนวนเต็ม w, จำนวนเต็ม c, จำนวนเต็ม crop_height, จำนวนเต็ม crop_width, จำนวนเต็ม flip, float angle, float saturation, float exposure);
void forward_crop_layer(const crop_layer l, network net);
void resize_crop_layer(layer *l, จำนวนเต็ม w, จำนวนเต็ม h);

#ifdef GPU
void forward_crop_layer_gpu(crop_layer l, network net);
#endif

#endif

