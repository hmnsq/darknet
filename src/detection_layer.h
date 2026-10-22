#جيڪڏهن_وصف_نه_ٿيل DETECTION_LAYER_H
#وصف_ڪريو DETECTION_LAYER_H

#ضمن "layer.h"
#ضمن "network.h"

تعريفنوع layer detection_layer;

detection_layer make_detection_layer(صحيح batch, صحيح inputs, صحيح n, صحيح سائيز, صحيح classes, صحيح coords, صحيح rescore);
فراغ forward_detection_layer(ثابت detection_layer l, ڄار net);
فراغ backward_detection_layer(ثابت detection_layer l, ڄار net);

#جيڪڏهن_وصف_ٿيل GPU
فراغ forward_detection_layer_gpu(ثابت detection_layer l, ڄار net);
فراغ backward_detection_layer_gpu(detection_layer l, ڄار net);
#جيڪڏهن_پورو

#جيڪڏهن_پورو
