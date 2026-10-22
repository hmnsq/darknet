#ifndef REGION_LAYER_H
#define REGION_LAYER_H

#include "darknet.h"
#include "layer.h"
#include "network.h"

layer make_region_layer(ຈຳນວນເຕັມ batch, ຈຳນວນເຕັມ w, ຈຳນວນເຕັມ h, ຈຳນວນເຕັມ n, ຈຳນວນເຕັມ classes, ຈຳນວນເຕັມ coords);
void forward_region_layer(const layer l, network net);
void backward_region_layer(const layer l, network net);
void resize_region_layer(layer *l, ຈຳນວນເຕັມ w, ຈຳນວນເຕັມ h);

#ifdef GPU
void forward_region_layer_gpu(const layer l, network net);
void backward_region_layer_gpu(layer l, network net);
#endif

#endif
