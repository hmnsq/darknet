#ifndef DROPOUT_LAYER_H
#define DROPOUT_LAYER_H

#شامل "layer.h"
#شامل "network.h"

تعریفنوع layer dropout_layer;

dropout_layer make_dropout_layer(صحیح batch, صحیح inputs, اعشاری probability);

تهی forward_dropout_layer(dropout_layer l, network net);
تهی backward_dropout_layer(dropout_layer l, network net);
تهی resize_dropout_layer(dropout_layer *l, صحیح inputs);

#ifdef GPU
تهی forward_dropout_layer_gpu(dropout_layer l, network net);
تهی backward_dropout_layer_gpu(dropout_layer l, network net);

#endif
#endif
