#ifndef SHORTCUT_LAYER_H
#define SHORTCUT_LAYER_H

#அந்தர்பாவ "layer.h"
#அந்தர்பாவ "network.h"

layer make_shortcut_layer(முழு batch, முழு index, முழு w, முழு h, முழு c, முழு w2, முழு h2, முழு c2);
வெறுமை forward_shortcut_layer(மாறிலி layer l, network net);
வெறுமை backward_shortcut_layer(மாறிலி layer l, network net);
வெறுமை resize_shortcut_layer(layer *l, முழு w, முழு h);

#ifdef GPU
வெறுமை forward_shortcut_layer_gpu(மாறிலி layer l, network net);
வெறுமை backward_shortcut_layer_gpu(மாறிலி layer l, network net);
#endif

#endif
