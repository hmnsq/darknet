#अगर_अघोषित AVGPOOL_LAYER_H
#घोषणा AVGPOOL_LAYER_H

#समावेश "image.h"
#समावेश "cuda.h"
#समावेश "layer.h"
#समावेश "network.h"

प्रकार layer avgpool_layer;

image get_avgpool_image(avgpool_layer l);
avgpool_layer make_avgpool_layer(पूर्णांक batch, पूर्णांक w, पूर्णांक h, पूर्णांक c);
व्योम resize_avgpool_layer(avgpool_layer *l, पूर्णांक w, पूर्णांक h);
व्योम forward_avgpool_layer(स्थिर avgpool_layer l, network net);
व्योम backward_avgpool_layer(स्थिर avgpool_layer l, network net);

#अगर_घोषित GPU
व्योम forward_avgpool_layer_gpu(avgpool_layer l, network net);
व्योम backward_avgpool_layer_gpu(avgpool_layer l, network net);
#पूर्ण_अगर

#पूर्ण_अगर

