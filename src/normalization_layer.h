#جے_بنا_تعریف NORMALIZATION_LAYER_H
#تعریف_کرو NORMALIZATION_LAYER_H

#ضمن "image.h"
#ضمن "layer.h"
#ضمن "network.h"

layer make_normalization_layer(صحيح batch, صحيح w, صحيح h, صحيح c, صحيح ناپ, عائم alpha, عائم beta, عائم kappa);
فراغ resize_normalization_layer(layer *layer, صحيح h, صحيح w);
فراغ forward_normalization_layer(ثابت layer layer, جال net);
فراغ backward_normalization_layer(ثابت layer layer, جال net);
فراغ visualize_normalization_layer(layer layer, حرف *کھڑکی);

#جے_تعریف_شدہ GPU
فراغ forward_normalization_layer_gpu(ثابت layer layer, جال net);
فراغ backward_normalization_layer_gpu(ثابت layer layer, جال net);
#جے_انت

#جے_انت
