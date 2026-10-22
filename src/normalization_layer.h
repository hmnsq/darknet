#که_نه_تعریف_شوی NORMALIZATION_LAYER_H
#تعریف_کړه NORMALIZATION_LAYER_H

#ضمن "image.h"
#ضمن "layer.h"
#ضمن "network.h"

layer make_normalization_layer(صحيح batch, صحيح w, صحيح h, صحيح c, صحيح اندازه, عائم alpha, عائم beta, عائم kappa);
فراغ resize_normalization_layer(layer *layer, صحيح h, صحيح w);
فراغ forward_normalization_layer(ثابت layer layer, شبکه net);
فراغ backward_normalization_layer(ثابت layer layer, شبکه net);
فراغ visualize_normalization_layer(layer layer, حرف *کړکۍ);

#که_تعریف_شوی GPU
فراغ forward_normalization_layer_gpu(ثابت layer layer, شبکه net);
فراغ backward_normalization_layer_gpu(ثابت layer layer, شبکه net);
#د_که_پای

#د_که_پای
