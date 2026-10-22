#ifndef CONNECTED_LAYER_H
#define CONNECTED_LAYER_H

#شامل "activations.h"
#شامل "layer.h"
#شامل "network.h"

layer make_connected_layer(صحیح batch, صحیح inputs, صحیح outputs, ACTIVATION activation, صحیح batch_normalize, صحیح adam);

تهی forward_connected_layer(layer l, network net);
تهی backward_connected_layer(layer l, network net);
تهی update_connected_layer(layer l, update_args a);

#ifdef GPU
تهی forward_connected_layer_gpu(layer l, network net);
تهی backward_connected_layer_gpu(layer l, network net);
تهی update_connected_layer_gpu(layer l, update_args a);
تهی افزودن_connected_layer(layer l);
تهی pull_connected_layer(layer l);
#endif

#endif

