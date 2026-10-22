#ifndef L2NORM_LAYER_H
#define L2NORM_LAYER_H
#ಅನ್ತರ್ಭಾವ "layer.h"
#ಅನ್ತರ್ಭಾವ "network.h"

layer make_l2norm_layer(ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ inputs);
ಶೂನ್ಯ forward_l2norm_layer(ಧ್ರುವ layer l, network net);
ಶೂನ್ಯ backward_l2norm_layer(ಧ್ರುವ layer l, network net);

#ifdef GPU
ಶೂನ್ಯ forward_l2norm_layer_gpu(ಧ್ರುವ layer l, network net);
ಶೂನ್ಯ backward_l2norm_layer_gpu(ಧ್ರುವ layer l, network net);
#endif

#endif
