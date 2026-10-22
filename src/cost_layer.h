#جيڪڏهن_وصف_نه_ٿيل لاڳت_LAYER_H
#وصف_ڪريو لاڳت_LAYER_H
#ضمن "layer.h"
#ضمن "network.h"

تعريفنوع layer لاڳت_layer;

لاڳت_TYPE get_لاڳت_type(حرف *s);
حرف *get_لاڳت_string(لاڳت_TYPE a);
لاڳت_layer make_لاڳت_layer(صحيح batch, صحيح inputs, لاڳت_TYPE type, عائم scale);
فراغ forward_لاڳت_layer(ثابت لاڳت_layer l, ڄار net);
فراغ backward_لاڳت_layer(ثابت لاڳت_layer l, ڄار net);
فراغ resize_لاڳت_layer(لاڳت_layer *l, صحيح inputs);

#جيڪڏهن_وصف_ٿيل GPU
فراغ forward_لاڳت_layer_gpu(لاڳت_layer l, ڄار net);
فراغ backward_لاڳت_layer_gpu(ثابت لاڳت_layer l, ڄار net);
#جيڪڏهن_پورو

#جيڪڏهن_پورو
