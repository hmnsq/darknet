#جيڪڏهن_وصف_نه_ٿيل REORG_LAYER_H
#وصف_ڪريو REORG_LAYER_H

#ضمن "image.h"
#ضمن "cuda.h"
#ضمن "layer.h"
#ضمن "network.h"

layer make_reorg_layer(صحيح batch, صحيح w, صحيح h, صحيح c, صحيح stride, صحيح ابتو, صحيح flatten, صحيح extra);
فراغ resize_reorg_layer(layer *l, صحيح w, صحيح h);
فراغ forward_reorg_layer(ثابت layer l, ڄار net);
فراغ backward_reorg_layer(ثابت layer l, ڄار net);

#جيڪڏهن_وصف_ٿيل GPU
فراغ forward_reorg_layer_gpu(layer l, ڄار net);
فراغ backward_reorg_layer_gpu(layer l, ڄار net);
#جيڪڏهن_پورو

#جيڪڏهن_پورو

