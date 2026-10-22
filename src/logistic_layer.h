#ifndef LOGISTIC_LAYER_H
#define LOGISTIC_LAYER_H
#अन्तर्भाव "layer.h"
#अन्तर्भाव "network.h"

layer make_logistic_layer(पूर्णाङ्क batch, पूर्णाङ्क inputs);
शून्य forward_logistic_layer(अचर layer l, network net);
शून्य backward_logistic_layer(अचर layer l, network net);

#ifdef GPU
शून्य forward_logistic_layer_gpu(अचर layer l, network net);
शून्य backward_logistic_layer_gpu(अचर layer l, network net);
#endif

#endif
