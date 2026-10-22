#ifndef AVGPOOL_LAYER_H
#define AVGPOOL_LAYER_H

#அந்தர்பாவ "image.h"
#அந்தர்பாவ "cuda.h"
#அந்தர்பாவ "layer.h"
#அந்தர்பாவ "network.h"

வகைவரை layer avgpool_layer;

image get_avgpool_image(avgpool_layer l);
avgpool_layer make_avgpool_layer(முழு batch, முழு w, முழு h, முழு c);
வெறுமை resize_avgpool_layer(avgpool_layer *l, முழு w, முழு h);
வெறுமை forward_avgpool_layer(மாறிலி avgpool_layer l, network net);
வெறுமை backward_avgpool_layer(மாறிலி avgpool_layer l, network net);

#ifdef GPU
வெறுமை forward_avgpool_layer_gpu(avgpool_layer l, network net);
வெறுமை backward_avgpool_layer_gpu(avgpool_layer l, network net);
#endif

#endif

