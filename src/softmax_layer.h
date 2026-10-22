#ifndef SOFTMAX_LAYER_H
#define SOFTMAX_LAYER_H
#include "layer.h"
#include "network.h"

typedef layer softmax_layer;

void softmax_array(float *input, 정수 n, float temp, float *output);
softmax_layer make_softmax_layer(정수 batch, 정수 inputs, 정수 groups);
void forward_softmax_layer(const softmax_layer l, network net);
void backward_softmax_layer(const softmax_layer l, network net);

#ifdef GPU
void pull_softmax_layer_output(const softmax_layer l);
void forward_softmax_layer_gpu(const softmax_layer l, network net);
void backward_softmax_layer_gpu(const softmax_layer l, network net);
#endif

#endif
