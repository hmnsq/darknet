#ifndef CROP_LAYER_H
#define CROP_LAYER_H

#அந்தர்பாவ "image.h"
#அந்தர்பாவ "layer.h"
#அந்தர்பாவ "network.h"

வகைவரை layer crop_layer;

image get_crop_image(crop_layer l);
crop_layer make_crop_layer(முழு batch, முழு h, முழு w, முழு c, முழு crop_height, முழு crop_width, முழு flip, தசம angle, தசம saturation, தசம exposure);
வெறுமை forward_crop_layer(மாறிலி crop_layer l, network net);
வெறுமை resize_crop_layer(layer *l, முழு w, முழு h);

#ifdef GPU
வெறுமை forward_crop_layer_gpu(crop_layer l, network net);
#endif

#endif

