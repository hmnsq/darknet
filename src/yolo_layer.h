#که_نه_تعریف_شوی YOLO_LAYER_H
#تعریف_کړه YOLO_LAYER_H

#ضمن "darknet.h"
#ضمن "layer.h"
#ضمن "network.h"

layer make_yolo_layer(صحيح batch, صحيح w, صحيح h, صحيح n, صحيح ټول, صحيح *ماسک, صحيح classes);
فراغ forward_yolo_layer(ثابت layer l, شبکه net);
فراغ backward_yolo_layer(ثابت layer l, شبکه net);
فراغ resize_yolo_layer(layer *l, صحيح w, صحيح h);
صحيح yolo_عدد_detections(layer l, عائم thresh);

#که_تعریف_شوی GPU
فراغ forward_yolo_layer_gpu(ثابت layer l, شبکه net);
فراغ backward_yolo_layer_gpu(layer l, شبکه net);
#د_که_پای

#د_که_پای
