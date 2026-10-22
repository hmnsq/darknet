#ifndef L2NORM_LAYER_H
#define L2NORM_LAYER_H
#ضمن "layer.h"
#ضمن "network.h"

layer make_l2norm_layer(صحيح batch, صحيح inputs);
فراغ forward_l2norm_layer(ثابت layer l, network net);
فراغ backward_l2norm_layer(ثابت layer l, network net);

#ifdef GPU
فراغ forward_l2norm_layer_gpu(ثابت layer l, network net);
فراغ backward_l2norm_layer_gpu(ثابت layer l, network net);
#endif

#endif
