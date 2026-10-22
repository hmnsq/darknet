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

layer make_deconvolutional_layer(จำนวนเต็ม batch, จำนวนเต็ม h, จำนวนเต็ม w, จำนวนเต็ม c, จำนวนเต็ม n, จำนวนเต็ม size, จำนวนเต็ม stride, จำนวนเต็ม padding, ACTIVATION activation, จำนวนเต็ม batch_normalize, จำนวนเต็ม adam);
void resize_deconvolutional_layer(layer *l, จำนวนเต็ม h, จำนวนเต็ม w);
void forward_deconvolutional_layer(const layer l, network net);
void update_deconvolutional_layer(layer l, update_args a);
void backward_deconvolutional_layer(layer l, network net);

#endif

