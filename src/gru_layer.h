
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ GRU_LAYER_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GRU_LAYER_H

#ਅਨ੍ਤਰ੍ਭਾਵ "activations.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "network.h"

layer make_gru_layer(ਪੂਰਨ batch, ਪੂਰਨ inputs, ਪੂਰਨ outputs, ਪੂਰਨ ਕਦਮਾਂ, ਪੂਰਨ batch_normalize, ਪੂਰਨ adam);

ਖਾਲੀ forward_gru_layer(layer l, ਜਾਲ state);
ਖਾਲੀ backward_gru_layer(layer l, ਜਾਲ state);
ਖਾਲੀ update_gru_layer(layer l, update_ਦਲੀਲਾਂ a);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
ਖਾਲੀ forward_gru_layer_gpu(layer l, ਜਾਲ state);
ਖਾਲੀ backward_gru_layer_gpu(layer l, ਜਾਲ state);
ਖਾਲੀ update_gru_layer_gpu(layer l, update_ਦਲੀਲਾਂ a);
ਖਾਲੀ ਧੱਕੋ_gru_layer(layer l);
ਖਾਲੀ pull_gru_layer(layer l);
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ

