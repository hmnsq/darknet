#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ LOGISTIC_LAYER_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LOGISTIC_LAYER_H
#ਅਨ੍ਤਰ੍ਭਾਵ "layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "network.h"

layer make_logistic_layer(ਪੂਰਨ batch, ਪੂਰਨ inputs);
ਖਾਲੀ forward_logistic_layer(ਅਚਲ layer l, ਜਾਲ net);
ਖਾਲੀ backward_logistic_layer(ਅਚਲ layer l, ਜਾਲ net);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
ਖਾਲੀ forward_logistic_layer_gpu(ਅਚਲ layer l, ਜਾਲ net);
ਖਾਲੀ backward_logistic_layer_gpu(ਅਚਲ layer l, ਜਾਲ net);
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ
