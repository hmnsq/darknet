#ifndef CONNECTED_LAYER_H
#define CONNECTED_LAYER_H

#अन्तर्भाव "activations.h"
#अन्तर्भाव "layer.h"
#अन्तर्भाव "network.h"

layer make_connected_layer(पूर्णांक batch, पूर्णांक inputs, पूर्णांक outputs, ACTIVATION activation, पूर्णांक batch_normalize, पूर्णांक adam);

शून्य forward_connected_layer(layer l, network net);
शून्य backward_connected_layer(layer l, network net);
शून्य update_connected_layer(layer l, update_args a);

#ifdef GPU
शून्य forward_connected_layer_gpu(layer l, network net);
शून्य backward_connected_layer_gpu(layer l, network net);
शून्य update_connected_layer_gpu(layer l, update_args a);
शून्य ढकला_connected_layer(layer l);
शून्य pull_connected_layer(layer l);
#endif

#endif

