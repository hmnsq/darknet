#यदि_अपरिभाषितम् REGION_LAYER_H
#परिभाषय REGION_LAYER_H

#अन्तर्भाव "darknet.h"
#अन्तर्भाव "layer.h"
#अन्तर्भाव "network.h"

layer make_region_layer(पूर्णाङ्क batch, पूर्णाङ्क w, पूर्णाङ्क h, पूर्णाङ्क n, पूर्णाङ्क classes, पूर्णाङ्क coords);
शून्य forward_region_layer(ध्रुव layer l, जालम् net);
शून्य backward_region_layer(ध्रुव layer l, जालम् net);
शून्य resize_region_layer(layer *l, पूर्णाङ्क w, पूर्णाङ्क h);

#यदि_परिभाषितम् GPU
शून्य forward_region_layer_gpu(ध्रुव layer l, जालम् net);
शून्य backward_region_layer_gpu(layer l, जालम् net);
#यदि_अन्तः

#यदि_अन्तः
