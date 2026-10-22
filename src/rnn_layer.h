
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ RNN_LAYER_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ RNN_LAYER_H

#ਅਨ੍ਤਰ੍ਭਾਵ "activations.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "network.h"
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ USET

layer make_rnn_layer(ਪੂਰਨ batch, ਪੂਰਨ inputs, ਪੂਰਨ outputs, ਪੂਰਨ ਕਦਮਾਂ, ACTIVATION activation, ਪੂਰਨ batch_normalize, ਪੂਰਨ adam);

ਖਾਲੀ forward_rnn_layer(layer l, ਜਾਲ net);
ਖਾਲੀ backward_rnn_layer(layer l, ਜਾਲ net);
ਖਾਲੀ update_rnn_layer(layer l, update_ਦਲੀਲਾਂ a);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
ਖਾਲੀ forward_rnn_layer_gpu(layer l, ਜਾਲ net);
ਖਾਲੀ backward_rnn_layer_gpu(layer l, ਜਾਲ net);
ਖਾਲੀ update_rnn_layer_gpu(layer l, update_ਦਲੀਲਾਂ a);
ਖਾਲੀ ਧੱਕੋ_rnn_layer(layer l);
ਖਾਲੀ pull_rnn_layer(layer l);
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ

