#ifndef SOFTMAX_LAYER_H
#define SOFTMAX_LAYER_H
#شامل "layer.h"
#شامل "network.h"

تعریفنوع layer softmax_layer;

تهی softmax_array(اعشاری *درون_داد, صحیح n, اعشاری temp, اعشاری *output);
softmax_layer make_softmax_layer(صحیح batch, صحیح inputs, صحیح groups);
تهی forward_softmax_layer(ثابت softmax_layer l, network net);
تهی backward_softmax_layer(ثابت softmax_layer l, network net);

#ifdef GPU
تهی pull_softmax_layer_output(ثابت softmax_layer l);
تهی forward_softmax_layer_gpu(ثابت softmax_layer l, network net);
تهی backward_softmax_layer_gpu(ثابت softmax_layer l, network net);
#endif

#endif
