#جيڪڏهن_وصف_نه_ٿيل ISEG_LAYER_H
#وصف_ڪريو ISEG_LAYER_H

#ضمن "darknet.h"
#ضمن "layer.h"
#ضمن "network.h"

layer make_iseg_layer(صحيح batch, صحيح w, صحيح h, صحيح classes, صحيح ids);
فراغ forward_iseg_layer(ثابت layer l, ڄار net);
فراغ backward_iseg_layer(ثابت layer l, ڄار net);
فراغ resize_iseg_layer(layer *l, صحيح w, صحيح h);
صحيح iseg_انگ_detections(layer l, عائم thresh);

#جيڪڏهن_وصف_ٿيل GPU
فراغ forward_iseg_layer_gpu(ثابت layer l, ڄار net);
فراغ backward_iseg_layer_gpu(layer l, ڄار net);
#جيڪڏهن_پورو

#جيڪڏهن_پورو
