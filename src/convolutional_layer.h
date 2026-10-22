#جيڪڏهن_وصف_نه_ٿيل CONVOLUTIONAL_LAYER_H
#وصف_ڪريو CONVOLUTIONAL_LAYER_H

#ضمن "cuda.h"
#ضمن "image.h"
#ضمن "activations.h"
#ضمن "layer.h"
#ضمن "network.h"

تعريفنوع layer convolutional_layer;

#جيڪڏهن_وصف_ٿيل GPU
فراغ forward_convolutional_layer_gpu(convolutional_layer layer, ڄار net);
فراغ backward_convolutional_layer_gpu(convolutional_layer layer, ڄار net);
فراغ update_convolutional_layer_gpu(convolutional_layer layer, update_دليلون a);

فراغ ڌڪو_convolutional_layer(convolutional_layer layer);
فراغ pull_convolutional_layer(convolutional_layer layer);

فراغ جوڙيو_bias_gpu(عائم *output, عائم *biases, صحيح batch, صحيح n, صحيح سائيز);
فراغ backward_bias_gpu(عائم *bias_updates, عائم *delta, صحيح batch, صحيح n, صحيح سائيز);
فراغ adam_update_gpu(عائم *w, عائم *d, عائم *m, عائم *v, عائم B1, عائم B2, عائم eps, عائم decay, عائم شرح, صحيح n, صحيح batch, صحيح t);
#جيڪڏهن_وصف_ٿيل CUDNN
فراغ cudnn_convolutional_تياري(layer *l);
#جيڪڏهن_پورو
#جيڪڏهن_پورو

convolutional_layer make_convolutional_layer(صحيح batch, صحيح h, صحيح w, صحيح c, صحيح n, صحيح groups, صحيح سائيز, صحيح stride, صحيح padding, ACTIVATION activation, صحيح batch_normalize, صحيح ٻٽو, صحيح xnor, صحيح adam);
فراغ resize_convolutional_layer(convolutional_layer *layer, صحيح w, صحيح h);
فراغ forward_convolutional_layer(ثابت convolutional_layer layer, ڄار net);
فراغ update_convolutional_layer(convolutional_layer layer, update_دليلون a);
image *visualize_convolutional_layer(convolutional_layer layer, حرف *دري, image *prev_weights);
فراغ binarize_weights(عائم *weights, صحيح n, صحيح سائيز, عائم *ٻٽو);
فراغ مٽا_سٽا_ٻٽو(convolutional_layer *l);
فراغ binarize_weights2(عائم *weights, صحيح n, صحيح سائيز, حرف *ٻٽو, عائم *scales);

فراغ backward_convolutional_layer(convolutional_layer layer, ڄار net);

فراغ جوڙيو_bias(عائم *output, عائم *biases, صحيح batch, صحيح n, صحيح سائيز);
فراغ backward_bias(عائم *bias_updates, عائم *delta, صحيح batch, صحيح n, صحيح سائيز);

image get_convolutional_image(convolutional_layer layer);
image get_convolutional_delta(convolutional_layer layer);
image get_convolutional_وزن(convolutional_layer layer, صحيح i);

صحيح convolutional_ٻاهريون_اوچائي(convolutional_layer layer);
صحيح convolutional_ٻاهريون_ويڪر(convolutional_layer layer);

#جيڪڏهن_پورو

