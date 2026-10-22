#ifndef UPSAMPLE_LAYER_H
#define UPSAMPLE_LAYER_H
#include "darknet.h"

layer make_upsample_layer(จำนวนเต็ม batch, จำนวนเต็ม w, จำนวนเต็ม h, จำนวนเต็ม c, จำนวนเต็ม stride);
void forward_upsample_layer(const layer l, network net);
void backward_upsample_layer(const layer l, network net);
void resize_upsample_layer(layer *l, จำนวนเต็ม w, จำนวนเต็ม h);

#ifdef GPU
void forward_upsample_layer_gpu(const layer l, network net);
void backward_upsample_layer_gpu(const layer l, network net);
#endif

#endif
