#ifndef SHORTCUT_LAYER_H
#define SHORTCUT_LAYER_H

#include "layer.h"
#include "network.h"

layer make_shortcut_layer(целое batch, целое index, целое w, целое h, целое c, целое w2, целое h2, целое c2);
void forward_shortcut_layer(const layer l, network net);
void backward_shortcut_layer(const layer l, network net);
void resize_shortcut_layer(layer *l, целое w, целое h);

#ifdef GPU
void forward_shortcut_layer_gpu(const layer l, network net);
void backward_shortcut_layer_gpu(const layer l, network net);
#endif

#endif
