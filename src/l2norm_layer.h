#جے_بنا_تعریف L2NORM_LAYER_H
#تعریف_کرو L2NORM_LAYER_H
#ضمن "layer.h"
#ضمن "network.h"

layer make_l2norm_layer(صحيح batch, صحيح inputs);
فراغ forward_l2norm_layer(ثابت layer l, جال net);
فراغ backward_l2norm_layer(ثابت layer l, جال net);

#جے_تعریف_شدہ GPU
فراغ forward_l2norm_layer_gpu(ثابت layer l, جال net);
فراغ backward_l2norm_layer_gpu(ثابت layer l, جال net);
#جے_انت

#جے_انت
