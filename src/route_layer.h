#ifndef ROUTE_LAYER_H
#define ROUTE_LAYER_H
#अन्तर्भाव "network.h"
#अन्तर्भाव "layer.h"

प्रकारनिर्देश layer route_layer;

route_layer make_route_layer(पूर्णाङ्क batch, पूर्णाङ्क n, पूर्णाङ्क *आगत_layers, पूर्णाङ्क *आगत_size);
शून्य forward_route_layer(अचर route_layer l, network net);
शून्य backward_route_layer(अचर route_layer l, network net);
शून्य resize_route_layer(route_layer *l, network *net);

#ifdef GPU
शून्य forward_route_layer_gpu(अचर route_layer l, network net);
शून्य backward_route_layer_gpu(अचर route_layer l, network net);
#endif

#endif
