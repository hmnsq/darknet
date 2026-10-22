#अगर_अघोषित LOCAL_LAYER_H
#घोषणा LOCAL_LAYER_H

#समावेश "cuda.h"
#समावेश "image.h"
#समावेश "activations.h"
#समावेश "layer.h"
#समावेश "network.h"

प्रकार layer local_layer;

#अगर_घोषित GPU
व्योम forward_local_layer_gpu(local_layer layer, network net);
व्योम backward_local_layer_gpu(local_layer layer, network net);
व्योम update_local_layer_gpu(local_layer layer, update_args a);

व्योम धकेलो_local_layer(local_layer layer);
व्योम pull_local_layer(local_layer layer);
#पूर्ण_अगर

local_layer make_local_layer(पूर्णांक batch, पूर्णांक h, पूर्णांक w, पूर्णांक c, पूर्णांक n, पूर्णांक आकार, पूर्णांक stride, पूर्णांक pad, ACTIVATION activation);

व्योम forward_local_layer(स्थिर local_layer layer, network net);
व्योम backward_local_layer(local_layer layer, network net);
व्योम update_local_layer(local_layer layer, update_args a);

व्योम bias_output(भग्न *output, भग्न *biases, पूर्णांक batch, पूर्णांक n, पूर्णांक आकार);
व्योम backward_bias(भग्न *bias_updates, भग्न *delta, पूर्णांक batch, पूर्णांक n, पूर्णांक आकार);

#पूर्ण_अगर

