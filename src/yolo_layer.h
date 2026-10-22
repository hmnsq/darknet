#ifndef YOLO_LAYER_H
#define YOLO_LAYER_H

#include "darknet.h"
#include "layer.h"
#include "network.h"

layer make_yolo_layer(จำนวนเต็ม batch, จำนวนเต็ม w, จำนวนเต็ม h, จำนวนเต็ม n, จำนวนเต็ม total, จำนวนเต็ม *mask, จำนวนเต็ม classes);
void forward_yolo_layer(const layer l, network net);
void backward_yolo_layer(const layer l, network net);
void resize_yolo_layer(layer *l, จำนวนเต็ม w, จำนวนเต็ม h);
จำนวนเต็ม yolo_num_detections(layer l, float thresh);

#ifdef GPU
void forward_yolo_layer_gpu(const layer l, network net);
void backward_yolo_layer_gpu(layer l, network net);
#endif

#endif
