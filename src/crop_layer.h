#اگر_تعریف_نشده CROP_LAYER_H
#تعریف_کن CROP_LAYER_H

#ضمن "image.h"
#ضمن "layer.h"
#ضمن "network.h"

تعريفنوع layer crop_layer;

image get_crop_image(crop_layer l);
crop_layer make_crop_layer(صحيح batch, صحيح h, صحيح w, صحيح c, صحيح crop_ارتفاع, صحيح crop_عرض, صحيح flip, عائم angle, عائم saturation, عائم exposure);
فراغ forward_crop_layer(ثابت crop_layer l, شبکه net);
فراغ resize_crop_layer(layer *l, صحيح w, صحيح h);

#اگر_تعریف_شده GPU
فراغ forward_crop_layer_gpu(crop_layer l, شبکه net);
#پایان_اگر

#پایان_اگر

