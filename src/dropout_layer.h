#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ DROPOUT_LAYER_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DROPOUT_LAYER_H

#ਅਨ੍ਤਰ੍ਭਾਵ "layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "network.h"

ਕਿਸਮਪਰਿਭਾਸ਼ਾ layer dropout_layer;

dropout_layer make_dropout_layer(ਪੂਰਨ batch, ਪੂਰਨ inputs, ਦਸ਼ਮਲਵ probability);

ਖਾਲੀ forward_dropout_layer(dropout_layer l, ਜਾਲ net);
ਖਾਲੀ backward_dropout_layer(dropout_layer l, ਜਾਲ net);
ਖਾਲੀ resize_dropout_layer(dropout_layer *l, ਪੂਰਨ inputs);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
ਖਾਲੀ forward_dropout_layer_gpu(dropout_layer l, ਜਾਲ net);
ਖਾਲੀ backward_dropout_layer_gpu(dropout_layer l, ਜਾਲ net);

#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ
