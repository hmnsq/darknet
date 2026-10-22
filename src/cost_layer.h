#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ਲਾਗਤ_LAYER_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ਲਾਗਤ_LAYER_H
#ਅਨ੍ਤਰ੍ਭਾਵ "layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "network.h"

ਕਿਸਮਪਰਿਭਾਸ਼ਾ layer ਲਾਗਤ_layer;

ਲਾਗਤ_TYPE get_ਲਾਗਤ_type(ਅੱਖਰ *s);
ਅੱਖਰ *get_ਲਾਗਤ_string(ਲਾਗਤ_TYPE a);
ਲਾਗਤ_layer make_ਲਾਗਤ_layer(ਪੂਰਨ batch, ਪੂਰਨ inputs, ਲਾਗਤ_TYPE type, ਦਸ਼ਮਲਵ scale);
ਖਾਲੀ forward_ਲਾਗਤ_layer(ਅਚਲ ਲਾਗਤ_layer l, ਜਾਲ net);
ਖਾਲੀ backward_ਲਾਗਤ_layer(ਅਚਲ ਲਾਗਤ_layer l, ਜਾਲ net);
ਖਾਲੀ resize_ਲਾਗਤ_layer(ਲਾਗਤ_layer *l, ਪੂਰਨ inputs);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
ਖਾਲੀ forward_ਲਾਗਤ_layer_gpu(ਲਾਗਤ_layer l, ਜਾਲ net);
ਖਾਲੀ backward_ਲਾਗਤ_layer_gpu(ਅਚਲ ਲਾਗਤ_layer l, ਜਾਲ net);
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ
