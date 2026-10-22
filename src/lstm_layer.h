#اگر_تعریف_نشده LSTM_LAYER_H
#تعریف_کن LSTM_LAYER_H

#ضمن "activations.h"
#ضمن "layer.h"
#ضمن "network.h"
#تعریف_کن USET

layer make_lstm_layer(صحيح batch, صحيح inputs, صحيح outputs, صحيح گامها, صحيح batch_normalize, صحيح adam);

فراغ forward_lstm_layer(layer l, شبکه net); 
فراغ update_lstm_layer(layer l, update_آرگومانها a);

#اگر_تعریف_شده GPU
فراغ forward_lstm_layer_gpu(layer l, شبکه net);
فراغ backward_lstm_layer_gpu(layer l, شبکه net);
فراغ update_lstm_layer_gpu(layer l, update_آرگومانها a); 

#پایان_اگر
#پایان_اگر
