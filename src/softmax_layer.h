#ifndef SOFTMAX_LAYER_H
#define SOFTMAX_LAYER_H
#include "layer.h"
#include "network.h"

typedef layer softmax_layer;

void softmax_array(float *input, පූර්ණ_සංඛ්‍යා n, float temp, float *output);
softmax_layer make_softmax_layer(පූර්ණ_සංඛ්‍යා batch, පූර්ණ_සංඛ්‍යා inputs, පූර්ණ_සංඛ්‍යා groups);
void forward_softmax_layer(const softmax_layer l, network net);
void backward_softmax_layer(const softmax_layer l, network net);

#ifdef GPU
void pull_softmax_layer_output(const softmax_layer l);
void forward_softmax_layer_gpu(const softmax_layer l, network net);
void backward_softmax_layer_gpu(const softmax_layer l, network net);
#endif

#endif
