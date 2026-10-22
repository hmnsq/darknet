#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ NORMALIZATION_LAYER_H
#ವ್ಯಾಖ್ಯಾನಿಸು NORMALIZATION_LAYER_H

#ಅನ್ತರ್ಭಾವ "image.h"
#ಅನ್ತರ್ಭಾವ "layer.h"
#ಅನ್ತರ್ಭಾವ "network.h"

layer make_normalization_layer(ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h, ಪೂರ್ಣಾಂಕ c, ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2, ದಶಮಾಂಶ alpha, ದಶಮಾಂಶ beta, ದಶಮಾಂಶ kappa);
ಶೂನ್ಯ resize_normalization_layer(layer *layer, ಪೂರ್ಣಾಂಕ h, ಪೂರ್ಣಾಂಕ w);
ಶೂನ್ಯ forward_normalization_layer(ಸ್ಥಿರಾಂಕ layer layer, ಜಾಲ net);
ಶೂನ್ಯ backward_normalization_layer(ಸ್ಥಿರಾಂಕ layer layer, ಜಾಲ net);
ಶೂನ್ಯ visualize_normalization_layer(layer layer, ಅಕ್ಷರ *ಕಿಟಕಿ);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
ಶೂನ್ಯ forward_normalization_layer_gpu(ಸ್ಥಿರಾಂಕ layer layer, ಜಾಲ net);
ಶೂನ್ಯ backward_normalization_layer_gpu(ಸ್ಥಿರಾಂಕ layer layer, ಜಾಲ net);
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ
