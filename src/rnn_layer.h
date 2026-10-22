
#جيڪڏهن_وصف_نه_ٿيل RNN_LAYER_H
#وصف_ڪريو RNN_LAYER_H

#ضمن "activations.h"
#ضمن "layer.h"
#ضمن "network.h"
#وصف_ڪريو USET

layer make_rnn_layer(صحيح batch, صحيح inputs, صحيح outputs, صحيح قدمن, ACTIVATION activation, صحيح batch_normalize, صحيح adam);

فراغ forward_rnn_layer(layer l, ڄار net);
فراغ backward_rnn_layer(layer l, ڄار net);
فراغ update_rnn_layer(layer l, update_دليلون a);

#جيڪڏهن_وصف_ٿيل GPU
فراغ forward_rnn_layer_gpu(layer l, ڄار net);
فراغ backward_rnn_layer_gpu(layer l, ڄار net);
فراغ update_rnn_layer_gpu(layer l, update_دليلون a);
فراغ ڌڪو_rnn_layer(layer l);
فراغ pull_rnn_layer(layer l);
#جيڪڏهن_پورو

#جيڪڏهن_پورو

