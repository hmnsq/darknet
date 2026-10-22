#ifndef LSTM_LAYER_H
#define LSTM_LAYER_H

#include "activations.h"
#include "layer.h"
#include "network.h"
#define USET

layer make_lstm_layer(จำนวนเต็ม batch, จำนวนเต็ม inputs, จำนวนเต็ม outputs, จำนวนเต็ม steps, จำนวนเต็ม batch_normalize, จำนวนเต็ม adam);

void forward_lstm_layer(layer l, network net); 
void update_lstm_layer(layer l, update_args a);

#ifdef GPU
void forward_lstm_layer_gpu(layer l, network net);
void backward_lstm_layer_gpu(layer l, network net);
void update_lstm_layer_gpu(layer l, update_args a); 

#endif
#endif
