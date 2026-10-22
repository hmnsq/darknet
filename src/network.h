// Oh boy, why am I about to do this....
#ifndef NETWORK_H
#define NETWORK_H
#ضمن "darknet.h"

#ضمن "image.h"
#ضمن "layer.h"
#ضمن "data.h"
#ضمن "tree.h"


#ifdef GPU
فراغ pull_network_output(network *net);
#endif

فراغ compare_networks(network *n1, network *n2, data d);
حرف *get_layer_string(LAYER_TYPE a);

network *make_network(صحيح n);


عائم network_accuracy_multi(network *net, data d, صحيح n);
صحيح get_predicted_class_network(network *net);
فراغ print_network(network *net);
صحيح resize_network(network *net, صحيح w, صحيح h);
فراغ calc_network_cost(network *net);

#endif

