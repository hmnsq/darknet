#ifndef ISEG_LAYER_H
#define ISEG_LAYER_H

#अन्तर्भाव "darknet.h"
#अन्तर्भाव "layer.h"
#अन्तर्भाव "network.h"

layer make_iseg_layer(पूर्णांक batch, पूर्णांक w, पूर्णांक h, पूर्णांक classes, पूर्णांक ids);
शून्य forward_iseg_layer(स्थिरांक layer l, network net);
शून्य backward_iseg_layer(स्थिरांक layer l, network net);
शून्य resize_iseg_layer(layer *l, पूर्णांक w, पूर्णांक h);
पूर्णांक iseg_अंक_detections(layer l, दशांश thresh);

#ifdef GPU
शून्य forward_iseg_layer_gpu(स्थिरांक layer l, network net);
शून्य backward_iseg_layer_gpu(layer l, network net);
#endif

#endif
