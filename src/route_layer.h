#اگر_تعریف_نشده ROUTE_LAYER_H
#تعریف_کن ROUTE_LAYER_H
#ضمن "network.h"
#ضمن "layer.h"

تعريفنوع layer route_layer;

route_layer make_route_layer(صحيح batch, صحيح n, صحيح *درون_داد_layers, صحيح *درون_داد_اندازه);
فراغ forward_route_layer(ثابت route_layer l, شبکه net);
فراغ backward_route_layer(ثابت route_layer l, شبکه net);
فراغ resize_route_layer(route_layer *l, شبکه *net);

#اگر_تعریف_شده GPU
فراغ forward_route_layer_gpu(ثابت route_layer l, شبکه net);
فراغ backward_route_layer_gpu(ثابت route_layer l, شبکه net);
#پایان_اگر

#پایان_اگر
