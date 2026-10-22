#ifndef LOGISTIC_LAYER_H
#define LOGISTIC_LAYER_H
#ضمن "layer.h"
#ضمن "network.h"

layer make_logistic_layer(صحيح batch, صحيح inputs);
فراغ forward_logistic_layer(ثابت layer l, network net);
فراغ backward_logistic_layer(ثابت layer l, network net);

#ifdef GPU
فراغ forward_logistic_layer_gpu(ثابت layer l, network net);
فراغ backward_logistic_layer_gpu(ثابت layer l, network net);
#endif

#endif
