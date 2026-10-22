#اگر_غیر_تعریف_شدہ SOFTMAX_LAYER_H
#تعریف_کرو SOFTMAX_LAYER_H
#شامل "layer.h"
#شامل "network.h"

قسمتعریف layer softmax_layer;

خالی softmax_array(اعشاری *داخلہ, صحیح n, اعشاری عارضی, اعشاری *output);
softmax_layer make_softmax_layer(صحیح batch, صحیح inputs, صحیح groups);
خالی forward_softmax_layer(ثابت softmax_layer l, جال net);
خالی backward_softmax_layer(ثابت softmax_layer l, جال net);

#اگر_تعریف_شدہ GPU
خالی pull_softmax_layer_output(ثابت softmax_layer l);
خالی forward_softmax_layer_gpu(ثابت softmax_layer l, جال net);
خالی backward_softmax_layer_gpu(ثابت softmax_layer l, جال net);
#اگر_ختم

#اگر_ختم
