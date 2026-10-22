// Oh boy, why am I about to do this....
#ifndef NETWORK_H
#define NETWORK_H
#অন্তর্ভাৱ "darknet.h"

#অন্তর্ভাৱ "image.h"
#অন্তর্ভাৱ "layer.h"
#অন্তর্ভাৱ "data.h"
#অন্তর্ভাৱ "tree.h"


#ifdef GPU
শূন্য pull_network_output(network *net);
#endif

শূন্য compare_networks(network *n1, network *n2, data d);
ৱর্ণ *get_layer_string(LAYER_TYPE a);

network *make_network(পূর্ণাঙ্ক n);


ভিন্ন network_accuracy_multi(network *net, data d, পূর্ণাঙ্ক n);
পূর্ণাঙ্ক get_predicted_class_network(network *net);
শূন্য print_network(network *net);
পূর্ণাঙ্ক resize_network(network *net, পূর্ণাঙ্ক w, পূর্ণাঙ্ক h);
শূন্য calc_network_cost(network *net);

#endif

