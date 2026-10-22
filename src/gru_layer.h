
#अगर_अघोषित GRU_LAYER_H
#घोषणा GRU_LAYER_H

#समावेश "activations.h"
#समावेश "layer.h"
#समावेश "network.h"

layer make_gru_layer(पूर्णांक batch, पूर्णांक inputs, पूर्णांक outputs, पूर्णांक steps, पूर्णांक batch_normalize, पूर्णांक adam);

व्योम forward_gru_layer(layer l, network state);
व्योम backward_gru_layer(layer l, network state);
व्योम update_gru_layer(layer l, update_args a);

#अगर_घोषित GPU
व्योम forward_gru_layer_gpu(layer l, network state);
व्योम backward_gru_layer_gpu(layer l, network state);
व्योम update_gru_layer_gpu(layer l, update_args a);
व्योम धकेलो_gru_layer(layer l);
व्योम pull_gru_layer(layer l);
#पूर्ण_अगर

#पूर्ण_अगर

