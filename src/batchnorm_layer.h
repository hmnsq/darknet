#ifndef BATCHNORM_LAYER_H
#define BATCHNORM_LAYER_H

#include "image.h"
#include "layer.h"
#include "network.h"

layer make_batchnorm_layer(정수 batch, 정수 w, 정수 h, 정수 c);
void forward_batchnorm_layer(layer l, network net);
void backward_batchnorm_layer(layer l, network net);

#ifdef GPU
void forward_batchnorm_layer_gpu(layer l, network net);
void backward_batchnorm_layer_gpu(layer l, network net);
void pull_batchnorm_layer(layer l);
void push_batchnorm_layer(layer l);
#endif

#endif
