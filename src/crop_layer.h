#ifndef CROP_LAYER_H
#define CROP_LAYER_H

#അന്തര്ഭാവ "image.h"
#അന്തര്ഭാവ "layer.h"
#അന്തര്ഭാവ "network.h"

തരനിർവചനം layer crop_layer;

image get_crop_image(crop_layer l);
crop_layer make_crop_layer(പൂർണ്ണസംഖ്യ batch, പൂർണ്ണസംഖ്യ h, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ c, പൂർണ്ണസംഖ്യ crop_height, പൂർണ്ണസംഖ്യ crop_width, പൂർണ്ണസംഖ്യ flip, ദശാംശം angle, ദശാംശം saturation, ദശാംശം exposure);
ശൂന്യം forward_crop_layer(സ്ഥിരം crop_layer l, network net);
ശൂന്യം resize_crop_layer(layer *l, പൂർണ്ണസംഖ്യ w, പൂർണ്ണസംഖ്യ h);

#ifdef GPU
ശൂന്യം forward_crop_layer_gpu(crop_layer l, network net);
#endif

#endif

