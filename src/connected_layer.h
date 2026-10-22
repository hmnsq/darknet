#ifndef CONNECTED_LAYER_H
#define CONNECTED_LAYER_H

#অন্তর্ভাব "activations.h"
#অন্তর্ভাব "layer.h"
#অন্তর্ভাব "network.h"

layer make_connected_layer(পূর্ণসংখ্যা batch, পূর্ণসংখ্যা inputs, পূর্ণসংখ্যা outputs, ACTIVATION activation, পূর্ণসংখ্যা batch_normalize, পূর্ণসংখ্যা adam);

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

