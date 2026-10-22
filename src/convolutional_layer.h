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

void add_bias_gpu(float *output, float *biases, 정수 batch, 정수 n, 정수 size);
void backward_bias_gpu(float *bias_updates, float *delta, 정수 batch, 정수 n, 정수 size);
void adam_update_gpu(float *w, float *d, float *m, float *v, float B1, float B2, float eps, float decay, float rate, 정수 n, 정수 batch, 정수 t);
#ifdef CUDNN
void cudnn_convolutional_setup(layer *l);
#endif
#endif

convolutional_layer make_convolutional_layer(정수 batch, 정수 h, 정수 w, 정수 c, 정수 n, 정수 groups, 정수 size, 정수 stride, 정수 padding, ACTIVATION activation, 정수 batch_normalize, 정수 binary, 정수 xnor, 정수 adam);
void resize_convolutional_layer(convolutional_layer *layer, 정수 w, 정수 h);
void forward_convolutional_layer(const convolutional_layer layer, network net);
void update_convolutional_layer(convolutional_layer layer, update_args a);
image *visualize_convolutional_layer(convolutional_layer layer, char *window, image *prev_weights);
void binarize_weights(float *weights, 정수 n, 정수 size, float *binary);
void swap_binary(convolutional_layer *l);
void binarize_weights2(float *weights, 정수 n, 정수 size, char *binary, float *scales);

void backward_convolutional_layer(convolutional_layer layer, network net);

void add_bias(float *output, float *biases, 정수 batch, 정수 n, 정수 size);
void backward_bias(float *bias_updates, float *delta, 정수 batch, 정수 n, 정수 size);

image get_convolutional_image(convolutional_layer layer);
image get_convolutional_delta(convolutional_layer layer);
image get_convolutional_weight(convolutional_layer layer, 정수 i);

정수 convolutional_out_height(convolutional_layer layer);
정수 convolutional_out_width(convolutional_layer layer);

#endif

