#اگر_تعریف_نشده REORG_LAYER_H
#تعریف_کن REORG_LAYER_H

#ضمن "image.h"
#ضمن "cuda.h"
#ضمن "layer.h"
#ضمن "network.h"

layer make_reorg_layer(صحيح batch, صحيح w, صحيح h, صحيح c, صحيح stride, صحيح معکوس, صحيح flatten, صحيح extra);
فراغ resize_reorg_layer(layer *l, صحيح w, صحيح h);
فراغ forward_reorg_layer(ثابت layer l, شبکه net);
فراغ backward_reorg_layer(ثابت layer l, شبکه net);

#اگر_تعریف_شده GPU
فراغ forward_reorg_layer_gpu(layer l, شبکه net);
فراغ backward_reorg_layer_gpu(layer l, شبکه net);
#پایان_اگر

#پایان_اگر

