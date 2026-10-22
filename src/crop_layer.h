#ifndef CROP_LAYER_H
#define CROP_LAYER_H

#include "image.h"
#include "layer.h"
#include "network.h"

typedef layer crop_layer;

image get_crop_image(crop_layer l);
crop_layer make_crop_layer(ចំនួនគត់ batch, ចំនួនគត់ h, ចំនួនគត់ w, ចំនួនគត់ c, ចំនួនគត់ crop_height, ចំនួនគត់ crop_width, ចំនួនគត់ flip, float angle, float saturation, float exposure);
void forward_crop_layer(const crop_layer l, network net);
void resize_crop_layer(layer *l, ចំនួនគត់ w, ចំនួនគត់ h);

#ifdef GPU
void forward_crop_layer_gpu(crop_layer l, network net);
#endif

#endif

