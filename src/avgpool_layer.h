#ifndef AVGPOOL_LAYER_H
#define AVGPOOL_LAYER_H

#അന്തര്ഭാവ "image.h"
#അന്തര്ഭാവ "cuda.h"
#അന്തര്ഭാവ "layer.h"
#അന്തര്ഭാവ "network.h"

തരനിർവചനം layer avgpool_layer;

image get_avgpool_image(avgpool_layer l);
avgpool_layer make_avgpool_layer(പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h, പൂർണ്ണസംഖ്യ c);
ശൂന്യം resize_avgpool_layer(avgpool_layer *l, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h);
ശൂന്യം forward_avgpool_layer(സ്ഥിരം avgpool_layer l, network net);
ശൂന്യം backward_avgpool_layer(സ്ഥിരം avgpool_layer l, network net);

#ifdef GPU
ശൂന്യം forward_avgpool_layer_gpu(avgpool_layer l, network net);
ശൂന്യം backward_avgpool_layer_gpu(avgpool_layer l, network net);
#endif

#endif

