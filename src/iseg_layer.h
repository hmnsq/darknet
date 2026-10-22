#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ISEG_LAYER_H
#ವ್ಯಾಖ್ಯಾನಿಸು ISEG_LAYER_H

#ಅನ್ತರ್ಭಾವ "darknet.h"
#ಅನ್ತರ್ಭಾವ "layer.h"
#ಅನ್ತರ್ಭಾವ "network.h"

layer make_iseg_layer(ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h, ಪೂರ್ಣಾಂಕ classes, ಪೂರ್ಣಾಂಕ ids);
ಶೂನ್ಯ forward_iseg_layer(ಸ್ಥಿರಾಂಕ layer l, ಜಾಲ net);
ಶೂನ್ಯ backward_iseg_layer(ಸ್ಥಿರಾಂಕ layer l, ಜಾಲ net);
ಶೂನ್ಯ resize_iseg_layer(layer *l, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h);
ಪೂರ್ಣಾಂಕ iseg_ಅಂಕೆ_detections(layer l, ದಶಮಾಂಶ thresh);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
ಶೂನ್ಯ forward_iseg_layer_gpu(ಸ್ಥಿರಾಂಕ layer l, ಜಾಲ net);
ಶೂನ್ಯ backward_iseg_layer_gpu(layer l, ಜಾಲ net);
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ
