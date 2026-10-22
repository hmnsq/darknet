#ifndef REGION_LAYER_H
#define REGION_LAYER_H

#شامل "darknet.h"
#شامل "layer.h"
#شامل "network.h"

layer make_region_layer(صحیح batch, صحیح w, صحیح h, صحیح n, صحیح classes, صحیح coords);
تهی forward_region_layer(ثابت layer l, network net);
تهی backward_region_layer(ثابت layer l, network net);
تهی resize_region_layer(layer *l, صحیح w, صحیح h);

#ifdef GPU
تهی forward_region_layer_gpu(ثابت layer l, network net);
تهی backward_region_layer_gpu(layer l, network net);
#endif

#endif
