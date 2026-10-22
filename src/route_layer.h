#اگر_غیر_تعریف_شدہ ROUTE_LAYER_H
#تعریف_کرو ROUTE_LAYER_H
#شامل "network.h"
#شامل "layer.h"

قسمتعریف layer route_layer;

route_layer make_route_layer(صحیح batch, صحیح n, صحیح *داخلہ_layers, صحیح *داخلہ_سائز);
خالی forward_route_layer(ثابت route_layer l, جال net);
خالی backward_route_layer(ثابت route_layer l, جال net);
خالی resize_route_layer(route_layer *l, جال *net);

#اگر_تعریف_شدہ GPU
خالی forward_route_layer_gpu(ثابت route_layer l, جال net);
خالی backward_route_layer_gpu(ثابت route_layer l, جال net);
#اگر_ختم

#اگر_ختم
