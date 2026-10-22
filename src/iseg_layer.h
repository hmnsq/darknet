#اگر_تعریف_نشده ISEG_LAYER_H
#تعریف_کن ISEG_LAYER_H

#ضمن "darknet.h"
#ضمن "layer.h"
#ضمن "network.h"

layer make_iseg_layer(صحيح batch, صحيح w, صحيح h, صحيح classes, صحيح ids);
فراغ forward_iseg_layer(ثابت layer l, شبکه net);
فراغ backward_iseg_layer(ثابت layer l, شبکه net);
فراغ resize_iseg_layer(layer *l, صحيح w, صحيح h);
صحيح iseg_رقم_detections(layer l, عائم thresh);

#اگر_تعریف_شده GPU
فراغ forward_iseg_layer_gpu(ثابت layer l, شبکه net);
فراغ backward_iseg_layer_gpu(layer l, شبکه net);
#پایان_اگر

#پایان_اگر
