#که_نه_تعریف_شوی DROPOUT_LAYER_H
#تعریف_کړه DROPOUT_LAYER_H

#ضمن "layer.h"
#ضمن "network.h"

تعريفنوع layer dropout_layer;

dropout_layer make_dropout_layer(صحيح batch, صحيح inputs, عائم probability);

فراغ forward_dropout_layer(dropout_layer l, شبکه net);
فراغ backward_dropout_layer(dropout_layer l, شبکه net);
فراغ resize_dropout_layer(dropout_layer *l, صحيح inputs);

#که_تعریف_شوی GPU
فراغ forward_dropout_layer_gpu(dropout_layer l, شبکه net);
فراغ backward_dropout_layer_gpu(dropout_layer l, شبکه net);

#د_که_پای
#د_که_پای
