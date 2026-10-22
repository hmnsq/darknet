#اگر_غیر_تعریف_شدہ SHORTCUT_LAYER_H
#تعریف_کرو SHORTCUT_LAYER_H

#شامل "layer.h"
#شامل "network.h"

layer make_shortcut_layer(صحیح batch, صحیح اشاریہ, صحیح w, صحیح h, صحیح c, صحیح w2, صحیح h2, صحیح c2);
خالی forward_shortcut_layer(ثابت layer l, جال net);
خالی backward_shortcut_layer(ثابت layer l, جال net);
خالی resize_shortcut_layer(layer *l, صحیح w, صحیح h);

#اگر_تعریف_شدہ GPU
خالی forward_shortcut_layer_gpu(ثابت layer l, جال net);
خالی backward_shortcut_layer_gpu(ثابت layer l, جال net);
#اگر_ختم

#اگر_ختم
