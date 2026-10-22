#ifndef REGION_LAYER_H
#define REGION_LAYER_H

#include "darknet.h"
#include "layer.h"
#include "network.h"

layer make_region_layer(ကိန်းပြည့် batch, ကိန်းပြည့် w, ကိန်းပြည့် h, ကိန်းပြည့် n, ကိန်းပြည့် classes, ကိန်းပြည့် coords);
void forward_region_layer(const layer l, network net);
void backward_region_layer(const layer l, network net);
void resize_region_layer(layer *l, ကိန်းပြည့် w, ကိန်းပြည့် h);

#ifdef GPU
void forward_region_layer_gpu(const layer l, network net);
void backward_region_layer_gpu(layer l, network net);
#endif

#endif
