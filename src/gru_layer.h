
#جے_بنا_تعریف GRU_LAYER_H
#تعریف_کرو GRU_LAYER_H

#ضمن "activations.h"
#ضمن "layer.h"
#ضمن "network.h"

layer make_gru_layer(صحيح batch, صحيح inputs, صحيح outputs, صحيح قدماں, صحيح batch_normalize, صحيح adam);

فراغ forward_gru_layer(layer l, جال state);
فراغ backward_gru_layer(layer l, جال state);
فراغ update_gru_layer(layer l, update_دلیلاں a);

#جے_تعریف_شدہ GPU
فراغ forward_gru_layer_gpu(layer l, جال state);
فراغ backward_gru_layer_gpu(layer l, جال state);
فراغ update_gru_layer_gpu(layer l, update_دلیلاں a);
فراغ ڈالو_gru_layer(layer l);
فراغ pull_gru_layer(layer l);
#جے_انت

#جے_انت

