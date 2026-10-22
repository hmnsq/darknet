#ifndef LOGISTIC_LAYER_H
#define LOGISTIC_LAYER_H
#अन्तर्भाव "layer.h"
#अन्तर्भाव "network.h"

layer make_logistic_layer(पूर्णांक batch, पूर्णांक inputs);
शून्य forward_logistic_layer(स्थिरांक layer l, network net);
शून्य backward_logistic_layer(स्थिरांक layer l, network net);

#ifdef GPU
शून्य forward_logistic_layer_gpu(स्थिरांक layer l, network net);
शून्य backward_logistic_layer_gpu(स्थिरांक layer l, network net);
#endif

#endif
