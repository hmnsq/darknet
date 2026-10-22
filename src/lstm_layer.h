#ifndef LSTM_LAYER_H
#define LSTM_LAYER_H

#અન્તર્ભાવ "activations.h"
#અન્તર્ભાવ "layer.h"
#અન્તર્ભાવ "network.h"
#define USET

layer make_lstm_layer(પૂર્ણાંક batch, પૂર્ણાંક inputs, પૂર્ણાંક outputs, પૂર્ણાંક steps, પૂર્ણાંક batch_normalize, પૂર્ણાંક adam);

શૂન્ય forward_lstm_layer(layer l, network net); 
શૂન્ય update_lstm_layer(layer l, update_args a);

#ifdef GPU
શૂન્ય forward_lstm_layer_gpu(layer l, network net);
શૂન્ય backward_lstm_layer_gpu(layer l, network net);
શૂન્ય update_lstm_layer_gpu(layer l, update_args a); 

#endif
#endif
