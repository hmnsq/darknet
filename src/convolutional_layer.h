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

void add_bias_gpu(float *output, float *biases, ចំនួនគត់ batch, ចំនួនគត់ n, ចំនួនគត់ size);
void backward_bias_gpu(float *bias_updates, float *delta, ចំនួនគត់ batch, ចំនួនគត់ n, ចំនួនគត់ size);
void adam_update_gpu(float *w, float *d, float *m, float *v, float B1, float B2, float eps, float decay, float rate, ចំនួនគត់ n, ចំនួនគត់ batch, ចំនួនគត់ t);
#ifdef CUDNN
void cudnn_convolutional_setup(layer *l);
#endif
#endif

convolutional_layer make_convolutional_layer(ចំនួនគត់ batch, ចំនួនគត់ h, ចំនួនគត់ w, ចំនួនគត់ c, ចំនួនគត់ n, ចំនួនគត់ groups, ចំនួនគត់ size, ចំនួនគត់ stride, ចំនួនគត់ padding, ACTIVATION activation, ចំនួនគត់ batch_normalize, ចំនួនគត់ binary, ចំនួនគត់ xnor, ចំនួនគត់ adam);
void resize_convolutional_layer(convolutional_layer *layer, ចំនួនគត់ w, ចំនួនគត់ h);
void forward_convolutional_layer(const convolutional_layer layer, network net);
void update_convolutional_layer(convolutional_layer layer, update_args a);
image *visualize_convolutional_layer(convolutional_layer layer, var_na *window, image *prev_weights);
void binarize_weights(float *weights, ចំនួនគត់ n, ចំនួនគត់ size, float *binary);
void swap_binary(convolutional_layer *l);
void binarize_weights2(float *weights, ចំនួនគត់ n, ចំនួនគត់ size, var_na *binary, float *scales);

void backward_convolutional_layer(convolutional_layer layer, network net);

void add_bias(float *output, float *biases, ចំនួនគត់ batch, ចំនួនគត់ n, ចំនួនគត់ size);
void backward_bias(float *bias_updates, float *delta, ចំនួនគត់ batch, ចំនួនគត់ n, ចំនួនគត់ size);

image get_convolutional_image(convolutional_layer layer);
image get_convolutional_delta(convolutional_layer layer);
image get_convolutional_weight(convolutional_layer layer, ចំនួនគត់ i);

ចំនួនគត់ convolutional_out_height(convolutional_layer layer);
ចំនួនគត់ convolutional_out_width(convolutional_layer layer);

#endif

