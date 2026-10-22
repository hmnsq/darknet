#ifndef DECONVOLUTIONAL_LAYER_H
#define DECONVOLUTIONAL_LAYER_H

#अन्तर्भाव "cuda.h"
#अन्तर्भाव "image.h"
#अन्तर्भाव "activations.h"
#अन्तर्भाव "layer.h"
#अन्तर्भाव "network.h"

#ifdef GPU
शून्य forward_deconvolutional_layer_gpu(layer l, network net);
शून्य backward_deconvolutional_layer_gpu(layer l, network net);
शून्य update_deconvolutional_layer_gpu(layer l, update_args a);
शून्य प्रक्षिप_deconvolutional_layer(layer l);
शून्य pull_deconvolutional_layer(layer l);
#endif

layer make_deconvolutional_layer(पूर्णाङ्क batch, पूर्णाङ्क h, पूर्णाङ्क w, पूर्णाङ्क c, पूर्णाङ्क n, पूर्णाङ्क size, पूर्णाङ्क stride, पूर्णाङ्क padding, ACTIVATION activation, पूर्णाङ्क batch_normalize, पूर्णाङ्क adam);
शून्य resize_deconvolutional_layer(layer *l, पूर्णाङ्क h, पूर्णाङ्क w);
शून्य forward_deconvolutional_layer(ध्रुव layer l, network net);
शून्य update_deconvolutional_layer(layer l, update_args a);
शून्य backward_deconvolutional_layer(layer l, network net);

#endif

