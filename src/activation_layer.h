#ifndef ACTIVATION_LAYER_H
#define ACTIVATION_LAYER_H

#অন্তর্ভাৱ "activations.h"
#অন্তর্ভাৱ "layer.h"
#অন্তর্ভাৱ "network.h"

layer make_activation_layer(পূর্ণাঙ্ক batch, পূর্ণাঙ্ক inputs, ACTIVATION activation);

শূন্য forward_activation_layer(layer l, network net);
শূন্য backward_activation_layer(layer l, network net);

#ifdef GPU
শূন্য forward_activation_layer_gpu(layer l, network net);
শূন্য backward_activation_layer_gpu(layer l, network net);
#endif

#endif

