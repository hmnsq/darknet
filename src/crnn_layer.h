
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CRNN_LAYER_H
#ವ್ಯಾಖ್ಯಾನಿಸು CRNN_LAYER_H

#ಅನ್ತರ್ಭಾವ "activations.h"
#ಅನ್ತರ್ಭಾವ "layer.h"
#ಅನ್ತರ್ಭಾವ "network.h"

layer make_crnn_layer(ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ h, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ c, ಪೂರ್ಣಾಂಕ hidden_filters, ಪೂರ್ಣಾಂಕ output_filters, ಪೂರ್ಣಾಂಕ ಹೆಜ್ಜೆಗಳು, ACTIVATION activation, ಪೂರ್ಣಾಂಕ batch_normalize);

ಶೂನ್ಯ forward_crnn_layer(layer l, ಜಾಲ net);
ಶೂನ್ಯ backward_crnn_layer(layer l, ಜಾಲ net);
ಶೂನ್ಯ update_crnn_layer(layer l, update_ವಾದಗಳು a);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
ಶೂನ್ಯ forward_crnn_layer_gpu(layer l, ಜಾಲ net);
ಶೂನ್ಯ backward_crnn_layer_gpu(layer l, ಜಾಲ net);
ಶೂನ್ಯ update_crnn_layer_gpu(layer l, update_ವಾದಗಳು a);
ಶೂನ್ಯ ತಳ್ಳು_crnn_layer(layer l);
ಶೂನ್ಯ pull_crnn_layer(layer l);
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ

