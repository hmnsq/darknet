#ifndef YOLO_LAYER_H
#define YOLO_LAYER_H

#അന്തര്ഭാവ "darknet.h"
#അന്തര്ഭാവ "layer.h"
#അന്തര്ഭാവ "network.h"

layer make_yolo_layer(പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ total, പൂർണ്ണസംഖ്യ *mask, പൂർണ്ണസംഖ്യ classes);
ശൂന്യം forward_yolo_layer(സ്ഥിരം layer l, network net);
ശൂന്യം backward_yolo_layer(സ്ഥിരം layer l, network net);
ശൂന്യം resize_yolo_layer(layer *l, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h);
പൂർണ്ണസംഖ്യ yolo_അക്കം_detections(layer l, ദശാംശം thresh);

#ifdef GPU
ശൂന്യം forward_yolo_layer_gpu(സ്ഥിരം layer l, network net);
ശൂന്യം backward_yolo_layer_gpu(layer l, network net);
#endif

#endif
