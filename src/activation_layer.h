#جيڪڏهن_وصف_نه_ٿيل ACTIVATION_LAYER_H
#وصف_ڪريو ACTIVATION_LAYER_H

#ضمن "activations.h"
#ضمن "layer.h"
#ضمن "network.h"

layer make_activation_layer(صحيح batch, صحيح inputs, ACTIVATION activation);

فراغ forward_activation_layer(layer l, ڄار net);
فراغ backward_activation_layer(layer l, ڄار net);

#جيڪڏهن_وصف_ٿيل GPU
فراغ forward_activation_layer_gpu(layer l, ڄار net);
فراغ backward_activation_layer_gpu(layer l, ڄار net);
#جيڪڏهن_پورو

#جيڪڏهن_پورو

