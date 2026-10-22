#جيڪڏهن_وصف_نه_ٿيل REGION_LAYER_H
#وصف_ڪريو REGION_LAYER_H

#ضمن "darknet.h"
#ضمن "layer.h"
#ضمن "network.h"

layer make_region_layer(صحيح batch, صحيح w, صحيح h, صحيح n, صحيح classes, صحيح coords);
فراغ forward_region_layer(ثابت layer l, ڄار net);
فراغ backward_region_layer(ثابت layer l, ڄار net);
فراغ resize_region_layer(layer *l, صحيح w, صحيح h);

#جيڪڏهن_وصف_ٿيل GPU
فراغ forward_region_layer_gpu(ثابت layer l, ڄار net);
فراغ backward_region_layer_gpu(layer l, ڄار net);
#جيڪڏهن_پورو

#جيڪڏهن_پورو
