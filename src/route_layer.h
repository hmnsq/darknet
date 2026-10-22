#ifndef ROUTE_LAYER_H
#define ROUTE_LAYER_H
#அந்தர்பாவ "network.h"
#அந்தர்பாவ "layer.h"

வகைவரை layer route_layer;

route_layer make_route_layer(முழு batch, முழு n, முழு *உள்ளீடு_layers, முழு *உள்ளீடு_size);
வெறுமை forward_route_layer(மாறிலி route_layer l, network net);
வெறுமை backward_route_layer(மாறிலி route_layer l, network net);
வெறுமை resize_route_layer(route_layer *l, network *net);

#ifdef GPU
வெறுமை forward_route_layer_gpu(மாறிலி route_layer l, network net);
வெறுமை backward_route_layer_gpu(மாறிலி route_layer l, network net);
#endif

#endif
