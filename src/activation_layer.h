#ifndef ACTIVATION_LAYER_H
#define ACTIVATION_LAYER_H

#شامل "activations.h"
#شامل "layer.h"
#شامل "network.h"

layer make_activation_layer(صحیح batch, صحیح inputs, ACTIVATION activation);

تهی forward_activation_layer(layer l, network net);
تهی backward_activation_layer(layer l, network net);

#ifdef GPU
تهی forward_activation_layer_gpu(layer l, network net);
تهی backward_activation_layer_gpu(layer l, network net);
#endif

#endif

