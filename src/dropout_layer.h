#अगर_अघोषित DROPOUT_LAYER_H
#घोषणा DROPOUT_LAYER_H

#समावेश "layer.h"
#समावेश "network.h"

प्रकार layer dropout_layer;

dropout_layer make_dropout_layer(पूर्णांक batch, पूर्णांक inputs, भग्न probability);

व्योम forward_dropout_layer(dropout_layer l, network net);
व्योम backward_dropout_layer(dropout_layer l, network net);
व्योम resize_dropout_layer(dropout_layer *l, पूर्णांक inputs);

#अगर_घोषित GPU
व्योम forward_dropout_layer_gpu(dropout_layer l, network net);
व्योम backward_dropout_layer_gpu(dropout_layer l, network net);

#पूर्ण_अगर
#पूर्ण_अगर
