#ifndef SHORTCUT_LAYER_H
#define SHORTCUT_LAYER_H

#ಅನ್ತರ್ಭಾವ "layer.h"
#ಅನ್ತರ್ಭಾವ "network.h"

layer make_shortcut_layer(ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ index, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h, ಪೂರ್ಣಾಙ್ಕ c, ಪೂರ್ಣಾಙ್ಕ w2, ಪೂರ್ಣಾಙ್ಕ h2, ಪೂರ್ಣಾಙ್ಕ c2);
ಶೂನ್ಯ forward_shortcut_layer(ಧ್ರುವ layer l, network net);
ಶೂನ್ಯ backward_shortcut_layer(ಧ್ರುವ layer l, network net);
ಶೂನ್ಯ resize_shortcut_layer(layer *l, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h);

#ifdef GPU
ಶೂನ್ಯ forward_shortcut_layer_gpu(ಧ್ರುವ layer l, network net);
ಶೂನ್ಯ backward_shortcut_layer_gpu(ಧ್ರುವ layer l, network net);
#endif

#endif
