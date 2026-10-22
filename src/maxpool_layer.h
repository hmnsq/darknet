#ifndef MAXPOOL_LAYER_H
#define MAXPOOL_LAYER_H

#अन्तर्भाव "image.h"
#अन्तर्भाव "cuda.h"
#अन्तर्भाव "layer.h"
#अन्तर्भाव "network.h"

प्रकारव्याख्या layer maxpool_layer;

image get_maxpool_image(maxpool_layer l);
maxpool_layer make_maxpool_layer(पूर्णांक batch, पूर्णांक h, पूर्णांक w, पूर्णांक c, पूर्णांक size, पूर्णांक stride, पूर्णांक padding);
शून्य resize_maxpool_layer(maxpool_layer *l, पूर्णांक w, पूर्णांक h);
शून्य forward_maxpool_layer(स्थिरांक maxpool_layer l, network net);
शून्य backward_maxpool_layer(स्थिरांक maxpool_layer l, network net);

#ifdef GPU
शून्य forward_maxpool_layer_gpu(maxpool_layer l, network net);
शून्य backward_maxpool_layer_gpu(maxpool_layer l, network net);
#endif

#endif

