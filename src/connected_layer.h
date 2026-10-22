#ifndef CONNECTED_LAYER_H
#define CONNECTED_LAYER_H

#include "activations.h"
#include "layer.h"
#include "network.h"

layer make_connected_layer(පූර්ණ_සංඛ්‍යා batch, පූර්ණ_සංඛ්‍යා inputs, පූර්ණ_සංඛ්‍යා outputs, ACTIVATION activation, පූර්ණ_සංඛ්‍යා batch_normalize, පූර්ණ_සංඛ්‍යා adam);

void forward_connected_layer(layer l, network net);
void backward_connected_layer(layer l, network net);
void update_connected_layer(layer l, update_args a);

#ifdef GPU
void forward_connected_layer_gpu(layer l, network net);
void backward_connected_layer_gpu(layer l, network net);
void update_connected_layer_gpu(layer l, update_args a);
void push_connected_layer(layer l);
void pull_connected_layer(layer l);
#endif

#endif

