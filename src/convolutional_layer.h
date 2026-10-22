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

void add_bias_gpu(float *output, float *biases, số_nguyên batch, số_nguyên n, số_nguyên size);
void backward_bias_gpu(float *bias_updates, float *delta, số_nguyên batch, số_nguyên n, số_nguyên size);
void adam_update_gpu(float *w, float *d, float *m, float *v, float B1, float B2, float eps, float decay, float rate, số_nguyên n, số_nguyên batch, số_nguyên t);
#ifdef CUDNN
void cudnn_convolutional_setup(layer *l);
#endif
#endif

convolutional_layer make_convolutional_layer(số_nguyên batch, số_nguyên h, số_nguyên w, số_nguyên c, số_nguyên n, số_nguyên groups, số_nguyên size, số_nguyên stride, số_nguyên padding, ACTIVATION activation, số_nguyên batch_normalize, số_nguyên binary, số_nguyên xnor, số_nguyên adam);
void resize_convolutional_layer(convolutional_layer *layer, số_nguyên w, số_nguyên h);
void forward_convolutional_layer(const convolutional_layer layer, network net);
void update_convolutional_layer(convolutional_layer layer, update_args a);
image *visualize_convolutional_layer(convolutional_layer layer, char *window, image *prev_weights);
void binarize_weights(float *weights, số_nguyên n, số_nguyên size, float *binary);
void swap_binary(convolutional_layer *l);
void binarize_weights2(float *weights, số_nguyên n, số_nguyên size, char *binary, float *scales);

void backward_convolutional_layer(convolutional_layer layer, network net);

void add_bias(float *output, float *biases, số_nguyên batch, số_nguyên n, số_nguyên size);
void backward_bias(float *bias_updates, float *delta, số_nguyên batch, số_nguyên n, số_nguyên size);

image get_convolutional_image(convolutional_layer layer);
image get_convolutional_delta(convolutional_layer layer);
image get_convolutional_weight(convolutional_layer layer, số_nguyên i);

số_nguyên convolutional_out_height(convolutional_layer layer);
số_nguyên convolutional_out_width(convolutional_layer layer);

#endif

