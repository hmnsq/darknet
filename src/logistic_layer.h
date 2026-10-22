#ifndef LOGISTIC_LAYER_H
#define LOGISTIC_LAYER_H
#شامل "layer.h"
#شامل "network.h"

layer make_logistic_layer(صحیح batch, صحیح inputs);
تهی forward_logistic_layer(ثابت layer l, network net);
تهی backward_logistic_layer(ثابت layer l, network net);

#ifdef GPU
تهی forward_logistic_layer_gpu(ثابت layer l, network net);
تهی backward_logistic_layer_gpu(ثابت layer l, network net);
#endif

#endif
