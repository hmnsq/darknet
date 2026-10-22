#यदि_अपरिभाषितम् L2NORM_LAYER_H
#परिभाषय L2NORM_LAYER_H
#अन्तर्भाव "layer.h"
#अन्तर्भाव "network.h"

layer make_l2norm_layer(पूर्णाङ्क batch, पूर्णाङ्क inputs);
शून्य forward_l2norm_layer(ध्रुव layer l, जालम् net);
शून्य backward_l2norm_layer(ध्रुव layer l, जालम् net);

#यदि_परिभाषितम् GPU
शून्य forward_l2norm_layer_gpu(ध्रुव layer l, जालम् net);
शून्य backward_l2norm_layer_gpu(ध्रुव layer l, जालम् net);
#यदि_अन्तः

#यदि_अन्तः
