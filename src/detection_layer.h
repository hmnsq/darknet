#ifndef DETECTION_LAYER_H
#define DETECTION_LAYER_H

#अन्तर्भाव "layer.h"
#अन्तर्भाव "network.h"

प्रकारनिर्देश layer detection_layer;

detection_layer make_detection_layer(पूर्णाङ्क batch, पूर्णाङ्क inputs, पूर्णाङ्क n, पूर्णाङ्क size, पूर्णाङ्क classes, पूर्णाङ्क coords, पूर्णाङ्क rescore);
शून्य forward_detection_layer(अचर detection_layer l, network net);
शून्य backward_detection_layer(अचर detection_layer l, network net);

#ifdef GPU
शून्य forward_detection_layer_gpu(अचर detection_layer l, network net);
शून्य backward_detection_layer_gpu(detection_layer l, network net);
#endif

#endif
