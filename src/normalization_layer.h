#ifndef NORMALIZATION_LAYER_H
#define NORMALIZATION_LAYER_H

#अन्तर्भाव "image.h"
#अन्तर्भाव "layer.h"
#अन्तर्भाव "network.h"

layer make_normalization_layer(पूर्णांक batch, पूर्णांक w, पूर्णांक h, पूर्णांक c, पूर्णांक size, दशांश alpha, दशांश beta, दशांश kappa);
शून्य resize_normalization_layer(layer *layer, पूर्णांक h, पूर्णांक w);
शून्य forward_normalization_layer(स्थिरांक layer layer, network net);
शून्य backward_normalization_layer(स्थिरांक layer layer, network net);
शून्य visualize_normalization_layer(layer layer, अक्षर *window);

#ifdef GPU
शून्य forward_normalization_layer_gpu(स्थिरांक layer layer, network net);
शून्य backward_normalization_layer_gpu(स्थिरांक layer layer, network net);
#endif

#endif
