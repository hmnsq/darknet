#अगर_अघोषित YOLO_LAYER_H
#घोषणा YOLO_LAYER_H

#समावेश "darknet.h"
#समावेश "layer.h"
#समावेश "network.h"

layer make_yolo_layer(पूर्णांक batch, पूर्णांक w, पूर्णांक h, पूर्णांक n, पूर्णांक योग_2, पूर्णांक *mask, पूर्णांक classes);
व्योम forward_yolo_layer(स्थिर layer l, network net);
व्योम backward_yolo_layer(स्थिर layer l, network net);
व्योम resize_yolo_layer(layer *l, पूर्णांक w, पूर्णांक h);
पूर्णांक yolo_अंक_detections(layer l, भग्न thresh);

#अगर_घोषित GPU
व्योम forward_yolo_layer_gpu(स्थिर layer l, network net);
व्योम backward_yolo_layer_gpu(layer l, network net);
#पूर्ण_अगर

#पूर्ण_अगर
