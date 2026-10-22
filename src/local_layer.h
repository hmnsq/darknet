#جے_بنا_تعریف LOCAL_LAYER_H
#تعریف_کرو LOCAL_LAYER_H

#ضمن "cuda.h"
#ضمن "image.h"
#ضمن "activations.h"
#ضمن "layer.h"
#ضمن "network.h"

تعريفنوع layer local_layer;

#جے_تعریف_شدہ GPU
فراغ forward_local_layer_gpu(local_layer layer, جال net);
فراغ backward_local_layer_gpu(local_layer layer, جال net);
فراغ update_local_layer_gpu(local_layer layer, update_دلیلاں a);

فراغ ڈالو_local_layer(local_layer layer);
فراغ pull_local_layer(local_layer layer);
#جے_انت

local_layer make_local_layer(صحيح batch, صحيح h, صحيح w, صحيح c, صحيح n, صحيح ناپ, صحيح stride, صحيح pad, ACTIVATION activation);

فراغ forward_local_layer(ثابت local_layer layer, جال net);
فراغ backward_local_layer(local_layer layer, جال net);
فراغ update_local_layer(local_layer layer, update_دلیلاں a);

فراغ bias_output(عائم *output, عائم *biases, صحيح batch, صحيح n, صحيح ناپ);
فراغ backward_bias(عائم *bias_updates, عائم *delta, صحيح batch, صحيح n, صحيح ناپ);

#جے_انت

