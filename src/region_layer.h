#अगर_अघोषित REGION_LAYER_H
#घोषणा REGION_LAYER_H

#समावेश "darknet.h"
#समावेश "layer.h"
#समावेश "network.h"

layer make_region_layer(पूर्णांक batch, पूर्णांक w, पूर्णांक h, पूर्णांक n, पूर्णांक classes, पूर्णांक coords);
व्योम forward_region_layer(स्थिर layer l, network net);
व्योम backward_region_layer(स्थिर layer l, network net);
व्योम resize_region_layer(layer *l, पूर्णांक w, पूर्णांक h);

#अगर_घोषित GPU
व्योम forward_region_layer_gpu(स्थिर layer l, network net);
व्योम backward_region_layer_gpu(layer l, network net);
#पूर्ण_अगर

#पूर्ण_अगर
