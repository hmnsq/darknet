#यदि_अपरिभाषितम् SOFTMAX_LAYER_H
#परिभाषय SOFTMAX_LAYER_H
#अन्तर्भाव "layer.h"
#अन्तर्भाव "network.h"

प्रकारनिर्देश layer softmax_layer;

शून्य softmax_array(भिन्न *निवेशः, पूर्णाङ्क n, भिन्न अस्थायी, भिन्न *output);
softmax_layer make_softmax_layer(पूर्णाङ्क batch, पूर्णाङ्क inputs, पूर्णाङ्क groups);
शून्य forward_softmax_layer(ध्रुव softmax_layer l, जालम् net);
शून्य backward_softmax_layer(ध्रुव softmax_layer l, जालम् net);

#यदि_परिभाषितम् GPU
शून्य pull_softmax_layer_output(ध्रुव softmax_layer l);
शून्य forward_softmax_layer_gpu(ध्रुव softmax_layer l, जालम् net);
शून्य backward_softmax_layer_gpu(ध्रुव softmax_layer l, जालम् net);
#यदि_अन्तः

#यदि_अन्तः
