#ifndef UPSAMPLE_LAYER_H
#define UPSAMPLE_LAYER_H
#شامل "darknet.h"

layer make_upsample_layer(صحیح batch, صحیح w, صحیح h, صحیح c, صحیح stride);
تهی forward_upsample_layer(ثابت layer l, network net);
تهی backward_upsample_layer(ثابت layer l, network net);
تهی resize_upsample_layer(layer *l, صحیح w, صحیح h);

#ifdef GPU
تهی forward_upsample_layer_gpu(ثابت layer l, network net);
تهی backward_upsample_layer_gpu(ثابت layer l, network net);
#endif

#endif
