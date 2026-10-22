#ifndef COST_LAYER_H
#define COST_LAYER_H
#include "layer.h"
#include "network.h"

typedef layer cost_layer;

COST_TYPE get_cost_type(var_na *s);
var_na *get_cost_string(COST_TYPE a);
cost_layer make_cost_layer(ကိန်းပြည့် batch, ကိန်းပြည့် inputs, COST_TYPE type, float scale);
void forward_cost_layer(const cost_layer l, network net);
void backward_cost_layer(const cost_layer l, network net);
void resize_cost_layer(cost_layer *l, ကိန်းပြည့် inputs);

#ifdef GPU
void forward_cost_layer_gpu(cost_layer l, network net);
void backward_cost_layer_gpu(const cost_layer l, network net);
#endif

#endif
