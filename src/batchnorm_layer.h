#اگر_غیر_تعریف_شدہ BATCHNORM_LAYER_H
#تعریف_کرو BATCHNORM_LAYER_H

#شامل "image.h"
#شامل "layer.h"
#شامل "network.h"

layer make_batchnorm_layer(صحیح batch, صحیح w, صحیح h, صحیح c);
خالی forward_batchnorm_layer(layer l, جال net);
خالی backward_batchnorm_layer(layer l, جال net);

#اگر_تعریف_شدہ GPU
خالی forward_batchnorm_layer_gpu(layer l, جال net);
خالی backward_batchnorm_layer_gpu(layer l, جال net);
خالی pull_batchnorm_layer(layer l);
خالی ڈالو_batchnorm_layer(layer l);
#اگر_ختم

#اگر_ختم
