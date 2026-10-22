
#که_نه_تعریف_شوی RNN_LAYER_H
#تعریف_کړه RNN_LAYER_H

#ضمن "activations.h"
#ضمن "layer.h"
#ضمن "network.h"
#تعریف_کړه USET

layer make_rnn_layer(صحيح batch, صحيح inputs, صحيح outputs, صحيح ګامونه, ACTIVATION activation, صحيح batch_normalize, صحيح adam);

فراغ forward_rnn_layer(layer l, شبکه net);
فراغ backward_rnn_layer(layer l, شبکه net);
فراغ update_rnn_layer(layer l, update_دلیلونه a);

#که_تعریف_شوی GPU
فراغ forward_rnn_layer_gpu(layer l, شبکه net);
فراغ backward_rnn_layer_gpu(layer l, شبکه net);
فراغ update_rnn_layer_gpu(layer l, update_دلیلونه a);
فراغ ټېل_rnn_layer(layer l);
فراغ pull_rnn_layer(layer l);
#د_که_پای

#د_که_پای

