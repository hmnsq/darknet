#ifndef ACTIVATION_LAYER_H
#define ACTIVATION_LAYER_H

#ಅನ್ತರ್ಭಾವ "activations.h"
#ಅನ್ತರ್ಭಾವ "layer.h"
#ಅನ್ತರ್ಭಾವ "network.h"

layer make_activation_layer(ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ inputs, ACTIVATION activation);

ಶೂನ್ಯ forward_activation_layer(layer l, network net);
ಶೂನ್ಯ backward_activation_layer(layer l, network net);

#ifdef GPU
ಶೂನ್ಯ forward_activation_layer_gpu(layer l, network net);
ಶೂನ್ಯ backward_activation_layer_gpu(layer l, network net);
#endif

#endif

