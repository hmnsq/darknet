#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ACTIVATION_LAYER_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ACTIVATION_LAYER_H

#ਅਨ੍ਤਰ੍ਭਾਵ "activations.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "network.h"

layer make_activation_layer(ਪੂਰਨ batch, ਪੂਰਨ inputs, ACTIVATION activation);

ਖਾਲੀ forward_activation_layer(layer l, ਜਾਲ net);
ਖਾਲੀ backward_activation_layer(layer l, ਜਾਲ net);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
ਖਾਲੀ forward_activation_layer_gpu(layer l, ਜਾਲ net);
ਖਾਲੀ backward_activation_layer_gpu(layer l, ਜਾਲ net);
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ

