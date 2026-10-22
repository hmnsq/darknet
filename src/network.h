// Oh boy, why am I about to do this....
#ifndef NETWORK_H
#define NETWORK_H
#ଅନ୍ତର୍ଭାଵ "darknet.h"

#ଅନ୍ତର୍ଭାଵ "image.h"
#ଅନ୍ତର୍ଭାଵ "layer.h"
#ଅନ୍ତର୍ଭାଵ "data.h"
#ଅନ୍ତର୍ଭାଵ "tree.h"


#ifdef GPU
ଶୂନ୍ୟ pull_network_output(network *net);
#endif

ଶୂନ୍ୟ compare_networks(network *n1, network *n2, data d);
ଅକ୍ଷର *get_layer_string(LAYER_TYPE a);

network *make_network(ପୂର୍ଣ୍ଣସଂଖ୍ୟା n);


ଦଶମିକ network_accuracy_multi(network *net, data d, ପୂର୍ଣ୍ଣସଂଖ୍ୟା n);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା get_predicted_class_network(network *net);
ଶୂନ୍ୟ print_network(network *net);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା resize_network(network *net, ପୂର୍ଣ୍ଣସଂଖ୍ୟା w, ପୂର୍ଣ୍ଣସଂଖ୍ୟା h);
ଶୂନ୍ୟ calc_network_cost(network *net);

#endif

