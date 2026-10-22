
#ifndef CRNN_LAYER_H
#define CRNN_LAYER_H

#অন্তর্ভাৱ "activations.h"
#অন্তর্ভাৱ "layer.h"
#অন্তর্ভাৱ "network.h"

layer make_crnn_layer(পূর্ণাঙ্ক batch, পূর্ণাঙ্ক h, পূর্ণাঙ্ক w, পূর্ণাঙ্ক c, পূর্ণাঙ্ক hidden_filters, পূর্ণাঙ্ক output_filters, পূর্ণাঙ্ক steps, ACTIVATION activation, পূর্ণাঙ্ক batch_normalize);

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

