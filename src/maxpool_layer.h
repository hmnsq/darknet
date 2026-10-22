#ifndef MAXPOOL_LAYER_H
#define MAXPOOL_LAYER_H

#അന്തര്ഭാവ "image.h"
#അന്തര്ഭാവ "cuda.h"
#അന്തര്ഭാവ "layer.h"
#അന്തര്ഭാവ "network.h"

തരനിർവചനം layer maxpool_layer;

image get_maxpool_image(maxpool_layer l);
maxpool_layer make_maxpool_layer(പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ h, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ c, പൂർണ്ണസംഖ്യ size, പൂർണ്ണസംഖ്യ stride, പൂർണ്ണസംഖ്യ padding);
ശൂന്യം resize_maxpool_layer(maxpool_layer *l, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h);
ശൂന്യം forward_maxpool_layer(സ്ഥിരം maxpool_layer l, network net);
ശൂന്യം backward_maxpool_layer(സ്ഥിരം maxpool_layer l, network net);

#ifdef GPU
ശൂന്യം forward_maxpool_layer_gpu(maxpool_layer l, network net);
ശൂന്യം backward_maxpool_layer_gpu(maxpool_layer l, network net);
#endif

#endif

