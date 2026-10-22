#ifndef LSTM_LAYER_H
#define LSTM_LAYER_H

#include "activations.h"
#include "layer.h"
#include "network.h"
#define USET

layer make_lstm_layer(ចំនួនគត់ batch, ចំនួនគត់ inputs, ចំនួនគត់ outputs, ចំនួនគត់ steps, ចំនួនគត់ batch_normalize, ចំនួនគត់ adam);

void forward_lstm_layer(layer l, network net); 
void update_lstm_layer(layer l, update_args a);

#ifdef GPU
void forward_lstm_layer_gpu(layer l, network net);
void backward_lstm_layer_gpu(layer l, network net);
void update_lstm_layer_gpu(layer l, update_args a); 

#endif
#endif
