#که_نه_تعریف_شوی ROUTE_LAYER_H
#تعریف_کړه ROUTE_LAYER_H
#ضمن "network.h"
#ضمن "layer.h"

تعريفنوع layer route_layer;

route_layer make_route_layer(صحيح batch, صحيح n, صحيح *ننوت_layers, صحيح *ننوت_اندازه);
فراغ forward_route_layer(ثابت route_layer l, شبکه net);
فراغ backward_route_layer(ثابت route_layer l, شبکه net);
فراغ resize_route_layer(route_layer *l, شبکه *net);

#که_تعریف_شوی GPU
فراغ forward_route_layer_gpu(ثابت route_layer l, شبکه net);
فراغ backward_route_layer_gpu(ثابت route_layer l, شبکه net);
#د_که_پای

#د_که_پای
