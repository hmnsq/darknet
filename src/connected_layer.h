#اگر_تعریف_نشده CONNECTED_LAYER_H
#تعریف_کن CONNECTED_LAYER_H

#ضمن "activations.h"
#ضمن "layer.h"
#ضمن "network.h"

layer make_connected_layer(صحيح batch, صحيح inputs, صحيح outputs, ACTIVATION activation, صحيح batch_normalize, صحيح adam);

فراغ forward_connected_layer(layer l, شبکه net);
فراغ backward_connected_layer(layer l, شبکه net);
فراغ update_connected_layer(layer l, update_آرگومانها a);

#اگر_تعریف_شده GPU
فراغ forward_connected_layer_gpu(layer l, شبکه net);
فراغ backward_connected_layer_gpu(layer l, شبکه net);
فراغ update_connected_layer_gpu(layer l, update_آرگومانها a);
فراغ افزودن_connected_layer(layer l);
فراغ pull_connected_layer(layer l);
#پایان_اگر

#پایان_اگر

