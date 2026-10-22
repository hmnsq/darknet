#ifndef SHORTCUT_LAYER_H
#define SHORTCUT_LAYER_H

#include "layer.h"
#include "network.h"

layer make_shortcut_layer(ចំនួនគត់ batch, ចំនួនគត់ index, ចំនួនគត់ w, ចំនួនគត់ h, ចំនួនគត់ c, ចំនួនគត់ w2, ចំនួនគត់ h2, ចំនួនគត់ c2);
void forward_shortcut_layer(const layer l, network net);
void backward_shortcut_layer(const layer l, network net);
void resize_shortcut_layer(layer *l, ចំនួនគត់ w, ចំនួនគត់ h);

#ifdef GPU
void forward_shortcut_layer_gpu(const layer l, network net);
void backward_shortcut_layer_gpu(const layer l, network net);
#endif

#endif
