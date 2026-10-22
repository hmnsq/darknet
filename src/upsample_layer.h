#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ UPSAMPLE_LAYER_H
#ವ್ಯಾಖ್ಯಾನಿಸು UPSAMPLE_LAYER_H
#ಅನ್ತರ್ಭಾವ "darknet.h"

layer make_upsample_layer(ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h, ಪೂರ್ಣಾಂಕ c, ಪೂರ್ಣಾಂಕ stride);
ಶೂನ್ಯ forward_upsample_layer(ಸ್ಥಿರಾಂಕ layer l, ಜಾಲ net);
ಶೂನ್ಯ backward_upsample_layer(ಸ್ಥಿರಾಂಕ layer l, ಜಾಲ net);
ಶೂನ್ಯ resize_upsample_layer(layer *l, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
ಶೂನ್ಯ forward_upsample_layer_gpu(ಸ್ಥಿರಾಂಕ layer l, ಜಾಲ net);
ಶೂನ್ಯ backward_upsample_layer_gpu(ಸ್ಥಿರಾಂಕ layer l, ಜಾಲ net);
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ
