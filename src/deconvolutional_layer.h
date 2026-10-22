#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ DECONVOLUTIONAL_LAYER_H
#ವ್ಯಾಖ್ಯಾನಿಸು DECONVOLUTIONAL_LAYER_H

#ಅನ್ತರ್ಭಾವ "cuda.h"
#ಅನ್ತರ್ಭಾವ "image.h"
#ಅನ್ತರ್ಭಾವ "activations.h"
#ಅನ್ತರ್ಭಾವ "layer.h"
#ಅನ್ತರ್ಭಾವ "network.h"

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
ಶೂನ್ಯ forward_deconvolutional_layer_gpu(layer l, ಜಾಲ net);
ಶೂನ್ಯ backward_deconvolutional_layer_gpu(layer l, ಜಾಲ net);
ಶೂನ್ಯ update_deconvolutional_layer_gpu(layer l, update_ವಾದಗಳು a);
ಶೂನ್ಯ ತಳ್ಳು_deconvolutional_layer(layer l);
ಶೂನ್ಯ pull_deconvolutional_layer(layer l);
#ಆದರೆ_ಅಂತ್ಯ

layer make_deconvolutional_layer(ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ h, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ c, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2, ಪೂರ್ಣಾಂಕ stride, ಪೂರ್ಣಾಂಕ padding, ACTIVATION activation, ಪೂರ್ಣಾಂಕ batch_normalize, ಪೂರ್ಣಾಂಕ adam);
ಶೂನ್ಯ resize_deconvolutional_layer(layer *l, ಪೂರ್ಣಾಂಕ h, ಪೂರ್ಣಾಂಕ w);
ಶೂನ್ಯ forward_deconvolutional_layer(ಸ್ಥಿರಾಂಕ layer l, ಜಾಲ net);
ಶೂನ್ಯ update_deconvolutional_layer(layer l, update_ವಾದಗಳು a);
ಶೂನ್ಯ backward_deconvolutional_layer(layer l, ಜಾಲ net);

#ಆದರೆ_ಅಂತ್ಯ

