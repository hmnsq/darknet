#अगर_अघोषित MAXPOOL_LAYER_H
#घोषणा MAXPOOL_LAYER_H

#समावेश "image.h"
#समावेश "cuda.h"
#समावेश "layer.h"
#समावेश "network.h"

प्रकार layer maxpool_layer;

image get_maxpool_image(maxpool_layer l);
maxpool_layer make_maxpool_layer(पूर्णांक batch, पूर्णांक h, पूर्णांक w, पूर्णांक c, पूर्णांक आकार, पूर्णांक stride, पूर्णांक padding);
व्योम resize_maxpool_layer(maxpool_layer *l, पूर्णांक w, पूर्णांक h);
व्योम forward_maxpool_layer(स्थिर maxpool_layer l, network net);
व्योम backward_maxpool_layer(स्थिर maxpool_layer l, network net);

#अगर_घोषित GPU
व्योम forward_maxpool_layer_gpu(maxpool_layer l, network net);
व्योम backward_maxpool_layer_gpu(maxpool_layer l, network net);
#पूर्ण_अगर

#पूर्ण_अगर

