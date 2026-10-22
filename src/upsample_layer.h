#ifndef UPSAMPLE_LAYER_H
#define UPSAMPLE_LAYER_H
#include "darknet.h"

layer make_upsample_layer(ціле batch, ціле w, ціле h, ціле c, ціле stride);
void forward_upsample_layer(const layer l, network net);
void backward_upsample_layer(const layer l, network net);
void resize_upsample_layer(layer *l, ціле w, ціле h);

#ifdef GPU
void forward_upsample_layer_gpu(const layer l, network net);
void backward_upsample_layer_gpu(const layer l, network net);
#endif

#endif
