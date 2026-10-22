#که_نه_تعریف_شوی REORG_LAYER_H
#تعریف_کړه REORG_LAYER_H

#ضمن "image.h"
#ضمن "cuda.h"
#ضمن "layer.h"
#ضمن "network.h"

layer make_reorg_layer(صحيح batch, صحيح w, صحيح h, صحيح c, صحيح stride, صحيح سرچپه, صحيح flatten, صحيح extra);
فراغ resize_reorg_layer(layer *l, صحيح w, صحيح h);
فراغ forward_reorg_layer(ثابت layer l, شبکه net);
فراغ backward_reorg_layer(ثابت layer l, شبکه net);

#که_تعریف_شوی GPU
فراغ forward_reorg_layer_gpu(layer l, شبکه net);
فراغ backward_reorg_layer_gpu(layer l, شبکه net);
#د_که_پای

#د_که_پای

