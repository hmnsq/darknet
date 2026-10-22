#यदि_अपरिभाषितम् COST_LAYER_H
#परिभाषय COST_LAYER_H
#अन्तर्भाव "layer.h"
#अन्तर्भाव "network.h"

प्रकारनिर्देश layer cost_layer;

COST_TYPE get_cost_type(वर्ण *s);
वर्ण *get_cost_string(COST_TYPE a);
cost_layer make_cost_layer(पूर्णाङ्क batch, पूर्णाङ्क inputs, COST_TYPE type, भिन्न scale);
शून्य forward_cost_layer(ध्रुव cost_layer l, जालम् net);
शून्य backward_cost_layer(ध्रुव cost_layer l, जालम् net);
शून्य resize_cost_layer(cost_layer *l, पूर्णाङ्क inputs);

#यदि_परिभाषितम् GPU
शून्य forward_cost_layer_gpu(cost_layer l, जालम् net);
शून्य backward_cost_layer_gpu(ध्रुव cost_layer l, जालम् net);
#यदि_अन्तः

#यदि_अन्तः
