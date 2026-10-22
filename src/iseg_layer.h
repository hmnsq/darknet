#ifndef ISEG_LAYER_H
#define ISEG_LAYER_H

#அந்தர்பாவ "darknet.h"
#அந்தர்பாவ "layer.h"
#அந்தர்பாவ "network.h"

layer make_iseg_layer(முழு batch, முழு w, முழு h, முழு classes, முழு ids);
வெறுமை forward_iseg_layer(மாறிலி layer l, network net);
வெறுமை backward_iseg_layer(மாறிலி layer l, network net);
வெறுமை resize_iseg_layer(layer *l, முழு w, முழு h);
முழு iseg_இலக்கம்_detections(layer l, தசம thresh);

#ifdef GPU
வெறுமை forward_iseg_layer_gpu(மாறிலி layer l, network net);
வெறுமை backward_iseg_layer_gpu(layer l, network net);
#endif

#endif
