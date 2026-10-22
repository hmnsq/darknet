#اگر_غیر_تعریف_شدہ CONNECTED_LAYER_H
#تعریف_کرو CONNECTED_LAYER_H

#شامل "activations.h"
#شامل "layer.h"
#شامل "network.h"

layer make_connected_layer(صحیح batch, صحیح inputs, صحیح outputs, ACTIVATION activation, صحیح batch_normalize, صحیح adam);

خالی forward_connected_layer(layer l, جال net);
خالی backward_connected_layer(layer l, جال net);
خالی update_connected_layer(layer l, update_دلائل a);

#اگر_تعریف_شدہ GPU
خالی forward_connected_layer_gpu(layer l, جال net);
خالی backward_connected_layer_gpu(layer l, جال net);
خالی update_connected_layer_gpu(layer l, update_دلائل a);
خالی ڈالو_connected_layer(layer l);
خالی pull_connected_layer(layer l);
#اگر_ختم

#اگر_ختم

