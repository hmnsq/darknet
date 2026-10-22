#ifndef DECONVOLUTIONAL_LAYER_H
#define DECONVOLUTIONAL_LAYER_H

#شامل "cuda.h"
#شامل "image.h"
#شامل "activations.h"
#شامل "layer.h"
#شامل "network.h"

#ifdef GPU
تهی forward_deconvolutional_layer_gpu(layer l, network net);
تهی backward_deconvolutional_layer_gpu(layer l, network net);
تهی update_deconvolutional_layer_gpu(layer l, update_args a);
تهی افزودن_deconvolutional_layer(layer l);
تهی pull_deconvolutional_layer(layer l);
#endif

layer make_deconvolutional_layer(صحیح batch, صحیح h, صحیح w, صحیح c, صحیح n, صحیح size, صحیح stride, صحیح padding, ACTIVATION activation, صحیح batch_normalize, صحیح adam);
تهی resize_deconvolutional_layer(layer *l, صحیح h, صحیح w);
تهی forward_deconvolutional_layer(ثابت layer l, network net);
تهی update_deconvolutional_layer(layer l, update_args a);
تهی backward_deconvolutional_layer(layer l, network net);

#endif

