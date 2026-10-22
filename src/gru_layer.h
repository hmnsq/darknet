
#ifndef GRU_LAYER_H
#define GRU_LAYER_H

#شامل "activations.h"
#شامل "layer.h"
#شامل "network.h"

layer make_gru_layer(صحیح batch, صحیح inputs, صحیح outputs, صحیح steps, صحیح batch_normalize, صحیح adam);

تهی forward_gru_layer(layer l, network state);
تهی backward_gru_layer(layer l, network state);
تهی update_gru_layer(layer l, update_args a);

#ifdef GPU
تهی forward_gru_layer_gpu(layer l, network state);
تهی backward_gru_layer_gpu(layer l, network state);
تهی update_gru_layer_gpu(layer l, update_args a);
تهی افزودن_gru_layer(layer l);
تهی pull_gru_layer(layer l);
#endif

#endif

