#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ LOCAL_LAYER_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LOCAL_LAYER_H

#ਅਨ੍ਤਰ੍ਭਾਵ "cuda.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "image.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "activations.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "network.h"

ਕਿਸਮਪਰਿਭਾਸ਼ਾ layer local_layer;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
ਖਾਲੀ forward_local_layer_gpu(local_layer layer, ਜਾਲ net);
ਖਾਲੀ backward_local_layer_gpu(local_layer layer, ਜਾਲ net);
ਖਾਲੀ update_local_layer_gpu(local_layer layer, update_ਦਲੀਲਾਂ a);

ਖਾਲੀ ਧੱਕੋ_local_layer(local_layer layer);
ਖਾਲੀ pull_local_layer(local_layer layer);
#ਜੇ_ਅੰਤ

local_layer make_local_layer(ਪੂਰਨ batch, ਪੂਰਨ h, ਪੂਰਨ w, ਪੂਰਨ c, ਪੂਰਨ n, ਪੂਰਨ ਮਾਪ, ਪੂਰਨ stride, ਪੂਰਨ pad, ACTIVATION activation);

ਖਾਲੀ forward_local_layer(ਅਚਲ local_layer layer, ਜਾਲ net);
ਖਾਲੀ backward_local_layer(local_layer layer, ਜਾਲ net);
ਖਾਲੀ update_local_layer(local_layer layer, update_ਦਲੀਲਾਂ a);

ਖਾਲੀ bias_output(ਦਸ਼ਮਲਵ *output, ਦਸ਼ਮਲਵ *biases, ਪੂਰਨ batch, ਪੂਰਨ n, ਪੂਰਨ ਮਾਪ);
ਖਾਲੀ backward_bias(ਦਸ਼ਮਲਵ *bias_updates, ਦਸ਼ਮਲਵ *delta, ਪੂਰਨ batch, ਪੂਰਨ n, ਪੂਰਨ ਮਾਪ);

#ਜੇ_ਅੰਤ

