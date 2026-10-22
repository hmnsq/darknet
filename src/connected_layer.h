#अगर_अघोषित CONNECTED_LAYER_H
#घोषणा CONNECTED_LAYER_H

#समावेश "activations.h"
#समावेश "layer.h"
#समावेश "network.h"

layer make_connected_layer(पूर्णांक batch, पूर्णांक inputs, पूर्णांक outputs, ACTIVATION activation, पूर्णांक batch_normalize, पूर्णांक adam);

व्योम forward_connected_layer(layer l, network net);
व्योम backward_connected_layer(layer l, network net);
व्योम update_connected_layer(layer l, update_args a);

#अगर_घोषित GPU
व्योम forward_connected_layer_gpu(layer l, network net);
व्योम backward_connected_layer_gpu(layer l, network net);
व्योम update_connected_layer_gpu(layer l, update_args a);
व्योम धकेलो_connected_layer(layer l);
व्योम pull_connected_layer(layer l);
#पूर्ण_अगर

#पूर्ण_अगर

