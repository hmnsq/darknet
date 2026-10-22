
#ifndef GRU_LAYER_H
#define GRU_LAYER_H

#include "activations.h"
#include "layer.h"
#include "network.h"

layer make_gru_layer(පූර්ණ_සංඛ්‍යා batch, පූර්ණ_සංඛ්‍යා inputs, පූර්ණ_සංඛ්‍යා outputs, පූර්ණ_සංඛ්‍යා steps, පූර්ණ_සංඛ්‍යා batch_normalize, පූර්ණ_සංඛ්‍යා adam);

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

