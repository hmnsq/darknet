#جيڪڏهن_وصف_نه_ٿيل ROUTE_LAYER_H
#وصف_ڪريو ROUTE_LAYER_H
#ضمن "network.h"
#ضمن "layer.h"

تعريفنوع layer route_layer;

route_layer make_route_layer(صحيح batch, صحيح n, صحيح *داخلا_layers, صحيح *داخلا_سائيز);
فراغ forward_route_layer(ثابت route_layer l, ڄار net);
فراغ backward_route_layer(ثابت route_layer l, ڄار net);
فراغ resize_route_layer(route_layer *l, ڄار *net);

#جيڪڏهن_وصف_ٿيل GPU
فراغ forward_route_layer_gpu(ثابت route_layer l, ڄار net);
فراغ backward_route_layer_gpu(ثابت route_layer l, ڄار net);
#جيڪڏهن_پورو

#جيڪڏهن_پورو
