#ifndef DROPOUT_LAYER_H
#define DROPOUT_LAYER_H

#ಅನ್ತರ್ಭಾವ "layer.h"
#ಅನ್ತರ್ಭಾವ "network.h"

ಪ್ರಕಾರನಿರ್ದೇಶ layer dropout_layer;

dropout_layer make_dropout_layer(ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ inputs, ಭಿನ್ನ probability);

ಶೂನ್ಯ forward_dropout_layer(dropout_layer l, network net);
ಶೂನ್ಯ backward_dropout_layer(dropout_layer l, network net);
ಶೂನ್ಯ resize_dropout_layer(dropout_layer *l, ಪೂರ್ಣಾಙ್ಕ inputs);

#ifdef GPU
ಶೂನ್ಯ forward_dropout_layer_gpu(dropout_layer l, network net);
ಶೂನ್ಯ backward_dropout_layer_gpu(dropout_layer l, network net);

#endif
#endif
