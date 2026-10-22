#اگر_تعریف_نشده CONVOLUTIONAL_LAYER_H
#تعریف_کن CONVOLUTIONAL_LAYER_H

#ضمن "cuda.h"
#ضمن "image.h"
#ضمن "activations.h"
#ضمن "layer.h"
#ضمن "network.h"

تعريفنوع layer convolutional_layer;

#اگر_تعریف_شده GPU
فراغ forward_convolutional_layer_gpu(convolutional_layer layer, شبکه net);
فراغ backward_convolutional_layer_gpu(convolutional_layer layer, شبکه net);
فراغ update_convolutional_layer_gpu(convolutional_layer layer, update_آرگومانها a);

فراغ افزودن_convolutional_layer(convolutional_layer layer);
فراغ pull_convolutional_layer(convolutional_layer layer);

فراغ جمع_کن_bias_gpu(عائم *output, عائم *biases, صحيح batch, صحيح n, صحيح اندازه);
فراغ backward_bias_gpu(عائم *bias_updates, عائم *delta, صحيح batch, صحيح n, صحيح اندازه);
فراغ adam_update_gpu(عائم *w, عائم *d, عائم *m, عائم *v, عائم B1, عائم B2, عائم eps, عائم decay, عائم نرخ, صحيح n, صحيح batch, صحيح t);
#اگر_تعریف_شده CUDNN
فراغ cudnn_convolutional_راهاندازی(layer *l);
#پایان_اگر
#پایان_اگر

convolutional_layer make_convolutional_layer(صحيح batch, صحيح h, صحيح w, صحيح c, صحيح n, صحيح groups, صحيح اندازه, صحيح stride, صحيح padding, ACTIVATION activation, صحيح batch_normalize, صحيح دودویی, صحيح xnor, صحيح adam);
فراغ resize_convolutional_layer(convolutional_layer *layer, صحيح w, صحيح h);
فراغ forward_convolutional_layer(ثابت convolutional_layer layer, شبکه net);
فراغ update_convolutional_layer(convolutional_layer layer, update_آرگومانها a);
image *visualize_convolutional_layer(convolutional_layer layer, حرف *پنجره, image *prev_weights);
فراغ binarize_weights(عائم *weights, صحيح n, صحيح اندازه, عائم *دودویی);
فراغ جابجا_دودویی(convolutional_layer *l);
فراغ binarize_weights2(عائم *weights, صحيح n, صحيح اندازه, حرف *دودویی, عائم *scales);

فراغ backward_convolutional_layer(convolutional_layer layer, شبکه net);

فراغ جمع_کن_bias(عائم *output, عائم *biases, صحيح batch, صحيح n, صحيح اندازه);
فراغ backward_bias(عائم *bias_updates, عائم *delta, صحيح batch, صحيح n, صحيح اندازه);

image get_convolutional_image(convolutional_layer layer);
image get_convolutional_delta(convolutional_layer layer);
image get_convolutional_وزن(convolutional_layer layer, صحيح i);

صحيح convolutional_بیرون_ارتفاع(convolutional_layer layer);
صحيح convolutional_بیرون_عرض(convolutional_layer layer);

#پایان_اگر

