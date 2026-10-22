#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ NORMALIZATION_LAYER_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NORMALIZATION_LAYER_H

#ਅਨ੍ਤਰ੍ਭਾਵ "image.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "network.h"

layer make_normalization_layer(ਪੂਰਨ batch, ਪੂਰਨ w, ਪੂਰਨ h, ਪੂਰਨ c, ਪੂਰਨ ਮਾਪ, ਦਸ਼ਮਲਵ alpha, ਦਸ਼ਮਲਵ beta, ਦਸ਼ਮਲਵ kappa);
ਖਾਲੀ resize_normalization_layer(layer *layer, ਪੂਰਨ h, ਪੂਰਨ w);
ਖਾਲੀ forward_normalization_layer(ਅਚਲ layer layer, ਜਾਲ net);
ਖਾਲੀ backward_normalization_layer(ਅਚਲ layer layer, ਜਾਲ net);
ਖਾਲੀ visualize_normalization_layer(layer layer, ਅੱਖਰ *ਖਿੜਕੀ);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
ਖਾਲੀ forward_normalization_layer_gpu(ਅਚਲ layer layer, ਜਾਲ net);
ਖਾਲੀ backward_normalization_layer_gpu(ਅਚਲ layer layer, ਜਾਲ net);
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ
