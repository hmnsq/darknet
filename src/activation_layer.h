#اگر_تعریف_نشده ACTIVATION_LAYER_H
#تعریف_کن ACTIVATION_LAYER_H

#ضمن "activations.h"
#ضمن "layer.h"
#ضمن "network.h"

layer make_activation_layer(صحيح batch, صحيح inputs, ACTIVATION activation);

فراغ forward_activation_layer(layer l, شبکه net);
فراغ backward_activation_layer(layer l, شبکه net);

#اگر_تعریف_شده GPU
فراغ forward_activation_layer_gpu(layer l, شبکه net);
فراغ backward_activation_layer_gpu(layer l, شبکه net);
#پایان_اگر

#پایان_اگر

