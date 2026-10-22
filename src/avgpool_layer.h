#اگر_غیر_تعریف_شدہ AVGPOOL_LAYER_H
#تعریف_کرو AVGPOOL_LAYER_H

#شامل "image.h"
#شامل "cuda.h"
#شامل "layer.h"
#شامل "network.h"

قسمتعریف layer avgpool_layer;

image get_avgpool_image(avgpool_layer l);
avgpool_layer make_avgpool_layer(صحیح batch, صحیح w, صحیح h, صحیح c);
خالی resize_avgpool_layer(avgpool_layer *l, صحیح w, صحیح h);
خالی forward_avgpool_layer(ثابت avgpool_layer l, جال net);
خالی backward_avgpool_layer(ثابت avgpool_layer l, جال net);

#اگر_تعریف_شدہ GPU
خالی forward_avgpool_layer_gpu(avgpool_layer l, جال net);
خالی backward_avgpool_layer_gpu(avgpool_layer l, جال net);
#اگر_ختم

#اگر_ختم

