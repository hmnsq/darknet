#اگر_تعریف_نشده LOGISTIC_LAYER_H
#تعریف_کن LOGISTIC_LAYER_H
#ضمن "layer.h"
#ضمن "network.h"

layer make_logistic_layer(صحيح batch, صحيح inputs);
فراغ forward_logistic_layer(ثابت layer l, شبکه net);
فراغ backward_logistic_layer(ثابت layer l, شبکه net);

#اگر_تعریف_شده GPU
فراغ forward_logistic_layer_gpu(ثابت layer l, شبکه net);
فراغ backward_logistic_layer_gpu(ثابت layer l, شبکه net);
#پایان_اگر

#پایان_اگر
