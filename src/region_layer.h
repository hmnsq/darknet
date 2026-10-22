#ifndef REGION_LAYER_H
#define REGION_LAYER_H

#അന്തര്ഭാവ "darknet.h"
#അന്തര്ഭാവ "layer.h"
#അന്തര്ഭാവ "network.h"

layer make_region_layer(പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ classes, പൂർണ്ണസംഖ്യ coords);
ശൂന്യം forward_region_layer(സ്ഥിരം layer l, network net);
ശൂന്യം backward_region_layer(സ്ഥിരം layer l, network net);
ശൂന്യം resize_region_layer(layer *l, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h);

#ifdef GPU
ശൂന്യം forward_region_layer_gpu(സ്ഥിരം layer l, network net);
ശൂന്യം backward_region_layer_gpu(layer l, network net);
#endif

#endif
