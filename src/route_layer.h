#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ ROUTE_LAYER_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ROUTE_LAYER_H
#ਅਨ੍ਤਰ੍ਭਾਵ "network.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "layer.h"

ਕਿਸਮਪਰਿਭਾਸ਼ਾ layer route_layer;

route_layer make_route_layer(ਪੂਰਨ batch, ਪੂਰਨ n, ਪੂਰਨ *ਨਿਵੇਸ਼_layers, ਪੂਰਨ *ਨਿਵੇਸ਼_ਮਾਪ);
ਖਾਲੀ forward_route_layer(ਅਚਲ route_layer l, ਜਾਲ net);
ਖਾਲੀ backward_route_layer(ਅਚਲ route_layer l, ਜਾਲ net);
ਖਾਲੀ resize_route_layer(route_layer *l, ਜਾਲ *net);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
ਖਾਲੀ forward_route_layer_gpu(ਅਚਲ route_layer l, ਜਾਲ net);
ਖਾਲੀ backward_route_layer_gpu(ਅਚਲ route_layer l, ਜਾਲ net);
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ
