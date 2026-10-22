#که_نه_تعریف_شوی BATCHNORM_LAYER_H
#تعریف_کړه BATCHNORM_LAYER_H

#ضمن "image.h"
#ضمن "layer.h"
#ضمن "network.h"

layer make_batchnorm_layer(صحيح batch, صحيح w, صحيح h, صحيح c);
فراغ forward_batchnorm_layer(layer l, شبکه net);
فراغ backward_batchnorm_layer(layer l, شبکه net);

#که_تعریف_شوی GPU
فراغ forward_batchnorm_layer_gpu(layer l, شبکه net);
فراغ backward_batchnorm_layer_gpu(layer l, شبکه net);
فراغ pull_batchnorm_layer(layer l);
فراغ ټېل_batchnorm_layer(layer l);
#د_که_پای

#د_که_پای
