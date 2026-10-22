#ifndef COST_LAYER_H
#define COST_LAYER_H
#ଅନ୍ତର୍ଭାଵ "layer.h"
#ଅନ୍ତର୍ଭାଵ "network.h"

ପ୍ରକାରବ୍ୟାଖ୍ୟା layer cost_layer;

COST_TYPE get_cost_type(ଅକ୍ଷର *s);
ଅକ୍ଷର *get_cost_string(COST_TYPE a);
cost_layer make_cost_layer(ପୂର୍ଣ୍ଣସଂଖ୍ୟା batch, ପୂର୍ଣ୍ଣସଂଖ୍ୟା inputs, COST_TYPE type, ଦଶମିକ scale);
ଶୂନ୍ୟ forward_cost_layer(ଅଚଳ cost_layer l, network net);
ଶୂନ୍ୟ backward_cost_layer(ଅଚଳ cost_layer l, network net);
ଶୂନ୍ୟ resize_cost_layer(cost_layer *l, ପୂର୍ଣ୍ଣସଂଖ୍ୟା inputs);

#ifdef GPU
ଶୂନ୍ୟ forward_cost_layer_gpu(cost_layer l, network net);
ଶୂନ୍ୟ backward_cost_layer_gpu(ଅଚଳ cost_layer l, network net);
#endif

#endif
