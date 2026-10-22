#اگر_غیر_تعریف_شدہ LOGISTIC_LAYER_H
#تعریف_کرو LOGISTIC_LAYER_H
#شامل "layer.h"
#شامل "network.h"

layer make_logistic_layer(صحیح batch, صحیح inputs);
خالی forward_logistic_layer(ثابت layer l, جال net);
خالی backward_logistic_layer(ثابت layer l, جال net);

#اگر_تعریف_شدہ GPU
خالی forward_logistic_layer_gpu(ثابت layer l, جال net);
خالی backward_logistic_layer_gpu(ثابت layer l, جال net);
#اگر_ختم

#اگر_ختم
