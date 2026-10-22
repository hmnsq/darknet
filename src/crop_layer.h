#ifndef CROP_LAYER_H
#define CROP_LAYER_H

#include "image.h"
#include "layer.h"
#include "network.h"

typedef layer crop_layer;

image get_crop_image(crop_layer l);
crop_layer make_crop_layer(ཧྲིལ batch, ཧྲིལ h, ཧྲིལ w, ཧྲིལ c, ཧྲིལ crop_height, ཧྲིལ crop_width, ཧྲིལ flip, float angle, float saturation, float exposure);
void forward_crop_layer(const crop_layer l, network net);
void resize_crop_layer(layer *l, ཧྲིལ w, ཧྲིལ h);

#ifdef GPU
void forward_crop_layer_gpu(crop_layer l, network net);
#endif

#endif

