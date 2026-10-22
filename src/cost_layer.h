#اگر_غیر_تعریف_شدہ لاگت_LAYER_H
#تعریف_کرو لاگت_LAYER_H
#شامل "layer.h"
#شامل "network.h"

قسمتعریف layer لاگت_layer;

لاگت_TYPE get_لاگت_type(حرف *s);
حرف *get_لاگت_string(لاگت_TYPE a);
لاگت_layer make_لاگت_layer(صحیح batch, صحیح inputs, لاگت_TYPE type, اعشاری scale);
خالی forward_لاگت_layer(ثابت لاگت_layer l, جال net);
خالی backward_لاگت_layer(ثابت لاگت_layer l, جال net);
خالی resize_لاگت_layer(لاگت_layer *l, صحیح inputs);

#اگر_تعریف_شدہ GPU
خالی forward_لاگت_layer_gpu(لاگت_layer l, جال net);
خالی backward_لاگت_layer_gpu(ثابت لاگت_layer l, جال net);
#اگر_ختم

#اگر_ختم
