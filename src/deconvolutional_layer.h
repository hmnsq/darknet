#جے_بنا_تعریف DECONVOLUTIONAL_LAYER_H
#تعریف_کرو DECONVOLUTIONAL_LAYER_H

#ضمن "cuda.h"
#ضمن "image.h"
#ضمن "activations.h"
#ضمن "layer.h"
#ضمن "network.h"

#جے_تعریف_شدہ GPU
فراغ forward_deconvolutional_layer_gpu(layer l, جال net);
فراغ backward_deconvolutional_layer_gpu(layer l, جال net);
فراغ update_deconvolutional_layer_gpu(layer l, update_دلیلاں a);
فراغ ڈالو_deconvolutional_layer(layer l);
فراغ pull_deconvolutional_layer(layer l);
#جے_انت

layer make_deconvolutional_layer(صحيح batch, صحيح h, صحيح w, صحيح c, صحيح n, صحيح ناپ, صحيح stride, صحيح padding, ACTIVATION activation, صحيح batch_normalize, صحيح adam);
فراغ resize_deconvolutional_layer(layer *l, صحيح h, صحيح w);
فراغ forward_deconvolutional_layer(ثابت layer l, جال net);
فراغ update_deconvolutional_layer(layer l, update_دلیلاں a);
فراغ backward_deconvolutional_layer(layer l, جال net);

#جے_انت

