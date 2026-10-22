#جے_بنا_تعریف UPSAMPLE_LAYER_H
#تعریف_کرو UPSAMPLE_LAYER_H
#ضمن "darknet.h"

layer make_upsample_layer(صحيح batch, صحيح w, صحيح h, صحيح c, صحيح stride);
فراغ forward_upsample_layer(ثابت layer l, جال net);
فراغ backward_upsample_layer(ثابت layer l, جال net);
فراغ resize_upsample_layer(layer *l, صحيح w, صحيح h);

#جے_تعریف_شدہ GPU
فراغ forward_upsample_layer_gpu(ثابت layer l, جال net);
فراغ backward_upsample_layer_gpu(ثابت layer l, جال net);
#جے_انت

#جے_انت
