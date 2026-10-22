#جيڪڏهن_وصف_نه_ٿيل DROPOUT_LAYER_H
#وصف_ڪريو DROPOUT_LAYER_H

#ضمن "layer.h"
#ضمن "network.h"

تعريفنوع layer dropout_layer;

dropout_layer make_dropout_layer(صحيح batch, صحيح inputs, عائم probability);

فراغ forward_dropout_layer(dropout_layer l, ڄار net);
فراغ backward_dropout_layer(dropout_layer l, ڄار net);
فراغ resize_dropout_layer(dropout_layer *l, صحيح inputs);

#جيڪڏهن_وصف_ٿيل GPU
فراغ forward_dropout_layer_gpu(dropout_layer l, ڄار net);
فراغ backward_dropout_layer_gpu(dropout_layer l, ڄار net);

#جيڪڏهن_پورو
#جيڪڏهن_پورو
