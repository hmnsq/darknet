#ifndef CONVOLUTIONAL_LAYER_H
#define CONVOLUTIONAL_LAYER_H

#شامل "cuda.h"
#شامل "image.h"
#شامل "activations.h"
#شامل "layer.h"
#شامل "network.h"

تعریفنوع layer convolutional_layer;

#ifdef GPU
تهی forward_convolutional_layer_gpu(convolutional_layer layer, network net);
تهی backward_convolutional_layer_gpu(convolutional_layer layer, network net);
تهی update_convolutional_layer_gpu(convolutional_layer layer, update_args a);

تهی افزودن_convolutional_layer(convolutional_layer layer);
تهی pull_convolutional_layer(convolutional_layer layer);

تهی add_bias_gpu(اعشاری *output, اعشاری *biases, صحیح batch, صحیح n, صحیح size);
تهی backward_bias_gpu(اعشاری *bias_updates, اعشاری *delta, صحیح batch, صحیح n, صحیح size);
تهی adam_update_gpu(اعشاری *w, اعشاری *d, اعشاری *m, اعشاری *v, اعشاری B1, اعشاری B2, اعشاری eps, اعشاری decay, اعشاری rate, صحیح n, صحیح batch, صحیح t);
#ifdef CUDNN
تهی cudnn_convolutional_setup(layer *l);
#endif
#endif

convolutional_layer make_convolutional_layer(صحیح batch, صحیح h, صحیح w, صحیح c, صحیح n, صحیح groups, صحیح size, صحیح stride, صحیح padding, ACTIVATION activation, صحیح batch_normalize, صحیح دودویی, صحیح xnor, صحیح adam);
تهی resize_convolutional_layer(convolutional_layer *layer, صحیح w, صحیح h);
تهی forward_convolutional_layer(ثابت convolutional_layer layer, network net);
تهی update_convolutional_layer(convolutional_layer layer, update_args a);
image *visualize_convolutional_layer(convolutional_layer layer, نویسه *window, image *prev_weights);
تهی binarize_weights(اعشاری *weights, صحیح n, صحیح size, اعشاری *دودویی);
تهی swap_دودویی(convolutional_layer *l);
تهی binarize_weights2(اعشاری *weights, صحیح n, صحیح size, نویسه *دودویی, اعشاری *scales);

تهی backward_convolutional_layer(convolutional_layer layer, network net);

تهی add_bias(اعشاری *output, اعشاری *biases, صحیح batch, صحیح n, صحیح size);
تهی backward_bias(اعشاری *bias_updates, اعشاری *delta, صحیح batch, صحیح n, صحیح size);

image get_convolutional_image(convolutional_layer layer);
image get_convolutional_delta(convolutional_layer layer);
image get_convolutional_weight(convolutional_layer layer, صحیح i);

صحیح convolutional_out_height(convolutional_layer layer);
صحیح convolutional_out_width(convolutional_layer layer);

#endif

