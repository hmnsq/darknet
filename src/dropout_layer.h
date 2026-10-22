#اگر_غیر_تعریف_شدہ DROPOUT_LAYER_H
#تعریف_کرو DROPOUT_LAYER_H

#شامل "layer.h"
#شامل "network.h"

قسمتعریف layer dropout_layer;

dropout_layer make_dropout_layer(صحیح batch, صحیح inputs, اعشاری probability);

خالی forward_dropout_layer(dropout_layer l, جال net);
خالی backward_dropout_layer(dropout_layer l, جال net);
خالی resize_dropout_layer(dropout_layer *l, صحیح inputs);

#اگر_تعریف_شدہ GPU
خالی forward_dropout_layer_gpu(dropout_layer l, جال net);
خالی backward_dropout_layer_gpu(dropout_layer l, جال net);

#اگر_ختم
#اگر_ختم
