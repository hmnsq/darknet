
#جيڪڏهن_وصف_نه_ٿيل GRU_LAYER_H
#وصف_ڪريو GRU_LAYER_H

#ضمن "activations.h"
#ضمن "layer.h"
#ضمن "network.h"

layer make_gru_layer(صحيح batch, صحيح inputs, صحيح outputs, صحيح قدمن, صحيح batch_normalize, صحيح adam);

فراغ forward_gru_layer(layer l, ڄار state);
فراغ backward_gru_layer(layer l, ڄار state);
فراغ update_gru_layer(layer l, update_دليلون a);

#جيڪڏهن_وصف_ٿيل GPU
فراغ forward_gru_layer_gpu(layer l, ڄار state);
فراغ backward_gru_layer_gpu(layer l, ڄار state);
فراغ update_gru_layer_gpu(layer l, update_دليلون a);
فراغ ڌڪو_gru_layer(layer l);
فراغ pull_gru_layer(layer l);
#جيڪڏهن_پورو

#جيڪڏهن_پورو

