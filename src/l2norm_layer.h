#ifndef L2NORM_LAYER_H
#define L2NORM_LAYER_H
#अन्तर्भाव "layer.h"
#अन्तर्भाव "network.h"

layer make_l2norm_layer(पूर्णाङ्क batch, पूर्णाङ्क inputs);
शून्य forward_l2norm_layer(ध्रुव layer l, network net);
शून्य backward_l2norm_layer(ध्रुव layer l, network net);

#ifdef GPU
शून्य forward_l2norm_layer_gpu(ध्रुव layer l, network net);
शून्य backward_l2norm_layer_gpu(ध्रुव layer l, network net);
#endif

#endif
