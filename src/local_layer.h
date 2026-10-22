#ifndef LOCAL_LAYER_H
#define LOCAL_LAYER_H

#ضمن "cuda.h"
#ضمن "image.h"
#ضمن "activations.h"
#ضمن "layer.h"
#ضمن "network.h"

تعريفنوع layer local_layer;

#ifdef GPU
فراغ forward_local_layer_gpu(local_layer layer, network net);
فراغ backward_local_layer_gpu(local_layer layer, network net);
فراغ update_local_layer_gpu(local_layer layer, update_args a);

فراغ ڈالو_local_layer(local_layer layer);
فراغ pull_local_layer(local_layer layer);
#endif

local_layer make_local_layer(صحيح batch, صحيح h, صحيح w, صحيح c, صحيح n, صحيح size, صحيح stride, صحيح pad, ACTIVATION activation);

فراغ forward_local_layer(ثابت local_layer layer, network net);
فراغ backward_local_layer(local_layer layer, network net);
فراغ update_local_layer(local_layer layer, update_args a);

فراغ bias_output(عائم *output, عائم *biases, صحيح batch, صحيح n, صحيح size);
فراغ backward_bias(عائم *bias_updates, عائم *delta, صحيح batch, صحيح n, صحيح size);

#endif

