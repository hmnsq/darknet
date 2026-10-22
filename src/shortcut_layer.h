#ifndef SHORTCUT_LAYER_H
#define SHORTCUT_LAYER_H

#include "layer.h"
#include "network.h"

layer make_shortcut_layer(ကိန်းပြည့် batch, ကိန်းပြည့် index, ကိန်းပြည့် w, ကိန်းပြည့် h, ကိန်းပြည့် c, ကိန်းပြည့် w2, ကိန်းပြည့် h2, ကိန်းပြည့် c2);
void forward_shortcut_layer(const layer l, network net);
void backward_shortcut_layer(const layer l, network net);
void resize_shortcut_layer(layer *l, ကိန်းပြည့် w, ကိန်းပြည့် h);

#ifdef GPU
void forward_shortcut_layer_gpu(const layer l, network net);
void backward_shortcut_layer_gpu(const layer l, network net);
#endif

#endif
