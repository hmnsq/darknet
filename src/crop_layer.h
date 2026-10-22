#ifndef CROP_LAYER_H
#define CROP_LAYER_H

#अन्तर्भाव "image.h"
#अन्तर्भाव "layer.h"
#अन्तर्भाव "network.h"

प्रकारव्याख्या layer crop_layer;

image get_crop_image(crop_layer l);
crop_layer make_crop_layer(पूर्णांक batch, पूर्णांक h, पूर्णांक w, पूर्णांक c, पूर्णांक crop_height, पूर्णांक crop_width, पूर्णांक flip, दशांश angle, दशांश saturation, दशांश exposure);
शून्य forward_crop_layer(स्थिरांक crop_layer l, network net);
शून्य resize_crop_layer(layer *l, पूर्णांक w, पूर्णांक h);

#ifdef GPU
शून्य forward_crop_layer_gpu(crop_layer l, network net);
#endif

#endif

