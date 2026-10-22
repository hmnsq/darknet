#اگر_غیر_تعریف_شدہ L2NORM_LAYER_H
#تعریف_کرو L2NORM_LAYER_H
#شامل "layer.h"
#شامل "network.h"

layer make_l2norm_layer(صحیح batch, صحیح inputs);
خالی forward_l2norm_layer(ثابت layer l, جال net);
خالی backward_l2norm_layer(ثابت layer l, جال net);

#اگر_تعریف_شدہ GPU
خالی forward_l2norm_layer_gpu(ثابت layer l, جال net);
خالی backward_l2norm_layer_gpu(ثابت layer l, جال net);
#اگر_ختم

#اگر_ختم
