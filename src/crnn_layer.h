
#ifndef CRNN_LAYER_H
#define CRNN_LAYER_H

#अन्तर्भाव "activations.h"
#अन्तर्भाव "layer.h"
#अन्तर्भाव "network.h"

layer make_crnn_layer(पूर्णांक batch, पूर्णांक h, पूर्णांक w, पूर्णांक c, पूर्णांक hidden_filters, पूर्णांक output_filters, पूर्णांक steps, ACTIVATION activation, पूर्णांक batch_normalize);

शून्य forward_crnn_layer(layer l, network net);
शून्य backward_crnn_layer(layer l, network net);
शून्य update_crnn_layer(layer l, update_args a);

#ifdef GPU
शून्य forward_crnn_layer_gpu(layer l, network net);
शून्य backward_crnn_layer_gpu(layer l, network net);
शून्य update_crnn_layer_gpu(layer l, update_args a);
शून्य ढकला_crnn_layer(layer l);
शून्य pull_crnn_layer(layer l);
#endif

#endif

