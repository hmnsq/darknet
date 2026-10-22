#اگر_تعریف_نشده SOFTMAX_LAYER_H
#تعریف_کن SOFTMAX_LAYER_H
#ضمن "layer.h"
#ضمن "network.h"

تعريفنوع layer softmax_layer;

فراغ softmax_array(عائم *درون_داد, صحيح n, عائم موقت, عائم *output);
softmax_layer make_softmax_layer(صحيح batch, صحيح inputs, صحيح groups);
فراغ forward_softmax_layer(ثابت softmax_layer l, شبکه net);
فراغ backward_softmax_layer(ثابت softmax_layer l, شبکه net);

#اگر_تعریف_شده GPU
فراغ pull_softmax_layer_output(ثابت softmax_layer l);
فراغ forward_softmax_layer_gpu(ثابت softmax_layer l, شبکه net);
فراغ backward_softmax_layer_gpu(ثابت softmax_layer l, شبکه net);
#پایان_اگر

#پایان_اگر
