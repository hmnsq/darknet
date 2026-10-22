
#ifndef CRNN_LAYER_H
#define CRNN_LAYER_H

#അന്തര്ഭാവ "activations.h"
#അന്തര്ഭാവ "layer.h"
#അന്തര്ഭാവ "network.h"

layer make_crnn_layer(പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ h, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ c, പൂർണ്ണസംഖ്യ hidden_filters, പൂർണ്ണസംഖ്യ output_filters, പൂർണ്ണസംഖ്യ steps, ACTIVATION activation, പൂർണ്ണസംഖ്യ batch_normalize);

ശൂന്യം forward_crnn_layer(layer l, network net);
ശൂന്യം backward_crnn_layer(layer l, network net);
ശൂന്യം update_crnn_layer(layer l, update_args a);

#ifdef GPU
ശൂന്യം forward_crnn_layer_gpu(layer l, network net);
ശൂന്യം backward_crnn_layer_gpu(layer l, network net);
ശൂന്യം update_crnn_layer_gpu(layer l, update_args a);
ശൂന്യം തള്ളുക_crnn_layer(layer l);
ശൂന്യം pull_crnn_layer(layer l);
#endif

#endif

