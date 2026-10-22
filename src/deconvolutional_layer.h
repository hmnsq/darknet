#جيڪڏهن_وصف_نه_ٿيل DECONVOLUTIONAL_LAYER_H
#وصف_ڪريو DECONVOLUTIONAL_LAYER_H

#ضمن "cuda.h"
#ضمن "image.h"
#ضمن "activations.h"
#ضمن "layer.h"
#ضمن "network.h"

#جيڪڏهن_وصف_ٿيل GPU
فراغ forward_deconvolutional_layer_gpu(layer l, ڄار net);
فراغ backward_deconvolutional_layer_gpu(layer l, ڄار net);
فراغ update_deconvolutional_layer_gpu(layer l, update_دليلون a);
فراغ ڌڪو_deconvolutional_layer(layer l);
فراغ pull_deconvolutional_layer(layer l);
#جيڪڏهن_پورو

layer make_deconvolutional_layer(صحيح batch, صحيح h, صحيح w, صحيح c, صحيح n, صحيح سائيز, صحيح stride, صحيح padding, ACTIVATION activation, صحيح batch_normalize, صحيح adam);
فراغ resize_deconvolutional_layer(layer *l, صحيح h, صحيح w);
فراغ forward_deconvolutional_layer(ثابت layer l, ڄار net);
فراغ update_deconvolutional_layer(layer l, update_دليلون a);
فراغ backward_deconvolutional_layer(layer l, ڄار net);

#جيڪڏهن_پورو

