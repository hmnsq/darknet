#अगर_अघोषित L2NORM_LAYER_H
#घोषणा L2NORM_LAYER_H
#समावेश "layer.h"
#समावेश "network.h"

layer make_l2norm_layer(पूर्णांक batch, पूर्णांक inputs);
व्योम forward_l2norm_layer(स्थिर layer l, network net);
व्योम backward_l2norm_layer(स्थिर layer l, network net);

#अगर_घोषित GPU
व्योम forward_l2norm_layer_gpu(स्थिर layer l, network net);
व्योम backward_l2norm_layer_gpu(स्थिर layer l, network net);
#पूर्ण_अगर

#पूर्ण_अगर
