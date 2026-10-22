#اگر_تعریف_نشده UPSAMPLE_LAYER_H
#تعریف_کن UPSAMPLE_LAYER_H
#ضمن "darknet.h"

layer make_upsample_layer(صحيح batch, صحيح w, صحيح h, صحيح c, صحيح stride);
فراغ forward_upsample_layer(ثابت layer l, شبکه net);
فراغ backward_upsample_layer(ثابت layer l, شبکه net);
فراغ resize_upsample_layer(layer *l, صحيح w, صحيح h);

#اگر_تعریف_شده GPU
فراغ forward_upsample_layer_gpu(ثابت layer l, شبکه net);
فراغ backward_upsample_layer_gpu(ثابت layer l, شبکه net);
#پایان_اگر

#پایان_اگر
