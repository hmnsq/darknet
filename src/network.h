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
अक्षर *get_layer_string(LAYER_TYPE a);

network *make_network(पूर्णांक n);


दशांश network_accuracy_multi(network *net, data d, पूर्णांक n);
पूर्णांक get_predicted_class_network(network *net);
शून्य print_network(network *net);
पूर्णांक resize_network(network *net, पूर्णांक w, पूर्णांक h);
शून्य calc_network_cost(network *net);

#endif

