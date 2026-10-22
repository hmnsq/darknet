
#اگر_غیر_تعریف_شدہ GRU_LAYER_H
#تعریف_کرو GRU_LAYER_H

#شامل "activations.h"
#شامل "layer.h"
#شامل "network.h"

layer make_gru_layer(صحیح batch, صحیح inputs, صحیح outputs, صحیح اقدام, صحیح batch_normalize, صحیح adam);

خالی forward_gru_layer(layer l, جال state);
خالی backward_gru_layer(layer l, جال state);
خالی update_gru_layer(layer l, update_دلائل a);

#اگر_تعریف_شدہ GPU
خالی forward_gru_layer_gpu(layer l, جال state);
خالی backward_gru_layer_gpu(layer l, جال state);
خالی update_gru_layer_gpu(layer l, update_دلائل a);
خالی ڈالو_gru_layer(layer l);
خالی pull_gru_layer(layer l);
#اگر_ختم

#اگر_ختم

