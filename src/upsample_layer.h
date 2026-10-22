#ifndef UPSAMPLE_LAYER_H
#define UPSAMPLE_LAYER_H
#അന്തര്ഭാവ "darknet.h"

layer make_upsample_layer(പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h, പൂർണ്ണസംഖ്യ c, പൂർണ്ണസംഖ്യ stride);
ശൂന്യം forward_upsample_layer(സ്ഥിരം layer l, network net);
ശൂന്യം backward_upsample_layer(സ്ഥിരം layer l, network net);
ശൂന്യം resize_upsample_layer(layer *l, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h);

#ifdef GPU
ശൂന്യം forward_upsample_layer_gpu(സ്ഥിരം layer l, network net);
ശൂന്യം backward_upsample_layer_gpu(സ്ഥിരം layer l, network net);
#endif

#endif
