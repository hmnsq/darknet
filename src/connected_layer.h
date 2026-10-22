#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CONNECTED_LAYER_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CONNECTED_LAYER_H

#ਅਨ੍ਤਰ੍ਭਾਵ "activations.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "network.h"

layer make_connected_layer(ਪੂਰਨ batch, ਪੂਰਨ inputs, ਪੂਰਨ outputs, ACTIVATION activation, ਪੂਰਨ batch_normalize, ਪੂਰਨ adam);

ਖਾਲੀ forward_connected_layer(layer l, ਜਾਲ net);
ਖਾਲੀ backward_connected_layer(layer l, ਜਾਲ net);
ਖਾਲੀ update_connected_layer(layer l, update_ਦਲੀਲਾਂ a);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
ਖਾਲੀ forward_connected_layer_gpu(layer l, ਜਾਲ net);
ਖਾਲੀ backward_connected_layer_gpu(layer l, ਜਾਲ net);
ਖਾਲੀ update_connected_layer_gpu(layer l, update_ਦਲੀਲਾਂ a);
ਖਾਲੀ ਧੱਕੋ_connected_layer(layer l);
ਖਾਲੀ pull_connected_layer(layer l);
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ

