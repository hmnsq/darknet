#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ DETECTION_LAYER_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DETECTION_LAYER_H

#ਅਨ੍ਤਰ੍ਭਾਵ "layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "network.h"

ਕਿਸਮਪਰਿਭਾਸ਼ਾ layer detection_layer;

detection_layer make_detection_layer(ਪੂਰਨ batch, ਪੂਰਨ inputs, ਪੂਰਨ n, ਪੂਰਨ ਮਾਪ, ਪੂਰਨ classes, ਪੂਰਨ coords, ਪੂਰਨ rescore);
ਖਾਲੀ forward_detection_layer(ਅਚਲ detection_layer l, ਜਾਲ net);
ਖਾਲੀ backward_detection_layer(ਅਚਲ detection_layer l, ਜਾਲ net);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
ਖਾਲੀ forward_detection_layer_gpu(ਅਚਲ detection_layer l, ਜਾਲ net);
ਖਾਲੀ backward_detection_layer_gpu(detection_layer l, ਜਾਲ net);
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ
