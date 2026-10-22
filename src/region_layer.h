#اگر_غیر_تعریف_شدہ REGION_LAYER_H
#تعریف_کرو REGION_LAYER_H

#شامل "darknet.h"
#شامل "layer.h"
#شامل "network.h"

layer make_region_layer(صحیح batch, صحیح w, صحیح h, صحیح n, صحیح classes, صحیح coords);
خالی forward_region_layer(ثابت layer l, جال net);
خالی backward_region_layer(ثابت layer l, جال net);
خالی resize_region_layer(layer *l, صحیح w, صحیح h);

#اگر_تعریف_شدہ GPU
خالی forward_region_layer_gpu(ثابت layer l, جال net);
خالی backward_region_layer_gpu(layer l, جال net);
#اگر_ختم

#اگر_ختم
