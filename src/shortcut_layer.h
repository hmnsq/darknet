#ifndef SHORTCUT_LAYER_H
#define SHORTCUT_LAYER_H

#include "layer.h"
#include "network.h"

layer make_shortcut_layer(puur_naa_nka batch, puur_naa_nka index, puur_naa_nka w, puur_naa_nka h, puur_naa_nka c, puur_naa_nka w2, puur_naa_nka h2, puur_naa_nka c2);
void forward_shortcut_layer(const layer l, network net);
void backward_shortcut_layer(const layer l, network net);
void resize_shortcut_layer(layer *l, puur_naa_nka w, puur_naa_nka h);

#ifdef GPU
void forward_shortcut_layer_gpu(const layer l, network net);
void backward_shortcut_layer_gpu(const layer l, network net);
#endif

#endif
