#که_نه_تعریف_شوی LOGISTIC_LAYER_H
#تعریف_کړه LOGISTIC_LAYER_H
#ضمن "layer.h"
#ضمن "network.h"

layer make_logistic_layer(صحيح batch, صحيح inputs);
فراغ forward_logistic_layer(ثابت layer l, شبکه net);
فراغ backward_logistic_layer(ثابت layer l, شبکه net);

#که_تعریف_شوی GPU
فراغ forward_logistic_layer_gpu(ثابت layer l, شبکه net);
فراغ backward_logistic_layer_gpu(ثابت layer l, شبکه net);
#د_که_پای

#د_که_پای
