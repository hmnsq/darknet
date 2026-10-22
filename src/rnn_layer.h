
#اگر_غیر_تعریف_شدہ RNN_LAYER_H
#تعریف_کرو RNN_LAYER_H

#شامل "activations.h"
#شامل "layer.h"
#شامل "network.h"
#تعریف_کرو USET

layer make_rnn_layer(صحیح batch, صحیح inputs, صحیح outputs, صحیح اقدام, ACTIVATION activation, صحیح batch_normalize, صحیح adam);

خالی forward_rnn_layer(layer l, جال net);
خالی backward_rnn_layer(layer l, جال net);
خالی update_rnn_layer(layer l, update_دلائل a);

#اگر_تعریف_شدہ GPU
خالی forward_rnn_layer_gpu(layer l, جال net);
خالی backward_rnn_layer_gpu(layer l, جال net);
خالی update_rnn_layer_gpu(layer l, update_دلائل a);
خالی ڈالو_rnn_layer(layer l);
خالی pull_rnn_layer(layer l);
#اگر_ختم

#اگر_ختم

