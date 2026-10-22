#यदि_अपरिभाषितम् CROP_LAYER_H
#परिभाषय CROP_LAYER_H

#अन्तर्भाव "image.h"
#अन्तर्भाव "layer.h"
#अन्तर्भाव "network.h"

प्रकारनिर्देश layer crop_layer;

image get_crop_image(crop_layer l);
crop_layer make_crop_layer(पूर्णाङ्क batch, पूर्णाङ्क h, पूर्णाङ्क w, पूर्णाङ्क c, पूर्णाङ्क crop_height, पूर्णाङ्क crop_विस्तारः, पूर्णाङ्क flip, भिन्न angle, भिन्न saturation, भिन्न exposure);
शून्य forward_crop_layer(ध्रुव crop_layer l, जालम् net);
शून्य resize_crop_layer(layer *l, पूर्णाङ्क w, पूर्णाङ्क h);

#यदि_परिभाषितम् GPU
शून्य forward_crop_layer_gpu(crop_layer l, जालम् net);
#यदि_अन्तः

#यदि_अन्तः

