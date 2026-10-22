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

void add_bias_gpu(float *output, float *biases, 整数 batch, 整数 n, 整数 size);
void backward_bias_gpu(float *bias_updates, float *delta, 整数 batch, 整数 n, 整数 size);
void adam_update_gpu(float *w, float *d, float *m, float *v, float B1, float B2, float eps, float decay, float rate, 整数 n, 整数 batch, 整数 t);
#ifdef CUDNN
void cudnn_convolutional_setup(layer *l);
#endif
#endif

convolutional_layer make_convolutional_layer(整数 batch, 整数 h, 整数 w, 整数 c, 整数 n, 整数 groups, 整数 size, 整数 stride, 整数 padding, ACTIVATION activation, 整数 batch_normalize, 整数 binary, 整数 xnor, 整数 adam);
void resize_convolutional_layer(convolutional_layer *layer, 整数 w, 整数 h);
void forward_convolutional_layer(const convolutional_layer layer, network net);
void update_convolutional_layer(convolutional_layer layer, update_args a);
image *visualize_convolutional_layer(convolutional_layer layer, char *window, image *prev_weights);
void binarize_weights(float *weights, 整数 n, 整数 size, float *binary);
void swap_binary(convolutional_layer *l);
void binarize_weights2(float *weights, 整数 n, 整数 size, char *binary, float *scales);

void backward_convolutional_layer(convolutional_layer layer, network net);

void add_bias(float *output, float *biases, 整数 batch, 整数 n, 整数 size);
void backward_bias(float *bias_updates, float *delta, 整数 batch, 整数 n, 整数 size);

image get_convolutional_image(convolutional_layer layer);
image get_convolutional_delta(convolutional_layer layer);
image get_convolutional_weight(convolutional_layer layer, 整数 i);

整数 convolutional_out_height(convolutional_layer layer);
整数 convolutional_out_width(convolutional_layer layer);

#endif

