#ifndef REGION_LAYER_H
#define REGION_LAYER_H

#include "darknet.h"
#include "layer.h"
#include "network.h"

layer make_region_layer(จำนวนเต็ม batch, จำนวนเต็ม w, จำนวนเต็ม h, จำนวนเต็ม n, จำนวนเต็ม classes, จำนวนเต็ม coords);
void forward_region_layer(const layer l, network net);
void backward_region_layer(const layer l, network net);
void resize_region_layer(layer *l, จำนวนเต็ม w, จำนวนเต็ม h);

#ifdef GPU
void forward_region_layer_gpu(const layer l, network net);
void backward_region_layer_gpu(layer l, network net);
#endif

#endif
