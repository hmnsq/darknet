#ifndef LOCAL_LAYER_H
#define LOCAL_LAYER_H

#ಅನ್ತರ್ಭಾವ "cuda.h"
#ಅನ್ತರ್ಭಾವ "image.h"
#ಅನ್ತರ್ಭಾವ "activations.h"
#ಅನ್ತರ್ಭಾವ "layer.h"
#ಅನ್ತರ್ಭಾವ "network.h"

ಪ್ರಕಾರನಿರ್ದೇಶ layer local_layer;

#ifdef GPU
ಶೂನ್ಯ forward_local_layer_gpu(local_layer layer, network net);
ಶೂನ್ಯ backward_local_layer_gpu(local_layer layer, network net);
ಶೂನ್ಯ update_local_layer_gpu(local_layer layer, update_args a);

ಶೂನ್ಯ ತಳ್ಳು_local_layer(local_layer layer);
ಶೂನ್ಯ pull_local_layer(local_layer layer);
#endif

local_layer make_local_layer(ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ h, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ c, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ size, ಪೂರ್ಣಾಙ್ಕ stride, ಪೂರ್ಣಾಙ್ಕ pad, ACTIVATION activation);

ಶೂನ್ಯ forward_local_layer(ಧ್ರುವ local_layer layer, network net);
ಶೂನ್ಯ backward_local_layer(local_layer layer, network net);
ಶೂನ್ಯ update_local_layer(local_layer layer, update_args a);

ಶೂನ್ಯ bias_output(ಭಿನ್ನ *output, ಭಿನ್ನ *biases, ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ size);
ಶೂನ್ಯ backward_bias(ಭಿನ್ನ *bias_updates, ಭಿನ್ನ *delta, ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ size);

#endif

