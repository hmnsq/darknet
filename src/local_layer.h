#که_نه_تعریف_شوی LOCAL_LAYER_H
#تعریف_کړه LOCAL_LAYER_H

#ضمن "cuda.h"
#ضمن "image.h"
#ضمن "activations.h"
#ضمن "layer.h"
#ضمن "network.h"

تعريفنوع layer local_layer;

#که_تعریف_شوی GPU
فراغ forward_local_layer_gpu(local_layer layer, شبکه net);
فراغ backward_local_layer_gpu(local_layer layer, شبکه net);
فراغ update_local_layer_gpu(local_layer layer, update_دلیلونه a);

فراغ ټېل_local_layer(local_layer layer);
فراغ pull_local_layer(local_layer layer);
#د_که_پای

local_layer make_local_layer(صحيح batch, صحيح h, صحيح w, صحيح c, صحيح n, صحيح اندازه, صحيح stride, صحيح pad, ACTIVATION activation);

فراغ forward_local_layer(ثابت local_layer layer, شبکه net);
فراغ backward_local_layer(local_layer layer, شبکه net);
فراغ update_local_layer(local_layer layer, update_دلیلونه a);

فراغ bias_output(عائم *output, عائم *biases, صحيح batch, صحيح n, صحيح اندازه);
فراغ backward_bias(عائم *bias_updates, عائم *delta, صحيح batch, صحيح n, صحيح اندازه);

#د_که_پای

