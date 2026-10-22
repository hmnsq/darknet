#ifndef LSTM_LAYER_H
#define LSTM_LAYER_H

#ଅନ୍ତର୍ଭାଵ "activations.h"
#ଅନ୍ତର୍ଭାଵ "layer.h"
#ଅନ୍ତର୍ଭାଵ "network.h"
#define USET

layer make_lstm_layer(ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା inputs, ପୂର୍ଣ୍ଣସଂଖ୍ୟା outputs, ପୂର୍ଣ୍ଣସଂଖ୍ୟା steps, ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch_normalize, ପୂର୍ଣ୍ଣସଂଖ୍ୟା adam);

ଶୂନ୍ୟ forward_lstm_layer(layer l, network net); 
ଶୂନ୍ୟ update_lstm_layer(layer l, update_args a);

#ifdef GPU
ଶୂନ୍ୟ forward_lstm_layer_gpu(layer l, network net);
ଶୂନ୍ୟ backward_lstm_layer_gpu(layer l, network net);
ଶୂନ୍ୟ update_lstm_layer_gpu(layer l, update_args a); 

#endif
#endif
