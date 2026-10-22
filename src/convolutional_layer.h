#ifndef CONVOLUTIONAL_LAYER_H
#define CONVOLUTIONAL_LAYER_H

#ضمن "cuda.h"
#ضمن "image.h"
#ضمن "activations.h"
#ضمن "layer.h"
#ضمن "network.h"

تعريفنوع layer convolutional_layer;

#ifdef GPU
فراغ forward_convolutional_layer_gpu(convolutional_layer layer, network net);
فراغ backward_convolutional_layer_gpu(convolutional_layer layer, network net);
فراغ update_convolutional_layer_gpu(convolutional_layer layer, update_args a);

فراغ پاڵنان_convolutional_layer(convolutional_layer layer);
فراغ pull_convolutional_layer(convolutional_layer layer);

فراغ add_bias_gpu(عائم *output, عائم *biases, صحيح batch, صحيح n, صحيح size);
فراغ backward_bias_gpu(عائم *bias_updates, عائم *delta, صحيح batch, صحيح n, صحيح size);
فراغ adam_update_gpu(عائم *w, عائم *d, عائم *m, عائم *v, عائم B1, عائم B2, عائم eps, عائم decay, عائم rate, صحيح n, صحيح batch, صحيح t);
#ifdef CUDNN
فراغ cudnn_convolutional_setup(layer *l);
#endif
#endif

convolutional_layer make_convolutional_layer(صحيح batch, صحيح h, صحيح w, صحيح c, صحيح n, صحيح groups, صحيح size, صحيح stride, صحيح padding, ACTIVATION activation, صحيح batch_normalize, صحيح دووانەیی, صحيح xnor, صحيح adam);
فراغ resize_convolutional_layer(convolutional_layer *layer, صحيح w, صحيح h);
فراغ forward_convolutional_layer(ثابت convolutional_layer layer, network net);
فراغ update_convolutional_layer(convolutional_layer layer, update_args a);
image *visualize_convolutional_layer(convolutional_layer layer, حرف *window, image *prev_weights);
فراغ binarize_weights(عائم *weights, صحيح n, صحيح size, عائم *دووانەیی);
فراغ swap_دووانەیی(convolutional_layer *l);
فراغ binarize_weights2(عائم *weights, صحيح n, صحيح size, حرف *دووانەیی, عائم *scales);

فراغ backward_convolutional_layer(convolutional_layer layer, network net);

فراغ add_bias(عائم *output, عائم *biases, صحيح batch, صحيح n, صحيح size);
فراغ backward_bias(عائم *bias_updates, عائم *delta, صحيح batch, صحيح n, صحيح size);

image get_convolutional_image(convolutional_layer layer);
image get_convolutional_delta(convolutional_layer layer);
image get_convolutional_weight(convolutional_layer layer, صحيح i);

صحيح convolutional_out_height(convolutional_layer layer);
صحيح convolutional_out_width(convolutional_layer layer);

#endif

