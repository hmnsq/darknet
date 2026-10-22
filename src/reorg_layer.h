#ifndef REORG_LAYER_H
#define REORG_LAYER_H

#include "image.h"
#include "cuda.h"
#include "layer.h"
#include "network.h"

layer make_reorg_layer(整数 batch, 整数 w, 整数 h, 整数 c, 整数 stride, 整数 reverse, 整数 flatten, 整数 extra);
void resize_reorg_layer(layer *l, 整数 w, 整数 h);
void forward_reorg_layer(const layer l, network net);
void backward_reorg_layer(const layer l, network net);

#ifdef GPU
void forward_reorg_layer_gpu(layer l, network net);
void backward_reorg_layer_gpu(layer l, network net);
#endif

#endif

