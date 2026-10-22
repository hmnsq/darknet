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

layer make_deconvolutional_layer(ចំនួនគត់ batch, ចំនួនគត់ h, ចំនួនគត់ w, ចំនួនគត់ c, ចំនួនគត់ n, ចំនួនគត់ size, ចំនួនគត់ stride, ចំនួនគត់ padding, ACTIVATION activation, ចំនួនគត់ batch_normalize, ចំនួនគត់ adam);
void resize_deconvolutional_layer(layer *l, ចំនួនគត់ h, ចំនួនគត់ w);
void forward_deconvolutional_layer(const layer l, network net);
void update_deconvolutional_layer(layer l, update_args a);
void backward_deconvolutional_layer(layer l, network net);

#endif

