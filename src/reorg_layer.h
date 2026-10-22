#ifndef REORG_LAYER_H
#define REORG_LAYER_H

#include "image.h"
#include "cuda.h"
#include "layer.h"
#include "network.h"

layer make_reorg_layer(puur_naa_nka batch, puur_naa_nka w, puur_naa_nka h, puur_naa_nka c, puur_naa_nka stride, puur_naa_nka reverse, puur_naa_nka flatten, puur_naa_nka extra);
void resize_reorg_layer(layer *l, puur_naa_nka w, puur_naa_nka h);
void forward_reorg_layer(const layer l, network net);
void backward_reorg_layer(const layer l, network net);

#ifdef GPU
void forward_reorg_layer_gpu(layer l, network net);
void backward_reorg_layer_gpu(layer l, network net);
#endif

#endif

