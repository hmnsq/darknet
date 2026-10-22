#ifndef DROPOUT_LAYER_H
#define DROPOUT_LAYER_H

#अन्तर्भाव "layer.h"
#अन्तर्भाव "network.h"

प्रकारव्याख्या layer dropout_layer;

dropout_layer make_dropout_layer(पूर्णांक batch, पूर्णांक inputs, दशांश probability);

शून्य forward_dropout_layer(dropout_layer l, network net);
शून्य backward_dropout_layer(dropout_layer l, network net);
शून्य resize_dropout_layer(dropout_layer *l, पूर्णांक inputs);

#ifdef GPU
शून्य forward_dropout_layer_gpu(dropout_layer l, network net);
शून्य backward_dropout_layer_gpu(dropout_layer l, network net);

#endif
#endif
