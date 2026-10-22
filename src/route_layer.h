#ifndef ROUTE_LAYER_H
#define ROUTE_LAYER_H
#ضمن "network.h"
#ضمن "layer.h"

تعريفنوع layer route_layer;

route_layer make_route_layer(صحيح batch, صحيح n, صحيح *ماسوقن_layers, صحيح *ماسوقن_size);
فراغ forward_route_layer(ثابت route_layer l, network net);
فراغ backward_route_layer(ثابت route_layer l, network net);
فراغ resize_route_layer(route_layer *l, network *net);

#ifdef GPU
فراغ forward_route_layer_gpu(ثابت route_layer l, network net);
فراغ backward_route_layer_gpu(ثابت route_layer l, network net);
#endif

#endif
