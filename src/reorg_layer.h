#अगर_अघोषित REORG_LAYER_H
#घोषणा REORG_LAYER_H

#समावेश "image.h"
#समावेश "cuda.h"
#समावेश "layer.h"
#समावेश "network.h"

layer make_reorg_layer(पूर्णांक batch, पूर्णांक w, पूर्णांक h, पूर्णांक c, पूर्णांक stride, पूर्णांक reverse, पूर्णांक flatten, पूर्णांक extra);
व्योम resize_reorg_layer(layer *l, पूर्णांक w, पूर्णांक h);
व्योम forward_reorg_layer(स्थिर layer l, network net);
व्योम backward_reorg_layer(स्थिर layer l, network net);

#अगर_घोषित GPU
व्योम forward_reorg_layer_gpu(layer l, network net);
व्योम backward_reorg_layer_gpu(layer l, network net);
#पूर्ण_अगर

#पूर्ण_अगर

