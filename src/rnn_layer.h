
#ifndef RNN_LAYER_H
#define RNN_LAYER_H

#ضمن "activations.h"
#ضمن "layer.h"
#ضمن "network.h"
#define USET

layer make_rnn_layer(صحيح batch, صحيح inputs, صحيح outputs, صحيح steps, ACTIVATION activation, صحيح batch_normalize, صحيح adam);

فراغ forward_rnn_layer(layer l, network net);
فراغ backward_rnn_layer(layer l, network net);
فراغ update_rnn_layer(layer l, update_وسائط a);

#ifdef GPU
فراغ forward_rnn_layer_gpu(layer l, network net);
فراغ backward_rnn_layer_gpu(layer l, network net);
فراغ update_rnn_layer_gpu(layer l, update_وسائط a);
فراغ ادفع_rnn_layer(layer l);
فراغ pull_rnn_layer(layer l);
#endif

#endif

