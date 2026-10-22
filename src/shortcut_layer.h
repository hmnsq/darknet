#ifndef SHORTCUT_LAYER_H
#define SHORTCUT_LAYER_H

#include "layer.h"
#include "network.h"

layer make_shortcut_layer(จำนวนเต็ม batch, จำนวนเต็ม index, จำนวนเต็ม w, จำนวนเต็ม h, จำนวนเต็ม c, จำนวนเต็ม w2, จำนวนเต็ม h2, จำนวนเต็ม c2);
void forward_shortcut_layer(const layer l, network net);
void backward_shortcut_layer(const layer l, network net);
void resize_shortcut_layer(layer *l, จำนวนเต็ม w, จำนวนเต็ม h);

#ifdef GPU
void forward_shortcut_layer_gpu(const layer l, network net);
void backward_shortcut_layer_gpu(const layer l, network net);
#endif

#endif
