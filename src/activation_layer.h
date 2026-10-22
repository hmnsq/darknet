#ifndef ACTIVATION_LAYER_H
#define ACTIVATION_LAYER_H

#அந்தர்பாவ "activations.h"
#அந்தர்பாவ "layer.h"
#அந்தர்பாவ "network.h"

layer make_activation_layer(முழு batch, முழு inputs, ACTIVATION activation);

வெறுமை forward_activation_layer(layer l, network net);
வெறுமை backward_activation_layer(layer l, network net);

#ifdef GPU
வெறுமை forward_activation_layer_gpu(layer l, network net);
வெறுமை backward_activation_layer_gpu(layer l, network net);
#endif

#endif

