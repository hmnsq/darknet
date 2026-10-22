#جيڪڏهن_وصف_نه_ٿيل LOCAL_LAYER_H
#وصف_ڪريو LOCAL_LAYER_H

#ضمن "cuda.h"
#ضمن "image.h"
#ضمن "activations.h"
#ضمن "layer.h"
#ضمن "network.h"

تعريفنوع layer local_layer;

#جيڪڏهن_وصف_ٿيل GPU
فراغ forward_local_layer_gpu(local_layer layer, ڄار net);
فراغ backward_local_layer_gpu(local_layer layer, ڄار net);
فراغ update_local_layer_gpu(local_layer layer, update_دليلون a);

فراغ ڌڪو_local_layer(local_layer layer);
فراغ pull_local_layer(local_layer layer);
#جيڪڏهن_پورو

local_layer make_local_layer(صحيح batch, صحيح h, صحيح w, صحيح c, صحيح n, صحيح سائيز, صحيح stride, صحيح pad, ACTIVATION activation);

فراغ forward_local_layer(ثابت local_layer layer, ڄار net);
فراغ backward_local_layer(local_layer layer, ڄار net);
فراغ update_local_layer(local_layer layer, update_دليلون a);

فراغ bias_output(عائم *output, عائم *biases, صحيح batch, صحيح n, صحيح سائيز);
فراغ backward_bias(عائم *bias_updates, عائم *delta, صحيح batch, صحيح n, صحيح سائيز);

#جيڪڏهن_پورو

