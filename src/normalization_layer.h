#ifndef NORMALIZATION_LAYER_H
#define NORMALIZATION_LAYER_H

#அந்தர்பாவ "image.h"
#அந்தர்பாவ "layer.h"
#அந்தர்பாவ "network.h"

layer make_normalization_layer(முழு batch, முழு w, முழு h, முழு c, முழு size, தசம alpha, தசம beta, தசம kappa);
வெறுமை resize_normalization_layer(layer *layer, முழு h, முழு w);
வெறுமை forward_normalization_layer(மாறிலி layer layer, network net);
வெறுமை backward_normalization_layer(மாறிலி layer layer, network net);
வெறுமை visualize_normalization_layer(layer layer, எழுத்து *window);

#ifdef GPU
வெறுமை forward_normalization_layer_gpu(மாறிலி layer layer, network net);
வெறுமை backward_normalization_layer_gpu(மாறிலி layer layer, network net);
#endif

#endif
