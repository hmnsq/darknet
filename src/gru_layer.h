
#ifndef GRU_LAYER_H
#define GRU_LAYER_H

#அந்தர்பாவ "activations.h"
#அந்தர்பாவ "layer.h"
#அந்தர்பாவ "network.h"

layer make_gru_layer(முழு batch, முழு inputs, முழு outputs, முழு steps, முழு batch_normalize, முழு adam);

வெறுமை forward_gru_layer(layer l, network state);
வெறுமை backward_gru_layer(layer l, network state);
வெறுமை update_gru_layer(layer l, update_args a);

#ifdef GPU
வெறுமை forward_gru_layer_gpu(layer l, network state);
வெறுமை backward_gru_layer_gpu(layer l, network state);
வெறுமை update_gru_layer_gpu(layer l, update_args a);
வெறுமை தள்ளு_gru_layer(layer l);
வெறுமை pull_gru_layer(layer l);
#endif

#endif

