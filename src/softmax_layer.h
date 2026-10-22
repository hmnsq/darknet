#ifndef SOFTMAX_LAYER_H
#define SOFTMAX_LAYER_H
#अन्तर्भाव "layer.h"
#अन्तर्भाव "network.h"

प्रकारव्याख्या layer softmax_layer;

शून्य softmax_array(दशांश *आदान, पूर्णांक n, दशांश temp, दशांश *output);
softmax_layer make_softmax_layer(पूर्णांक batch, पूर्णांक inputs, पूर्णांक groups);
शून्य forward_softmax_layer(स्थिरांक softmax_layer l, network net);
शून्य backward_softmax_layer(स्थिरांक softmax_layer l, network net);

#ifdef GPU
शून्य pull_softmax_layer_output(स्थिरांक softmax_layer l);
शून्य forward_softmax_layer_gpu(स्थिरांक softmax_layer l, network net);
शून्य backward_softmax_layer_gpu(स्थिरांक softmax_layer l, network net);
#endif

#endif
