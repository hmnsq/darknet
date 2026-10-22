#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ISEG_LAYER_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ISEG_LAYER_H

#ਅਨ੍ਤਰ੍ਭਾਵ "darknet.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "network.h"

layer make_iseg_layer(ਪੂਰਨ batch, ਪੂਰਨ w, ਪੂਰਨ h, ਪੂਰਨ classes, ਪੂਰਨ ids);
ਖਾਲੀ forward_iseg_layer(ਅਚਲ layer l, ਜਾਲ net);
ਖਾਲੀ backward_iseg_layer(ਅਚਲ layer l, ਜਾਲ net);
ਖਾਲੀ resize_iseg_layer(layer *l, ਪੂਰਨ w, ਪੂਰਨ h);
ਪੂਰਨ iseg_ਅੰਕ_detections(layer l, ਦਸ਼ਮਲਵ thresh);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
ਖਾਲੀ forward_iseg_layer_gpu(ਅਚਲ layer l, ਜਾਲ net);
ਖਾਲੀ backward_iseg_layer_gpu(layer l, ਜਾਲ net);
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ
