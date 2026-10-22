#अगर_अघोषित ROUTE_LAYER_H
#घोषणा ROUTE_LAYER_H
#समावेश "network.h"
#समावेश "layer.h"

प्रकार layer route_layer;

route_layer make_route_layer(पूर्णांक batch, पूर्णांक n, पूर्णांक *निवेश_layers, पूर्णांक *निवेश_आकार);
व्योम forward_route_layer(स्थिर route_layer l, network net);
व्योम backward_route_layer(स्थिर route_layer l, network net);
व्योम resize_route_layer(route_layer *l, network *net);

#अगर_घोषित GPU
व्योम forward_route_layer_gpu(स्थिर route_layer l, network net);
व्योम backward_route_layer_gpu(स्थिर route_layer l, network net);
#पूर्ण_अगर

#पूर्ण_अगर
