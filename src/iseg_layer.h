#ifndef ISEG_LAYER_H
#define ISEG_LAYER_H

#അന്തര്ഭാവ "darknet.h"
#അന്തര്ഭാവ "layer.h"
#അന്തര്ഭാവ "network.h"

layer make_iseg_layer(പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h, പൂർണ്ണസംഖ്യ classes, പൂർണ്ണസംഖ്യ ids);
ശൂന്യം forward_iseg_layer(സ്ഥിരം layer l, network net);
ശൂന്യം backward_iseg_layer(സ്ഥിരം layer l, network net);
ശൂന്യം resize_iseg_layer(layer *l, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h);
പൂർണ്ണസംഖ്യ iseg_അക്കം_detections(layer l, ദശാംശം thresh);

#ifdef GPU
ശൂന്യം forward_iseg_layer_gpu(സ്ഥിരം layer l, network net);
ശൂന്യം backward_iseg_layer_gpu(layer l, network net);
#endif

#endif
