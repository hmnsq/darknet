#ifndef REGION_LAYER_H
#define REGION_LAYER_H

#அந்தர்பாவ "darknet.h"
#அந்தர்பாவ "layer.h"
#அந்தர்பாவ "network.h"

layer make_region_layer(முழு batch, முழு w, முழு h, முழு n, முழு classes, முழு coords);
வெறுமை forward_region_layer(மாறிலி layer l, network net);
வெறுமை backward_region_layer(மாறிலி layer l, network net);
வெறுமை resize_region_layer(layer *l, முழு w, முழு h);

#ifdef GPU
வெறுமை forward_region_layer_gpu(மாறிலி layer l, network net);
வெறுமை backward_region_layer_gpu(layer l, network net);
#endif

#endif
