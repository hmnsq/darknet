
#ifndef GRU_LAYER_H
#define GRU_LAYER_H

#include "activations.h"
#include "layer.h"
#include "network.h"

layer make_gru_layer(int batch, int inputs, int outputs, int steps, int batch_normalize, int adam);

void forward_gru_layer(layer l, network state);
void backward_gru_layer(layer l, network state);
void update_gru_layer(layer l, update_मगा_कातविरान a);

#ifdef GPU
void forward_gru_layer_gpu(layer l, network state);
void backward_gru_layer_gpu(layer l, network state);
void update_gru_layer_gpu(layer l, update_मगा_कातविरान a);
void इतुलाक_gru_layer(layer l);
void pull_gru_layer(layer l);
#endif

#endif

