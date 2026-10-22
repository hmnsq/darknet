#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ CROP_LAYER_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ CROP_LAYER_H

#ਅਨ੍ਤਰ੍ਭਾਵ "image.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "layer.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "network.h"

ਕਿਸਮਪਰਿਭਾਸ਼ਾ layer crop_layer;

image get_crop_image(crop_layer l);
crop_layer make_crop_layer(ਪੂਰਨ batch, ਪੂਰਨ h, ਪੂਰਨ w, ਪੂਰਨ c, ਪੂਰਨ crop_ਉਚਾਈ, ਪੂਰਨ crop_ਚੌੜਾਈ, ਪੂਰਨ flip, ਦਸ਼ਮਲਵ angle, ਦਸ਼ਮਲਵ saturation, ਦਸ਼ਮਲਵ exposure);
ਖਾਲੀ forward_crop_layer(ਅਚਲ crop_layer l, ਜਾਲ net);
ਖਾਲੀ resize_crop_layer(layer *l, ਪੂਰਨ w, ਪੂਰਨ h);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ GPU
ਖਾਲੀ forward_crop_layer_gpu(crop_layer l, ਜਾਲ net);
#ਜੇ_ਅੰਤ

#ਜੇ_ਅੰਤ

