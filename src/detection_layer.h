#ifndef DETECTION_LAYER_H
#define DETECTION_LAYER_H

#अन्तर्भाव "layer.h"
#अन्तर्भाव "network.h"

प्रकारव्याख्या layer detection_layer;

detection_layer make_detection_layer(पूर्णांक batch, पूर्णांक inputs, पूर्णांक n, पूर्णांक size, पूर्णांक classes, पूर्णांक coords, पूर्णांक rescore);
शून्य forward_detection_layer(स्थिरांक detection_layer l, network net);
शून्य backward_detection_layer(स्थिरांक detection_layer l, network net);

#ifdef GPU
शून्य forward_detection_layer_gpu(स्थिरांक detection_layer l, network net);
शून्य backward_detection_layer_gpu(detection_layer l, network net);
#endif

#endif
