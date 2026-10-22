#ifndef REORG_LAYER_H
#define REORG_LAYER_H

#include "image.h"
#include "cuda.h"
#include "layer.h"
#include "network.h"

layer make_reorg_layer(số_nguyên batch, số_nguyên w, số_nguyên h, số_nguyên c, số_nguyên stride, số_nguyên reverse, số_nguyên flatten, số_nguyên extra);
void resize_reorg_layer(layer *l, số_nguyên w, số_nguyên h);
void forward_reorg_layer(const layer l, network net);
void backward_reorg_layer(const layer l, network net);

#ifdef GPU
void forward_reorg_layer_gpu(layer l, network net);
void backward_reorg_layer_gpu(layer l, network net);
#endif

#endif

