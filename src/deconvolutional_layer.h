#अगर_अघोषित DECONVOLUTIONAL_LAYER_H
#घोषणा DECONVOLUTIONAL_LAYER_H

#समावेश "cuda.h"
#समावेश "image.h"
#समावेश "activations.h"
#समावेश "layer.h"
#समावेश "network.h"

#अगर_घोषित GPU
व्योम forward_deconvolutional_layer_gpu(layer l, network net);
व्योम backward_deconvolutional_layer_gpu(layer l, network net);
व्योम update_deconvolutional_layer_gpu(layer l, update_args a);
व्योम धकेलो_deconvolutional_layer(layer l);
व्योम pull_deconvolutional_layer(layer l);
#पूर्ण_अगर

layer make_deconvolutional_layer(पूर्णांक batch, पूर्णांक h, पूर्णांक w, पूर्णांक c, पूर्णांक n, पूर्णांक आकार, पूर्णांक stride, पूर्णांक padding, ACTIVATION activation, पूर्णांक batch_normalize, पूर्णांक adam);
व्योम resize_deconvolutional_layer(layer *l, पूर्णांक h, पूर्णांक w);
व्योम forward_deconvolutional_layer(स्थिर layer l, network net);
व्योम update_deconvolutional_layer(layer l, update_args a);
व्योम backward_deconvolutional_layer(layer l, network net);

#पूर्ण_अगर

