#ifndef SHORTCUT_LAYER_H
#define SHORTCUT_LAYER_H

#അന്തര്ഭാവ "layer.h"
#അന്തര്ഭാവ "network.h"

layer make_shortcut_layer(പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ index, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h, പൂർണ്ണസംഖ്യ c, പൂർണ്ണസംഖ്യ w2, പൂർണ്ണസംഖ്യ h2, പൂർണ്ണസംഖ്യ c2);
ശൂന്യം forward_shortcut_layer(സ്ഥിരം layer l, network net);
ശൂന്യം backward_shortcut_layer(സ്ഥിരം layer l, network net);
ശൂന്യം resize_shortcut_layer(layer *l, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h);

#ifdef GPU
ശൂന്യം forward_shortcut_layer_gpu(സ്ഥിരം layer l, network net);
ശൂന്യം backward_shortcut_layer_gpu(സ്ഥിരം layer l, network net);
#endif

#endif
