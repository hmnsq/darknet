#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CROP_LAYER_H
#ವ್ಯಾಖ್ಯಾನಿಸು CROP_LAYER_H

#ಅನ್ತರ್ಭಾವ "image.h"
#ಅನ್ತರ್ಭಾವ "layer.h"
#ಅನ್ತರ್ಭಾವ "network.h"

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ layer crop_layer;

image get_crop_image(crop_layer l);
crop_layer make_crop_layer(ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ h, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ c, ಪೂರ್ಣಾಂಕ crop_ಎತ್ತರ, ಪೂರ್ಣಾಂಕ crop_ಅಗಲ, ಪೂರ್ಣಾಂಕ flip, ದಶಮಾಂಶ angle, ದಶಮಾಂಶ saturation, ದಶಮಾಂಶ exposure);
ಶೂನ್ಯ forward_crop_layer(ಸ್ಥಿರಾಂಕ crop_layer l, ಜಾಲ net);
ಶೂನ್ಯ resize_crop_layer(layer *l, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
ಶೂನ್ಯ forward_crop_layer_gpu(crop_layer l, ಜಾಲ net);
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ

