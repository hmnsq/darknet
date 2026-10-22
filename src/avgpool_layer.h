#ifndef AVGPOOL_LAYER_H
#define AVGPOOL_LAYER_H

#شامل "image.h"
#شامل "cuda.h"
#شامل "layer.h"
#شامل "network.h"

تعریفنوع layer avgpool_layer;

image get_avgpool_image(avgpool_layer l);
avgpool_layer make_avgpool_layer(صحیح batch, صحیح w, صحیح h, صحیح c);
تهی resize_avgpool_layer(avgpool_layer *l, صحیح w, صحیح h);
تهی forward_avgpool_layer(ثابت avgpool_layer l, network net);
تهی backward_avgpool_layer(ثابت avgpool_layer l, network net);

#ifdef GPU
تهی forward_avgpool_layer_gpu(avgpool_layer l, network net);
تهی backward_avgpool_layer_gpu(avgpool_layer l, network net);
#endif

#endif

