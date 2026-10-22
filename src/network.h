// Oh boy, why am I about to do this....
#ifndef NETWORK_H
#define NETWORK_H
#અન્તર્ભાવ "darknet.h"

#અન્તર્ભાવ "image.h"
#અન્તર્ભાવ "layer.h"
#અન્તર્ભાવ "data.h"
#અન્તર્ભાવ "tree.h"


#ifdef GPU
શૂન્ય pull_network_output(network *net);
#endif

શૂન્ય compare_networks(network *n1, network *n2, data d);
અક્ષર *get_layer_string(LAYER_TYPE a);

network *make_network(પૂર્ણાંક n);


દશાંશ network_accuracy_multi(network *net, data d, પૂર્ણાંક n);
પૂર્ણાંક get_predicted_class_network(network *net);
શૂન્ય print_network(network *net);
પૂર્ણાંક resize_network(network *net, પૂર્ણાંક w, પૂર્ણાંક h);
શૂન્ય calc_network_cost(network *net);

#endif

