#ifndef UPSAMPLE_LAYER_H
#define UPSAMPLE_LAYER_H
#include "darknet.h"

layer make_upsample_layer(ကိန်းပြည့် batch, ကိန်းပြည့် w, ကိန်းပြည့် h, ကိန်းပြည့် c, ကိန်းပြည့် stride);
void forward_upsample_layer(const layer l, network net);
void backward_upsample_layer(const layer l, network net);
void resize_upsample_layer(layer *l, ကိန်းပြည့် w, ကိန်းပြည့် h);

#ifdef GPU
void forward_upsample_layer_gpu(const layer l, network net);
void backward_upsample_layer_gpu(const layer l, network net);
#endif

#endif
