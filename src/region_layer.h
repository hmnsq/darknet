#ifndef REGION_LAYER_H
#define REGION_LAYER_H

#अन्तर्भाव "darknet.h"
#अन्तर्भाव "layer.h"
#अन्तर्भाव "network.h"

layer make_region_layer(पूर्णांक batch, पूर्णांक w, पूर्णांक h, पूर्णांक n, पूर्णांक classes, पूर्णांक coords);
शून्य forward_region_layer(स्थिरांक layer l, network net);
शून्य backward_region_layer(स्थिरांक layer l, network net);
शून्य resize_region_layer(layer *l, पूर्णांक w, पूर्णांक h);

#ifdef GPU
शून्य forward_region_layer_gpu(स्थिरांक layer l, network net);
शून्य backward_region_layer_gpu(layer l, network net);
#endif

#endif
