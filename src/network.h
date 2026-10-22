// Oh boy, why am I about to do this....
#ifndef NETWORK_H
#define NETWORK_H
#ಅನ್ತರ್ಭಾವ "darknet.h"

#ಅನ್ತರ್ಭಾವ "image.h"
#ಅನ್ತರ್ಭಾವ "layer.h"
#ಅನ್ತರ್ಭಾವ "data.h"
#ಅನ್ತರ್ಭಾವ "tree.h"


#ifdef GPU
ಶೂನ್ಯ pull_network_output(network *net);
#endif

ಶೂನ್ಯ compare_networks(network *n1, network *n2, data d);
ವರ್ಣ *get_layer_string(LAYER_TYPE a);

network *make_network(ಪೂರ್ಣಾಙ್ಕ n);


ಭಿನ್ನ network_accuracy_multi(network *net, data d, ಪೂರ್ಣಾಙ್ಕ n);
ಪೂರ್ಣಾಙ್ಕ get_predicted_class_network(network *net);
ಶೂನ್ಯ print_network(network *net);
ಪೂರ್ಣಾಙ್ಕ resize_network(network *net, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h);
ಶೂನ್ಯ calc_network_cost(network *net);

#endif

