#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ AVGPOOL_LAYER_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ AVGPOOL_LAYER_H

#ਅਨ੍ਤਰ੍ਭਾਵ "image.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "cuda.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "network.h"

ਕਿਸਮਪਰਿਭਾਸ਼ਾ layer avgpool_layer;

image get_avgpool_image(avgpool_layer l);
avgpool_layer make_avgpool_layer(ਪੂਰਨ batch, ਪੂਰਨ w, ਪੂਰਨ h, ਪੂਰਨ c);
ਖਾਲੀ resize_avgpool_layer(avgpool_layer *l, ਪੂਰਨ w, ਪੂਰਨ h);
ਖਾਲੀ forward_avgpool_layer(ਅਚਲ avgpool_layer l, ਜਾਲ net);
ਖਾਲੀ backward_avgpool_layer(ਅਚਲ avgpool_layer l, ਜਾਲ net);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
ਖਾਲੀ forward_avgpool_layer_gpu(avgpool_layer l, ਜਾਲ net);
ਖਾਲੀ backward_avgpool_layer_gpu(avgpool_layer l, ਜਾਲ net);
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ

