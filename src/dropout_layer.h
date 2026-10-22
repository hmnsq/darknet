#ifndef DROPOUT_LAYER_H
#define DROPOUT_LAYER_H

#ضمن "layer.h"
#ضمن "network.h"

تعريفنوع layer dropout_layer;

dropout_layer make_dropout_layer(صحيح batch, صحيح inputs, عائم probability);

فراغ forward_dropout_layer(dropout_layer l, network net);
فراغ backward_dropout_layer(dropout_layer l, network net);
فراغ resize_dropout_layer(dropout_layer *l, صحيح inputs);

#ifdef GPU
فراغ forward_dropout_layer_gpu(dropout_layer l, network net);
فراغ backward_dropout_layer_gpu(dropout_layer l, network net);

#endif
#endif
