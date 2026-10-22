#ifndef UPSAMPLE_LAYER_H
#define UPSAMPLE_LAYER_H
#include "darknet.h"

layer make_upsample_layer(정수 batch, 정수 w, 정수 h, 정수 c, 정수 stride);
void forward_upsample_layer(const layer l, network net);
void backward_upsample_layer(const layer l, network net);
void resize_upsample_layer(layer *l, 정수 w, 정수 h);

#ifdef GPU
void forward_upsample_layer_gpu(const layer l, network net);
void backward_upsample_layer_gpu(const layer l, network net);
#endif

#endif
