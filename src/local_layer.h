#ifndef LOCAL_LAYER_H
#define LOCAL_LAYER_H

#अन्तर्भाव "cuda.h"
#अन्तर्भाव "image.h"
#अन्तर्भाव "activations.h"
#अन्तर्भाव "layer.h"
#अन्तर्भाव "network.h"

प्रकारनिर्देश layer local_layer;

#ifdef GPU
शून्य forward_local_layer_gpu(local_layer layer, network net);
शून्य backward_local_layer_gpu(local_layer layer, network net);
शून्य update_local_layer_gpu(local_layer layer, update_args a);

शून्य ढकला_local_layer(local_layer layer);
शून्य pull_local_layer(local_layer layer);
#endif

local_layer make_local_layer(पूर्णाङ्क batch, पूर्णाङ्क h, पूर्णाङ्क w, पूर्णाङ्क c, पूर्णाङ्क n, पूर्णाङ्क size, पूर्णाङ्क stride, पूर्णाङ्क pad, ACTIVATION activation);

शून्य forward_local_layer(ध्रुव local_layer layer, network net);
शून्य backward_local_layer(local_layer layer, network net);
शून्य update_local_layer(local_layer layer, update_args a);

शून्य bias_output(भिन्न *output, भिन्न *biases, पूर्णाङ्क batch, पूर्णाङ्क n, पूर्णाङ्क size);
शून्य backward_bias(भिन्न *bias_updates, भिन्न *delta, पूर्णाङ्क batch, पूर्णाङ्क n, पूर्णाङ्क size);

#endif

