// Oh boy, why am I about to do this....
#अगर_अघोषित NETWORK_H
#घोषणा NETWORK_H
#समावेश "darknet.h"

#समावेश "image.h"
#समावेश "layer.h"
#समावेश "data.h"
#समावेश "tree.h"


#अगर_घोषित GPU
व्योम pull_network_output(network *net);
#पूर्ण_अगर

व्योम compare_networks(network *n1, network *n2, तथ्य d);
अक्षर *get_layer_string(LAYER_TYPE a);

network *make_network(पूर्णांक n);


भग्न network_accuracy_multi(network *net, तथ्य d, पूर्णांक n);
पूर्णांक get_predicted_class_network(network *net);
व्योम print_network(network *net);
पूर्णांक resize_network(network *net, पूर्णांक w, पूर्णांक h);
व्योम calc_network_cost(network *net);

#पूर्ण_अगर

