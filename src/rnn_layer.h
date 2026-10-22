
#ifndef RNN_LAYER_H
#define RNN_LAYER_H

#include "activations.h"
#include "layer.h"
#include "network.h"
#define USET

layer make_rnn_layer(จำนวนเต็ม batch, จำนวนเต็ม inputs, จำนวนเต็ม outputs, จำนวนเต็ม steps, ACTIVATION activation, จำนวนเต็ม batch_normalize, จำนวนเต็ม adam);

void forward_rnn_layer(layer l, network net);
void backward_rnn_layer(layer l, network net);
void update_rnn_layer(layer l, update_args a);

#ifdef GPU
void forward_rnn_layer_gpu(layer l, network net);
void backward_rnn_layer_gpu(layer l, network net);
void update_rnn_layer_gpu(layer l, update_args a);
void push_rnn_layer(layer l);
void pull_rnn_layer(layer l);
#endif

#endif

