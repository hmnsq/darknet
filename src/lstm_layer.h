#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ LSTM_LAYER_H
#ವ್ಯಾಖ್ಯಾನಿಸು LSTM_LAYER_H

#ಅನ್ತರ್ಭಾವ "activations.h"
#ಅನ್ತರ್ಭಾವ "layer.h"
#ಅನ್ತರ್ಭಾವ "network.h"
#ವ್ಯಾಖ್ಯಾನಿಸು USET

layer make_lstm_layer(ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ inputs, ಪೂರ್ಣಾಂಕ outputs, ಪೂರ್ಣಾಂಕ ಹೆಜ್ಜೆಗಳು, ಪೂರ್ಣಾಂಕ batch_normalize, ಪೂರ್ಣಾಂಕ adam);

ಶೂನ್ಯ forward_lstm_layer(layer l, ಜಾಲ net); 
ಶೂನ್ಯ update_lstm_layer(layer l, update_ವಾದಗಳು a);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
ಶೂನ್ಯ forward_lstm_layer_gpu(layer l, ಜಾಲ net);
ಶೂನ್ಯ backward_lstm_layer_gpu(layer l, ಜಾಲ net);
ಶೂನ್ಯ update_lstm_layer_gpu(layer l, update_ವಾದಗಳು a); 

#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ
