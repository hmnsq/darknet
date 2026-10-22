
#ifndef GRU_LAYER_H
#define GRU_LAYER_H

#ضمن "activations.h"
#ضمن "layer.h"
#ضمن "network.h"

layer make_gru_layer(صحيح batch, صحيح inputs, صحيح outputs, صحيح steps, صحيح batch_normalize, صحيح adam);

فراغ forward_gru_layer(layer l, network state);
فراغ backward_gru_layer(layer l, network state);
فراغ update_gru_layer(layer l, update_وسائط a);

#ifdef GPU
فراغ forward_gru_layer_gpu(layer l, network state);
فراغ backward_gru_layer_gpu(layer l, network state);
فراغ update_gru_layer_gpu(layer l, update_وسائط a);
فراغ ادفع_gru_layer(layer l);
فراغ pull_gru_layer(layer l);
#endif

#endif

