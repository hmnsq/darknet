#ifndef LOCAL_LAYER_H
#define LOCAL_LAYER_H

#അന്തര്ഭാവ "cuda.h"
#അന്തര്ഭാവ "image.h"
#അന്തര്ഭാവ "activations.h"
#അന്തര്ഭാവ "layer.h"
#അന്തര്ഭാവ "network.h"

തരനിർവചനം layer local_layer;

#ifdef GPU
ശൂന്യം forward_local_layer_gpu(local_layer layer, network net);
ശൂന്യം backward_local_layer_gpu(local_layer layer, network net);
ശൂന്യം update_local_layer_gpu(local_layer layer, update_args a);

ശൂന്യം തള്ളുക_local_layer(local_layer layer);
ശൂന്യം pull_local_layer(local_layer layer);
#endif

local_layer make_local_layer(പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ h, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ c, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ size, പൂർണ്ണസംഖ്യ stride, പൂർണ്ണസംഖ്യ pad, ACTIVATION activation);

ശൂന്യം forward_local_layer(സ്ഥിരം local_layer layer, network net);
ശൂന്യം backward_local_layer(local_layer layer, network net);
ശൂന്യം update_local_layer(local_layer layer, update_args a);

ശൂന്യം bias_output(ദശാംശം *output, ദശാംശം *biases, പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ size);
ശൂന്യം backward_bias(ദശാംശം *bias_updates, ദശാംശം *delta, പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ size);

#endif

