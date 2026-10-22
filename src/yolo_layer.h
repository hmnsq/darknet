#جيڪڏهن_وصف_نه_ٿيل YOLO_LAYER_H
#وصف_ڪريو YOLO_LAYER_H

#ضمن "darknet.h"
#ضمن "layer.h"
#ضمن "network.h"

layer make_yolo_layer(صحيح batch, صحيح w, صحيح h, صحيح n, صحيح ڪل, صحيح *نقاب, صحيح classes);
فراغ forward_yolo_layer(ثابت layer l, ڄار net);
فراغ backward_yolo_layer(ثابت layer l, ڄار net);
فراغ resize_yolo_layer(layer *l, صحيح w, صحيح h);
صحيح yolo_انگ_detections(layer l, عائم thresh);

#جيڪڏهن_وصف_ٿيل GPU
فراغ forward_yolo_layer_gpu(ثابت layer l, ڄار net);
فراغ backward_yolo_layer_gpu(layer l, ڄار net);
#جيڪڏهن_پورو

#جيڪڏهن_پورو
