#ifndef COST_LAYER_H
#define COST_LAYER_H
#شامل "layer.h"
#شامل "network.h"

تعریفنوع layer cost_layer;

COST_TYPE get_cost_type(نویسه *s);
نویسه *get_cost_string(COST_TYPE a);
cost_layer make_cost_layer(صحیح batch, صحیح inputs, COST_TYPE type, اعشاری scale);
تهی forward_cost_layer(ثابت cost_layer l, network net);
تهی backward_cost_layer(ثابت cost_layer l, network net);
تهی resize_cost_layer(cost_layer *l, صحیح inputs);

#ifdef GPU
تهی forward_cost_layer_gpu(cost_layer l, network net);
تهی backward_cost_layer_gpu(ثابت cost_layer l, network net);
#endif

#endif
