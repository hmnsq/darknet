#ifndef COST_LAYER_H
#define COST_LAYER_H
#অন্তর্ভাব "layer.h"
#অন্তর্ভাব "network.h"

প্রকারলেখ layer cost_layer;

COST_TYPE get_cost_type(অক্ষর *s);
অক্ষর *get_cost_string(COST_TYPE a);
cost_layer make_cost_layer(পূর্ণসংখ্যা batch, পূর্ণসংখ্যা inputs, COST_TYPE type, দশমিক scale);
শূন্য forward_cost_layer(ধ্রুবক cost_layer l, network net);
শূন্য backward_cost_layer(ধ্রুবক cost_layer l, network net);
শূন্য resize_cost_layer(cost_layer *l, পূর্ণসংখ্যা inputs);

#ifdef GPU
শূন্য forward_cost_layer_gpu(cost_layer l, network net);
শূন্য backward_cost_layer_gpu(ধ্রুবক cost_layer l, network net);
#endif

#endif
