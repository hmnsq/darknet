#اگر_تعریف_نشده REGION_LAYER_H
#تعریف_کن REGION_LAYER_H

#ضمن "darknet.h"
#ضمن "layer.h"
#ضمن "network.h"

layer make_region_layer(صحيح batch, صحيح w, صحيح h, صحيح n, صحيح classes, صحيح coords);
فراغ forward_region_layer(ثابت layer l, شبکه net);
فراغ backward_region_layer(ثابت layer l, شبکه net);
فراغ resize_region_layer(layer *l, صحيح w, صحيح h);

#اگر_تعریف_شده GPU
فراغ forward_region_layer_gpu(ثابت layer l, شبکه net);
فراغ backward_region_layer_gpu(layer l, شبکه net);
#پایان_اگر

#پایان_اگر
