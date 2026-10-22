#ifndef LSTM_LAYER_H
#define LSTM_LAYER_H

#अन्तर्भाव "activations.h"
#अन्तर्भाव "layer.h"
#अन्तर्भाव "network.h"
#define USET

layer make_lstm_layer(पूर्णाङ्क batch, पूर्णाङ्क inputs, पूर्णाङ्क outputs, पूर्णाङ्क steps, पूर्णाङ्क batch_normalize, पूर्णाङ्क adam);

शून्य forward_lstm_layer(layer l, network net); 
शून्य update_lstm_layer(layer l, update_args a);

#ifdef GPU
शून्य forward_lstm_layer_gpu(layer l, network net);
शून्य backward_lstm_layer_gpu(layer l, network net);
शून्य update_lstm_layer_gpu(layer l, update_args a); 

#endif
#endif
