#ifndef COST_LAYER_H
#define COST_LAYER_H
#அந்தர்பாவ "layer.h"
#அந்தர்பாவ "network.h"

வகைவரை layer cost_layer;

COST_TYPE get_cost_type(எழுத்து *s);
எழுத்து *get_cost_string(COST_TYPE a);
cost_layer make_cost_layer(முழு batch, முழு inputs, COST_TYPE type, தசம scale);
வெறுமை forward_cost_layer(மாறிலி cost_layer l, network net);
வெறுமை backward_cost_layer(மாறிலி cost_layer l, network net);
வெறுமை resize_cost_layer(cost_layer *l, முழு inputs);

#ifdef GPU
வெறுமை forward_cost_layer_gpu(cost_layer l, network net);
வெறுமை backward_cost_layer_gpu(மாறிலி cost_layer l, network net);
#endif

#endif
