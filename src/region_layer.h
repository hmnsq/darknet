#ifndef REGION_LAYER_H
#define REGION_LAYER_H

#include "darknet.h"
#include "layer.h"
#include "network.h"

layer make_region_layer(целое batch, целое w, целое h, целое n, целое classes, целое coords);
void forward_region_layer(const layer l, network net);
void backward_region_layer(const layer l, network net);
void resize_region_layer(layer *l, целое w, целое h);

#ifdef GPU
void forward_region_layer_gpu(const layer l, network net);
void backward_region_layer_gpu(layer l, network net);
#endif

#endif
