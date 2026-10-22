#ifndef AVGPOOL_LAYER_H
#define AVGPOOL_LAYER_H

#include "image.h"
#include "cuda.h"
#include "layer.h"
#include "network.h"

typedef layer avgpool_layer;

image get_avgpool_image(avgpool_layer l);
avgpool_layer make_avgpool_layer(ціле batch, ціле w, ціле h, ціле c);
void resize_avgpool_layer(avgpool_layer *l, ціле w, ціле h);
void forward_avgpool_layer(const avgpool_layer l, network net);
void backward_avgpool_layer(const avgpool_layer l, network net);

#ifdef GPU
void forward_avgpool_layer_gpu(avgpool_layer l, network net);
void backward_avgpool_layer_gpu(avgpool_layer l, network net);
#endif

#endif

