
#ifndef GRU_LAYER_H
#define GRU_LAYER_H

#অন্তর্ভাব "activations.h"
#অন্তর্ভাব "layer.h"
#অন্তর্ভাব "network.h"

layer make_gru_layer(পূর্ণসংখ্যা batch, পূর্ণসংখ্যা inputs, পূর্ণসংখ্যা outputs, পূর্ণসংখ্যা steps, পূর্ণসংখ্যা batch_normalize, পূর্ণসংখ্যা adam);

শূন্য forward_gru_layer(layer l, network state);
শূন্য backward_gru_layer(layer l, network state);
শূন্য update_gru_layer(layer l, update_args a);

#ifdef GPU
শূন্য forward_gru_layer_gpu(layer l, network state);
শূন্য backward_gru_layer_gpu(layer l, network state);
শূন্য update_gru_layer_gpu(layer l, update_args a);
শূন্য ঠেলো_gru_layer(layer l);
শূন্য pull_gru_layer(layer l);
#endif

#endif

