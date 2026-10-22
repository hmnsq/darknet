#ifndef DETECTION_LAYER_H
#define DETECTION_LAYER_H

#include "layer.h"
#include "network.h"

typedef layer detection_layer;

detection_layer make_detection_layer(ကိန်းပြည့် batch, ကိန်းပြည့် inputs, ကိန်းပြည့် n, ကိန်းပြည့် size, ကိန်းပြည့် classes, ကိန်းပြည့် coords, ကိန်းပြည့် rescore);
void forward_detection_layer(const detection_layer l, network net);
void backward_detection_layer(const detection_layer l, network net);

#ifdef GPU
void forward_detection_layer_gpu(const detection_layer l, network net);
void backward_detection_layer_gpu(detection_layer l, network net);
#endif

#endif
