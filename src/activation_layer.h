#ifndef ACTIVATION_LAYER_H
#define ACTIVATION_LAYER_H

#अन्तर्भाव "activations.h"
#अन्तर्भाव "layer.h"
#अन्तर्भाव "network.h"

layer make_activation_layer(पूर्णांक batch, पूर्णांक inputs, ACTIVATION activation);

शून्य forward_activation_layer(layer l, network net);
शून्य backward_activation_layer(layer l, network net);

#ifdef GPU
शून्य forward_activation_layer_gpu(layer l, network net);
शून्य backward_activation_layer_gpu(layer l, network net);
#endif

#endif

