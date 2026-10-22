#جے_بنا_تعریف DETECTION_LAYER_H
#تعریف_کرو DETECTION_LAYER_H

#ضمن "layer.h"
#ضمن "network.h"

تعريفنوع layer detection_layer;

detection_layer make_detection_layer(صحيح batch, صحيح inputs, صحيح n, صحيح ناپ, صحيح classes, صحيح coords, صحيح rescore);
فراغ forward_detection_layer(ثابت detection_layer l, جال net);
فراغ backward_detection_layer(ثابت detection_layer l, جال net);

#جے_تعریف_شدہ GPU
فراغ forward_detection_layer_gpu(ثابت detection_layer l, جال net);
فراغ backward_detection_layer_gpu(detection_layer l, جال net);
#جے_انت

#جے_انت
