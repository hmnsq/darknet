#ifndef NORMALIZATION_LAYER_H
#define NORMALIZATION_LAYER_H

#അന്തര്ഭാവ "image.h"
#അന്തര്ഭാവ "layer.h"
#അന്തര്ഭാവ "network.h"

layer make_normalization_layer(പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h, പൂർണ്ണസംഖ്യ c, പൂർണ്ണസംഖ്യ size, ദശാംശം alpha, ദശാംശം beta, ദശാംശം kappa);
ശൂന്യം resize_normalization_layer(layer *layer, പൂർണ്ണസംഖ്യ h, പൂർണ്ണസംഖ്യ w);
ശൂന്യം forward_normalization_layer(സ്ഥിരം layer layer, network net);
ശൂന്യം backward_normalization_layer(സ്ഥിരം layer layer, network net);
ശൂന്യം visualize_normalization_layer(layer layer, അക്ഷരം *window);

#ifdef GPU
ശൂന്യം forward_normalization_layer_gpu(സ്ഥിരം layer layer, network net);
ശൂന്യം backward_normalization_layer_gpu(സ്ഥിരം layer layer, network net);
#endif

#endif
