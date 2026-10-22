#ifndef ROUTE_LAYER_H
#define ROUTE_LAYER_H
#അന്തര്ഭാവ "network.h"
#അന്തര്ഭാവ "layer.h"

തരനിർവചനം layer route_layer;

route_layer make_route_layer(പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ *നിവേശം_layers, പൂർണ്ണസംഖ്യ *നിവേശം_size);
ശൂന്യം forward_route_layer(സ്ഥിരം route_layer l, network net);
ശൂന്യം backward_route_layer(സ്ഥിരം route_layer l, network net);
ശൂന്യം resize_route_layer(route_layer *l, network *net);

#ifdef GPU
ശൂന്യം forward_route_layer_gpu(സ്ഥിരം route_layer l, network net);
ശൂന്യം backward_route_layer_gpu(സ്ഥിരം route_layer l, network net);
#endif

#endif
