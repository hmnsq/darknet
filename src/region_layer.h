#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ REGION_LAYER_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ REGION_LAYER_H

#ਅਨ੍ਤਰ੍ਭਾਵ "darknet.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "network.h"

layer make_region_layer(ਪੂਰਨ batch, ਪੂਰਨ w, ਪੂਰਨ h, ਪੂਰਨ n, ਪੂਰਨ classes, ਪੂਰਨ coords);
ਖਾਲੀ forward_region_layer(ਅਚਲ layer l, ਜਾਲ net);
ਖਾਲੀ backward_region_layer(ਅਚਲ layer l, ਜਾਲ net);
ਖਾਲੀ resize_region_layer(layer *l, ਪੂਰਨ w, ਪੂਰਨ h);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
ਖਾਲੀ forward_region_layer_gpu(ਅਚਲ layer l, ਜਾਲ net);
ਖਾਲੀ backward_region_layer_gpu(layer l, ਜਾਲ net);
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ
