// Oh boy, why am I about to do this....
#ifndef NETWORK_H
#define NETWORK_H
#അന്തര്ഭാവ "darknet.h"

#അന്തര്ഭാവ "image.h"
#അന്തര്ഭാവ "layer.h"
#അന്തര്ഭാവ "data.h"
#അന്തര്ഭാവ "tree.h"


#ifdef GPU
ശൂന്യം pull_network_output(network *net);
#endif

ശൂന്യം compare_networks(network *n1, network *n2, data d);
അക്ഷരം *get_layer_string(LAYER_TYPE a);

network *make_network(പൂർണ്ണസംഖ്യ n);


ദശാംശം network_accuracy_multi(network *net, data d, പൂർണ്ണസംഖ്യ n);
പൂർണ്ണസംഖ്യ get_predicted_class_network(network *net);
ശൂന്യം print_network(network *net);
പൂർണ്ണസംഖ്യ resize_network(network *net, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h);
ശൂന്യം calc_network_cost(network *net);

#endif

