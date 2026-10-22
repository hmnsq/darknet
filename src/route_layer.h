#ifndef ROUTE_LAYER_H
#define ROUTE_LAYER_H
#অন্তর্ভাৱ "network.h"
#অন্তর্ভাৱ "layer.h"

প্রকারনির্দেশ layer route_layer;

route_layer make_route_layer(পূর্ণাঙ্ক batch, পূর্ণাঙ্ক n, পূর্ণাঙ্ক *প্ৰবেশ_layers, পূর্ণাঙ্ক *প্ৰবেশ_size);
শূন্য forward_route_layer(ধ্রুৱ route_layer l, network net);
শূন্য backward_route_layer(ধ্রুৱ route_layer l, network net);
শূন্য resize_route_layer(route_layer *l, network *net);

#ifdef GPU
শূন্য forward_route_layer_gpu(ধ্রুৱ route_layer l, network net);
শূন্য backward_route_layer_gpu(ধ্রুৱ route_layer l, network net);
#endif

#endif
