// Oh boy, why am I about to do this....
#ifndef NETWORK_H
#define NETWORK_H
#include "darknet.h"

#include "image.h"
#include "layer.h"
#include "data.h"
#include "tree.h"


#ifdef GPU
void pull_network_output(network *net);
#endif

void compare_networks(network *n1, network *n2, data d);
var_na *get_layer_string(LAYER_TYPE a);

network *make_network(පූර්ණ_සංඛ්‍යා n);


float network_accuracy_multi(network *net, data d, පූර්ණ_සංඛ්‍යා n);
පූර්ණ_සංඛ්‍යා get_predicted_class_network(network *net);
void print_network(network *net);
පූර්ණ_සංඛ්‍යා resize_network(network *net, පූර්ණ_සංඛ්‍යා w, පූර්ණ_සංඛ්‍යා h);
void calc_network_cost(network *net);

#endif

