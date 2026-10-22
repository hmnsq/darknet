#यदि_अपरिभाषितम् BATCHNORM_LAYER_H
#परिभाषय BATCHNORM_LAYER_H

#अन्तर्भाव "image.h"
#अन्तर्भाव "layer.h"
#अन्तर्भाव "network.h"

layer make_batchnorm_layer(पूर्णाङ्क batch, पूर्णाङ्क w, पूर्णाङ्क h, पूर्णाङ्क c);
शून्य forward_batchnorm_layer(layer l, जालम् net);
शून्य backward_batchnorm_layer(layer l, जालम् net);

#यदि_परिभाषितम् GPU
शून्य forward_batchnorm_layer_gpu(layer l, जालम् net);
शून्य backward_batchnorm_layer_gpu(layer l, जालम् net);
शून्य pull_batchnorm_layer(layer l);
शून्य प्रक्षिप_batchnorm_layer(layer l);
#यदि_अन्तः

#यदि_अन्तः
