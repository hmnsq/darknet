#ifndef YOLO_LAYER_H
#define YOLO_LAYER_H

#அந்தர்பாவ "darknet.h"
#அந்தர்பாவ "layer.h"
#அந்தர்பாவ "network.h"

layer make_yolo_layer(முழு batch, முழு w, முழு h, முழு n, முழு total, முழு *mask, முழு classes);
வெறுமை forward_yolo_layer(மாறிலி layer l, network net);
வெறுமை backward_yolo_layer(மாறிலி layer l, network net);
வெறுமை resize_yolo_layer(layer *l, முழு w, முழு h);
முழு yolo_இலக்கம்_detections(layer l, தசம thresh);

#ifdef GPU
வெறுமை forward_yolo_layer_gpu(மாறிலி layer l, network net);
வெறுமை backward_yolo_layer_gpu(layer l, network net);
#endif

#endif
