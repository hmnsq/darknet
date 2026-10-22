#جے_بنا_تعریف BATCHNORM_LAYER_H
#تعریف_کرو BATCHNORM_LAYER_H

#ضمن "image.h"
#ضمن "layer.h"
#ضمن "network.h"

layer make_batchnorm_layer(صحيح batch, صحيح w, صحيح h, صحيح c);
فراغ forward_batchnorm_layer(layer l, جال net);
فراغ backward_batchnorm_layer(layer l, جال net);

#جے_تعریف_شدہ GPU
فراغ forward_batchnorm_layer_gpu(layer l, جال net);
فراغ backward_batchnorm_layer_gpu(layer l, جال net);
فراغ pull_batchnorm_layer(layer l);
فراغ ڈالو_batchnorm_layer(layer l);
#جے_انت

#جے_انت
