#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ DECONVOLUTIONAL_LAYER_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DECONVOLUTIONAL_LAYER_H

#ਅਨ੍ਤਰ੍ਭਾਵ "cuda.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "image.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "activations.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "network.h"

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
ਖਾਲੀ forward_deconvolutional_layer_gpu(layer l, ਜਾਲ net);
ਖਾਲੀ backward_deconvolutional_layer_gpu(layer l, ਜਾਲ net);
ਖਾਲੀ update_deconvolutional_layer_gpu(layer l, update_ਦਲੀਲਾਂ a);
ਖਾਲੀ ਧੱਕੋ_deconvolutional_layer(layer l);
ਖਾਲੀ pull_deconvolutional_layer(layer l);
#ਜੇ_ਅੰਤ

layer make_deconvolutional_layer(ਪੂਰਨ batch, ਪੂਰਨ h, ਪੂਰਨ w, ਪੂਰਨ c, ਪੂਰਨ n, ਪੂਰਨ ਮਾਪ, ਪੂਰਨ stride, ਪੂਰਨ padding, ACTIVATION activation, ਪੂਰਨ batch_normalize, ਪੂਰਨ adam);
ਖਾਲੀ resize_deconvolutional_layer(layer *l, ਪੂਰਨ h, ਪੂਰਨ w);
ਖਾਲੀ forward_deconvolutional_layer(ਅਚਲ layer l, ਜਾਲ net);
ਖਾਲੀ update_deconvolutional_layer(layer l, update_ਦਲੀਲਾਂ a);
ਖਾਲੀ backward_deconvolutional_layer(layer l, ਜਾਲ net);

#ਜੇ_ਅੰਤ

