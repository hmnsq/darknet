#ifndef SOFTMAX_LAYER_H
#define SOFTMAX_LAYER_H
#ضمن "layer.h"
#ضمن "network.h"

تعريفنوع layer softmax_layer;

فراغ softmax_array(عائم *مدخل, صحيح n, عائم temp, عائم *output);
softmax_layer make_softmax_layer(صحيح batch, صحيح inputs, صحيح groups);
فراغ forward_softmax_layer(ثابت softmax_layer l, network net);
فراغ backward_softmax_layer(ثابت softmax_layer l, network net);

#ifdef GPU
فراغ pull_softmax_layer_output(ثابت softmax_layer l);
فراغ forward_softmax_layer_gpu(ثابت softmax_layer l, network net);
فراغ backward_softmax_layer_gpu(ثابت softmax_layer l, network net);
#endif

#endif
