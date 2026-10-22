// Oh boy, why am I about to do this....
#ifndef NETWORK_H
#define NETWORK_H
#अन्तर्भाव "darknet.h"

#अन्तर्भाव "image.h"
#अन्तर्भाव "layer.h"
#अन्तर्भाव "data.h"
#अन्तर्भाव "tree.h"


#ifdef GPU
शून्य pull_network_output(network *net);
#endif

शून्य compare_networks(network *n1, network *n2, data d);
वर्ण *get_layer_string(LAYER_TYPE a);

network *make_network(पूर्णाङ्क n);


भिन्न network_accuracy_multi(network *net, data d, पूर्णाङ्क n);
पूर्णाङ्क get_predicted_class_network(network *net);
शून्य print_network(network *net);
पूर्णाङ्क resize_network(network *net, पूर्णाङ्क w, पूर्णाङ्क h);
शून्य calc_network_cost(network *net);

#endif

