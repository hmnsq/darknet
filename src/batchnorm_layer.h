#ifndef BATCHNORM_LAYER_H
#define BATCHNORM_LAYER_H

#ಅನ್ತರ್ಭಾವ "image.h"
#ಅನ್ತರ್ಭಾವ "layer.h"
#ಅನ್ತರ್ಭಾವ "network.h"

layer make_batchnorm_layer(ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h, ಪೂರ್ಣಾಙ್ಕ c);
ಶೂನ್ಯ forward_batchnorm_layer(layer l, network net);
ಶೂನ್ಯ backward_batchnorm_layer(layer l, network net);

#ifdef GPU
ಶೂನ್ಯ forward_batchnorm_layer_gpu(layer l, network net);
ಶೂನ್ಯ backward_batchnorm_layer_gpu(layer l, network net);
ಶೂನ್ಯ pull_batchnorm_layer(layer l);
ಶೂನ್ಯ ತಳ್ಳು_batchnorm_layer(layer l);
#endif

#endif
