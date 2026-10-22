#ifndef CONVOLUTIONAL_LAYER_H
#define CONVOLUTIONAL_LAYER_H

#include "cuda.h"
#include "image.h"
#include "activations.h"
#include "layer.h"
#include "network.h"

typedef layer convolutional_layer;

#ifdef GPU
void forward_convolutional_layer_gpu(convolutional_layer layer, network net);
void backward_convolutional_layer_gpu(convolutional_layer layer, network net);
void update_convolutional_layer_gpu(convolutional_layer layer, update_args a);

void push_convolutional_layer(convolutional_layer layer);
void pull_convolutional_layer(convolutional_layer layer);

void add_bias_gpu(float *output, float *biases, ကိန်းပြည့် batch, ကိန်းပြည့် n, ကိန်းပြည့် size);
void backward_bias_gpu(float *bias_updates, float *delta, ကိန်းပြည့် batch, ကိန်းပြည့် n, ကိန်းပြည့် size);
void adam_update_gpu(float *w, float *d, float *m, float *v, float B1, float B2, float eps, float decay, float rate, ကိန်းပြည့် n, ကိန်းပြည့် batch, ကိန်းပြည့် t);
#ifdef CUDNN
void cudnn_convolutional_setup(layer *l);
#endif
#endif

convolutional_layer make_convolutional_layer(ကိန်းပြည့် batch, ကိန်းပြည့် h, ကိန်းပြည့် w, ကိန်းပြည့် c, ကိန်းပြည့် n, ကိန်းပြည့် groups, ကိန်းပြည့် size, ကိန်းပြည့် stride, ကိန်းပြည့် padding, ACTIVATION activation, ကိန်းပြည့် batch_normalize, ကိန်းပြည့် binary, ကိန်းပြည့် xnor, ကိန်းပြည့် adam);
void resize_convolutional_layer(convolutional_layer *layer, ကိန်းပြည့် w, ကိန်းပြည့် h);
void forward_convolutional_layer(const convolutional_layer layer, network net);
void update_convolutional_layer(convolutional_layer layer, update_args a);
image *visualize_convolutional_layer(convolutional_layer layer, var_na *window, image *prev_weights);
void binarize_weights(float *weights, ကိန်းပြည့် n, ကိန်းပြည့် size, float *binary);
void swap_binary(convolutional_layer *l);
void binarize_weights2(float *weights, ကိန်းပြည့် n, ကိန်းပြည့် size, var_na *binary, float *scales);

void backward_convolutional_layer(convolutional_layer layer, network net);

void add_bias(float *output, float *biases, ကိန်းပြည့် batch, ကိန်းပြည့် n, ကိန်းပြည့် size);
void backward_bias(float *bias_updates, float *delta, ကိန်းပြည့် batch, ကိန်းပြည့် n, ကိန်းပြည့် size);

image get_convolutional_image(convolutional_layer layer);
image get_convolutional_delta(convolutional_layer layer);
image get_convolutional_weight(convolutional_layer layer, ကိန်းပြည့် i);

ကိန်းပြည့် convolutional_out_height(convolutional_layer layer);
ကိန်းပြည့် convolutional_out_width(convolutional_layer layer);

#endif

