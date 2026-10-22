#ifndef REGION_LAYER_H
#define REGION_LAYER_H

#include "darknet.h"
#include "layer.h"
#include "network.h"

layer make_region_layer(ចំនួនគត់ batch, ចំនួនគត់ w, ចំនួនគត់ h, ចំនួនគត់ n, ចំនួនគត់ classes, ចំនួនគត់ coords);
void forward_region_layer(const layer l, network net);
void backward_region_layer(const layer l, network net);
void resize_region_layer(layer *l, ចំនួនគត់ w, ចំនួនគត់ h);

#ifdef GPU
void forward_region_layer_gpu(const layer l, network net);
void backward_region_layer_gpu(layer l, network net);
#endif

#endif
