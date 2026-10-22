
#ifndef GRU_LAYER_H
#define GRU_LAYER_H

#include "activations.h"
#include "layer.h"
#include "network.h"

layer make_gru_layer(정수 batch, 정수 inputs, 정수 outputs, 정수 steps, 정수 batch_normalize, 정수 adam);

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

