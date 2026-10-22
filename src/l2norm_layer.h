#اگر_تعریف_نشده L2NORM_LAYER_H
#تعریف_کن L2NORM_LAYER_H
#ضمن "layer.h"
#ضمن "network.h"

layer make_l2norm_layer(صحيح batch, صحيح inputs);
فراغ forward_l2norm_layer(ثابت layer l, شبکه net);
فراغ backward_l2norm_layer(ثابت layer l, شبکه net);

#اگر_تعریف_شده GPU
فراغ forward_l2norm_layer_gpu(ثابت layer l, شبکه net);
فراغ backward_l2norm_layer_gpu(ثابت layer l, شبکه net);
#پایان_اگر

#پایان_اگر
