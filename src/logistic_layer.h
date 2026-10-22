#यदि_अपरिभाषितम् LOGISTIC_LAYER_H
#परिभाषय LOGISTIC_LAYER_H
#अन्तर्भाव "layer.h"
#अन्तर्भाव "network.h"

layer make_logistic_layer(पूर्णाङ्क batch, पूर्णाङ्क inputs);
शून्य forward_logistic_layer(ध्रुव layer l, जालम् net);
शून्य backward_logistic_layer(ध्रुव layer l, जालम् net);

#यदि_परिभाषितम् GPU
शून्य forward_logistic_layer_gpu(ध्रुव layer l, जालम् net);
शून्य backward_logistic_layer_gpu(ध्रुव layer l, जालम् net);
#यदि_अन्तः

#यदि_अन्तः
