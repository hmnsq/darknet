#ifndef ISEG_LAYER_H
#define ISEG_LAYER_H

#include "darknet.h"
#include "layer.h"
#include "network.h"

layer make_iseg_layer(ຈຳນວນເຕັມ batch, ຈຳນວນເຕັມ w, ຈຳນວນເຕັມ h, ຈຳນວນເຕັມ classes, ຈຳນວນເຕັມ ids);
void forward_iseg_layer(const layer l, network net);
void backward_iseg_layer(const layer l, network net);
void resize_iseg_layer(layer *l, ຈຳນວນເຕັມ w, ຈຳນວນເຕັມ h);
ຈຳນວນເຕັມ iseg_num_detections(layer l, float thresh);

#ifdef GPU
void forward_iseg_layer_gpu(const layer l, network net);
void backward_iseg_layer_gpu(layer l, network net);
#endif

#endif
