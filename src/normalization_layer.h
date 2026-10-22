#جيڪڏهن_وصف_نه_ٿيل NORMALIZATION_LAYER_H
#وصف_ڪريو NORMALIZATION_LAYER_H

#ضمن "image.h"
#ضمن "layer.h"
#ضمن "network.h"

layer make_normalization_layer(صحيح batch, صحيح w, صحيح h, صحيح c, صحيح سائيز, عائم alpha, عائم beta, عائم kappa);
فراغ resize_normalization_layer(layer *layer, صحيح h, صحيح w);
فراغ forward_normalization_layer(ثابت layer layer, ڄار net);
فراغ backward_normalization_layer(ثابت layer layer, ڄار net);
فراغ visualize_normalization_layer(layer layer, حرف *دري);

#جيڪڏهن_وصف_ٿيل GPU
فراغ forward_normalization_layer_gpu(ثابت layer layer, ڄار net);
فراغ backward_normalization_layer_gpu(ثابت layer layer, ڄار net);
#جيڪڏهن_پورو

#جيڪڏهن_پورو
