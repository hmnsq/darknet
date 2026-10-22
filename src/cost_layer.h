#ifndef COST_LAYER_H
#define COST_LAYER_H
#অন্তর্ভাৱ "layer.h"
#অন্তর্ভাৱ "network.h"

প্রকারনির্দেশ layer cost_layer;

COST_TYPE get_cost_type(ৱর্ণ *s);
ৱর্ণ *get_cost_string(COST_TYPE a);
cost_layer make_cost_layer(পূর্ণাঙ্ক batch, পূর্ণাঙ্ক inputs, COST_TYPE type, ভিন্ন scale);
শূন্য forward_cost_layer(ধ্রুৱ cost_layer l, network net);
শূন্য backward_cost_layer(ধ্রুৱ cost_layer l, network net);
শূন্য resize_cost_layer(cost_layer *l, পূর্ণাঙ্ক inputs);

#ifdef GPU
শূন্য forward_cost_layer_gpu(cost_layer l, network net);
শূন্য backward_cost_layer_gpu(ধ্রুৱ cost_layer l, network net);
#endif

#endif
