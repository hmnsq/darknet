#ifndef L2NORM_LAYER_H
#define L2NORM_LAYER_H
#அந்தர்பாவ "layer.h"
#அந்தர்பாவ "network.h"

layer make_l2norm_layer(முழு batch, முழு inputs);
வெறுமை forward_l2norm_layer(மாறிலி layer l, network net);
வெறுமை backward_l2norm_layer(மாறிலி layer l, network net);

#ifdef GPU
வெறுமை forward_l2norm_layer_gpu(மாறிலி layer l, network net);
வெறுமை backward_l2norm_layer_gpu(மாறிலி layer l, network net);
#endif

#endif
