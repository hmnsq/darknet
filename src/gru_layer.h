
#ifndef GRU_LAYER_H
#define GRU_LAYER_H

#അന്തര്ഭാവ "activations.h"
#അന്തര്ഭാവ "layer.h"
#അന്തര്ഭാവ "network.h"

layer make_gru_layer(പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ inputs, പൂർണ്ണസംഖ്യ outputs, പൂർണ്ണസംഖ്യ steps, പൂർണ്ണസംഖ്യ batch_normalize, പൂർണ്ണസംഖ്യ adam);

ശൂന്യം forward_gru_layer(layer l, network state);
ശൂന്യം backward_gru_layer(layer l, network state);
ശൂന്യം update_gru_layer(layer l, update_args a);

#ifdef GPU
ശൂന്യം forward_gru_layer_gpu(layer l, network state);
ശൂന്യം backward_gru_layer_gpu(layer l, network state);
ശൂന്യം update_gru_layer_gpu(layer l, update_args a);
ശൂന്യം തള്ളുക_gru_layer(layer l);
ശൂന്യം pull_gru_layer(layer l);
#endif

#endif

