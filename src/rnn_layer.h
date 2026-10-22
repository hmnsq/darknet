
#ifndef RNN_LAYER_H
#define RNN_LAYER_H

#অন্তর্ভাব "activations.h"
#অন্তর্ভাব "layer.h"
#অন্তর্ভাব "network.h"
#define USET

layer make_rnn_layer(পূর্ণাঙ্ক batch, পূর্ণাঙ্ক inputs, পূর্ণাঙ্ক outputs, পূর্ণাঙ্ক steps, ACTIVATION activation, পূর্ণাঙ্ক batch_normalize, পূর্ণাঙ্ক adam);

শূন্য forward_rnn_layer(layer l, network net);
শূন্য backward_rnn_layer(layer l, network net);
শূন্য update_rnn_layer(layer l, update_args a);

#ifdef GPU
শূন্য forward_rnn_layer_gpu(layer l, network net);
শূন্য backward_rnn_layer_gpu(layer l, network net);
শূন্য update_rnn_layer_gpu(layer l, update_args a);
শূন্য ঠেলো_rnn_layer(layer l);
শূন্য pull_rnn_layer(layer l);
#endif

#endif

