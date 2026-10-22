#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ MAXPOOL_LAYER_H
#ವ್ಯಾಖ್ಯಾನಿಸು MAXPOOL_LAYER_H

#ಅನ್ತರ್ಭಾವ "image.h"
#ಅನ್ತರ್ಭಾವ "cuda.h"
#ಅನ್ತರ್ಭಾವ "layer.h"
#ಅನ್ತರ್ಭಾವ "network.h"

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ layer maxpool_layer;

image get_maxpool_image(maxpool_layer l);
maxpool_layer make_maxpool_layer(ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ h, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ c, ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2, ಪೂರ್ಣಾಂಕ stride, ಪೂರ್ಣಾಂಕ padding);
ಶೂನ್ಯ resize_maxpool_layer(maxpool_layer *l, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h);
ಶೂನ್ಯ forward_maxpool_layer(ಸ್ಥಿರಾಂಕ maxpool_layer l, ಜಾಲ net);
ಶೂನ್ಯ backward_maxpool_layer(ಸ್ಥಿರಾಂಕ maxpool_layer l, ಜಾಲ net);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
ಶೂನ್ಯ forward_maxpool_layer_gpu(maxpool_layer l, ಜಾಲ net);
ಶೂನ್ಯ backward_maxpool_layer_gpu(maxpool_layer l, ಜಾಲ net);
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ

