#ifndef CONNECTED_LAYER_H
#define CONNECTED_LAYER_H

#ಅನ್ತರ್ಭಾವ "activations.h"
#ಅನ್ತರ್ಭಾವ "layer.h"
#ಅನ್ತರ್ಭಾವ "network.h"

layer make_connected_layer(ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ inputs, ಪೂರ್ಣಾಙ್ಕ outputs, ACTIVATION activation, ಪೂರ್ಣಾಙ್ಕ batch_normalize, ಪೂರ್ಣಾಙ್ಕ adam);

ಶೂನ್ಯ forward_connected_layer(layer l, network net);
ಶೂನ್ಯ backward_connected_layer(layer l, network net);
ಶೂನ್ಯ update_connected_layer(layer l, update_args a);

#ifdef GPU
ಶೂನ್ಯ forward_connected_layer_gpu(layer l, network net);
ಶೂನ್ಯ backward_connected_layer_gpu(layer l, network net);
ಶೂನ್ಯ update_connected_layer_gpu(layer l, update_args a);
ಶೂನ್ಯ ತಳ್ಳು_connected_layer(layer l);
ಶೂನ್ಯ pull_connected_layer(layer l);
#endif

#endif

