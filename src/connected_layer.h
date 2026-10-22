#ifndef CONNECTED_LAYER_H
#define CONNECTED_LAYER_H

#অন্তর্ভাৱ "activations.h"
#অন্তর্ভাৱ "layer.h"
#অন্তর্ভাৱ "network.h"

layer make_connected_layer(পূর্ণাঙ্ক batch, পূর্ণাঙ্ক inputs, পূর্ণাঙ্ক outputs, ACTIVATION activation, পূর্ণাঙ্ক batch_normalize, পূর্ণাঙ্ক adam);

শূন্য forward_connected_layer(layer l, network net);
শূন্য backward_connected_layer(layer l, network net);
শূন্য update_connected_layer(layer l, update_args a);

#ifdef GPU
শূন্য forward_connected_layer_gpu(layer l, network net);
শূন্য backward_connected_layer_gpu(layer l, network net);
শূন্য update_connected_layer_gpu(layer l, update_args a);
শূন্য ঠেলো_connected_layer(layer l);
শূন্য pull_connected_layer(layer l);
#endif

#endif

