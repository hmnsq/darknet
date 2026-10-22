#ifndef SOFTMAX_LAYER_H
#define SOFTMAX_LAYER_H
#अन्तर्भाव "layer.h"
#अन्तर्भाव "network.h"

प्रकारनिर्देश layer softmax_layer;

शून्य softmax_array(भिन्न *आदान, पूर्णाङ्क n, भिन्न temp, भिन्न *output);
softmax_layer make_softmax_layer(पूर्णाङ्क batch, पूर्णाङ्क inputs, पूर्णाङ्क groups);
शून्य forward_softmax_layer(ध्रुव softmax_layer l, network net);
शून्य backward_softmax_layer(ध्रुव softmax_layer l, network net);

#ifdef GPU
शून्य pull_softmax_layer_output(ध्रुव softmax_layer l);
शून्य forward_softmax_layer_gpu(ध्रुव softmax_layer l, network net);
शून्य backward_softmax_layer_gpu(ध्रुव softmax_layer l, network net);
#endif

#endif
