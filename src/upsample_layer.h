#جيڪڏهن_وصف_نه_ٿيل UPSAMPLE_LAYER_H
#وصف_ڪريو UPSAMPLE_LAYER_H
#ضمن "darknet.h"

layer make_upsample_layer(صحيح batch, صحيح w, صحيح h, صحيح c, صحيح stride);
فراغ forward_upsample_layer(ثابت layer l, ڄار net);
فراغ backward_upsample_layer(ثابت layer l, ڄار net);
فراغ resize_upsample_layer(layer *l, صحيح w, صحيح h);

#جيڪڏهن_وصف_ٿيل GPU
فراغ forward_upsample_layer_gpu(ثابت layer l, ڄار net);
فراغ backward_upsample_layer_gpu(ثابت layer l, ڄار net);
#جيڪڏهن_پورو

#جيڪڏهن_پورو
