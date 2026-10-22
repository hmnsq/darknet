#اگر_غیر_تعریف_شدہ DECONVOLUTIONAL_LAYER_H
#تعریف_کرو DECONVOLUTIONAL_LAYER_H

#شامل "cuda.h"
#شامل "image.h"
#شامل "activations.h"
#شامل "layer.h"
#شامل "network.h"

#اگر_تعریف_شدہ GPU
خالی forward_deconvolutional_layer_gpu(layer l, جال net);
خالی backward_deconvolutional_layer_gpu(layer l, جال net);
خالی update_deconvolutional_layer_gpu(layer l, update_دلائل a);
خالی ڈالو_deconvolutional_layer(layer l);
خالی pull_deconvolutional_layer(layer l);
#اگر_ختم

layer make_deconvolutional_layer(صحیح batch, صحیح h, صحیح w, صحیح c, صحیح n, صحیح سائز, صحیح stride, صحیح padding, ACTIVATION activation, صحیح batch_normalize, صحیح adam);
خالی resize_deconvolutional_layer(layer *l, صحیح h, صحیح w);
خالی forward_deconvolutional_layer(ثابت layer l, جال net);
خالی update_deconvolutional_layer(layer l, update_دلائل a);
خالی backward_deconvolutional_layer(layer l, جال net);

#اگر_ختم

