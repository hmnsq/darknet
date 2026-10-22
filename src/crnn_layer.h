
#ifndef CRNN_LAYER_H
#define CRNN_LAYER_H

#अन्तर्भाव "activations.h"
#अन्तर्भाव "layer.h"
#अन्तर्भाव "network.h"

layer make_crnn_layer(पूर्णाङ्क batch, पूर्णाङ्क h, पूर्णाङ्क w, पूर्णाङ्क c, पूर्णाङ्क hidden_filters, पूर्णाङ्क output_filters, पूर्णाङ्क steps, ACTIVATION activation, पूर्णाङ्क batch_normalize);

शून्य forward_crnn_layer(layer l, network net);
शून्य backward_crnn_layer(layer l, network net);
शून्य update_crnn_layer(layer l, update_args a);

#ifdef GPU
शून्य forward_crnn_layer_gpu(layer l, network net);
शून्य backward_crnn_layer_gpu(layer l, network net);
शून्य update_crnn_layer_gpu(layer l, update_args a);
शून्य धकेलो_crnn_layer(layer l);
शून्य pull_crnn_layer(layer l);
#endif

#endif

