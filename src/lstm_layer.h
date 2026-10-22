#اگر_غیر_تعریف_شدہ LSTM_LAYER_H
#تعریف_کرو LSTM_LAYER_H

#شامل "activations.h"
#شامل "layer.h"
#شامل "network.h"
#تعریف_کرو USET

layer make_lstm_layer(صحیح batch, صحیح inputs, صحیح outputs, صحیح اقدام, صحیح batch_normalize, صحیح adam);

خالی forward_lstm_layer(layer l, جال net); 
خالی update_lstm_layer(layer l, update_دلائل a);

#اگر_تعریف_شدہ GPU
خالی forward_lstm_layer_gpu(layer l, جال net);
خالی backward_lstm_layer_gpu(layer l, جال net);
خالی update_lstm_layer_gpu(layer l, update_دلائل a); 

#اگر_ختم
#اگر_ختم
