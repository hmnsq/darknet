
#अगर_अघोषित CRNN_LAYER_H
#घोषणा CRNN_LAYER_H

#समावेश "activations.h"
#समावेश "layer.h"
#समावेश "network.h"

layer make_crnn_layer(पूर्णांक batch, पूर्णांक h, पूर्णांक w, पूर्णांक c, पूर्णांक hidden_filters, पूर्णांक output_filters, पूर्णांक steps, ACTIVATION activation, पूर्णांक batch_normalize);

व्योम forward_crnn_layer(layer l, network net);
व्योम backward_crnn_layer(layer l, network net);
व्योम update_crnn_layer(layer l, update_args a);

#अगर_घोषित GPU
व्योम forward_crnn_layer_gpu(layer l, network net);
व्योम backward_crnn_layer_gpu(layer l, network net);
व्योम update_crnn_layer_gpu(layer l, update_args a);
व्योम धकेलो_crnn_layer(layer l);
व्योम pull_crnn_layer(layer l);
#पूर्ण_अगर

#पूर्ण_अगर

