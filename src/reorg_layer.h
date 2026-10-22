#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ REORG_LAYER_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ REORG_LAYER_H

#ਅਨ੍ਤਰ੍ਭਾਵ "image.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cuda.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "network.h"

layer make_reorg_layer(ਪੂਰਨ batch, ਪੂਰਨ w, ਪੂਰਨ h, ਪੂਰਨ c, ਪੂਰਨ stride, ਪੂਰਨ ਉਲਟ, ਪੂਰਨ flatten, ਪੂਰਨ extra);
ਖਾਲੀ resize_reorg_layer(layer *l, ਪੂਰਨ w, ਪੂਰਨ h);
ਖਾਲੀ forward_reorg_layer(ਅਚਲ layer l, ਜਾਲ net);
ਖਾਲੀ backward_reorg_layer(ਅਚਲ layer l, ਜਾਲ net);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
ਖਾਲੀ forward_reorg_layer_gpu(layer l, ਜਾਲ net);
ਖਾਲੀ backward_reorg_layer_gpu(layer l, ਜਾਲ net);
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ

