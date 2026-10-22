#ifndef LSTM_LAYER_H
#define LSTM_LAYER_H

#അന്തര്ഭാവ "activations.h"
#അന്തര്ഭാവ "layer.h"
#അന്തര്ഭാവ "network.h"
#define USET

layer make_lstm_layer(പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ inputs, പൂർണ്ണസംഖ്യ outputs, പൂർണ്ണസംഖ്യ steps, പൂർണ്ണസംഖ്യ batch_normalize, പൂർണ്ണസംഖ്യ adam);

ശൂന്യം forward_lstm_layer(layer l, network net); 
ശൂന്യം update_lstm_layer(layer l, update_args a);

#ifdef GPU
ശൂന്യം forward_lstm_layer_gpu(layer l, network net);
ശൂന്യം backward_lstm_layer_gpu(layer l, network net);
ശൂന്യം update_lstm_layer_gpu(layer l, update_args a); 

#endif
#endif
