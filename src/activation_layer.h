#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ACTIVATION_LAYER_H
#ವ್ಯಾಖ್ಯಾನಿಸು ACTIVATION_LAYER_H

#ಅನ್ತರ್ಭಾವ "activations.h"
#ಅನ್ತರ್ಭಾವ "layer.h"
#ಅನ್ತರ್ಭಾವ "network.h"

layer make_activation_layer(ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ inputs, ACTIVATION activation);

ಶೂನ್ಯ forward_activation_layer(layer l, ಜಾಲ net);
ಶೂನ್ಯ backward_activation_layer(layer l, ಜಾಲ net);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
ಶೂನ್ಯ forward_activation_layer_gpu(layer l, ಜಾಲ net);
ಶೂನ್ಯ backward_activation_layer_gpu(layer l, ಜಾಲ net);
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ

