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

layer make_deconvolutional_layer(puur_naa_nka batch, puur_naa_nka h, puur_naa_nka w, puur_naa_nka c, puur_naa_nka n, puur_naa_nka size, puur_naa_nka stride, puur_naa_nka padding, ACTIVATION activation, puur_naa_nka batch_normalize, puur_naa_nka adam);
void resize_deconvolutional_layer(layer *l, puur_naa_nka h, puur_naa_nka w);
void forward_deconvolutional_layer(const layer l, network net);
void update_deconvolutional_layer(layer l, update_args a);
void backward_deconvolutional_layer(layer l, network net);

#endif

