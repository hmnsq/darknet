#اگر_تعریف_نشده BATCHNORM_LAYER_H
#تعریف_کن BATCHNORM_LAYER_H

#ضمن "image.h"
#ضمن "layer.h"
#ضمن "network.h"

layer make_batchnorm_layer(صحيح batch, صحيح w, صحيح h, صحيح c);
فراغ forward_batchnorm_layer(layer l, شبکه net);
فراغ backward_batchnorm_layer(layer l, شبکه net);

#اگر_تعریف_شده GPU
فراغ forward_batchnorm_layer_gpu(layer l, شبکه net);
فراغ backward_batchnorm_layer_gpu(layer l, شبکه net);
فراغ pull_batchnorm_layer(layer l);
فراغ افزودن_batchnorm_layer(layer l);
#پایان_اگر

#پایان_اگر
