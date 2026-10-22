#جے_بنا_تعریف LSTM_LAYER_H
#تعریف_کرو LSTM_LAYER_H

#ضمن "activations.h"
#ضمن "layer.h"
#ضمن "network.h"
#تعریف_کرو USET

layer make_lstm_layer(صحيح batch, صحيح inputs, صحيح outputs, صحيح قدماں, صحيح batch_normalize, صحيح adam);

فراغ forward_lstm_layer(layer l, جال net); 
فراغ update_lstm_layer(layer l, update_دلیلاں a);

#جے_تعریف_شدہ GPU
فراغ forward_lstm_layer_gpu(layer l, جال net);
فراغ backward_lstm_layer_gpu(layer l, جال net);
فراغ update_lstm_layer_gpu(layer l, update_دلیلاں a); 

#جے_انت
#جے_انت
