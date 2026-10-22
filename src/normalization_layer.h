#ifndef NORMALIZATION_LAYER_H
#define NORMALIZATION_LAYER_H

#अन्तर्भाव "image.h"
#अन्तर्भाव "layer.h"
#अन्तर्भाव "network.h"

layer make_normalization_layer(पूर्णाङ्क batch, पूर्णाङ्क w, पूर्णाङ्क h, पूर्णाङ्क c, पूर्णाङ्क size, भिन्न alpha, भिन्न beta, भिन्न kappa);
शून्य resize_normalization_layer(layer *layer, पूर्णाङ्क h, पूर्णाङ्क w);
शून्य forward_normalization_layer(ध्रुव layer layer, network net);
शून्य backward_normalization_layer(ध्रुव layer layer, network net);
शून्य visualize_normalization_layer(layer layer, वर्ण *window);

#ifdef GPU
शून्य forward_normalization_layer_gpu(ध्रुव layer layer, network net);
शून्य backward_normalization_layer_gpu(ध्रुव layer layer, network net);
#endif

#endif
