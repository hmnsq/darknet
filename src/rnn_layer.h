
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ RNN_LAYER_H
#ವ್ಯಾಖ್ಯಾನಿಸು RNN_LAYER_H

#ಅನ್ತರ್ಭಾವ "activations.h"
#ಅನ್ತರ್ಭಾವ "layer.h"
#ಅನ್ತರ್ಭಾವ "network.h"
#ವ್ಯಾಖ್ಯಾನಿಸು USET

layer make_rnn_layer(ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ inputs, ಪೂರ್ಣಾಂಕ outputs, ಪೂರ್ಣಾಂಕ ಹೆಜ್ಜೆಗಳು, ACTIVATION activation, ಪೂರ್ಣಾಂಕ batch_normalize, ಪೂರ್ಣಾಂಕ adam);

ಶೂನ್ಯ forward_rnn_layer(layer l, ಜಾಲ net);
ಶೂನ್ಯ backward_rnn_layer(layer l, ಜಾಲ net);
ಶೂನ್ಯ update_rnn_layer(layer l, update_ವಾದಗಳು a);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
ಶೂನ್ಯ forward_rnn_layer_gpu(layer l, ಜಾಲ net);
ಶೂನ್ಯ backward_rnn_layer_gpu(layer l, ಜಾಲ net);
ಶೂನ್ಯ update_rnn_layer_gpu(layer l, update_ವಾದಗಳು a);
ಶೂನ್ಯ ತಳ್ಳು_rnn_layer(layer l);
ಶೂನ್ಯ pull_rnn_layer(layer l);
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ

