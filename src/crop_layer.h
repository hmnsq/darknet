#که_نه_تعریف_شوی CROP_LAYER_H
#تعریف_کړه CROP_LAYER_H

#ضمن "image.h"
#ضمن "layer.h"
#ضمن "network.h"

تعريفنوع layer crop_layer;

image get_crop_image(crop_layer l);
crop_layer make_crop_layer(صحيح batch, صحيح h, صحيح w, صحيح c, صحيح crop_لوړوالی, صحيح crop_پلنوالی, صحيح flip, عائم angle, عائم saturation, عائم exposure);
فراغ forward_crop_layer(ثابت crop_layer l, شبکه net);
فراغ resize_crop_layer(layer *l, صحيح w, صحيح h);

#که_تعریف_شوی GPU
فراغ forward_crop_layer_gpu(crop_layer l, شبکه net);
#د_که_پای

#د_که_پای

