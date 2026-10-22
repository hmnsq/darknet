#ifndef L2NORM_LAYER_H
#define L2NORM_LAYER_H
#അന്തര്ഭാവ "layer.h"
#അന്തര്ഭാവ "network.h"

layer make_l2norm_layer(പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ inputs);
ശൂന്യം forward_l2norm_layer(സ്ഥിരം layer l, network net);
ശൂന്യം backward_l2norm_layer(സ്ഥിരം layer l, network net);

#ifdef GPU
ശൂന്യം forward_l2norm_layer_gpu(സ്ഥിരം layer l, network net);
ശൂന്യം backward_l2norm_layer_gpu(സ്ഥിരം layer l, network net);
#endif

#endif
