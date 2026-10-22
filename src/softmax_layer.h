#अगर_अघोषित SOFTMAX_LAYER_H
#घोषणा SOFTMAX_LAYER_H
#समावेश "layer.h"
#समावेश "network.h"

प्रकार layer softmax_layer;

व्योम softmax_array(भग्न *निवेश, पूर्णांक n, भग्न अस्थायी, भग्न *output);
softmax_layer make_softmax_layer(पूर्णांक batch, पूर्णांक inputs, पूर्णांक groups);
व्योम forward_softmax_layer(स्थिर softmax_layer l, network net);
व्योम backward_softmax_layer(स्थिर softmax_layer l, network net);

#अगर_घोषित GPU
व्योम pull_softmax_layer_output(स्थिर softmax_layer l);
व्योम forward_softmax_layer_gpu(स्थिर softmax_layer l, network net);
व्योम backward_softmax_layer_gpu(स्थिर softmax_layer l, network net);
#पूर्ण_अगर

#पूर्ण_अगर
