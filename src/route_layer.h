#ifndef ROUTE_LAYER_H
#define ROUTE_LAYER_H
#अन्तर्भाव "network.h"
#अन्तर्भाव "layer.h"

प्रकारव्याख्या layer route_layer;

route_layer make_route_layer(पूर्णांक batch, पूर्णांक n, पूर्णांक *आदान_layers, पूर्णांक *आदान_size);
शून्य forward_route_layer(स्थिरांक route_layer l, network net);
शून्य backward_route_layer(स्थिरांक route_layer l, network net);
शून्य resize_route_layer(route_layer *l, network *net);

#ifdef GPU
शून्य forward_route_layer_gpu(स्थिरांक route_layer l, network net);
शून्य backward_route_layer_gpu(स्थिरांक route_layer l, network net);
#endif

#endif
