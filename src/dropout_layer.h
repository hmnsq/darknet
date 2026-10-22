#ifndef DROPOUT_LAYER_H
#define DROPOUT_LAYER_H

#அந்தர்பாவ "layer.h"
#அந்தர்பாவ "network.h"

வகைவரை layer dropout_layer;

dropout_layer make_dropout_layer(முழு batch, முழு inputs, தசம probability);

வெறுமை forward_dropout_layer(dropout_layer l, network net);
வெறுமை backward_dropout_layer(dropout_layer l, network net);
வெறுமை resize_dropout_layer(dropout_layer *l, முழு inputs);

#ifdef GPU
வெறுமை forward_dropout_layer_gpu(dropout_layer l, network net);
வெறுமை backward_dropout_layer_gpu(dropout_layer l, network net);

#endif
#endif
