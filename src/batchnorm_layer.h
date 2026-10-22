#ifndef BATCHNORM_LAYER_H
#define BATCHNORM_LAYER_H

#അന്തര്ഭാവ "image.h"
#അന്തര്ഭാവ "layer.h"
#അന്തര്ഭാവ "network.h"

layer make_batchnorm_layer(പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h, പൂർണ്ണസംഖ്യ c);
ശൂന്യം forward_batchnorm_layer(layer l, network net);
ശൂന്യം backward_batchnorm_layer(layer l, network net);

#ifdef GPU
ശൂന്യം forward_batchnorm_layer_gpu(layer l, network net);
ശൂന്യം backward_batchnorm_layer_gpu(layer l, network net);
ശൂന്യം pull_batchnorm_layer(layer l);
ശൂന്യം തള്ളുക_batchnorm_layer(layer l);
#endif

#endif
