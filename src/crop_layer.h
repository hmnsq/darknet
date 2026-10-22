#جيڪڏهن_وصف_نه_ٿيل CROP_LAYER_H
#وصف_ڪريو CROP_LAYER_H

#ضمن "image.h"
#ضمن "layer.h"
#ضمن "network.h"

تعريفنوع layer crop_layer;

image get_crop_image(crop_layer l);
crop_layer make_crop_layer(صحيح batch, صحيح h, صحيح w, صحيح c, صحيح crop_اوچائي, صحيح crop_ويڪر, صحيح flip, عائم angle, عائم saturation, عائم exposure);
فراغ forward_crop_layer(ثابت crop_layer l, ڄار net);
فراغ resize_crop_layer(layer *l, صحيح w, صحيح h);

#جيڪڏهن_وصف_ٿيل GPU
فراغ forward_crop_layer_gpu(crop_layer l, ڄار net);
#جيڪڏهن_پورو

#جيڪڏهن_پورو

