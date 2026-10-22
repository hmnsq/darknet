#ifndef REORG_LAYER_H
#define REORG_LAYER_H

#अन्तर्भाव "image.h"
#अन्तर्भाव "cuda.h"
#अन्तर्भाव "layer.h"
#अन्तर्भाव "network.h"

layer make_reorg_layer(पूर्णांक batch, पूर्णांक w, पूर्णांक h, पूर्णांक c, पूर्णांक stride, पूर्णांक reverse, पूर्णांक flatten, पूर्णांक extra);
शून्य resize_reorg_layer(layer *l, पूर्णांक w, पूर्णांक h);
शून्य forward_reorg_layer(स्थिरांक layer l, network net);
शून्य backward_reorg_layer(स्थिरांक layer l, network net);

#ifdef GPU
शून्य forward_reorg_layer_gpu(layer l, network net);
शून्य backward_reorg_layer_gpu(layer l, network net);
#endif

#endif

