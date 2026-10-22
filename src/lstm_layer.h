#که_نه_تعریف_شوی LSTM_LAYER_H
#تعریف_کړه LSTM_LAYER_H

#ضمن "activations.h"
#ضمن "layer.h"
#ضمن "network.h"
#تعریف_کړه USET

layer make_lstm_layer(صحيح batch, صحيح inputs, صحيح outputs, صحيح ګامونه, صحيح batch_normalize, صحيح adam);

فراغ forward_lstm_layer(layer l, شبکه net); 
فراغ update_lstm_layer(layer l, update_دلیلونه a);

#که_تعریف_شوی GPU
فراغ forward_lstm_layer_gpu(layer l, شبکه net);
فراغ backward_lstm_layer_gpu(layer l, شبکه net);
فراغ update_lstm_layer_gpu(layer l, update_دلیلونه a); 

#د_که_پای
#د_که_پای
