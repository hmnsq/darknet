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

layer make_deconvolutional_layer(පූර්ණ_සංඛ්‍යා batch, පූර්ණ_සංඛ්‍යා h, පූර්ණ_සංඛ්‍යා w, පූර්ණ_සංඛ්‍යා c, පූර්ණ_සංඛ්‍යා n, පූර්ණ_සංඛ්‍යා size, පූර්ණ_සංඛ්‍යා stride, පූර්ණ_සංඛ්‍යා padding, ACTIVATION activation, පූර්ණ_සංඛ්‍යා batch_normalize, පූර්ණ_සංඛ්‍යා adam);
void resize_deconvolutional_layer(layer *l, පූර්ණ_සංඛ්‍යා h, පූර්ණ_සංඛ්‍යා w);
void forward_deconvolutional_layer(const layer l, network net);
void update_deconvolutional_layer(layer l, update_args a);
void backward_deconvolutional_layer(layer l, network net);

#endif

