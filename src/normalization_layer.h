#अगर_अघोषित NORMALIZATION_LAYER_H
#घोषणा NORMALIZATION_LAYER_H

#समावेश "image.h"
#समावेश "layer.h"
#समावेश "network.h"

layer make_normalization_layer(पूर्णांक batch, पूर्णांक w, पूर्णांक h, पूर्णांक c, पूर्णांक आकार, भग्न alpha, भग्न beta, भग्न kappa);
व्योम resize_normalization_layer(layer *layer, पूर्णांक h, पूर्णांक w);
व्योम forward_normalization_layer(स्थिर layer layer, network net);
व्योम backward_normalization_layer(स्थिर layer layer, network net);
व्योम visualize_normalization_layer(layer layer, अक्षर *window);

#अगर_घोषित GPU
व्योम forward_normalization_layer_gpu(स्थिर layer layer, network net);
व्योम backward_normalization_layer_gpu(स्थिर layer layer, network net);
#पूर्ण_अगर

#पूर्ण_अगर
