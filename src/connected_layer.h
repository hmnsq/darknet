#که_نه_تعریف_شوی CONNECTED_LAYER_H
#تعریف_کړه CONNECTED_LAYER_H

#ضمن "activations.h"
#ضمن "layer.h"
#ضمن "network.h"

layer make_connected_layer(صحيح batch, صحيح inputs, صحيح outputs, ACTIVATION activation, صحيح batch_normalize, صحيح adam);

فراغ forward_connected_layer(layer l, شبکه net);
فراغ backward_connected_layer(layer l, شبکه net);
فراغ update_connected_layer(layer l, update_دلیلونه a);

#که_تعریف_شوی GPU
فراغ forward_connected_layer_gpu(layer l, شبکه net);
فراغ backward_connected_layer_gpu(layer l, شبکه net);
فراغ update_connected_layer_gpu(layer l, update_دلیلونه a);
فراغ ټېل_connected_layer(layer l);
فراغ pull_connected_layer(layer l);
#د_که_پای

#د_که_پای

