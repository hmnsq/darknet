
#جے_بنا_تعریف RNN_LAYER_H
#تعریف_کرو RNN_LAYER_H

#ضمن "activations.h"
#ضمن "layer.h"
#ضمن "network.h"
#تعریف_کرو USET

layer make_rnn_layer(صحيح batch, صحيح inputs, صحيح outputs, صحيح قدماں, ACTIVATION activation, صحيح batch_normalize, صحيح adam);

فراغ forward_rnn_layer(layer l, جال net);
فراغ backward_rnn_layer(layer l, جال net);
فراغ update_rnn_layer(layer l, update_دلیلاں a);

#جے_تعریف_شدہ GPU
فراغ forward_rnn_layer_gpu(layer l, جال net);
فراغ backward_rnn_layer_gpu(layer l, جال net);
فراغ update_rnn_layer_gpu(layer l, update_دلیلاں a);
فراغ ڈالو_rnn_layer(layer l);
فراغ pull_rnn_layer(layer l);
#جے_انت

#جے_انت

