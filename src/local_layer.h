#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ LOCAL_LAYER_H
#ವ್ಯಾಖ್ಯಾನಿಸು LOCAL_LAYER_H

#ಅನ್ತರ್ಭಾವ "cuda.h"
#ಅನ್ತರ್ಭಾವ "image.h"
#ಅನ್ತರ್ಭಾವ "activations.h"
#ಅನ್ತರ್ಭಾವ "layer.h"
#ಅನ್ತರ್ಭಾವ "network.h"

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ layer local_layer;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
ಶೂನ್ಯ forward_local_layer_gpu(local_layer layer, ಜಾಲ net);
ಶೂನ್ಯ backward_local_layer_gpu(local_layer layer, ಜಾಲ net);
ಶೂನ್ಯ update_local_layer_gpu(local_layer layer, update_ವಾದಗಳು a);

ಶೂನ್ಯ ತಳ್ಳು_local_layer(local_layer layer);
ಶೂನ್ಯ pull_local_layer(local_layer layer);
#ಆದರೆ_ಅಂತ್ಯ

local_layer make_local_layer(ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ h, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ c, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2, ಪೂರ್ಣಾಂಕ stride, ಪೂರ್ಣಾಂಕ pad, ACTIVATION activation);

ಶೂನ್ಯ forward_local_layer(ಸ್ಥಿರಾಂಕ local_layer layer, ಜಾಲ net);
ಶೂನ್ಯ backward_local_layer(local_layer layer, ಜಾಲ net);
ಶೂನ್ಯ update_local_layer(local_layer layer, update_ವಾದಗಳು a);

ಶೂನ್ಯ bias_output(ದಶಮಾಂಶ *output, ದಶಮಾಂಶ *biases, ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2);
ಶೂನ್ಯ backward_bias(ದಶಮಾಂಶ *bias_updates, ದಶಮಾಂಶ *delta, ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2);

#ಆದರೆ_ಅಂತ್ಯ

