#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CONVOLUTIONAL_LAYER_H
#ವ್ಯಾಖ್ಯಾನಿಸು CONVOLUTIONAL_LAYER_H

#ಅನ್ತರ್ಭಾವ "cuda.h"
#ಅನ್ತರ್ಭಾವ "image.h"
#ಅನ್ತರ್ಭಾವ "activations.h"
#ಅನ್ತರ್ಭಾವ "layer.h"
#ಅನ್ತರ್ಭಾವ "network.h"

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ layer convolutional_layer;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ GPU
ಶೂನ್ಯ forward_convolutional_layer_gpu(convolutional_layer layer, ಜಾಲ net);
ಶೂನ್ಯ backward_convolutional_layer_gpu(convolutional_layer layer, ಜಾಲ net);
ಶೂನ್ಯ update_convolutional_layer_gpu(convolutional_layer layer, update_ವಾದಗಳು a);

ಶೂನ್ಯ ತಳ್ಳು_convolutional_layer(convolutional_layer layer);
ಶೂನ್ಯ pull_convolutional_layer(convolutional_layer layer);

ಶೂನ್ಯ ಕೂಡಿಸು_bias_gpu(ದಶಮಾಂಶ *output, ದಶಮಾಂಶ *biases, ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2);
ಶೂನ್ಯ backward_bias_gpu(ದಶಮಾಂಶ *bias_updates, ದಶಮಾಂಶ *delta, ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2);
ಶೂನ್ಯ adam_update_gpu(ದಶಮಾಂಶ *w, ದಶಮಾಂಶ *d, ದಶಮಾಂಶ *m, ದಶಮಾಂಶ *v, ದಶಮಾಂಶ B1, ದಶಮಾಂಶ B2, ದಶಮಾಂಶ eps, ದಶಮಾಂಶ decay, ದಶಮಾಂಶ ದರ, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ t);
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ CUDNN
ಶೂನ್ಯ cudnn_convolutional_ಸಿದ್ಧತೆ(layer *l);
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

convolutional_layer make_convolutional_layer(ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ h, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ c, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ groups, ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2, ಪೂರ್ಣಾಂಕ stride, ಪೂರ್ಣಾಂಕ padding, ACTIVATION activation, ಪೂರ್ಣಾಂಕ batch_normalize, ಪೂರ್ಣಾಂಕ ದ್ವಿಮಾನ, ಪೂರ್ಣಾಂಕ xnor, ಪೂರ್ಣಾಂಕ adam);
ಶೂನ್ಯ resize_convolutional_layer(convolutional_layer *layer, ಪೂರ್ಣಾಂಕ w, ಪೂರ್ಣಾಂಕ h);
ಶೂನ್ಯ forward_convolutional_layer(ಸ್ಥಿರಾಂಕ convolutional_layer layer, ಜಾಲ net);
ಶೂನ್ಯ update_convolutional_layer(convolutional_layer layer, update_ವಾದಗಳು a);
image *visualize_convolutional_layer(convolutional_layer layer, ಅಕ್ಷರ *ಕಿಟಕಿ, image *prev_weights);
ಶೂನ್ಯ binarize_weights(ದಶಮಾಂಶ *weights, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2, ದಶಮಾಂಶ *ದ್ವಿಮಾನ);
ಶೂನ್ಯ ಅದಲುಬದಲು_ದ್ವಿಮಾನ(convolutional_layer *l);
ಶೂನ್ಯ binarize_weights2(ದಶಮಾಂಶ *weights, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2, ಅಕ್ಷರ *ದ್ವಿಮಾನ, ದಶಮಾಂಶ *scales);

ಶೂನ್ಯ backward_convolutional_layer(convolutional_layer layer, ಜಾಲ net);

ಶೂನ್ಯ ಕೂಡಿಸು_bias(ದಶಮಾಂಶ *output, ದಶಮಾಂಶ *biases, ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2);
ಶೂನ್ಯ backward_bias(ದಶಮಾಂಶ *bias_updates, ದಶಮಾಂಶ *delta, ಪೂರ್ಣಾಂಕ batch, ಪೂರ್ಣಾಂಕ n, ಪೂರ್ಣಾಂಕ ಗಾತ್ರ_2);

image get_convolutional_image(convolutional_layer layer);
image get_convolutional_delta(convolutional_layer layer);
image get_convolutional_ತೂಕ(convolutional_layer layer, ಪೂರ್ಣಾಂಕ i);

ಪೂರ್ಣಾಂಕ convolutional_ಹೊರಹೋಗು_ಎತ್ತರ(convolutional_layer layer);
ಪೂರ್ಣಾಂಕ convolutional_ಹೊರಹೋಗು_ಅಗಲ(convolutional_layer layer);

#ಆದರೆ_ಅಂತ್ಯ

