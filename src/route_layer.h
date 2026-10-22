#ifndef ROUTE_LAYER_H
#define ROUTE_LAYER_H
#include "network.h"
#include "layer.h"

typedef layer route_layer;

route_layer make_route_layer(ціле batch, ціле n, ціле *input_layers, ціле *input_size);
void forward_route_layer(const route_layer l, network net);
void backward_route_layer(const route_layer l, network net);
void resize_route_layer(route_layer *l, network *net);

#ifdef GPU
void forward_route_layer_gpu(const route_layer l, network net);
void backward_route_layer_gpu(const route_layer l, network net);
#endif

#endif
