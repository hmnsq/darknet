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
char *get_layer_string(LAYER_TYPE a);

network *make_network(целое n);


float network_accuracy_multi(network *net, data d, целое n);
целое get_predicted_class_network(network *net);
void print_network(network *net);
целое resize_network(network *net, целое w, целое h);
void calc_network_cost(network *net);

#endif

