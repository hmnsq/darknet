// Oh boy, why am I about to do this....
#यदि_अपरिभाषितम् जालम्_H
#परिभाषय जालम्_H
#अन्तर्भाव "darknet.h"

#अन्तर्भाव "image.h"
#अन्तर्भाव "layer.h"
#अन्तर्भाव "data.h"
#अन्तर्भाव "tree.h"


#यदि_परिभाषितम् GPU
शून्य pull_जालम्_output(जालम् *net);
#यदि_अन्तः

शून्य compare_networks(जालम् *n1, जालम् *n2, दत्त d);
वर्ण *get_layer_string(LAYER_TYPE a);

जालम् *make_जालम्(पूर्णाङ्क n);


भिन्न जालम्_accuracy_multi(जालम् *net, दत्त d, पूर्णाङ्क n);
पूर्णाङ्क get_predicted_class_जालम्(जालम् *net);
शून्य print_जालम्(जालम् *net);
पूर्णाङ्क resize_जालम्(जालम् *net, पूर्णाङ्क w, पूर्णाङ्क h);
शून्य calc_जालम्_cost(जालम् *net);

#यदि_अन्तः

