
#ifndef CRNN_LAYER_H
#define CRNN_LAYER_H

#include "activations.h"
#include "layer.h"
#include "network.h"

layer make_crnn_layer(ကိန်းပြည့် batch, ကိန်းပြည့် h, ကိန်းပြည့် w, ကိန်းပြည့် c, ကိန်းပြည့် hidden_filters, ကိန်းပြည့် output_filters, ကိန်းပြည့် steps, ACTIVATION activation, ကိန်းပြည့် batch_normalize);

void forward_crnn_layer(layer l, network net);
void backward_crnn_layer(layer l, network net);
void update_crnn_layer(layer l, update_args a);

#ifdef GPU
void forward_crnn_layer_gpu(layer l, network net);
void backward_crnn_layer_gpu(layer l, network net);
void update_crnn_layer_gpu(layer l, update_args a);
void push_crnn_layer(layer l);
void pull_crnn_layer(layer l);
#endif

#endif

