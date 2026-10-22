// Oh boy, why am I about to do this....
#ifndef NETWORK_H
#define NETWORK_H
#অন্তর্ভাব "darknet.h"

#অন্তর্ভাব "image.h"
#অন্তর্ভাব "layer.h"
#অন্তর্ভাব "data.h"
#অন্তর্ভাব "tree.h"


#ifdef GPU
শূন্য pull_network_output(network *net);
#endif

শূন্য compare_networks(network *n1, network *n2, তথ্য d);
অক্ষর *get_layer_string(LAYER_TYPE a);

network *make_network(পূর্ণসংখ্যা n);


দশমিক network_accuracy_multi(network *net, তথ্য d, পূর্ণসংখ্যা n);
পূর্ণসংখ্যা get_predicted_class_network(network *net);
শূন্য print_network(network *net);
পূর্ণসংখ্যা resize_network(network *net, পূর্ণসংখ্যা w, পূর্ণসংখ্যা h);
শূন্য calc_network_cost(network *net);

#endif

