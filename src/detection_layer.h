#ifndef DETECTION_LAYER_H
#define DETECTION_LAYER_H

#شامل "layer.h"
#شامل "network.h"

تعریفنوع layer detection_layer;

detection_layer make_detection_layer(صحیح batch, صحیح inputs, صحیح n, صحیح size, صحیح classes, صحیح coords, صحیح rescore);
تهی forward_detection_layer(ثابت detection_layer l, network net);
تهی backward_detection_layer(ثابت detection_layer l, network net);

#ifdef GPU
تهی forward_detection_layer_gpu(ثابت detection_layer l, network net);
تهی backward_detection_layer_gpu(detection_layer l, network net);
#endif

#endif
