#ifndef NORMALIZATION_LAYER_H
#define NORMALIZATION_LAYER_H

#include "image.h"
#include "layer.h"
#include "network.h"

layer make_normalization_layer(целое batch, целое w, целое h, целое c, целое size, float alpha, float beta, float kappa);
void resize_normalization_layer(layer *layer, целое h, целое w);
void forward_normalization_layer(const layer layer, network net);
void backward_normalization_layer(const layer layer, network net);
void visualize_normalization_layer(layer layer, char *window);

#ifdef GPU
void forward_normalization_layer_gpu(const layer layer, network net);
void backward_normalization_layer_gpu(const layer layer, network net);
#endif

#endif
