#ifndef DETECTION_LAYER_H
#define DETECTION_LAYER_H

#അന്തര്ഭാവ "layer.h"
#അന്തര്ഭാവ "network.h"

തരനിർവചനം layer detection_layer;

detection_layer make_detection_layer(പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ inputs, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ size, പൂർണ്ണസംഖ്യ classes, പൂർണ്ണസംഖ്യ coords, പൂർണ്ണസംഖ്യ rescore);
ശൂന്യം forward_detection_layer(സ്ഥിരം detection_layer l, network net);
ശൂന്യം backward_detection_layer(സ്ഥിരം detection_layer l, network net);

#ifdef GPU
ശൂന്യം forward_detection_layer_gpu(സ്ഥിരം detection_layer l, network net);
ശൂന്യം backward_detection_layer_gpu(detection_layer l, network net);
#endif

#endif
