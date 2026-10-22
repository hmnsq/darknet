#ifndef DECONVOLUTIONAL_LAYER_H
#define DECONVOLUTIONAL_LAYER_H

#ضمن "cuda.h"
#ضمن "image.h"
#ضمن "activations.h"
#ضمن "layer.h"
#ضمن "network.h"

#ifdef GPU
فراغ forward_deconvolutional_layer_gpu(layer l, network net);
فراغ backward_deconvolutional_layer_gpu(layer l, network net);
فراغ update_deconvolutional_layer_gpu(layer l, update_args a);
فراغ push_deconvolutional_layer(layer l);
فراغ pull_deconvolutional_layer(layer l);
#endif

layer make_deconvolutional_layer(صحيح batch, صحيح h, صحيح w, صحيح c, صحيح n, صحيح size, صحيح stride, صحيح padding, ACTIVATION activation, صحيح batch_normalize, صحيح adam);
فراغ resize_deconvolutional_layer(layer *l, صحيح h, صحيح w);
فراغ forward_deconvolutional_layer(ثابت layer l, network net);
فراغ update_deconvolutional_layer(layer l, update_args a);
فراغ backward_deconvolutional_layer(layer l, network net);

#endif

