#ifndef LSTM_LAYER_H
#define LSTM_LAYER_H

#شامل "activations.h"
#شامل "layer.h"
#شامل "network.h"
#define USET

layer make_lstm_layer(صحیح batch, صحیح inputs, صحیح outputs, صحیح steps, صحیح batch_normalize, صحیح adam);

تهی forward_lstm_layer(layer l, network net); 
تهی update_lstm_layer(layer l, update_args a);

#ifdef GPU
تهی forward_lstm_layer_gpu(layer l, network net);
تهی backward_lstm_layer_gpu(layer l, network net);
تهی update_lstm_layer_gpu(layer l, update_args a); 

#endif
#endif
