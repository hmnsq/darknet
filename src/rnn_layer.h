
#ifndef RNN_LAYER_H
#define RNN_LAYER_H

#அந்தர்பாவ "activations.h"
#அந்தர்பாவ "layer.h"
#அந்தர்பாவ "network.h"
#define USET

layer make_rnn_layer(முழு batch, முழு inputs, முழு outputs, முழு steps, ACTIVATION activation, முழு batch_normalize, முழு adam);

வெறுமை forward_rnn_layer(layer l, network net);
வெறுமை backward_rnn_layer(layer l, network net);
வெறுமை update_rnn_layer(layer l, update_args a);

#ifdef GPU
வெறுமை forward_rnn_layer_gpu(layer l, network net);
வெறுமை backward_rnn_layer_gpu(layer l, network net);
வெறுமை update_rnn_layer_gpu(layer l, update_args a);
வெறுமை தள்ளு_rnn_layer(layer l);
வெறுமை pull_rnn_layer(layer l);
#endif

#endif

