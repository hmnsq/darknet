#ifndef SOFTMAX_LAYER_H
#define SOFTMAX_LAYER_H
#அந்தர்பாவ "layer.h"
#அந்தர்பாவ "network.h"

வகைவரை layer softmax_layer;

வெறுமை softmax_array(தசம *உள்ளீடு, முழு n, தசம temp, தசம *output);
softmax_layer make_softmax_layer(முழு batch, முழு inputs, முழு groups);
வெறுமை forward_softmax_layer(மாறிலி softmax_layer l, network net);
வெறுமை backward_softmax_layer(மாறிலி softmax_layer l, network net);

#ifdef GPU
வெறுமை pull_softmax_layer_output(மாறிலி softmax_layer l);
வெறுமை forward_softmax_layer_gpu(மாறிலி softmax_layer l, network net);
வெறுமை backward_softmax_layer_gpu(மாறிலி softmax_layer l, network net);
#endif

#endif
