#اگر_غیر_تعریف_شدہ ACTIVATION_LAYER_H
#تعریف_کرو ACTIVATION_LAYER_H

#شامل "activations.h"
#شامل "layer.h"
#شامل "network.h"

layer make_activation_layer(صحیح batch, صحیح inputs, ACTIVATION activation);

خالی forward_activation_layer(layer l, جال net);
خالی backward_activation_layer(layer l, جال net);

#اگر_تعریف_شدہ GPU
خالی forward_activation_layer_gpu(layer l, جال net);
خالی backward_activation_layer_gpu(layer l, جال net);
#اگر_ختم

#اگر_ختم

