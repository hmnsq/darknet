#ifndef MAXPOOL_LAYER_H
#define MAXPOOL_LAYER_H

#ضمن "image.h"
#ضمن "cuda.h"
#ضمن "layer.h"
#ضمن "network.h"

تعريفنوع layer maxpool_layer;

image get_maxpool_image(maxpool_layer l);
maxpool_layer make_maxpool_layer(صحيح batch, صحيح h, صحيح w, صحيح c, صحيح size, صحيح stride, صحيح padding);
فراغ resize_maxpool_layer(maxpool_layer *l, صحيح w, صحيح h);
فراغ forward_maxpool_layer(ثابت maxpool_layer l, network net);
فراغ backward_maxpool_layer(ثابت maxpool_layer l, network net);

#ifdef GPU
فراغ forward_maxpool_layer_gpu(maxpool_layer l, network net);
فراغ backward_maxpool_layer_gpu(maxpool_layer l, network net);
#endif

#endif

