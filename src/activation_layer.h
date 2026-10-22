#ifndef ACTIVATION_LAYER_H
#define ACTIVATION_LAYER_H

#അന്തര്ഭാവ "activations.h"
#അന്തര്ഭാവ "layer.h"
#അന്തര്ഭാവ "network.h"

layer make_activation_layer(പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ inputs, ACTIVATION activation);

ശൂന്യം forward_activation_layer(layer l, network net);
ശൂന്യം backward_activation_layer(layer l, network net);

#ifdef GPU
ശൂന്യം forward_activation_layer_gpu(layer l, network net);
ശൂന്യം backward_activation_layer_gpu(layer l, network net);
#endif

#endif

