#ifndef REORG_LAYER_H
#define REORG_LAYER_H

#அந்தர்பாவ "image.h"
#அந்தர்பாவ "cuda.h"
#அந்தர்பாவ "layer.h"
#அந்தர்பாவ "network.h"

layer make_reorg_layer(முழு batch, முழு w, முழு h, முழு c, முழு stride, முழு reverse, முழு flatten, முழு extra);
வெறுமை resize_reorg_layer(layer *l, முழு w, முழு h);
வெறுமை forward_reorg_layer(மாறிலி layer l, network net);
வெறுமை backward_reorg_layer(மாறிலி layer l, network net);

#ifdef GPU
வெறுமை forward_reorg_layer_gpu(layer l, network net);
வெறுமை backward_reorg_layer_gpu(layer l, network net);
#endif

#endif

