#ifndef ISEG_LAYER_H
#define ISEG_LAYER_H

#ಅನ್ತರ್ಭಾವ "darknet.h"
#ಅನ್ತರ್ಭಾವ "layer.h"
#ಅನ್ತರ್ಭಾವ "network.h"

layer make_iseg_layer(ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h, ಪೂರ್ಣಾಙ್ಕ classes, ಪೂರ್ಣಾಙ್ಕ ids);
ಶೂನ್ಯ forward_iseg_layer(ಧ್ರುವ layer l, network net);
ಶೂನ್ಯ backward_iseg_layer(ಧ್ರುವ layer l, network net);
ಶೂನ್ಯ resize_iseg_layer(layer *l, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h);
ಪೂರ್ಣಾಙ್ಕ iseg_ಅಂಕೆ_detections(layer l, ಭಿನ್ನ thresh);

#ifdef GPU
ಶೂನ್ಯ forward_iseg_layer_gpu(ಧ್ರುವ layer l, network net);
ಶೂನ್ಯ backward_iseg_layer_gpu(layer l, network net);
#endif

#endif
