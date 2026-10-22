#ifndef CROP_LAYER_H
#define CROP_LAYER_H

#ضمن "image.h"
#ضمن "layer.h"
#ضمن "network.h"

تعريفنوع layer crop_layer;

image get_crop_image(crop_layer l);
crop_layer make_crop_layer(صحيح batch, صحيح h, صحيح w, صحيح c, صحيح crop_height, صحيح crop_width, صحيح flip, عائم angle, عائم saturation, عائم exposure);
فراغ forward_crop_layer(ثابت crop_layer l, network net);
فراغ resize_crop_layer(layer *l, صحيح w, صحيح h);

#ifdef GPU
فراغ forward_crop_layer_gpu(crop_layer l, network net);
#endif

#endif

