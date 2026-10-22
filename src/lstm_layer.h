#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ LSTM_LAYER_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LSTM_LAYER_H

#ਅਨ੍ਤਰ੍ਭਾਵ "activations.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "network.h"
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USET

layer make_lstm_layer(ਪੂਰਨ batch, ਪੂਰਨ inputs, ਪੂਰਨ outputs, ਪੂਰਨ ਕਦਮਾਂ, ਪੂਰਨ batch_normalize, ਪੂਰਨ adam);

ਖਾਲੀ forward_lstm_layer(layer l, ਜਾਲ net); 
ਖਾਲੀ update_lstm_layer(layer l, update_ਦਲੀਲਾਂ a);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
ਖਾਲੀ forward_lstm_layer_gpu(layer l, ਜਾਲ net);
ਖਾਲੀ backward_lstm_layer_gpu(layer l, ਜਾਲ net);
ਖਾਲੀ update_lstm_layer_gpu(layer l, update_ਦਲੀਲਾਂ a); 

#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ
