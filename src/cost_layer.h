#ifndef COST_LAYER_H
#define COST_LAYER_H
#ضمن "layer.h"
#ضمن "network.h"

تعريفنوع layer cost_layer;

COST_TYPE get_cost_type(حرف *s);
حرف *get_cost_string(COST_TYPE a);
cost_layer make_cost_layer(صحيح batch, صحيح inputs, COST_TYPE type, عائم scale);
فراغ forward_cost_layer(ثابت cost_layer l, network net);
فراغ backward_cost_layer(ثابت cost_layer l, network net);
فراغ resize_cost_layer(cost_layer *l, صحيح inputs);

#ifdef GPU
فراغ forward_cost_layer_gpu(cost_layer l, network net);
فراغ backward_cost_layer_gpu(ثابت cost_layer l, network net);
#endif

#endif
