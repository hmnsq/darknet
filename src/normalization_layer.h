#ifndef NORMALIZATION_LAYER_H
#define NORMALIZATION_LAYER_H

#ಅನ್ತರ್ಭಾವ "image.h"
#ಅನ್ತರ್ಭಾವ "layer.h"
#ಅನ್ತರ್ಭಾವ "network.h"

layer make_normalization_layer(ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h, ಪೂರ್ಣಾಙ್ಕ c, ಪೂರ್ಣಾಙ್ಕ size, ಭಿನ್ನ alpha, ಭಿನ್ನ beta, ಭಿನ್ನ kappa);
ಶೂನ್ಯ resize_normalization_layer(layer *layer, ಪೂರ್ಣಾಙ್ಕ h, ಪೂರ್ಣಾಙ್ಕ w);
ಶೂನ್ಯ forward_normalization_layer(ಧ್ರುವ layer layer, network net);
ಶೂನ್ಯ backward_normalization_layer(ಧ್ರುವ layer layer, network net);
ಶೂನ್ಯ visualize_normalization_layer(layer layer, ವರ್ಣ *window);

#ifdef GPU
ಶೂನ್ಯ forward_normalization_layer_gpu(ಧ್ರುವ layer layer, network net);
ಶೂನ್ಯ backward_normalization_layer_gpu(ಧ್ರುವ layer layer, network net);
#endif

#endif
