#ifndef REORG_LAYER_H
#define REORG_LAYER_H

#അന്തര്ഭാവ "image.h"
#അന്തര്ഭാവ "cuda.h"
#അന്തര്ഭാവ "layer.h"
#അന്തര്ഭാവ "network.h"

layer make_reorg_layer(പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h, പൂർണ്ണസംഖ്യ c, പൂർണ്ണസംഖ്യ stride, പൂർണ്ണസംഖ്യ reverse, പൂർണ്ണസംഖ്യ flatten, പൂർണ്ണസംഖ്യ extra);
ശൂന്യം resize_reorg_layer(layer *l, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h);
ശൂന്യം forward_reorg_layer(സ്ഥിരം layer l, network net);
ശൂന്യം backward_reorg_layer(സ്ഥിരം layer l, network net);

#ifdef GPU
ശൂന്യം forward_reorg_layer_gpu(layer l, network net);
ശൂന്യം backward_reorg_layer_gpu(layer l, network net);
#endif

#endif

