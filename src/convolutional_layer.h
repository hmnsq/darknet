#ifndef CONVOLUTIONAL_LAYER_H
#define CONVOLUTIONAL_LAYER_H

#അന്തര്ഭാവ "cuda.h"
#അന്തര്ഭാവ "image.h"
#അന്തര്ഭാവ "activations.h"
#അന്തര്ഭാവ "layer.h"
#അന്തര്ഭാവ "network.h"

തരനിർവചനം layer convolutional_layer;

#ifdef GPU
ശൂന്യം forward_convolutional_layer_gpu(convolutional_layer layer, network net);
ശൂന്യം backward_convolutional_layer_gpu(convolutional_layer layer, network net);
ശൂന്യം update_convolutional_layer_gpu(convolutional_layer layer, update_args a);

ശൂന്യം തള്ളുക_convolutional_layer(convolutional_layer layer);
ശൂന്യം pull_convolutional_layer(convolutional_layer layer);

ശൂന്യം add_bias_gpu(ദശാംശം *output, ദശാംശം *biases, പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ size);
ശൂന്യം backward_bias_gpu(ദശാംശം *bias_updates, ദശാംശം *delta, പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ size);
ശൂന്യം adam_update_gpu(ദശാംശം *w, ദശാംശം *d, ദശാംശം *m, ദശാംശം *v, ദശാംശം B1, ദശാംശം B2, ദശാംശം eps, ദശാംശം decay, ദശാംശം rate, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ t);
#ifdef CUDNN
ശൂന്യം cudnn_convolutional_setup(layer *l);
#endif
#endif

convolutional_layer make_convolutional_layer(പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ h, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ c, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ groups, പൂർണ്ണസംഖ്യ size, പൂർണ്ണസംഖ്യ stride, പൂർണ്ണസംഖ്യ padding, ACTIVATION activation, പൂർണ്ണസംഖ്യ batch_normalize, പൂർണ്ണസംഖ്യ ദ്വിമാന, പൂർണ്ണസംഖ്യ xnor, പൂർണ്ണസംഖ്യ adam);
ശൂന്യം resize_convolutional_layer(convolutional_layer *layer, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h);
ശൂന്യം forward_convolutional_layer(സ്ഥിരം convolutional_layer layer, network net);
ശൂന്യം update_convolutional_layer(convolutional_layer layer, update_args a);
image *visualize_convolutional_layer(convolutional_layer layer, അക്ഷരം *window, image *prev_weights);
ശൂന്യം binarize_weights(ദശാംശം *weights, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ size, ദശാംശം *ദ്വിമാന);
ശൂന്യം swap_ദ്വിമാന(convolutional_layer *l);
ശൂന്യം binarize_weights2(ദശാംശം *weights, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ size, അക്ഷരം *ദ്വിമാന, ദശാംശം *scales);

ശൂന്യം backward_convolutional_layer(convolutional_layer layer, network net);

ശൂന്യം add_bias(ദശാംശം *output, ദശാംശം *biases, പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ size);
ശൂന്യം backward_bias(ദശാംശം *bias_updates, ദശാംശം *delta, പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ n, പൂർണ്ണസംഖ്യ size);

image get_convolutional_image(convolutional_layer layer);
image get_convolutional_delta(convolutional_layer layer);
image get_convolutional_weight(convolutional_layer layer, പൂർണ്ണസംഖ്യ i);

പൂർണ്ണസംഖ്യ convolutional_out_height(convolutional_layer layer);
പൂർണ്ണസംഖ്യ convolutional_out_width(convolutional_layer layer);

#endif

