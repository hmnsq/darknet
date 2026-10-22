#ifndef CONNECTED_LAYER_H
#define CONNECTED_LAYER_H

#അന്തര്ഭാവ "activations.h"
#അന്തര്ഭാവ "layer.h"
#അന്തര്ഭാവ "network.h"

layer make_connected_layer(പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ inputs, പൂർണ്ണസംഖ്യ outputs, ACTIVATION activation, പൂർണ്ണസംഖ്യ batch_normalize, പൂർണ്ണസംഖ്യ adam);

ശൂന്യം forward_connected_layer(layer l, network net);
ശൂന്യം backward_connected_layer(layer l, network net);
ശൂന്യം update_connected_layer(layer l, update_args a);

#ifdef GPU
ശൂന്യം forward_connected_layer_gpu(layer l, network net);
ശൂന്യം backward_connected_layer_gpu(layer l, network net);
ശൂന്യം update_connected_layer_gpu(layer l, update_args a);
ശൂന്യം തള്ളുക_connected_layer(layer l);
ശൂന്യം pull_connected_layer(layer l);
#endif

#endif

