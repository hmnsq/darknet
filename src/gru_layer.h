
#ifndef GRU_LAYER_H
#define GRU_LAYER_H

#include "activations.h"
#include "layer.h"
#include "network.h"

layer make_gru_layer(ကိန်းပြည့် batch, ကိန်းပြည့် inputs, ကိန်းပြည့် outputs, ကိန်းပြည့် steps, ကိန်းပြည့် batch_normalize, ကိန်းပြည့် adam);

void forward_gru_layer(layer l, network state);
void backward_gru_layer(layer l, network state);
void update_gru_layer(layer l, update_args a);

#ifdef GPU
void forward_gru_layer_gpu(layer l, network state);
void backward_gru_layer_gpu(layer l, network state);
void update_gru_layer_gpu(layer l, update_args a);
void push_gru_layer(layer l);
void pull_gru_layer(layer l);
#endif

#endif

