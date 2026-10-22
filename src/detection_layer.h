#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ DETECTION_LAYER_H
#ವ್ಯಾಖ್ಯಾನಿಸು DETECTION_LAYER_H

#ಅನ್ತರ್ಭಾವ "layer.h"
#ಅನ್ತರ್ಭಾವ "network.h"

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ layer detection_layer;

detection_layer make_detection_layer(ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ inputs, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2, ಪೂರ್ಣಾಂಕ classes, ಪೂರ್ಣಾಂಕ coords, ಪೂರ್ಣಾಂಕ rescore);
ಶೂನ್ಯ forward_detection_layer(ಸ್ಥಿರಾಂಕ detection_layer l, ಜಾಲ net);
ಶೂನ್ಯ backward_detection_layer(ಸ್ಥಿರಾಂಕ detection_layer l, ಜಾಲ net);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
ಶೂನ್ಯ forward_detection_layer_gpu(ಸ್ಥಿರಾಂಕ detection_layer l, ಜಾಲ net);
ಶೂನ್ಯ backward_detection_layer_gpu(detection_layer l, ಜಾಲ net);
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ
