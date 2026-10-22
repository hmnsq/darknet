
#ifndef RNN_LAYER_H
#define RNN_LAYER_H

#അന്തര്ഭാവ "activations.h"
#അന്തര്ഭാവ "layer.h"
#അന്തര്ഭാവ "network.h"
#define USET

layer make_rnn_layer(പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ inputs, പൂർണ്ണസംഖ്യ outputs, പൂർണ്ണസംഖ്യ steps, ACTIVATION activation, പൂർണ്ണസംഖ്യ batch_normalize, പൂർണ്ണസംഖ്യ adam);

ശൂന്യം forward_rnn_layer(layer l, network net);
ശൂന്യം backward_rnn_layer(layer l, network net);
ശൂന്യം update_rnn_layer(layer l, update_args a);

#ifdef GPU
ശൂന്യം forward_rnn_layer_gpu(layer l, network net);
ശൂന്യം backward_rnn_layer_gpu(layer l, network net);
ശൂന്യം update_rnn_layer_gpu(layer l, update_args a);
ശൂന്യം തള്ളുക_rnn_layer(layer l);
ശൂന്യം pull_rnn_layer(layer l);
#endif

#endif

