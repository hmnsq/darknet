#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ YOLO_LAYER_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ YOLO_LAYER_H

#ਅਨ੍ਤਰ੍ਭਾਵ "darknet.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "network.h"

layer make_yolo_layer(ਪੂਰਨ batch, ਪੂਰਨ w, ਪੂਰਨ h, ਪੂਰਨ n, ਪੂਰਨ ਕੁੱਲ, ਪੂਰਨ *ਨਕਾਬ, ਪੂਰਨ classes);
ਖਾਲੀ forward_yolo_layer(ਅਚਲ layer l, ਜਾਲ net);
ਖਾਲੀ backward_yolo_layer(ਅਚਲ layer l, ਜਾਲ net);
ਖਾਲੀ resize_yolo_layer(layer *l, ਪੂਰਨ w, ਪੂਰਨ h);
ਪੂਰਨ yolo_ਅੰਕ_detections(layer l, ਦਸ਼ਮਲਵ thresh);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
ਖਾਲੀ forward_yolo_layer_gpu(ਅਚਲ layer l, ਜਾਲ net);
ਖਾਲੀ backward_yolo_layer_gpu(layer l, ਜਾਲ net);
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ
