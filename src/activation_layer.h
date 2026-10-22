#که_نه_تعریف_شوی ACTIVATION_LAYER_H
#تعریف_کړه ACTIVATION_LAYER_H

#ضمن "activations.h"
#ضمن "layer.h"
#ضمن "network.h"

layer make_activation_layer(صحيح batch, صحيح inputs, ACTIVATION activation);

فراغ forward_activation_layer(layer l, شبکه net);
فراغ backward_activation_layer(layer l, شبکه net);

#که_تعریف_شوی GPU
فراغ forward_activation_layer_gpu(layer l, شبکه net);
فراغ backward_activation_layer_gpu(layer l, شبکه net);
#د_که_پای

#د_که_پای

