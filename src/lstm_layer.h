#अगर_अघोषित LSTM_LAYER_H
#घोषणा LSTM_LAYER_H

#समावेश "activations.h"
#समावेश "layer.h"
#समावेश "network.h"
#घोषणा USET

layer make_lstm_layer(पूर्णांक batch, पूर्णांक inputs, पूर्णांक outputs, पूर्णांक steps, पूर्णांक batch_normalize, पूर्णांक adam);

व्योम forward_lstm_layer(layer l, network net); 
व्योम update_lstm_layer(layer l, update_args a);

#अगर_घोषित GPU
व्योम forward_lstm_layer_gpu(layer l, network net);
व्योम backward_lstm_layer_gpu(layer l, network net);
व्योम update_lstm_layer_gpu(layer l, update_args a); 

#पूर्ण_अगर
#पूर्ण_अगर
