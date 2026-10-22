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
शून्य ढकला_deconvolutional_layer(layer l);
शून्य pull_deconvolutional_layer(layer l);
#endif

layer make_deconvolutional_layer(पूर्णांक batch, पूर्णांक h, पूर्णांक w, पूर्णांक c, पूर्णांक n, पूर्णांक size, पूर्णांक stride, पूर्णांक padding, ACTIVATION activation, पूर्णांक batch_normalize, पूर्णांक adam);
शून्य resize_deconvolutional_layer(layer *l, पूर्णांक h, पूर्णांक w);
शून्य forward_deconvolutional_layer(स्थिरांक layer l, network net);
शून्य update_deconvolutional_layer(layer l, update_args a);
शून्य backward_deconvolutional_layer(layer l, network net);

#endif

