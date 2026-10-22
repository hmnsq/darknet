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

void add_bias_gpu(float *output, float *biases, целое batch, целое n, целое size);
void backward_bias_gpu(float *bias_updates, float *delta, целое batch, целое n, целое size);
void adam_update_gpu(float *w, float *d, float *m, float *v, float B1, float B2, float eps, float decay, float rate, целое n, целое batch, целое t);
#ifdef CUDNN
void cudnn_convolutional_setup(layer *l);
#endif
#endif

convolutional_layer make_convolutional_layer(целое batch, целое h, целое w, целое c, целое n, целое groups, целое size, целое stride, целое padding, ACTIVATION activation, целое batch_normalize, целое binary, целое xnor, целое adam);
void resize_convolutional_layer(convolutional_layer *layer, целое w, целое h);
void forward_convolutional_layer(const convolutional_layer layer, network net);
void update_convolutional_layer(convolutional_layer layer, update_args a);
image *visualize_convolutional_layer(convolutional_layer layer, char *window, image *prev_weights);
void binarize_weights(float *weights, целое n, целое size, float *binary);
void swap_binary(convolutional_layer *l);
void binarize_weights2(float *weights, целое n, целое size, char *binary, float *scales);

void backward_convolutional_layer(convolutional_layer layer, network net);

void add_bias(float *output, float *biases, целое batch, целое n, целое size);
void backward_bias(float *bias_updates, float *delta, целое batch, целое n, целое size);

image get_convolutional_image(convolutional_layer layer);
image get_convolutional_delta(convolutional_layer layer);
image get_convolutional_weight(convolutional_layer layer, целое i);

целое convolutional_out_height(convolutional_layer layer);
целое convolutional_out_width(convolutional_layer layer);

#endif

