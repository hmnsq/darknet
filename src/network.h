// Oh boy, why am I about to do this....
#ifndef NETWORK_H
#define NETWORK_H
#شامل "darknet.h"

#شامل "image.h"
#شامل "layer.h"
#شامل "data.h"
#شامل "tree.h"


#ifdef GPU
تهی pull_network_output(network *net);
#endif

تهی compare_networks(network *n1, network *n2, data d);
نویسه *get_layer_string(LAYER_TYPE a);

network *make_network(صحیح n);


اعشاری network_accuracy_multi(network *net, data d, صحیح n);
صحیح get_predicted_class_network(network *net);
تهی print_network(network *net);
صحیح resize_network(network *net, صحیح w, صحیح h);
تهی calc_network_cost(network *net);

#endif

