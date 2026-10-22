
#ifndef RNN_LAYER_H
#define RNN_LAYER_H

#شامل "activations.h"
#شامل "layer.h"
#شامل "network.h"
#define USET

layer make_rnn_layer(صحیح batch, صحیح inputs, صحیح outputs, صحیح steps, ACTIVATION activation, صحیح batch_normalize, صحیح adam);

تهی forward_rnn_layer(layer l, network net);
تهی backward_rnn_layer(layer l, network net);
تهی update_rnn_layer(layer l, update_args a);

#ifdef GPU
تهی forward_rnn_layer_gpu(layer l, network net);
تهی backward_rnn_layer_gpu(layer l, network net);
تهی update_rnn_layer_gpu(layer l, update_args a);
تهی افزودن_rnn_layer(layer l);
تهی pull_rnn_layer(layer l);
#endif

#endif

