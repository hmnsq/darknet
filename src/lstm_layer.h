#ifndef LSTM_LAYER_H
#define LSTM_LAYER_H

#ضمن "activations.h"
#ضمن "layer.h"
#ضمن "network.h"
#define USET

layer make_lstm_layer(صحيح batch, صحيح inputs, صحيح outputs, صحيح steps, صحيح batch_normalize, صحيح adam);

فراغ forward_lstm_layer(layer l, network net); 
فراغ update_lstm_layer(layer l, update_وسائط a);

#ifdef GPU
فراغ forward_lstm_layer_gpu(layer l, network net);
فراغ backward_lstm_layer_gpu(layer l, network net);
فراغ update_lstm_layer_gpu(layer l, update_وسائط a); 

#endif
#endif
