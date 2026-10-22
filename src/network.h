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

network *make_network(จำนวนเต็ม n);


float network_accuracy_multi(network *net, data d, จำนวนเต็ม n);
จำนวนเต็ม get_predicted_class_network(network *net);
void print_network(network *net);
จำนวนเต็ม resize_network(network *net, จำนวนเต็ม w, จำนวนเต็ม h);
void calc_network_cost(network *net);

#endif

