#ifndef LOCAL_LAYER_H
#define LOCAL_LAYER_H

#அந்தர்பாவ "cuda.h"
#அந்தர்பாவ "image.h"
#அந்தர்பாவ "activations.h"
#அந்தர்பாவ "layer.h"
#அந்தர்பாவ "network.h"

வகைவரை layer local_layer;

#ifdef GPU
வெறுமை forward_local_layer_gpu(local_layer layer, network net);
வெறுமை backward_local_layer_gpu(local_layer layer, network net);
வெறுமை update_local_layer_gpu(local_layer layer, update_args a);

வெறுமை தள்ளு_local_layer(local_layer layer);
வெறுமை pull_local_layer(local_layer layer);
#endif

local_layer make_local_layer(முழு batch, முழு h, முழு w, முழு c, முழு n, முழு size, முழு stride, முழு pad, ACTIVATION activation);

வெறுமை forward_local_layer(மாறிலி local_layer layer, network net);
வெறுமை backward_local_layer(local_layer layer, network net);
வெறுமை update_local_layer(local_layer layer, update_args a);

வெறுமை bias_output(தசம *output, தசம *biases, முழு batch, முழு n, முழு size);
வெறுமை backward_bias(தசம *bias_updates, தசம *delta, முழு batch, முழு n, முழு size);

#endif

