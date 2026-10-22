#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ LOGISTIC_LAYER_H
#ವ್ಯಾಖ್ಯಾನಿಸು LOGISTIC_LAYER_H
#ಅನ್ತರ್ಭಾವ "layer.h"
#ಅನ್ತರ್ಭಾವ "network.h"

layer make_logistic_layer(ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ inputs);
ಶೂನ್ಯ forward_logistic_layer(ಸ್ಥಿರಾಂಕ layer l, ಜಾಲ net);
ಶೂನ್ಯ backward_logistic_layer(ಸ್ಥಿರಾಂಕ layer l, ಜಾಲ net);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
ಶೂನ್ಯ forward_logistic_layer_gpu(ಸ್ಥಿರಾಂಕ layer l, ಜಾಲ net);
ಶೂನ್ಯ backward_logistic_layer_gpu(ಸ್ಥಿರಾಂಕ layer l, ಜಾಲ net);
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ
