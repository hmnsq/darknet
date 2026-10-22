#ifndef COST_LAYER_H
#define COST_LAYER_H
#अन्तर्भाव "layer.h"
#अन्तर्भाव "network.h"

प्रकारव्याख्या layer cost_layer;

COST_TYPE get_cost_type(अक्षर *s);
अक्षर *get_cost_string(COST_TYPE a);
cost_layer make_cost_layer(पूर्णांक batch, पूर्णांक inputs, COST_TYPE type, दशांश scale);
शून्य forward_cost_layer(स्थिरांक cost_layer l, network net);
शून्य backward_cost_layer(स्थिरांक cost_layer l, network net);
शून्य resize_cost_layer(cost_layer *l, पूर्णांक inputs);

#ifdef GPU
शून्य forward_cost_layer_gpu(cost_layer l, network net);
शून्य backward_cost_layer_gpu(स्थिरांक cost_layer l, network net);
#endif

#endif
