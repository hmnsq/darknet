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

local_layer make_local_layer(ကိန်းပြည့် batch, ကိန်းပြည့် h, ကိန်းပြည့် w, ကိန်းပြည့် c, ကိန်းပြည့် n, ကိန်းပြည့် size, ကိန်းပြည့် stride, ကိန်းပြည့် pad, ACTIVATION activation);

void forward_local_layer(const local_layer layer, network net);
void backward_local_layer(local_layer layer, network net);
void update_local_layer(local_layer layer, update_args a);

void bias_output(float *output, float *biases, ကိန်းပြည့် batch, ကိန်းပြည့် n, ကိန်းပြည့် size);
void backward_bias(float *bias_updates, float *delta, ကိန်းပြည့် batch, ကိန်းပြည့် n, ကိန်းပြည့် size);

#endif

