#ifndef NORMALIZATION_LAYER_H
#define NORMALIZATION_LAYER_H

#شامل "image.h"
#شامل "layer.h"
#شامل "network.h"

layer make_normalization_layer(صحیح batch, صحیح w, صحیح h, صحیح c, صحیح size, اعشاری alpha, اعشاری beta, اعشاری kappa);
تهی resize_normalization_layer(layer *layer, صحیح h, صحیح w);
تهی forward_normalization_layer(ثابت layer layer, network net);
تهی backward_normalization_layer(ثابت layer layer, network net);
تهی visualize_normalization_layer(layer layer, نویسه *window);

#ifdef GPU
تهی forward_normalization_layer_gpu(ثابت layer layer, network net);
تهی backward_normalization_layer_gpu(ثابت layer layer, network net);
#endif

#endif
