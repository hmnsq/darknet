#ifndef SHORTCUT_LAYER_H
#define SHORTCUT_LAYER_H

#अन्तर्भाव "layer.h"
#अन्तर्भाव "network.h"

layer make_shortcut_layer(पूर्णाङ्क batch, पूर्णाङ्क index, पूर्णाङ्क w, पूर्णाङ्क h, पूर्णाङ्क c, पूर्णाङ्क w2, पूर्णाङ्क h2, पूर्णाङ्क c2);
शून्य forward_shortcut_layer(ध्रुव layer l, network net);
शून्य backward_shortcut_layer(ध्रुव layer l, network net);
शून्य resize_shortcut_layer(layer *l, पूर्णाङ्क w, पूर्णाङ्क h);

#ifdef GPU
शून्य forward_shortcut_layer_gpu(ध्रुव layer l, network net);
शून्य backward_shortcut_layer_gpu(ध्रुव layer l, network net);
#endif

#endif
