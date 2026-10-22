#جے_بنا_تعریف SOFTMAX_LAYER_H
#تعریف_کرو SOFTMAX_LAYER_H
#ضمن "layer.h"
#ضمن "network.h"

تعريفنوع layer softmax_layer;

فراغ softmax_array(عائم *داخلہ, صحيح n, عائم عارضی, عائم *output);
softmax_layer make_softmax_layer(صحيح batch, صحيح inputs, صحيح groups);
فراغ forward_softmax_layer(ثابت softmax_layer l, جال net);
فراغ backward_softmax_layer(ثابت softmax_layer l, جال net);

#جے_تعریف_شدہ GPU
فراغ pull_softmax_layer_output(ثابت softmax_layer l);
فراغ forward_softmax_layer_gpu(ثابت softmax_layer l, جال net);
فراغ backward_softmax_layer_gpu(ثابت softmax_layer l, جال net);
#جے_انت

#جے_انت
