
#ifndef RNN_LAYER_H
#define RNN_LAYER_H

#include "activations.h"
#include "layer.h"
#include "network.h"
#define USET

layer make_rnn_layer(ကိန်းပြည့် batch, ကိန်းပြည့် inputs, ကိန်းပြည့် outputs, ကိန်းပြည့် steps, ACTIVATION activation, ကိန်းပြည့် batch_normalize, ကိန်းပြည့် adam);

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

