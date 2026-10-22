#جے_بنا_تعریف YOLO_LAYER_H
#تعریف_کرو YOLO_LAYER_H

#ضمن "darknet.h"
#ضمن "layer.h"
#ضمن "network.h"

layer make_yolo_layer(صحيح batch, صحيح w, صحيح h, صحيح n, صحيح کل, صحيح *نقاب, صحيح classes);
فراغ forward_yolo_layer(ثابت layer l, جال net);
فراغ backward_yolo_layer(ثابت layer l, جال net);
فراغ resize_yolo_layer(layer *l, صحيح w, صحيح h);
صحيح yolo_عدد_detections(layer l, عائم thresh);

#جے_تعریف_شدہ GPU
فراغ forward_yolo_layer_gpu(ثابت layer l, جال net);
فراغ backward_yolo_layer_gpu(layer l, جال net);
#جے_انت

#جے_انت
