#ifndef REORG_LAYER_H
#define REORG_LAYER_H

#include "image.h"
#include "cuda.h"
#include "layer.h"
#include "network.h"

layer make_reorg_layer(ចំនួនគត់ batch, ចំនួនគត់ w, ចំនួនគត់ h, ចំនួនគត់ c, ចំនួនគត់ stride, ចំនួនគត់ reverse, ចំនួនគត់ flatten, ចំនួនគត់ extra);
void resize_reorg_layer(layer *l, ចំនួនគត់ w, ចំនួនគត់ h);
void forward_reorg_layer(const layer l, network net);
void backward_reorg_layer(const layer l, network net);

#ifdef GPU
void forward_reorg_layer_gpu(layer l, network net);
void backward_reorg_layer_gpu(layer l, network net);
#endif

#endif

