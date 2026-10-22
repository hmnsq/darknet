#ifndef ISEG_LAYER_H
#define ISEG_LAYER_H

#अन्तर्भाव "darknet.h"
#अन्तर्भाव "layer.h"
#अन्तर्भाव "network.h"

layer make_iseg_layer(पूर्णाङ्क batch, पूर्णाङ्क w, पूर्णाङ्क h, पूर्णाङ्क classes, पूर्णाङ्क ids);
शून्य forward_iseg_layer(अचर layer l, network net);
शून्य backward_iseg_layer(अचर layer l, network net);
शून्य resize_iseg_layer(layer *l, पूर्णाङ्क w, पूर्णाङ्क h);
पूर्णाङ्क iseg_अङ्क_detections(layer l, भिन्न thresh);

#ifdef GPU
शून्य forward_iseg_layer_gpu(अचर layer l, network net);
शून्य backward_iseg_layer_gpu(layer l, network net);
#endif

#endif
