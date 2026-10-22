#ifndef MAXPOOL_LAYER_H
#define MAXPOOL_LAYER_H

#अन्तर्भाव "image.h"
#अन्तर्भाव "cuda.h"
#अन्तर्भाव "layer.h"
#अन्तर्भाव "network.h"

प्रकारनिर्देश layer maxpool_layer;

image get_maxpool_image(maxpool_layer l);
maxpool_layer make_maxpool_layer(पूर्णाङ्क batch, पूर्णाङ्क h, पूर्णाङ्क w, पूर्णाङ्क c, पूर्णाङ्क size, पूर्णाङ्क stride, पूर्णाङ्क padding);
शून्य resize_maxpool_layer(maxpool_layer *l, पूर्णाङ्क w, पूर्णाङ्क h);
शून्य forward_maxpool_layer(अचर maxpool_layer l, network net);
शून्य backward_maxpool_layer(अचर maxpool_layer l, network net);

#ifdef GPU
शून्य forward_maxpool_layer_gpu(maxpool_layer l, network net);
शून्य backward_maxpool_layer_gpu(maxpool_layer l, network net);
#endif

#endif

