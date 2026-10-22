#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ UPSAMPLE_LAYER_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ UPSAMPLE_LAYER_H
#ਅਨ੍ਤਰ੍ਭਾਵ "darknet.h"

layer make_upsample_layer(ਪੂਰਨ batch, ਪੂਰਨ w, ਪੂਰਨ h, ਪੂਰਨ c, ਪੂਰਨ stride);
ਖਾਲੀ forward_upsample_layer(ਅਚਲ layer l, ਜਾਲ net);
ਖਾਲੀ backward_upsample_layer(ਅਚਲ layer l, ਜਾਲ net);
ਖਾਲੀ resize_upsample_layer(layer *l, ਪੂਰਨ w, ਪੂਰਨ h);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
ਖਾਲੀ forward_upsample_layer_gpu(ਅਚਲ layer l, ਜਾਲ net);
ਖਾਲੀ backward_upsample_layer_gpu(ਅਚਲ layer l, ਜਾਲ net);
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ
