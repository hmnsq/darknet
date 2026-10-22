#ifndef REORG_LAYER_H
#define REORG_LAYER_H

#include "image.h"
#include "cuda.h"
#include "layer.h"
#include "network.h"

layer make_reorg_layer(정수 batch, 정수 w, 정수 h, 정수 c, 정수 stride, 정수 reverse, 정수 flatten, 정수 extra);
void resize_reorg_layer(layer *l, 정수 w, 정수 h);
void forward_reorg_layer(const layer l, network net);
void backward_reorg_layer(const layer l, network net);

#ifdef GPU
void forward_reorg_layer_gpu(layer l, network net);
void backward_reorg_layer_gpu(layer l, network net);
#endif

#endif

