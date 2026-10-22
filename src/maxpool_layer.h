#ifndef MAXPOOL_LAYER_H
#define MAXPOOL_LAYER_H

#அந்தர்பாவ "image.h"
#அந்தர்பாவ "cuda.h"
#அந்தர்பாவ "layer.h"
#அந்தர்பாவ "network.h"

வகைவரை layer maxpool_layer;

image get_maxpool_image(maxpool_layer l);
maxpool_layer make_maxpool_layer(முழு batch, முழு h, முழு w, முழு c, முழு size, முழு stride, முழு padding);
வெறுமை resize_maxpool_layer(maxpool_layer *l, முழு w, முழு h);
வெறுமை forward_maxpool_layer(மாறிலி maxpool_layer l, network net);
வெறுமை backward_maxpool_layer(மாறிலி maxpool_layer l, network net);

#ifdef GPU
வெறுமை forward_maxpool_layer_gpu(maxpool_layer l, network net);
வெறுமை backward_maxpool_layer_gpu(maxpool_layer l, network net);
#endif

#endif

