#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ L2NORM_LAYER_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ L2NORM_LAYER_H
#ਅਨ੍ਤਰ੍ਭਾਵ "layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "network.h"

layer make_l2norm_layer(ਪੂਰਨ batch, ਪੂਰਨ inputs);
ਖਾਲੀ forward_l2norm_layer(ਅਚਲ layer l, ਜਾਲ net);
ਖਾਲੀ backward_l2norm_layer(ਅਚਲ layer l, ਜਾਲ net);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
ਖਾਲੀ forward_l2norm_layer_gpu(ਅਚਲ layer l, ਜਾਲ net);
ਖਾਲੀ backward_l2norm_layer_gpu(ਅਚਲ layer l, ਜਾਲ net);
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ
