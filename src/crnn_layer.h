
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CRNN_LAYER_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CRNN_LAYER_H

#ਅਨ੍ਤਰ੍ਭਾਵ "activations.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "network.h"

layer make_crnn_layer(ਪੂਰਨ batch, ਪੂਰਨ h, ਪੂਰਨ w, ਪੂਰਨ c, ਪੂਰਨ hidden_filters, ਪੂਰਨ output_filters, ਪੂਰਨ ਕਦਮਾਂ, ACTIVATION activation, ਪੂਰਨ batch_normalize);

ਖਾਲੀ forward_crnn_layer(layer l, ਜਾਲ net);
ਖਾਲੀ backward_crnn_layer(layer l, ਜਾਲ net);
ਖਾਲੀ update_crnn_layer(layer l, update_ਦਲੀਲਾਂ a);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
ਖਾਲੀ forward_crnn_layer_gpu(layer l, ਜਾਲ net);
ਖਾਲੀ backward_crnn_layer_gpu(layer l, ਜਾਲ net);
ਖਾਲੀ update_crnn_layer_gpu(layer l, update_ਦਲੀਲਾਂ a);
ਖਾਲੀ ਧੱਕੋ_crnn_layer(layer l);
ਖਾਲੀ pull_crnn_layer(layer l);
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ

