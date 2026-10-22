#ifndef ACTIVATION_LAYER_H
#define ACTIVATION_LAYER_H

#ضمن "activations.h"
#ضمن "layer.h"
#ضمن "network.h"

layer make_activation_layer(صحيح batch, صحيح inputs, ACTIVATION activation);

فراغ forward_activation_layer(layer l, network net);
فراغ backward_activation_layer(layer l, network net);

#ifdef GPU
فراغ forward_activation_layer_gpu(layer l, network net);
فراغ backward_activation_layer_gpu(layer l, network net);
#endif

#endif

