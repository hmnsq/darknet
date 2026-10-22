#اگر_تعریف_نشده DECONVOLUTIONAL_LAYER_H
#تعریف_کن DECONVOLUTIONAL_LAYER_H

#ضمن "cuda.h"
#ضمن "image.h"
#ضمن "activations.h"
#ضمن "layer.h"
#ضمن "network.h"

#اگر_تعریف_شده GPU
فراغ forward_deconvolutional_layer_gpu(layer l, شبکه net);
فراغ backward_deconvolutional_layer_gpu(layer l, شبکه net);
فراغ update_deconvolutional_layer_gpu(layer l, update_آرگومانها a);
فراغ افزودن_deconvolutional_layer(layer l);
فراغ pull_deconvolutional_layer(layer l);
#پایان_اگر

layer make_deconvolutional_layer(صحيح batch, صحيح h, صحيح w, صحيح c, صحيح n, صحيح اندازه, صحيح stride, صحيح padding, ACTIVATION activation, صحيح batch_normalize, صحيح adam);
فراغ resize_deconvolutional_layer(layer *l, صحيح h, صحيح w);
فراغ forward_deconvolutional_layer(ثابت layer l, شبکه net);
فراغ update_deconvolutional_layer(layer l, update_آرگومانها a);
فراغ backward_deconvolutional_layer(layer l, شبکه net);

#پایان_اگر

