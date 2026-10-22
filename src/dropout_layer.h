#यदि_अपरिभाषितम् DROPOUT_LAYER_H
#परिभाषय DROPOUT_LAYER_H

#अन्तर्भाव "layer.h"
#अन्तर्भाव "network.h"

प्रकारनिर्देश layer dropout_layer;

dropout_layer make_dropout_layer(पूर्णाङ्क batch, पूर्णाङ्क inputs, भिन्न probability);

शून्य forward_dropout_layer(dropout_layer l, जालम् net);
शून्य backward_dropout_layer(dropout_layer l, जालम् net);
शून्य resize_dropout_layer(dropout_layer *l, पूर्णाङ्क inputs);

#यदि_परिभाषितम् GPU
शून्य forward_dropout_layer_gpu(dropout_layer l, जालम् net);
शून्य backward_dropout_layer_gpu(dropout_layer l, जालम् net);

#यदि_अन्तः
#यदि_अन्तः
