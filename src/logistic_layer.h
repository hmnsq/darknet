#अगर_अघोषित LOGISTIC_LAYER_H
#घोषणा LOGISTIC_LAYER_H
#समावेश "layer.h"
#समावेश "network.h"

layer make_logistic_layer(पूर्णांक batch, पूर्णांक inputs);
व्योम forward_logistic_layer(स्थिर layer l, network net);
व्योम backward_logistic_layer(स्थिर layer l, network net);

#अगर_घोषित GPU
व्योम forward_logistic_layer_gpu(स्थिर layer l, network net);
व्योम backward_logistic_layer_gpu(स्थिर layer l, network net);
#पूर्ण_अगर

#पूर्ण_अगर
