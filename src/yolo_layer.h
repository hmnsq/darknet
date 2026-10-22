#اگر_تعریف_نشده YOLO_LAYER_H
#تعریف_کن YOLO_LAYER_H

#ضمن "darknet.h"
#ضمن "layer.h"
#ضمن "network.h"

layer make_yolo_layer(صحيح batch, صحيح w, صحيح h, صحيح n, صحيح مجموع, صحيح *ماسک, صحيح classes);
فراغ forward_yolo_layer(ثابت layer l, شبکه net);
فراغ backward_yolo_layer(ثابت layer l, شبکه net);
فراغ resize_yolo_layer(layer *l, صحيح w, صحيح h);
صحيح yolo_رقم_detections(layer l, عائم thresh);

#اگر_تعریف_شده GPU
فراغ forward_yolo_layer_gpu(ثابت layer l, شبکه net);
فراغ backward_yolo_layer_gpu(layer l, شبکه net);
#پایان_اگر

#پایان_اگر
