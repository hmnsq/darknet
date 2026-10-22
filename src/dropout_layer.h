#جے_بنا_تعریف DROPOUT_LAYER_H
#تعریف_کرو DROPOUT_LAYER_H

#ضمن "layer.h"
#ضمن "network.h"

تعريفنوع layer dropout_layer;

dropout_layer make_dropout_layer(صحيح batch, صحيح inputs, عائم probability);

فراغ forward_dropout_layer(dropout_layer l, جال net);
فراغ backward_dropout_layer(dropout_layer l, جال net);
فراغ resize_dropout_layer(dropout_layer *l, صحيح inputs);

#جے_تعریف_شدہ GPU
فراغ forward_dropout_layer_gpu(dropout_layer l, جال net);
فراغ backward_dropout_layer_gpu(dropout_layer l, جال net);

#جے_انت
#جے_انت
