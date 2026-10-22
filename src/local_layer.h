#ifndef LOCAL_LAYER_H
#define LOCAL_LAYER_H

#include "cuda.h"
#include "image.h"
#include "activations.h"
#include "layer.h"
#include "network.h"

typedef layer local_layer;

#ifdef GPU
void forward_local_layer_gpu(local_layer layer, network net);
void backward_local_layer_gpu(local_layer layer, network net);
void update_local_layer_gpu(local_layer layer, update_args a);

void push_local_layer(local_layer layer);
void pull_local_layer(local_layer layer);
#endif

local_layer make_local_layer(ціле batch, ціле h, ціле w, ціле c, ціле n, ціле size, ціле stride, ціле pad, ACTIVATION activation);

void forward_local_layer(const local_layer layer, network net);
void backward_local_layer(local_layer layer, network net);
void update_local_layer(local_layer layer, update_args a);

void bias_output(float *output, float *biases, ціле batch, ціле n, ціле size);
void backward_bias(float *bias_updates, float *delta, ціле batch, ціле n, ціле size);

#endif

