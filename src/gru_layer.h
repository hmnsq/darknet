
#ifndef GRU_LAYER_H
#define GRU_LAYER_H

#include "activations.h"
#include "layer.h"
#include "network.h"

layer make_gru_layer(puur_naa_nka batch, puur_naa_nka inputs, puur_naa_nka outputs, puur_naa_nka steps, puur_naa_nka batch_normalize, puur_naa_nka adam);

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

