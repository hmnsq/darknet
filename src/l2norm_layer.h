#که_نه_تعریف_شوی L2NORM_LAYER_H
#تعریف_کړه L2NORM_LAYER_H
#ضمن "layer.h"
#ضمن "network.h"

layer make_l2norm_layer(صحيح batch, صحيح inputs);
فراغ forward_l2norm_layer(ثابت layer l, شبکه net);
فراغ backward_l2norm_layer(ثابت layer l, شبکه net);

#که_تعریف_شوی GPU
فراغ forward_l2norm_layer_gpu(ثابت layer l, شبکه net);
فراغ backward_l2norm_layer_gpu(ثابت layer l, شبکه net);
#د_که_پای

#د_که_پای
