
#अगर_अघोषित RNN_LAYER_H
#घोषणा RNN_LAYER_H

#समावेश "activations.h"
#समावेश "layer.h"
#समावेश "network.h"
#घोषणा USET

layer make_rnn_layer(पूर्णांक batch, पूर्णांक inputs, पूर्णांक outputs, पूर्णांक steps, ACTIVATION activation, पूर्णांक batch_normalize, पूर्णांक adam);

व्योम forward_rnn_layer(layer l, network net);
व्योम backward_rnn_layer(layer l, network net);
व्योम update_rnn_layer(layer l, update_args a);

#अगर_घोषित GPU
व्योम forward_rnn_layer_gpu(layer l, network net);
व्योम backward_rnn_layer_gpu(layer l, network net);
व्योम update_rnn_layer_gpu(layer l, update_args a);
व्योम धकेलो_rnn_layer(layer l);
व्योम pull_rnn_layer(layer l);
#पूर्ण_अगर

#पूर्ण_अगर

