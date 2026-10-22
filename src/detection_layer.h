#که_نه_تعریف_شوی DETECTION_LAYER_H
#تعریف_کړه DETECTION_LAYER_H

#ضمن "layer.h"
#ضمن "network.h"

تعريفنوع layer detection_layer;

detection_layer make_detection_layer(صحيح batch, صحيح inputs, صحيح n, صحيح اندازه, صحيح classes, صحيح coords, صحيح rescore);
فراغ forward_detection_layer(ثابت detection_layer l, شبکه net);
فراغ backward_detection_layer(ثابت detection_layer l, شبکه net);

#که_تعریف_شوی GPU
فراغ forward_detection_layer_gpu(ثابت detection_layer l, شبکه net);
فراغ backward_detection_layer_gpu(detection_layer l, شبکه net);
#د_که_پای

#د_که_پای
