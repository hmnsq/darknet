#ifndef REORG_LAYER_H
#define REORG_LAYER_H

#include "image.h"
#include "cuda.h"
#include "layer.h"
#include "network.h"

layer make_reorg_layer(ཧྲིལ batch, ཧྲིལ w, ཧྲིལ h, ཧྲིལ c, ཧྲིལ stride, ཧྲིལ reverse, ཧྲིལ flatten, ཧྲིལ extra);
void resize_reorg_layer(layer *l, ཧྲིལ w, ཧྲིལ h);
void forward_reorg_layer(const layer l, network net);
void backward_reorg_layer(const layer l, network net);

#ifdef GPU
void forward_reorg_layer_gpu(layer l, network net);
void backward_reorg_layer_gpu(layer l, network net);
#endif

#endif

