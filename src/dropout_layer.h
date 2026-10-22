#ifndef DROPOUT_LAYER_H
#define DROPOUT_LAYER_H

#അന്തര്ഭാവ "layer.h"
#അന്തര്ഭാവ "network.h"

തരനിർവചനം layer dropout_layer;

dropout_layer make_dropout_layer(പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ inputs, ദശാംശം probability);

ശൂന്യം forward_dropout_layer(dropout_layer l, network net);
ശൂന്യം backward_dropout_layer(dropout_layer l, network net);
ശൂന്യം resize_dropout_layer(dropout_layer *l, പൂർണ്ണസംഖ്യ inputs);

#ifdef GPU
ശൂന്യം forward_dropout_layer_gpu(dropout_layer l, network net);
ശൂന്യം backward_dropout_layer_gpu(dropout_layer l, network net);

#endif
#endif
