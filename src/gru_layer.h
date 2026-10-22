
#ifndef GRU_LAYER_H
#define GRU_LAYER_H

#ಅನ್ತರ್ಭಾವ "activations.h"
#ಅನ್ತರ್ಭಾವ "layer.h"
#ಅನ್ತರ್ಭಾವ "network.h"

layer make_gru_layer(ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ inputs, ಪೂರ್ಣಾಙ್ಕ outputs, ಪೂರ್ಣಾಙ್ಕ steps, ಪೂರ್ಣಾಙ್ಕ batch_normalize, ಪೂರ್ಣಾಙ್ಕ adam);

ಶೂನ್ಯ forward_gru_layer(layer l, network state);
ಶೂನ್ಯ backward_gru_layer(layer l, network state);
ಶೂನ್ಯ update_gru_layer(layer l, update_args a);

#ifdef GPU
ಶೂನ್ಯ forward_gru_layer_gpu(layer l, network state);
ಶೂನ್ಯ backward_gru_layer_gpu(layer l, network state);
ಶೂನ್ಯ update_gru_layer_gpu(layer l, update_args a);
ಶೂನ್ಯ ತಳ್ಳು_gru_layer(layer l);
ಶೂನ್ಯ pull_gru_layer(layer l);
#endif

#endif

