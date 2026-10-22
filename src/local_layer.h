#ifndef LOCAL_LAYER_H
#define LOCAL_LAYER_H

#شامل "cuda.h"
#شامل "image.h"
#شامل "activations.h"
#شامل "layer.h"
#شامل "network.h"

تعریفنوع layer local_layer;

#ifdef GPU
تهی forward_local_layer_gpu(local_layer layer, network net);
تهی backward_local_layer_gpu(local_layer layer, network net);
تهی update_local_layer_gpu(local_layer layer, update_args a);

تهی افزودن_local_layer(local_layer layer);
تهی pull_local_layer(local_layer layer);
#endif

local_layer make_local_layer(صحیح batch, صحیح h, صحیح w, صحیح c, صحیح n, صحیح size, صحیح stride, صحیح pad, ACTIVATION activation);

تهی forward_local_layer(ثابت local_layer layer, network net);
تهی backward_local_layer(local_layer layer, network net);
تهی update_local_layer(local_layer layer, update_args a);

تهی bias_output(اعشاری *output, اعشاری *biases, صحیح batch, صحیح n, صحیح size);
تهی backward_bias(اعشاری *bias_updates, اعشاری *delta, صحیح batch, صحیح n, صحیح size);

#endif

