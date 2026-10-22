#अगर_अघोषित ACTIVATION_LAYER_H
#घोषणा ACTIVATION_LAYER_H

#समावेश "activations.h"
#समावेश "layer.h"
#समावेश "network.h"

layer make_activation_layer(पूर्णांक batch, पूर्णांक inputs, ACTIVATION activation);

व्योम forward_activation_layer(layer l, network net);
व्योम backward_activation_layer(layer l, network net);

#अगर_घोषित GPU
व्योम forward_activation_layer_gpu(layer l, network net);
व्योम backward_activation_layer_gpu(layer l, network net);
#पूर्ण_अगर

#पूर्ण_अगर

