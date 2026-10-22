
#ifndef RNN_LAYER_H
#define RNN_LAYER_H

#ଅନ୍ତର୍ଭାଵ "activations.h"
#ଅନ୍ତର୍ଭାଵ "layer.h"
#ଅନ୍ତର୍ଭାଵ "network.h"
#define USET

layer make_rnn_layer(ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା inputs, ପୂର୍ଣ୍ଣସଂଖ୍ୟା outputs, ପୂର୍ଣ୍ଣସଂଖ୍ୟା steps, ACTIVATION activation, ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch_normalize, ପୂର୍ଣ୍ଣସଂଖ୍ୟା adam);

ଶୂନ୍ୟ forward_rnn_layer(layer l, network net);
ଶୂନ୍ୟ backward_rnn_layer(layer l, network net);
ଶୂନ୍ୟ update_rnn_layer(layer l, update_args a);

#ifdef GPU
ଶୂନ୍ୟ forward_rnn_layer_gpu(layer l, network net);
ଶୂନ୍ୟ backward_rnn_layer_gpu(layer l, network net);
ଶୂନ୍ୟ update_rnn_layer_gpu(layer l, update_args a);
ଶୂନ୍ୟ ଠେଲ_rnn_layer(layer l);
ଶୂନ୍ୟ pull_rnn_layer(layer l);
#endif

#endif

