#اگر_غیر_تعریف_شدہ NORMALIZATION_LAYER_H
#تعریف_کرو NORMALIZATION_LAYER_H

#شامل "image.h"
#شامل "layer.h"
#شامل "network.h"

layer make_normalization_layer(صحیح batch, صحیح w, صحیح h, صحیح c, صحیح سائز, اعشاری alpha, اعشاری beta, اعشاری kappa);
خالی resize_normalization_layer(layer *layer, صحیح h, صحیح w);
خالی forward_normalization_layer(ثابت layer layer, جال net);
خالی backward_normalization_layer(ثابت layer layer, جال net);
خالی visualize_normalization_layer(layer layer, حرف *کھڑکی);

#اگر_تعریف_شدہ GPU
خالی forward_normalization_layer_gpu(ثابت layer layer, جال net);
خالی backward_normalization_layer_gpu(ثابت layer layer, جال net);
#اگر_ختم

#اگر_ختم
