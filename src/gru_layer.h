
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ GRU_LAYER_H
#ವ್ಯಾಖ್ಯಾನಿಸು GRU_LAYER_H

#ಅನ್ತರ್ಭಾವ "activations.h"
#ಅನ್ತರ್ಭಾವ "layer.h"
#ಅನ್ತರ್ಭಾವ "network.h"

layer make_gru_layer(ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ inputs, ಪೂರ್ಣಾಂಕ outputs, ಪೂರ್ಣಾಂಕ ಹೆಜ್ಜೆಗಳು, ಪೂರ್ಣಾಂಕ batch_normalize, ಪೂರ್ಣಾಂಕ adam);

ಶೂನ್ಯ forward_gru_layer(layer l, ಜಾಲ state);
ಶೂನ್ಯ backward_gru_layer(layer l, ಜಾಲ state);
ಶೂನ್ಯ update_gru_layer(layer l, update_ವಾದಗಳು a);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
ಶೂನ್ಯ forward_gru_layer_gpu(layer l, ಜಾಲ state);
ಶೂನ್ಯ backward_gru_layer_gpu(layer l, ಜಾಲ state);
ಶೂನ್ಯ update_gru_layer_gpu(layer l, update_ವಾದಗಳು a);
ಶೂನ್ಯ ತಳ್ಳು_gru_layer(layer l);
ಶೂನ್ಯ pull_gru_layer(layer l);
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ

