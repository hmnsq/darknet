#यदि_अपरिभाषितम् YOLO_LAYER_H
#परिभाषय YOLO_LAYER_H

#अन्तर्भाव "darknet.h"
#अन्तर्भाव "layer.h"
#अन्तर्भाव "network.h"

layer make_yolo_layer(पूर्णाङ्क batch, पूर्णाङ्क w, पूर्णाङ्क h, पूर्णाङ्क n, पूर्णाङ्क योग_2, पूर्णाङ्क *आवरणम्, पूर्णाङ्क classes);
शून्य forward_yolo_layer(ध्रुव layer l, जालम् net);
शून्य backward_yolo_layer(ध्रुव layer l, जालम् net);
शून्य resize_yolo_layer(layer *l, पूर्णाङ्क w, पूर्णाङ्क h);
पूर्णाङ्क yolo_अङ्कः_detections(layer l, भिन्न thresh);

#यदि_परिभाषितम् GPU
शून्य forward_yolo_layer_gpu(ध्रुव layer l, जालम् net);
शून्य backward_yolo_layer_gpu(layer l, जालम् net);
#यदि_अन्तः

#यदि_अन्तः
