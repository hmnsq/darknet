#ifndef ROUTE_LAYER_H
#define ROUTE_LAYER_H
#شامل "network.h"
#شامل "layer.h"

تعریفنوع layer route_layer;

route_layer make_route_layer(صحیح batch, صحیح n, صحیح *درون_داد_layers, صحیح *درون_داد_size);
تهی forward_route_layer(ثابت route_layer l, network net);
تهی backward_route_layer(ثابت route_layer l, network net);
تهی resize_route_layer(route_layer *l, network *net);

#ifdef GPU
تهی forward_route_layer_gpu(ثابت route_layer l, network net);
تهی backward_route_layer_gpu(ثابت route_layer l, network net);
#endif

#endif
