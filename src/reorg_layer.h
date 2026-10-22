#ifndef REORG_LAYER_H
#define REORG_LAYER_H

#شامل "image.h"
#شامل "cuda.h"
#شامل "layer.h"
#شامل "network.h"

layer make_reorg_layer(صحیح batch, صحیح w, صحیح h, صحیح c, صحیح stride, صحیح reverse, صحیح flatten, صحیح extra);
تهی resize_reorg_layer(layer *l, صحیح w, صحیح h);
تهی forward_reorg_layer(ثابت layer l, network net);
تهی backward_reorg_layer(ثابت layer l, network net);

#ifdef GPU
تهی forward_reorg_layer_gpu(layer l, network net);
تهی backward_reorg_layer_gpu(layer l, network net);
#endif

#endif

