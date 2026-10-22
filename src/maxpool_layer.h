#ifndef MAXPOOL_LAYER_H
#define MAXPOOL_LAYER_H

#شامل "image.h"
#شامل "cuda.h"
#شامل "layer.h"
#شامل "network.h"

تعریفنوع layer maxpool_layer;

image get_maxpool_image(maxpool_layer l);
maxpool_layer make_maxpool_layer(صحیح batch, صحیح h, صحیح w, صحیح c, صحیح size, صحیح stride, صحیح padding);
تهی resize_maxpool_layer(maxpool_layer *l, صحیح w, صحیح h);
تهی forward_maxpool_layer(ثابت maxpool_layer l, network net);
تهی backward_maxpool_layer(ثابت maxpool_layer l, network net);

#ifdef GPU
تهی forward_maxpool_layer_gpu(maxpool_layer l, network net);
تهی backward_maxpool_layer_gpu(maxpool_layer l, network net);
#endif

#endif

