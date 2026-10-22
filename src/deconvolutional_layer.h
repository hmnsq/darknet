#ifndef DECONVOLUTIONAL_LAYER_H
#define DECONVOLUTIONAL_LAYER_H

#ಅನ್ತರ್ಭಾವ "cuda.h"
#ಅನ್ತರ್ಭಾವ "image.h"
#ಅನ್ತರ್ಭಾವ "activations.h"
#ಅನ್ತರ್ಭಾವ "layer.h"
#ಅನ್ತರ್ಭಾವ "network.h"

#ifdef GPU
ಶೂನ್ಯ forward_deconvolutional_layer_gpu(layer l, network net);
ಶೂನ್ಯ backward_deconvolutional_layer_gpu(layer l, network net);
ಶೂನ್ಯ update_deconvolutional_layer_gpu(layer l, update_args a);
ಶೂನ್ಯ ತಳ್ಳು_deconvolutional_layer(layer l);
ಶೂನ್ಯ pull_deconvolutional_layer(layer l);
#endif

layer make_deconvolutional_layer(ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ h, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ c, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ size, ಪೂರ್ಣಾಙ್ಕ stride, ಪೂರ್ಣಾಙ್ಕ padding, ACTIVATION activation, ಪೂರ್ಣಾಙ್ಕ batch_normalize, ಪೂರ್ಣಾಙ್ಕ adam);
ಶೂನ್ಯ resize_deconvolutional_layer(layer *l, ಪೂರ್ಣಾಙ್ಕ h, ಪೂರ್ಣಾಙ್ಕ w);
ಶೂನ್ಯ forward_deconvolutional_layer(ಧ್ರುವ layer l, network net);
ಶೂನ್ಯ update_deconvolutional_layer(layer l, update_args a);
ಶೂನ್ಯ backward_deconvolutional_layer(layer l, network net);

#endif

