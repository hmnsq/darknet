#यदि_अपरिभाषितम् CONNECTED_LAYER_H
#परिभाषय CONNECTED_LAYER_H

#अन्तर्भाव "activations.h"
#अन्तर्भाव "layer.h"
#अन्तर्भाव "network.h"

layer make_connected_layer(पूर्णाङ्क batch, पूर्णाङ्क inputs, पूर्णाङ्क outputs, ACTIVATION activation, पूर्णाङ्क batch_normalize, पूर्णाङ्क adam);

शून्य forward_connected_layer(layer l, जालम् net);
शून्य backward_connected_layer(layer l, जालम् net);
शून्य update_connected_layer(layer l, update_तर्काः a);

#यदि_परिभाषितम् GPU
शून्य forward_connected_layer_gpu(layer l, जालम् net);
शून्य backward_connected_layer_gpu(layer l, जालम् net);
शून्य update_connected_layer_gpu(layer l, update_तर्काः a);
शून्य प्रक्षिप_connected_layer(layer l);
शून्य pull_connected_layer(layer l);
#यदि_अन्तः

#यदि_अन्तः

