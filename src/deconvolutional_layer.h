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

layer make_deconvolutional_layer(số_nguyên batch, số_nguyên h, số_nguyên w, số_nguyên c, số_nguyên n, số_nguyên size, số_nguyên stride, số_nguyên padding, ACTIVATION activation, số_nguyên batch_normalize, số_nguyên adam);
void resize_deconvolutional_layer(layer *l, số_nguyên h, số_nguyên w);
void forward_deconvolutional_layer(const layer l, network net);
void update_deconvolutional_layer(layer l, update_args a);
void backward_deconvolutional_layer(layer l, network net);

#endif

