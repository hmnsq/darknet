#جيڪڏهن_وصف_نه_ٿيل CONNECTED_LAYER_H
#وصف_ڪريو CONNECTED_LAYER_H

#ضمن "activations.h"
#ضمن "layer.h"
#ضمن "network.h"

layer make_connected_layer(صحيح batch, صحيح inputs, صحيح outputs, ACTIVATION activation, صحيح batch_normalize, صحيح adam);

فراغ forward_connected_layer(layer l, ڄار net);
فراغ backward_connected_layer(layer l, ڄار net);
فراغ update_connected_layer(layer l, update_دليلون a);

#جيڪڏهن_وصف_ٿيل GPU
فراغ forward_connected_layer_gpu(layer l, ڄار net);
فراغ backward_connected_layer_gpu(layer l, ڄار net);
فراغ update_connected_layer_gpu(layer l, update_دليلون a);
فراغ ڌڪو_connected_layer(layer l);
فراغ pull_connected_layer(layer l);
#جيڪڏهن_پورو

#جيڪڏهن_پورو

