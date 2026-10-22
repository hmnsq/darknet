#ifndef YOLO_LAYER_H
#define YOLO_LAYER_H

#ضمن "darknet.h"
#ضمن "layer.h"
#ضمن "network.h"

layer make_yolo_layer(صحيح batch, صحيح w, صحيح h, صحيح n, صحيح total, صحيح *mask, صحيح classes);
فراغ forward_yolo_layer(ثابت layer l, network net);
فراغ backward_yolo_layer(ثابت layer l, network net);
فراغ resize_yolo_layer(layer *l, صحيح w, صحيح h);
صحيح yolo_لمب_detections(layer l, عائم thresh);

#ifdef GPU
فراغ forward_yolo_layer_gpu(ثابت layer l, network net);
فراغ backward_yolo_layer_gpu(layer l, network net);
#endif

#endif
