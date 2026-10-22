#ifndef SOFTMAX_LAYER_H
#define SOFTMAX_LAYER_H
#അന്തര്ഭാവ "layer.h"
#അന്തര്ഭാവ "network.h"

തരനിർവചനം layer softmax_layer;

ശൂന്യം softmax_array(ദശാംശം *നിവേശം, പൂർണ്ണസംഖ്യ n, ദശാംശം temp, ദശാംശം *output);
softmax_layer make_softmax_layer(പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ inputs, പൂർണ്ണസംഖ്യ groups);
ശൂന്യം forward_softmax_layer(സ്ഥിരം softmax_layer l, network net);
ശൂന്യം backward_softmax_layer(സ്ഥിരം softmax_layer l, network net);

#ifdef GPU
ശൂന്യം pull_softmax_layer_output(സ്ഥിരം softmax_layer l);
ശൂന്യം forward_softmax_layer_gpu(സ്ഥിരം softmax_layer l, network net);
ശൂന്യം backward_softmax_layer_gpu(സ്ഥിരം softmax_layer l, network net);
#endif

#endif
