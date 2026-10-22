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

void add_bias_gpu(float *output, float *biases, පූර්ණ_සංඛ්‍යා batch, පූර්ණ_සංඛ්‍යා n, පූර්ණ_සංඛ්‍යා size);
void backward_bias_gpu(float *bias_updates, float *delta, පූර්ණ_සංඛ්‍යා batch, පූර්ණ_සංඛ්‍යා n, පූර්ණ_සංඛ්‍යා size);
void adam_update_gpu(float *w, float *d, float *m, float *v, float B1, float B2, float eps, float decay, float rate, පූර්ණ_සංඛ්‍යා n, පූර්ණ_සංඛ්‍යා batch, පූර්ණ_සංඛ්‍යා t);
#ifdef CUDNN
void cudnn_convolutional_setup(layer *l);
#endif
#endif

convolutional_layer make_convolutional_layer(පූර්ණ_සංඛ්‍යා batch, පූර්ණ_සංඛ්‍යා h, පූර්ණ_සංඛ්‍යා w, පූර්ණ_සංඛ්‍යා c, පූර්ණ_සංඛ්‍යා n, පූර්ණ_සංඛ්‍යා groups, පූර්ණ_සංඛ්‍යා size, පූර්ණ_සංඛ්‍යා stride, පූර්ණ_සංඛ්‍යා padding, ACTIVATION activation, පූර්ණ_සංඛ්‍යා batch_normalize, පූර්ණ_සංඛ්‍යා binary, පූර්ණ_සංඛ්‍යා xnor, පූර්ණ_සංඛ්‍යා adam);
void resize_convolutional_layer(convolutional_layer *layer, පූර්ණ_සංඛ්‍යා w, පූර්ණ_සංඛ්‍යා h);
void forward_convolutional_layer(const convolutional_layer layer, network net);
void update_convolutional_layer(convolutional_layer layer, update_args a);
image *visualize_convolutional_layer(convolutional_layer layer, var_na *window, image *prev_weights);
void binarize_weights(float *weights, පූර්ණ_සංඛ්‍යා n, පූර්ණ_සංඛ්‍යා size, float *binary);
void swap_binary(convolutional_layer *l);
void binarize_weights2(float *weights, පූර්ණ_සංඛ්‍යා n, පූර්ණ_සංඛ්‍යා size, var_na *binary, float *scales);

void backward_convolutional_layer(convolutional_layer layer, network net);

void add_bias(float *output, float *biases, පූර්ණ_සංඛ්‍යා batch, පූර්ණ_සංඛ්‍යා n, පූර්ණ_සංඛ්‍යා size);
void backward_bias(float *bias_updates, float *delta, පූර්ණ_සංඛ්‍යා batch, පූර්ණ_සංඛ්‍යා n, පූර්ණ_සංඛ්‍යා size);

image get_convolutional_image(convolutional_layer layer);
image get_convolutional_delta(convolutional_layer layer);
image get_convolutional_weight(convolutional_layer layer, පූර්ණ_සංඛ්‍යා i);

පූර්ණ_සංඛ්‍යා convolutional_out_height(convolutional_layer layer);
පූර්ණ_සංඛ්‍යා convolutional_out_width(convolutional_layer layer);

#endif

