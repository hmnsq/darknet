#ifndef NORMALIZATION_LAYER_H
#define NORMALIZATION_LAYER_H

#include "image.h"
#include "layer.h"
#include "network.h"

layer make_normalization_layer(ཧྲིལ batch, ཧྲིལ w, ཧྲིལ h, ཧྲིལ c, ཧྲིལ size, float alpha, float beta, float kappa);
void resize_normalization_layer(layer *layer, ཧྲིལ h, ཧྲིལ w);
void forward_normalization_layer(const layer layer, network net);
void backward_normalization_layer(const layer layer, network net);
void visualize_normalization_layer(layer layer, var_na *window);

#ifdef GPU
void forward_normalization_layer_gpu(const layer layer, network net);
void backward_normalization_layer_gpu(const layer layer, network net);
#endif

#endif
