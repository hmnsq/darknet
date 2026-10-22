#ifndef REORG_LAYER_H
#define REORG_LAYER_H

#अन्तर्भाव "image.h"
#अन्तर्भाव "cuda.h"
#अन्तर्भाव "layer.h"
#अन्तर्भाव "network.h"

layer make_reorg_layer(पूर्णाङ्क batch, पूर्णाङ्क w, पूर्णाङ्क h, पूर्णाङ्क c, पूर्णाङ्क stride, पूर्णाङ्क reverse, पूर्णाङ्क flatten, पूर्णाङ्क extra);
शून्य resize_reorg_layer(layer *l, पूर्णाङ्क w, पूर्णाङ्क h);
शून्य forward_reorg_layer(अचर layer l, network net);
शून्य backward_reorg_layer(अचर layer l, network net);

#ifdef GPU
शून्य forward_reorg_layer_gpu(layer l, network net);
शून्य backward_reorg_layer_gpu(layer l, network net);
#endif

#endif

