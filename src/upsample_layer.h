#ifndef UPSAMPLE_LAYER_H
#define UPSAMPLE_LAYER_H
#அந்தர்பாவ "darknet.h"

layer make_upsample_layer(முழு batch, முழு w, முழு h, முழு c, முழு stride);
வெறுமை forward_upsample_layer(மாறிலி layer l, network net);
வெறுமை backward_upsample_layer(மாறிலி layer l, network net);
வெறுமை resize_upsample_layer(layer *l, முழு w, முழு h);

#ifdef GPU
வெறுமை forward_upsample_layer_gpu(மாறிலி layer l, network net);
வெறுமை backward_upsample_layer_gpu(மாறிலி layer l, network net);
#endif

#endif
