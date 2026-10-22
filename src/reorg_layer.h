#جے_بنا_تعریف REORG_LAYER_H
#تعریف_کرو REORG_LAYER_H

#ضمن "image.h"
#ضمن "cuda.h"
#ضمن "layer.h"
#ضمن "network.h"

layer make_reorg_layer(صحيح batch, صحيح w, صحيح h, صحيح c, صحيح stride, صحيح الٹ, صحيح flatten, صحيح extra);
فراغ resize_reorg_layer(layer *l, صحيح w, صحيح h);
فراغ forward_reorg_layer(ثابت layer l, جال net);
فراغ backward_reorg_layer(ثابت layer l, جال net);

#جے_تعریف_شدہ GPU
فراغ forward_reorg_layer_gpu(layer l, جال net);
فراغ backward_reorg_layer_gpu(layer l, جال net);
#جے_انت

#جے_انت

