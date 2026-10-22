// Oh boy, why am I about to do this....
#ifndef NETWORK_H
#define NETWORK_H
#அந்தர்பாவ "darknet.h"

#அந்தர்பாவ "image.h"
#அந்தர்பாவ "layer.h"
#அந்தர்பாவ "data.h"
#அந்தர்பாவ "tree.h"


#ifdef GPU
வெறுமை pull_network_output(network *net);
#endif

வெறுமை compare_networks(network *n1, network *n2, data d);
எழுத்து *get_layer_string(LAYER_TYPE a);

network *make_network(முழு n);


தசம network_accuracy_multi(network *net, data d, முழு n);
முழு get_predicted_class_network(network *net);
வெறுமை print_network(network *net);
முழு resize_network(network *net, முழு w, முழு h);
வெறுமை calc_network_cost(network *net);

#endif

