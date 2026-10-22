
#که_نه_تعریف_شوی GRU_LAYER_H
#تعریف_کړه GRU_LAYER_H

#ضمن "activations.h"
#ضمن "layer.h"
#ضمن "network.h"

layer make_gru_layer(صحيح batch, صحيح inputs, صحيح outputs, صحيح ګامونه, صحيح batch_normalize, صحيح adam);

فراغ forward_gru_layer(layer l, شبکه state);
فراغ backward_gru_layer(layer l, شبکه state);
فراغ update_gru_layer(layer l, update_دلیلونه a);

#که_تعریف_شوی GPU
فراغ forward_gru_layer_gpu(layer l, شبکه state);
فراغ backward_gru_layer_gpu(layer l, شبکه state);
فراغ update_gru_layer_gpu(layer l, update_دلیلونه a);
فراغ ټېل_gru_layer(layer l);
فراغ pull_gru_layer(layer l);
#د_که_پای

#د_که_پای

