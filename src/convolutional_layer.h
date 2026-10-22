#جے_بنا_تعریف CONVOLUTIONAL_LAYER_H
#تعریف_کرو CONVOLUTIONAL_LAYER_H

#ضمن "cuda.h"
#ضمن "image.h"
#ضمن "activations.h"
#ضمن "layer.h"
#ضمن "network.h"

تعريفنوع layer convolutional_layer;

#جے_تعریف_شدہ GPU
فراغ forward_convolutional_layer_gpu(convolutional_layer layer, جال net);
فراغ backward_convolutional_layer_gpu(convolutional_layer layer, جال net);
فراغ update_convolutional_layer_gpu(convolutional_layer layer, update_دلیلاں a);

فراغ ڈالو_convolutional_layer(convolutional_layer layer);
فراغ pull_convolutional_layer(convolutional_layer layer);

فراغ جوڑو_bias_gpu(عائم *output, عائم *biases, صحيح batch, صحيح n, صحيح ناپ);
فراغ backward_bias_gpu(عائم *bias_updates, عائم *delta, صحيح batch, صحيح n, صحيح ناپ);
فراغ adam_update_gpu(عائم *w, عائم *d, عائم *m, عائم *v, عائم B1, عائم B2, عائم eps, عائم decay, عائم شرح, صحيح n, صحيح batch, صحيح t);
#جے_تعریف_شدہ CUDNN
فراغ cudnn_convolutional_تیاری(layer *l);
#جے_انت
#جے_انت

convolutional_layer make_convolutional_layer(صحيح batch, صحيح h, صحيح w, صحيح c, صحيح n, صحيح groups, صحيح ناپ, صحيح stride, صحيح padding, ACTIVATION activation, صحيح batch_normalize, صحيح ثنائی, صحيح xnor, صحيح adam);
فراغ resize_convolutional_layer(convolutional_layer *layer, صحيح w, صحيح h);
فراغ forward_convolutional_layer(ثابت convolutional_layer layer, جال net);
فراغ update_convolutional_layer(convolutional_layer layer, update_دلیلاں a);
image *visualize_convolutional_layer(convolutional_layer layer, حرف *کھڑکی, image *prev_weights);
فراغ binarize_weights(عائم *weights, صحيح n, صحيح ناپ, عائم *ثنائی);
فراغ وٹاندرا_ثنائی(convolutional_layer *l);
فراغ binarize_weights2(عائم *weights, صحيح n, صحيح ناپ, حرف *ثنائی, عائم *scales);

فراغ backward_convolutional_layer(convolutional_layer layer, جال net);

فراغ جوڑو_bias(عائم *output, عائم *biases, صحيح batch, صحيح n, صحيح ناپ);
فراغ backward_bias(عائم *bias_updates, عائم *delta, صحيح batch, صحيح n, صحيح ناپ);

image get_convolutional_image(convolutional_layer layer);
image get_convolutional_delta(convolutional_layer layer);
image get_convolutional_بھار(convolutional_layer layer, صحيح i);

صحيح convolutional_باہرلا_اچائی(convolutional_layer layer);
صحيح convolutional_باہرلا_چوڑائی(convolutional_layer layer);

#جے_انت

