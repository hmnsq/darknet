#ifndef CONNECTED_LAYER_H
#define CONNECTED_LAYER_H

#ضمن "activations.h"
#ضمن "layer.h"
#ضمن "network.h"

layer make_connected_layer(صحيح batch, صحيح inputs, صحيح outputs, ACTIVATION activation, صحيح batch_normalize, صحيح adam);

فراغ forward_connected_layer(layer l, network net);
فراغ backward_connected_layer(layer l, network net);
فراغ update_connected_layer(layer l, update_args a);

#ifdef GPU
فراغ forward_connected_layer_gpu(layer l, network net);
فراغ backward_connected_layer_gpu(layer l, network net);
فراغ update_connected_layer_gpu(layer l, update_args a);
فراغ پاڵنان_connected_layer(layer l);
فراغ pull_connected_layer(layer l);
#endif

#endif

