#ifndef LSTM_LAYER_H
#define LSTM_LAYER_H

#অন্তর্ভাব "activations.h"
#অন্তর্ভাব "layer.h"
#অন্তর্ভাব "network.h"
#define USET

layer make_lstm_layer(পূর্ণাঙ্ক batch, পূর্ণাঙ্ক inputs, পূর্ণাঙ্ক outputs, পূর্ণাঙ্ক steps, পূর্ণাঙ্ক batch_normalize, পূর্ণাঙ্ক adam);

শূন্য forward_lstm_layer(layer l, network net); 
শূন্য update_lstm_layer(layer l, update_args a);

#ifdef GPU
শূন্য forward_lstm_layer_gpu(layer l, network net);
শূন্য backward_lstm_layer_gpu(layer l, network net);
শূন্য update_lstm_layer_gpu(layer l, update_args a); 

#endif
#endif
