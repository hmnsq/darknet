#ifndef BATCHNORM_LAYER_H
#define BATCHNORM_LAYER_H

#ضمن "image.h"
#ضمن "layer.h"
#ضمن "network.h"

layer make_batchnorm_layer(صحيح batch, صحيح w, صحيح h, صحيح c);
فراغ forward_batchnorm_layer(layer l, network net);
فراغ backward_batchnorm_layer(layer l, network net);

#ifdef GPU
فراغ forward_batchnorm_layer_gpu(layer l, network net);
فراغ backward_batchnorm_layer_gpu(layer l, network net);
فراغ pull_batchnorm_layer(layer l);
فراغ ئىتتىرىش_batchnorm_layer(layer l);
#endif

#endif
