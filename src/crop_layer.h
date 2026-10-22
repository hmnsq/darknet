#جے_بنا_تعریف CROP_LAYER_H
#تعریف_کرو CROP_LAYER_H

#ضمن "image.h"
#ضمن "layer.h"
#ضمن "network.h"

تعريفنوع layer crop_layer;

image get_crop_image(crop_layer l);
crop_layer make_crop_layer(صحيح batch, صحيح h, صحيح w, صحيح c, صحيح crop_اچائی, صحيح crop_چوڑائی, صحيح flip, عائم angle, عائم saturation, عائم exposure);
فراغ forward_crop_layer(ثابت crop_layer l, جال net);
فراغ resize_crop_layer(layer *l, صحيح w, صحيح h);

#جے_تعریف_شدہ GPU
فراغ forward_crop_layer_gpu(crop_layer l, جال net);
#جے_انت

#جے_انت

