
#ifndef CRNN_LAYER_H
#define CRNN_LAYER_H

#অন্তর্ভাব "activations.h"
#অন্তর্ভাব "layer.h"
#অন্তর্ভাব "network.h"

layer make_crnn_layer(পূর্ণসংখ্যা batch, পূর্ণসংখ্যা h, পূর্ণসংখ্যা w, পূর্ণসংখ্যা c, পূর্ণসংখ্যা hidden_filters, পূর্ণসংখ্যা output_filters, পূর্ণসংখ্যা steps, ACTIVATION activation, পূর্ণসংখ্যা batch_normalize);

শূন্য forward_crnn_layer(layer l, network net);
শূন্য backward_crnn_layer(layer l, network net);
শূন্য update_crnn_layer(layer l, update_args a);

#ifdef GPU
শূন্য forward_crnn_layer_gpu(layer l, network net);
শূন্য backward_crnn_layer_gpu(layer l, network net);
শূন্য update_crnn_layer_gpu(layer l, update_args a);
শূন্য ঠেলো_crnn_layer(layer l);
শূন্য pull_crnn_layer(layer l);
#endif

#endif

