
#ifndef CRNN_LAYER_H
#define CRNN_LAYER_H

#include "activations.h"
#include "layer.h"
#include "network.h"

layer make_crnn_layer(ཧྲིལ batch, ཧྲིལ h, ཧྲིལ w, ཧྲིལ c, ཧྲིལ hidden_filters, ཧྲིལ output_filters, ཧྲིལ steps, ACTIVATION activation, ཧྲིལ batch_normalize);

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

