#ifndef ROUTE_LAYER_H
#define ROUTE_LAYER_H
#ଅନ୍ତର୍ଭାଵ "network.h"
#ଅନ୍ତର୍ଭାଵ "layer.h"

ପ୍ରକାରବ୍ୟାଖ୍ୟା layer route_layer;

route_layer make_route_layer(ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *ନିବେଶ_layers, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *ନିବେଶ_size);
ଶୂନ୍ୟ forward_route_layer(ଅଚଳ route_layer l, network net);
ଶୂନ୍ୟ backward_route_layer(ଅଚଳ route_layer l, network net);
ଶୂନ୍ୟ resize_route_layer(route_layer *l, network *net);

#ifdef GPU
ଶୂନ୍ୟ forward_route_layer_gpu(ଅଚଳ route_layer l, network net);
ଶୂନ୍ୟ backward_route_layer_gpu(ଅଚଳ route_layer l, network net);
#endif

#endif
