#که_نه_تعریف_شوی CONVOLUTIONAL_LAYER_H
#تعریف_کړه CONVOLUTIONAL_LAYER_H

#ضمن "cuda.h"
#ضمن "image.h"
#ضمن "activations.h"
#ضمن "layer.h"
#ضمن "network.h"

تعريفنوع layer convolutional_layer;

#که_تعریف_شوی GPU
فراغ forward_convolutional_layer_gpu(convolutional_layer layer, شبکه net);
فراغ backward_convolutional_layer_gpu(convolutional_layer layer, شبکه net);
فراغ update_convolutional_layer_gpu(convolutional_layer layer, update_دلیلونه a);

فراغ ټېل_convolutional_layer(convolutional_layer layer);
فراغ pull_convolutional_layer(convolutional_layer layer);

فراغ جمع_کړه_bias_gpu(عائم *output, عائم *biases, صحيح batch, صحيح n, صحيح اندازه);
فراغ backward_bias_gpu(عائم *bias_updates, عائم *delta, صحيح batch, صحيح n, صحيح اندازه);
فراغ adam_update_gpu(عائم *w, عائم *d, عائم *m, عائم *v, عائم B1, عائم B2, عائم eps, عائم decay, عائم شرح, صحيح n, صحيح batch, صحيح t);
#که_تعریف_شوی CUDNN
فراغ cudnn_convolutional_تیاری(layer *l);
#د_که_پای
#د_که_پای

convolutional_layer make_convolutional_layer(صحيح batch, صحيح h, صحيح w, صحيح c, صحيح n, صحيح groups, صحيح اندازه, صحيح stride, صحيح padding, ACTIVATION activation, صحيح batch_normalize, صحيح دوګونی, صحيح xnor, صحيح adam);
فراغ resize_convolutional_layer(convolutional_layer *layer, صحيح w, صحيح h);
فراغ forward_convolutional_layer(ثابت convolutional_layer layer, شبکه net);
فراغ update_convolutional_layer(convolutional_layer layer, update_دلیلونه a);
image *visualize_convolutional_layer(convolutional_layer layer, حرف *کړکۍ, image *prev_weights);
فراغ binarize_weights(عائم *weights, صحيح n, صحيح اندازه, عائم *دوګونی);
فراغ بدلون_دوګونی(convolutional_layer *l);
فراغ binarize_weights2(عائم *weights, صحيح n, صحيح اندازه, حرف *دوګونی, عائم *scales);

فراغ backward_convolutional_layer(convolutional_layer layer, شبکه net);

فراغ جمع_کړه_bias(عائم *output, عائم *biases, صحيح batch, صحيح n, صحيح اندازه);
فراغ backward_bias(عائم *bias_updates, عائم *delta, صحيح batch, صحيح n, صحيح اندازه);

image get_convolutional_image(convolutional_layer layer);
image get_convolutional_delta(convolutional_layer layer);
image get_convolutional_وزن(convolutional_layer layer, صحيح i);

صحيح convolutional_بهر_لوړوالی(convolutional_layer layer);
صحيح convolutional_بهر_پلنوالی(convolutional_layer layer);

#د_که_پای

