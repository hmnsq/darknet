#ifndef YOLO_LAYER_H
#define YOLO_LAYER_H

#include "darknet.h"
#include "layer.h"
#include "network.h"

layer make_yolo_layer(ចំនួនគត់ batch, ចំនួនគត់ w, ចំនួនគត់ h, ចំនួនគត់ n, ចំនួនគត់ total, ចំនួនគត់ *mask, ចំនួនគត់ classes);
void forward_yolo_layer(const layer l, network net);
void backward_yolo_layer(const layer l, network net);
void resize_yolo_layer(layer *l, ចំនួនគត់ w, ចំនួនគត់ h);
ចំនួនគត់ yolo_num_detections(layer l, float thresh);

#ifdef GPU
void forward_yolo_layer_gpu(const layer l, network net);
void backward_yolo_layer_gpu(layer l, network net);
#endif

#endif
