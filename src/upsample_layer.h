#ifndef UPSAMPLE_LAYER_H
#define UPSAMPLE_LAYER_H
#अन्तर्भाव "darknet.h"

layer make_upsample_layer(पूर्णांक batch, पूर्णांक w, पूर्णांक h, पूर्णांक c, पूर्णांक stride);
शून्य forward_upsample_layer(स्थिरांक layer l, network net);
शून्य backward_upsample_layer(स्थिरांक layer l, network net);
शून्य resize_upsample_layer(layer *l, पूर्णांक w, पूर्णांक h);

#ifdef GPU
शून्य forward_upsample_layer_gpu(स्थिरांक layer l, network net);
शून्य backward_upsample_layer_gpu(स्थिरांक layer l, network net);
#endif

#endif
