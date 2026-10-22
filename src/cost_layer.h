#که_نه_تعریف_شوی لګښت_LAYER_H
#تعریف_کړه لګښت_LAYER_H
#ضمن "layer.h"
#ضمن "network.h"

تعريفنوع layer لګښت_layer;

لګښت_TYPE get_لګښت_type(حرف *s);
حرف *get_لګښت_string(لګښت_TYPE a);
لګښت_layer make_لګښت_layer(صحيح batch, صحيح inputs, لګښت_TYPE type, عائم scale);
فراغ forward_لګښت_layer(ثابت لګښت_layer l, شبکه net);
فراغ backward_لګښت_layer(ثابت لګښت_layer l, شبکه net);
فراغ resize_لګښت_layer(لګښت_layer *l, صحيح inputs);

#که_تعریف_شوی GPU
فراغ forward_لګښت_layer_gpu(لګښت_layer l, شبکه net);
فراغ backward_لګښت_layer_gpu(ثابت لګښت_layer l, شبکه net);
#د_که_پای

#د_که_پای
