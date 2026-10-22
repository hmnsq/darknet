#अगर_अघोषित ISEG_LAYER_H
#घोषणा ISEG_LAYER_H

#समावेश "darknet.h"
#समावेश "layer.h"
#समावेश "network.h"

layer make_iseg_layer(पूर्णांक batch, पूर्णांक w, पूर्णांक h, पूर्णांक classes, पूर्णांक ids);
व्योम forward_iseg_layer(स्थिर layer l, network net);
व्योम backward_iseg_layer(स्थिर layer l, network net);
व्योम resize_iseg_layer(layer *l, पूर्णांक w, पूर्णांक h);
पूर्णांक iseg_अंक_detections(layer l, भग्न thresh);

#अगर_घोषित GPU
व्योम forward_iseg_layer_gpu(स्थिर layer l, network net);
व्योम backward_iseg_layer_gpu(layer l, network net);
#पूर्ण_अगर

#पूर्ण_अगर
