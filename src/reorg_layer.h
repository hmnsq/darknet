#ifndef REORG_LAYER_H
#define REORG_LAYER_H

#include "image.h"
#include "cuda.h"
#include "layer.h"
#include "network.h"

layer make_reorg_layer(ကိန်းပြည့် batch, ကိန်းပြည့် w, ကိန်းပြည့် h, ကိန်းပြည့် c, ကိန်းပြည့် stride, ကိန်းပြည့် reverse, ကိန်းပြည့် flatten, ကိန်းပြည့် extra);
void resize_reorg_layer(layer *l, ကိန်းပြည့် w, ကိန်းပြည့် h);
void forward_reorg_layer(const layer l, network net);
void backward_reorg_layer(const layer l, network net);

#ifdef GPU
void forward_reorg_layer_gpu(layer l, network net);
void backward_reorg_layer_gpu(layer l, network net);
#endif

#endif

