#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ SHORTCUT_LAYER_H
#ವ್ಯಾಖ್ಯಾನಿಸು SHORTCUT_LAYER_H

#ಅನ್ತರ್ಭಾವ "layer.h"
#ಅನ್ತರ್ಭಾವ "network.h"

layer make_shortcut_layer(ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ ಸೂಚ್ಯಂಕ, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h, ಪೂರ್ಣಾಂಕ c, ಪೂರ್ಣಾಂಕ w2, ಪೂರ್ಣಾಂಕ h2, ಪೂರ್ಣಾಂಕ c2);
ಶೂನ್ಯ forward_shortcut_layer(ಸ್ಥಿರಾಂಕ layer l, ಜಾಲ net);
ಶೂನ್ಯ backward_shortcut_layer(ಸ್ಥಿರಾಂಕ layer l, ಜಾಲ net);
ಶೂನ್ಯ resize_shortcut_layer(layer *l, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
ಶೂನ್ಯ forward_shortcut_layer_gpu(ಸ್ಥಿರಾಂಕ layer l, ಜಾಲ net);
ಶೂನ್ಯ backward_shortcut_layer_gpu(ಸ್ಥಿರಾಂಕ layer l, ಜಾಲ net);
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ
