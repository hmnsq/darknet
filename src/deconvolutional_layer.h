#ifndef DECONVOLUTIONAL_LAYER_H
#define DECONVOLUTIONAL_LAYER_H

#അന്തര്ഭാവ "cuda.h"
#അന്തര്ഭാവ "image.h"
#അന്തര്ഭാവ "activations.h"
#അന്തര്ഭാവ "layer.h"
#അന്തര്ഭാവ "network.h"

#ifdef GPU
ശൂന്യം forward_deconvolutional_layer_gpu(layer l, network net);
ശൂന്യം backward_deconvolutional_layer_gpu(layer l, network net);
ശൂന്യം update_deconvolutional_layer_gpu(layer l, update_args a);
ശൂന്യം തള്ളുക_deconvolutional_layer(layer l);
ശൂന്യം pull_deconvolutional_layer(layer l);
#endif

layer make_deconvolutional_layer(പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ h, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ c, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ size, പൂർണ്ണസംഖ്യ stride, പൂർണ്ണസംഖ്യ padding, ACTIVATION activation, പൂർണ്ണസംഖ്യ batch_normalize, പൂർണ്ണസംഖ്യ adam);
ശൂന്യം resize_deconvolutional_layer(layer *l, പൂർണ്ണസംഖ്യ h, പൂർണ്ണസംഖ്യ w);
ശൂന്യം forward_deconvolutional_layer(സ്ഥിരം layer l, network net);
ശൂന്യം update_deconvolutional_layer(layer l, update_args a);
ശൂന്യം backward_deconvolutional_layer(layer l, network net);

#endif

