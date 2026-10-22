#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ DROPOUT_LAYER_H
#ವ್ಯಾಖ್ಯಾನಿಸು DROPOUT_LAYER_H

#ಅನ್ತರ್ಭಾವ "layer.h"
#ಅನ್ತರ್ಭಾವ "network.h"

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ layer dropout_layer;

dropout_layer make_dropout_layer(ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ inputs, ದಶಮಾಂಶ probability);

ಶೂನ್ಯ forward_dropout_layer(dropout_layer l, ಜಾಲ net);
ಶೂನ್ಯ backward_dropout_layer(dropout_layer l, ಜಾಲ net);
ಶೂನ್ಯ resize_dropout_layer(dropout_layer *l, ಪೂರ್ಣಾಂಕ inputs);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
ಶೂನ್ಯ forward_dropout_layer_gpu(dropout_layer l, ಜಾಲ net);
ಶೂನ್ಯ backward_dropout_layer_gpu(dropout_layer l, ಜಾಲ net);

#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ
