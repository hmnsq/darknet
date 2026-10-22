
#اگر_تعریف_نشده RNN_LAYER_H
#تعریف_کن RNN_LAYER_H

#ضمن "activations.h"
#ضمن "layer.h"
#ضمن "network.h"
#تعریف_کن USET

layer make_rnn_layer(صحيح batch, صحيح inputs, صحيح outputs, صحيح گامها, ACTIVATION activation, صحيح batch_normalize, صحيح adam);

فراغ forward_rnn_layer(layer l, شبکه net);
فراغ backward_rnn_layer(layer l, شبکه net);
فراغ update_rnn_layer(layer l, update_آرگومانها a);

#اگر_تعریف_شده GPU
فراغ forward_rnn_layer_gpu(layer l, شبکه net);
فراغ backward_rnn_layer_gpu(layer l, شبکه net);
فراغ update_rnn_layer_gpu(layer l, update_آرگومانها a);
فراغ افزودن_rnn_layer(layer l);
فراغ pull_rnn_layer(layer l);
#پایان_اگر

#پایان_اگر

