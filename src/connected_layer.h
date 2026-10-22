#ifndef CONNECTED_LAYER_H
#define CONNECTED_LAYER_H

#அந்தர்பாவ "activations.h"
#அந்தர்பாவ "layer.h"
#அந்தர்பாவ "network.h"

layer make_connected_layer(முழு batch, முழு inputs, முழு outputs, ACTIVATION activation, முழு batch_normalize, முழு adam);

வெறுமை forward_connected_layer(layer l, network net);
வெறுமை backward_connected_layer(layer l, network net);
வெறுமை update_connected_layer(layer l, update_args a);

#ifdef GPU
வெறுமை forward_connected_layer_gpu(layer l, network net);
வெறுமை backward_connected_layer_gpu(layer l, network net);
வெறுமை update_connected_layer_gpu(layer l, update_args a);
வெறுமை தள்ளு_connected_layer(layer l);
வெறுமை pull_connected_layer(layer l);
#endif

#endif

