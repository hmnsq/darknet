#ifndef REORG_LAYER_H
#define REORG_LAYER_H

#include "image.h"
#include "cuda.h"
#include "layer.h"
#include "network.h"

layer make_reorg_layer(පූර්ණ_සංඛ්‍යා batch, පූර්ණ_සංඛ්‍යා w, පූර්ණ_සංඛ්‍යා h, පූර්ණ_සංඛ්‍යා c, පූර්ණ_සංඛ්‍යා stride, පූර්ණ_සංඛ්‍යා reverse, පූර්ණ_සංඛ්‍යා flatten, පූර්ණ_සංඛ්‍යා extra);
void resize_reorg_layer(layer *l, පූර්ණ_සංඛ්‍යා w, පූර්ණ_සංඛ්‍යා h);
void forward_reorg_layer(const layer l, network net);
void backward_reorg_layer(const layer l, network net);

#ifdef GPU
void forward_reorg_layer_gpu(layer l, network net);
void backward_reorg_layer_gpu(layer l, network net);
#endif

#endif

