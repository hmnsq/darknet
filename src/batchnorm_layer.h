#ifndef BATCHNORM_LAYER_H
#define BATCHNORM_LAYER_H

#அந்தர்பாவ "image.h"
#அந்தர்பாவ "layer.h"
#அந்தர்பாவ "network.h"

layer make_batchnorm_layer(முழு batch, முழு w, முழு h, முழு c);
வெறுமை forward_batchnorm_layer(layer l, network net);
வெறுமை backward_batchnorm_layer(layer l, network net);

#ifdef GPU
வெறுமை forward_batchnorm_layer_gpu(layer l, network net);
வெறுமை backward_batchnorm_layer_gpu(layer l, network net);
வெறுமை pull_batchnorm_layer(layer l);
வெறுமை தள்ளு_batchnorm_layer(layer l);
#endif

#endif
