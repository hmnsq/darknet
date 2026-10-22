#ifndef AVGPOOL_LAYER_H
#define AVGPOOL_LAYER_H

#अन्तर्भाव "image.h"
#अन्तर्भाव "cuda.h"
#अन्तर्भाव "layer.h"
#अन्तर्भाव "network.h"

प्रकारनिर्देश layer avgpool_layer;

image get_avgpool_image(avgpool_layer l);
avgpool_layer make_avgpool_layer(पूर्णाङ्क batch, पूर्णाङ्क w, पूर्णाङ्क h, पूर्णाङ्क c);
शून्य resize_avgpool_layer(avgpool_layer *l, पूर्णाङ्क w, पूर्णाङ्क h);
शून्य forward_avgpool_layer(ध्रुव avgpool_layer l, network net);
शून्य backward_avgpool_layer(ध्रुव avgpool_layer l, network net);

#ifdef GPU
शून्य forward_avgpool_layer_gpu(avgpool_layer l, network net);
शून्य backward_avgpool_layer_gpu(avgpool_layer l, network net);
#endif

#endif

