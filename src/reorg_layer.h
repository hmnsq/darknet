#اگر_غیر_تعریف_شدہ REORG_LAYER_H
#تعریف_کرو REORG_LAYER_H

#شامل "image.h"
#شامل "cuda.h"
#شامل "layer.h"
#شامل "network.h"

layer make_reorg_layer(صحیح batch, صحیح w, صحیح h, صحیح c, صحیح stride, صحیح الٹ, صحیح flatten, صحیح extra);
خالی resize_reorg_layer(layer *l, صحیح w, صحیح h);
خالی forward_reorg_layer(ثابت layer l, جال net);
خالی backward_reorg_layer(ثابت layer l, جال net);

#اگر_تعریف_شدہ GPU
خالی forward_reorg_layer_gpu(layer l, جال net);
خالی backward_reorg_layer_gpu(layer l, جال net);
#اگر_ختم

#اگر_ختم

