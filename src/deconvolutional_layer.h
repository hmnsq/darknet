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

layer make_deconvolutional_layer(ကိန်းပြည့် batch, ကိန်းပြည့် h, ကိန်းပြည့် w, ကိန်းပြည့် c, ကိန်းပြည့် n, ကိန်းပြည့် size, ကိန်းပြည့် stride, ကိန်းပြည့် padding, ACTIVATION activation, ကိန်းပြည့် batch_normalize, ကိန်းပြည့် adam);
void resize_deconvolutional_layer(layer *l, ကိန်းပြည့် h, ကိန်းပြည့် w);
void forward_deconvolutional_layer(const layer l, network net);
void update_deconvolutional_layer(layer l, update_args a);
void backward_deconvolutional_layer(layer l, network net);

#endif

