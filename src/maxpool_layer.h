#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ MAXPOOL_LAYER_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MAXPOOL_LAYER_H

#ਅਨ੍ਤਰ੍ਭਾਵ "image.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cuda.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "network.h"

ਕਿਸਮਪਰਿਭਾਸ਼ਾ layer maxpool_layer;

image get_maxpool_image(maxpool_layer l);
maxpool_layer make_maxpool_layer(ਪੂਰਨ batch, ਪੂਰਨ h, ਪੂਰਨ w, ਪੂਰਨ c, ਪੂਰਨ ਮਾਪ, ਪੂਰਨ stride, ਪੂਰਨ padding);
ਖਾਲੀ resize_maxpool_layer(maxpool_layer *l, ਪੂਰਨ w, ਪੂਰਨ h);
ਖਾਲੀ forward_maxpool_layer(ਅਚਲ maxpool_layer l, ਜਾਲ net);
ਖਾਲੀ backward_maxpool_layer(ਅਚਲ maxpool_layer l, ਜਾਲ net);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
ਖਾਲੀ forward_maxpool_layer_gpu(maxpool_layer l, ਜਾਲ net);
ਖਾਲੀ backward_maxpool_layer_gpu(maxpool_layer l, ਜਾਲ net);
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ

