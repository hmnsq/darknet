#که_نه_تعریف_شوی SOFTMAX_LAYER_H
#تعریف_کړه SOFTMAX_LAYER_H
#ضمن "layer.h"
#ضمن "network.h"

تعريفنوع layer softmax_layer;

فراغ softmax_array(عائم *ننوت, صحيح n, عائم لنډمهاله, عائم *output);
softmax_layer make_softmax_layer(صحيح batch, صحيح inputs, صحيح groups);
فراغ forward_softmax_layer(ثابت softmax_layer l, شبکه net);
فراغ backward_softmax_layer(ثابت softmax_layer l, شبکه net);

#که_تعریف_شوی GPU
فراغ pull_softmax_layer_output(ثابت softmax_layer l);
فراغ forward_softmax_layer_gpu(ثابت softmax_layer l, شبکه net);
فراغ backward_softmax_layer_gpu(ثابت softmax_layer l, شبکه net);
#د_که_پای

#د_که_پای
