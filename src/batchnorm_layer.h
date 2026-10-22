#ifndef BATCHNORM_LAYER_H
#define BATCHNORM_LAYER_H

#شامل "image.h"
#شامل "layer.h"
#شامل "network.h"

layer make_batchnorm_layer(صحیح batch, صحیح w, صحیح h, صحیح c);
تهی forward_batchnorm_layer(layer l, network net);
تهی backward_batchnorm_layer(layer l, network net);

#ifdef GPU
تهی forward_batchnorm_layer_gpu(layer l, network net);
تهی backward_batchnorm_layer_gpu(layer l, network net);
تهی pull_batchnorm_layer(layer l);
تهی افزودن_batchnorm_layer(layer l);
#endif

#endif
