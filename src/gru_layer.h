
#ifndef GRU_LAYER_H
#define GRU_LAYER_H

#अन्तर्भाव "activations.h"
#अन्तर्भाव "layer.h"
#अन्तर्भाव "network.h"

layer make_gru_layer(पूर्णाङ्क batch, पूर्णाङ्क inputs, पूर्णाङ्क outputs, पूर्णाङ्क steps, पूर्णाङ्क batch_normalize, पूर्णाङ्क adam);

शून्य forward_gru_layer(layer l, network state);
शून्य backward_gru_layer(layer l, network state);
शून्य update_gru_layer(layer l, update_args a);

#ifdef GPU
शून्य forward_gru_layer_gpu(layer l, network state);
शून्य backward_gru_layer_gpu(layer l, network state);
शून्य update_gru_layer_gpu(layer l, update_args a);
शून्य ढकला_gru_layer(layer l);
शून्य pull_gru_layer(layer l);
#endif

#endif

