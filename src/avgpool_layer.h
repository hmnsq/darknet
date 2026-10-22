#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ AVGPOOL_LAYER_H
#ವ್ಯಾಖ್ಯಾನಿಸು AVGPOOL_LAYER_H

#ಅನ್ತರ್ಭಾವ "image.h"
#ಅನ್ತರ್ಭಾವ "cuda.h"
#ಅನ್ತರ್ಭಾವ "layer.h"
#ಅನ್ತರ್ಭಾವ "network.h"

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ layer avgpool_layer;

image get_avgpool_image(avgpool_layer l);
avgpool_layer make_avgpool_layer(ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h, ಪೂರ್ಣಾಂಕ c);
ಶೂನ್ಯ resize_avgpool_layer(avgpool_layer *l, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h);
ಶೂನ್ಯ forward_avgpool_layer(ಸ್ಥಿರಾಂಕ avgpool_layer l, ಜಾಲ net);
ಶೂನ್ಯ backward_avgpool_layer(ಸ್ಥಿರಾಂಕ avgpool_layer l, ಜಾಲ net);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
ಶೂನ್ಯ forward_avgpool_layer_gpu(avgpool_layer l, ಜಾಲ net);
ಶೂನ್ಯ backward_avgpool_layer_gpu(avgpool_layer l, ಜಾಲ net);
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ

