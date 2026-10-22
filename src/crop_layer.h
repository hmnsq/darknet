#ifndef CROP_LAYER_H
#define CROP_LAYER_H

#شامل "image.h"
#شامل "layer.h"
#شامل "network.h"

تعریفنوع layer crop_layer;

image get_crop_image(crop_layer l);
crop_layer make_crop_layer(صحیح batch, صحیح h, صحیح w, صحیح c, صحیح crop_height, صحیح crop_width, صحیح flip, اعشاری angle, اعشاری saturation, اعشاری exposure);
تهی forward_crop_layer(ثابت crop_layer l, network net);
تهی resize_crop_layer(layer *l, صحیح w, صحیح h);

#ifdef GPU
تهی forward_crop_layer_gpu(crop_layer l, network net);
#endif

#endif

