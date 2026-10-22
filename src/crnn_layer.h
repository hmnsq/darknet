
#ifndef CRNN_LAYER_H
#define CRNN_LAYER_H

#அந்தர்பாவ "activations.h"
#அந்தர்பாவ "layer.h"
#அந்தர்பாவ "network.h"

layer make_crnn_layer(முழு batch, முழு h, முழு w, முழு c, முழு hidden_filters, முழு output_filters, முழு steps, ACTIVATION activation, முழு batch_normalize);

வெறுமை forward_crnn_layer(layer l, network net);
வெறுமை backward_crnn_layer(layer l, network net);
வெறுமை update_crnn_layer(layer l, update_args a);

#ifdef GPU
வெறுமை forward_crnn_layer_gpu(layer l, network net);
வெறுமை backward_crnn_layer_gpu(layer l, network net);
வெறுமை update_crnn_layer_gpu(layer l, update_args a);
வெறுமை தள்ளு_crnn_layer(layer l);
வெறுமை pull_crnn_layer(layer l);
#endif

#endif

