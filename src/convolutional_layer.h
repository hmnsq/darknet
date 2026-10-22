#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CONVOLUTIONAL_LAYER_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CONVOLUTIONAL_LAYER_H

#ਅਨ੍ਤਰ੍ਭਾਵ "cuda.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "image.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "activations.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "network.h"

ਕਿਸਮਪਰਿਭਾਸ਼ਾ layer convolutional_layer;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
ਖਾਲੀ forward_convolutional_layer_gpu(convolutional_layer layer, ਜਾਲ net);
ਖਾਲੀ backward_convolutional_layer_gpu(convolutional_layer layer, ਜਾਲ net);
ਖਾਲੀ update_convolutional_layer_gpu(convolutional_layer layer, update_ਦਲੀਲਾਂ a);

ਖਾਲੀ ਧੱਕੋ_convolutional_layer(convolutional_layer layer);
ਖਾਲੀ pull_convolutional_layer(convolutional_layer layer);

ਖਾਲੀ ਜੋੜੋ_bias_gpu(ਦਸ਼ਮਲਵ *output, ਦਸ਼ਮਲਵ *biases, ਪੂਰਨ batch, ਪੂਰਨ n, ਪੂਰਨ ਮਾਪ);
ਖਾਲੀ backward_bias_gpu(ਦਸ਼ਮਲਵ *bias_updates, ਦਸ਼ਮਲਵ *delta, ਪੂਰਨ batch, ਪੂਰਨ n, ਪੂਰਨ ਮਾਪ);
ਖਾਲੀ adam_update_gpu(ਦਸ਼ਮਲਵ *w, ਦਸ਼ਮਲਵ *d, ਦਸ਼ਮਲਵ *m, ਦਸ਼ਮਲਵ *v, ਦਸ਼ਮਲਵ B1, ਦਸ਼ਮਲਵ B2, ਦਸ਼ਮਲਵ eps, ਦਸ਼ਮਲਵ decay, ਦਸ਼ਮਲਵ ਦਰ, ਪੂਰਨ n, ਪੂਰਨ batch, ਪੂਰਨ t);
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ CUDNN
ਖਾਲੀ cudnn_convolutional_ਤਿਆਰੀ(layer *l);
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

convolutional_layer make_convolutional_layer(ਪੂਰਨ batch, ਪੂਰਨ h, ਪੂਰਨ w, ਪੂਰਨ c, ਪੂਰਨ n, ਪੂਰਨ groups, ਪੂਰਨ ਮਾਪ, ਪੂਰਨ stride, ਪੂਰਨ padding, ACTIVATION activation, ਪੂਰਨ batch_normalize, ਪੂਰਨ ਦੁਆਧਾਰੀ, ਪੂਰਨ xnor, ਪੂਰਨ adam);
ਖਾਲੀ resize_convolutional_layer(convolutional_layer *layer, ਪੂਰਨ w, ਪੂਰਨ h);
ਖਾਲੀ forward_convolutional_layer(ਅਚਲ convolutional_layer layer, ਜਾਲ net);
ਖਾਲੀ update_convolutional_layer(convolutional_layer layer, update_ਦਲੀਲਾਂ a);
image *visualize_convolutional_layer(convolutional_layer layer, ਅੱਖਰ *ਖਿੜਕੀ, image *prev_weights);
ਖਾਲੀ binarize_weights(ਦਸ਼ਮਲਵ *weights, ਪੂਰਨ n, ਪੂਰਨ ਮਾਪ, ਦਸ਼ਮਲਵ *ਦੁਆਧਾਰੀ);
ਖਾਲੀ ਅਦਲਾ_ਬਦਲੀ_ਦੁਆਧਾਰੀ(convolutional_layer *l);
ਖਾਲੀ binarize_weights2(ਦਸ਼ਮਲਵ *weights, ਪੂਰਨ n, ਪੂਰਨ ਮਾਪ, ਅੱਖਰ *ਦੁਆਧਾਰੀ, ਦਸ਼ਮਲਵ *scales);

ਖਾਲੀ backward_convolutional_layer(convolutional_layer layer, ਜਾਲ net);

ਖਾਲੀ ਜੋੜੋ_bias(ਦਸ਼ਮਲਵ *output, ਦਸ਼ਮਲਵ *biases, ਪੂਰਨ batch, ਪੂਰਨ n, ਪੂਰਨ ਮਾਪ);
ਖਾਲੀ backward_bias(ਦਸ਼ਮਲਵ *bias_updates, ਦਸ਼ਮਲਵ *delta, ਪੂਰਨ batch, ਪੂਰਨ n, ਪੂਰਨ ਮਾਪ);

image get_convolutional_image(convolutional_layer layer);
image get_convolutional_delta(convolutional_layer layer);
image get_convolutional_ਭਾਰ(convolutional_layer layer, ਪੂਰਨ i);

ਪੂਰਨ convolutional_ਬਾਹਰਲਾ_ਉਚਾਈ(convolutional_layer layer);
ਪੂਰਨ convolutional_ਬਾਹਰਲਾ_ਚੌੜਾਈ(convolutional_layer layer);

#ਜੇ_ਅੰਤ

