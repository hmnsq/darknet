#اگر_غیر_تعریف_شدہ UPSAMPLE_LAYER_H
#تعریف_کرو UPSAMPLE_LAYER_H
#شامل "darknet.h"

layer make_upsample_layer(صحیح batch, صحیح w, صحیح h, صحیح c, صحیح stride);
خالی forward_upsample_layer(ثابت layer l, جال net);
خالی backward_upsample_layer(ثابت layer l, جال net);
خالی resize_upsample_layer(layer *l, صحیح w, صحیح h);

#اگر_تعریف_شدہ GPU
خالی forward_upsample_layer_gpu(ثابت layer l, جال net);
خالی backward_upsample_layer_gpu(ثابت layer l, جال net);
#اگر_ختم

#اگر_ختم
