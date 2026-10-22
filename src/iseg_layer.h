#جے_بنا_تعریف ISEG_LAYER_H
#تعریف_کرو ISEG_LAYER_H

#ضمن "darknet.h"
#ضمن "layer.h"
#ضمن "network.h"

layer make_iseg_layer(صحيح batch, صحيح w, صحيح h, صحيح classes, صحيح ids);
فراغ forward_iseg_layer(ثابت layer l, جال net);
فراغ backward_iseg_layer(ثابت layer l, جال net);
فراغ resize_iseg_layer(layer *l, صحيح w, صحيح h);
صحيح iseg_عدد_detections(layer l, عائم thresh);

#جے_تعریف_شدہ GPU
فراغ forward_iseg_layer_gpu(ثابت layer l, جال net);
فراغ backward_iseg_layer_gpu(layer l, جال net);
#جے_انت

#جے_انت
