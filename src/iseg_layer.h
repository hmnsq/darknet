#اگر_غیر_تعریف_شدہ ISEG_LAYER_H
#تعریف_کرو ISEG_LAYER_H

#شامل "darknet.h"
#شامل "layer.h"
#شامل "network.h"

layer make_iseg_layer(صحیح batch, صحیح w, صحیح h, صحیح classes, صحیح ids);
خالی forward_iseg_layer(ثابت layer l, جال net);
خالی backward_iseg_layer(ثابت layer l, جال net);
خالی resize_iseg_layer(layer *l, صحیح w, صحیح h);
صحیح iseg_عدد_detections(layer l, اعشاری thresh);

#اگر_تعریف_شدہ GPU
خالی forward_iseg_layer_gpu(ثابت layer l, جال net);
خالی backward_iseg_layer_gpu(layer l, جال net);
#اگر_ختم

#اگر_ختم
