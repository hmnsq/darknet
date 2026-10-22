#यदि_अपरिभाषितम् DETECTION_LAYER_H
#परिभाषय DETECTION_LAYER_H

#अन्तर्भाव "layer.h"
#अन्तर्भाव "network.h"

प्रकारनिर्देश layer detection_layer;

detection_layer make_detection_layer(पूर्णाङ्क batch, पूर्णाङ्क inputs, पूर्णाङ्क n, पूर्णाङ्क आकार, पूर्णाङ्क classes, पूर्णाङ्क coords, पूर्णाङ्क rescore);
शून्य forward_detection_layer(ध्रुव detection_layer l, जालम् net);
शून्य backward_detection_layer(ध्रुव detection_layer l, जालम् net);

#यदि_परिभाषितम् GPU
शून्य forward_detection_layer_gpu(ध्रुव detection_layer l, जालम् net);
शून्य backward_detection_layer_gpu(detection_layer l, जालम् net);
#यदि_अन्तः

#यदि_अन्तः
