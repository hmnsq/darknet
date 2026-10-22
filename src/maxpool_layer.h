#ifndef MAXPOOL_LAYER_H
#define MAXPOOL_LAYER_H

#include "image.h"
#include "cuda.h"
#include "layer.h"
#include "network.h"

typedef layer maxpool_layer;

image get_maxpool_image(maxpool_layer l);
maxpool_layer make_maxpool_layer(පූර්ණ_සංඛ්‍යා batch, පූර්ණ_සංඛ්‍යා h, පූර්ණ_සංඛ්‍යා w, පූර්ණ_සංඛ්‍යා c, පූර්ණ_සංඛ්‍යා size, පූර්ණ_සංඛ්‍යා stride, පූර්ණ_සංඛ්‍යා padding);
void resize_maxpool_layer(maxpool_layer *l, පූර්ණ_සංඛ්‍යා w, පූර්ණ_සංඛ්‍යා h);
void forward_maxpool_layer(const maxpool_layer l, network net);
void backward_maxpool_layer(const maxpool_layer l, network net);

#ifdef GPU
void forward_maxpool_layer_gpu(maxpool_layer l, network net);
void backward_maxpool_layer_gpu(maxpool_layer l, network net);
#endif

#endif

