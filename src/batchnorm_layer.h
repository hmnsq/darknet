#جيڪڏهن_وصف_نه_ٿيل BATCHNORM_LAYER_H
#وصف_ڪريو BATCHNORM_LAYER_H

#ضمن "image.h"
#ضمن "layer.h"
#ضمن "network.h"

layer make_batchnorm_layer(صحيح batch, صحيح w, صحيح h, صحيح c);
فراغ forward_batchnorm_layer(layer l, ڄار net);
فراغ backward_batchnorm_layer(layer l, ڄار net);

#جيڪڏهن_وصف_ٿيل GPU
فراغ forward_batchnorm_layer_gpu(layer l, ڄار net);
فراغ backward_batchnorm_layer_gpu(layer l, ڄار net);
فراغ pull_batchnorm_layer(layer l);
فراغ ڌڪو_batchnorm_layer(layer l);
#جيڪڏهن_پورو

#جيڪڏهن_پورو
