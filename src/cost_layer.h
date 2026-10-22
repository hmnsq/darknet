#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ವೆಚ್ಚ_LAYER_H
#ವ್ಯಾಖ್ಯಾನಿಸು ವೆಚ್ಚ_LAYER_H
#ಅನ್ತರ್ಭಾವ "layer.h"
#ಅನ್ತರ್ಭಾವ "network.h"

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ layer ವೆಚ್ಚ_layer;

ವೆಚ್ಚ_TYPE get_ವೆಚ್ಚ_type(ಅಕ್ಷರ *s);
ಅಕ್ಷರ *get_ವೆಚ್ಚ_string(ವೆಚ್ಚ_TYPE a);
ವೆಚ್ಚ_layer make_ವೆಚ್ಚ_layer(ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ inputs, ವೆಚ್ಚ_TYPE type, ದಶಮಾಂಶ scale);
ಶೂನ್ಯ forward_ವೆಚ್ಚ_layer(ಸ್ಥಿರಾಂಕ ವೆಚ್ಚ_layer l, ಜಾಲ net);
ಶೂನ್ಯ backward_ವೆಚ್ಚ_layer(ಸ್ಥಿರಾಂಕ ವೆಚ್ಚ_layer l, ಜಾಲ net);
ಶೂನ್ಯ resize_ವೆಚ್ಚ_layer(ವೆಚ್ಚ_layer *l, ಪೂರ್ಣಾಂಕ inputs);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
ಶೂನ್ಯ forward_ವೆಚ್ಚ_layer_gpu(ವೆಚ್ಚ_layer l, ಜಾಲ net);
ಶೂನ್ಯ backward_ವೆಚ್ಚ_layer_gpu(ಸ್ಥಿರಾಂಕ ವೆಚ್ಚ_layer l, ಜಾಲ net);
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ
