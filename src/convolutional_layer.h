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

void add_bias_gpu(float *output, float *biases, ཧྲིལ batch, ཧྲིལ n, ཧྲིལ size);
void backward_bias_gpu(float *bias_updates, float *delta, ཧྲིལ batch, ཧྲིལ n, ཧྲིལ size);
void adam_update_gpu(float *w, float *d, float *m, float *v, float B1, float B2, float eps, float decay, float rate, ཧྲིལ n, ཧྲིལ batch, ཧྲིལ t);
#ifdef CUDNN
void cudnn_convolutional_setup(layer *l);
#endif
#endif

convolutional_layer make_convolutional_layer(ཧྲིལ batch, ཧྲིལ h, ཧྲིལ w, ཧྲིལ c, ཧྲིལ n, ཧྲིལ groups, ཧྲིལ size, ཧྲིལ stride, ཧྲིལ padding, ACTIVATION activation, ཧྲིལ batch_normalize, ཧྲིལ binary, ཧྲིལ xnor, ཧྲིལ adam);
void resize_convolutional_layer(convolutional_layer *layer, ཧྲིལ w, ཧྲིལ h);
void forward_convolutional_layer(const convolutional_layer layer, network net);
void update_convolutional_layer(convolutional_layer layer, update_args a);
image *visualize_convolutional_layer(convolutional_layer layer, var_na *window, image *prev_weights);
void binarize_weights(float *weights, ཧྲིལ n, ཧྲིལ size, float *binary);
void swap_binary(convolutional_layer *l);
void binarize_weights2(float *weights, ཧྲིལ n, ཧྲིལ size, var_na *binary, float *scales);

void backward_convolutional_layer(convolutional_layer layer, network net);

void add_bias(float *output, float *biases, ཧྲིལ batch, ཧྲིལ n, ཧྲིལ size);
void backward_bias(float *bias_updates, float *delta, ཧྲིལ batch, ཧྲིལ n, ཧྲིལ size);

image get_convolutional_image(convolutional_layer layer);
image get_convolutional_delta(convolutional_layer layer);
image get_convolutional_weight(convolutional_layer layer, ཧྲིལ i);

ཧྲིལ convolutional_out_height(convolutional_layer layer);
ཧྲིལ convolutional_out_width(convolutional_layer layer);

#endif

