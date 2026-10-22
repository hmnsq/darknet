#ifndef DETECTION_LAYER_H
#define DETECTION_LAYER_H

#include "layer.h"
#include "network.h"

typedef layer detection_layer;

detection_layer make_detection_layer(цео batch, цео inputs, цео n, цео size, цео classes, цео coords, цео rescore);
void forward_detection_layer(const detection_layer l, network net);
void backward_detection_layer(const detection_layer l, network net);

#ifdef GPU
void forward_detection_layer_gpu(const detection_layer l, network net);
void backward_detection_layer_gpu(detection_layer l, network net);
#endif

#endif
