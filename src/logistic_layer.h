#ifndef LOGISTIC_LAYER_H
#define LOGISTIC_LAYER_H
#அந்தர்பாவ "layer.h"
#அந்தர்பாவ "network.h"

layer make_logistic_layer(முழு batch, முழு inputs);
வெறுமை forward_logistic_layer(மாறிலி layer l, network net);
வெறுமை backward_logistic_layer(மாறிலி layer l, network net);

#ifdef GPU
வெறுமை forward_logistic_layer_gpu(மாறிலி layer l, network net);
வெறுமை backward_logistic_layer_gpu(மாறிலி layer l, network net);
#endif

#endif
