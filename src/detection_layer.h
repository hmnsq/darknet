#اگر_غیر_تعریف_شدہ DETECTION_LAYER_H
#تعریف_کرو DETECTION_LAYER_H

#شامل "layer.h"
#شامل "network.h"

قسمتعریف layer detection_layer;

detection_layer make_detection_layer(صحیح batch, صحیح inputs, صحیح n, صحیح سائز, صحیح classes, صحیح coords, صحیح rescore);
خالی forward_detection_layer(ثابت detection_layer l, جال net);
خالی backward_detection_layer(ثابت detection_layer l, جال net);

#اگر_تعریف_شدہ GPU
خالی forward_detection_layer_gpu(ثابت detection_layer l, جال net);
خالی backward_detection_layer_gpu(detection_layer l, جال net);
#اگر_ختم

#اگر_ختم
