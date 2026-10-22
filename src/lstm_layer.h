#جيڪڏهن_وصف_نه_ٿيل LSTM_LAYER_H
#وصف_ڪريو LSTM_LAYER_H

#ضمن "activations.h"
#ضمن "layer.h"
#ضمن "network.h"
#وصف_ڪريو USET

layer make_lstm_layer(صحيح batch, صحيح inputs, صحيح outputs, صحيح قدمن, صحيح batch_normalize, صحيح adam);

فراغ forward_lstm_layer(layer l, ڄار net); 
فراغ update_lstm_layer(layer l, update_دليلون a);

#جيڪڏهن_وصف_ٿيل GPU
فراغ forward_lstm_layer_gpu(layer l, ڄار net);
فراغ backward_lstm_layer_gpu(layer l, ڄار net);
فراغ update_lstm_layer_gpu(layer l, update_دليلون a); 

#جيڪڏهن_پورو
#جيڪڏهن_پورو
