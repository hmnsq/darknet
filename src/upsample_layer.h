#ifndef UPSAMPLE_LAYER_H
#define UPSAMPLE_LAYER_H
#अन्तर्भाव "darknet.h"

layer make_upsample_layer(पूर्णाङ्क batch, पूर्णाङ्क w, पूर्णाङ्क h, पूर्णाङ्क c, पूर्णाङ्क stride);
शून्य forward_upsample_layer(अचर layer l, network net);
शून्य backward_upsample_layer(अचर layer l, network net);
शून्य resize_upsample_layer(layer *l, पूर्णाङ्क w, पूर्णाङ्क h);

#ifdef GPU
शून्य forward_upsample_layer_gpu(अचर layer l, network net);
शून्य backward_upsample_layer_gpu(अचर layer l, network net);
#endif

#endif
