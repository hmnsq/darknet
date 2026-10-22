#ifndef REGION_LAYER_H
#define REGION_LAYER_H

#ಅನ್ತರ್ಭಾವ "darknet.h"
#ಅನ್ತರ್ಭಾವ "layer.h"
#ಅನ್ತರ್ಭಾವ "network.h"

layer make_region_layer(ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h, ಪೂರ್ಣಾಙ್ಕ n, ಪೂರ್ಣಾಙ್ಕ classes, ಪೂರ್ಣಾಙ್ಕ coords);
ಶೂನ್ಯ forward_region_layer(ಧ್ರುವ layer l, network net);
ಶೂನ್ಯ backward_region_layer(ಧ್ರುವ layer l, network net);
ಶೂನ್ಯ resize_region_layer(layer *l, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h);

#ifdef GPU
ಶೂನ್ಯ forward_region_layer_gpu(ಧ್ರುವ layer l, network net);
ಶೂನ್ಯ backward_region_layer_gpu(layer l, network net);
#endif

#endif
