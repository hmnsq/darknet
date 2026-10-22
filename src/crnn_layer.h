
#ifndef CRNN_LAYER_H
#define CRNN_LAYER_H

#ಅನ್ತರ್ಭಾವ "activations.h"
#ಅನ್ತರ್ಭಾವ "layer.h"
#ಅನ್ತರ್ಭಾವ "network.h"

layer make_crnn_layer(ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ h, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ c, ಪೂರ್ಣಾಙ್ಕ hidden_filters, ಪೂರ್ಣಾಙ್ಕ output_filters, ಪೂರ್ಣಾಙ್ಕ steps, ACTIVATION activation, ಪೂರ್ಣಾಙ್ಕ batch_normalize);

ಶೂನ್ಯ forward_crnn_layer(layer l, network net);
ಶೂನ್ಯ backward_crnn_layer(layer l, network net);
ಶೂನ್ಯ update_crnn_layer(layer l, update_args a);

#ifdef GPU
ಶೂನ್ಯ forward_crnn_layer_gpu(layer l, network net);
ಶೂನ್ಯ backward_crnn_layer_gpu(layer l, network net);
ಶೂನ್ಯ update_crnn_layer_gpu(layer l, update_args a);
ಶೂನ್ಯ ತಳ್ಳು_crnn_layer(layer l);
ಶೂನ್ಯ pull_crnn_layer(layer l);
#endif

#endif

