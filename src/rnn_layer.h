
#ifndef RNN_LAYER_H
#define RNN_LAYER_H

#अन्तर्भाव "activations.h"
#अन्तर्भाव "layer.h"
#अन्तर्भाव "network.h"
#define USET

layer make_rnn_layer(पूर्णांक batch, पूर्णांक inputs, पूर्णांक outputs, पूर्णांक steps, ACTIVATION activation, पूर्णांक batch_normalize, पूर्णांक adam);

शून्य forward_rnn_layer(layer l, network net);
शून्य backward_rnn_layer(layer l, network net);
शून्य update_rnn_layer(layer l, update_args a);

#ifdef GPU
शून्य forward_rnn_layer_gpu(layer l, network net);
शून्य backward_rnn_layer_gpu(layer l, network net);
शून्य update_rnn_layer_gpu(layer l, update_args a);
शून्य ढकला_rnn_layer(layer l);
शून्य pull_rnn_layer(layer l);
#endif

#endif

