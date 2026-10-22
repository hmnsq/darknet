#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ BATCHNORM_LAYER_H
#ವ್ಯಾಖ್ಯಾನಿಸು BATCHNORM_LAYER_H

#ಅನ್ತರ್ಭಾವ "image.h"
#ಅನ್ತರ್ಭಾವ "layer.h"
#ಅನ್ತರ್ಭಾವ "network.h"

layer make_batchnorm_layer(ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h, ಪೂರ್ಣಾಂಕ c);
ಶೂನ್ಯ forward_batchnorm_layer(layer l, ಜಾಲ net);
ಶೂನ್ಯ backward_batchnorm_layer(layer l, ಜಾಲ net);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
ಶೂನ್ಯ forward_batchnorm_layer_gpu(layer l, ಜಾಲ net);
ಶೂನ್ಯ backward_batchnorm_layer_gpu(layer l, ಜಾಲ net);
ಶೂನ್ಯ pull_batchnorm_layer(layer l);
ಶೂನ್ಯ ತಳ್ಳು_batchnorm_layer(layer l);
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ
