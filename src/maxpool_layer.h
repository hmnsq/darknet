#اگر_غیر_تعریف_شدہ MAXPOOL_LAYER_H
#تعریف_کرو MAXPOOL_LAYER_H

#شامل "image.h"
#شامل "cuda.h"
#شامل "layer.h"
#شامل "network.h"

قسمتعریف layer maxpool_layer;

image get_maxpool_image(maxpool_layer l);
maxpool_layer make_maxpool_layer(صحیح batch, صحیح h, صحیح w, صحیح c, صحیح سائز, صحیح stride, صحیح padding);
خالی resize_maxpool_layer(maxpool_layer *l, صحیح w, صحیح h);
خالی forward_maxpool_layer(ثابت maxpool_layer l, جال net);
خالی backward_maxpool_layer(ثابت maxpool_layer l, جال net);

#اگر_تعریف_شدہ GPU
خالی forward_maxpool_layer_gpu(maxpool_layer l, جال net);
خالی backward_maxpool_layer_gpu(maxpool_layer l, جال net);
#اگر_ختم

#اگر_ختم

