#جے_بنا_تعریف ROUTE_LAYER_H
#تعریف_کرو ROUTE_LAYER_H
#ضمن "network.h"
#ضمن "layer.h"

تعريفنوع layer route_layer;

route_layer make_route_layer(صحيح batch, صحيح n, صحيح *داخلہ_layers, صحيح *داخلہ_ناپ);
فراغ forward_route_layer(ثابت route_layer l, جال net);
فراغ backward_route_layer(ثابت route_layer l, جال net);
فراغ resize_route_layer(route_layer *l, جال *net);

#جے_تعریف_شدہ GPU
فراغ forward_route_layer_gpu(ثابت route_layer l, جال net);
فراغ backward_route_layer_gpu(ثابت route_layer l, جال net);
#جے_انت

#جے_انت
