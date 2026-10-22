#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ SHORTCUT_LAYER_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SHORTCUT_LAYER_H

#ਅਨ੍ਤਰ੍ਭਾਵ "layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "network.h"

layer make_shortcut_layer(ਪੂਰਨ batch, ਪੂਰਨ ਸੂਚਕਾਂਕ, ਪੂਰਨ w, ਪੂਰਨ h, ਪੂਰਨ c, ਪੂਰਨ w2, ਪੂਰਨ h2, ਪੂਰਨ c2);
ਖਾਲੀ forward_shortcut_layer(ਅਚਲ layer l, ਜਾਲ net);
ਖਾਲੀ backward_shortcut_layer(ਅਚਲ layer l, ਜਾਲ net);
ਖਾਲੀ resize_shortcut_layer(layer *l, ਪੂਰਨ w, ਪੂਰਨ h);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
ਖਾਲੀ forward_shortcut_layer_gpu(ਅਚਲ layer l, ਜਾਲ net);
ਖਾਲੀ backward_shortcut_layer_gpu(ਅਚਲ layer l, ਜਾਲ net);
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ
