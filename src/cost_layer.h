#جے_بنا_تعریف لاگت_LAYER_H
#تعریف_کرو لاگت_LAYER_H
#ضمن "layer.h"
#ضمن "network.h"

تعريفنوع layer لاگت_layer;

لاگت_TYPE get_لاگت_type(حرف *s);
حرف *get_لاگت_string(لاگت_TYPE a);
لاگت_layer make_لاگت_layer(صحيح batch, صحيح inputs, لاگت_TYPE type, عائم scale);
فراغ forward_لاگت_layer(ثابت لاگت_layer l, جال net);
فراغ backward_لاگت_layer(ثابت لاگت_layer l, جال net);
فراغ resize_لاگت_layer(لاگت_layer *l, صحيح inputs);

#جے_تعریف_شدہ GPU
فراغ forward_لاگت_layer_gpu(لاگت_layer l, جال net);
فراغ backward_لاگت_layer_gpu(ثابت لاگت_layer l, جال net);
#جے_انت

#جے_انت
