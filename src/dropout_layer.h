#اگر_تعریف_نشده DROPOUT_LAYER_H
#تعریف_کن DROPOUT_LAYER_H

#ضمن "layer.h"
#ضمن "network.h"

تعريفنوع layer dropout_layer;

dropout_layer make_dropout_layer(صحيح batch, صحيح inputs, عائم probability);

فراغ forward_dropout_layer(dropout_layer l, شبکه net);
فراغ backward_dropout_layer(dropout_layer l, شبکه net);
فراغ resize_dropout_layer(dropout_layer *l, صحيح inputs);

#اگر_تعریف_شده GPU
فراغ forward_dropout_layer_gpu(dropout_layer l, شبکه net);
فراغ backward_dropout_layer_gpu(dropout_layer l, شبکه net);

#پایان_اگر
#پایان_اگر
