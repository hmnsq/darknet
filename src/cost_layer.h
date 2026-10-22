#अगर_अघोषित COST_LAYER_H
#घोषणा COST_LAYER_H
#समावेश "layer.h"
#समावेश "network.h"

प्रकार layer cost_layer;

COST_TYPE get_cost_type(अक्षर *s);
अक्षर *get_cost_string(COST_TYPE a);
cost_layer make_cost_layer(पूर्णांक batch, पूर्णांक inputs, COST_TYPE type, भग्न scale);
व्योम forward_cost_layer(स्थिर cost_layer l, network net);
व्योम backward_cost_layer(स्थिर cost_layer l, network net);
व्योम resize_cost_layer(cost_layer *l, पूर्णांक inputs);

#अगर_घोषित GPU
व्योम forward_cost_layer_gpu(cost_layer l, network net);
व्योम backward_cost_layer_gpu(स्थिर cost_layer l, network net);
#पूर्ण_अगर

#पूर्ण_अगर
