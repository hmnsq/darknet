#جے_بنا_تعریف LOGISTIC_LAYER_H
#تعریف_کرو LOGISTIC_LAYER_H
#ضمن "layer.h"
#ضمن "network.h"

layer make_logistic_layer(صحيح batch, صحيح inputs);
فراغ forward_logistic_layer(ثابت layer l, جال net);
فراغ backward_logistic_layer(ثابت layer l, جال net);

#جے_تعریف_شدہ GPU
فراغ forward_logistic_layer_gpu(ثابت layer l, جال net);
فراغ backward_logistic_layer_gpu(ثابت layer l, جال net);
#جے_انت

#جے_انت
