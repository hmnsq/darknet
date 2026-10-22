#अगर_अघोषित DETECTION_LAYER_H
#घोषणा DETECTION_LAYER_H

#समावेश "layer.h"
#समावेश "network.h"

प्रकार layer detection_layer;

detection_layer make_detection_layer(पूर्णांक batch, पूर्णांक inputs, पूर्णांक n, पूर्णांक आकार, पूर्णांक classes, पूर्णांक coords, पूर्णांक rescore);
व्योम forward_detection_layer(स्थिर detection_layer l, network net);
व्योम backward_detection_layer(स्थिर detection_layer l, network net);

#अगर_घोषित GPU
व्योम forward_detection_layer_gpu(स्थिर detection_layer l, network net);
व्योम backward_detection_layer_gpu(detection_layer l, network net);
#पूर्ण_अगर

#पूर्ण_अगर
