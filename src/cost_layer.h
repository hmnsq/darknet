#ifndef COST_LAYER_H
#define COST_LAYER_H
#അന്തര്ഭാവ "layer.h"
#അന്തര്ഭാവ "network.h"

തരനിർവചനം layer cost_layer;

COST_TYPE get_cost_type(അക്ഷരം *s);
അക്ഷരം *get_cost_string(COST_TYPE a);
cost_layer make_cost_layer(പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ inputs, COST_TYPE type, ദശാംശം scale);
ശൂന്യം forward_cost_layer(സ്ഥിരം cost_layer l, network net);
ശൂന്യം backward_cost_layer(സ്ഥിരം cost_layer l, network net);
ശൂന്യം resize_cost_layer(cost_layer *l, പൂർണ്ണസംഖ്യ inputs);

#ifdef GPU
ശൂന്യം forward_cost_layer_gpu(cost_layer l, network net);
ശൂന്യം backward_cost_layer_gpu(സ്ഥിരം cost_layer l, network net);
#endif

#endif
