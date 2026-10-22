#ifndef UPSAMPLE_LAYER_H
#define UPSAMPLE_LAYER_H
#ಅನ್ತರ್ಭಾವ "darknet.h"

layer make_upsample_layer(ಪೂರ್ಣಾಙ್ಕ batch, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h, ಪೂರ್ಣಾಙ್ಕ c, ಪೂರ್ಣಾಙ್ಕ stride);
ಶೂನ್ಯ forward_upsample_layer(ಧ್ರುವ layer l, network net);
ಶೂನ್ಯ backward_upsample_layer(ಧ್ರುವ layer l, network net);
ಶೂನ್ಯ resize_upsample_layer(layer *l, ಪೂರ್ಣಾಙ್ಕ w, ಪೂರ್ಣಾಙ್ಕ h);

#ifdef GPU
ಶೂನ್ಯ forward_upsample_layer_gpu(ಧ್ರುವ layer l, network net);
ಶೂನ್ಯ backward_upsample_layer_gpu(ಧ್ರುವ layer l, network net);
#endif

#endif
