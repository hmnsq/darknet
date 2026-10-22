#ifndef SHORTCUT_LAYER_H
#define SHORTCUT_LAYER_H

#include "layer.h"
#include "network.h"

layer make_shortcut_layer(ຈຳນວນເຕັມ batch, ຈຳນວນເຕັມ index, ຈຳນວນເຕັມ w, ຈຳນວນເຕັມ h, ຈຳນວນເຕັມ c, ຈຳນວນເຕັມ w2, ຈຳນວນເຕັມ h2, ຈຳນວນເຕັມ c2);
void forward_shortcut_layer(const layer l, network net);
void backward_shortcut_layer(const layer l, network net);
void resize_shortcut_layer(layer *l, ຈຳນວນເຕັມ w, ຈຳນວນເຕັມ h);

#ifdef GPU
void forward_shortcut_layer_gpu(const layer l, network net);
void backward_shortcut_layer_gpu(const layer l, network net);
#endif

#endif
