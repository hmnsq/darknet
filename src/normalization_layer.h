#ifndef NORMALIZATION_LAYER_H
#define NORMALIZATION_LAYER_H

#ضمن "image.h"
#ضمن "layer.h"
#ضمن "network.h"

layer make_normalization_layer(صحيح batch, صحيح w, صحيح h, صحيح c, صحيح size, عائم alpha, عائم beta, عائم kappa);
فراغ resize_normalization_layer(layer *layer, صحيح h, صحيح w);
فراغ forward_normalization_layer(ثابت layer layer, network net);
فراغ backward_normalization_layer(ثابت layer layer, network net);
فراغ visualize_normalization_layer(layer layer, حرف *window);

#ifdef GPU
فراغ forward_normalization_layer_gpu(ثابت layer layer, network net);
فراغ backward_normalization_layer_gpu(ثابت layer layer, network net);
#endif

#endif
