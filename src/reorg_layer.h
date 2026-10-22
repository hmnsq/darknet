#ifndef REORG_LAYER_H
#define REORG_LAYER_H

#include "image.h"
#include "cuda.h"
#include "layer.h"
#include "network.h"

layer make_reorg_layer(จำนวนเต็ม batch, จำนวนเต็ม w, จำนวนเต็ม h, จำนวนเต็ม c, จำนวนเต็ม stride, จำนวนเต็ม reverse, จำนวนเต็ม flatten, จำนวนเต็ม extra);
void resize_reorg_layer(layer *l, จำนวนเต็ม w, จำนวนเต็ม h);
void forward_reorg_layer(const layer l, network net);
void backward_reorg_layer(const layer l, network net);

#ifdef GPU
void forward_reorg_layer_gpu(layer l, network net);
void backward_reorg_layer_gpu(layer l, network net);
#endif

#endif

