#ifndef YOLO_LAYER_H
#define YOLO_LAYER_H

#अन्तर्भाव "darknet.h"
#अन्तर्भाव "layer.h"
#अन्तर्भाव "network.h"

layer make_yolo_layer(पूर्णांक batch, पूर्णांक w, पूर्णांक h, पूर्णांक n, पूर्णांक total, पूर्णांक *mask, पूर्णांक classes);
शून्य forward_yolo_layer(स्थिरांक layer l, network net);
शून्य backward_yolo_layer(स्थिरांक layer l, network net);
शून्य resize_yolo_layer(layer *l, पूर्णांक w, पूर्णांक h);
पूर्णांक yolo_अंक_detections(layer l, दशांश thresh);

#ifdef GPU
शून्य forward_yolo_layer_gpu(स्थिरांक layer l, network net);
शून्य backward_yolo_layer_gpu(layer l, network net);
#endif

#endif
