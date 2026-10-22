#اگر_تعریف_نشده هزینه_LAYER_H
#تعریف_کن هزینه_LAYER_H
#ضمن "layer.h"
#ضمن "network.h"

تعريفنوع layer هزینه_layer;

هزینه_TYPE get_هزینه_type(حرف *s);
حرف *get_هزینه_string(هزینه_TYPE a);
هزینه_layer make_هزینه_layer(صحيح batch, صحيح inputs, هزینه_TYPE type, عائم scale);
فراغ forward_هزینه_layer(ثابت هزینه_layer l, شبکه net);
فراغ backward_هزینه_layer(ثابت هزینه_layer l, شبکه net);
فراغ resize_هزینه_layer(هزینه_layer *l, صحيح inputs);

#اگر_تعریف_شده GPU
فراغ forward_هزینه_layer_gpu(هزینه_layer l, شبکه net);
فراغ backward_هزینه_layer_gpu(ثابت هزینه_layer l, شبکه net);
#پایان_اگر

#پایان_اگر
