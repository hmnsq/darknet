#ifndef YOLO_LAYER_H
#define YOLO_LAYER_H

#include "darknet.h"
#include "layer.h"
#include "network.h"

layer make_yolo_layer(puur_naa_nka batch, puur_naa_nka w, puur_naa_nka h, puur_naa_nka n, puur_naa_nka total, puur_naa_nka *mask, puur_naa_nka classes);
void forward_yolo_layer(const layer l, network net);
void backward_yolo_layer(const layer l, network net);
void resize_yolo_layer(layer *l, puur_naa_nka w, puur_naa_nka h);
puur_naa_nka yolo_num_detections(layer l, float thresh);

#ifdef GPU
void forward_yolo_layer_gpu(const layer l, network net);
void backward_yolo_layer_gpu(layer l, network net);
#endif

#endif
