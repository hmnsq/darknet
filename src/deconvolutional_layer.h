#ifndef DECONVOLUTIONAL_LAYER_H
#define DECONVOLUTIONAL_LAYER_H

#include "cuda.h"
#include "image.h"
#include "activations.h"
#include "layer.h"
#include "network.h"

#ifdef GPU
void forward_deconvolutional_layer_gpu(layer l, network net);
void backward_deconvolutional_layer_gpu(layer l, network net);
void update_deconvolutional_layer_gpu(layer l, update_args a);
void push_deconvolutional_layer(layer l);
void pull_deconvolutional_layer(layer l);
#endif

layer make_deconvolutional_layer(정수 batch, 정수 h, 정수 w, 정수 c, 정수 n, 정수 size, 정수 stride, 정수 padding, ACTIVATION activation, 정수 batch_normalize, 정수 adam);
void resize_deconvolutional_layer(layer *l, 정수 h, 정수 w);
void forward_deconvolutional_layer(const layer l, network net);
void update_deconvolutional_layer(layer l, update_args a);
void backward_deconvolutional_layer(layer l, network net);

#endif

