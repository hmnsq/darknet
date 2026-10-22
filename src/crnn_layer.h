
#ifndef CRNN_LAYER_H
#define CRNN_LAYER_H

#include "activations.h"
#include "layer.h"
#include "network.h"

layer make_crnn_layer(puur_naa_nka batch, puur_naa_nka h, puur_naa_nka w, puur_naa_nka c, puur_naa_nka hidden_filters, puur_naa_nka output_filters, puur_naa_nka steps, ACTIVATION activation, puur_naa_nka batch_normalize);

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

