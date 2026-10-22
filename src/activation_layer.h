#यदि_अपरिभाषितम् ACTIVATION_LAYER_H
#परिभाषय ACTIVATION_LAYER_H

#अन्तर्भाव "activations.h"
#अन्तर्भाव "layer.h"
#अन्तर्भाव "network.h"

layer make_activation_layer(पूर्णाङ्क batch, पूर्णाङ्क inputs, ACTIVATION activation);

शून्य forward_activation_layer(layer l, जालम् net);
शून्य backward_activation_layer(layer l, जालम् net);

#यदि_परिभाषितम् GPU
शून्य forward_activation_layer_gpu(layer l, जालम् net);
शून्य backward_activation_layer_gpu(layer l, जालम् net);
#यदि_अन्तः

#यदि_अन्तः

