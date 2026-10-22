#ifndef SHORTCUT_LAYER_H
#define SHORTCUT_LAYER_H

#include "layer.h"
#include "network.h"

layer make_shortcut_layer(ཧྲིལ batch, ཧྲིལ index, ཧྲིལ w, ཧྲིལ h, ཧྲིལ c, ཧྲིལ w2, ཧྲིལ h2, ཧྲིལ c2);
void forward_shortcut_layer(const layer l, network net);
void backward_shortcut_layer(const layer l, network net);
void resize_shortcut_layer(layer *l, ཧྲིལ w, ཧྲིལ h);

#ifdef GPU
void forward_shortcut_layer_gpu(const layer l, network net);
void backward_shortcut_layer_gpu(const layer l, network net);
#endif

#endif
