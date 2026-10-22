#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ SOFTMAX_LAYER_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SOFTMAX_LAYER_H
#ਅਨ੍ਤਰ੍ਭਾਵ "layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "network.h"

ਕਿਸਮਪਰਿਭਾਸ਼ਾ layer softmax_layer;

ਖਾਲੀ softmax_array(ਦਸ਼ਮਲਵ *ਨਿਵੇਸ਼, ਪੂਰਨ n, ਦਸ਼ਮਲਵ ਆਰਜ਼ੀ, ਦਸ਼ਮਲਵ *output);
softmax_layer make_softmax_layer(ਪੂਰਨ batch, ਪੂਰਨ inputs, ਪੂਰਨ groups);
ਖਾਲੀ forward_softmax_layer(ਅਚਲ softmax_layer l, ਜਾਲ net);
ਖਾਲੀ backward_softmax_layer(ਅਚਲ softmax_layer l, ਜਾਲ net);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
ਖਾਲੀ pull_softmax_layer_output(ਅਚਲ softmax_layer l);
ਖਾਲੀ forward_softmax_layer_gpu(ਅਚਲ softmax_layer l, ਜਾਲ net);
ਖਾਲੀ backward_softmax_layer_gpu(ਅਚਲ softmax_layer l, ਜਾਲ net);
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ
