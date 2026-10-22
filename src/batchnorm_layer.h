#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ BATCHNORM_LAYER_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ BATCHNORM_LAYER_H

#ਅਨ੍ਤਰ੍ਭਾਵ "image.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "network.h"

layer make_batchnorm_layer(ਪੂਰਨ batch, ਪੂਰਨ w, ਪੂਰਨ h, ਪੂਰਨ c);
ਖਾਲੀ forward_batchnorm_layer(layer l, ਜਾਲ net);
ਖਾਲੀ backward_batchnorm_layer(layer l, ਜਾਲ net);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
ਖਾਲੀ forward_batchnorm_layer_gpu(layer l, ਜਾਲ net);
ਖਾਲੀ backward_batchnorm_layer_gpu(layer l, ਜਾਲ net);
ਖਾਲੀ pull_batchnorm_layer(layer l);
ਖਾਲੀ ਧੱਕੋ_batchnorm_layer(layer l);
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ
