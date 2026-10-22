#ifndef REORG_LAYER_H
#define REORG_LAYER_H

#include "image.h"
#include "cuda.h"
#include "layer.h"
#include "network.h"

layer make_reorg_layer(ຈຳນວນເຕັມ batch, ຈຳນວນເຕັມ w, ຈຳນວນເຕັມ h, ຈຳນວນເຕັມ c, ຈຳນວນເຕັມ stride, ຈຳນວນເຕັມ reverse, ຈຳນວນເຕັມ flatten, ຈຳນວນເຕັມ extra);
void resize_reorg_layer(layer *l, ຈຳນວນເຕັມ w, ຈຳນວນເຕັມ h);
void forward_reorg_layer(const layer l, network net);
void backward_reorg_layer(const layer l, network net);

#ifdef GPU
void forward_reorg_layer_gpu(layer l, network net);
void backward_reorg_layer_gpu(layer l, network net);
#endif

#endif

