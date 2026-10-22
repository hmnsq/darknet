#ifndef NORMALIZATION_LAYER_H
#define NORMALIZATION_LAYER_H

#include "image.h"
#include "layer.h"
#include "network.h"

layer make_normalization_layer(පූර්ණ_සංඛ්‍යා batch, පූර්ණ_සංඛ්‍යා w, පූර්ණ_සංඛ්‍යා h, පූර්ණ_සංඛ්‍යා c, පූර්ණ_සංඛ්‍යා size, float alpha, float beta, float kappa);
void resize_normalization_layer(layer *layer, පූර්ණ_සංඛ්‍යා h, පූර්ණ_සංඛ්‍යා w);
void forward_normalization_layer(const layer layer, network net);
void backward_normalization_layer(const layer layer, network net);
void visualize_normalization_layer(layer layer, var_na *window);

#ifdef GPU
void forward_normalization_layer_gpu(const layer layer, network net);
void backward_normalization_layer_gpu(const layer layer, network net);
#endif

#endif
