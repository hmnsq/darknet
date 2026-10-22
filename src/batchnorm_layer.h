#ifndef BATCHNORM_LAYER_H
#define BATCHNORM_LAYER_H

#include "image.h"
#include "layer.h"
#include "network.h"

layer make_batchnorm_layer(ကိန်းပြည့် batch, ကိန်းပြည့် w, ကိန်းပြည့် h, ကိန်းပြည့် c);
void forward_batchnorm_layer(layer l, network net);
void backward_batchnorm_layer(layer l, network net);

#ifdef GPU
void forward_batchnorm_layer_gpu(layer l, network net);
void backward_batchnorm_layer_gpu(layer l, network net);
void pull_batchnorm_layer(layer l);
void push_batchnorm_layer(layer l);
#endif

#endif
