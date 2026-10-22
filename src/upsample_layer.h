#ifndef UPSAMPLE_LAYER_H
#define UPSAMPLE_LAYER_H
#include "darknet.h"

layer make_upsample_layer(ຈຳນວນເຕັມ batch, ຈຳນວນເຕັມ w, ຈຳນວນເຕັມ h, ຈຳນວນເຕັມ c, ຈຳນວນເຕັມ stride);
void forward_upsample_layer(const layer l, network net);
void backward_upsample_layer(const layer l, network net);
void resize_upsample_layer(layer *l, ຈຳນວນເຕັມ w, ຈຳນວນເຕັມ h);

#ifdef GPU
void forward_upsample_layer_gpu(const layer l, network net);
void backward_upsample_layer_gpu(const layer l, network net);
#endif

#endif
