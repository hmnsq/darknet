#अगर_अघोषित CROP_LAYER_H
#घोषणा CROP_LAYER_H

#समावेश "image.h"
#समावेश "layer.h"
#समावेश "network.h"

प्रकार layer crop_layer;

image get_crop_image(crop_layer l);
crop_layer make_crop_layer(पूर्णांक batch, पूर्णांक h, पूर्णांक w, पूर्णांक c, पूर्णांक crop_height, पूर्णांक crop_width, पूर्णांक flip, भग्न angle, भग्न saturation, भग्न exposure);
व्योम forward_crop_layer(स्थिर crop_layer l, network net);
व्योम resize_crop_layer(layer *l, पूर्णांक w, पूर्णांक h);

#अगर_घोषित GPU
व्योम forward_crop_layer_gpu(crop_layer l, network net);
#पूर्ण_अगर

#पूर्ण_अगर

