#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ SOFTMAX_LAYER_H
#ವ್ಯಾಖ್ಯಾನಿಸು SOFTMAX_LAYER_H
#ಅನ್ತರ್ಭಾವ "layer.h"
#ಅನ್ತರ್ಭಾವ "network.h"

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ layer softmax_layer;

ಶೂನ್ಯ softmax_array(ದಶಮಾಂಶ *ಒಳಹರಿವು, ಪೂರ್ಣಾಂಕ n, ದಶಮಾಂಶ ತಾತ್ಕಾಲಿಕ, ದಶಮಾಂಶ *output);
softmax_layer make_softmax_layer(ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ inputs, ಪೂರ್ಣಾಂಕ groups);
ಶೂನ್ಯ forward_softmax_layer(ಸ್ಥಿರಾಂಕ softmax_layer l, ಜಾಲ net);
ಶೂನ್ಯ backward_softmax_layer(ಸ್ಥಿರಾಂಕ softmax_layer l, ಜಾಲ net);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
ಶೂನ್ಯ pull_softmax_layer_output(ಸ್ಥಿರಾಂಕ softmax_layer l);
ಶೂನ್ಯ forward_softmax_layer_gpu(ಸ್ಥಿರಾಂಕ softmax_layer l, ಜಾಲ net);
ಶೂನ್ಯ backward_softmax_layer_gpu(ಸ್ಥಿರಾಂಕ softmax_layer l, ಜಾಲ net);
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ_ಅಂತ್ಯ
