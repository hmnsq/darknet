#ifndef ISEG_LAYER_H
#define ISEG_LAYER_H

#include "darknet.h"
#include "layer.h"
#include "network.h"

layer make_iseg_layer(ចំនួនគត់ batch, ចំនួនគត់ w, ចំនួនគត់ h, ចំនួនគត់ classes, ចំនួនគត់ ids);
void forward_iseg_layer(const layer l, network net);
void backward_iseg_layer(const layer l, network net);
void resize_iseg_layer(layer *l, ចំនួនគត់ w, ចំនួនគត់ h);
ចំនួនគត់ iseg_num_detections(layer l, float thresh);

#ifdef GPU
void forward_iseg_layer_gpu(const layer l, network net);
void backward_iseg_layer_gpu(layer l, network net);
#endif

#endif
