#ifndef BATCHNORM_LAYER_H
#define BATCHNORM_LAYER_H

#अन्तर्भाव "image.h"
#अन्तर्भाव "layer.h"
#अन्तर्भाव "network.h"

layer make_batchnorm_layer(पूर्णाङ्क batch, पूर्णाङ्क w, पूर्णाङ्क h, पूर्णाङ्क c);
शून्य forward_batchnorm_layer(layer l, network net);
शून्य backward_batchnorm_layer(layer l, network net);

#ifdef GPU
शून्य forward_batchnorm_layer_gpu(layer l, network net);
शून्य backward_batchnorm_layer_gpu(layer l, network net);
शून्य pull_batchnorm_layer(layer l);
शून्य थप_batchnorm_layer(layer l);
#endif

#endif
