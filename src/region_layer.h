#جے_بنا_تعریف REGION_LAYER_H
#تعریف_کرو REGION_LAYER_H

#ضمن "darknet.h"
#ضمن "layer.h"
#ضمن "network.h"

layer make_region_layer(صحيح batch, صحيح w, صحيح h, صحيح n, صحيح classes, صحيح coords);
فراغ forward_region_layer(ثابت layer l, جال net);
فراغ backward_region_layer(ثابت layer l, جال net);
فراغ resize_region_layer(layer *l, صحيح w, صحيح h);

#جے_تعریف_شدہ GPU
فراغ forward_region_layer_gpu(ثابت layer l, جال net);
فراغ backward_region_layer_gpu(layer l, جال net);
#جے_انت

#جے_انت
