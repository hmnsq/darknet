
#ifndef CRNN_LAYER_H
#define CRNN_LAYER_H

#شامل "activations.h"
#شامل "layer.h"
#شامل "network.h"

layer make_crnn_layer(صحیح batch, صحیح h, صحیح w, صحیح c, صحیح hidden_filters, صحیح output_filters, صحیح steps, ACTIVATION activation, صحیح batch_normalize);

تهی forward_crnn_layer(layer l, network net);
تهی backward_crnn_layer(layer l, network net);
تهی update_crnn_layer(layer l, update_args a);

#ifdef GPU
تهی forward_crnn_layer_gpu(layer l, network net);
تهی backward_crnn_layer_gpu(layer l, network net);
تهی update_crnn_layer_gpu(layer l, update_args a);
تهی افزودن_crnn_layer(layer l);
تهی pull_crnn_layer(layer l);
#endif

#endif

