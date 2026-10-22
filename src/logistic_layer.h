#ifndef LOGISTIC_LAYER_H
#define LOGISTIC_LAYER_H
#include "layer.h"
#include "network.h"

layer make_logistic_layer(整数 batch, 整数 inputs);
void forward_logistic_layer(const layer l, network net);
void backward_logistic_layer(const layer l, network net);

#ifdef GPU
void forward_logistic_layer_gpu(const layer l, network net);
void backward_logistic_layer_gpu(const layer l, network net);
#endif

#endif
