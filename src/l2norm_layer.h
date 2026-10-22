#ifndef L2NORM_LAYER_H
#define L2NORM_LAYER_H
#شامل "layer.h"
#شامل "network.h"

layer make_l2norm_layer(صحیح batch, صحیح inputs);
تهی forward_l2norm_layer(ثابت layer l, network net);
تهی backward_l2norm_layer(ثابت layer l, network net);

#ifdef GPU
تهی forward_l2norm_layer_gpu(ثابت layer l, network net);
تهی backward_l2norm_layer_gpu(ثابت layer l, network net);
#endif

#endif
