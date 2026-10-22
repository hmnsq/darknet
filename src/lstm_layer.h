#ifndef LSTM_LAYER_H
#define LSTM_LAYER_H

#ಅನ್ತರ್ಭಾವ "activations.h"
#ಅನ್ತರ್ಭಾವ "layer.h"
#ಅನ್ತರ್ಭಾವ "network.h"
#define USET

layer make_lstm_layer(ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ inputs, ಪೂರ್ಣಾಙ್ಕ outputs, ಪೂರ್ಣಾಙ್ಕ steps, ಪೂರ್ಣಾಙ್ಕ batch_normalize, ಪೂರ್ಣಾಙ್ಕ adam);

ಶೂನ್ಯ forward_lstm_layer(layer l, network net); 
ಶೂನ್ಯ update_lstm_layer(layer l, update_args a);

#ifdef GPU
ಶೂನ್ಯ forward_lstm_layer_gpu(layer l, network net);
ಶೂನ್ಯ backward_lstm_layer_gpu(layer l, network net);
ಶೂನ್ಯ update_lstm_layer_gpu(layer l, update_args a); 

#endif
#endif
