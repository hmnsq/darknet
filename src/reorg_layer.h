#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ REORG_LAYER_H
#ವ್ಯಾಖ್ಯಾನಿಸು REORG_LAYER_H

#ಅನ್ತರ್ಭಾವ "image.h"
#ಅನ್ತರ್ಭಾವ "cuda.h"
#ಅನ್ತರ್ಭಾವ "layer.h"
#ಅನ್ತರ್ಭಾವ "network.h"

layer make_reorg_layer(ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h, ಪೂರ್ಣಾಂಕ c, ಪೂರ್ಣಾಂಕ stride, ಪೂರ್ಣಾಂಕ ಹಿಮ್ಮುಖ, ಪೂರ್ಣಾಂಕ flatten, ಪೂರ್ಣಾಂಕ extra);
ಶೂನ್ಯ resize_reorg_layer(layer *l, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h);
ಶೂನ್ಯ forward_reorg_layer(ಸ್ಥಿರಾಂಕ layer l, ಜಾಲ net);
ಶೂನ್ಯ backward_reorg_layer(ಸ್ಥಿರಾಂಕ layer l, ಜಾಲ net);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
ಶೂನ್ಯ forward_reorg_layer_gpu(layer l, ಜಾಲ net);
ಶೂನ್ಯ backward_reorg_layer_gpu(layer l, ಜಾಲ net);
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ

