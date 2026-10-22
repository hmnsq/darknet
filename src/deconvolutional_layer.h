#ifndef DECONVOLUTIONAL_LAYER_H
#define DECONVOLUTIONAL_LAYER_H

#அந்தர்பாவ "cuda.h"
#அந்தர்பாவ "image.h"
#அந்தர்பாவ "activations.h"
#அந்தர்பாவ "layer.h"
#அந்தர்பாவ "network.h"

#ifdef GPU
வெறுமை forward_deconvolutional_layer_gpu(layer l, network net);
வெறுமை backward_deconvolutional_layer_gpu(layer l, network net);
வெறுமை update_deconvolutional_layer_gpu(layer l, update_args a);
வெறுமை தள்ளு_deconvolutional_layer(layer l);
வெறுமை pull_deconvolutional_layer(layer l);
#endif

layer make_deconvolutional_layer(முழு batch, முழு h, முழு w, முழு c, முழு n, முழு size, முழு stride, முழு padding, ACTIVATION activation, முழு batch_normalize, முழு adam);
வெறுமை resize_deconvolutional_layer(layer *l, முழு h, முழு w);
வெறுமை forward_deconvolutional_layer(மாறிலி layer l, network net);
வெறுமை update_deconvolutional_layer(layer l, update_args a);
வெறுமை backward_deconvolutional_layer(layer l, network net);

#endif

