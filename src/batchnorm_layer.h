#अगर_अघोषित BATCHNORM_LAYER_H
#घोषणा BATCHNORM_LAYER_H

#समावेश "image.h"
#समावेश "layer.h"
#समावेश "network.h"

layer make_batchnorm_layer(पूर्णांक batch, पूर्णांक w, पूर्णांक h, पूर्णांक c);
व्योम forward_batchnorm_layer(layer l, network net);
व्योम backward_batchnorm_layer(layer l, network net);

#अगर_घोषित GPU
व्योम forward_batchnorm_layer_gpu(layer l, network net);
व्योम backward_batchnorm_layer_gpu(layer l, network net);
व्योम pull_batchnorm_layer(layer l);
व्योम धकेलो_batchnorm_layer(layer l);
#पूर्ण_अगर

#पूर्ण_अगर
