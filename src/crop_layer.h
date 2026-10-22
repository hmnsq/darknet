#اگر_غیر_تعریف_شدہ CROP_LAYER_H
#تعریف_کرو CROP_LAYER_H

#شامل "image.h"
#شامل "layer.h"
#شامل "network.h"

قسمتعریف layer crop_layer;

image get_crop_image(crop_layer l);
crop_layer make_crop_layer(صحیح batch, صحیح h, صحیح w, صحیح c, صحیح crop_اونچائی, صحیح crop_چوڑائی, صحیح flip, اعشاری angle, اعشاری saturation, اعشاری exposure);
خالی forward_crop_layer(ثابت crop_layer l, جال net);
خالی resize_crop_layer(layer *l, صحیح w, صحیح h);

#اگر_تعریف_شدہ GPU
خالی forward_crop_layer_gpu(crop_layer l, جال net);
#اگر_ختم

#اگر_ختم

