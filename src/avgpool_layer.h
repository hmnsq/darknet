#ifndef AVGPOOL_LAYER_H
#define AVGPOOL_LAYER_H

#include "image.h"
#include "cuda.h"
#include "layer.h"
#include "network.h"

typedef layer avgpool_layer;

image get_avgpool_image(avgpool_layer l);
avgpool_layer make_avgpool_layer(ကိန်းပြည့် batch, ကိန်းပြည့် w, ကိန်းပြည့် h, ကိန်းပြည့် c);
void resize_avgpool_layer(avgpool_layer *l, ကိန်းပြည့် w, ကိန်းပြည့် h);
void forward_avgpool_layer(const avgpool_layer l, network net);
void backward_avgpool_layer(const avgpool_layer l, network net);

#ifdef GPU
void forward_avgpool_layer_gpu(avgpool_layer l, network net);
void backward_avgpool_layer_gpu(avgpool_layer l, network net);
#endif

#endif

