#جيڪڏهن_وصف_نه_ٿيل SOFTMAX_LAYER_H
#وصف_ڪريو SOFTMAX_LAYER_H
#ضمن "layer.h"
#ضمن "network.h"

تعريفنوع layer softmax_layer;

فراغ softmax_array(عائم *داخلا, صحيح n, عائم عارضي, عائم *output);
softmax_layer make_softmax_layer(صحيح batch, صحيح inputs, صحيح groups);
فراغ forward_softmax_layer(ثابت softmax_layer l, ڄار net);
فراغ backward_softmax_layer(ثابت softmax_layer l, ڄار net);

#جيڪڏهن_وصف_ٿيل GPU
فراغ pull_softmax_layer_output(ثابت softmax_layer l);
فراغ forward_softmax_layer_gpu(ثابت softmax_layer l, ڄار net);
فراغ backward_softmax_layer_gpu(ثابت softmax_layer l, ڄار net);
#جيڪڏهن_پورو

#جيڪڏهن_پورو
