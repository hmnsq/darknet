#ifndef CONVOLUTIONAL_LAYER_H
#define CONVOLUTIONAL_LAYER_H

#ಅನ್ತರ್ಭಾವ "cuda.h"
#ಅನ್ತರ್ಭಾವ "image.h"
#ಅನ್ತರ್ಭಾವ "activations.h"
#ಅನ್ತರ್ಭಾವ "layer.h"
#ಅನ್ತರ್ಭಾವ "network.h"

ಪ್ರಕಾರನಿರ್ದೇಶ layer convolutional_layer;

#ifdef GPU
ಶೂನ್ಯ forward_convolutional_layer_gpu(convolutional_layer layer, network net);
ಶೂನ್ಯ backward_convolutional_layer_gpu(convolutional_layer layer, network net);
ಶೂನ್ಯ update_convolutional_layer_gpu(convolutional_layer layer, update_args a);

ಶೂನ್ಯ ತಳ್ಳು_convolutional_layer(convolutional_layer layer);
ಶೂನ್ಯ pull_convolutional_layer(convolutional_layer layer);

ಶೂನ್ಯ add_bias_gpu(ಭಿನ್ನ *output, ಭಿನ್ನ *biases, ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ size);
ಶೂನ್ಯ backward_bias_gpu(ಭಿನ್ನ *bias_updates, ಭಿನ್ನ *delta, ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ size);
ಶೂನ್ಯ adam_update_gpu(ಭಿನ್ನ *w, ಭಿನ್ನ *d, ಭಿನ್ನ *m, ಭಿನ್ನ *v, ಭಿನ್ನ B1, ಭಿನ್ನ B2, ಭಿನ್ನ eps, ಭಿನ್ನ decay, ಭಿನ್ನ rate, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ t);
#ifdef CUDNN
ಶೂನ್ಯ cudnn_convolutional_setup(layer *l);
#endif
#endif

convolutional_layer make_convolutional_layer(ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ h, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ c, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ groups, ಪೂರ್ಣಾಙ್ಕ size, ಪೂರ್ಣಾಙ್ಕ stride, ಪೂರ್ಣಾಙ್ಕ padding, ACTIVATION activation, ಪೂರ್ಣಾಙ್ಕ batch_normalize, ಪೂರ್ಣಾಙ್ಕ ದ್ವಿಮಾನ, ಪೂರ್ಣಾಙ್ಕ xnor, ಪೂರ್ಣಾಙ್ಕ adam);
ಶೂನ್ಯ resize_convolutional_layer(convolutional_layer *layer, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h);
ಶೂನ್ಯ forward_convolutional_layer(ಧ್ರುವ convolutional_layer layer, network net);
ಶೂನ್ಯ update_convolutional_layer(convolutional_layer layer, update_args a);
image *visualize_convolutional_layer(convolutional_layer layer, ವರ್ಣ *window, image *prev_weights);
ಶೂನ್ಯ binarize_weights(ಭಿನ್ನ *weights, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ size, ಭಿನ್ನ *ದ್ವಿಮಾನ);
ಶೂನ್ಯ swap_ದ್ವಿಮಾನ(convolutional_layer *l);
ಶೂನ್ಯ binarize_weights2(ಭಿನ್ನ *weights, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ size, ವರ್ಣ *ದ್ವಿಮಾನ, ಭಿನ್ನ *scales);

ಶೂನ್ಯ backward_convolutional_layer(convolutional_layer layer, network net);

ಶೂನ್ಯ add_bias(ಭಿನ್ನ *output, ಭಿನ್ನ *biases, ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ size);
ಶೂನ್ಯ backward_bias(ಭಿನ್ನ *bias_updates, ಭಿನ್ನ *delta, ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ size);

image get_convolutional_image(convolutional_layer layer);
image get_convolutional_delta(convolutional_layer layer);
image get_convolutional_weight(convolutional_layer layer, ಪೂರ್ಣಾಙ್ಕ i);

ಪೂರ್ಣಾಙ್ಕ convolutional_out_height(convolutional_layer layer);
ಪೂರ್ಣಾಙ್ಕ convolutional_out_width(convolutional_layer layer);

#endif

