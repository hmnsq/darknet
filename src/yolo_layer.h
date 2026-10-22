#ifndef YOLO_LAYER_H
#define YOLO_LAYER_H

#شامل "darknet.h"
#شامل "layer.h"
#شامل "network.h"

layer make_yolo_layer(صحیح batch, صحیح w, صحیح h, صحیح n, صحیح total, صحیح *mask, صحیح classes);
تهی forward_yolo_layer(ثابت layer l, network net);
تهی backward_yolo_layer(ثابت layer l, network net);
تهی resize_yolo_layer(layer *l, صحیح w, صحیح h);
صحیح yolo_رقم_detections(layer l, اعشاری thresh);

#ifdef GPU
تهی forward_yolo_layer_gpu(ثابت layer l, network net);
تهی backward_yolo_layer_gpu(layer l, network net);
#endif

#endif
